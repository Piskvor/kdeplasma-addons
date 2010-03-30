/*/***************************************************************************
 *   Copyright (C) 2010 by Björn Ruberg <bjoern@ruberg-wegener.de>    *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA .        *
 ***************************************************************************/


#ifndef STICKYKEY_H
#define STICKYKEY_H

#include "FuncKey.h"

class StickyKey : public FuncKey
{
public:
    StickyKey(QPoint relativePosition, QSize relativeSize, unsigned int keycode, QString label);

    virtual void pressed();
    virtual void released();
    virtual void reset();
    virtual void setPixmap(QPixmap *pixmap);
    virtual void unpress();

private:
    bool m_acceptPixmap;
    bool m_dorelease;
    bool m_toggled;


};

#endif // STICKYKEY_H
