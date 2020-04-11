#ifndef MUMBLELINK_H
#define MUMBLELINK_H

#ifdef _WIN32
#include <windows.h>
#endif

#include <QObject>
#include <QPoint>

class MumbleLink : public QObject
{
    Q_OBJECT
public:
    explicit MumbleLink(QObject *parent = nullptr);
    ~MumbleLink();

    void update(const QString &name, const QPointF& userPositionMeters);

private:
    struct LinkedMem {
    #ifdef _WIN32
        UINT32	uiVersion;
        DWORD	uiTick;
    #else
        uint32_t uiVersion;
        uint32_t uiTick;
    #endif
        float	fAvatarPosition[3];
        float	fAvatarFront[3];
        float	fAvatarTop[3];
        wchar_t	name[256];
        float	fCameraPosition[3];
        float	fCameraFront[3];
        float	fCameraTop[3];
        wchar_t	identity[256];
    #ifdef _WIN32
        UINT32	context_len;
    #else
        uint32_t context_len;
    #endif
        unsigned char context[256];
        wchar_t description[2048];
    };

    LinkedMem *lm = nullptr;

signals:

public slots:
};

#endif // MUMBLELINK_H
