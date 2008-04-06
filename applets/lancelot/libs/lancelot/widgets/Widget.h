/*
 *   Copyright (C) 2007 Ivan Cukic <ivan.cukic+kde@gmail.com>
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU Lesser/Library General Public License version 2,
 *   or (at your option) any later version, as published by the Free
 *   Software Foundation
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU Lesser/Library General Public License for more details
 *
 *   You should have received a copy of the GNU Lesser/Library General Public
 *   License along with this program; if not, write to the
 *   Free Software Foundation, Inc.,
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#ifndef LANCELOT_WIDGET_H_
#define LANCELOT_WIDGET_H_

#include "../lancelot_export.h"

#include <QtGui>
#include <plasma/widgets/widget.h>
#include "../Global.h"

namespace Lancelot
{

class LANCELOT_EXPORT Widget : public Plasma::Widget
{
    Q_OBJECT
public:
    Widget(QString name = "", QGraphicsItem * parent = 0);
    virtual ~Widget();

    void enable(bool value = true);
    void disable();
    bool isEnabled() const;

    void setGeometry (const QRectF & geometry);
    void update (const QRectF &rect = QRectF());
    void update (qreal x, qreal y, qreal w, qreal h);

    virtual void setGroupByName(const QString & groupName);
    virtual void setGroup(WidgetGroup * group = NULL);
    WidgetGroup * group();
    Instance * instance();

    QString name() const;
    void setName(QString name);

    bool isHovered() const;

Q_SIGNALS:
    void mouseHoverEnter();
    void mouseHoverLeave();

protected:
    class Private;
    Private * d;

    virtual void groupUpdated();

    virtual void paintWidget (QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget = 0);
    virtual void hoverEnterEvent ( QGraphicsSceneHoverEvent * event );
    virtual void hoverLeaveEvent ( QGraphicsSceneHoverEvent * event );

    void paintBackground (QPainter * painter);
    void paintBackground (QPainter * painter, const QString & element);

    friend class WidgetGroup;
    friend class Global;
};

}

#endif /*LANCELOT_WIDGET_H_*/
