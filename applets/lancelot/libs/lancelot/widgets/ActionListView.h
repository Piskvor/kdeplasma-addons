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

// TODO: Convert to dptr

#ifndef LANCELOT_LISTVIEW_H_
#define LANCELOT_LISTVIEW_H_

#include "../lancelot_export.h"

#include <QtGui>
#include <QtCore>
#include <KDebug>
#include <KIcon>
#include "Widget.h"
#include "ExtenderButton.h"
#include "../models/ActionListViewModels.h"
namespace Lancelot
{


class LANCELOT_EXPORT ActionListView : public Widget
{
    Q_OBJECT

public:
    ActionListView(QString name, ActionListViewModel * model = 0, QGraphicsItem * parent = 0);
    virtual ~ActionListView();

    void setModel(ActionListViewModel * model);
    ActionListViewModel * model();

    void setMinimumItemHeight(int height);
    void setMaximumItemHeight(int height);
    void setPreferredItemHeight(int height);

    int minimumItemHeight();
    int maximumItemHeight();
    int preferredItemHeight();

    void setCategoryItemHeight(int height);
    int categoryItemHeight();

    void setExtenderPosition(ExtenderButton::ExtenderPosition position);
    ExtenderButton::ExtenderPosition extenderPosition();

    //void paintWidget ( QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget );
    void setGeometry (const QRectF & geometry);

    void setItemsGroup(WidgetGroup * group = NULL);
    void setItemsGroupByName(const QString & group);
    WidgetGroup * itemsGroup();

    void wheelEvent ( QGraphicsSceneWheelEvent * event );
    void setGroup(WidgetGroup * group = NULL);

Q_SIGNALS:
    void activated(int index);

protected slots:
    void scrollTimer();
    void itemActivated(int index);

    void modelUpdated();
    void modelItemInserted(int index);
    void modelItemDeleted(int index);
    void modelItemAltered(int index);

private:
    enum ScrollDirection {
        Up = 1, No = 0, Down = -1
    };

    enum ListTail {
        Start = 0, End = 1
    };

    class ScrollButton : public Lancelot::BaseActionWidget {
    public:
        ScrollButton (ActionListView::ScrollDirection direction, ActionListView * list = NULL, QGraphicsItem * parent = NULL);
        void hoverEnterEvent ( QGraphicsSceneHoverEvent * event);
        void hoverLeaveEvent ( QGraphicsSceneHoverEvent * event);

    private:
        ActionListView * m_list;
        ActionListView::ScrollDirection m_direction;
    };

    ActionListViewModel * m_model;
    WidgetGroup * m_itemsGroup;


    int m_minimumItemHeight;
    int m_maximumItemHeight;
    int m_preferredItemHeight;
    int m_categoryItemHeight;
    int m_currentItemHeight;

    ExtenderButton::ExtenderPosition m_extenderPosition;
    ScrollButton * scrollButtonUp, * scrollButtonDown;

    void scroll(ScrollDirection direction);
    void scrollBy(int ammount);
    ScrollDirection m_scrollDirection;
    QTimer m_scrollTimer;
    int m_scrollInterval;
    int m_scrollTimes;

    int m_topButtonIndex;
    QList< QPair < ExtenderButton *, int > > m_buttons;
    QList< ExtenderButton * > m_buttonsLimbo; // Buttons that are no longer needed, but not deleted

    QTransform m_topButtonScale;
    QTransform m_bottomButtonScale;

    QSignalMapper m_signalMapper;

    bool m_initialButtonsCreationRunning;
    void initialButtonsCreation();

    void positionScrollButtons();
    Lancelot::ExtenderButton * createButton();
    void deleteAllButtons();
    bool addButton(ListTail where);
    void deleteButton(ListTail where);
    int calculateItemHeight();

    friend class ScrollButton;
};

}

#endif /*LANCELOT_LISTVIEW_H_*/
