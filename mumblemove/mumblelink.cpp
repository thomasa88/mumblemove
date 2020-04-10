#include "mumblelink.h"

#include <stdint.h>

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

    lm = (LinkedMem *)(mmap(nullptr, sizeof(struct LinkedMem), PROT_READ | PROT_WRITE, MAP_SHARED, shmfd,0));

    if (lm == reinterpret_cast<void*>(-1)) {
        lm = nullptr;
        return;
    }
#endif
}

MumbleLink::~MumbleLink()
{

}

void MumbleLink::update() {
    if (lm == nullptr)
        return;

    if(lm->uiVersion != 2) {
        wcsncpy(lm->name, L"TestLink", 256);
        wcsncpy(lm->description, L"TestLink is a test of the Link plugin.", 2048);
        lm->uiVersion = 2;
    }
    lm->uiTick++;

    // Left handed coordinate system.
    // X positive towards "right".
    // Y positive towards "up".
    // Z positive towards "front".
    //
    // 1 unit = 1 meter

    // Unit vector pointing out of the avatar's eyes aka "At"-vector.
    lm->fAvatarFront[0] = 0.0f;
    lm->fAvatarFront[1] = 0.0f;
    lm->fAvatarFront[2] = 1.0f;

    // Unit vector pointing out of the top of the avatar's head aka "Up"-vector (here Top points straight up).
    lm->fAvatarTop[0] = 0.0f;
    lm->fAvatarTop[1] = 1.0f;
    lm->fAvatarTop[2] = 0.0f;

    // Position of the avatar (here standing slightly off the origin)
    lm->fAvatarPosition[0] = 0.001f;
    lm->fAvatarPosition[1] = 0.0f;
    lm->fAvatarPosition[2] = 0.5f;

    // Same as avatar but for the camera.
    lm->fCameraPosition[0] = 0.0f;
    lm->fCameraPosition[1] = 0.0f;
    lm->fCameraPosition[2] = 0.0f;

    lm->fCameraFront[0] = 0.0f;
    lm->fCameraFront[1] = 0.0f;
    lm->fCameraFront[2] = 1.0f;

    lm->fCameraTop[0] = 0.0f;
    lm->fCameraTop[1] = 1.0f;
    lm->fCameraTop[2] = 0.0f;

    // Identifier which uniquely identifies a certain player in a context (e.g. the ingame name).
    wcsncpy(lm->identity, L"Unique ID", 256);
    // Context should be equal for players which should be able to hear each other positional and
    // differ for those who shouldn't (e.g. it could contain the server+port and team)
    memcpy(lm->context, "ContextBlob\x00\x01\x02\x03\x04", 16);
    lm->context_len = 16;
}

