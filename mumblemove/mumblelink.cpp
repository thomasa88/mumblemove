#include "mumblelink.h"

#include <cstdint>
#include <cstring>

// Mumble coordinate system
// https://wiki.mumble.info/wiki/Pluginguide//Explanation_of_Sound_and_Coordinate_Systems
// Person looking into the monitor
//
// y
// ^  z
// | /
// |/
// o------> x
//
// Looking from the top, as on a map. Our character (@) looking to the sky, to get balanced sound.
// Note: Having one ear up (sky) and one down (floor) did not work out well in testing.
//
//    z
//    ^      @ front     pos   = (x, 0, z)
//    | pos. |           front = (0, 1, 0)
//    |  .   v top       top   = (0, 0,-1)
//    |.
//    o------> x
//   /
//  /
// y
//
// Our local drawing, on a computer screen, as an image:
//
// o------> x          Screen vector: (x, y)
// |
// |                   Mumble vector: (x, 0, -y)
// v
// y
//

// Shared memory code from https://wiki.mumble.info/wiki/Link

#ifdef _WIN32
    #include <windows.h>
#else
    #include <sys/mman.h>
    #include <fcntl.h> /* For O_* constants */
    #include <unistd.h>
    #include <sys/types.h>
#endif


MumbleLink::MumbleLink(QObject *parent) {
    Q_UNUSED(parent);
#ifdef _WIN32
    HANDLE hMapObject = OpenFileMappingW(FILE_MAP_ALL_ACCESS, FALSE, L"MumbleLink");
    if (hMapObject == NULL)
        return;

    lm = (LinkedMem *) MapViewOfFile(hMapObject, FILE_MAP_ALL_ACCESS, 0, 0, sizeof(LinkedMem));
    if (lm == NULL) {
        CloseHandle(hMapObject);
        hMapObject = NULL;
        return;
    }
#else
    char memname[256];
    snprintf(memname, 256, "/MumbleLink.%d", getuid());

    int shmfd = shm_open(memname, O_RDWR, S_IRUSR | S_IWUSR);

    if (shmfd < 0) {
        return;
    }

    lm = reinterpret_cast<LinkedMem *>(mmap(nullptr, sizeof(struct LinkedMem), PROT_READ | PROT_WRITE, MAP_SHARED, shmfd,0));

    if (lm == reinterpret_cast<void*>(-1)) {
        lm = nullptr;
        return;
    }
#endif

    initStaticValues();
}

void MumbleLink::initStaticValues() {
    if(lm->uiVersion != 2) {
        wcsncpy(lm->name, L"MumbleMove", 256);
        wcsncpy(lm->description, L"MumbleMove.", 2048);
        lm->uiVersion = 2;
    }

    // Left handed coordinate system.
    // X positive towards "right".
    // Y positive towards "up".
    // Z positive towards "front".
    //
    // 1 unit = 1 meter

    // Unit vector pointing out of the avatar's eyes aka "At"-vector.
    lm->fAvatarFront[0] = 0.0f;
    lm->fAvatarFront[1] = 1.0f;
    lm->fAvatarFront[2] = 0.0f;

    // Unit vector pointing out of the top of the avatar's head aka "Up"-vector.
    lm->fAvatarTop[0] = 0.0f;
    lm->fAvatarTop[1] = 0.0f;
    lm->fAvatarTop[2] = -1.0f;
}

MumbleLink::~MumbleLink() {

}

void MumbleLink::update(const QString &name, const QPointF& userPositionMeters) {
    if (lm == nullptr)
        return;

    lm->uiTick++; // Should this be updated after updating all values???

    // Position of the avatar
    lm->fAvatarPosition[0] = static_cast<float>(userPositionMeters.x());
    lm->fAvatarPosition[1] = 0.0f;
    lm->fAvatarPosition[2] = static_cast<float>(-userPositionMeters.y());

    // Same as avatar but for the camera.
    memcpy(lm->fCameraFront, lm->fAvatarFront, sizeof(lm->fCameraFront));
    memcpy(lm->fCameraTop, lm->fAvatarTop, sizeof(lm->fCameraTop));
    memcpy(lm->fCameraPosition, lm->fAvatarPosition, sizeof(lm->fCameraPosition));

    // Identifier which uniquely identifies a certain player in a context (e.g. the ingame name).
    wcsncpy(lm->identity, name.toStdWString().c_str(), 256);
    // Context should be equal for players which should be able to hear each other positional and
    // differ for those who shouldn't (e.g. it could contain the server+port and team)
    lm->context_len = static_cast<uint32_t>(snprintf(reinterpret_cast<char *>(lm->context),
                                                     sizeof(lm->context), "MumbleMoveContext"));
}

