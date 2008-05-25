/***************************************************************************
 *   Copyright (C) 2007 by Pino Toscano <pino@kde.org>                     *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#include "kolourpicker.h"

#include <qapplication.h>
#include <qclipboard.h>
#include <qcursor.h>
#include <qdesktopwidget.h>
#include <qevent.h>
#include <qgraphicssceneevent.h>
#include <qgraphicslinearlayout.h>
#include <qicon.h>
#include <qiconengine.h>
#include <qimage.h>
#include <qmimedata.h>
#include <qpainter.h>
#include <qpixmap.h>
#include <qpushbutton.h>

#include <kdebug.h>
#include <kicon.h>
#include <klocale.h>
#include <kmenu.h>

#include <plasma/widgets/pushbutton.h>

static KMenu* buildMenuForColor(const QColor &color)
{
    KMenu *menu = new KMenu();
    QAction *act = menu->addAction(KIcon("draw-text"), QString("%1, %2, %3").arg(color.red()).arg(color.green()).arg(color.blue()));
    act->setData(color);
    QString htmlName = color.name();
    QString htmlNameUp = htmlName.toUpper();
    KIcon mimeIcon("text-html");
    act = menu->addAction(mimeIcon, htmlName);
    act->setData(color);
    act = menu->addAction(mimeIcon, htmlName.mid(1));
    act->setData(color);
    if (htmlNameUp != htmlName)
    {
        act = menu->addAction(mimeIcon, htmlNameUp);
        act->setData(color);
        act = menu->addAction(mimeIcon, htmlNameUp.mid(1));
        act->setData(color);
    }
    return menu;
}


class ColorIconEngine : public QIconEngine
{
    public:
        ColorIconEngine(const QColor &color);
        virtual ~ColorIconEngine();

        virtual void paint(QPainter *painter, const QRect &rect, QIcon::Mode mode, QIcon::State state);
        virtual QPixmap pixmap(const QSize &size, QIcon::Mode mode, QIcon::State state);

    public:
        QColor m_color;
};

ColorIconEngine::ColorIconEngine(const QColor &color)
    : m_color(color)
{
}

ColorIconEngine::~ColorIconEngine()
{
}

void ColorIconEngine::paint(QPainter *painter, const QRect &rect, QIcon::Mode mode, QIcon::State state)
{
    Q_UNUSED(mode)
    Q_UNUSED(state)
    painter->setPen(Qt::NoPen);
    painter->setBrush(m_color);
    painter->drawEllipse(rect);
}

QPixmap ColorIconEngine::pixmap(const QSize &size, QIcon::Mode mode, QIcon::State state)
{
    QPixmap pix(size);
    pix.fill(QColor(0,0,0,0));
    QPainter p(&pix);
    paint(&p, pix.rect(), mode, state);
    p.end();
    return pix;
}


class PickerButton : public Plasma::PushButton
{
    public:
        PickerButton(QGraphicsWidget *parent = 0);

        void adaptToFormFactor(Plasma::FormFactor formFactor);
};

PickerButton::PickerButton(QGraphicsWidget *parent)
    : Plasma::PushButton(parent)
{
}

void PickerButton::adaptToFormFactor(Plasma::FormFactor formFactor)
{
    switch (formFactor)
    {
    case Plasma::Planar:
    case Plasma::MediaCenter:
        setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
        break;
    case Plasma::Horizontal:
        setSizePolicy(QSizePolicy::Minimum, QSizePolicy::MinimumExpanding);
        break;
    case Plasma::Vertical:
        setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Minimum);
        break;
    }
}


Kolourpicker::Kolourpicker(QObject *parent, const QVariantList &args)
    : Plasma::Applet(parent, args), m_grabWidget(0)
{
    resize(40, 80);
    setAspectRatioMode(Plasma::IgnoreAspectRatio);

    QGraphicsLinearLayout *mainlay = new QGraphicsLinearLayout(Qt::Vertical);
    setLayout(mainlay);
    mainlay->setSpacing(4);
    mainlay->setContentsMargins(0.0, 0.0, 0.0, 0.0);

    m_grabButton = new PickerButton(this);
    mainlay->addItem(m_grabButton);
    m_grabButton->nativeWidget()->setIcon(KIcon("color-picker"));
    connect(m_grabButton, SIGNAL(clicked()), this, SLOT(grabClicked()));

    m_historyButton = new PickerButton(this);
    mainlay->addItem(m_historyButton);
    m_historyButton->setEnabled(false);
    m_historyButton->nativeWidget()->setIcon(QIcon(new ColorIconEngine(Qt::gray)));
    connect(m_historyButton, SIGNAL(clicked()), this, SLOT(historyClicked()));

    KMenu *menu = new KMenu();
    menu->addTitle(i18n("History"));
    m_historyMenu = menu;
    m_historyMenu->addSeparator();
    QAction *act = m_historyMenu->addAction(KIcon("edit-clear-history"), i18n("Clear History"));
    connect(act, SIGNAL(triggered(bool)), this, SLOT(clearHistory()));
}

Kolourpicker::~Kolourpicker()
{
    clearHistory();
    delete m_historyMenu;
}

void Kolourpicker::constraintsEvent(Plasma::Constraints constraints)
{
    if (constraints & Plasma::FormFactorConstraint) {
        if (formFactor() == Plasma::Planar) {
            setBackgroundHints(Plasma::Applet::StandardBackground);
        } else {
            setBackgroundHints(Plasma::Applet::NoBackground);
        }
        m_grabButton->adaptToFormFactor(formFactor());
        m_historyButton->adaptToFormFactor(formFactor());

        QGraphicsLinearLayout *l = dynamic_cast<QGraphicsLinearLayout *>(layout());
        if (formFactor() == Plasma::Horizontal) {
            l->setOrientation(Qt::Horizontal);
        } else {
            l->setOrientation(Qt::Vertical);
        }
    }
}

bool Kolourpicker::sceneEventFilter(QGraphicsItem *watched, QEvent *event)
{
    if (watched == m_grabButton && event->type() == QEvent::GraphicsSceneMouseRelease)
    {
        m_grabWidget = static_cast<QGraphicsSceneMouseEvent *>(event)->widget();
        if (m_grabWidget && m_grabWidget->parentWidget())
        {
            m_grabWidget = m_grabWidget->parentWidget();
        }
        if (m_grabWidget)
        {
            m_grabWidget->installEventFilter(this);
        }
    }
    return false;
}

bool Kolourpicker::eventFilter(QObject *watched, QEvent *event)
{
    if (watched == m_grabWidget && event->type() == QEvent::MouseButtonRelease)
    {
        m_grabWidget->removeEventFilter(this);
        m_grabWidget->releaseMouse();

        QMouseEvent *me = static_cast<QMouseEvent *>(event);

        QDesktopWidget *desktop = QApplication::desktop();
        QPixmap pix = QPixmap::grabWindow(desktop->winId(), me->globalPos().x(), me->globalPos().y(), 1, 1);
        QImage img = pix.toImage();
        QColor color(img.pixel(0, 0));

        kDebug() << event->type() << me->globalPos() << color;

        addColor(color);

        KMenu *newmenu = buildMenuForColor(color);
        newmenu->addTitle(QIcon(new ColorIconEngine(color)), i18n("Copy Color Value"), newmenu->actions().first());
        connect(newmenu, SIGNAL(triggered(QAction*)), this, SLOT(colorActionTriggered(QAction*)));
        newmenu->exec(QCursor::pos());
        delete newmenu;
    }
    return Plasma::Applet::eventFilter(watched, event);
}

QVariant Kolourpicker::itemChange(GraphicsItemChange change, const QVariant &value)
{
    if (change == ItemSceneChange)
    {
        QMetaObject::invokeMethod(this, "installFilter", Qt::QueuedConnection);
    }
    return Plasma::Applet::itemChange(change, value);
}

void Kolourpicker::grabClicked()
{
    if (m_grabWidget)
    {
        m_grabWidget->grabMouse(Qt::CrossCursor);
    }
}

void Kolourpicker::historyClicked()
{
    m_historyMenu->popup(QCursor::pos());
}

void Kolourpicker::colorActionTriggered(QAction *act)
{
    if (!act)
        return;

    QColor color = qvariant_cast<QColor>(act->data());
    if (!color.isValid())
        return;

    QMimeData *mime = new QMimeData();
    mime->setColorData(color);
    mime->setText(act->text());
    QApplication::clipboard()->setMimeData(mime, QClipboard::Clipboard);
}

void Kolourpicker::clearHistory()
{
    m_historyButton->setEnabled(false);
    m_historyButton->nativeWidget()->setIcon(QIcon(new ColorIconEngine(Qt::gray)));
    QHash<QColor, QAction *>::ConstIterator it = m_menus.begin(), itEnd = m_menus.end();
    for ( ; it != itEnd; ++it )
    {
        m_historyMenu->removeAction(*it);
        delete *it;
    }
    m_menus.clear();
}

void Kolourpicker::installFilter()
{
    m_grabButton->installSceneEventFilter(this);
}

void Kolourpicker::addColor(const QColor &color)
{
    QHash<QColor, QAction *>::ConstIterator it = m_menus.find(color);
    if (it != m_menus.end())
        return;

    KMenu *newmenu = buildMenuForColor(color);
    QAction *act = newmenu->menuAction();
    QIcon colorIcon(new ColorIconEngine(color));
    act->setIcon(colorIcon);
    act->setText(QString("%1, %2, %3").arg(color.red()).arg(color.green()).arg(color.blue()));
    connect(newmenu, SIGNAL(triggered(QAction*)), this, SLOT(colorActionTriggered(QAction*)));
    m_historyMenu->insertMenu(m_historyMenu->actions().at(1), newmenu);
    m_historyButton->nativeWidget()->setIcon(colorIcon);
    m_menus.insert(color, act);
    m_historyButton->setEnabled(true);
}

#include "kolourpicker.moc"
