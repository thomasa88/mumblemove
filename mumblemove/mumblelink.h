// MumbleMove
//
// Copyright (C) 2020  Thomas Axelsson
//
// This file is part of MumbleMove.
//
// MumbleMove is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// MumbleMove is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with MumbleMove.  If not, see <https://www.gnu.org/licenses/>.
//

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

    bool link();
    void initStaticValues();

    LinkedMem *lm = nullptr;
#ifdef _WIN32
    HANDLE hMapObject = nullptr;
#endif

signals:

public slots:
};

#endif // MUMBLELINK_H
