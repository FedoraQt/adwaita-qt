/*************************************************************************
 * Copyright (C) 2014 by Hugo Pereira Da Costa <hugo.pereira@free.fr>    *
 * Copyright (C) 2014-2018 Martin Bříza <m@rtinbriza.cz>                 *
 * Copyright (C) 2019-2020 Jan Grulich <jgrulich@redhat.com>             *
 *                                                                       *
 * This program is free software; you can redistribute it and/or modify  *
 * it under the terms of the GNU General Public License as published by  *
 * the Free Software Foundation; either version 2 of the License, or     *
 * (at your option) any later version.                                   *
 *                                                                       *
 * This program is distributed in the hope that it will be useful,       *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 * GNU General Public License for more details.                          *
 *                                                                       *
 * You should have received a copy of the GNU General Public License     *
 * along with this program; if not, write to the                         *
 * Free Software Foundation, Inc.,                                       *
 * 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA .        *
 *************************************************************************/

// Adwaita style
#include "adwaitastyle.h"
#include "adwaitahelper.h"

// Adwaita lib
#include "animations/adwaitaanimations.h"
#include "adwaita.h"
#include "adwaitacolors.h"
#include "adwaitamnemonics.h"
#include "adwaitasplitterproxy.h"
#include "adwaitarenderer.h"
#include "adwaitawidgetexplorer.h"
#include "adwaitawindowmanager.h"

#include <QApplication>
#include <QCheckBox>
#include <QComboBox>
#include <QDial>
#include <QDialog>
#include <QDockWidget>
#include <QFormLayout>
#include <QGraphicsView>
#include <QGroupBox>
#include <QItemDelegate>
#include <QLineEdit>
#include <QMainWindow>
#include <QMdiSubWindow>
#include <QMenu>
#include <QPainter>
#include <QProxyStyle>
#include <QPushButton>
#include <QRadioButton>
#include <QScrollBar>
#include <QSpinBox>
#include <QSplitterHandle>
#include <QTextEdit>
#include <QToolBar>
#include <QToolBox>
#include <QToolButton>
#include <QWidgetAction>

#ifndef M_PI
    #define M_PI 3.14159265358979323846
#endif

namespace AdwaitaPrivate
{
// needed to keep track of tabbars when being dragged
class TabBarData: public QObject
{
public:
    //* constructor
    explicit TabBarData(QObject *parent)
        : QObject(parent)
    {}

    //* destructor
    virtual ~TabBarData(void)
    {}

    //* assign target tabBar
    void lock(const QWidget *widget)
    {
        _tabBar = widget;
    }

    //* true if tabbar is locked
    bool isLocked(const QWidget *widget) const
    {
        return _tabBar && _tabBar.data() == widget;
    }

    //* release
    void release(void)
    {
        _tabBar.clear();
    }

private:
    //* pointer to target tabBar
    Adwaita::WeakPointer<const QWidget> _tabBar;
};

//* needed to have spacing added to items in combobox
class ComboBoxItemDelegate: public QItemDelegate
{
public:
    //* constructor
    ComboBoxItemDelegate(QAbstractItemView *parent)
        : QItemDelegate(parent)
        , _proxy(parent->itemDelegate())
        , _itemMargin(Adwaita::Metrics::ItemView_ItemMarginWidth)
    {}

    //* destructor
    virtual ~ComboBoxItemDelegate(void)
    {}

    //* paint
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
    {
        // call either proxy or parent class
        if (_proxy) {
            _proxy.data()->paint(painter, option, index);
        } else {
            QItemDelegate::paint(painter, option, index);
        }
    }

    //* size hint for index
    virtual QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
    {
        // get size from either proxy or parent class
        QSize size(_proxy ?
                   _proxy.data()->sizeHint(option, index) :
                   QItemDelegate::sizeHint(option, index));

        // adjust and return
        if (size.isValid()) {
            size.rheight() += _itemMargin * 2;
        }

        return size;
    }

private:
    //* proxy
    Adwaita::WeakPointer<QAbstractItemDelegate> _proxy;

    //* margin
    int _itemMargin;
};

} // namespace AdwaitaPrivate

void tabLayout(const QStyleOptionTab *opt, const QWidget *widget, QRect *textRect, QRect *iconRect, const QStyle *proxyStyle)
{
    Q_ASSERT(textRect);
    Q_ASSERT(iconRect);
    QRect tr = opt->rect;
    bool verticalTabs = opt->shape == QTabBar::RoundedEast
                        || opt->shape == QTabBar::RoundedWest
                        || opt->shape == QTabBar::TriangularEast
                        || opt->shape == QTabBar::TriangularWest;
    if (verticalTabs)
        tr.setRect(0, 0, tr.height(), tr.width()); //0, 0 as we will have a translate transform

    int verticalShift = proxyStyle->pixelMetric(QStyle::PM_TabBarTabShiftVertical, opt, widget);
    int horizontalShift = proxyStyle->pixelMetric(QStyle::PM_TabBarTabShiftHorizontal, opt, widget);
    int hpadding = proxyStyle->pixelMetric(QStyle::PM_TabBarTabHSpace, opt, widget) / 2;
    int vpadding = proxyStyle->pixelMetric(QStyle::PM_TabBarTabVSpace, opt, widget) / 2;
    if (opt->shape == QTabBar::RoundedSouth || opt->shape == QTabBar::TriangularSouth)
        verticalShift = -verticalShift;
    tr.adjust(hpadding, verticalShift - vpadding, horizontalShift - hpadding, vpadding);
    bool selected = opt->state & QStyle::State_Selected;
    if (selected) {
        tr.setTop(tr.top() - verticalShift);
        tr.setRight(tr.right() - horizontalShift);
    }

    // left widget
    if (!opt->leftButtonSize.isEmpty()) {
        tr.setLeft(tr.left() + 4 +
                   (verticalTabs ? opt->leftButtonSize.height() : opt->leftButtonSize.width()));
    }
    // right widget
    if (!opt->rightButtonSize.isEmpty()) {
        tr.setRight(tr.right() - 4 -
                    (verticalTabs ? opt->rightButtonSize.height() : opt->rightButtonSize.width()));
    }

    // icon
    if (!opt->icon.isNull()) {
        QSize iconSize = opt->iconSize;
        if (!iconSize.isValid()) {
            int iconExtent = proxyStyle->pixelMetric(QStyle::PM_SmallIconSize);
            iconSize = QSize(iconExtent, iconExtent);
        }
        QSize tabIconSize = opt->icon.actualSize(iconSize,
                            (opt->state & QStyle::State_Enabled) ? QIcon::Normal : QIcon::Disabled,
                            (opt->state & QStyle::State_Selected) ? QIcon::On : QIcon::Off);

        *iconRect = QRect(tr.left(), tr.center().y() - tabIconSize.height() / 2,
                          tabIconSize.width(), tabIconSize .height());
        if (!verticalTabs) {
            *iconRect = proxyStyle->visualRect(opt->direction, opt->rect, *iconRect);
        }
        tr.setLeft(tr.left() + tabIconSize.width() + 4);
    }

    if (!verticalTabs) {
        tr = proxyStyle->visualRect(opt->direction, opt->rect, tr);
    }

    *textRect = tr;
}

namespace Adwaita
{
//______________________________________________________________
Style::Style(ColorVariant variant)
    : _addLineButtons(SingleButton)
    , _subLineButtons(SingleButton)
    , _helper(new Helper())
    , _animations(new Animations(this))
    , _mnemonics(new Mnemonics(this))
    , _windowManager(new WindowManager(this))
    , _splitterFactory(new SplitterFactory(this))
    , _widgetExplorer(new WidgetExplorer(this))
    , _tabBarData(new AdwaitaPrivate::TabBarData(this))
    , _variant(variant)
    , _dark(variant == AdwaitaDark || variant == AdwaitaHighcontrastInverse)
{
    // Detect if running under KDE, if so set menus, etc, to have translucent background.
    // For GNOME desktop, dont want translucent backgrounds otherwise no menu shadow is drawn.
    _isKDE = qgetenv("XDG_CURRENT_DESKTOP").toLower() == "kde";
    _isGNOME = qgetenv("XDG_CURRENT_DESKTOP").toLower() == "gnome";

    // call the slot directly; this initial call will set up things that also
    // need to be reset when the system palette changes
    loadConfiguration();
}

//______________________________________________________________
Style::~Style(void)
{
    delete _helper;
}

//______________________________________________________________
void Style::polish(QWidget *widget)
{
    if (!widget) {
        return;
    }

    // register widget to animations
    _animations->registerWidget(widget);
    _windowManager->registerWidget(widget);
    _splitterFactory->registerWidget(widget);

    // enable mouse over effects for all necessary widgets
    if (qobject_cast<QAbstractItemView *>(widget)
            || qobject_cast<QAbstractSpinBox *>(widget)
            || qobject_cast<QCheckBox *>(widget)
            || qobject_cast<QComboBox *>(widget)
            || qobject_cast<QDial *>(widget)
            || qobject_cast<QLineEdit *>(widget)
            || qobject_cast<QPushButton *>(widget)
            || qobject_cast<QRadioButton *>(widget)
            || qobject_cast<QScrollBar *>(widget)
            || qobject_cast<QSlider *>(widget)
            || qobject_cast<QSplitterHandle *>(widget)
            || qobject_cast<QTabBar *>(widget)
            || qobject_cast<QTextEdit *>(widget)
            || qobject_cast<QToolButton *>(widget)
            || qobject_cast<QHeaderView *>(widget)
            || widget->inherits("KTextEditor::View")) {
        widget->setAttribute(Qt::WA_Hover);
    }

    if (qobject_cast<QTabBar *>(widget)) {
        qobject_cast<QTabBar *>(widget)->setDrawBase(true);
    }

    // enforce translucency for drag and drop window
    if (widget->testAttribute(Qt::WA_X11NetWmWindowTypeDND) && _helper->compositingActive()) {
        widget->setAttribute(Qt::WA_TranslucentBackground);
        widget->clearMask();
    }

    // scrollarea polishing is somewhat complex. It is moved to a dedicated method
    polishScrollArea(qobject_cast<QAbstractScrollArea *>(widget));

    if (QAbstractItemView *itemView = qobject_cast<QAbstractItemView *>(widget)) {
        // enable mouse over effects in itemviews' viewport
        itemView->viewport()->setAttribute(Qt::WA_Hover);
    } else if (QGroupBox *groupBox = qobject_cast<QGroupBox *>(widget))  {
        // checkable group boxes
        if (groupBox->isCheckable()) {
            groupBox->setAttribute(Qt::WA_Hover);
        }
    } else if (qobject_cast<QAbstractButton *>(widget) && qobject_cast<QDockWidget *>(widget->parent())) {
        widget->setAttribute(Qt::WA_Hover);
    } else if (qobject_cast<QAbstractButton *>(widget) && qobject_cast<QToolBox *>(widget->parent())) {
        widget->setAttribute(Qt::WA_Hover);
    } else if (qobject_cast<QFrame *>(widget) && widget->parent() && widget->parent()->inherits("KTitleWidget")) {
        widget->setAutoFillBackground(false);
        if (!Adwaita::Config::TitleWidgetDrawFrame) {
            widget->setBackgroundRole(QPalette::Window);
        }
    }

    if (qobject_cast<QScrollBar *>(widget)) {
        // remove opaque painting for scrollbars
        widget->setAttribute(Qt::WA_OpaquePaintEvent, false);
    } else if (widget->inherits("KTextEditor::View")) {
        addEventFilter(widget);
    } else if (QToolButton *toolButton = qobject_cast<QToolButton *>(widget)) {
        if (toolButton->autoRaise()) {
            // for flat toolbuttons, adjust foreground and background role accordingly
            widget->setBackgroundRole(QPalette::NoRole);
            widget->setForegroundRole(QPalette::WindowText);
        }

        if (widget->parentWidget() &&
                widget->parentWidget()->parentWidget() &&
                widget->parentWidget()->parentWidget()->inherits("Gwenview::SideBarGroup")) {
            widget->setProperty(PropertyNames::toolButtonAlignment, Qt::AlignLeft);
        }
    } else if (qobject_cast<QDockWidget *>(widget)) {
        // add event filter on dock widgets
        // and alter palette
        widget->setAutoFillBackground(false);
        widget->setContentsMargins(Metrics::Frame_FrameWidth, Metrics::Frame_FrameWidth, Metrics::Frame_FrameWidth, Metrics::Frame_FrameWidth);
        addEventFilter(widget);
    } else if (qobject_cast<QMdiSubWindow *>(widget)) {
        widget->setAutoFillBackground(false);
        addEventFilter(widget);
    } else if (qobject_cast<QToolBox *>(widget)) {
        widget->setBackgroundRole(QPalette::NoRole);
        widget->setAutoFillBackground(false);
    } else if (widget->parentWidget() && widget->parentWidget()->parentWidget() && qobject_cast<QToolBox *>(widget->parentWidget()->parentWidget()->parentWidget())) {
        widget->setBackgroundRole(QPalette::NoRole);
        widget->setAutoFillBackground(false);
        widget->parentWidget()->setAutoFillBackground(false);
    } else if (qobject_cast<QMenu *>(widget)) {
        setTranslucentBackground(widget);
    } else if (qobject_cast<QCommandLinkButton *>(widget)) {
        addEventFilter(widget);
    } else if (QComboBox *comboBox = qobject_cast<QComboBox *>(widget)) {
        if (!hasParent(widget, "QWebView")) {
            QAbstractItemView *itemView(comboBox->view());
            if (itemView && itemView->itemDelegate() && itemView->itemDelegate()->inherits("QComboBoxDelegate")) {
                itemView->setItemDelegate(new AdwaitaPrivate::ComboBoxItemDelegate(itemView));
            }
            if (comboBox->isEditable()) {
                QLineEdit *lineEdit = comboBox->lineEdit();
                if (lineEdit && !comboBox->isEnabled()) {
                    QPalette pal = lineEdit->palette();
                    pal.setColor(QPalette::Base, Colors::palette(_variant).color(QPalette::Window));
                    lineEdit->setPalette(pal);
                    lineEdit->setAutoFillBackground(true);
                }
            }
        }
    } else if (widget->inherits("QComboBoxPrivateContainer")) {
        addEventFilter(widget);
        setTranslucentBackground(widget);
    } else if (widget->inherits("QTipLabel")) {
        setTranslucentBackground(widget);
    } else if (QLineEdit *lineEdit = qobject_cast<QLineEdit *>(widget)) {
        // Do not use additional margin if the QLineEdit is really small
        const bool useMarginWidth = lineEdit->width() > lineEdit->fontMetrics().horizontalAdvance("#####");
        const bool useMarginHeight = lineEdit->height() > lineEdit->fontMetrics().height() + (2 * Metrics::LineEdit_MarginHeight);
        const int marginHeight = useMarginHeight ? Metrics::LineEdit_MarginHeight : 0;
        const int marginWidth = useMarginWidth ? Metrics::LineEdit_MarginWidth : 0;
        lineEdit->setTextMargins(marginWidth, marginHeight, marginWidth, marginHeight);
    } else if (QSpinBox *spinBox = qobject_cast<QSpinBox *>(widget)) {
        if (!spinBox->isEnabled()) {
            QPalette pal = spinBox->palette();
            pal.setColor(QPalette::Base, Colors::palette(_variant).color(QPalette::Window));
            spinBox->setPalette(pal);
            spinBox->setAutoFillBackground(true);
        }
    }

    // HACK to avoid different text color in unfocused views
    // This has a side effect that the view will never grey out, but it's still better then having
    // views greyed out when the application is active
    if (QPointer<QAbstractItemView> view = qobject_cast<QAbstractItemView *>(widget)) {
        QPalette pal = view->palette();
        // No custom text color used, we can do our HACK
        if (!hasCustomTextColors(pal)) {
            pal.setColor(QPalette::Inactive, QPalette::Text, pal.color(QPalette::Active, QPalette::Text));
            view->setPalette(pal);
        }
    }

    if (!widget->parent() || !qobject_cast<QWidget *>(widget->parent()) || qobject_cast<QDialog *>(widget) || qobject_cast<QMainWindow *>(widget)) {
        addEventFilter(widget);
    }

    // base class polishing
    ParentStyleClass::polish(widget);
}

//______________________________________________________________
void Style::polishScrollArea(QAbstractScrollArea *scrollArea)
{
    // check argument
    if (!scrollArea) {
        return;
    }

    // enable mouse over effect in sunken scrollareas that support focus
    if (scrollArea->frameShadow() == QFrame::Sunken && scrollArea->focusPolicy() & Qt::StrongFocus) {
        scrollArea->setAttribute(Qt::WA_Hover);
    }

    if (scrollArea->viewport() && scrollArea->inherits("KItemListContainer") && scrollArea->frameShape() == QFrame::NoFrame) {
        scrollArea->viewport()->setBackgroundRole(QPalette::Window);
        scrollArea->viewport()->setForegroundRole(QPalette::WindowText);
    }

    // add event filter, to make sure proper background is rendered behind scrollbars
    addEventFilter(scrollArea);

    // force side panels as flat, on option
    if (scrollArea->inherits("KDEPrivate::KPageListView") || scrollArea->inherits("KDEPrivate::KPageTreeView")) {
        scrollArea->setProperty(PropertyNames::sidePanelView, true);
    }

    // for all side view panels, unbold font (design choice)
    if (scrollArea->property(PropertyNames::sidePanelView).toBool()) {
        // upbold list font
        QFont font(scrollArea->font());
        font.setBold(false);
        scrollArea->setFont(font);

        // adjust background role
        if (!Adwaita::Config::SidePanelDrawFrame) {
            scrollArea->setBackgroundRole(QPalette::Window);
            scrollArea->setForegroundRole(QPalette::WindowText);

            if (scrollArea->viewport()) {
                scrollArea->viewport()->setBackgroundRole(QPalette::Window);
                scrollArea->viewport()->setForegroundRole(QPalette::WindowText);
            }
        }
    }

    // disable autofill background for flat (== NoFrame) scrollareas, with QPalette::Window as a background
    // this fixes flat scrollareas placed in a tinted widget, such as groupboxes, tabwidgets or framed dock-widgets
    if (!(scrollArea->frameShape() == QFrame::NoFrame || scrollArea->backgroundRole() == QPalette::Window)) {
        return;
    }

    // get viewport and check background role
    QWidget *viewport(scrollArea->viewport());
    if (!(viewport && viewport->backgroundRole() == QPalette::Window)) {
        return;
    }

    // change viewport autoFill background.
    // do the same for all children if the background role is QPalette::Window
    viewport->setAutoFillBackground(false);
    QList<QWidget *> children(viewport->findChildren<QWidget *>());
    foreach (QWidget *child, children) {
        if (child->parent() == viewport && child->backgroundRole() == QPalette::Window) {
            child->setAutoFillBackground(false);
        }
    }
}

//_______________________________________________________________
void Style::unpolish(QWidget *widget)
{
    // register widget to animations
    _animations->unregisterWidget(widget);
    _windowManager->unregisterWidget(widget);
    _splitterFactory->unregisterWidget(widget);

    // remove event filter
    if (qobject_cast<QAbstractScrollArea *>(widget)
            || qobject_cast<QDockWidget *>(widget)
            || qobject_cast<QMdiSubWindow *>(widget)
            || widget->inherits("QComboBoxPrivateContainer")
            || qobject_cast<QDialog *>(widget)
            || qobject_cast<QMainWindow *>(widget)) {
        widget->removeEventFilter(this);
    }

    ParentStyleClass::unpolish(widget);
}

void Style::polish(QPalette &palette)
{
    palette = Colors::palette(_variant);
}

QPalette Style::standardPalette() const
{
    return Colors::palette(_variant);
}

//______________________________________________________________
int Style::pixelMetric(PixelMetric metric, const QStyleOption *option, const QWidget *widget) const
{
    // handle special cases
    switch (metric) {

    // frame width
    case PM_DefaultFrameWidth:
        if (widget && widget->inherits("QComboBoxPrivateContainer")) {
            return 1;
        }

        if (qobject_cast<const QMenu *>(widget)) {
            return Metrics::Menu_FrameWidth;
        }

        if (qobject_cast<const QLineEdit *>(widget)) {
            return Metrics::LineEdit_FrameWidth;
        }

        if (qobject_cast<const QAbstractScrollArea *>(widget)) {
            return Metrics::ScrollArea_FrameWidth;
        } else if (isQtQuickControl(option, widget)) {
            const QString &elementType = option->styleObject->property("elementType").toString();
            if (elementType == QLatin1String("edit") || elementType == QLatin1String("spinbox")) {
                return Metrics::LineEdit_FrameWidth;
            } else if (elementType == QLatin1String("combobox")) {
                return Metrics::ComboBox_FrameWidth;
            }
        }
        // fallback
        return Metrics::Frame_FrameWidth;
    case PM_ComboBoxFrameWidth: {
        const QStyleOptionComboBox *comboBoxOption(qstyleoption_cast< const QStyleOptionComboBox *>(option));
        return comboBoxOption && comboBoxOption->editable ? Metrics::LineEdit_FrameWidth : Metrics::ComboBox_FrameWidth;
    }
    case PM_SpinBoxFrameWidth:
        return Metrics::SpinBox_FrameWidth;
    case PM_ToolBarFrameWidth:
        return Metrics::ToolBar_FrameWidth;
    case PM_ToolTipLabelFrameWidth:
        return Metrics::ToolTip_FrameWidth;

    // layout
    case PM_LayoutLeftMargin:
    case PM_LayoutTopMargin:
    case PM_LayoutRightMargin:
    case PM_LayoutBottomMargin: {
        /*
         * use either Child margin or TopLevel margin,
         * depending on  widget type
         */
        if ((option && (option->state & QStyle::State_Window)) || (widget && widget->isWindow())) {
            return Metrics::Layout_TopLevelMarginWidth;
        } else if (widget && widget->inherits("KPageView")) {
            return 0;
        } else {
            return Metrics::Layout_ChildMarginWidth;
        }
    }
    case PM_LayoutHorizontalSpacing:
        return Metrics::Layout_DefaultSpacing;
    case PM_LayoutVerticalSpacing:
        return Metrics::Layout_DefaultSpacing;

    // buttons
    case PM_ButtonMargin: {
        // needs special case for kcalc buttons, to prevent the application to set too small margins
        if (widget && widget->inherits("KCalcButton")) {
            return Metrics::Button_MarginWidth + 4;
        } else {
            return Metrics::Button_MarginWidth;
        }
    }

    case PM_ButtonDefaultIndicator:
        return 0;
    case PM_ButtonShiftHorizontal:
        return 0;
    case PM_ButtonShiftVertical:
        return 0;

    // menubars
    case PM_MenuBarPanelWidth:
        return 0;
    case PM_MenuBarHMargin:
        return 0;
    case PM_MenuBarVMargin:
        return 0;
    case PM_MenuBarItemSpacing:
        return 0;
    case PM_MenuDesktopFrameWidth:
        return 0;

    // menu buttons
    case PM_MenuButtonIndicator:
        return Metrics::MenuButton_IndicatorWidth;
    case PM_MenuVMargin:
        return 2;
    case PM_MenuHMargin:
        return _isGNOME ? 0 : 1;

    // toolbars
    case PM_ToolBarHandleExtent:
        return Metrics::ToolBar_HandleExtent;
    case PM_ToolBarSeparatorExtent:
        return Metrics::ToolBar_SeparatorWidth;
    case PM_ToolBarExtensionExtent:
        return pixelMetric(PM_SmallIconSize, option, widget) + 2 * Metrics::ToolButton_MarginWidth;

    case PM_ToolBarItemMargin:
        return 0;
    case PM_ToolBarItemSpacing:
        return Metrics::ToolBar_ItemSpacing;

    // tabbars
    case PM_TabBarTabShiftVertical:
        return 0;
    case PM_TabBarTabShiftHorizontal:
        return 0;
    case PM_TabBarTabOverlap:
        return Metrics::TabBar_TabOverlap;
    case PM_TabBarBaseOverlap:
        return Metrics::TabBar_BaseOverlap;
    case PM_TabBarTabHSpace:
        return 2 * Metrics::TabBar_TabMarginWidth;
    case PM_TabBarTabVSpace:
        return 2 * Metrics::TabBar_TabMarginHeight;
    case PM_TabCloseIndicatorWidth:
    case PM_TabCloseIndicatorHeight:
        return pixelMetric(PM_SmallIconSize, option, widget);

    // scrollbars
    case PM_ScrollBarExtent:
        return Metrics::ScrollBar_Extend;
    case PM_ScrollBarSliderMin:
        return Metrics::ScrollBar_MinSliderHeight;

    // title bar
    case PM_TitleBarHeight:
        return 2 * Metrics::TitleBar_MarginWidth + pixelMetric(PM_SmallIconSize, option, widget);

    // sliders
    case PM_SliderThickness:
        return Metrics::Slider_ControlThickness;
    case PM_SliderControlThickness:
        return Metrics::Slider_ControlThickness;
    case PM_SliderLength:
        return Metrics::Slider_ControlThickness;

    // checkboxes and radio buttons
    case PM_IndicatorWidth:
        return Metrics::CheckBox_Size;
    case PM_IndicatorHeight:
        return Metrics::CheckBox_Size;
    case PM_ExclusiveIndicatorWidth:
        return Metrics::CheckBox_Size;
    case PM_ExclusiveIndicatorHeight:
        return Metrics::CheckBox_Size;

    // list heaaders
    case PM_HeaderMarkSize:
        return Metrics::Header_ArrowSize;
    case PM_HeaderMargin:
        return Metrics::Header_MarginWidth;

    // dock widget
    // return 0 here, since frame is handled directly in polish
    case PM_DockWidgetFrameWidth:
        return 0;
    case PM_DockWidgetTitleMargin:
        return Metrics::Frame_FrameWidth;
    case PM_DockWidgetTitleBarButtonMargin:
        return Metrics::ToolButton_MarginWidth;

    case PM_SplitterWidth:
        return Metrics::Splitter_SplitterWidth;
    case PM_DockWidgetSeparatorExtent:
        return Metrics::Splitter_SplitterWidth;
    // fallback
    default:
        return ParentStyleClass::pixelMetric(metric, option, widget);
    }
}

//______________________________________________________________
int Style::styleHint(StyleHint hint, const QStyleOption *option, const QWidget *widget, QStyleHintReturn *returnData) const
{
    switch (hint) {
    case SH_RubberBand_Mask: {
        if (QStyleHintReturnMask *mask = qstyleoption_cast<QStyleHintReturnMask *>(returnData)) {
            mask->region = option->rect;

            /*
             * need to check on widget before removing inner region
             * in order to still preserve rubberband in MainWindow and QGraphicsView
             * in QMainWindow because it looks better
             * in QGraphicsView because the painting fails completely otherwise
             */
            if (widget && (qobject_cast<const QAbstractItemView *>(widget->parent())
                           || qobject_cast<const QGraphicsView *>(widget->parent())
                           || qobject_cast<const QMainWindow *>(widget->parent()))) {
                return true;
            }

            // also check if widget's parent is some itemView viewport
            if (widget && widget->parent() && qobject_cast<const QAbstractItemView *>(widget->parent()->parent())
                    && static_cast<const QAbstractItemView *>(widget->parent()->parent())->viewport() == widget->parent()) {
                return true;
            }

            // mask out center
            mask->region -= insideMargin(option->rect, 1);

            return true;
        }
        return false;
    }

    case SH_ComboBox_ListMouseTracking:
        return true;
    case SH_MenuBar_MouseTracking:
        return true;
    case SH_Menu_MouseTracking:
        return true;
    case SH_Menu_SubMenuPopupDelay:
        return 150;
    case SH_Menu_SloppySubMenus:
        return true;
    case SH_Widget_Animate:
        return Adwaita::Config::AnimationsEnabled;
    case SH_Menu_SupportsSections:
        return true;
    case SH_DialogButtonBox_ButtonsHaveIcons:
        return false;
    case SH_GroupBox_TextLabelVerticalAlignment:
        return Qt::AlignVCenter;
    case SH_TabBar_Alignment:
        return Adwaita::Config::TabBarDrawCenteredTabs ? Qt::AlignCenter : Qt::AlignLeft;
    case SH_ToolBox_SelectedPageTitleBold:
        return false;
    case SH_ScrollBar_MiddleClickAbsolutePosition:
        return true;
    case SH_ScrollView_FrameOnlyAroundContents:
        return false;
    case SH_FormLayoutFormAlignment:
        return Qt::AlignLeft | Qt::AlignTop;
    case SH_FormLayoutLabelAlignment:
        return Qt::AlignRight;
    case SH_FormLayoutFieldGrowthPolicy:
        return QFormLayout::ExpandingFieldsGrow;
    case SH_FormLayoutWrapPolicy:
        return QFormLayout::DontWrapRows;
    case SH_MessageBox_TextInteractionFlags:
        return Qt::TextSelectableByMouse | Qt::LinksAccessibleByMouse;
    case SH_ProgressDialog_CenterCancelButton:
        return false;
    case SH_MessageBox_CenterButtons:
        return false;
    case SH_RequestSoftwareInputPanel:
        return RSIP_OnMouseClick;
    case SH_TitleBar_NoBorder:
        return true;
    case SH_DockWidget_ButtonsHaveFrame:
        return false;
    case SH_ToolTipLabel_Opacity:
        return 204 ;// Should have 30% transparency
    default:
        return ParentStyleClass::styleHint(hint, option, widget, returnData);
    }
}

//______________________________________________________________
QRect Style::subElementRect(SubElement element, const QStyleOption *option, const QWidget *widget) const
{
    switch (element) {
    case SE_PushButtonContents:
        return pushButtonContentsRect(option, widget);
    case SE_PushButtonFocusRect:
        return pushButtonFocusRect(option, widget);
    case SE_CheckBoxContents:
        return checkBoxContentsRect(option, widget);
    case SE_CheckBoxIndicator:
        return checkBoxIndicatorRect(option, widget);
    case SE_CheckBoxFocusRect:
        return checkBoxFocusRect(option, widget);
    case SE_RadioButtonContents:
        return checkBoxContentsRect(option, widget);
    case SE_RadioButtonIndicator:
        return checkBoxIndicatorRect(option, widget);
    case SE_RadioButtonFocusRect:
        return checkBoxFocusRect(option, widget);
    case SE_LineEditContents:
        return lineEditContentsRect(option, widget);
    case SE_ProgressBarGroove:
        return progressBarGrooveRect(option, widget);
    case SE_ProgressBarContents:
        return progressBarContentsRect(option, widget);
    case SE_ProgressBarLabel:
        return progressBarLabelRect(option, widget);
    case SE_HeaderArrow:
        return headerArrowRect(option, widget);
    case SE_HeaderLabel:
        return headerLabelRect(option, widget);
    case SE_SliderFocusRect:
        return sliderFocusRect(option, widget);
    case SE_TabBarTabLeftButton:
        return tabBarTabLeftButtonRect(option, widget);
    case SE_TabBarTabRightButton:
        return tabBarTabRightButtonRect(option, widget);
    case SE_TabWidgetTabBar:
        return tabWidgetTabBarRect(option, widget);
    case SE_TabWidgetTabContents:
        return tabWidgetTabContentsRect(option, widget);
    case SE_TabWidgetTabPane:
        return tabWidgetTabPaneRect(option, widget);
    case SE_TabWidgetLeftCorner:
        return tabWidgetCornerRect(SE_TabWidgetLeftCorner, option, widget);
    case SE_TabWidgetRightCorner:
        return tabWidgetCornerRect(SE_TabWidgetRightCorner, option, widget);
    case SE_ToolBoxTabContents:
        return toolBoxTabContentsRect(option, widget);
    // fallback
    default:
        return ParentStyleClass::subElementRect(element, option, widget);
    }
}

//______________________________________________________________
QRect Style::subControlRect(ComplexControl element, const QStyleOptionComplex *option, SubControl subControl, const QWidget *widget) const
{
    switch (element) {
    case CC_GroupBox:
        return groupBoxSubControlRect(option, subControl, widget);
    case CC_ToolButton:
        return toolButtonSubControlRect(option, subControl, widget);
    case CC_ComboBox:
        return comboBoxSubControlRect(option, subControl, widget);
    case CC_SpinBox:
        return spinBoxSubControlRect(option, subControl, widget);
    case CC_ScrollBar:
        return scrollBarSubControlRect(option, subControl, widget);
    case CC_Dial:
        return dialSubControlRect(option, subControl, widget);
    case CC_Slider:
        return sliderSubControlRect(option, subControl, widget);

    // fallback
    default:
        return ParentStyleClass::subControlRect(element, option, subControl, widget);
    }
}

//______________________________________________________________
QSize Style::sizeFromContents(ContentsType element, const QStyleOption *option, const QSize &size, const QWidget *widget) const
{
    switch (element) {
    case CT_CheckBox:
        return checkBoxSizeFromContents(option, size, widget);
    case CT_RadioButton:
        return checkBoxSizeFromContents(option, size, widget);
    case CT_LineEdit:
        return lineEditSizeFromContents(option, size, widget);
    case CT_ComboBox:
        return comboBoxSizeFromContents(option, size, widget);
    case CT_SpinBox:
        return spinBoxSizeFromContents(option, size, widget);
    case CT_Slider:
        return sliderSizeFromContents(option, size, widget);
    case CT_PushButton:
        return pushButtonSizeFromContents(option, size, widget);
    case CT_ToolButton:
        return toolButtonSizeFromContents(option, size, widget);
    case CT_MenuBar:
        return defaultSizeFromContents(option, size, widget);
    case CT_MenuBarItem:
        return menuBarItemSizeFromContents(option, size, widget);
    case CT_MenuItem:
        return menuItemSizeFromContents(option, size, widget);
    case CT_ProgressBar:
        return progressBarSizeFromContents(option, size, widget);
    case CT_TabWidget:
        return tabWidgetSizeFromContents(option, size, widget);
    case CT_TabBarTab:
        return tabBarTabSizeFromContents(option, size, widget);
    case CT_HeaderSection:
        return headerSectionSizeFromContents(option, size, widget);
    case CT_ItemViewItem:
        return itemViewItemSizeFromContents(option, size, widget);

    // fallback
    default:
        return ParentStyleClass::sizeFromContents(element, option, size, widget);
    }

}

//______________________________________________________________
QStyle::SubControl Style::hitTestComplexControl(ComplexControl control, const QStyleOptionComplex *option, const QPoint &point, const QWidget *widget) const
{
    switch (control) {
    case CC_ScrollBar: {
        QRect grooveRect = subControlRect(CC_ScrollBar, option, SC_ScrollBarGroove, widget);
        if (grooveRect.contains(point)) {
            // Must be either page up/page down, or just click on the slider.
            QRect sliderRect = subControlRect(CC_ScrollBar, option, SC_ScrollBarSlider, widget);

            if (sliderRect.contains(point)) {
                return SC_ScrollBarSlider;
            } else if (preceeds(point, sliderRect, option)) {
                return SC_ScrollBarSubPage;
            } else {
                return SC_ScrollBarAddPage;
            }
        }

        // This is one of the up/down buttons. First, decide which one it is.
        if (preceeds(point, grooveRect, option)) {
            if (_subLineButtons == DoubleButton) {
                QRect buttonRect = scrollBarInternalSubControlRect(option, SC_ScrollBarSubLine);
                return scrollBarHitTest(buttonRect, point, option);
            } else {
                return SC_ScrollBarSubLine;
            }
        }

        if (_addLineButtons == DoubleButton) {
            QRect buttonRect = scrollBarInternalSubControlRect(option, SC_ScrollBarAddLine);
            return scrollBarHitTest(buttonRect, point, option);
        } else {
            return SC_ScrollBarAddLine;
        }
    }

    // fallback
    default:
        return ParentStyleClass::hitTestComplexControl(control, option, point, widget);
    }
}

//______________________________________________________________
void Style::drawPrimitive(PrimitiveElement element, const QStyleOption *option, QPainter *painter, const QWidget *widget) const
{
    StylePrimitive fcn(nullptr);
    switch (element) {
    case PE_PanelButtonCommand:
        fcn = &Style::drawPanelButtonCommandPrimitive;
        break;
    case PE_PanelButtonTool:
        fcn = &Style::drawPanelButtonToolPrimitive;
        break;
    case PE_PanelScrollAreaCorner:
        fcn = &Style::drawPanelScrollAreaCornerPrimitive;
        break;
    case PE_PanelMenu:
        fcn = &Style::drawPanelMenuPrimitive;
        break;
    case PE_PanelTipLabel:
        fcn = &Style::drawPanelTipLabelPrimitive;
        break;
    case PE_PanelItemViewRow:
        fcn = &Style::drawPanelItemViewRowPrimitive;
        break;
    case PE_PanelItemViewItem:
        fcn = &Style::drawPanelItemViewItemPrimitive;
        break;
    case PE_IndicatorCheckBox:
        fcn = &Style::drawIndicatorCheckBoxPrimitive;
        break;
    case PE_IndicatorRadioButton:
        fcn = &Style::drawIndicatorRadioButtonPrimitive;
        break;
    case PE_IndicatorButtonDropDown:
        fcn = &Style::drawIndicatorButtonDropDownPrimitive;
        break;
    case PE_IndicatorTabClose:
        fcn = &Style::drawIndicatorTabClosePrimitive;
        break;
    case PE_IndicatorTabTear:
        fcn = &Style::drawIndicatorTabTearPrimitive;
        break;
    case PE_IndicatorArrowUp:
        fcn = &Style::drawIndicatorArrowUpPrimitive;
        break;
    case PE_IndicatorArrowDown:
        fcn = &Style::drawIndicatorArrowDownPrimitive;
        break;
    case PE_IndicatorArrowLeft:
        fcn = &Style::drawIndicatorArrowLeftPrimitive;
        break;
    case PE_IndicatorArrowRight:
        fcn = &Style::drawIndicatorArrowRightPrimitive;
        break;
    case PE_IndicatorHeaderArrow:
        fcn = &Style::drawIndicatorHeaderArrowPrimitive;
        break;
    case PE_IndicatorToolBarHandle:
        fcn = &Style::drawIndicatorToolBarHandlePrimitive;
        break;
    case PE_IndicatorToolBarSeparator:
        fcn = &Style::drawIndicatorToolBarSeparatorPrimitive;
        break;
    case PE_IndicatorBranch:
        fcn = &Style::drawIndicatorBranchPrimitive;
        break;
    case PE_FrameStatusBarItem:
        fcn = &Style::emptyPrimitive;
        break;
    case PE_Frame:
        fcn = &Style::drawFramePrimitive;
        break;
    case PE_FrameLineEdit:
        fcn = &Style::drawFrameLineEditPrimitive;
        break;
    case PE_FrameMenu:
        fcn = &Style::drawFrameMenuPrimitive;
        break;
    case PE_FrameGroupBox:
        fcn = &Style::drawFrameGroupBoxPrimitive;
        break;
    case PE_FrameTabWidget:
        fcn = &Style::drawFrameTabWidgetPrimitive;
        break;
    case PE_FrameTabBarBase:
        fcn = &Style::drawFrameTabBarBasePrimitive;
        break;
    case PE_FrameWindow:
        fcn = &Style::drawFrameWindowPrimitive;
        break;
    case PE_FrameFocusRect:
        fcn = _frameFocusPrimitive;
        break;
    // fallback
    default:
        break;
    }

    painter->save();

    // call function if implemented
    if (!(fcn && (this->*fcn)(option, painter, widget))) {
        ParentStyleClass::drawPrimitive(element, option, painter, widget);
    }

    painter->restore();
}

//______________________________________________________________
void Style::drawControl(ControlElement element, const QStyleOption *option, QPainter *painter, const QWidget *widget) const
{
    StyleControl fcn(nullptr);
    switch (element) {
    case CE_PushButtonBevel:
        fcn = &Style::drawPanelButtonCommandPrimitive;
        break;
    case CE_PushButtonLabel:
        fcn = &Style::drawPushButtonLabelControl;
        break;
    case CE_CheckBoxLabel:
        fcn = &Style::drawCheckBoxLabelControl;
        break;
    case CE_RadioButtonLabel:
        fcn = &Style::drawCheckBoxLabelControl;
        break;
    case CE_ToolButtonLabel:
        fcn = &Style::drawToolButtonLabelControl;
        break;
    case CE_ComboBoxLabel:
        fcn = &Style::drawComboBoxLabelControl;
        break;
    case CE_MenuBarEmptyArea:
        fcn = &Style::drawMenuBarEmptyArea;
        break;
    case CE_MenuBarItem:
        fcn = &Style::drawMenuBarItemControl;
        break;
    case CE_MenuItem:
        fcn = &Style::drawMenuItemControl;
        break;
    case CE_ToolBar:
        fcn = &Style::emptyControl;
        break;
    case CE_ProgressBar:
        fcn = &Style::drawProgressBarControl;
        break;
    case CE_ProgressBarContents:
        fcn = &Style::drawProgressBarContentsControl;
        break;
    case CE_ProgressBarGroove:
        fcn = &Style::drawProgressBarGrooveControl;
        break;
    case CE_ProgressBarLabel:
        fcn = &Style::drawProgressBarLabelControl;
        break;
    case CE_ScrollBarSlider:
        fcn = &Style::drawScrollBarSliderControl;
        break;
    case CE_ScrollBarAddLine:
        fcn = &Style::drawScrollBarAddLineControl;
        break;
    case CE_ScrollBarSubLine:
        fcn = &Style::drawScrollBarSubLineControl;
        break;
    case CE_ScrollBarAddPage:
        fcn = &Style::emptyControl;
        break;
    case CE_ScrollBarSubPage:
        fcn = &Style::emptyControl;
        break;
    case CE_ShapedFrame:
        fcn = &Style::drawShapedFrameControl;
        break;
    case CE_RubberBand:
        fcn = &Style::drawRubberBandControl;
        break;
    case CE_SizeGrip:
        fcn = &Style::emptyControl;
        break;
    case CE_HeaderSection:
        fcn = &Style::drawHeaderSectionControl;
        break;
    case CE_HeaderLabel:
        fcn = &Style::drawHeaderLabelControl;
        break;
    case CE_HeaderEmptyArea:
        fcn = &Style::drawHeaderEmptyAreaControl;
        break;
    case CE_TabBarTabLabel:
        fcn = &Style::drawTabBarTabLabelControl;
        break;
    case CE_TabBarTabShape:
        fcn = &Style::drawTabBarTabShapeControl;
        break;
    case CE_ToolBoxTabLabel:
        fcn = &Style::drawToolBoxTabLabelControl;
        break;
    case CE_ToolBoxTabShape:
        fcn = &Style::drawToolBoxTabShapeControl;
        break;
    case CE_DockWidgetTitle:
        fcn = &Style::drawDockWidgetTitleControl;
        break;
    case CE_ItemViewItem:
        fcn = &Style::drawItemViewItemControl;
        break;
    // fallback
    default:
        break;
    }

    painter->save();

    // call function if implemented
    if (!(fcn && (this->*fcn)(option, painter, widget))) {
        ParentStyleClass::drawControl(element, option, painter, widget);
    }

    painter->restore();
}

//______________________________________________________________
void Style::drawComplexControl(ComplexControl element, const QStyleOptionComplex *option, QPainter *painter, const QWidget *widget) const
{
    StyleComplexControl fcn(nullptr);
    switch (element) {
    case CC_GroupBox:
        fcn = &Style::drawGroupBoxComplexControl;
        break;
    case CC_ToolButton:
        fcn = &Style::drawToolButtonComplexControl;
        break;
    case CC_ComboBox:
        fcn = &Style::drawComboBoxComplexControl;
        break;
    case CC_SpinBox:
        fcn = &Style::drawSpinBoxComplexControl;
        break;
    case CC_Slider:
        fcn = &Style::drawSliderComplexControl;
        break;
    case CC_Dial:
        fcn = &Style::drawDialComplexControl;
        break;
    case CC_ScrollBar:
        fcn = &Style::drawScrollBarComplexControl;
        break;
    case CC_TitleBar:
        fcn = &Style::drawTitleBarComplexControl;
        break;

    // fallback
    default:
        break;
    }

    painter->save();

    // call function if implemented
    if (!(fcn && (this->*fcn)(option, painter, widget))) {
        ParentStyleClass::drawComplexControl(element, option, painter, widget);
    }

    painter->restore();
}

//___________________________________________________________________________________
void Style::drawItemText(QPainter *painter, const QRect &rect, int flags, const QPalette &palette, bool enabled,
                         const QString &text, QPalette::ColorRole textRole) const
{
    // hide mnemonics if requested
    if (!_mnemonics->enabled() && (flags & Qt::TextShowMnemonic) && !(flags & Qt::TextHideMnemonic)) {
        flags &= ~Qt::TextShowMnemonic;
        flags |= Qt::TextHideMnemonic;
    }

    // make sure vertical alignment is defined
    // fallback on Align::VCenter if not
    if (!(flags & Qt::AlignVertical_Mask)) {
        flags |= Qt::AlignVCenter;
    }

    if (_animations->widgetEnabilityEngine().enabled()) {
        /*
         * check if painter engine is registered to WidgetEnabilityEngine, and animated
         * if yes, merge the palettes. Note: a static_cast is safe here, since only the address
         * of the pointer is used, not the actual content.
         */
        const QWidget *widget(static_cast<const QWidget *>(painter->device()));
        if (_animations->widgetEnabilityEngine().isAnimated(widget, AnimationEnable)) {
            QPalette copy(Colors::disabledPalette(palette, _animations->widgetEnabilityEngine().opacity(widget, AnimationEnable)));
            return ParentStyleClass::drawItemText(painter, rect, flags, copy, enabled, text, textRole);
        }
    }

    // fallback
    return ParentStyleClass::drawItemText(painter, rect, flags, palette, enabled, text, textRole);
}

//_____________________________________________________________________
bool Style::eventFilter(QObject *object, QEvent *event)
{
    if (QDockWidget *dockWidget = qobject_cast<QDockWidget *>(object)) {
        return eventFilterDockWidget(dockWidget, event);
    } else if (QMdiSubWindow *subWindow = qobject_cast<QMdiSubWindow *>(object)) {
        return eventFilterMdiSubWindow(subWindow, event);
    } else if (QCommandLinkButton *commandLinkButton = qobject_cast<QCommandLinkButton *>(object)) {
        return eventFilterCommandLinkButton(commandLinkButton, event);
    }

    // cast to QWidget
    QWidget *widget = static_cast<QWidget *>(object);
    if (widget->inherits("QAbstractScrollArea") || widget->inherits("KTextEditor::View")) {
        return eventFilterScrollArea(widget, event);
    } else if (widget->inherits("QComboBoxPrivateContainer")) {
        return eventFilterComboBoxContainer(widget, event);
    }

    if (QEvent::PaletteChange == event->type()) {
        configurationChanged();
    }

    if ((!widget->parent() || !qobject_cast<QWidget *>(widget->parent()) || qobject_cast<QDialog *>(widget) || qobject_cast<QMainWindow *>(widget))
            && (event->type() == QEvent::Show || event->type() == QEvent::StyleChange)) {
        configurationChanged();
    }

    // fallback
    return ParentStyleClass::eventFilter(object, event);
}

//____________________________________________________________________________
bool Style::eventFilterScrollArea(QWidget *widget, QEvent *event)
{
    switch (event->type()) {
    case QEvent::Paint: {
        // get scrollarea viewport
        QAbstractScrollArea *scrollArea(qobject_cast<QAbstractScrollArea *>(widget));
        QWidget *viewport;
        if (!(scrollArea && (viewport = scrollArea->viewport()))) {
            break;
        }

        // get scrollarea horizontal and vertical containers
        QWidget *child(nullptr);
        QList<QWidget *> children;
        if ((child = scrollArea->findChild<QWidget *>("qt_scrollarea_vcontainer")) && child->isVisible()) {
            children.append(child);
        }

        if ((child = scrollArea->findChild<QWidget *>("qt_scrollarea_hcontainer")) && child->isVisible()) {
            children.append(child);
        }

        if (children.empty()) {
            break;
        }

        if (!scrollArea->styleSheet().isEmpty()) {
            break;
        }

        // make sure proper background is rendered behind the containers
        QPainter painter(scrollArea);
        painter.setClipRegion(static_cast<QPaintEvent *>(event)->region());

        painter.setPen(Qt::NoPen);

        // decide background color
        const QPalette::ColorRole role(viewport->backgroundRole());
        QColor background;
        if (role == QPalette::Window && hasAlteredBackground(viewport)) {
            background = Colors::frameBackgroundColor(StyleOptions(viewport->palette(), _variant));
        } else {
            background = viewport->palette().color(role);
        }
        painter.setBrush(background);

        // render
        foreach (auto *child, children) {
            painter.drawRect(child->geometry());
        }

        break;
    }

    case QEvent::MouseButtonPress:
    case QEvent::MouseButtonRelease:
    case QEvent::MouseMove: {
        // case event
        QMouseEvent *mouseEvent(static_cast<QMouseEvent *>(event));

        // get frame framewidth
        int frameWidth(pixelMetric(PM_DefaultFrameWidth, 0, widget));

        // find list of scrollbars
        QList<QScrollBar *> scrollBars;
        if (QAbstractScrollArea *scrollArea = qobject_cast<QAbstractScrollArea *>(widget)) {
            if (scrollArea->horizontalScrollBarPolicy() != Qt::ScrollBarAlwaysOff) {
                scrollBars.append(scrollArea->horizontalScrollBar());
            }
            if (scrollArea->verticalScrollBarPolicy() != Qt::ScrollBarAlwaysOff) {
                scrollBars.append(scrollArea->verticalScrollBar());
            }
        } else if (widget->inherits("KTextEditor::View")) {
            scrollBars = widget->findChildren<QScrollBar *>();
        }

        // loop over found scrollbars
        foreach (QScrollBar *scrollBar, scrollBars) {
            if (!(scrollBar && scrollBar->isVisible())) {
                continue;
            }

            QPoint offset;
            if (scrollBar->orientation() == Qt::Horizontal) {
                offset = QPoint(0, frameWidth);
            } else {
                offset = QPoint(QApplication::isLeftToRight() ? frameWidth : -frameWidth, 0);
            }

            // map position to scrollarea
#if QT_VERSION >= 0x060000
            QPoint position(scrollBar->mapFrom(widget, mouseEvent->position().toPoint() - offset));
#else
            QPoint position(scrollBar->mapFrom(widget, mouseEvent->pos() - offset));
#endif

            // check if contains
            if (!scrollBar->rect().contains(position)) {
                continue;
            }

            // copy event, send and return
            QMouseEvent copy(mouseEvent->type(), position, scrollBar->mapToGlobal(position),
                             mouseEvent->button(), mouseEvent->buttons(), mouseEvent->modifiers());

            QCoreApplication::sendEvent(scrollBar, &copy);
            event->setAccepted(true);
            return true;
        }

        break;
    }

    default:
        break;
    }

    return  ParentStyleClass::eventFilter(widget, event);
}

//_________________________________________________________
bool Style::eventFilterComboBoxContainer(QWidget *widget, QEvent *event)
{
    if (event->type() == QEvent::Paint) {
        QPainter painter(widget);
        QPaintEvent *paintEvent = static_cast<QPaintEvent *>(event);
        painter.setClipRegion(paintEvent->region());

        QRect rect(widget->rect());
        const QPalette &palette(widget->palette());

        bool hasAlpha(_helper->hasAlphaChannel(widget));
        if (hasAlpha) {
            painter.setCompositionMode(QPainter::CompositionMode_Source);
        }

        StyleOptions styleOptions(&painter, rect);
        styleOptions.setColorVariant(_variant);
        styleOptions.setColor(Colors::frameBackgroundColor(StyleOptions(palette, _variant)));
        styleOptions.setOutlineColor(Colors::frameOutlineColor(StyleOptions(palette, _variant)));

        Adwaita::Renderer::renderMenuFrame(styleOptions, hasAlpha);
    }

    return false;
}

//____________________________________________________________________________
bool Style::eventFilterDockWidget(QDockWidget *dockWidget, QEvent *event)
{
    if (event->type() == QEvent::Paint) {
        // create painter and clip
        QPainter painter(dockWidget);
        QPaintEvent *paintEvent = static_cast<QPaintEvent *>(event);
        painter.setClipRegion(paintEvent->region());

        // store palette and set colors
        const QPalette &palette(dockWidget->palette());
        QColor background(Colors::frameBackgroundColor(StyleOptions(palette, _variant)));
        QColor outline(Colors::frameOutlineColor(StyleOptions(palette, _variant)));

        // store rect
        QRect rect(dockWidget->rect());

        // render
        StyleOptions styleOptions(&painter, rect);
        styleOptions.setColorVariant(_variant);
        styleOptions.setColor(background);
        styleOptions.setOutlineColor(outline);
        if (dockWidget->isFloating()) {
            Adwaita::Renderer::renderMenuFrame(styleOptions, false);
        } else if (Adwaita::Config::DockWidgetDrawFrame || (dockWidget->features() & (QDockWidget::DockWidgetClosable|QDockWidget::DockWidgetMovable|QDockWidget::DockWidgetFloatable))) {
            Adwaita::Renderer::renderFrame(styleOptions);
        }
    }

    return false;
}

//____________________________________________________________________________
bool Style::eventFilterMdiSubWindow(QMdiSubWindow *subWindow, QEvent *event)
{
    if (event->type() == QEvent::Paint) {
        QPainter painter(subWindow);
        QPaintEvent *paintEvent(static_cast<QPaintEvent *>(event));
        painter.setClipRegion(paintEvent->region());

        QRect rect(subWindow->rect());
        QColor background(subWindow->palette().color(QPalette::Window));

        if (subWindow->isMaximized()) {
            // full painting
            painter.setPen(Qt::NoPen);
            painter.setBrush(background);
            painter.drawRect(rect);
        } else {
            // framed painting
            StyleOptions styleOptions(&painter, rect);
            styleOptions.setColor(background);
            styleOptions.setColorVariant(_variant);
            Adwaita::Renderer::renderMenuFrame(styleOptions);
        }
    }

    // continue with normal painting
    return false;
}

//____________________________________________________________________________
bool Style::eventFilterCommandLinkButton(QCommandLinkButton *button, QEvent *event)
{
    if (event->type() == QEvent::Paint) {
        // painter
        QPainter painter(button);
        painter.setClipRegion(static_cast<QPaintEvent *>(event)->region());

        bool isFlat = false;

        // option
        QStyleOptionButton option;
        option.initFrom(button);
        option.features |= QStyleOptionButton::CommandLinkButton;
        if (isFlat) {
            option.features |= QStyleOptionButton::Flat;
        }
        option.text = QString();
        option.icon = QIcon();

        if (button->isChecked()) {
            option.state |= State_On;
        }
        if (button->isDown()) {
            option.state |= State_Sunken;
        }
        // frame
        drawControl(QStyle::CE_PushButton, &option, &painter, button);

        // offset
        int margin(Metrics::Button_MarginWidth + Metrics::Frame_FrameWidth);
        QPoint offset(margin, margin);

        if (button->isDown() && !isFlat)
            painter.translate(1, 1);
        {
            offset += QPoint(1, 1);
        }

        // state
        const State &state(option.state);
        bool enabled(state & State_Enabled);
        bool mouseOver((state & State_Active) && enabled && (state & State_MouseOver));
        bool hasFocus(enabled && (state & State_HasFocus));

        // icon
        if (!button->icon().isNull()) {
            QSize pixmapSize(button->icon().actualSize(button->iconSize()));
            QRect pixmapRect(QPoint(offset.x(), button->description().isEmpty() ? (button->height() - pixmapSize.height()) / 2 : offset.y()), pixmapSize);
            const QPixmap pixmap(button->icon().pixmap(pixmapSize,
                                 enabled ? QIcon::Normal : QIcon::Disabled,
                                 button->isChecked() ? QIcon::On : QIcon::Off));
            drawItemPixmap(&painter, pixmapRect, Qt::AlignCenter, pixmap);

            offset.rx() += pixmapSize.width() + Metrics::Button_ItemSpacing;
        }

        // text rect
        QRect textRect(offset, QSize(button->size().width() - offset.x() - margin, button->size().height() - 2 * margin));
        const QPalette::ColorRole textRole = (enabled && hasFocus && !mouseOver && !isFlat) ? QPalette::HighlightedText : QPalette::ButtonText;
        if (!button->text().isEmpty()) {
            QFont font(button->font());
            font.setBold(true);
            painter.setFont(font);
            if (button->description().isEmpty()) {
                drawItemText(&painter, textRect, Qt::AlignLeft | Qt::AlignVCenter | Qt::TextHideMnemonic, button->palette(), enabled, button->text(), textRole);
            } else {
                drawItemText(&painter, textRect, Qt::AlignLeft | Qt::AlignTop | Qt::TextHideMnemonic, button->palette(), enabled, button->text(), textRole);
                textRect.setTop(textRect.top() + QFontMetrics(font).height());
            }

            painter.setFont(button->font());
        }

        if (!button->description().isEmpty()) {
            drawItemText(&painter, textRect, Qt::AlignLeft | Qt::AlignVCenter | Qt::TextWordWrap, button->palette(), enabled, button->description(), textRole);
        }

        return true;
    }

    // continue with normal painting
    return false;
}

//_____________________________________________________________________
void Style::configurationChanged(void)
{
    // reload configuration
    loadConfiguration();
}

//____________________________________________________________________
QIcon Style::standardIconImplementation(StandardPixmap standardPixmap, const QStyleOption *option, const QWidget *widget) const
{
    // lookup cache
    if (_iconCache.contains(standardPixmap)) {
        return _iconCache.value(standardPixmap);
    }

    QIcon icon;
    switch (standardPixmap) {
    case SP_TitleBarNormalButton:
    case SP_TitleBarMinButton:
    case SP_TitleBarMaxButton:
    case SP_TitleBarCloseButton:
    case SP_DockWidgetCloseButton:
        icon = titleBarButtonIcon(standardPixmap, option, widget);
        break;
    case SP_ToolBarHorizontalExtensionButton:
    case SP_ToolBarVerticalExtensionButton:
        icon = toolBarExtensionIcon(standardPixmap, option, widget);
        break;
    default:
        break;
    }

    if (icon.isNull()) {
        // do not cache parent style icon, since it may change at runtime
        return  ParentStyleClass::standardIcon(standardPixmap, option, widget);
    } else {
        const_cast<IconCache *>(&_iconCache)->insert(standardPixmap, icon);
        return icon;
    }
}

//_____________________________________________________________________
void Style::loadConfiguration()
{
    // reinitialize engines
    _animations->setupEngines();
    _windowManager->initialize();

    // mnemonics
    _mnemonics->setMode(Adwaita::Config::MnemonicsMode);

    // splitter proxy
    _splitterFactory->setEnabled(Adwaita::Config::SplitterProxyEnabled);

    // clear icon cache
    _iconCache.clear();

    // scrollbar buttons
    switch (Adwaita::Config::ScrollBarAddLineButtons) {
    case 0:
        _addLineButtons = NoButton;
        break;
    case 1:
        _addLineButtons = SingleButton;
        break;

    default:
    case 2:
        _addLineButtons = DoubleButton;
        break;
    }

    switch (Adwaita::Config::ScrollBarSubLineButtons) {
    case 0:
        _subLineButtons = NoButton;
        break;
    case 1:
        _subLineButtons = SingleButton;
        break;

    default:
    case 2:
        _subLineButtons = DoubleButton;
        break;
    }

    // frame focus
    if (Adwaita::Config::ViewDrawFocusIndicator) {
        _frameFocusPrimitive = &Style::drawFrameFocusRectPrimitive;
    } else {
        _frameFocusPrimitive = &Style::emptyPrimitive;
    }

    // widget explorer
    _widgetExplorer->setEnabled(Adwaita::Config::WidgetExplorerEnabled);
    _widgetExplorer->setDrawWidgetRects(Adwaita::Config::DrawWidgetRects);
}

//___________________________________________________________________________________________________________________
QRect Style::pushButtonContentsRect(const QStyleOption *option, const QWidget *) const
{
    return insideMargin(option->rect, Metrics::Frame_FrameWidth);
}

//___________________________________________________________________________________________________________________
QRect Style::pushButtonFocusRect(const QStyleOption *option, const QWidget *) const
{
    return insideMargin(option->rect, 3);
}

//___________________________________________________________________________________________________________________
QRect Style::checkBoxContentsRect(const QStyleOption *option, const QWidget *) const
{
    return visualRect(option, option->rect.adjusted(Metrics::CheckBox_Size + Metrics::CheckBox_ItemSpacing, 0, 0, 0));
}

//___________________________________________________________________________________________________________________
QRect Style::checkBoxIndicatorRect(const QStyleOption *option, const QWidget *widget) const
{
    return ParentStyleClass::subElementRect(SE_CheckBoxIndicator, option, widget);
}

//___________________________________________________________________________________________________________________
QRect Style::checkBoxFocusRect(const QStyleOption *option, const QWidget *widget) const
{
    return QRect(option->rect.left() + 2, option->rect.top() + 1,
                 ParentStyleClass::subElementRect(SE_CheckBoxFocusRect, option, widget).right() - option->rect.left(),
                 option->rect.height() - 2);
}

//___________________________________________________________________________________________________________________
QRect Style::lineEditContentsRect(const QStyleOption *option, const QWidget *widget) const
{
    // cast option and check
    const QStyleOptionFrame *frameOption(qstyleoption_cast<const QStyleOptionFrame *>(option));
    if (!frameOption) {
        return option->rect;
    }

    // check flatness
    bool flat(frameOption->lineWidth == 0);
    if (flat) {
        return option->rect;
    }

    // copy rect and take out margins
    QRect rect(option->rect);

    // take out margins if there is enough room
    int frameWidth(pixelMetric(PM_DefaultFrameWidth, option, widget));
    if (rect.height() >= option->fontMetrics.height() + 2 * frameWidth) {
        return insideMargin(rect, frameWidth);
    } else {
        return rect;
    }
}

//___________________________________________________________________________________________________________________
QRect Style::progressBarGrooveRect(const QStyleOption *option, const QWidget *widget) const
{
    // cast option and check
    const QStyleOptionProgressBar *progressBarOption(qstyleoption_cast<const QStyleOptionProgressBar *>(option));
    if (!progressBarOption) {
        return option->rect;
    }

    // get flags and orientation
    bool textVisible(progressBarOption->textVisible);
    bool busy(progressBarOption->minimum == 0 && progressBarOption->maximum == 0);

    const State &state(option->state);
#if QT_VERSION >= 0x060000
    bool horizontal(state & QStyle::State_Horizontal);
#else
    bool horizontal((state & QStyle::State_Horizontal) || (progressBarOption->orientation == Qt::Horizontal));
#endif

    // copy rectangle and adjust
    QRect rect(option->rect);
    int frameWidth(pixelMetric(PM_DefaultFrameWidth, option, widget));
    if (horizontal) {
        rect = insideMargin(rect, frameWidth, 0);
    } else {
        rect = insideMargin(rect, 0, frameWidth);
    }

    if (textVisible && !busy && horizontal) {
        QRect textRect(subElementRect(SE_ProgressBarLabel, option, widget));
        textRect = visualRect(option, textRect);
        rect.setRight(textRect.left() - Metrics::ProgressBar_ItemSpacing - 1);
        rect = visualRect(option, rect);
        rect = centerRect(rect, rect.width(), Metrics::ProgressBar_Thickness);
    } else if (horizontal) {
        rect = centerRect(rect, rect.width(), Metrics::ProgressBar_Thickness);
    } else {
        rect = centerRect(rect, Metrics::ProgressBar_Thickness, rect.height());
    }

    return rect;
}

//___________________________________________________________________________________________________________________
QRect Style::progressBarContentsRect(const QStyleOption *option, const QWidget *widget) const
{
    // cast option and check
    const QStyleOptionProgressBar *progressBarOption(qstyleoption_cast<const QStyleOptionProgressBar *>(option));
    if (!progressBarOption) {
        return QRect();
    }

    // get groove rect
    QRect rect(progressBarGrooveRect(option, widget));

    // in busy mode, grooveRect is used
    bool busy(progressBarOption->minimum == 0 && progressBarOption->maximum == 0);
    if (busy) {
        return rect;
    }

    // get orientation
    const State &state(option->state);
#if QT_VERSION >= 0x060000
    bool horizontal(state & QStyle::State_Horizontal);
#else
    bool horizontal((state & QStyle::State_Horizontal) || (progressBarOption->orientation == Qt::Horizontal));
#endif

    // check inverted appearance
    bool inverted(progressBarOption ? progressBarOption->invertedAppearance : false);

    // get progress and steps
    qreal progress(progressBarOption->progress - progressBarOption->minimum);
    int steps(qMax(progressBarOption->maximum  - progressBarOption->minimum, 1));

    //Calculate width fraction
    qreal widthFrac = qMin(qreal(1), progress / steps);

    // convert the pixel width
    int indicatorSize(widthFrac * (horizontal ? rect.width() : rect.height()));

    QRect indicatorRect;
    if (horizontal) {
        indicatorRect = QRect(inverted ? (rect.right() - indicatorSize + 1) : rect.left(), rect.y(), indicatorSize, rect.height());
        indicatorRect = visualRect(option->direction, rect, indicatorRect);
    } else {
        indicatorRect = QRect(rect.x(), inverted ? rect.top() : (rect.bottom() - indicatorSize + 1), rect.width(), indicatorSize);
    }

    return indicatorRect;
}

//___________________________________________________________________________________________________________________
QRect Style::progressBarLabelRect(const QStyleOption *option, const QWidget *) const
{
    // cast option and check
    const QStyleOptionProgressBar *progressBarOption(qstyleoption_cast<const QStyleOptionProgressBar *>(option));
    if (!progressBarOption) {
        return QRect();
    }

    // get flags and check
    bool textVisible(progressBarOption->textVisible);
    bool busy(progressBarOption->minimum == 0 && progressBarOption->maximum == 0);
    if (!textVisible || busy) {
        return QRect();
    }

    // get orientation
    const State &state(option->state);
#if QT_VERSION >= 0x060000
    bool horizontal(state & QStyle::State_Horizontal);
#else
    bool horizontal((state & QStyle::State_Horizontal) || (progressBarOption->orientation == Qt::Horizontal));
#endif

    if (!horizontal) {
        return QRect();
    }

    int textWidth = qMax(option->fontMetrics.size(_mnemonics->textFlags(), progressBarOption->text).width(),
                         option->fontMetrics.size(_mnemonics->textFlags(), QStringLiteral("100%")).width());

    QRect rect(insideMargin(option->rect, Metrics::Frame_FrameWidth, 0));
    rect.setLeft(rect.right() - textWidth + 1);
    rect = visualRect(option, rect);

    return rect;
}

//___________________________________________________________________________________________________________________
QRect Style::headerArrowRect(const QStyleOption *option, const QWidget *) const
{
    // cast option and check
    const QStyleOptionHeader *headerOption(qstyleoption_cast<const QStyleOptionHeader *>(option));
    if (!headerOption) {
        return option->rect;
    }

    // check if arrow is necessary
    if (headerOption->sortIndicator == QStyleOptionHeader::None) {
        return QRect();
    }

    QRect arrowRect(insideMargin(option->rect, Metrics::Header_MarginWidth));
    arrowRect.setLeft(arrowRect.right() - Metrics::Header_ArrowSize + 1);

    return visualRect(option, arrowRect);
}

//___________________________________________________________________________________________________________________
QRect Style::headerLabelRect(const QStyleOption *option, const QWidget *) const
{
    // cast option and check
    const QStyleOptionHeader *headerOption(qstyleoption_cast<const QStyleOptionHeader *>(option));
    if (!headerOption) {
        return option->rect;
    }

    // check if arrow is necessary
    // QRect labelRect( insideMargin( option->rect, Metrics::Header_MarginWidth ) );
    QRect labelRect(insideMargin(option->rect, Metrics::Header_MarginWidth, 0));
    if (headerOption->sortIndicator == QStyleOptionHeader::None) {
        return labelRect;
    }

    labelRect.adjust(0, 0, -Metrics::Header_ArrowSize - Metrics::Header_ItemSpacing, 0);
    return visualRect(option, labelRect);
}

//___________________________________________________________________________________________________________________
QRect Style::sliderFocusRect(const QStyleOption *option, const QWidget *widget) const
{
    const QStyleOptionSlider *sliderOption(qstyleoption_cast<const QStyleOptionSlider *>(option));

    QRect r(option->rect);

    if (sliderOption->orientation == Qt::Vertical) {
        int thickness = Slider_GrooveThickness + 8;
        return QRect(r.center().x() - thickness / 2, r.top() + 1, thickness + 1, r.height() - 1);
    } else {
        int thickness = Slider_GrooveThickness + 6;
        return QRect(r.left() + 1, r.center().y() - thickness / 2, r.width() - 1, thickness + 1);
    }
}

//____________________________________________________________________
QRect Style::tabBarTabLeftButtonRect(const QStyleOption *option, const QWidget *) const
{
    // cast option and check
    const QStyleOptionTab *tabOption(qstyleoption_cast<const QStyleOptionTab *>(option));
    if (!tabOption || tabOption->leftButtonSize.isEmpty()) {
        return QRect();
    }

    QRect rect(option->rect);
    QSize size(tabOption->leftButtonSize);
    QRect buttonRect(QPoint(0, 0), size);

    // vertical positioning
    switch (tabOption->shape) {
    case QTabBar::RoundedNorth:
    case QTabBar::TriangularNorth:

    case QTabBar::RoundedSouth:
    case QTabBar::TriangularSouth:
        buttonRect.moveLeft(rect.left() + Metrics::TabBar_TabMarginWidth);
        buttonRect.moveTop((rect.height() - buttonRect.height()) / 2);
        buttonRect = visualRect(option, buttonRect);
        break;
    case QTabBar::RoundedWest:
    case QTabBar::TriangularWest:
        buttonRect.moveBottom(rect.bottom() - Metrics::TabBar_TabMarginWidth);
        buttonRect.moveLeft((rect.width() - buttonRect.width()) / 2);
        break;
    case QTabBar::RoundedEast:
    case QTabBar::TriangularEast:
        buttonRect.moveTop(rect.top() + Metrics::TabBar_TabMarginWidth);
        buttonRect.moveLeft((rect.width() - buttonRect.width()) / 2);
        break;
    default:
        break;
    }

    return buttonRect;
}

//____________________________________________________________________
QRect Style::tabBarTabRightButtonRect(const QStyleOption *option, const QWidget *) const
{
    // cast option and check
    const QStyleOptionTab *tabOption(qstyleoption_cast<const QStyleOptionTab *>(option));
    if (!tabOption || tabOption->rightButtonSize.isEmpty()) {
        return QRect();
    }

    QRect rect(option->rect);
    QSize size(tabOption->rightButtonSize);
    QRect buttonRect(QPoint(0, 0), size);

    // vertical positioning
    switch (tabOption->shape) {
    case QTabBar::RoundedNorth:
    case QTabBar::TriangularNorth:

    case QTabBar::RoundedSouth:
    case QTabBar::TriangularSouth:
        buttonRect.moveRight(rect.right() - Metrics::TabBar_TabMarginWidth);
        buttonRect.moveTop((rect.height() - buttonRect.height()) / 2);
        buttonRect = visualRect(option, buttonRect);
        break;
    case QTabBar::RoundedWest:
    case QTabBar::TriangularWest:
        buttonRect.moveTop(rect.top() + Metrics::TabBar_TabMarginWidth);
        buttonRect.moveLeft((rect.width() - buttonRect.width()) / 2);
        break;
    case QTabBar::RoundedEast:
    case QTabBar::TriangularEast:
        buttonRect.moveBottom(rect.bottom() - Metrics::TabBar_TabMarginWidth);
        buttonRect.moveLeft((rect.width() - buttonRect.width()) / 2);
        break;
    default:
        break;
    }

    return buttonRect;
}

//____________________________________________________________________
QRect Style::tabWidgetTabBarRect(const QStyleOption *option, const QWidget *widget) const
{
    // cast option and check
    const QStyleOptionTabWidgetFrame *tabOption = qstyleoption_cast<const QStyleOptionTabWidgetFrame *>(option);
    if (!tabOption) {
        return ParentStyleClass::subElementRect(SE_TabWidgetTabBar, option, widget);
    }

    // do nothing if tabbar is hidden
    QSize tabBarSize(tabOption->tabBarSize);

    QRect rect(option->rect);
    QRect tabBarRect(QPoint(0, 0), tabBarSize);

    Qt::Alignment tabBarAlignment(styleHint(SH_TabBar_Alignment, option, widget));

    // horizontal positioning
    bool verticalTabs(isVerticalTab(tabOption->shape));
    if (verticalTabs) {
        tabBarRect.setHeight(qMin(tabBarRect.height(), rect.height() - 2));
        if (tabBarAlignment == Qt::AlignCenter) {
            tabBarRect.moveTop(rect.top() + (rect.height() - tabBarRect.height()) / 2);
        } else {
            tabBarRect.moveTop(rect.top() + 1);
        }
    } else {
        // account for corner rects
        // need to re-run visualRect to remove right-to-left handling, since it is re-added on tabBarRect at the end
        QRect leftButtonRect(visualRect(option, subElementRect(SE_TabWidgetLeftCorner, option, widget)));
        QRect rightButtonRect(visualRect(option, subElementRect(SE_TabWidgetRightCorner, option, widget)));

        rect.setLeft(leftButtonRect.width());
        rect.setRight(rightButtonRect.left() - 1);

        tabBarRect.setWidth(qMin(tabBarRect.width(), rect.width() - 2));

        if (tabBarAlignment == Qt::AlignCenter) {
            tabBarRect.moveLeft(rect.left() + (rect.width() - tabBarRect.width()) / 2);
        } else {
            tabBarRect.moveLeft(rect.left() + 1);
        }

        tabBarRect = visualRect(option, tabBarRect);
    }

    // vertical positioning
    switch (tabOption->shape) {
    case QTabBar::RoundedNorth:
    case QTabBar::TriangularNorth:
        tabBarRect.moveTop(rect.top() + 1);
        break;
    case QTabBar::RoundedSouth:
    case QTabBar::TriangularSouth:
        tabBarRect.moveBottom(rect.bottom() - 1);
        break;
    case QTabBar::RoundedWest:
    case QTabBar::TriangularWest:
        tabBarRect.moveLeft(rect.left() + 1);
        break;
    case QTabBar::RoundedEast:
    case QTabBar::TriangularEast:
        tabBarRect.moveRight(rect.right() - 1);
        break;
    default:
        break;
    }

    return tabBarRect;
}

//____________________________________________________________________
QRect Style::tabWidgetTabContentsRect(const QStyleOption *option, const QWidget *widget) const
{
    // cast option and check
    const QStyleOptionTabWidgetFrame *tabOption = qstyleoption_cast<const QStyleOptionTabWidgetFrame *>(option);
    if (!tabOption) {
        return option->rect;
    }

    // do nothing if tabbar is hidden
    if (tabOption->tabBarSize.isEmpty()) {
        return option->rect;
    }
    QRect rect = tabWidgetTabPaneRect(option, widget);

    bool documentMode(tabOption->lineWidth == 0);
    if (documentMode) {
        // add margin only to the relevant side
        switch (tabOption->shape) {
        case QTabBar::RoundedNorth:
        case QTabBar::TriangularNorth:
            return rect.adjusted(0, Metrics::TabWidget_MarginWidth, 0, 0);
        case QTabBar::RoundedSouth:
        case QTabBar::TriangularSouth:
            return rect.adjusted(0, 0, 0, -Metrics::TabWidget_MarginWidth);
        case QTabBar::RoundedWest:
        case QTabBar::TriangularWest:
            return rect.adjusted(Metrics::TabWidget_MarginWidth, 0, 0, 0);
        case QTabBar::RoundedEast:
        case QTabBar::TriangularEast:
            return rect.adjusted(0, 0, -Metrics::TabWidget_MarginWidth, 0);
        default:
            return rect;
        }
    } else {
        return insideMargin(rect, Metrics::TabWidget_MarginWidth);
    }
}

//____________________________________________________________________
QRect Style::tabWidgetTabPaneRect(const QStyleOption *option, const QWidget *) const
{
    const QStyleOptionTabWidgetFrame *tabOption = qstyleoption_cast<const QStyleOptionTabWidgetFrame *>(option);
    if (!tabOption || tabOption->tabBarSize.isEmpty()) {
        return option->rect;
    }

    int overlap = Metrics::TabBar_BaseOverlap + 1;
    QSize tabBarSize(tabOption->tabBarSize - QSize(overlap, overlap));

    QRect rect(option->rect);
    switch (tabOption->shape) {
    case QTabBar::RoundedNorth:
    case QTabBar::TriangularNorth:
        rect.adjust(0, tabBarSize.height(), 0, 0);
        break;
    case QTabBar::RoundedSouth:
    case QTabBar::TriangularSouth:
        rect.adjust(0, 0, 0, -tabBarSize.height());
        break;
    case QTabBar::RoundedWest:
    case QTabBar::TriangularWest:
        rect.adjust(tabBarSize.width(), 0, 0, 0);
        break;
    case QTabBar::RoundedEast:
    case QTabBar::TriangularEast:
        rect.adjust(0, 0, -tabBarSize.width(), 0);
        break;
    default:
        return QRect();
    }

    return rect;
}

//____________________________________________________________________
QRect Style::tabWidgetCornerRect(SubElement element, const QStyleOption *option, const QWidget *) const
{
    // cast option and check
    const QStyleOptionTabWidgetFrame *tabOption = qstyleoption_cast<const QStyleOptionTabWidgetFrame *>(option);
    if (!tabOption) {
        return option->rect;
    }

    // do nothing if tabbar is hidden
    QSize tabBarSize(tabOption->tabBarSize);
    if (tabBarSize.isEmpty()) {
        return QRect();
    }

    // do nothing for vertical tabs
    bool verticalTabs(isVerticalTab(tabOption->shape));
    if (verticalTabs) {
        return QRect();
    }

    QRect rect(option->rect);
    QRect cornerRect;
    switch (element) {
    case SE_TabWidgetLeftCorner:
        cornerRect = QRect(QPoint(0, 0), tabOption->leftCornerWidgetSize);
        cornerRect.moveLeft(rect.left());
        break;
    case SE_TabWidgetRightCorner:
        cornerRect = QRect(QPoint(0, 0), tabOption->rightCornerWidgetSize);
        cornerRect.moveRight(rect.right());
        break;
    default:
        break;
    }

    // expend height to tabBarSize, if needed, to make sure base is properly rendered
    cornerRect.setHeight(qMax(cornerRect.height(), tabBarSize.height() + 1));

    switch (tabOption->shape) {
    case QTabBar::RoundedNorth:
    case QTabBar::TriangularNorth:
        cornerRect.moveTop(rect.top());
        break;
    case QTabBar::RoundedSouth:
    case QTabBar::TriangularSouth:
        cornerRect.moveBottom(rect.bottom());
        break;
    default:
        break;
    }

    // return cornerRect;
    cornerRect = visualRect(option, cornerRect);
    return cornerRect;
}

//____________________________________________________________________
QRect Style::toolBoxTabContentsRect(const QStyleOption *option, const QWidget *widget) const
{
    // cast option and check
    const QStyleOptionToolBox *toolBoxOption(qstyleoption_cast<const QStyleOptionToolBox *>(option));
    if (!toolBoxOption) {
        return option->rect;
    }

    // copy rect
    const QRect &rect(option->rect);

    int contentsWidth(0);
    if (!toolBoxOption->icon.isNull()) {
        int iconSize(pixelMetric(QStyle::PM_SmallIconSize, option, widget));
        contentsWidth += iconSize;

        if (!toolBoxOption->text.isEmpty()) {
            contentsWidth += Metrics::ToolBox_TabItemSpacing;
        }
    }

    if (!toolBoxOption->text.isEmpty()) {
        int textWidth = toolBoxOption->fontMetrics.size(_mnemonics->textFlags(), toolBoxOption->text).width();
        contentsWidth += textWidth;
    }

    contentsWidth += 2 * Metrics::ToolBox_TabMarginWidth;
    contentsWidth = qMin(contentsWidth, rect.width());
    contentsWidth = qMax(contentsWidth, int(Metrics::ToolBox_TabMinWidth));
    return centerRect(rect, contentsWidth, rect.height());
}

//____________________________________________________________________
QRect Style::genericLayoutItemRect(const QStyleOption *option, const QWidget *widget) const
{
    Q_UNUSED(widget)
    return insideMargin(option->rect, -Metrics::Frame_FrameWidth);
}

//______________________________________________________________
QRect Style::groupBoxSubControlRect(const QStyleOptionComplex *option, SubControl subControl, const QWidget *widget) const
{
    if (const QStyleOptionGroupBox *groupBox = qstyleoption_cast<const QStyleOptionGroupBox *>(option)) {
        QRect rect = ParentStyleClass::subControlRect(CC_GroupBox, option, subControl, widget);
        int topMargin = 0;
        int topHeight = 0;
        int verticalAlignment = proxy()->styleHint(SH_GroupBox_TextLabelVerticalAlignment, groupBox, widget);

        if (!groupBox->text.isEmpty()) {
            topHeight = groupBox->fontMetrics.height();
            if (verticalAlignment & Qt::AlignVCenter) {
                topMargin = topHeight / 2;
            } else if (verticalAlignment & Qt::AlignTop) {
                topMargin = topHeight;
            }
        }
        QRect frameRect = groupBox->rect;
        frameRect.setTop(topMargin);
        if (subControl == SC_GroupBoxFrame) {
            return rect;
        } else if (subControl == SC_GroupBoxContents) {
            int margin = 0;
            int leftMarginExtension = 16;
            return frameRect.adjusted(leftMarginExtension + margin, margin + topHeight, -margin, -margin);
        }

        if (const QGroupBox *groupBoxWidget = qobject_cast<const QGroupBox *>(widget)) {
            //Prepare metrics for a bold font
            QFont font = widget->font();
            font.setBold(true);
            QFontMetrics fontMetrics(font);

            QSize textRect = fontMetrics.boundingRect(groupBoxWidget->title()).size() + QSize(2, 2);
            if (subControl == SC_GroupBoxCheckBox) {
                int indicatorWidth = proxy()->pixelMetric(PM_IndicatorWidth, option, widget);
                int indicatorHeight = proxy()->pixelMetric(PM_IndicatorHeight, option, widget);
                rect.setWidth(indicatorWidth);
                rect.setHeight(indicatorHeight);
                rect.moveTop((textRect.height() - indicatorHeight) / 2);
            } else if (subControl == SC_GroupBoxLabel) {
                rect.setSize(textRect);
            }
        }
        return rect;
    }

    return ParentStyleClass::subControlRect(CC_GroupBox, option, subControl, widget);
}

//___________________________________________________________________________________________________________________
QRect Style::toolButtonSubControlRect(const QStyleOptionComplex *option, SubControl subControl, const QWidget *widget) const
{

    // cast option and check
    const QStyleOptionToolButton *toolButtonOption = qstyleoption_cast<const QStyleOptionToolButton *>(option);
    if (!toolButtonOption) {
        return ParentStyleClass::subControlRect(CC_ToolButton, option, subControl, widget);
    }

    bool hasPopupMenu(toolButtonOption->features & QStyleOptionToolButton::MenuButtonPopup);
    const bool hasInlineIndicator(toolButtonOption->features & QStyleOptionToolButton::HasMenu
                                  && toolButtonOption->features & QStyleOptionToolButton::PopupDelay
                                  && !hasPopupMenu);

    // store rect
    const QRect &rect(option->rect);
    int menuButtonWidth(Metrics::MenuButton_IndicatorWidth);
    switch (subControl) {
    case SC_ToolButtonMenu: {
        // check fratures
        if (!(hasPopupMenu || hasInlineIndicator)) {
            return QRect();
        }

        // check features
        QRect menuRect(rect);
        menuRect.setLeft(rect.right() - menuButtonWidth + 1);
        if (hasInlineIndicator) {
            menuRect.setTop(menuRect.bottom() - menuButtonWidth + 1);
        }

        return visualRect(option, menuRect);
    }

    case SC_ToolButton: {
        if (hasPopupMenu) {
            QRect contentsRect(rect);
            contentsRect.setRight(rect.right() - menuButtonWidth);
            return visualRect(option, contentsRect);
        } else {
            return rect;
        }
    }

    default:
        return QRect();
    }
}

//___________________________________________________________________________________________________________________
QRect Style::comboBoxSubControlRect(const QStyleOptionComplex *option, SubControl subControl, const QWidget *widget) const
{
    // cast option and check
    const QStyleOptionComboBox *comboBoxOption(qstyleoption_cast<const QStyleOptionComboBox *>(option));
    if (!comboBoxOption) {
        return ParentStyleClass::subControlRect(CC_ComboBox, option, subControl, widget);
    }

    bool editable(comboBoxOption->editable);
    bool flat(editable && !comboBoxOption->frame);

    // copy rect
    QRect rect(option->rect);

    switch (subControl) {
    case SC_ComboBoxFrame:
        return flat ? rect : QRect();
    case SC_ComboBoxListBoxPopup:
        return rect;
    case SC_ComboBoxArrow: {
        QRect arrowRect(
            rect.right() - rect.height() + 1,
            rect.top(),
            rect.height(),
            rect.height());

        return arrowRect;
    }

    case SC_ComboBoxEditField: {
        QRect labelRect;
        int frameWidth(pixelMetric(PM_ComboBoxFrameWidth, option, widget));
        labelRect = QRect(
                        rect.left(), rect.top(),
                        rect.width() - rect.height() - 4,
                        rect.height());

        // remove margins
        if (!flat && rect.height() >= option->fontMetrics.height() + 2 * frameWidth) {
            labelRect.adjust(frameWidth, frameWidth, 0, -frameWidth);
        }

        return visualRect(option, labelRect);
    }

    default:
        break;

    }

    return ParentStyleClass::subControlRect(CC_ComboBox, option, subControl, widget);
}

//___________________________________________________________________________________________________________________
QRect Style::spinBoxSubControlRect(const QStyleOptionComplex *option, SubControl subControl, const QWidget *widget) const
{
    // cast option and check
    const QStyleOptionSpinBox *spinBoxOption(qstyleoption_cast<const QStyleOptionSpinBox *>(option));
    if (!spinBoxOption) {
        return ParentStyleClass::subControlRect(CC_SpinBox, option, subControl, widget);
    }
    bool flat(!spinBoxOption->frame);

    // copy rect
    QRect rect(option->rect);

    switch (subControl) {
    case SC_SpinBoxFrame:
        return flat ? QRect() : rect;
    case SC_SpinBoxUp:
        if (rect.width() > 2 * rect.height() + 24) {
            return QRect(rect.right() - rect.height() - 1,
                         rect.top(),
                         rect.height(),
                         rect.height() - 1);
        } else {
            return QRect(rect.right() - 0.6 * rect.height(),
                         rect.top(),
                         rect.height() * 0.6,
                         rect.height() / 2 + 3);
        }
    case SC_SpinBoxDown: {
        if (rect.width() > 2 * rect.height() + 24) {
            return QRect(rect.right() - 2 * rect.height(),
                         rect.top(),
                         rect.height(),
                         rect.height() - 1);
        } else {
            return QRect(rect.right() - 0.6 * rect.height(),
                         rect.top() + rect.height() / 2 - 2,
                         rect.height() * 0.6,
                         rect.height() / 2 + 1);
        }
    }

    case SC_SpinBoxEditField: {
        int frameWidth(pixelMetric(PM_SpinBoxFrameWidth, option, widget));

        QRect labelRect;

        if (rect.width() > 2 * rect.height() + 24) {
            labelRect = QRect(rect.left(), rect.top(),
                              rect.width() - 2 * rect.height() - frameWidth,
                              rect.height());
        } else {
            labelRect = QRect(rect.left(), rect.top(),
                              rect.width() - 0.6 * rect.height() - frameWidth,
                              rect.height());
        }

        // remove right side line editor margins
        if (!flat && labelRect.height() >= option->fontMetrics.height() + 2 * frameWidth) {
            labelRect.adjust(frameWidth, frameWidth, 0, -frameWidth);
        }

        return visualRect(option, labelRect);
    }

    default:
        break;
    }

    return ParentStyleClass::subControlRect(CC_SpinBox, option, subControl, widget);
}

//___________________________________________________________________________________________________________________
QRect Style::scrollBarInternalSubControlRect(const QStyleOptionComplex *option, SubControl subControl) const
{
    const QRect &rect = option->rect;
    const State &state(option->state);
    bool horizontal(state & State_Horizontal);

    switch (subControl) {
    case SC_ScrollBarSubLine: {
        int majorSize(scrollBarButtonHeight(_subLineButtons));
        if (horizontal) {
            return visualRect(option, QRect(rect.left(), rect.top(), majorSize, rect.height()));
        } else {
            return visualRect(option, QRect(rect.left(), rect.top(), rect.width(), majorSize));
        }
    }
    case SC_ScrollBarAddLine: {
        int majorSize(scrollBarButtonHeight(_addLineButtons));
        if (horizontal) {
            return visualRect(option, QRect(rect.right() - majorSize + 1, rect.top(), majorSize, rect.height()));
        } else {
            return visualRect(option, QRect(rect.left(), rect.bottom() - majorSize + 1, rect.width(), majorSize));
        }
    }

    default:
        return QRect();
    }
}

//___________________________________________________________________________________________________________________
QRect Style::scrollBarSubControlRect(const QStyleOptionComplex *option, SubControl subControl, const QWidget *widget) const
{
    // cast option and check
    const QStyleOptionSlider *sliderOption(qstyleoption_cast<const QStyleOptionSlider *>(option));
    if (!sliderOption) {
        return ParentStyleClass::subControlRect(CC_ScrollBar, option, subControl, widget);
    }

    // get relevant state
    const State &state(option->state);
    bool horizontal(state & State_Horizontal);

    switch (subControl) {
    case SC_ScrollBarSubLine:
    case SC_ScrollBarAddLine:
        return QRect();
    case SC_ScrollBarGroove: {
        QRect topRect = visualRect(option, scrollBarInternalSubControlRect(option, SC_ScrollBarSubLine));
        QRect bottomRect = visualRect(option, scrollBarInternalSubControlRect(option, SC_ScrollBarAddLine));

        QPoint topLeftCorner;
        QPoint botRightCorner;

        if (horizontal) {
            topLeftCorner  = QPoint(topRect.right() + 1, topRect.top());
            botRightCorner = QPoint(bottomRect.left()  - 1, topRect.bottom());
        } else {
            topLeftCorner  = QPoint(topRect.left(),  topRect.bottom() + 1);
            botRightCorner = QPoint(topRect.right(), bottomRect.top() - 1);
        }

        // define rect
        return visualRect(option, QRect(topLeftCorner, botRightCorner));
    }
    case SC_ScrollBarSlider: {
        // handle RTL here to unreflect things if need be
        QRect groove = visualRect(option, subControlRect(CC_ScrollBar, option, SC_ScrollBarGroove, widget));
        groove.adjust(0, 0, 1, 1);

        if (sliderOption->minimum == sliderOption->maximum) {
            return groove;
        }

        // Figure out how much room there is
        int space(horizontal ? groove.width() : groove.height());

        // Calculate the portion of this space that the slider should occupy
        int sliderSize = space * qreal(sliderOption->pageStep) / (sliderOption->maximum - sliderOption->minimum + sliderOption->pageStep);
        sliderSize = qMax(sliderSize, static_cast<int>(Metrics::ScrollBar_MinSliderHeight));
        sliderSize = qMin(sliderSize, space);

        space -= sliderSize;
        if (space <= 0) {
            return groove;
        }

        int pos = qRound(qreal(sliderOption->sliderPosition - sliderOption->minimum) / (sliderOption->maximum - sliderOption->minimum) * space);
        if (sliderOption->upsideDown) {
            pos = space - pos;
        }
        if (horizontal) {
            return visualRect(option, QRect(groove.left() + pos, groove.top(), sliderSize, groove.height()));
        } else {
            return visualRect(option, QRect(groove.left(), groove.top() + pos, groove.width(), sliderSize));
        }
    }
    case SC_ScrollBarSubPage: {
        // handle RTL here to unreflect things if need be
        QRect slider = visualRect(option, subControlRect(CC_ScrollBar, option, SC_ScrollBarSlider, widget));
        QRect groove = visualRect(option, subControlRect(CC_ScrollBar, option, SC_ScrollBarGroove, widget));

        if (horizontal) {
            return visualRect(option, QRect(groove.left(), groove.top(), slider.left() - groove.left(), groove.height()));
        } else {
            return visualRect(option, QRect(groove.left(), groove.top(), groove.width(), slider.top() - groove.top()));
        }
    }
    case SC_ScrollBarAddPage: {
        // handle RTL here to unreflect things if need be
        QRect slider = visualRect(option, subControlRect(CC_ScrollBar, option, SC_ScrollBarSlider, widget));
        QRect groove = visualRect(option, subControlRect(CC_ScrollBar, option, SC_ScrollBarGroove, widget));

        if (horizontal) {
            return visualRect(option, QRect(slider.right() + 1, groove.top(), groove.right() - slider.right(), groove.height()));
        } else {
            return visualRect(option, QRect(groove.left(), slider.bottom() + 1, groove.width(), groove.bottom() - slider.bottom()));
        }
    }

    default:
        return ParentStyleClass::subControlRect(CC_ScrollBar, option, subControl, widget);
    }
}

//___________________________________________________________________________________________________________________
QRect Style::dialSubControlRect(const QStyleOptionComplex *option, SubControl subControl, const QWidget *widget) const
{
    // cast option and check
    const QStyleOptionSlider *sliderOption(qstyleoption_cast<const QStyleOptionSlider *>(option));
    if (!sliderOption) {
        return ParentStyleClass::subControlRect(CC_Dial, option, subControl, widget);
    }

    // adjust rect to be square, and centered
    QRect rect(option->rect);
    int dimension(qMin(rect.width(), rect.height()));
    rect = centerRect(rect, dimension, dimension);

    switch (subControl) {
    case QStyle::SC_DialGroove:
        return insideMargin(rect, (Metrics::Slider_ControlThickness - Metrics::Slider_GrooveThickness) / 2 + 2);
    case QStyle::SC_DialHandle: {

        // calculate angle at which handle needs to be drawn
        qreal angle(dialAngle(sliderOption, sliderOption->sliderPosition));

        // groove rect
        QRectF grooveRect(insideMargin(rect, Metrics::Slider_ControlThickness / 2));
        qreal radius(grooveRect.width() / 2);

        // slider center
        QPointF center(grooveRect.center() + QPointF(radius * std::cos(angle), -radius * std::sin(angle)));

        // slider rect
        QRect handleRect(0, 0, Metrics::Slider_ControlThickness, Metrics::Slider_ControlThickness);
        handleRect.moveCenter(center.toPoint());
        return handleRect;
    }

    default:
        return ParentStyleClass::subControlRect(CC_Dial, option, subControl, widget);

    }
}

//___________________________________________________________________________________________________________________
QRect Style::sliderSubControlRect(const QStyleOptionComplex *option, SubControl subControl, const QWidget *widget) const
{
    // cast option and check
    const QStyleOptionSlider *sliderOption(qstyleoption_cast<const QStyleOptionSlider *>(option));
    if (!sliderOption) {
        return ParentStyleClass::subControlRect(CC_Slider, option, subControl, widget);
    }

    // direction
    bool horizontal(sliderOption->orientation == Qt::Horizontal);

    // get base class rect
    QRect rect(ParentStyleClass::subControlRect(CC_Slider, option, subControl, widget));

    // centering
    switch (subControl) {
    case SC_SliderGroove: {
        rect = insideMargin(rect, pixelMetric(PM_DefaultFrameWidth, option, widget));
        if (horizontal) {
            rect = centerRect(sliderOption->rect, rect.width(), Metrics::Slider_GrooveThickness);
        } else {
            rect = centerRect(sliderOption->rect, Metrics::Slider_GrooveThickness, rect.height());
        }
        break;
    }

    case SC_SliderHandle: {
        if (horizontal) {
            rect.moveTop(sliderOption->rect.center().y() - rect.height() / 2);
        } else {
            rect.moveLeft(sliderOption->rect.center().x() - rect.width() / 2);
        }
        break;
    }

    default:
        break;
    }

    return rect;
}

//______________________________________________________________
QSize Style::checkBoxSizeFromContents(const QStyleOption *, const QSize &contentsSize, const QWidget *) const
{
    // get contents size
    QSize size(contentsSize);

    // add focus height
    size = expandSize(size, 0, Metrics::CheckBox_FocusMarginWidth);

    // make sure there is enough height for indicator
    size.setHeight(qMax(size.height(), int(Metrics::CheckBox_Size)));

    // Add space for the indicator and the icon
    size.rwidth() += Metrics::CheckBox_Size + Metrics::CheckBox_ItemSpacing;

    // also add extra space, to leave room to the right of the label
    size.rwidth() += Metrics::CheckBox_ItemSpacing;

    return size;
}

//______________________________________________________________
QSize Style::lineEditSizeFromContents(const QStyleOption *option, const QSize &contentsSize, const QWidget *widget) const
{
    // cast option and check
    const QStyleOptionFrame *frameOption(qstyleoption_cast<const QStyleOptionFrame *>(option));
    if (!frameOption) {
        return contentsSize;
    }

    bool flat(frameOption->lineWidth == 0);
    int frameWidth(pixelMetric(PM_DefaultFrameWidth, option, widget));
    QSize size = flat ? contentsSize : expandSize(contentsSize, frameWidth);

    size.setHeight(qMax(size.height(), int(Metrics::LineEdit_MinHeight)));
    size.setWidth(qMax(size.width(), int(Metrics::LineEdit_MinWidth)));

    return size;
}

//______________________________________________________________
QSize Style::comboBoxSizeFromContents(const QStyleOption *option, const QSize &contentsSize, const QWidget *widget) const
{
    // cast option and check
    const QStyleOptionComboBox *comboBoxOption(qstyleoption_cast<const QStyleOptionComboBox *>(option));
    if (!comboBoxOption) {
        return contentsSize;
    }

    // copy size
    QSize size(contentsSize);

    // add relevant margin
    bool flat(!comboBoxOption->frame);
    int frameWidth(pixelMetric(PM_ComboBoxFrameWidth, option, widget));
    if (!flat) {
        size = expandSize(size, frameWidth);
    }

    size.rwidth() += Metrics::MenuButton_IndicatorWidth;
    size.rwidth() += Metrics::Button_ItemSpacing;

    // FIXME this shouldn't be needed but apparently some width is still missing
    size.rwidth() += size.height();

    // make sure there is enough height for the button
    size.setHeight(qMax(size.height(), int(Metrics::MenuButton_IndicatorWidth)));

    size = expandSize(size, Metrics::ComboBox_MarginWidth, Metrics::ComboBox_MarginHeight);

    // set minimum size
    size.setHeight(qMax(size.height(), int(Metrics::ComboBox_MinHeight)));
    size.setWidth(qMax(size.width(), int(Metrics::ComboBox_MinWidth)));

    return size;
}

//______________________________________________________________
QSize Style::spinBoxSizeFromContents(const QStyleOption *option, const QSize &contentsSize, const QWidget *widget) const
{
    // cast option and check
    const QStyleOptionSpinBox *spinBoxOption(qstyleoption_cast<const QStyleOptionSpinBox *>(option));
    if (!spinBoxOption) {
        return contentsSize;
    }

    bool flat(!spinBoxOption->frame);

    // copy size
    QSize size(contentsSize);

    // add editor margins
    int frameWidth(pixelMetric(PM_SpinBoxFrameWidth, option, widget));
    if (!flat) {
        size = expandSize(size, frameWidth);
    }

    size.rwidth() += 2 * Metrics::SpinBox_MinHeight;
    size.rwidth() += Metrics::Button_ItemSpacing;

    // FIXME this shouldn't be needed but apparently some width is still missing
    size.rwidth() += size.height() / 2;

    // set minimum size
    size.setHeight(qMax(size.height(), int(Metrics::SpinBox_MinHeight)));
    size.setWidth(qMax(size.width(), int(Metrics::SpinBox_MinWidth)));

    return size;
}

//______________________________________________________________
QSize Style::sliderSizeFromContents(const QStyleOption *option, const QSize &contentsSize, const QWidget *) const
{
    // cast option and check
    const QStyleOptionSlider *sliderOption(qstyleoption_cast<const QStyleOptionSlider *>(option));
    if (!sliderOption) {
        return contentsSize;
    }

    // store tick position and orientation
    const QSlider::TickPosition &tickPosition(sliderOption->tickPosition);
    bool horizontal(sliderOption->orientation == Qt::Horizontal);
    bool disableTicks(!Adwaita::Config::SliderDrawTickMarks);

    // do nothing if no ticks are requested
    if (tickPosition == QSlider::NoTicks) {
        return contentsSize;
    }

    /*
     * Qt adds its own tick length directly inside QSlider.
     * Take it out and replace by ours, if needed
     */
    const int tickLength(disableTicks ? 0 : (
                             Metrics::Slider_TickLength + Metrics::Slider_TickMarginWidth +
                             (Metrics::Slider_GrooveThickness - Metrics::Slider_ControlThickness) / 2));

    int builtInTickLength(5);

    QSize size(contentsSize);
    if (horizontal) {
        if (tickPosition & QSlider::TicksAbove) {
            size.rheight() += tickLength - builtInTickLength;
        }
        if (tickPosition & QSlider::TicksBelow) {
            size.rheight() += tickLength - builtInTickLength;
        }
    } else {
        if (tickPosition & QSlider::TicksAbove) {
            size.rwidth() += tickLength - builtInTickLength;
        }
        if (tickPosition & QSlider::TicksBelow) {
            size.rwidth() += tickLength - builtInTickLength;
        }
    }

    return size;
}

//______________________________________________________________
QSize Style::pushButtonSizeFromContents(const QStyleOption *option, const QSize &contentsSize, const QWidget *widget) const
{
    // cast option and check
    const QStyleOptionButton *buttonOption(qstyleoption_cast<const QStyleOptionButton *>(option));
    if (!buttonOption) {
        return contentsSize;
    }

    // output
    QSize size;

    // check text and icon
    bool hasText(!buttonOption->text.isEmpty());
    bool flat(buttonOption->features & QStyleOptionButton::Flat);
    bool hasIcon(!buttonOption->icon.isNull());

    if (!(hasText || hasIcon)) {
        /*
        no text nor icon is passed.
        assume custom button and use contentsSize as a starting point
        */
        size = contentsSize;
    } else {
        /*
        rather than trying to guess what Qt puts into its contents size calculation,
        we recompute the button size entirely, based on button option
        this ensures consistency with the rendering stage
        */

        // update has icon to honour showIconsOnPushButtons, when possible
        hasIcon &= (showIconsOnPushButtons() || flat || !hasText);

        // text
        if (hasText) {
            size = buttonOption->fontMetrics.size(Qt::TextShowMnemonic, buttonOption->text);
        }

        // icon
        if (hasIcon) {
            QSize iconSize = buttonOption->iconSize;
            if (!iconSize.isValid()) {
                iconSize = QSize(pixelMetric(PM_SmallIconSize, option, widget), pixelMetric(PM_SmallIconSize, option, widget));
            }

            size.setHeight(qMax(size.height(), iconSize.height()));
            size.rwidth() += iconSize.width();

            if (hasText) {
                size.rwidth() += Metrics::Button_ItemSpacing;
            }
        }

    }

    // menu
    bool hasMenu(buttonOption->features & QStyleOptionButton::HasMenu);
    if (hasMenu) {
        size.rwidth() += Metrics::MenuButton_IndicatorWidth;
        if (hasText || hasIcon) {
            size.rwidth() += Metrics::Button_ItemSpacing;
        }
    }

    // expand with buttons margin
    size = expandSize(size, Metrics::Button_MarginWidth, Button_MarginHeight);

    // finally add frame margins
    size = expandSize(size, Metrics::Frame_FrameWidth);

    // make sure buttons have a minimum width
    if (hasText) {
        size.setWidth(qMax(size.width(), int(Metrics::Button_MinWidth)));
    }

    // make sure buttons have a minimum height
    size.setHeight(qMax(size.height(), int(Metrics::Button_MinHeight)));

    return size;
}

//______________________________________________________________
QSize Style::toolButtonSizeFromContents(const QStyleOption *option, const QSize &contentsSize, const QWidget *) const
{
    // cast option and check
    const QStyleOptionToolButton *toolButtonOption = qstyleoption_cast<const QStyleOptionToolButton *>(option);
    if (!toolButtonOption) {
        return contentsSize;
    }

    // copy size
    QSize size = contentsSize;

    // get relevant state flags
    bool hasPopupMenu(toolButtonOption->features & QStyleOptionToolButton::MenuButtonPopup);
    const bool hasInlineIndicator(toolButtonOption->features & QStyleOptionToolButton::HasMenu
                                  && toolButtonOption->features & QStyleOptionToolButton::PopupDelay
                                  && !hasPopupMenu);

    int marginWidth(Metrics::ToolButton_MarginWidth);

    if (hasInlineIndicator) {
        size.rwidth() += Metrics::ToolButton_InlineIndicatorWidth;
    }
    size = expandSize(size, marginWidth);

    // We need to add 1px as the toolbutton is smaller by 1px when rendering button frame
    size = expandSize(size, 1);

    return size;
}

//______________________________________________________________
QSize Style::menuBarItemSizeFromContents(const QStyleOption *, const QSize &contentsSize, const QWidget *) const
{
    return expandSize(contentsSize, Metrics::MenuBarItem_MarginWidth, Metrics::MenuBarItem_MarginHeight);
}

//______________________________________________________________
QSize Style::menuItemSizeFromContents(const QStyleOption *option, const QSize &contentsSize, const QWidget *widget) const
{
    // cast option and check
    const QStyleOptionMenuItem *menuItemOption = qstyleoption_cast<const QStyleOptionMenuItem *>(option);
    if (!menuItemOption) {
        return contentsSize;
    }

    /*
     * First calculate the intrinsic size of the item.
     * this must be kept consistent with what's in drawMenuItemControl
     */
    QSize size(contentsSize);
    switch (menuItemOption->menuItemType) {
    case QStyleOptionMenuItem::Normal:
    case QStyleOptionMenuItem::DefaultItem:
    case QStyleOptionMenuItem::SubMenu: {
        int iconWidth = 0;
        if (showIconsInMenuItems()) {
            iconWidth = isQtQuickControl(option, widget) ? qMax(pixelMetric(PM_SmallIconSize, option, widget), menuItemOption->maxIconWidth) : menuItemOption->maxIconWidth;
        }
        int leftColumnWidth(iconWidth);

        // add space with respect to text
        leftColumnWidth += Metrics::MenuItem_ItemSpacing;

        // add checkbox indicator width
        if (menuItemOption->menuHasCheckableItems) {
            leftColumnWidth += Metrics::CheckBox_Size + Metrics::MenuItem_ItemSpacing;
        }

        // add spacing for accelerator
        /*
         * Note:
         * The width of the accelerator itself is not included here since
         * Qt will add that on separately after obtaining the
         * sizeFromContents() for each menu item in the menu to be shown
         * ( see QMenuPrivate::calcActionRects() )
         */
        bool hasAccelerator(menuItemOption->text.indexOf(QLatin1Char('\t')) >= 0);
        if (hasAccelerator) {
            size.rwidth() += Metrics::MenuItem_AcceleratorSpace;
        }

        // right column
        int rightColumnWidth = Metrics::MenuButton_IndicatorWidth + Metrics::MenuItem_ItemSpacing;
        size.rwidth() += leftColumnWidth + rightColumnWidth;

        // make sure height is large enough for icon and arrow
        size.setHeight(qMax(size.height(), int(Metrics::MenuButton_IndicatorWidth)));
        size.setHeight(qMax(size.height(), int(Metrics::CheckBox_Size)));
        size.setHeight(qMax(size.height(), iconWidth));

        // Looks Gtk adds some additional space to the right
        size.rwidth() += Metrics::MenuItem_MarginWidth * 4;

        return expandSize(size, Metrics::MenuItem_MarginWidth);
    }

    case QStyleOptionMenuItem::Separator: {
        if (menuItemOption->text.isEmpty() && menuItemOption->icon.isNull()) {
            return expandSize(QSize(0, 1), Metrics::MenuItem_MarginWidth, 0);
        } else {
            // build toolbutton option
            QStyleOptionToolButton toolButtonOption(separatorMenuItemOption(menuItemOption, widget));

            // make sure height is large enough for icon and text
            int iconWidth(menuItemOption->maxIconWidth);
            int textHeight(menuItemOption->fontMetrics.height());
            if (!menuItemOption->icon.isNull()) {
                size.setHeight(qMax(size.height(), iconWidth));
            }
            if (!menuItemOption->text.isEmpty()) {
                size.setHeight(qMax(size.height(), textHeight));
                size.setWidth(qMax(size.width(), menuItemOption->fontMetrics.horizontalAdvance(menuItemOption->text)));
            }

            return sizeFromContents(CT_ToolButton, &toolButtonOption, size, widget);
        }
    }

    // for all other cases, return input
    default:
        return contentsSize;
    }
}

//______________________________________________________________
QSize Style::progressBarSizeFromContents(const QStyleOption *option, const QSize &contentsSize, const QWidget *) const
{
    // cast option
    const QStyleOptionProgressBar *progressBarOption(qstyleoption_cast<const QStyleOptionProgressBar *>(option));
    if (!progressBarOption) {
        return contentsSize;
    }

    const State &state(option->state);
#if QT_VERSION >= 0x060000
    bool horizontal(state & QStyle::State_Horizontal);
#else
    bool horizontal((state & QStyle::State_Horizontal) || (progressBarOption->orientation == Qt::Horizontal));
#endif

    // make local copy
    QSize size(contentsSize);

    if (horizontal) {
        // check text visibility
        bool textVisible(progressBarOption->textVisible);

        size.setWidth(qMax(size.width(), int(Metrics::ProgressBar_Thickness)));
        size.setHeight(qMax(size.height(), int(Metrics::ProgressBar_Thickness)));
        if (textVisible) {
            size.setHeight(qMax(size.height(), option->fontMetrics.height()));
        }
    } else {
        size.setHeight(qMax(size.height(), int(Metrics::ProgressBar_Thickness)));
        size.setWidth(qMax(size.width(), int(Metrics::ProgressBar_Thickness)));
    }

    return size;
}

//______________________________________________________________
QSize Style::tabWidgetSizeFromContents(const QStyleOption *option, const QSize &contentsSize, const QWidget *widget) const
{
    // cast option and check
    const QStyleOptionTabWidgetFrame *tabOption = qstyleoption_cast<const QStyleOptionTabWidgetFrame *>(option);
    if (!tabOption) {
        return expandSize(contentsSize, Metrics::TabWidget_MarginWidth);
    }

    // try find direct children of type QTabBar and QStackedWidget
    // this is needed in order to add TabWidget margins only if they are necessary around tabWidget content, not the tabbar
    if (!widget) {
        return expandSize(contentsSize, Metrics::TabWidget_MarginWidth);
    }

    QTabBar *tabBar = nullptr;
    QStackedWidget *stack = nullptr;
    auto children(widget->children());
    foreach (auto child, children) {
        if (!tabBar) {
            tabBar = qobject_cast<QTabBar *>(child);
        }
        if (!stack) {
            stack = qobject_cast<QStackedWidget *>(child);
        }
        if (tabBar && stack) {
            break;
        }
    }

    if (!(tabBar && stack)) {
        return expandSize(contentsSize, Metrics::TabWidget_MarginWidth);
    }

    // tab orientation
    bool verticalTabs(tabOption && isVerticalTab(tabOption->shape));
    if (verticalTabs) {
        int tabBarHeight = tabBar->minimumSizeHint().height();
        int stackHeight = stack->minimumSizeHint().height();
        if (contentsSize.height() == tabBarHeight && tabBarHeight + 2 * (Metrics::Frame_FrameWidth - 1) >= stackHeight + 2 * Metrics::TabWidget_MarginWidth) {
            return QSize(contentsSize.width() + 2 * Metrics::TabWidget_MarginWidth, contentsSize.height() + 2 * (Metrics::Frame_FrameWidth - 1));
        } else {
            return expandSize(contentsSize, Metrics::TabWidget_MarginWidth);
        }
    } else {
        int tabBarWidth = tabBar->minimumSizeHint().width();
        int stackWidth = stack->minimumSizeHint().width();
        if (contentsSize.width() == tabBarWidth && tabBarWidth + 2 * (Metrics::Frame_FrameWidth - 1) >= stackWidth + 2 * Metrics::TabWidget_MarginWidth) {
            return QSize(contentsSize.width() + 2 * (Metrics::Frame_FrameWidth - 1), contentsSize.height() + 2 * Metrics::TabWidget_MarginWidth);
        } else {
            return expandSize(contentsSize, Metrics::TabWidget_MarginWidth);
        }
    }
}

//______________________________________________________________
QSize Style::tabBarTabSizeFromContents(const QStyleOption *option, const QSize &contentsSize, const QWidget *) const
{
    const QStyleOptionTab *tabOption(qstyleoption_cast<const QStyleOptionTab *>(option));
    bool hasText(tabOption && !tabOption->text.isEmpty());
    bool hasIcon(tabOption && !tabOption->icon.isNull());
    bool hasLeftButton(tabOption && !tabOption->leftButtonSize.isEmpty());
    bool hasRightButton(tabOption && !tabOption->leftButtonSize.isEmpty());

    // calculate width increment for horizontal tabs
    int widthIncrement = 0;
    if (hasIcon && !(hasText || hasLeftButton || hasRightButton)) {
        widthIncrement -= 4;
    }
    if (hasText && hasIcon) {
        widthIncrement += Metrics::TabBar_TabItemSpacing;
    }
    if (hasLeftButton && (hasText || hasIcon)) {
        widthIncrement += Metrics::TabBar_TabItemSpacing;
    }
    if (hasRightButton && (hasText || hasIcon || hasLeftButton)) {
        widthIncrement += Metrics::TabBar_TabItemSpacing;
    }

    // add margins
    QSize size(contentsSize);

    if (hasText) {
        widthIncrement += option->fontMetrics.horizontalAdvance(tabOption->text) * 0.2;
    }

    // compare to minimum size
    bool verticalTabs(tabOption && isVerticalTab(tabOption));
    if (verticalTabs) {
        size.rheight() += widthIncrement;
        if (hasIcon && !hasText) {
            size = size.expandedTo(QSize(Metrics::TabBar_TabMinHeight, 0));
        } else {
            size = size.expandedTo(QSize(Metrics::TabBar_TabMinHeight, Metrics::TabBar_TabMinWidth));
        }
    } else {
        size.rwidth() += widthIncrement;
        if (hasIcon && !hasText) {
            size = size.expandedTo(QSize(0, Metrics::TabBar_TabMinHeight));
        } else {
            size = size.expandedTo(QSize(Metrics::TabBar_TabMinWidth, Metrics::TabBar_TabMinHeight));
        }
    }

    return size;
}

//______________________________________________________________
QSize Style::headerSectionSizeFromContents(const QStyleOption *option, const QSize &contentsSize, const QWidget *) const
{
    // cast option and check
    const QStyleOptionHeader *headerOption(qstyleoption_cast<const QStyleOptionHeader *>(option));
    if (!headerOption) {
        return contentsSize;
    }

    // get text size
    bool horizontal(headerOption->orientation == Qt::Horizontal);
    bool hasText(!headerOption->text.isEmpty());
    bool hasIcon(!headerOption->icon.isNull());

    QSize textSize(hasText ? headerOption->fontMetrics.size(0, headerOption->text) : QSize());
    QSize iconSize(hasIcon ? QSize(22, 22) : QSize());

    // contents width
    int contentsWidth(0);
    if (hasText) {
        contentsWidth += textSize.width();
    }
    if (hasIcon) {
        contentsWidth += iconSize.width();
        if (hasText) {
            contentsWidth += Metrics::Header_ItemSpacing;
        }
    }

    // contents height
    int contentsHeight(headerOption->fontMetrics.height());
    if (hasIcon) {
        contentsHeight = qMax(contentsHeight, iconSize.height());
    }

    if (horizontal) {
        // also add space for icon
        contentsWidth += Metrics::Header_ArrowSize + Metrics::Header_ItemSpacing;
        contentsHeight = qMax(contentsHeight, int(Metrics::Header_ArrowSize));
    }

    // update contents size, add margins and return
    QSize size(contentsSize.expandedTo(QSize(contentsWidth, contentsHeight)));
    return expandSize(size, Metrics::Header_MarginWidth);
}

//______________________________________________________________
QSize Style::itemViewItemSizeFromContents(const QStyleOption *option, const QSize &contentsSize, const QWidget *widget) const
{
    // call base class
    QSize size(ParentStyleClass::sizeFromContents(CT_ItemViewItem, option, contentsSize, widget));
    return expandSize(size, Metrics::ItemView_ItemMarginWidth);
}

//______________________________________________________________
bool Style::drawFramePrimitive(const QStyleOption *option, QPainter *painter, const QWidget *widget) const
{
    // copy palette and rect
    const QPalette &palette(option->palette);
    const QRect &rect(option->rect);

    // detect title widgets
    const bool isTitleWidget(Adwaita::Config::TitleWidgetDrawFrame && widget && widget->parent() && widget->parent()->inherits("KTitleWidget"));
    // copy state
    const State &state(option->state);
    if (!isTitleWidget && !(state & (State_Sunken | State_Raised))) {
        return true;
    }

    const bool isInputWidget((widget && widget->testAttribute(Qt::WA_Hover))
                             || (isQtQuickControl(option, widget) && option->styleObject->property("elementType").toString() == QStringLiteral("edit")));
    bool enabled(state & State_Enabled);
    bool mouseOver((state & State_Active) && enabled && isInputWidget && (state & State_MouseOver));
    bool hasFocus(enabled && isInputWidget && (state & State_HasFocus));

    // focus takes precedence over mouse over
    _animations->inputWidgetEngine().updateState(widget, AnimationFocus, hasFocus);
    _animations->inputWidgetEngine().updateState(widget, AnimationHover, mouseOver && !hasFocus);

    // retrieve animation mode and opacity
    AnimationMode mode(_animations->inputWidgetEngine().frameAnimationMode(widget));
    qreal opacity(_animations->inputWidgetEngine().frameOpacity(widget));

    // Style options
    StyleOptions styleOptions(palette, _variant);
    styleOptions.setHasFocus(hasFocus);
    styleOptions.setMouseOver(mouseOver);
    styleOptions.setOpacity(opacity);
    styleOptions.setAnimationMode(mode);
    styleOptions.setPainter(painter);
    styleOptions.setRect(rect);

    // render
    if (!Adwaita::Config::SidePanelDrawFrame && widget && widget->property(PropertyNames::sidePanelView).toBool()) {
        bool reverseLayout(option->direction == Qt::RightToLeft);
        Side side(reverseLayout ? SideRight : SideLeft);
        styleOptions.setColor(Colors::sidePanelOutlineColor(styleOptions));
        Adwaita::Renderer::renderSidePanelFrame(styleOptions, side);
    } else if (qobject_cast<const QAbstractScrollArea *>(widget)) {
        styleOptions.setColor(Colors::frameOutlineColor(styleOptions));
        Adwaita::Renderer::renderSquareFrame(styleOptions);
    } else {
        styleOptions.setColor(isTitleWidget ? palette.color(widget->backgroundRole()) : QColor());
        styleOptions.setOutlineColor(Colors::frameOutlineColor(styleOptions));
        Adwaita::Renderer::renderFrame(styleOptions);
    }

    return true;
}

//______________________________________________________________
bool Style::drawFrameLineEditPrimitive(const QStyleOption *option, QPainter *painter, const QWidget *widget) const
{
    // copy palette and rect
    const QPalette &palette(option->palette);
    const QRect &rect(option->rect);

    // make sure there is enough room to render frame
    if (rect.height() < 2 * Metrics::LineEdit_FrameWidth + option->fontMetrics.height()) {
        QColor background(palette.currentColorGroup() == QPalette::Disabled ? palette.color(QPalette::Window) : palette.color(QPalette::Base));

        painter->setPen(Qt::NoPen);
        painter->setBrush(background);
        painter->drawRect(rect);
        return true;
    } else {
        // copy state
        const State &state(option->state);
        bool enabled(state & State_Enabled);
        bool mouseOver((state & State_Active) && enabled && (state & State_MouseOver));
        bool hasFocus(enabled && (state & State_HasFocus));

        // focus takes precedence over mouse over
        _animations->inputWidgetEngine().updateState(widget, AnimationFocus, hasFocus);

        // retrieve animation mode and opacity
        AnimationMode mode(_animations->inputWidgetEngine().frameAnimationMode(widget));
        qreal opacity(_animations->inputWidgetEngine().frameOpacity(widget));

        // Style options
        StyleOptions styleOptions(palette, _variant);
        styleOptions.setMouseOver(mouseOver);
        styleOptions.setHasFocus(hasFocus);
        styleOptions.setOpacity(opacity);
        styleOptions.setAnimationMode(mode);
        styleOptions.setPainter(painter);
        styleOptions.setRect(rect);
        styleOptions.setColor(palette.currentColorGroup() == QPalette::Disabled ? palette.color(QPalette::Window) : palette.color(QPalette::Base));
        styleOptions.setOutlineColor(Colors::inputOutlineColor(styleOptions));

        // render
        if (qobject_cast<const QComboBox *>(widget)) {
            Adwaita::Renderer::renderFlatFrame(styleOptions);
        } else {
            Adwaita::Renderer::renderFrame(styleOptions);
        }
    }

    return true;
}

//___________________________________________________________________________________
bool Style::drawFrameFocusRectPrimitive(const QStyleOption *option, QPainter *painter, const QWidget *widget) const
{
    if (option->styleObject && option->styleObject->property("elementType") == QLatin1String("button")) {
        return true;
    }

    QRectF rect(QRectF(option->rect).adjusted(0, 0, -1, -1));
    const QPalette &palette(option->palette);

    if (rect.width() < 10) {
        return true;
    }

    QColor outlineColor(Colors::mix(palette.color(QPalette::Window), palette.color(QPalette::WindowText), 0.35));
    QPen pen(outlineColor, 1);
    pen.setStyle(Qt::CustomDashLine);
    pen.setDashPattern(QVector<qreal>() << 2 << 1);

    painter->setRenderHint(QPainter::Antialiasing, false);

    painter->setPen(pen);
    painter->drawRoundedRect(rect, 2, 2);

    return true;
}

//___________________________________________________________________________________
bool Style::drawFrameMenuPrimitive(const QStyleOption *option, QPainter *painter, const QWidget *widget) const
{
    // only draw frame for (expanded) toolbars and QtQuick controls
    // do nothing for other cases, for which frame is rendered via drawPanelMenuPrimitive
    const QPalette &palette(option->palette);
    bool hasAlpha(_helper->hasAlphaChannel(widget));

    StyleOptions styleOptions(painter, option->rect);
    styleOptions.setColor(Colors::frameBackgroundColor(StyleOptions(palette, _variant)));
    styleOptions.setOutlineColor(Colors::frameOutlineColor(StyleOptions(palette, _variant)));
    styleOptions.setColorVariant(_variant);

    if (qobject_cast<const QToolBar *>(widget) || isQtQuickControl(option, widget)) {
       Adwaita::Renderer::renderMenuFrame(styleOptions, hasAlpha);
    }

    return true;
}

//______________________________________________________________
bool Style::drawFrameGroupBoxPrimitive(const QStyleOption *option, QPainter *painter, const QWidget *) const
{
    return true;
}

//___________________________________________________________________________________
bool Style::drawFrameTabWidgetPrimitive(const QStyleOption *option, QPainter *painter, const QWidget *widget) const
{
    // cast option and check
    const QStyleOptionTabWidgetFrame *tabOption(qstyleoption_cast<const QStyleOptionTabWidgetFrame *>(option));
    if (!tabOption) {
        return true;
    }

    // do nothing if tabbar is hidden
    bool isQtQuickControl(this->isQtQuickControl(option, widget));
    if (tabOption->tabBarSize.isEmpty() && !isQtQuickControl) {
        return true;
    }

    // adjust rect to handle overlaps
    QRect rect(option->rect);

    QRect tabBarRect(tabOption->tabBarRect);
    QSize tabBarSize(tabOption->tabBarSize);
    Corners corners = AllCorners;

    // adjust corners to deal with oversized tabbars
    switch (tabOption->shape) {
    case QTabBar::RoundedNorth:
    case QTabBar::TriangularNorth:
        if (isQtQuickControl) {
            rect.adjust(-1, -1, 1, 0);
        }
        if (tabBarSize.width() >= rect.width() - 2 * Metrics::Frame_FrameRadius) {
            corners &= ~CornersTop;
        }
        if (tabBarRect.left() < rect.left() + Metrics::Frame_FrameRadius) {
            corners &= ~CornerTopLeft;
        }
        if (tabBarRect.right() > rect.right() - Metrics::Frame_FrameRadius) {
            corners &= ~CornerTopRight;
        }
        break;
    case QTabBar::RoundedSouth:
    case QTabBar::TriangularSouth:
        if (isQtQuickControl) {
            rect.adjust(-1, 0, 1, 1);
        }
        if (tabBarSize.width() >= rect.width() - 2 * Metrics::Frame_FrameRadius) {
            corners &= ~CornersBottom;
        }
        if (tabBarRect.left() < rect.left() + Metrics::Frame_FrameRadius) {
            corners &= ~CornerBottomLeft;
        }
        if (tabBarRect.right() > rect.right() - Metrics::Frame_FrameRadius) {
            corners &= ~CornerBottomRight;
        }
        break;
    case QTabBar::RoundedWest:
    case QTabBar::TriangularWest:
        if (isQtQuickControl) {
            rect.adjust(-1, 0, 0, 0);
        }
        if (tabBarSize.height() >= rect.height() - 2 * Metrics::Frame_FrameRadius) {
            corners &= ~CornersLeft;
        }
        if (tabBarRect.top() < rect.top() + Metrics::Frame_FrameRadius) {
            corners &= ~CornerTopLeft;
        }
        if (tabBarRect.bottom() > rect.bottom() - Metrics::Frame_FrameRadius) {
            corners &= ~CornerBottomLeft;
        }
        break;
    case QTabBar::RoundedEast:
    case QTabBar::TriangularEast:
        if (isQtQuickControl) {
            rect.adjust(0, 0, 1, 0);
        }
        if (tabBarSize.height() >= rect.height() - 2 * Metrics::Frame_FrameRadius) {
            corners &= ~CornersRight;
        }
        if (tabBarRect.top() < rect.top() + Metrics::Frame_FrameRadius) {
            corners &= ~CornerTopRight;
        }
        if (tabBarRect.bottom() > rect.bottom() - Metrics::Frame_FrameRadius) {
            corners &= ~CornerBottomRight;
        }
        break;
    default:
        break;
    }

    // define colors
    const QPalette &palette(option->palette);
    StyleOptions styleOptions(painter, rect);
    styleOptions.setColor(palette.color(QPalette::Base));
    styleOptions.setColorVariant(_variant);
    styleOptions.setOutlineColor(Colors::frameOutlineColor(StyleOptions(palette, _variant)));
    Adwaita::Renderer::renderTabWidgetFrame(styleOptions, corners);

    return true;
}

//___________________________________________________________________________________
bool Style::drawFrameTabBarBasePrimitive(const QStyleOption *option, QPainter *painter, const QWidget *) const
{
    // tabbar frame used either for 'separate' tabbar, or in 'document mode'

    // cast option and check
    const QStyleOptionTabBarBase *tabOption(qstyleoption_cast<const QStyleOptionTabBarBase *>(option));
    if (!tabOption) {
        return true;
    }

    // Style options
    StyleOptions styleOptions(option->palette, _variant);
    styleOptions.setState(option->state);

    // get rect, orientation, palette
    QRect rect(option->rect);
    QColor outline(Colors::frameOutlineColor(styleOptions));
    QColor background = Colors::tabBarColor(styleOptions);

    // setup painter
    painter->setBrush(background);
    painter->setRenderHint(QPainter::Antialiasing, false);
    painter->setPen(QPen(outline, 1));

    painter->drawRect(rect.adjusted(0, 0, -1, -1));

    return true;
}

//___________________________________________________________________________________
bool Style::drawFrameWindowPrimitive(const QStyleOption *option, QPainter *painter, const QWidget *) const
{
    // copy rect and palette
    const QRect &rect(option->rect);
    const QPalette &palette(option->palette);
    State state(option->state);
    bool selected(state & State_Selected);

    // Style options
    StyleOptions styleOptions(palette, _variant);
    styleOptions.setMouseOver(false);
    styleOptions.setHasFocus(selected);
    styleOptions.setPainter(painter);
    styleOptions.setRect(rect);
    styleOptions.setOutlineColor(Colors::frameOutlineColor(styleOptions));

    // render frame outline
    Adwaita::Renderer::renderMenuFrame(styleOptions);

    return true;
}

//___________________________________________________________________________________
bool Style::drawIndicatorArrowPrimitive(ArrowOrientation orientation, const QStyleOption *option, QPainter *painter, const QWidget *widget) const
{
    // store rect and palette
    const QRect &rect(option->rect);
    const QPalette &palette(option->palette);

    // store state
    const State &state(option->state);
    bool enabled(state & State_Enabled);
    bool mouseOver((state & State_Active) && enabled && (state & State_MouseOver));
    bool hasFocus(enabled && (state & State_HasFocus));

    // detect special buttons
    bool inTabBar(widget && qobject_cast<const QTabBar *>(widget->parentWidget()));
    bool inToolButton(qstyleoption_cast<const QStyleOptionToolButton *>(option));

    // Style options
    StyleOptions styleOptions(palette, _variant);
    styleOptions.setMouseOver(mouseOver);
    styleOptions.setHasFocus(hasFocus);
    styleOptions.setRect(rect);
    styleOptions.setPainter(painter);

    // color
    QColor color;
    if (inTabBar) {
        // for tabbar arrows one uses animations to get the arrow color
        /*
         * get animation state
         * there is no need to update the engine since this was already done when rendering the frame
         */
        AnimationMode mode(_animations->widgetStateEngine().buttonAnimationMode(widget));
        qreal opacity(_animations->widgetStateEngine().buttonOpacity(widget));

        styleOptions.setAnimationMode(mode);
        styleOptions.setOpacity(opacity);

        color = Colors::arrowOutlineColor(styleOptions);
    } else if (mouseOver && !inToolButton) {
        color = Colors::hoverColor(styleOptions);
    } else if (inToolButton) {
        bool flat(state & State_AutoRaise);

        // cast option
        const QStyleOptionToolButton *toolButtonOption(static_cast<const QStyleOptionToolButton *>(option));
        bool hasPopupMenu(toolButtonOption->subControls & SC_ToolButtonMenu);
        if (flat && hasPopupMenu) {
            // for menu arrows in flat toolbutton one uses animations to get the arrow color
            // handle arrow over animation
            bool arrowHover(mouseOver && (toolButtonOption->activeSubControls & SC_ToolButtonMenu));
            _animations->toolButtonEngine().updateState(widget, AnimationHover, arrowHover);

            bool animated(_animations->toolButtonEngine().isAnimated(widget, AnimationHover));

            // Style ooptions
            styleOptions.setAnimationMode(animated ? AnimationHover : AnimationNone);

            color = Colors::arrowOutlineColor(styleOptions);
        } else {
            bool sunken(state & (State_On | State_Sunken));
            if (flat) {
                if (sunken && hasFocus && !mouseOver) {
                    color = palette.color(QPalette::HighlightedText);
                } else {
                    // Style options
                    styleOptions.setColorRole(QPalette::WindowText);

                    color = Colors::arrowOutlineColor(styleOptions);
                }
            } else if (hasFocus && !mouseOver)  {
                color = palette.color(QPalette::HighlightedText);
            } else {
                // Style options
                styleOptions.setColorRole(QPalette::ButtonText);

                color = Colors::arrowOutlineColor(styleOptions);
            }
        }
    } else {
        // Style options
        styleOptions.setColorRole(QPalette::WindowText);

        color = Colors::arrowOutlineColor(styleOptions);
    }

    // render
    styleOptions.setColor(color);
    Adwaita::Renderer::renderArrow(styleOptions, orientation);

    return true;
}

//___________________________________________________________________________________
bool Style::drawIndicatorHeaderArrowPrimitive(const QStyleOption *option, QPainter *painter, const QWidget *) const
{
    const QStyleOptionHeader *headerOption(qstyleoption_cast<const QStyleOptionHeader *>(option));
    const State &state(option->state);

    // arrow orientation
    ArrowOrientation orientation(ArrowNone);
    if (state & State_UpArrow || (headerOption && headerOption->sortIndicator == QStyleOptionHeader::SortUp)) {
        orientation = ArrowUp;
    } else if (state & State_DownArrow || (headerOption && headerOption->sortIndicator == QStyleOptionHeader::SortDown)) {
        orientation = ArrowDown;
    }
    if (orientation == ArrowNone) {
        return true;
    }

    // invert arrows if requested by (hidden) options
    if (Adwaita::Config::ViewInvertSortIndicator) {
        orientation = (orientation == ArrowUp) ? ArrowDown : ArrowUp;
    }

    // Style options
    StyleOptions styleOptions(option->palette, _variant);
    styleOptions.setState(state);
    // render
    styleOptions.setPainter(painter);
    styleOptions.setRect(option->rect);
    // define color and polygon for drawing arrow
    styleOptions.setColor(Colors::headerTextColor(styleOptions));
    Adwaita::Renderer::renderArrow(styleOptions, orientation);

    return true;
}

//______________________________________________________________
bool Style::drawPanelButtonCommandPrimitive(const QStyleOption *option, QPainter *painter, const QWidget *widget) const
{
    // cast option and check
    const QStyleOptionButton *buttonOption(qstyleoption_cast< const QStyleOptionButton * >(option));
    if (!buttonOption) {
        return true;
    }

    // rect and palette
    const QRect &rect(option->rect);

    // button state
    const State &state(option->state);
    bool enabled(state & State_Enabled);
    bool windowActive(state & State_Active);
    bool mouseOver((state & State_Active) && enabled && (state & State_MouseOver));
    bool hasFocus((enabled && (state & State_HasFocus)) && !(widget && widget->focusProxy()));
    bool sunken(state & (State_On | State_Sunken));
    bool flat(buttonOption->features & QStyleOptionButton::Flat);

    // update animation state
    // mouse over takes precedence over focus
    _animations->widgetStateEngine().updateState(widget, AnimationPressed, sunken);
    _animations->widgetStateEngine().updateState(widget, AnimationHover, mouseOver);

    AnimationMode mode(_animations->widgetStateEngine().buttonAnimationMode(widget));
    qreal opacity(_animations->widgetStateEngine().buttonOpacity(widget));

    // Style options
    StyleOptions styleOptions(option->palette, _variant);
    styleOptions.setMouseOver(mouseOver);
    styleOptions.setHasFocus(hasFocus);
    styleOptions.setSunken(sunken);
    styleOptions.setOpacity(opacity);
    styleOptions.setAnimationMode(mode);
    styleOptions.setPainter(painter);
    styleOptions.setRect(rect);

    if (flat) {
        // define colors and render
        styleOptions.setColor(Colors::toolButtonColor(styleOptions));
        Adwaita::Renderer::renderToolButtonFrame(styleOptions);
    } else {
        // update button color from palette in case button is default
        QPalette palette(option->palette);
        if (enabled && buttonOption->features & QStyleOptionButton::DefaultButton) {
            QColor button(palette.color(QPalette::Button));
            QColor base(palette.color(QPalette::Base));
            palette.setColor(QPalette::Button, Colors::mix(button, base, 0.7));
        }

        // render
        styleOptions.setActive(enabled && windowActive);
        styleOptions.setColor(Colors::buttonBackgroundColor(styleOptions));
        styleOptions.setOutlineColor(Colors::buttonOutlineColor(styleOptions));
        Adwaita::Renderer::renderButtonFrame(styleOptions);
    }

    return true;
}

//______________________________________________________________
bool Style::drawPanelButtonToolPrimitive(const QStyleOption *option, QPainter *painter, const QWidget *widget) const
{
    // copy palette and rect
    QRect rect(option->rect);

    // store relevant flags
    const State &state(option->state);
    bool autoRaise(state & State_AutoRaise);
    bool enabled(state & State_Enabled);
    bool windowActive(state & State_Active);
    bool sunken(state & (State_On | State_Sunken));
    bool mouseOver((state & State_Active) && enabled && (option->state & State_MouseOver));
    bool hasFocus(enabled && (option->state & (State_HasFocus | State_Sunken)));

    /*
     * get animation state
     * no need to update, this was already done in drawToolButtonComplexControl
     */
    AnimationMode mode(_animations->widgetStateEngine().buttonAnimationMode(widget));
    qreal opacity(_animations->widgetStateEngine().buttonOpacity(widget));

    // Style options
    StyleOptions styleOptions(option->palette, _variant);
    styleOptions.setMouseOver(mouseOver);
    styleOptions.setHasFocus(hasFocus);
    styleOptions.setSunken(sunken);
    styleOptions.setOpacity(opacity);
    styleOptions.setAnimationMode(mode);
    styleOptions.setPainter(painter);
    styleOptions.setRect(rect);

    if (!autoRaise || mouseOver || sunken) {
        // need to check widget for popup mode, because option is not set properly
        const QToolButton *toolButton(qobject_cast<const QToolButton *>(widget));
        bool hasPopupMenu(toolButton && toolButton->popupMode() == QToolButton::MenuButtonPopup);

        // render as push button
        QColor shadow(Colors::shadowColor(styleOptions));
        QColor outline(Colors::buttonOutlineColor(styleOptions));
        QColor background(Colors::buttonBackgroundColor(styleOptions));

        // adjust frame in case of menu
        if (hasPopupMenu) {
            painter->setClipRect(rect);
            rect.adjust(0, 0, Metrics::Frame_FrameRadius + 2, 0);
            rect = visualRect(option, rect);
        }

        styleOptions.setActive(windowActive);
        styleOptions.setColor(background);
        styleOptions.setOutlineColor(outline);
        styleOptions.setRect(rect);
        // render
        Adwaita::Renderer::renderButtonFrame(styleOptions);
    } else {
        styleOptions.setColor(Colors::toolButtonColor(styleOptions));
        Adwaita::Renderer::renderToolButtonFrame(styleOptions);
    }

    return true;
}

//______________________________________________________________
bool Style::drawTabBarPanelButtonToolPrimitive(const QStyleOption *option, QPainter *painter, const QWidget *widget) const
{
    // copy palette and rect
    QRect rect(option->rect);

    // static_cast is safe here since check was already performed in calling function
    const QTabBar *tabBar(static_cast<QTabBar *>(widget->parentWidget()));

    // overlap.
    // subtract 1, because of the empty pixel left the tabwidget frame
    int overlap(Metrics::TabBar_BaseOverlap - 1);

    // adjust rect based on tabbar shape
    switch (tabBar->shape()) {
    case QTabBar::RoundedNorth:
    case QTabBar::TriangularNorth:
        rect.adjust(0, 0, 0, -overlap);
        break;
    case QTabBar::RoundedSouth:
    case QTabBar::TriangularSouth:
        rect.adjust(0, overlap, 0, 0);
        break;
    case QTabBar::RoundedWest:
    case QTabBar::TriangularWest:
        rect.adjust(0, 0, -overlap, 0);
        break;

    case QTabBar::RoundedEast:
    case QTabBar::TriangularEast:
        rect.adjust(overlap, 0, 0, 0);
        break;

    default:
        break;
    }

    // get the relevant palette
    const QWidget *parent(tabBar->parentWidget());
    if (qobject_cast<const QTabWidget *>(parent)) {
        parent = parent->parentWidget();
    }
    QPalette palette(parent ? parent->palette() : QApplication::palette());

    QColor color = hasAlteredBackground(parent) ? Colors::frameBackgroundColor(StyleOptions(palette, _variant)) : palette.color(QPalette::Window);

    // render flat background
    painter->setPen(Qt::NoPen);
    painter->setBrush(color);
    painter->drawRect(rect);

    return true;
}

//___________________________________________________________________________________
bool Style::drawPanelScrollAreaCornerPrimitive(const QStyleOption *option, QPainter *painter, const QWidget *widget) const
{
    // make sure background role matches viewport
    const QAbstractScrollArea *scrollArea;
    if ((scrollArea = qobject_cast<const QAbstractScrollArea *>(widget)) && scrollArea->viewport()) {
        // need to adjust clipRect in order not to render outside of frame
        int frameWidth(pixelMetric(PM_DefaultFrameWidth, 0, scrollArea));
        painter->setClipRect(insideMargin(scrollArea->rect(), frameWidth));
        painter->setBrush(scrollArea->viewport()->palette().color(scrollArea->viewport()->backgroundRole()));
        painter->setPen(Qt::NoPen);
        painter->drawRect(option->rect);
        return true;
    } else {
        return false;
    }
}
//___________________________________________________________________________________
bool Style::drawPanelMenuPrimitive(const QStyleOption *option, QPainter *painter, const QWidget *widget) const
{
    /*
     * do nothing if menu is embedded in another widget
     * this corresponds to having a transparent background
     */
    if (widget && !widget->isWindow()) {
        return true;
    }

    const QPalette &palette(option->palette);
    bool hasAlpha(_helper->hasAlphaChannel(widget));

    StyleOptions styleOptions(painter, option->rect);
    styleOptions.setColor(Colors::frameBackgroundColor(StyleOptions(palette, _variant)));
    styleOptions.setColorVariant(_variant);
    styleOptions.setOutlineColor(Colors::frameOutlineColor(StyleOptions(palette, _variant)));

    Adwaita::Renderer::renderMenuFrame(styleOptions, hasAlpha);

    return true;
}

//___________________________________________________________________________________
bool Style::drawPanelTipLabelPrimitive(const QStyleOption *option, QPainter *painter, const QWidget *widget) const
{
    const QPalette &palette(option->palette);
    QColor background(palette.color(QPalette::ToolTipBase));

    bool hasAlpha(_helper->hasAlphaChannel(widget));

    if (hasAlpha) {
        int alpha = styleHint(SH_ToolTipLabel_Opacity, option, widget);
        int h, s, l, a;
        background.getHsl(&h, &s, &l, &a);
        background = QColor::fromHsl(h, s, l, alpha);
    }

    StyleOptions styleOptions(painter, option->rect);
    styleOptions.setColor(background);
    styleOptions.setColorVariant(_variant);
    styleOptions.setOutlineColor(Colors::transparentize(QColor("black"), 0.3));

    Adwaita::Renderer::renderMenuFrame(styleOptions, hasAlpha);
    return true;
}

//__________________________________________________________________________________
bool Style::drawPanelItemViewRowPrimitive(const QStyleOption *option, QPainter *painter, const QWidget *widget) const
{
    const QStyleOptionViewItem *vopt = qstyleoption_cast<const QStyleOptionViewItem *>(option);

    if (!vopt) {
        return false;
    }

    QPalette::ColorGroup cg = (widget ? widget->isEnabled() : (vopt->state & QStyle::State_Enabled))
                              ? QPalette::Normal : QPalette::Disabled;
    if (cg == QPalette::Normal && !(vopt->state & QStyle::State_Active)) {
        cg = QPalette::Inactive;
    }

    if ((vopt->state & QStyle::State_Selected) &&  proxy()->styleHint(QStyle::SH_ItemView_ShowDecorationSelected, option, widget)) {
        painter->fillRect(vopt->rect, vopt->palette.color(cg, QPalette::Highlight));
    }

    return true;
}

//___________________________________________________________________________________
bool Style::drawPanelItemViewItemPrimitive(const QStyleOption *option, QPainter *painter, const QWidget *widget) const
{
    // cast option and check
    const QStyleOptionViewItem *viewItemOption = qstyleoption_cast<const QStyleOptionViewItem *>(option);
    if (!viewItemOption) {
        return false;
    }

    // store palette and rect
    const QPalette &palette(option->palette);
    QRect rect(option->rect);

    // store flags
    const State &state(option->state);
    bool selected(state & State_Selected);
    bool enabled(state & State_Enabled);
    bool windowActive(state & State_Active);

    bool hasCustomBackground = viewItemOption->backgroundBrush.style() != Qt::NoBrush && !(state & State_Selected);
    bool hasSolidBackground = !hasCustomBackground || viewItemOption->backgroundBrush.style() == Qt::SolidPattern;

    // do nothing if no background is to be rendered
    if (!(selected || hasCustomBackground)) {
        return true;
    }

    // define color group
    QPalette::ColorGroup colorGroup;
    if (enabled) {
        colorGroup = windowActive ? QPalette::Active : QPalette::Inactive;
    } else {
        colorGroup = QPalette::Disabled;
    }

    // render custom background
    if (hasCustomBackground && !hasSolidBackground) {
        painter->setBrushOrigin(viewItemOption->rect.topLeft());
        painter->setBrush(viewItemOption->backgroundBrush);
        painter->setPen(Qt::NoPen);
        painter->drawRect(viewItemOption->rect);
        return true;
    }

    // render selection
    // define color
    QColor color;
    if (hasCustomBackground && hasSolidBackground) {
        color = viewItemOption->backgroundBrush.color();
    } else {
        color = palette.color(colorGroup, QPalette::Highlight);
    }

    // render
    StyleOptions styleOptions(painter, rect);
    styleOptions.setColor(color);
    styleOptions.setColorVariant(_variant);
    Adwaita::Renderer::renderSelection(styleOptions);

    return true;
}

//___________________________________________________________________________________
bool Style::drawIndicatorCheckBoxPrimitive(const QStyleOption *option, QPainter *painter, const QWidget *widget) const
{
    // copy rect and palette
    const QRect &rect(option->rect.adjusted(1, 1, -1, -1));
    const QPalette &palette(option->palette);

    // store flags
    const State &state(option->state);
    bool enabled(state & State_Enabled);
    bool mouseOver((state & State_Active) && enabled && (state & State_MouseOver));
    bool sunken(enabled && (state & State_Sunken));
    bool active((state & (State_On | State_NoChange)));
    bool windowActive(state & State_Active);

    // checkbox state
    CheckBoxState checkBoxState(CheckOff);
    if (state & State_NoChange) {
        checkBoxState = CheckPartial;
    } else if (state & State_On) {
        checkBoxState = CheckOn;
    }

    // Style options
    StyleOptions styleOptions(option->palette, _variant);
    styleOptions.setMouseOver(mouseOver);
    styleOptions.setHasFocus(false);
    styleOptions.setSunken(sunken);
    styleOptions.setOpacity(AnimationData::OpacityInvalid);
    styleOptions.setAnimationMode(AnimationNone);
    styleOptions.setCheckboxState(checkBoxState);
    styleOptions.setPainter(painter);
    styleOptions.setRect(rect);

    // detect checkboxes in lists
    bool isSelectedItem(this->isSelectedItem(widget, rect.center()));

    // animation state
    _animations->widgetStateEngine().updateState(widget, AnimationHover, mouseOver);
    if (checkBoxState != CheckPartial) {
        _animations->widgetStateEngine().updateState(widget, AnimationPressed, checkBoxState != CheckOff);
        if (_animations->widgetStateEngine().isAnimated(widget, AnimationPressed) && checkBoxState == CheckOn) {
            checkBoxState = CheckAnimated;
        }
    }
    qreal animation(_animations->widgetStateEngine().opacity(widget, AnimationPressed));

    QColor tickColor;
    if (isSelectedItem) {
        // Style options
        styleOptions.setActive(enabled && active);

        tickColor = Colors::checkBoxIndicatorColor(styleOptions);
        styleOptions.setColor(palette.color(QPalette::Base));
        Adwaita::Renderer::renderCheckBoxBackground(styleOptions);
    } else {
        AnimationMode mode(_animations->widgetStateEngine().isAnimated(widget, AnimationHover) ? AnimationHover : AnimationNone);
        qreal opacity(_animations->widgetStateEngine().opacity(widget, AnimationHover));

        // Style options
        styleOptions.setAnimationMode(mode);
        styleOptions.setOpacity(opacity);
        styleOptions.setActive(enabled && active);

        tickColor = Colors::checkBoxIndicatorColor(styleOptions);
    }
    // render
    styleOptions.setActive(enabled && windowActive);
    styleOptions.setColor(Colors::indicatorBackgroundColor(styleOptions));
    styleOptions.setCheckboxState(checkBoxState);
    styleOptions.setOutlineColor(Colors::indicatorOutlineColor(styleOptions));

    Adwaita::Renderer::renderCheckBox(styleOptions, tickColor, animation);
    return true;
}

//___________________________________________________________________________________
bool Style::drawIndicatorRadioButtonPrimitive(const QStyleOption *option, QPainter *painter, const QWidget *widget) const
{
    // copy rect and palette
    const QRect &rect(option->rect);
    const QPalette &palette(option->palette);

    // store flags
    const State &state(option->state);
    bool enabled(state & State_Enabled);
    bool mouseOver((state & State_Active) && enabled && (state & State_MouseOver));
    bool sunken(state & State_Sunken);
    bool checked(state & State_On);
    bool windowActive(state & State_Active);

    // Style options
    StyleOptions styleOptions(option->palette, _variant);
    styleOptions.setMouseOver(mouseOver);
    styleOptions.setHasFocus(false);
    styleOptions.setSunken(sunken);
    styleOptions.setOpacity(AnimationData::OpacityInvalid);
    styleOptions.setAnimationMode(AnimationNone);
    styleOptions.setCheckboxState(checked ? CheckOn : CheckOff);
    styleOptions.setPainter(painter);
    styleOptions.setRect(rect);

    // radio button state
    RadioButtonState radioButtonState(state & State_On ? RadioOn : RadioOff);

    // detect radiobuttons in lists
    bool isSelectedItem(this->isSelectedItem(widget, rect.center()));

    // animation state
    _animations->widgetStateEngine().updateState(widget, AnimationHover, mouseOver);
    _animations->widgetStateEngine().updateState(widget, AnimationPressed, radioButtonState != RadioOff);
    if (_animations->widgetStateEngine().isAnimated(widget, AnimationPressed)) {
        radioButtonState = RadioAnimated;
    }
    qreal animation(_animations->widgetStateEngine().opacity(widget, AnimationPressed));

    // colors
    QColor shadow(Colors::shadowColor(StyleOptions(palette, _variant)));
    QColor tickColor;
    if (isSelectedItem) {
        // Style options
        styleOptions.setActive(enabled && checked);

        tickColor = Colors::checkBoxIndicatorColor(styleOptions);
        styleOptions.setColor(palette.color(QPalette::Base));

        Adwaita::Renderer::renderRadioButtonBackground(styleOptions);
    } else {
        AnimationMode mode(_animations->widgetStateEngine().isAnimated(widget, AnimationHover) ? AnimationHover : AnimationNone);
        qreal opacity(_animations->widgetStateEngine().opacity(widget, AnimationHover));

        // Style options
        styleOptions.setAnimationMode(mode);
        styleOptions.setOpacity(opacity);
        styleOptions.setActive(enabled && checked);

        tickColor = Colors::checkBoxIndicatorColor(styleOptions);
    }

    // render
    styleOptions.setActive(enabled && windowActive);
    styleOptions.setColor(Colors::indicatorBackgroundColor(styleOptions));
    styleOptions.setInMenu(false);
    styleOptions.setOutlineColor(Colors::indicatorOutlineColor(styleOptions));
    styleOptions.setRadioButtonState(radioButtonState);

    Adwaita::Renderer::renderRadioButton(styleOptions, tickColor, animation);

    return true;
}

//___________________________________________________________________________________
bool Style::drawIndicatorButtonDropDownPrimitive(const QStyleOption *option, QPainter *painter, const QWidget *widget) const
{
    // cast option and check
    const QStyleOptionToolButton *toolButtonOption(qstyleoption_cast<const QStyleOptionToolButton *>(option));
    if (!toolButtonOption) {
        return true;
    }

    // store state
    const State &state(option->state);
    bool autoRaise(state & State_AutoRaise);
    bool enabled(state & State_Enabled);
    bool windowActive(state & State_Active);
    bool hasFocus(enabled && (state & (State_HasFocus | State_Sunken)));
    bool mouseOver((state & State_Active) && enabled && (state & State_MouseOver));
    bool sunken(enabled && (state & State_Sunken));

    // do nothing for autoraise buttons
    if ((autoRaise && !sunken && !mouseOver) || !(toolButtonOption->subControls & SC_ToolButtonMenu)) {
        return true;
    }

    // store palette and rect
    const QRect &rect(option->rect);

    // update animation state
    // mouse over takes precedence over focus
    _animations->widgetStateEngine().updateState(widget, AnimationPressed, sunken);
    _animations->widgetStateEngine().updateState(widget, AnimationHover, mouseOver);

    // Style options
    StyleOptions styleOptions(option->palette, _variant);
    styleOptions.setMouseOver(mouseOver);
    styleOptions.setHasFocus(false);
    styleOptions.setSunken(sunken);
    styleOptions.setOpacity(AnimationData::OpacityInvalid);
    styleOptions.setAnimationMode(AnimationNone);

    QRect frameRect(rect);
    painter->setClipRect(rect);
    frameRect.adjust(-Metrics::Frame_FrameRadius - 1, 0, 0, 0);
    frameRect = visualRect(option, frameRect);

    // render
    styleOptions.setPainter(painter);
    styleOptions.setRect(frameRect);
    styleOptions.setColor(Colors::buttonBackgroundColor(styleOptions));
    styleOptions.setOutlineColor(Colors::buttonOutlineColor(styleOptions));
    styleOptions.setHasFocus(hasFocus);
    styleOptions.setActive(windowActive);
    Adwaita::Renderer::renderButtonFrame(styleOptions);

    // also render separator
    QRect separatorRect(rect.adjusted(0, 2, -2, -2));
    separatorRect.setWidth(1);
    separatorRect = visualRect(option, separatorRect);

    styleOptions.setRect(separatorRect);

    Adwaita::Renderer::renderSeparator(styleOptions, true);

    return true;
}

//___________________________________________________________________________________
bool Style::drawIndicatorTabClosePrimitive(const QStyleOption *option, QPainter *painter, const QWidget *widget) const
{
    // get icon and check
    QIcon icon(standardIcon(SP_TitleBarCloseButton, option, widget));
    if (icon.isNull()) {
        return false;
    }

    // store state
    const State &state(option->state);
    bool enabled(state & State_Enabled);
    bool active(state & State_Raised);
    bool sunken(state & State_Sunken);

    // decide icon mode and state
    QIcon::Mode iconMode;
    QIcon::State iconState;
    if (!enabled) {
        iconMode = QIcon::Disabled;
        iconState = QIcon::Off;
    } else {
        if (active) {
            iconMode = QIcon::Active;
        } else {
            iconMode = QIcon::Normal;
        }

        iconState = sunken ? QIcon::On : QIcon::Off;
    }

    // icon size
    int iconWidth(pixelMetric(QStyle::PM_SmallIconSize, option, widget));
    QSize iconSize(iconWidth, iconWidth);

    // get pixmap
    QPixmap pixmap(icon.pixmap(iconSize, iconMode, iconState));

    // render
    drawItemPixmap(painter, option->rect, Qt::AlignCenter, pixmap);
    return true;
}

//___________________________________________________________________________________
bool Style::drawIndicatorTabTearPrimitive(const QStyleOption *option, QPainter *painter, const QWidget *) const
{
    // cast option and check
    const QStyleOptionTab *tabOption(qstyleoption_cast<const QStyleOptionTab *>(option));
    if (!tabOption) {
        return true;
    }

    // store palette and rect
    const QPalette &palette(option->palette);
    QRect rect(option->rect);

    bool reverseLayout(option->direction == Qt::RightToLeft);

    QColor color(Colors::alphaColor(palette.color(QPalette::WindowText), 0.2));
    painter->setRenderHint(QPainter::Antialiasing, false);
    painter->setPen(color);
    painter->setBrush(Qt::NoBrush);
    switch (tabOption->shape) {
    case QTabBar::TriangularNorth:
    case QTabBar::RoundedNorth:
        rect.adjust(0, 1, 0, 0);
        if (reverseLayout) {
            painter->drawLine(rect.topRight(), rect.bottomRight());
        } else {
            painter->drawLine(rect.topLeft(), rect.bottomLeft());
        }
        break;
    case QTabBar::TriangularSouth:
    case QTabBar::RoundedSouth:
        rect.adjust(0, 0, 0, -1);
        if (reverseLayout) {
            painter->drawLine(rect.topRight(), rect.bottomRight());
        } else {
            painter->drawLine(rect.topLeft(), rect.bottomLeft());
        }
        break;
    case QTabBar::TriangularWest:
    case QTabBar::RoundedWest:
        rect.adjust(1, 0, 0, 0);
        painter->drawLine(rect.topLeft(), rect.topRight());
        break;
    case QTabBar::TriangularEast:
    case QTabBar::RoundedEast:
        rect.adjust(0, 0, -1, 0);
        painter->drawLine(rect.topLeft(), rect.topRight());
        break;

    default:
        break;
    }

    return true;
}

//___________________________________________________________________________________
bool Style::drawIndicatorToolBarHandlePrimitive(const QStyleOption *option, QPainter *painter, const QWidget *) const
{
    // do nothing if disabled from options
    if (!Adwaita::Config::ToolBarDrawItemSeparator) {
        return true;
    }

    // store rect and palette
    QRect rect(option->rect);

    // store state
    const State &state(option->state);
    bool separatorIsVertical(state & State_Horizontal);

    // Style options
    StyleOptions styleOptions(option->palette, _variant);
    styleOptions.setPainter(painter);
    styleOptions.setColor(Colors::separatorColor(styleOptions));

    if (separatorIsVertical) {
        rect.setWidth(Metrics::ToolBar_HandleWidth);
        rect = centerRect(option->rect, rect.size());
        rect.setWidth(3);
        styleOptions.setRect(rect);

        Adwaita::Renderer::renderSeparator(styleOptions, separatorIsVertical);

        rect.translate(2, 0);
        styleOptions.setRect(rect);

        Adwaita::Renderer::renderSeparator(styleOptions, separatorIsVertical);
    } else {
        rect.setHeight(Metrics::ToolBar_HandleWidth);
        rect = centerRect(option->rect, rect.size());
        rect.setHeight(3);
        styleOptions.setRect(rect);

        Adwaita::Renderer::renderSeparator(styleOptions, separatorIsVertical);

        rect.translate(0, 2);
        styleOptions.setRect(rect);

        Adwaita::Renderer::renderSeparator(styleOptions, separatorIsVertical);
    }

    return true;
}

//___________________________________________________________________________________
bool Style::drawIndicatorToolBarSeparatorPrimitive(const QStyleOption *option, QPainter *painter, const QWidget *widget) const
{
    /*
     * do nothing if disabled from options
     * also need to check if widget is a combobox, because of Qt hack using 'toolbar' separator primitive
     * for rendering separators in comboboxes
     */
    if (!(Adwaita::Config::ToolBarDrawItemSeparator || qobject_cast<const QComboBox *>(widget))) {
        return true;
    }

    // store rect and palette
    const QRect &rect(option->rect);

    // store state
    const State &state(option->state);
    bool separatorIsVertical(state & State_Horizontal);

    // Style options
    StyleOptions styleOptions(option->palette, _variant);
    styleOptions.setPainter(painter);
    styleOptions.setRect(rect);
    styleOptions.setColor(Colors::separatorColor(styleOptions));

    // define color and render
    Adwaita::Renderer::renderSeparator(styleOptions, separatorIsVertical);

    return true;
}

//___________________________________________________________________________________
bool Style::drawIndicatorBranchPrimitive(const QStyleOption *option, QPainter *painter, const QWidget *) const
{
    // copy rect and palette
    const QRect &rect(option->rect);
    const QPalette &palette(option->palette);

    // state
    const State &state(option->state);
    bool reverseLayout(option->direction == Qt::RightToLeft);

    //draw expander
    int expanderAdjust = 0;
    if (state & State_Children) {
        // state
        bool expanderOpen(state & State_Open);
        bool enabled(state & State_Enabled);
        bool mouseOver((state & State_Active) && enabled && (state & State_MouseOver));

        // expander rect
        int expanderSize = qMin(rect.width(), rect.height());
        expanderSize = qMin(expanderSize, int(Metrics::ItemView_ArrowSize));
        expanderAdjust = expanderSize / 2 + 1;
        QRect arrowRect = centerRect(rect, expanderSize, expanderSize);

        // get orientation from option
        ArrowOrientation orientation;
        if (expanderOpen) {
            orientation = ArrowDown;
        } else if (reverseLayout) {
            orientation = ArrowLeft;
        } else {
            orientation = ArrowRight;
        }

        // Style options
        StyleOptions styleOptions(option->palette, _variant);
        styleOptions.setColorRole(QPalette::Text);
        styleOptions.setPainter(painter);
        styleOptions.setRect(arrowRect);
        styleOptions.setColor(mouseOver ? Colors::hoverColor(StyleOptions(palette, _variant)) : Colors::arrowOutlineColor(styleOptions));

        // render
        Adwaita::Renderer::renderArrow(styleOptions, orientation);
    }

    // tree branches
    if (!Adwaita::Config::ViewDrawTreeBranchLines) {
        return true;
    }

    QPoint center(rect.center());
    QColor lineColor(Colors::mix(palette.color(QPalette::Base), palette.color(QPalette::Text), 0.25));
    painter->save();
    painter->setRenderHint(QPainter::Antialiasing, true);
    painter->translate(0.5, 0.5);
    painter->setPen(QPen(lineColor, 1));
    if (state & (State_Item | State_Children | State_Sibling)) {
        QLineF line(QPointF(center.x(), rect.top()), QPointF(center.x(), center.y() - expanderAdjust - 1));
        painter->drawLine(line);
    }

    // The right/left (depending on direction) line gets drawn if we have an item
    if (state & State_Item) {
        const QLineF line = reverseLayout ?
                            QLineF(QPointF(rect.left(), center.y()), QPointF(center.x() - expanderAdjust, center.y())) :
                            QLineF(QPointF(center.x() + expanderAdjust, center.y()), QPointF(rect.right(), center.y()));
        painter->drawLine(line);
    }

    // The bottom if we have a sibling
    if (state & State_Sibling) {
        QLineF line(QPointF(center.x(), center.y() + expanderAdjust), QPointF(center.x(), rect.bottom()));
        painter->drawLine(line);
    }
    painter->restore();

    return true;
}

//___________________________________________________________________________________
bool Style::drawPushButtonLabelControl(const QStyleOption *option, QPainter *painter, const QWidget *widget) const
{
    // cast option and check
    const QStyleOptionButton *buttonOption(qstyleoption_cast<const QStyleOptionButton *>(option));
    if (!buttonOption) {
        return true;
    }

    // copy rect and palette
    const QRect &rect(option->rect);
    const QPalette &palette(option->palette);

    // state
    const State &state(option->state);
    bool enabled(state & State_Enabled);
    bool sunken(state & (State_On | State_Sunken));
    bool mouseOver((state & State_Active) && enabled && (option->state & State_MouseOver));
    bool hasFocus(enabled && !mouseOver && (option->state & State_HasFocus));
    bool flat(buttonOption->features & QStyleOptionButton::Flat);

    // content
    bool hasText(!buttonOption->text.isEmpty());
    bool hasIcon((showIconsOnPushButtons() || flat || !hasText) && !buttonOption->icon.isNull());

    // contents
    QRect contentsRect(rect);

    // color role
    QPalette::ColorRole textRole;
    if (flat) {
        if (hasFocus && sunken) {
            textRole = QPalette::ButtonText;
        } else {
            textRole = QPalette::WindowText;
        }
    } else if (hasFocus) {
        textRole = QPalette::ButtonText;
    } else {
        textRole = QPalette::ButtonText;
    }

    // menu arrow
    if (buttonOption->features & QStyleOptionButton::HasMenu) {
        // define rect
        QRect arrowRect(contentsRect);
        arrowRect.setLeft(contentsRect.right() - Metrics::MenuButton_IndicatorWidth + 1);
        arrowRect = centerRect(arrowRect, Metrics::MenuButton_IndicatorWidth, Metrics::MenuButton_IndicatorWidth);

        contentsRect.setRight(arrowRect.left() - Metrics::Button_ItemSpacing - 1);
        contentsRect.adjust(Metrics::Button_MarginWidth, 0, 0, 0);

        arrowRect = visualRect(option, arrowRect);

        // Style options
        StyleOptions styleOptions(option->palette, _variant);
        styleOptions.setColorRole(textRole);
        styleOptions.setPainter(painter);
        styleOptions.setRect(arrowRect);
        styleOptions.setColor(Colors::arrowOutlineColor(styleOptions));

        Adwaita::Renderer::renderArrow(styleOptions, ArrowDown);
    }

    // icon size
    QSize iconSize;
    if (hasIcon) {
        iconSize = buttonOption->iconSize;
        if (!iconSize.isValid()) {
            int metric(pixelMetric(PM_SmallIconSize, option, widget));
            iconSize = QSize(metric, metric);
        }
    }

    // text size
    int textFlags(_mnemonics->textFlags() | Qt::AlignCenter);
    QSize textSize(option->fontMetrics.size(textFlags, buttonOption->text));

    // adjust text and icon rect based on options
    QRect iconRect;
    QRect textRect;

    if (hasText && !hasIcon) {
        textRect = contentsRect;
    } else if (hasIcon && !hasText) {
        iconRect = contentsRect;
    } else {
        int contentsWidth(iconSize.width() + textSize.width() + Metrics::Button_ItemSpacing);
        iconRect = QRect(QPoint(contentsRect.left() + (contentsRect.width() - contentsWidth) / 2, contentsRect.top() + (contentsRect.height() - iconSize.height()) / 2), iconSize);
        textRect = QRect(QPoint(iconRect.right() + Metrics::ToolButton_ItemSpacing + 1, contentsRect.top() + (contentsRect.height() - textSize.height()) / 2), textSize);
    }

    // handle right to left
    if (iconRect.isValid()) {
        iconRect = visualRect(option, iconRect);
    }
    if (textRect.isValid()) {
        textRect = visualRect(option, textRect);
    }

    // make sure there is enough room for icon
    if (iconRect.isValid()) {
        iconRect = centerRect(iconRect, iconSize);
    }

    // render icon
    if (hasIcon && iconRect.isValid()) {
        // icon state and mode
        const QIcon::State iconState(sunken ? QIcon::On : QIcon::Off);
        QIcon::Mode iconMode;
        if (!enabled) {
            iconMode = QIcon::Disabled;
        } else if (!flat && hasFocus) {
            iconMode = QIcon::Selected;
        } else if (mouseOver && flat) {
            iconMode = QIcon::Active;
        } else {
            iconMode = QIcon::Normal;
        }

        QPixmap pixmap = buttonOption->icon.pixmap(iconSize, iconMode, iconState);
        drawItemPixmap(painter, iconRect, Qt::AlignCenter, pixmap);
    }

    // render text
    if (hasText && textRect.isValid()) {
        if (enabled && !sunken && !mouseOver && !flat) {
            if (_dark) {
                drawItemText(painter, textRect.adjusted(0, -1, 0, -1), textFlags, palette, false, buttonOption->text, QPalette::Dark);
            } else {
                drawItemText(painter, textRect.adjusted(0, 1, 0, 1), textFlags, palette, false, buttonOption->text, QPalette::Light);
            }
        }
        drawItemText(painter, textRect, textFlags, palette, enabled, buttonOption->text, textRole);
    }

    return true;
}

//___________________________________________________________________________________
bool Style::drawToolButtonLabelControl(const QStyleOption *option, QPainter *painter, const QWidget *widget) const
{
    // cast option and check
    const QStyleOptionToolButton *toolButtonOption(qstyleoption_cast<const QStyleOptionToolButton *>(option));

    // copy rect and palette
    const QRect &rect = option->rect;
    const QPalette &palette = option->palette;

    // state
    const State &state(option->state);
    bool enabled(state & State_Enabled);
    bool sunken(state & (State_On | State_Sunken));
    bool mouseOver((state & State_Active) && enabled && (option->state & State_MouseOver));
    bool flat(state & State_AutoRaise);

    // focus flag is set to match the background color in either renderButtonFrame or renderToolButtonFrame
    bool hasFocus(false);
    if (flat) {
        hasFocus = enabled && !mouseOver && (option->state & State_HasFocus);
    } else {
        hasFocus = enabled && !mouseOver && (option->state & (State_HasFocus | State_Sunken));
    }

    bool hasArrow(toolButtonOption->features & QStyleOptionToolButton::Arrow);
    bool hasIcon(!(hasArrow || toolButtonOption->icon.isNull()));
    bool hasText(!toolButtonOption->text.isEmpty());

    // contents
    QRect contentsRect(rect);

    // icon size
    QSize iconSize(toolButtonOption->iconSize);

    // text size
    int textFlags(_mnemonics->textFlags());
    QSize textSize(option->fontMetrics.size(textFlags, toolButtonOption->text));

    // adjust text and icon rect based on options
    QRect iconRect;
    QRect textRect;

    if (hasText && (!(hasArrow || hasIcon) || toolButtonOption->toolButtonStyle == Qt::ToolButtonTextOnly)) {
        // text only
        textRect = contentsRect;
        textFlags |= Qt::AlignCenter;
    } else if ((hasArrow || hasIcon) && (!hasText || toolButtonOption->toolButtonStyle == Qt::ToolButtonIconOnly)) {
        // icon only
        iconRect = contentsRect;
    } else if (toolButtonOption->toolButtonStyle == Qt::ToolButtonTextUnderIcon) {
        int contentsHeight(iconSize.height() + textSize.height() + Metrics::ToolButton_ItemSpacing);
        iconRect = QRect(QPoint(contentsRect.left() + (contentsRect.width() - iconSize.width()) / 2, contentsRect.top() + (contentsRect.height() - contentsHeight) / 2), iconSize);
        textRect = QRect(QPoint(contentsRect.left() + (contentsRect.width() - textSize.width()) / 2, iconRect.bottom() + Metrics::ToolButton_ItemSpacing + 1), textSize);
        textFlags |= Qt::AlignCenter;
    } else {
        bool leftAlign(widget && widget->property(PropertyNames::toolButtonAlignment).toInt() == Qt::AlignLeft);
        if (leftAlign) {
            iconRect = QRect(QPoint(contentsRect.left(), contentsRect.top() + (contentsRect.height() - iconSize.height()) / 2), iconSize);
        } else {
            int contentsWidth(iconSize.width() + textSize.width() + Metrics::ToolButton_ItemSpacing);
            iconRect = QRect(QPoint(contentsRect.left() + (contentsRect.width() - contentsWidth) / 2, contentsRect.top() + (contentsRect.height() - iconSize.height()) / 2), iconSize);
        }

        textRect = QRect(QPoint(iconRect.right() + Metrics::ToolButton_ItemSpacing + 1, contentsRect.top() + (contentsRect.height() - textSize.height()) / 2), textSize);

        // handle right to left layouts
        iconRect = visualRect(option, iconRect);
        textRect = visualRect(option, textRect);

        textFlags |= Qt::AlignLeft | Qt::AlignVCenter;
    }

    // make sure there is enough room for icon
    if (iconRect.isValid()) {
        iconRect = centerRect(iconRect, iconSize);
    }

    // render arrow or icon
    if (hasArrow && iconRect.isValid()) {
        QStyleOptionToolButton copy(*toolButtonOption);
        copy.rect = iconRect;
        switch (toolButtonOption->arrowType) {
        case Qt::LeftArrow:
            drawPrimitive(PE_IndicatorArrowLeft, &copy, painter, widget);
            break;
        case Qt::RightArrow:
            drawPrimitive(PE_IndicatorArrowRight, &copy, painter, widget);
            break;
        case Qt::UpArrow:
            drawPrimitive(PE_IndicatorArrowUp, &copy, painter, widget);
            break;
        case Qt::DownArrow:
            drawPrimitive(PE_IndicatorArrowDown, &copy, painter, widget);
            break;
        default:
            break;
        }

    } else if (hasIcon && iconRect.isValid()) {
        // icon state and mode
        const QIcon::State iconState(sunken ? QIcon::On : QIcon::Off);
        QIcon::Mode iconMode;
        if (!enabled) {
            iconMode = QIcon::Disabled;
        } else if (!flat && hasFocus) {
            iconMode = QIcon::Selected;
        } else if (mouseOver && flat) {
            iconMode = QIcon::Active;
        } else {
            iconMode = QIcon::Normal;
        }

        QPixmap pixmap = toolButtonOption->icon.pixmap(iconSize, iconMode, iconState);
        drawItemPixmap(painter, iconRect, Qt::AlignCenter, pixmap);
    }

    // render text
    if (hasText && textRect.isValid()) {
        QPalette::ColorRole textRole(QPalette::ButtonText);
        if (flat) {
            textRole = (hasFocus && sunken && !mouseOver) ? QPalette::HighlightedText : QPalette::WindowText;
        } else if (hasFocus && !mouseOver) {
            textRole = QPalette::HighlightedText;
        }

        painter->setFont(toolButtonOption->font);
        drawItemText(painter, textRect, textFlags, palette, enabled, toolButtonOption->text, textRole);
    }

    return true;
}

//___________________________________________________________________________________
bool Style::drawCheckBoxLabelControl(const QStyleOption *option, QPainter *painter, const QWidget *widget) const
{
    // cast option and check
    const QStyleOptionButton *buttonOption(qstyleoption_cast<const QStyleOptionButton *>(option));
    if (!buttonOption) {
        return true;
    }

    // copy palette and rect
    const QPalette &palette(option->palette);
    const QRect &rect(option->rect);

    // store state
    const State &state(option->state);
    bool enabled(state & State_Enabled);

    // text alignment
    bool reverseLayout(option->direction == Qt::RightToLeft);
    int textFlags(_mnemonics->textFlags() | Qt::AlignVCenter | (reverseLayout ? Qt::AlignRight : Qt::AlignLeft));

    // text rect
    QRect textRect(rect);

    // render icon
    if (!buttonOption->icon.isNull()) {
        const QIcon::Mode mode(enabled ? QIcon::Normal : QIcon::Disabled);
        QPixmap pixmap(buttonOption->icon.pixmap(buttonOption->iconSize, mode));
        drawItemPixmap(painter, rect, textFlags, pixmap);

        // adjust rect (copied from QCommonStyle)
        textRect.setLeft(textRect.left() + buttonOption->iconSize.width() + 4);
        textRect = visualRect(option, textRect);
    }

    // render text
    if (!buttonOption->text.isEmpty()) {
        textRect = option->fontMetrics.boundingRect(textRect, textFlags, buttonOption->text);
        drawItemText(painter, textRect, textFlags, palette, enabled, buttonOption->text, QPalette::Text);
    }

    return true;
}


//___________________________________________________________________________________
bool Style::drawComboBoxLabelControl(const QStyleOption *option, QPainter *painter, const QWidget *widget) const
{
    const QStyleOptionComboBox *comboBoxOption(qstyleoption_cast<const QStyleOptionComboBox *>(option));
    if (!comboBoxOption) {
        return false;
    }
    if (comboBoxOption->editable) {
        return false;
    }

    QPalette::ColorRole textRole = QPalette::ButtonText;

    // change pen color directly
    painter->save();
    painter->setPen(QPen(option->palette.color(textRole), 1));

    if (const QStyleOptionComboBox *cb = qstyleoption_cast<const QStyleOptionComboBox *>(option)) {
        QRect editRect = proxy()->subControlRect(CC_ComboBox, cb, SC_ComboBoxEditField, widget);
        painter->save();
        painter->setClipRect(editRect);
        if (!cb->currentIcon.isNull() && qobject_cast<const QComboBox *>(widget)) {
            QIcon::Mode mode;

            if ((cb->state & QStyle::State_Selected) && (cb->state & QStyle::State_Active)) {
                mode = QIcon::Selected;
            } else if (cb->state & QStyle::State_Enabled) {
                mode = QIcon::Normal;
            } else {
                mode = QIcon::Disabled;
            }

            QPixmap pixmap = cb->currentIcon.pixmap(cb->iconSize, mode);
            QRect iconRect(editRect);
            iconRect.setWidth(cb->iconSize.width() + 4);
            iconRect = alignedRect(cb->direction,
                                   Qt::AlignLeft | Qt::AlignVCenter,
                                   iconRect.size(), editRect);
            if (cb->editable) {
                painter->fillRect(iconRect, option->palette.brush(QPalette::Base));
            }
            proxy()->drawItemPixmap(painter, iconRect, Qt::AlignCenter, pixmap);

            if (cb->direction == Qt::RightToLeft) {
                editRect.translate(-4 - cb->iconSize.width(), 0);
            } else {
                editRect.translate(cb->iconSize.width() + 4, 0);
            }
        }
        if (!cb->currentText.isEmpty() && !cb->editable) {
            proxy()->drawItemText(painter, editRect.adjusted(Metrics::ComboBox_MarginWidth, 0, -1, 0),
                                  visualAlignment(cb->direction, Qt::AlignLeft | Qt::AlignVCenter),
                                  cb->palette, cb->state & State_Enabled, cb->currentText);
        }
        painter->restore();
    }

    painter->restore();

    return true;
}

//
bool Style::drawItemViewItemControl(const QStyleOption *option, QPainter *painter, const QWidget *widget) const
{
    const QStyleOptionViewItem *vopt = qstyleoption_cast<const QStyleOptionViewItem *>(option);
    if (!vopt) {
        return true;
    }

    QStyleOptionViewItem op(*vopt);

    if (_helper->isWindowActive(widget)) {
        // No custom text color used, we can do our HACK
        QPalette pal = op.palette;
        if (!hasCustomTextColors(pal)) {
            pal.setColor(QPalette::Inactive, QPalette::Text, pal.color(QPalette::Active, QPalette::Text));
            op.palette = pal;
        }
    }

    ParentStyleClass::drawControl(CE_ItemViewItem, &op, painter, widget);

    return true;
}

//___________________________________________________________________________________
bool Style::drawMenuBarEmptyArea(const QStyleOption *option, QPainter *painter, const QWidget *) const
{
    const QRect &rect(option->rect);
    const QPalette &palette(option->palette);

    painter->save();
    painter->setRenderHint(QPainter::Antialiasing, false);
    painter->setBrush(Qt::NoBrush);
    painter->setPen(Colors::mix(palette.color(QPalette::Window), palette.color(QPalette::Shadow), 0.2));

    painter->drawLine(rect.bottomLeft(), rect.bottomRight());
    painter->restore();

    return true;
}

//___________________________________________________________________________________
bool Style::drawMenuBarItemControl(const QStyleOption *option, QPainter *painter, const QWidget *) const
{

    // cast option and check
    const QStyleOptionMenuItem *menuItemOption = qstyleoption_cast<const QStyleOptionMenuItem *>(option);
    if (!menuItemOption)
        return true;

    // copy rect and palette
    const QRect &rect(option->rect);
    const QPalette &palette(option->palette);

    // store state
    const State &state(option->state);
    bool enabled(state & State_Enabled);
    bool sunken(enabled && (state & State_Sunken));
    bool useStrongFocus(Adwaita::Config::MenuItemDrawStrongFocus);

    painter->save();
    painter->setRenderHint(QPainter::Antialiasing, false);

    painter->setBrush(palette.window().color());
    painter->setPen(Qt::NoPen);
    painter->drawRect(rect);

    painter->setBrush(Qt::NoBrush);
    painter->setPen(Colors::mix(palette.color(QPalette::Window), palette.color(QPalette::Shadow), 0.2));

    painter->drawLine(rect.bottomLeft(), rect.bottomRight());
    painter->restore();

    // render hover and focus
    if (useStrongFocus && sunken) {
        StyleOptions styleOptions(painter, QRect(rect.left(), rect.bottom() - 2, rect.width(), 3));
        styleOptions.setColorVariant(_variant);
        styleOptions.setOutlineColor(Colors::focusColor(StyleOptions(palette, _variant)));
        styleOptions.setColor(palette.color(QPalette::Highlight));
        Adwaita::Renderer::renderFocusRect(styleOptions);
    }

    // get text rect
    int textFlags(Qt::AlignCenter | _mnemonics->textFlags());
    QRect textRect = option->fontMetrics.boundingRect(rect, textFlags, menuItemOption->text);

    // render text
    const QPalette::ColorRole role = (useStrongFocus && sunken) ? QPalette::Link : QPalette::WindowText;
    drawItemText(painter, textRect, textFlags, palette, enabled, menuItemOption->text, role);

    return true;
}

//___________________________________________________________________________________
bool Style::drawMenuItemControl(const QStyleOption *option, QPainter *painter, const QWidget *widget) const
{
    // cast option and check
    const QStyleOptionMenuItem *menuItemOption = qstyleoption_cast<const QStyleOptionMenuItem *>(option);
    if (!menuItemOption) {
        return true;
    }
    if (menuItemOption->menuItemType == QStyleOptionMenuItem::EmptyArea) {
        return true;
    }

    // copy rect and palette
    QRect rect(option->rect);
    const QPalette &palette(option->palette);

    // leave room for the menu border
    rect.adjust(1, 0, -1, 0);

    // deal with separators
    if (menuItemOption->menuItemType == QStyleOptionMenuItem::Separator) {
        // normal separator
        if (menuItemOption->text.isEmpty() && menuItemOption->icon.isNull()) {
            // Style options
            StyleOptions styleOptions(option->palette, _variant);
            styleOptions.setPainter(painter);
            styleOptions.setRect(rect);
            styleOptions.setColor(Colors::separatorColor(styleOptions));

            Adwaita::Renderer::renderSeparator(styleOptions);
            return true;
        } else {
            /*
             * separator can have a title and an icon
             * in that case they are rendered as menu title buttons
             */
            QStyleOptionToolButton copy(separatorMenuItemOption(menuItemOption, widget));
            renderMenuTitle(&copy, painter, widget);

            return true;
        }
    }

    // store state
    const State &state(option->state);
    bool enabled(state & State_Enabled);
    bool windowActive(state & State_Active);
    bool selected(enabled && (state & State_Selected));
    bool mouseOver((state & State_Active) && enabled && (state & State_MouseOver));
    bool sunken(enabled && (state & (State_On | State_Sunken)));
    bool reverseLayout(option->direction == Qt::RightToLeft);
    bool useStrongFocus(Adwaita::Config::MenuItemDrawStrongFocus);

    // render hover and focus
    if (useStrongFocus && (selected || sunken)) {
        StyleOptions styleOptions(painter, rect);
        styleOptions.setColor(Colors::selectedMenuColor(StyleOptions(palette, _variant)));
        styleOptions.setColorVariant(_variant);
        styleOptions.setOutlineColor(Qt::transparent);
        Adwaita::Renderer::renderFocusRect(styleOptions);
    }

    // get rect available for contents
    QRect contentsRect(insideMargin(rect,  Metrics::MenuItem_MarginWidth));

    // define relevant rectangles
    // checkbox
    QRect checkBoxRect;
    if (menuItemOption->menuHasCheckableItems) {
        checkBoxRect = QRect(contentsRect.left(), contentsRect.top() + (contentsRect.height() - Metrics::CheckBox_Size) / 2, Metrics::CheckBox_Size, Metrics::CheckBox_Size).adjusted(1, 1, -1, -1);
    }

    // We want to always to keep the space for checkbox
    contentsRect.setLeft(rect.left() + Metrics::CheckBox_Size + Metrics::MenuItem_ItemSpacing);

    CheckBoxState checkState(menuItemOption->checked ? CheckOn : CheckOff);

    // Style options
    StyleOptions styleOptions(option->palette, _variant);
    styleOptions.setMouseOver(mouseOver);
    styleOptions.setHasFocus(false);
    styleOptions.setSunken(false);
    styleOptions.setOpacity(AnimationData::OpacityInvalid);
    styleOptions.setAnimationMode(AnimationNone);
    styleOptions.setCheckboxState(checkState);
    styleOptions.setInMenu(true);

    const QColor &outline(palette.windowText().color());
    const QColor &indicatorBackground(Colors::indicatorBackgroundColor(styleOptions));
    // render checkbox indicator
    if (menuItemOption->checkType == QStyleOptionMenuItem::NonExclusive) {
        checkBoxRect = visualRect(option, checkBoxRect);

        // checkbox state

        /*
        if( useStrongFocus && ( selected || sunken ) )
        { _helper->renderCheckBoxBackground( painter, checkBoxRect, palette.color( QPalette::Window ), outline, sunken ); }
        */

        bool active(menuItemOption->checked);
        AnimationMode mode(_animations->widgetStateEngine().isAnimated(widget, AnimationHover) ? AnimationHover : AnimationNone);
        qreal opacity(_animations->widgetStateEngine().opacity(widget, AnimationHover));

        // Style options
        styleOptions.setActive(enabled && active);
        styleOptions.setAnimationMode(mode);
        styleOptions.setOpacity(opacity);
        styleOptions.setPainter(painter);
        styleOptions.setRect(checkBoxRect);
        styleOptions.setColor(indicatorBackground);
        styleOptions.setOutlineColor(outline);
        styleOptions.setSunken(false);
        styleOptions.setActive(enabled && windowActive);

        QColor tickColor = Colors::checkBoxIndicatorColor(styleOptions);
        Adwaita::Renderer::renderCheckBox(styleOptions, tickColor);
    } else if (menuItemOption->checkType == QStyleOptionMenuItem::Exclusive) {
        checkBoxRect = visualRect(option, checkBoxRect);

        /*
        if( useStrongFocus && ( selected || sunken ) )
        { _helper->renderRadioButtonBackground( painter, checkBoxRect, palette.color( QPalette::Window ), outline, sunken ); }
        */

        bool active(menuItemOption->checked);
        AnimationMode mode(_animations->widgetStateEngine().isAnimated(widget, AnimationHover) ? AnimationHover : AnimationNone);
        qreal opacity(_animations->widgetStateEngine().opacity(widget, AnimationHover));

        // Style options
        styleOptions.setActive(enabled && active);
        styleOptions.setAnimationMode(mode);
        styleOptions.setOpacity(opacity);
        styleOptions.setPainter(painter);
        styleOptions.setRect(checkBoxRect);
        styleOptions.setColor(indicatorBackground);
        styleOptions.setOutlineColor(outline);
        styleOptions.setSunken(false);
        styleOptions.setActive(enabled && windowActive);
        styleOptions.setRadioButtonState(active ? RadioOn : RadioOff);

        QColor tickColor = Colors::checkBoxIndicatorColor(styleOptions);
        Adwaita::Renderer::renderRadioButton(styleOptions, tickColor);
    }

    // icon
    int iconWidth = 0;
    bool showIcon(showIconsInMenuItems());
    if (showIcon) {
        iconWidth = isQtQuickControl(option, widget) ? qMax(pixelMetric(PM_SmallIconSize, option, widget), menuItemOption->maxIconWidth) : menuItemOption->maxIconWidth;
    }

    QRect iconRect(contentsRect.left(), contentsRect.top() + (contentsRect.height() - iconWidth) / 2, iconWidth, iconWidth);
    contentsRect.setLeft(iconRect.right() + Metrics::MenuItem_ItemSpacing + 1);

    if (showIcon && !menuItemOption->icon.isNull()) {
        QSize iconSize(pixelMetric(PM_SmallIconSize, option, widget), pixelMetric(PM_SmallIconSize, option, widget));
        iconRect = centerRect(iconRect, iconSize);
        iconRect = visualRect(option, iconRect);

        // icon mode
        QIcon::Mode mode;
        if (selected && !useStrongFocus) {
            mode = QIcon::Active;
        } else if (selected) {
            mode = QIcon::Selected;
        } else if (enabled) {
            mode = QIcon::Normal;
        } else {
            mode = QIcon::Disabled;
        }

        // icon state
        const QIcon::State iconState(sunken ? QIcon::On : QIcon::Off);
        QPixmap icon = menuItemOption->icon.pixmap(iconRect.size(), mode, iconState);
        painter->drawPixmap(iconRect, icon);
    }

    const QColor acceleratorColor = Colors::transparentize(palette.color(QPalette::Active, QPalette::WindowText), 0.55);

    // arrow
    QRect arrowRect(contentsRect.right() - Metrics::MenuButton_IndicatorWidth + 1, contentsRect.top() + (contentsRect.height() - Metrics::MenuButton_IndicatorWidth) / 2, Metrics::MenuButton_IndicatorWidth, Metrics::MenuButton_IndicatorWidth);

    if (menuItemOption->menuItemType == QStyleOptionMenuItem::SubMenu) {
        // apply right-to-left layout
        arrowRect = visualRect(option, arrowRect);

        // arrow orientation
        ArrowOrientation orientation(reverseLayout ? ArrowLeft : ArrowRight);

        styleOptions.setPainter(painter);
        styleOptions.setRect(arrowRect);
        styleOptions.setColor(acceleratorColor);
        styleOptions.setColorVariant(_variant);

        // render
        Adwaita::Renderer::renderArrow(styleOptions, orientation);
    }

    // text
    QRect textRect = contentsRect;
    if (!menuItemOption->text.isEmpty()) {
        // adjust textRect
        QString text = menuItemOption->text;

        textRect = centerRect(textRect, textRect.width(), option->fontMetrics.size(_mnemonics->textFlags(), text).height());
        textRect = visualRect(option, textRect);

        const int arrowWidth = menuItemOption->menuItemType == QStyleOptionMenuItem::SubMenu ? Metrics::MenuButton_IndicatorWidth : 0;
        textRect.setRight(textRect.right() - Metrics::MenuItem_MarginWidth - arrowWidth);

        // set font
        painter->setFont(menuItemOption->font);

        // locate accelerator and render
        int tabPosition(text.indexOf(QLatin1Char('\t')));
        if (tabPosition >= 0) {
            int textFlags(Qt::AlignVCenter | Qt::AlignRight);
            QString accelerator(text.mid(tabPosition + 1));
            text = text.left(tabPosition);
            QPalette copy(palette);
            copy.setColor(QPalette::Active, QPalette::WindowText, acceleratorColor);
            copy.setColor(QPalette::Active, QPalette::HighlightedText, Colors::transparentize(copy.color(QPalette::Active, QPalette::HighlightedText), 0.55));
            drawItemText(painter, textRect, textFlags, copy, enabled, accelerator, QPalette::WindowText);
        }

        // render text
        int textFlags(Qt::AlignVCenter | (reverseLayout ? Qt::AlignRight : Qt::AlignLeft) | _mnemonics->textFlags());
        textRect = option->fontMetrics.boundingRect(textRect, textFlags, text);
        drawItemText(painter, textRect, textFlags, palette, enabled, text, QPalette::WindowText);
    }

    return true;
}

//___________________________________________________________________________________
bool Style::drawProgressBarControl(const QStyleOption *option, QPainter *painter, const QWidget *widget) const
{
    const QStyleOptionProgressBar *progressBarOptionTmp(qstyleoption_cast<const QStyleOptionProgressBar *>(option));
    if (!progressBarOptionTmp)
        return true;

    QStyleOptionProgressBar progressBarOption = *progressBarOptionTmp;

    // render groove
    progressBarOption.rect = subElementRect(SE_ProgressBarGroove, progressBarOptionTmp, widget);
    drawControl(CE_ProgressBarGroove, &progressBarOption, painter, widget);

    const QObject *styleObject(widget ? widget : progressBarOptionTmp->styleObject);

    // enable busy animations
    // need to check both widget and passed styleObject, used for QML
    if (styleObject && _animations->busyIndicatorEngine().enabled()) {

        // register QML object if defined
        if (!widget && progressBarOptionTmp->styleObject) {
            _animations->busyIndicatorEngine().registerWidget(progressBarOptionTmp->styleObject);
        }

        _animations->busyIndicatorEngine().setAnimated(styleObject, progressBarOptionTmp->maximum == 0 && progressBarOptionTmp->minimum == 0);
    }

    // check if animated and pass to option
    if (_animations->busyIndicatorEngine().isAnimated(styleObject)) {
        progressBarOption.progress = _animations->busyIndicatorEngine().value();
    }

    // render contents
    progressBarOption.rect = subElementRect(SE_ProgressBarContents, progressBarOptionTmp, widget);
    drawControl(CE_ProgressBarContents, &progressBarOption, painter, widget);

    // render text
    bool textVisible(progressBarOptionTmp->textVisible);
    bool busy(progressBarOptionTmp->minimum == 0 && progressBarOptionTmp->maximum == 0);
    if (textVisible && !busy) {
        progressBarOption.rect = subElementRect(SE_ProgressBarLabel, progressBarOptionTmp, widget);
        drawControl(CE_ProgressBarLabel, &progressBarOption, painter, widget);
    }

    return true;
}

//___________________________________________________________________________________
bool Style::drawProgressBarContentsControl(const QStyleOption *option, QPainter *painter, const QWidget *) const
{
    const QStyleOptionProgressBar *progressBarOption(qstyleoption_cast<const QStyleOptionProgressBar *>(option));
    if (!progressBarOption)
        return true;

    // copy rect and palette
    QRect rect(option->rect);
    const QPalette &palette(option->palette);

    // get direction
    const State &state(option->state);
#if QT_VERSION >= 0x060000
    bool horizontal(state & QStyle::State_Horizontal);
#else
    bool horizontal((state & QStyle::State_Horizontal) || (progressBarOption->orientation == Qt::Horizontal));
#endif
    bool inverted(progressBarOption ? progressBarOption->invertedAppearance : false);
    bool reverse = horizontal && option->direction == Qt::RightToLeft;
    if (inverted)
        reverse = !reverse;

    // check if anything is to be drawn
    bool busy((progressBarOption->minimum == 0 && progressBarOption->maximum == 0));
    if (busy) {
        qreal progress(_animations->busyIndicatorEngine().value());

        QColor color(palette.color(QPalette::Highlight));

        StyleOptions styleOptions(painter, rect);
        styleOptions.setColor(color);
        styleOptions.setColorVariant(_variant);
        styleOptions.setOutlineColor(color);

        Adwaita::Renderer::renderProgressBarBusyContents(styleOptions, horizontal, reverse, progress);
    } else {
        QRegion oldClipRegion(painter->clipRegion());
        if (horizontal) {
            if (rect.width() < Metrics::ProgressBar_Thickness) {
                painter->setClipRect(rect, Qt::IntersectClip);
                if (reverse) {
                    rect.setLeft(rect.left() - Metrics::ProgressBar_Thickness + rect.width());
                } else {
                    rect.setWidth(Metrics::ProgressBar_Thickness);
                }
            }
        } else {
            if (rect.height() < Metrics::ProgressBar_Thickness) {
                painter->setClipRect(rect, Qt::IntersectClip);
                if (reverse) {
                    rect.setHeight(Metrics::ProgressBar_Thickness);
                } else {
                    rect.setTop(rect.top() - Metrics::ProgressBar_Thickness + rect.height());
                }
            }
        }

        StyleOptions styleOptions(painter, rect);
        styleOptions.setColor(palette.color(QPalette::Highlight));
        styleOptions.setColorVariant(_variant);
        styleOptions.setOutlineColor(palette.color(QPalette::Highlight));
        Adwaita::Renderer::renderProgressBarContents(styleOptions);
        painter->setClipRegion(oldClipRegion);
    }

    return true;
}

//___________________________________________________________________________________
bool Style::drawProgressBarGrooveControl(const QStyleOption *option, QPainter *painter, const QWidget *) const
{
    const QPalette &palette(option->palette);

    // Style options
    StyleOptions styleOptions(option->palette, _variant);
    styleOptions.setHasFocus(false);
    styleOptions.setSunken(false);
    styleOptions.setOpacity(AnimationData::OpacityInvalid);
    styleOptions.setAnimationMode(AnimationNone);
    styleOptions.setPainter(painter);
    styleOptions.setRect(option->rect);
    styleOptions.setColor(Colors::mix(Colors::buttonOutlineColor(styleOptions), palette.color(QPalette::Window)));
    styleOptions.setOutlineColor(Colors::mix(Colors::buttonOutlineColor(styleOptions), palette.color(QPalette::Window)));

    Adwaita::Renderer::renderProgressBarGroove(styleOptions);
    return true;
}

//___________________________________________________________________________________
bool Style::drawProgressBarLabelControl(const QStyleOption *option, QPainter *painter, const QWidget *) const
{
    // cast option and check
    const QStyleOptionProgressBar *progressBarOption(qstyleoption_cast<const QStyleOptionProgressBar *>(option));
    if (!progressBarOption) {
        return true;
    }

    // get direction and check
    const State &state(option->state);
#if QT_VERSION >= 0x060000
    bool horizontal(state & QStyle::State_Horizontal);
#else
    bool horizontal((state & QStyle::State_Horizontal) || (progressBarOption->orientation == Qt::Horizontal));
#endif

    if (!horizontal) {
        return true;
    }

    // store rect and palette
    const QRect &rect(option->rect);

    QPalette palette(option->palette);
    palette.setColor(QPalette::WindowText, Colors::transparentize(palette.color(QPalette::Active, QPalette::WindowText), 0.6));

    // store state and direction
    bool enabled(state & State_Enabled);

    // define text rect
    Qt::Alignment hAlign((progressBarOption->textAlignment == Qt::AlignLeft) ? Qt::AlignHCenter : progressBarOption->textAlignment);
    drawItemText(painter, rect, Qt::AlignVCenter | hAlign, palette, enabled, progressBarOption->text, QPalette::WindowText);

    return true;
}

//___________________________________________________________________________________
bool Style::drawScrollBarSliderControl(const QStyleOption *option, QPainter *painter, const QWidget *widget) const
{
    // cast option and check
    const QStyleOptionSlider *sliderOption(qstyleoption_cast<const QStyleOptionSlider *>(option));
    if (!sliderOption) {
        return true;
    }

    const State &state(option->state);
    bool horizontal(state & State_Horizontal);

    // copy rect and palette
    const QRect &rect(horizontal ? option->rect.adjusted(-1, 4, 0, -4) : option->rect.adjusted(4, -1, -4, 0));

    // define handle rect
    QRect handleRect;

    bool enabled(state & State_Enabled);
    bool mouseOver((state & State_Active) && enabled && (state & State_MouseOver));
    bool sunken(enabled && (state & (State_On | State_Sunken)));

    // check focus from relevant parent
    const QWidget *parent(scrollBarParent(widget));
    bool hasFocus(enabled && parent && parent->hasFocus());

    // enable animation state
    _animations->scrollBarEngine().updateState(widget, AnimationFocus, hasFocus);
    _animations->scrollBarEngine().updateState(widget, AnimationPressed, sunken);
    _animations->scrollBarEngine().updateState(widget, AnimationHover, mouseOver);

    AnimationMode mode(_animations->scrollBarEngine().animationMode(widget, SC_ScrollBarSlider));
    qreal opacity(_animations->scrollBarEngine().opacity(widget, SC_ScrollBarSlider));

    // Style options
    StyleOptions styleOptions(option->palette, _variant);
    styleOptions.setMouseOver(mouseOver);
    styleOptions.setHasFocus(hasFocus);
    styleOptions.setSunken(sunken);
    styleOptions.setOpacity(opacity);
    styleOptions.setAnimationMode(mode);

    QColor color = Colors::scrollBarHandleColor(styleOptions);
    if (mouseOver) {
        opacity = 1;
    } else {
        opacity = 0;
    }

    /*
    if( horizontal ) handleRect = centerRect( rect, rect.width(), rect.height() * (0.5 + 0.5 * opacity));
    else handleRect = centerRect( rect, rect.width() * (0.5 + 0.5 * opacity), rect.height() );
    */
    if (horizontal) {
        handleRect = rect.adjusted(0, 6, 0, 2);
        handleRect.adjust(0, -6.0 * opacity, 0, -2.0 * opacity);
    } else {
        handleRect = rect.adjusted(6, 0, 2, 0);
        handleRect.adjust(-6.0 * opacity, 0, -2.0 * opacity, 0);
    }

    styleOptions.setPainter(painter);
    styleOptions.setRect(handleRect);
    styleOptions.setColor(color);

    Adwaita::Renderer::renderScrollBarHandle(styleOptions);
    return true;
}

//___________________________________________________________________________________
bool Style::drawScrollBarAddLineControl(const QStyleOption *option, QPainter *painter, const QWidget *widget) const
{
    // do nothing if no buttons are defined
    if (_addLineButtons == NoButton) {
        return true;
    }

    // cast option and check
    const QStyleOptionSlider *sliderOption(qstyleoption_cast<const QStyleOptionSlider *>(option));
    if (!sliderOption) {
        return true;
    }

    const State &state(option->state);
    bool horizontal(state & State_Horizontal);
    bool reverseLayout(option->direction == Qt::RightToLeft);

    // adjust rect, based on number of buttons to be drawn
    QRect rect(scrollBarInternalSubControlRect(sliderOption, SC_ScrollBarAddLine));

    QColor color;
    QStyleOptionSlider copy(*sliderOption);
    if (_addLineButtons == DoubleButton) {
        if (horizontal) {
            //Draw the arrows
            QSize halfSize(rect.width() / 2, rect.height());
            QRect leftSubButton(rect.topLeft(), halfSize);
            QRect rightSubButton(leftSubButton.topRight() + QPoint(1, 0), halfSize);

            copy.rect = leftSubButton;
            color = scrollBarArrowColor(&copy,  reverseLayout ? SC_ScrollBarAddLine : SC_ScrollBarSubLine, widget);

            StyleOptions styleOptions(painter, leftSubButton);
            styleOptions.setColor(color);
            styleOptions.setColorVariant(_variant);

            Adwaita::Renderer::renderArrow(styleOptions, ArrowLeft);

            copy.rect = rightSubButton;
            color = scrollBarArrowColor(&copy,  reverseLayout ? SC_ScrollBarSubLine : SC_ScrollBarAddLine, widget);

            styleOptions.setColor(color);
            styleOptions.setRect(rightSubButton);

            Adwaita::Renderer::renderArrow(styleOptions, ArrowRight);
        } else {
            QSize halfSize(rect.width(), rect.height() / 2);
            QRect topSubButton(rect.topLeft(), halfSize);
            QRect botSubButton(topSubButton.bottomLeft() + QPoint(0, 1), halfSize);

            copy.rect = topSubButton;
            color = scrollBarArrowColor(&copy, SC_ScrollBarSubLine, widget);

            StyleOptions styleOptions(painter, topSubButton);
            styleOptions.setColor(color);
            styleOptions.setColorVariant(_variant);

            Adwaita::Renderer::renderArrow(styleOptions, ArrowUp);

            copy.rect = botSubButton;
            color = scrollBarArrowColor(&copy, SC_ScrollBarAddLine, widget);

            styleOptions.setColor(color);
            styleOptions.setRect(botSubButton);

            Adwaita::Renderer::renderArrow(styleOptions, ArrowDown);
        }
    } else if (_addLineButtons == SingleButton) {
        copy.rect = rect;
        color = scrollBarArrowColor(&copy,  SC_ScrollBarAddLine, widget);

        StyleOptions styleOptions(painter, QRect());
        styleOptions.setColor(color);
        styleOptions.setColorVariant(_variant);

        if (horizontal) {
            if (reverseLayout) {
                styleOptions.setRect(rect);
                Adwaita::Renderer::renderArrow(styleOptions, ArrowLeft);
            } else {
                styleOptions.setRect(rect.translated(1, 0));
                Adwaita::Renderer::renderArrow(styleOptions, ArrowRight);
            }
        } else {
            styleOptions.setRect(rect.translated(0, 1));
            Adwaita::Renderer::renderArrow(styleOptions, ArrowDown);
        }
    }

    return true;
}

//___________________________________________________________________________________
bool Style::drawScrollBarSubLineControl(const QStyleOption *option, QPainter *painter, const QWidget *widget) const
{
    // do nothing if no buttons are set
    if (_subLineButtons == NoButton) {
        return true;
    }

    // cast option and check
    const QStyleOptionSlider *sliderOption(qstyleoption_cast<const QStyleOptionSlider *>(option));
    if (!sliderOption) {
        return true;
    }

    const State &state(option->state);
    bool horizontal(state & State_Horizontal);
    bool reverseLayout(option->direction == Qt::RightToLeft);

    // colors
    const QPalette &palette(option->palette);
    QColor background(palette.color(QPalette::Window));

    // adjust rect, based on number of buttons to be drawn
    QRect rect(scrollBarInternalSubControlRect(sliderOption, SC_ScrollBarSubLine));

    QColor color;
    QStyleOptionSlider copy(*sliderOption);
    if (_subLineButtons == DoubleButton) {
        if (horizontal) {
            //Draw the arrows
            QSize halfSize(rect.width() / 2, rect.height());
            QRect leftSubButton(rect.topLeft(), halfSize);
            QRect rightSubButton(leftSubButton.topRight() + QPoint(1, 0), halfSize);

            copy.rect = leftSubButton;
            color = scrollBarArrowColor(&copy,  reverseLayout ? SC_ScrollBarAddLine : SC_ScrollBarSubLine, widget);

            StyleOptions styleOptions(painter, leftSubButton);
            styleOptions.setColor(color);
            styleOptions.setColorVariant(_variant);

            Adwaita::Renderer::renderArrow(styleOptions, ArrowLeft);

            copy.rect = rightSubButton;
            color = scrollBarArrowColor(&copy,  reverseLayout ? SC_ScrollBarSubLine : SC_ScrollBarAddLine, widget);

            styleOptions.setColor(color);
            styleOptions.setRect(rightSubButton);

            Adwaita::Renderer::renderArrow(styleOptions, ArrowRight);
        } else {
            QSize halfSize(rect.width(), rect.height() / 2);
            QRect topSubButton(rect.topLeft(), halfSize);
            QRect botSubButton(topSubButton.bottomLeft() + QPoint(0, 1), halfSize);

            copy.rect = topSubButton;
            color = scrollBarArrowColor(&copy, SC_ScrollBarSubLine, widget);

            StyleOptions styleOptions(painter, topSubButton);
            styleOptions.setColor(color);
            styleOptions.setColorVariant(_variant);

            Adwaita::Renderer::renderArrow(styleOptions, ArrowUp);

            copy.rect = botSubButton;
            color = scrollBarArrowColor(&copy, SC_ScrollBarAddLine, widget);

            styleOptions.setColor(color);
            styleOptions.setRect(botSubButton);

            Adwaita::Renderer::renderArrow(styleOptions, ArrowDown);
        }

    } else if (_subLineButtons == SingleButton) {
        copy.rect = rect;
        color = scrollBarArrowColor(&copy,  SC_ScrollBarSubLine, widget);

        StyleOptions styleOptions(painter, QRect());
        styleOptions.setColor(color);
        styleOptions.setColorVariant(_variant);

        if (horizontal) {
            if (reverseLayout) {
                styleOptions.setRect(rect.translated(1, 0));
                Adwaita::Renderer::renderArrow(styleOptions, ArrowRight);
            } else {
                styleOptions.setRect(rect);
                Adwaita::Renderer::renderArrow(styleOptions, ArrowLeft);
            }
        } else {
            styleOptions.setRect(rect);
            Adwaita::Renderer::renderArrow(styleOptions, ArrowUp);
        }
    }

    return true;
}

//___________________________________________________________________________________
bool Style::drawShapedFrameControl(const QStyleOption *option, QPainter *painter, const QWidget *widget) const
{
    // cast option and check
    const QStyleOptionFrame *frameOpt = qstyleoption_cast<const QStyleOptionFrame *>(option);
    if (!frameOpt) {
        return false;
    }

    switch (frameOpt->frameShape) {
    case QFrame::Box: {
        if (option->state & State_Sunken) {
            return true;
        } else {
            break;
        }
    }
    case QFrame::HLine:
    case QFrame::VLine: {
        const QRect &rect(option->rect);

        // Style options
        StyleOptions styleOptions(option->palette, _variant);

        bool isVertical(frameOpt->frameShape == QFrame::VLine);

        styleOptions.setPainter(painter);
        styleOptions.setRect(rect);
        styleOptions.setColor(Colors::separatorColor(styleOptions));

        Adwaita::Renderer::renderSeparator(styleOptions, isVertical);
        return true;
    }
    case QFrame::StyledPanel: {
        if (isQtQuickControl(option, widget)) {
            // ComboBox popup frame
            drawFrameMenuPrimitive(option, painter, widget);
            return true;
        }

        break;
    }

    default:
        break;

    }

    return false;
}

//___________________________________________________________________________________
bool Style::drawRubberBandControl(const QStyleOption *option, QPainter *painter, const QWidget *) const
{
    const QPalette &palette(option->palette);
    QRect rect(option->rect);

    QColor color = palette.color(QPalette::Highlight);
    painter->setPen(Colors::mix(color, palette.color(QPalette::Active, QPalette::WindowText)));
    color.setAlpha(50);
    painter->setBrush(color);
    painter->setClipRegion(rect);
    painter->drawRect(rect.adjusted(0, 0, -1, -1));
    return true;
}

//___________________________________________________________________________________
bool Style::drawHeaderSectionControl(const QStyleOption *option, QPainter *painter, const QWidget *widget) const
{
    const QRect &rect(option->rect);
    const QPalette &palette(option->palette);
    const State &state(option->state);
    bool enabled(state & State_Enabled);
    bool mouseOver((state & State_Active) && enabled && (state & State_MouseOver));

    const QStyleOptionHeader *headerOption(qstyleoption_cast<const QStyleOptionHeader *>(option));
    if (!headerOption) {
        return true;
    }

    bool horizontal(headerOption->orientation == Qt::Horizontal);
    bool isFirst(horizontal && (headerOption->position == QStyleOptionHeader::Beginning));
    bool isCorner(widget && widget->inherits("QTableCornerButton"));
    bool reverseLayout(option->direction == Qt::RightToLeft);

    // update animation state
    _animations->headerViewEngine().updateState(widget, rect.topLeft(), mouseOver);

    QBrush color = palette.base();

    painter->setRenderHint(QPainter::Antialiasing, false);
    painter->setBrush(color);
    painter->setPen(Qt::NoPen);
    painter->drawRect(rect);

    // outline
    painter->setBrush(Qt::NoBrush);
    painter->setPen(Colors::alphaColor(palette.color(QPalette::WindowText), 0.2));

    if (isCorner) {
        if (reverseLayout) {
            painter->drawPoint(rect.bottomLeft());
        } else {
            painter->drawPoint(rect.bottomRight());
        }
    } else if (horizontal) {
        painter->drawLine(rect.bottomLeft(), rect.bottomRight());
    } else {
        if (reverseLayout) {
            painter->drawLine(rect.topLeft(), rect.bottomLeft());
        } else {
            painter->drawLine(rect.topRight(), rect.bottomRight());
        }
    }

    // separators
    if (horizontal) {
        if (headerOption->section != 0 || isFirst) {
            if (reverseLayout) {
                painter->drawLine(rect.topLeft(), rect.bottomLeft() - QPoint(0, 1));
            } else {
                painter->drawLine(rect.topRight(), rect.bottomRight() - QPoint(0, 1));
            }
        }
    } else {
        if (reverseLayout) {
            painter->drawLine(rect.bottomLeft() + QPoint(1, 0), rect.bottomRight());
        } else {
            painter->drawLine(rect.bottomLeft(), rect.bottomRight() - QPoint(1, 0));
        }
    }

    return true;
}

bool Style::drawHeaderLabelControl(const QStyleOption *option, QPainter *painter, const QWidget *widget) const
{
    if (const QStyleOptionHeader *header = qstyleoption_cast<const QStyleOptionHeader *>(option)) {
        QRect rect = header->rect;
        if (!header->icon.isNull()) {
            QPixmap pixmap = header->icon.pixmap(proxy()->pixelMetric(PM_SmallIconSize), (header->state & State_Enabled) ? QIcon::Normal : QIcon::Disabled);
            int pixw = pixmap.width() / pixmap.devicePixelRatio();

            QRect aligned = alignedRect(header->direction, QFlag(header->iconAlignment), pixmap.size() / pixmap.devicePixelRatio(), rect);
            QRect inter = aligned.intersected(rect);
            painter->drawPixmap(inter.x(), inter.y(), pixmap,
                                inter.x() - aligned.x(), inter.y() - aligned.y(),
                                aligned.width() * pixmap.devicePixelRatio(),
                                aligned.height() * pixmap.devicePixelRatio());

            const int margin = proxy()->pixelMetric(QStyle::PM_HeaderMargin, option, widget);
            if (header->direction == Qt::LeftToRight)
                rect.setLeft(rect.left() + pixw + margin);
            else
                rect.setRight(rect.right() - pixw - margin);
        }
        QFont fnt = painter->font();
        fnt.setBold(true);
        painter->setFont(fnt);
        QPalette palette(header->palette);

        // Style options
        StyleOptions styleOptions(option->palette, _variant);
        styleOptions.setState(header->state);

        palette.setColor(QPalette::Text, Colors::headerTextColor(styleOptions));
        proxy()->drawItemText(painter, rect, header->textAlignment, palette, (header->state & State_Active), header->text, QPalette::Text);
    }
    return true;
}

//___________________________________________________________________________________
bool Style::drawHeaderEmptyAreaControl(const QStyleOption *option, QPainter *painter, const QWidget *) const
{
    // use the same background as in drawHeaderPrimitive
    const QRect &rect(option->rect);
    QPalette palette(option->palette);

    bool horizontal(option->state & QStyle::State_Horizontal);
    bool reverseLayout(option->direction == Qt::RightToLeft);

    // fill
    painter->setRenderHint(QPainter::Antialiasing, false);
    painter->setBrush(palette.color(QPalette::Base));
    painter->setPen(Qt::NoPen);
    painter->drawRect(rect);

    // outline
    painter->setBrush(Qt::NoBrush);
    painter->setPen(Colors::alphaColor(palette.color(QPalette::ButtonText), 0.1));

    if (horizontal) {
        painter->drawLine(rect.bottomLeft(), rect.bottomRight());
    } else {
        if (reverseLayout) {
            painter->drawLine(rect.topLeft(), rect.bottomLeft());
        } else {
            painter->drawLine(rect.topRight(), rect.bottomRight());
        }
    }

    return true;
}

//___________________________________________________________________________________
bool Style::drawTabBarTabLabelControl(const QStyleOption *option, QPainter *painter, const QWidget *widget) const
{
    if (const QStyleOptionTab *tab = qstyleoption_cast<const QStyleOptionTab *>(option)) {
        QRect tr = tab->rect;
        bool verticalTabs = tab->shape == QTabBar::RoundedEast
                            || tab->shape == QTabBar::RoundedWest
                            || tab->shape == QTabBar::TriangularEast
                            || tab->shape == QTabBar::TriangularWest;

        int alignment = Qt::AlignCenter | Qt::TextShowMnemonic;
        if (!proxy()->styleHint(SH_UnderlineShortcut, option, widget))
            alignment |= Qt::TextHideMnemonic;

        if (verticalTabs) {
            painter->save();
            int newX, newY, newRot;
            if (tab->shape == QTabBar::RoundedEast || tab->shape == QTabBar::TriangularEast) {
                newX = tr.width() + tr.x();
                newY = tr.y();
                newRot = 90;
            } else {
                newX = tr.x();
                newY = tr.y() + tr.height();
                newRot = -90;
            }
            QTransform m = QTransform::fromTranslate(newX, newY);
            m.rotate(newRot);
            painter->setTransform(m, true);
        }
        QRect iconRect;
        tabLayout(tab, widget, &tr, &iconRect, proxy());
        tr = proxy()->subElementRect(SE_TabBarTabText, option, widget); //we compute tr twice because the style may override subElementRect

        if (!tab->icon.isNull()) {
            QPixmap tabIcon = tab->icon.pixmap(tab->iconSize, (tab->state & State_Enabled) ? QIcon::Normal : QIcon::Disabled,
                                                (tab->state & State_Selected) ? QIcon::On : QIcon::Off);
            painter->drawPixmap(iconRect.x(), iconRect.y(), tabIcon);
        }

        QFont font = painter->font();
        font.setBold(true);
        painter->setFont(font);
        if (!(tab->state & State_Enabled)) {
            if (tab->state & State_Selected) {
                painter->setPen(Colors::mix(option->palette.brush(QPalette::Text).color(), option->palette.brush(QPalette::Window).color(), 0.3));
            } else {
                painter->setPen(Colors::mix(option->palette.brush(QPalette::Text).color(), option->palette.brush(QPalette::Window).color(), 0.4));
            }
        } else {
            if (tab->state & State_Selected) {
                painter->setPen(option->palette.brush(QPalette::WindowText).color());
            } else if (tab->state & State_Active && tab->state & State_MouseOver) {
                painter->setPen(Colors::mix(option->palette.brush(QPalette::Dark).color(), option->palette.brush(QPalette::Text).color(), 0.7));
            } else {
                painter->setPen(Colors::mix(option->palette.brush(QPalette::Dark).color(), option->palette.brush(QPalette::Text).color(), 0.6));
            }
        }

        proxy()->drawItemText(painter, tr, alignment, tab->palette, tab->state & State_Enabled, tab->text, QPalette::NoRole);

        if (verticalTabs) {
            painter->restore();
        }

        if (tab->state & State_HasFocus) {
            int OFFSET = 1 + pixelMetric(PM_DefaultFrameWidth);

            int x1, x2;
            x1 = tab->rect.left();
            x2 = tab->rect.right() - 1;

            QStyleOptionFocusRect fropt;
            fropt.QStyleOption::operator=(*tab);
            fropt.rect.setRect(x1 + 1 + OFFSET, tab->rect.y() + OFFSET,
                               x2 - x1 - 2 * OFFSET, tab->rect.height() - 2 * OFFSET);
            drawPrimitive(PE_FrameFocusRect, &fropt, painter, widget);
        }
    }

    // store rect and palette
    const QRect &rect(option->rect);

    // check focus
    const State &state(option->state);
    bool enabled(state & State_Enabled);
    bool selected(state & State_Selected);
    bool hasFocus(enabled && selected && (state & State_HasFocus));

    // update mouse over animation state
    _animations->tabBarEngine().updateState(widget, rect.topLeft(), AnimationFocus, hasFocus);
    bool animated(enabled && selected && _animations->tabBarEngine().isAnimated(widget, rect.topLeft(), AnimationFocus));

    if (!(hasFocus || animated))
        return true;

    // code is copied from QCommonStyle, but adds focus
    // cast option and check
    const QStyleOptionTab *tabOption(qstyleoption_cast<const QStyleOptionTab *>(option));
    if (!tabOption || tabOption->text.isEmpty()) {
        return true;
    }

    // tab option rect
    bool verticalTabs(isVerticalTab(tabOption));
    int textFlags(Qt::AlignCenter | _mnemonics->textFlags());

    // text rect
    QRect textRect(subElementRect(SE_TabBarTabText, option, widget));

    if (verticalTabs) {
        // properly rotate painter
        painter->save();
        int newX, newY, newRot;
        if (tabOption->shape == QTabBar::RoundedEast || tabOption->shape == QTabBar::TriangularEast) {
            newX = rect.width() + rect.x();
            newY = rect.y();
            newRot = 90;
        } else {
            newX = rect.x();
            newY = rect.y() + rect.height();
            newRot = -90;
        }

        QTransform transform;
        transform.translate(newX, newY);
        transform.rotate(newRot);
        painter->setTransform(transform, true);
    }

    // adjust text rect based on font metrics
    textRect = option->fontMetrics.boundingRect(textRect, textFlags, tabOption->text);

    if (verticalTabs) {
        painter->restore();
    }

    return true;
}

//___________________________________________________________________________________
bool Style::drawTabBarTabShapeControl(const QStyleOption *option, QPainter *painter, const QWidget *widget) const
{
    const QStyleOptionTab *tabOption(qstyleoption_cast<const QStyleOptionTab *>(option));
    if (!tabOption) {
        return true;
    }

    // palette and state
    const QPalette &palette(option->palette);
    const State &state(option->state);
    bool enabled(state & State_Enabled);
    bool selected(state & State_Selected);
    bool mouseOver((state & State_Active) && !selected && (state & State_MouseOver) && enabled);

    // check if tab is being dragged
    bool isDragged(widget && selected && painter->device() != widget);
    bool isLocked(widget && _tabBarData->isLocked(widget));

    // store rect
    QRect rect(option->rect);

    // update mouse over animation state
    _animations->tabBarEngine().updateState(widget, rect.topLeft(), AnimationHover, mouseOver);

    // lock state
    if (selected && widget && isDragged) {
        _tabBarData->lock(widget);
    } else if (widget && selected  && _tabBarData->isLocked(widget)) {
        _tabBarData->release();
    }

    // tab position
    const QStyleOptionTab::TabPosition &position = tabOption->position;
    bool isSingle(position == QStyleOptionTab::OnlyOneTab);
    bool isFirst(isSingle || position == QStyleOptionTab::Beginning);
    bool isLast(isSingle || position == QStyleOptionTab::End);
    bool isLeftOfSelected(!isLocked && tabOption->selectedPosition == QStyleOptionTab::NextIsSelected);
    bool isRightOfSelected(!isLocked && tabOption->selectedPosition == QStyleOptionTab::PreviousIsSelected);

    // true if widget is aligned to the frame
    // need to check for 'isRightOfSelected' because for some reason the isFirst flag is set when active tab is being moved
    isFirst &= !isRightOfSelected;
    isLast &= !isLeftOfSelected;

    // swap state based on reverse layout, so that they become layout independent
    bool reverseLayout(option->direction == Qt::RightToLeft);
    bool verticalTabs(isVerticalTab(tabOption));
    if (reverseLayout && !verticalTabs) {
        qSwap(isFirst, isLast);
        qSwap(isLeftOfSelected, isRightOfSelected);
    }

    // adjust rect and define corners based on tabbar orientation
    Corners corners;
    switch (tabOption->shape) {
    case QTabBar::RoundedNorth:
    case QTabBar::TriangularNorth:
        corners = CornersTop;
        break;
    case QTabBar::RoundedSouth:
    case QTabBar::TriangularSouth:
        corners = CornersBottom;
        break;
    case QTabBar::RoundedWest:
    case QTabBar::TriangularWest:
        corners = CornersLeft;
        break;
    case QTabBar::RoundedEast:
    case QTabBar::TriangularEast:
        corners = CornersRight;
        break;
    default:
        break;
    }

    // Style options
    StyleOptions styleOptions(option->palette, _variant);
    styleOptions.setState(option->state);

    // underline
    QColor underline(enabled && selected ? Colors::focusColor(StyleOptions(palette, _variant)) : selected || mouseOver ? option->palette.color(QPalette::Window).darker() : Qt::transparent);

    // outline
    QColor outline = QColor();
    if (selected && widget && widget->property("movable").toBool()) {
        outline = Colors::frameOutlineColor(StyleOptions(palette, _variant));
    }

    // background
    QColor background = Colors::tabBarColor(styleOptions);

    // render
    QRegion oldRegion(painter->clipRegion());
    painter->setClipRect(option->rect, Qt::IntersectClip);

    styleOptions.setPainter(painter);
    styleOptions.setRect(rect);
    styleOptions.setColor(underline);
    styleOptions.setOutlineColor(outline);

    Adwaita::Renderer::renderTabBarTab(styleOptions, background, corners, widget && widget->property("movable").toBool());
    painter->setClipRegion(oldRegion);

    return true;
}

//___________________________________________________________________________________
bool Style::drawToolBoxTabLabelControl(const QStyleOption *option, QPainter *painter, const QWidget *widget) const
{
    // rendering is similar to drawPushButtonLabelControl
    // cast option and check
    const QStyleOptionToolBox *toolBoxOption(qstyleoption_cast<const QStyleOptionToolBox *>(option));
    if (!toolBoxOption) {
        return true;
    }

    // copy palette
    const QPalette &palette(option->palette);

    const State &state(option->state);
    bool enabled(state & State_Enabled);

    // text alignment
    int textFlags(_mnemonics->textFlags() | Qt::AlignCenter);

    // contents rect
    QRect rect(subElementRect(SE_ToolBoxTabContents, option, widget));

    // store icon size
    int iconSize(pixelMetric(QStyle::PM_SmallIconSize, option, widget));

    // find contents size and rect
    QRect contentsRect(rect);
    QSize contentsSize;
    if (!toolBoxOption->text.isEmpty()) {
        contentsSize = option->fontMetrics.size(_mnemonics->textFlags(), toolBoxOption->text);
        if (!toolBoxOption->icon.isNull()) {
            contentsSize.rwidth() += Metrics::ToolBox_TabItemSpacing;
        }
    }

    // icon size
    if (!toolBoxOption->icon.isNull()) {
        contentsSize.setHeight(qMax(contentsSize.height(), iconSize));
        contentsSize.rwidth() += iconSize;
    }

    // adjust contents rect
    contentsRect = centerRect(contentsRect, contentsSize);

    // render icon
    if (!toolBoxOption->icon.isNull()) {
        // icon rect
        QRect iconRect;
        if (toolBoxOption->text.isEmpty()) {
            iconRect = centerRect(contentsRect, iconSize, iconSize);
        } else {
            iconRect = contentsRect;
            iconRect.setWidth(iconSize);
            iconRect = centerRect(iconRect, iconSize, iconSize);
            contentsRect.setLeft(iconRect.right() + Metrics::ToolBox_TabItemSpacing + 1);
        }

        iconRect = visualRect(option, iconRect);
        const QIcon::Mode mode(enabled ? QIcon::Normal : QIcon::Disabled);
        QPixmap pixmap(toolBoxOption->icon.pixmap(iconSize, mode));
        drawItemPixmap(painter, iconRect, textFlags, pixmap);
    }

    // render text
    if (!toolBoxOption->text.isEmpty()) {
        contentsRect = visualRect(option, contentsRect);
        drawItemText(painter, contentsRect, textFlags, palette, enabled, toolBoxOption->text, QPalette::WindowText);
    }

    return true;
}

//___________________________________________________________________________________
bool Style::drawToolBoxTabShapeControl(const QStyleOption *option, QPainter *painter, const QWidget *widget) const
{
    // cast option and check
    const QStyleOptionToolBox *toolBoxOption(qstyleoption_cast<const QStyleOptionToolBox *>(option));
    if (!toolBoxOption) {
        return true;
    }

    // copy rect and palette
    const QRect &rect(option->rect);
    QRect tabRect(toolBoxTabContentsRect(option, widget));

    /*
     * important: option returns the wrong palette.
     * we use the widget palette instead, when set
     */
    QPalette palette(widget ? widget->palette() : option->palette);

    // store flags
    const State &flags(option->state);
    bool enabled(flags & State_Enabled);
    bool selected(flags & State_Selected);
    bool mouseOver((flags & State_Active) && enabled && !selected && (flags & State_MouseOver));

    // update animation state
    /*
     * the proper widget ( the toolbox tab ) is not passed as argument by Qt.
     * What is passed is the toolbox directly. To implement animations properly,
     *the painter->device() is used instead
     */
    bool isAnimated(false);
    qreal opacity(AnimationData::OpacityInvalid);
    QPaintDevice *device = painter->device();
    if (enabled && device) {
        _animations->toolBoxEngine().updateState(device, mouseOver);
        isAnimated = _animations->toolBoxEngine().isAnimated(device);
        opacity = _animations->toolBoxEngine().opacity(device);
    }

    // Style options
    StyleOptions styleOptions(option->palette, _variant);
    styleOptions.setMouseOver(mouseOver);
    styleOptions.setHasFocus(false);
    styleOptions.setOpacity(opacity);
    styleOptions.setAnimationMode(isAnimated ? AnimationHover : AnimationNone);

    // color
    QColor outline;
    if (selected) {
        outline = Colors::focusColor(StyleOptions(palette, _variant));
    } else {
        outline = Colors::frameOutlineColor(styleOptions);
    }

    styleOptions.setPainter(painter);
    styleOptions.setRect(rect);
    styleOptions.setColor(outline);

    // render
    Adwaita::Renderer::renderToolBoxFrame(styleOptions, tabRect.width());

    return true;
}

//___________________________________________________________________________________
bool Style::drawDockWidgetTitleControl(const QStyleOption *option, QPainter *painter, const QWidget *widget) const
{
    // cast option and check
    const QStyleOptionDockWidget *dockWidgetOption = ::qstyleoption_cast<const QStyleOptionDockWidget *>(option);
    if (!dockWidgetOption) {
        return true;
    }

    const QPalette &palette(option->palette);
    const State &state(option->state);
    bool enabled(state & State_Enabled);
    bool reverseLayout(option->direction == Qt::RightToLeft);

    bool verticalTitleBar(dockWidgetOption ? dockWidgetOption->verticalTitleBar : false);

    QRect buttonRect(subElementRect(dockWidgetOption->floatable ? SE_DockWidgetFloatButton : SE_DockWidgetCloseButton, option, widget));

    // get rectangle and adjust to properly accounts for buttons
    QRect rect(insideMargin(dockWidgetOption->rect, Metrics::Frame_FrameWidth));
    if (verticalTitleBar) {
        if (buttonRect.isValid()) {
            rect.setTop(buttonRect.bottom() + 1);
        }
    } else if (reverseLayout) {
        if (buttonRect.isValid()) {
            rect.setLeft(buttonRect.right() + 1);
        }
        rect.adjust(0, 0, -4, 0);
    } else {
        if (buttonRect.isValid()) {
            rect.setRight(buttonRect.left() - 1);
        }
        rect.adjust(4, 0, 0, 0);
    }

    QString title(dockWidgetOption->title);
    int titleWidth = dockWidgetOption->fontMetrics.size(_mnemonics->textFlags(), title).width();
    int width = verticalTitleBar ? rect.height() : rect.width();
    if (width < titleWidth) {
        title = dockWidgetOption->fontMetrics.elidedText(title, Qt::ElideMiddle, width, Qt::TextShowMnemonic);
    }

    if (verticalTitleBar) {
        QSize size = rect.size();
        size.transpose();
        rect.setSize(size);

        painter->save();
        painter->translate(rect.left(), rect.top() + rect.width());
        painter->rotate(-90);
        painter->translate(-rect.left(), -rect.top());
        drawItemText(painter, rect, Qt::AlignLeft | Qt::AlignVCenter | _mnemonics->textFlags(), palette, enabled, title, QPalette::WindowText);
        painter->restore();
    } else {
        drawItemText(painter, rect, Qt::AlignLeft | Qt::AlignVCenter | _mnemonics->textFlags(), palette, enabled, title, QPalette::WindowText);
    }

    return true;
}

//______________________________________________________________
bool Style::drawGroupBoxComplexControl(const QStyleOptionComplex *option, QPainter *painter, const QWidget *widget) const
{
    if (const QStyleOptionGroupBox *groupBox = qstyleoption_cast<const QStyleOptionGroupBox *>(option)) {
        painter->save();

        QRect textRect = proxy()->subControlRect(CC_GroupBox, groupBox, SC_GroupBoxLabel, widget);
        QRect checkBoxRect = proxy()->subControlRect(CC_GroupBox, groupBox, SC_GroupBoxCheckBox, widget);

        // Draw title
        if ((groupBox->subControls & QStyle::SC_GroupBoxLabel) && !groupBox->text.isEmpty()) {
            QColor textColor = groupBox->textColor;
            if (textColor.isValid()) {
                painter->setPen(textColor);
            }
            int alignment = int(groupBox->textAlignment);
            if (!styleHint(QStyle::SH_UnderlineShortcut, option, widget)) {
                alignment |= Qt::TextHideMnemonic;
            }
            QFont font = painter->font();
            font.setBold(true);
            painter->setFont(font);
            painter->drawText(textRect, Qt::TextShowMnemonic | Qt::AlignLeft | Qt::AlignVCenter | alignment, groupBox->text);
        }
        if (groupBox->subControls & SC_GroupBoxCheckBox) {
            QStyleOptionButton box;
            box.QStyleOption::operator=(*groupBox);
            box.rect = checkBoxRect;
            proxy()->drawPrimitive(PE_IndicatorCheckBox, &box, painter, widget);
        }
        painter->restore();
    }

    return true;
}

//______________________________________________________________
bool Style::drawToolButtonComplexControl(const QStyleOptionComplex *option, QPainter *painter, const QWidget *widget) const
{
    // cast option and check
    const QStyleOptionToolButton *toolButtonOption(qstyleoption_cast<const QStyleOptionToolButton *>(option));
    if (!toolButtonOption) {
        return true;
    }

    // need to alter palette for focused buttons
    const State &state(option->state);
    bool enabled(state & State_Enabled);
    bool mouseOver((state & State_Active) && enabled && (option->state & State_MouseOver));
    bool sunken(state & (State_On | State_Sunken));
    bool flat(state & State_AutoRaise);

    // update animation state
    // pressed takes precedence over mouse

    qreal mouseOpacity = 0.0;
    qreal pressedOpacity = 0.0;

    _animations->widgetStateEngine().updateState(widget, AnimationPressed, sunken);
    _animations->widgetStateEngine().updateState(widget, AnimationHover, mouseOver);

    AnimationMode mode(_animations->widgetStateEngine().buttonAnimationMode(widget));

    if (mode == AnimationPressed) {
        pressedOpacity = _animations->widgetStateEngine().buttonOpacity(widget);
    } else if (sunken) {
        pressedOpacity = 1.0;
    } else if (mode == AnimationHover) {
        mouseOpacity = _animations->widgetStateEngine().buttonOpacity(widget);
    } else if (mouseOver) {
        mouseOpacity = 1.0;
    }

    // detect buttons in tabbar, for which special rendering is needed
    bool isDockWidgetTitleButton(widget && widget->inherits("QDockWidgetTitleButton"));
    bool inTabBar(widget && qobject_cast<const QTabBar *>(widget->parentWidget()));
    bool isMenuTitle(this->isMenuTitle(widget));
    if (isMenuTitle) {
        // copy option to adust state, and set font as not-bold
        QStyleOptionToolButton copy(*toolButtonOption);
        copy.font.setBold(false);
        copy.state = State_Enabled;

        // render
        renderMenuTitle(&copy, painter, widget);
        return true;
    }

    // copy option and alter palette
    QStyleOptionToolButton copy(*toolButtonOption);

    if (isDockWidgetTitleButton) {
        // cast to abstract button
        // adjust state to have correct icon rendered
        const QAbstractButton *button(qobject_cast<const QAbstractButton *>(widget));
        if (button->isChecked() || button->isDown()) {
            copy.state |= State_Enabled | State_On | State_Sunken;
        }
        if (button->underMouse()) {
            copy.state |= State_Enabled | State_MouseOver | State_Active;
        }

    }

    bool hasPopupMenu(toolButtonOption->features & QStyleOptionToolButton::MenuButtonPopup);
    const bool hasInlineIndicator(
        toolButtonOption->features & QStyleOptionToolButton::HasMenu
        && toolButtonOption->features & QStyleOptionToolButton::PopupDelay
        && !hasPopupMenu);

    QRect buttonRect(subControlRect(CC_ToolButton, option, SC_ToolButton, widget));
    QRect menuRect(subControlRect(CC_ToolButton, option, SC_ToolButtonMenu, widget));

    // frame
    if (toolButtonOption->subControls & SC_ToolButton || isDockWidgetTitleButton) {
        copy.rect = buttonRect;
        if (inTabBar) {
            QRect rect(option->rect);
            QColor background(Colors::mix(option->palette.window().color(), option->palette.shadow().color(), 0.15));
            background = Colors::mix(background, Qt::white, 0.2 * mouseOpacity);
            background = Colors::mix(background, Qt::black, 0.15 * pressedOpacity);
            QColor outline(Colors::frameOutlineColor(StyleOptions(option->palette, _variant)));
            painter->setPen(background);
            painter->setBrush(background);
            switch (toolButtonOption->arrowType) {
            case Qt::UpArrow:
                painter->drawRect(rect.adjusted(1, 1, -2, -1));
                break;
            case Qt::DownArrow:
                painter->drawRect(rect.adjusted(1, 0, -2, -2));
                break;
            case Qt::LeftArrow:
                painter->drawRect(rect.adjusted(1, 1, -1, -2));
                break;
            case Qt::RightArrow:
                painter->drawRect(rect.adjusted(0, 1, -2, -2));
                break;
            default:
                break;
            }
            painter->setPen(outline);
            switch (toolButtonOption->arrowType) {
            case Qt::DownArrow:
                painter->drawLine(rect.bottomLeft(), rect.bottomRight());
                break;
            case Qt::RightArrow:
                painter->drawLine(rect.topRight(), rect.bottomRight());
                break;
            default:
                break;
            }
            switch (toolButtonOption->arrowType) {
            case Qt::UpArrow:
            case Qt::DownArrow:
                painter->drawLine(rect.topLeft(), rect.bottomLeft());
                painter->drawLine(rect.topLeft(), rect.bottomLeft());
                break;
            case Qt::LeftArrow:
            case Qt::RightArrow:
                painter->drawLine(rect.topLeft(), rect.topRight());
                painter->drawLine(rect.bottomLeft(), rect.bottomRight());
                break;
            default:
                break;
            }
        } else if (sunken && hasPopupMenu && !(toolButtonOption->activeSubControls & SC_ToolButton)) {
            // Only menu button is active. so draw left hand side od button raised
            QStyleOptionToolButton btn(copy);
            btn.state |= State_Raised;
            btn.state &= ~State_Sunken;
            btn.state &= ~State_AutoRaise;
            drawPrimitive(PE_PanelButtonTool, &btn, painter, widget);
        } else {
            drawPrimitive(PE_PanelButtonTool, &copy, painter, widget);
        }
    }

    // arrow
    if (hasPopupMenu) {
        copy.rect = menuRect;
        if (!flat || mouseOver || sunken) {
            drawPrimitive(PE_IndicatorButtonDropDown, &copy, painter, widget);
        }

        drawPrimitive(PE_IndicatorArrowDown, &copy, painter, widget);
    } else if (hasInlineIndicator) {
        copy.rect = menuRect;

        drawPrimitive(PE_IndicatorArrowDown, &copy, painter, widget);
    }

    // contents
    {
        // restore state
        copy.state = state;

        // define contents rect
        QRect contentsRect(buttonRect);

        // detect dock widget title button
        // for dockwidget title buttons, do not take out margins, so that icon do not get scaled down
        if (isDockWidgetTitleButton) {
            // cast to abstract button
            // adjust state to have correct icon rendered
            const QAbstractButton *button(qobject_cast<const QAbstractButton *>(widget));
            if (button->isChecked() || button->isDown()) {
                copy.state |= State_Enabled | State_On | State_Sunken;
            }
            if (button->underMouse()) {
                copy.state |= State_Enabled | State_MouseOver | State_Active;
            }
        } else if (!inTabBar && hasInlineIndicator) {
            int marginWidth(flat ? Metrics::ToolButton_MarginWidth : Metrics::Button_MarginWidth + Metrics::Frame_FrameWidth);
            contentsRect = insideMargin(contentsRect, marginWidth, 0);
            contentsRect.setRight(contentsRect.right() - Metrics::ToolButton_InlineIndicatorWidth);
            contentsRect = visualRect(option, contentsRect);
        }

        copy.rect = contentsRect;

        // render
        drawControl(CE_ToolButtonLabel, &copy, painter, widget);
    }

    return true;
}

//______________________________________________________________
bool Style::drawComboBoxComplexControl(const QStyleOptionComplex *option, QPainter *painter, const QWidget *widget) const
{
    // cast option and check
    const QStyleOptionComboBox *comboBoxOption(qstyleoption_cast<const QStyleOptionComboBox *>(option));
    if (!comboBoxOption) {
        return true;
    }

    // rect and palette
    const QRect &rect(option->rect);
    const QPalette &palette(option->palette);

    // state
    const State &state(option->state);
    bool enabled(state & State_Enabled);
    bool windowActive(state & State_Active);
    bool editable(comboBoxOption->editable);
    bool arrowActive(comboBoxOption->activeSubControls & SC_ComboBoxArrow);
    bool flat(!comboBoxOption->frame);
    bool mouseOver;
    bool hasFocus;
    bool sunken;
    if (editable) {
        mouseOver = windowActive && arrowActive && enabled && (state & State_MouseOver);
        hasFocus = enabled && (state & (State_HasFocus | State_Sunken));
        sunken = arrowActive && enabled && (state & (State_On | State_Sunken));
    } else {
        mouseOver = windowActive && enabled && (state & State_MouseOver);
        hasFocus = enabled && (state & (State_HasFocus | State_Sunken));
        sunken = enabled && (state & (State_On | State_Sunken));
    }

    // update animation state
    // sunken takes precedence over hover that takes precedence over focus
    _animations->inputWidgetEngine().updateState(widget, AnimationPressed, sunken);
    _animations->inputWidgetEngine().updateState(widget, AnimationHover, mouseOver);
    _animations->inputWidgetEngine().updateState(widget, AnimationFocus, hasFocus && !mouseOver);

    // Style options
    StyleOptions styleOptions(option->palette, _variant);
    styleOptions.setMouseOver(mouseOver);
    styleOptions.setHasFocus(hasFocus);
    styleOptions.setSunken(sunken);

    // frame
    if (option->subControls & SC_ComboBoxFrame) {
        if (editable) {
            flat |= (rect.height() <= 2 * Metrics::Frame_FrameWidth + Metrics::MenuButton_IndicatorWidth);
            if (flat) {
                QColor background(palette.color(QPalette::Base));

                painter->setBrush(background);
                painter->setPen(Qt::NoPen);
                painter->drawRect(rect);
            } else {
                AnimationMode mode(_animations->inputWidgetEngine().buttonAnimationMode(widget));
                qreal opacity(_animations->inputWidgetEngine().buttonOpacity(widget));

                // Style options
                styleOptions.setAnimationMode(mode);
                styleOptions.setOpacity(opacity);

                // define colors
                QColor shadow(Colors::shadowColor(styleOptions));
                QColor outline(Colors::buttonOutlineColor(styleOptions));
                QColor background(Colors::buttonBackgroundColor(styleOptions));

                styleOptions.setPainter(painter);
                styleOptions.setRect(rect);
                styleOptions.setColor(background);
                styleOptions.setOutlineColor(outline);
                styleOptions.setActive(enabled && windowActive);

                // render
                Adwaita::Renderer::renderButtonFrame(styleOptions);

                QStyleOptionComplex tmpOpt(*option);
                tmpOpt.rect.setWidth(tmpOpt.rect.width() - subControlRect(CC_ComboBox, option, SC_ComboBoxArrow, widget).width() + 3);

                drawPrimitive(PE_FrameLineEdit, &tmpOpt, painter, widget);
            }
        } else {
            AnimationMode mode(_animations->inputWidgetEngine().buttonAnimationMode(widget));
            qreal opacity(_animations->inputWidgetEngine().buttonOpacity(widget));

            // Style options
            styleOptions.setAnimationMode(mode);
            styleOptions.setOpacity(opacity);
            styleOptions.setPainter(painter);
            styleOptions.setRect(rect);
            styleOptions.setActive(enabled && windowActive);

            if (flat) {
                // define colors and render
                QColor color(Colors::toolButtonColor(styleOptions));
                styleOptions.setColor(color);
                Adwaita::Renderer::renderToolButtonFrame(styleOptions);
            } else {
                // define colors
                QColor shadow(Colors::shadowColor(styleOptions));
                QColor outline(Colors::buttonOutlineColor(styleOptions));
                QColor background(Colors::buttonBackgroundColor(styleOptions));

                styleOptions.setColor(background);
                styleOptions.setOutlineColor(outline);

                // render
                Adwaita::Renderer::renderButtonFrame(styleOptions);

                if (hasFocus) {
                    QStyleOption copy(*option);
                    copy.rect.adjust(4, 4, -4, -4);
                    drawPrimitive(PE_FrameFocusRect, &copy, painter, widget);
                }
            }
        }
    }

    // arrow
    if (option->subControls & SC_ComboBoxArrow) {
        // Style options
        styleOptions.setColorRole(QPalette::ButtonText);

        // arrow color
        QColor arrowColor = Colors::arrowOutlineColor(styleOptions);

        // arrow rect
        QRect arrowRect(subControlRect(CC_ComboBox, option, SC_ComboBoxArrow, widget));

        styleOptions.setRect(arrowRect);
        styleOptions.setColor(arrowColor);

        // render
        Adwaita::Renderer::renderArrow(styleOptions, ArrowDown);
    }

    return true;
}

//______________________________________________________________
bool Style::drawSpinBoxComplexControl(const QStyleOptionComplex *option, QPainter *painter, const QWidget *widget) const
{
    const QStyleOptionSpinBox *spinBoxOption(qstyleoption_cast<const QStyleOptionSpinBox *>(option));
    if (!spinBoxOption) {
        return true;
    }

    // store palette and rect
    const QPalette &palette(option->palette);
    const QRect &rect(option->rect);

    if (option->subControls & SC_SpinBoxFrame) {
        // detect flat spinboxes
        bool flat(!spinBoxOption->frame);
        flat |= (rect.height() < 2 * Metrics::Frame_FrameWidth + Metrics::SpinBox_ArrowButtonWidth);
        if (flat) {
            QColor background(palette.color(QPalette::Base));

            painter->setBrush(background);
            painter->setPen(Qt::NoPen);
            painter->drawRect(rect);
        } else {
            drawPrimitive(PE_FrameLineEdit, option, painter, widget);
        }
    }

    if (option->subControls & SC_SpinBoxUp) {
        renderSpinBoxArrow(SC_SpinBoxUp, spinBoxOption, painter, widget);
    }
    if (option->subControls & SC_SpinBoxDown) {
        renderSpinBoxArrow(SC_SpinBoxDown, spinBoxOption, painter, widget);
    }

    return true;
}

//______________________________________________________________
bool Style::drawSliderComplexControl(const QStyleOptionComplex *option, QPainter *painter, const QWidget *widget) const
{
    // cast option and check
    const QStyleOptionSlider *sliderOption(qstyleoption_cast<const QStyleOptionSlider *>(option));
    if (!sliderOption) {
        return true;
    }

    // copy rect and palette
    const QRect &rect(option->rect);
    const QPalette &palette(option->palette);

    // copy state
    const State &state(option->state);
    bool enabled(state & State_Enabled);
    bool windowActive(state & State_Active);
    bool mouseOver((state & State_Active) && enabled && (state & State_MouseOver));
    bool hasFocus(enabled && (state & State_HasFocus));
    bool horizontal(sliderOption->orientation == Qt::Horizontal);
    Side tickSide { SideNone };
    if (horizontal && sliderOption->tickPosition == QSlider::TicksAbove) {
        tickSide = (Side)((int) tickSide | (int) SideTop);
    }
    if (horizontal && sliderOption->tickPosition == QSlider::TicksBelow) {
        tickSide = (Side)((int) tickSide | (int) SideBottom);
    }
    if (!horizontal && sliderOption->tickPosition == QSlider::TicksLeft) {
        tickSide = (Side)((int) tickSide | (int) SideLeft);
    }
    if (!horizontal && sliderOption->tickPosition == QSlider::TicksRight) {
        tickSide = (Side)((int) tickSide | (int) SideRight);
    }

    // tickmarks
    if (Adwaita::Config::SliderDrawTickMarks && (sliderOption->subControls & SC_SliderTickmarks)) {
        bool upsideDown(sliderOption->upsideDown);
        int tickPosition(sliderOption->tickPosition);
        int available(pixelMetric(PM_SliderSpaceAvailable, option, widget));
        int interval = sliderOption->tickInterval;
        if (interval < 1) {
            interval = sliderOption->pageStep;
        }
        if (interval >= 1) {
            int fudge(pixelMetric(PM_SliderLength, option, widget) / 2);
            int current(sliderOption->minimum);

            // store tick lines
            QRect grooveRect(subControlRect(CC_Slider, sliderOption, SC_SliderGroove, widget));
            QList<QLine> tickLines;
            if (horizontal) {
                if (tickPosition & QSlider::TicksAbove) {
                    tickLines.append(QLine(rect.left(), grooveRect.top() - Metrics::Slider_TickMarginWidth, rect.left(), grooveRect.top() - Metrics::Slider_TickMarginWidth - Metrics::Slider_TickLength));
                }
                if (tickPosition & QSlider::TicksBelow) {
                    tickLines.append(QLine(rect.left(), grooveRect.bottom() + Metrics::Slider_TickMarginWidth, rect.left(), grooveRect.bottom() + Metrics::Slider_TickMarginWidth + Metrics::Slider_TickLength));
                }
            } else {
                if (tickPosition & QSlider::TicksAbove) {
                    tickLines.append(QLine(grooveRect.left() - Metrics::Slider_TickMarginWidth, rect.top(), grooveRect.left() - Metrics::Slider_TickMarginWidth - Metrics::Slider_TickLength, rect.top()));
                }
                if (tickPosition & QSlider::TicksBelow) {
                    tickLines.append(QLine(grooveRect.right() + Metrics::Slider_TickMarginWidth, rect.top(), grooveRect.right() + Metrics::Slider_TickMarginWidth + Metrics::Slider_TickLength, rect.top()));
                }
            }

            // Style options
            StyleOptions styleOptions(palette, _variant);

            // colors
            QColor base(Colors::separatorColor(styleOptions));

            while (current <= sliderOption->maximum) {
                // adjust color
                QColor color(base);
                painter->setPen(color);

                // calculate positions and draw lines
                int position(sliderPositionFromValue(sliderOption->minimum, sliderOption->maximum, current, available) + fudge);
                foreach (const QLine &tickLine, tickLines) {
                    if (horizontal) {
                        painter->drawLine(tickLine.translated(upsideDown ? (rect.width() - position) : position, 0));
                    } else {
                        painter->drawLine(tickLine.translated(0, upsideDown ? (rect.height() - position) : position));
                    }
                }

                // go to next position
                current += interval;
            }
        }
    }

    // groove
    if (sliderOption->subControls & SC_SliderGroove) {
        if (hasFocus) {
            QRect focusRect = proxy()->subElementRect(SE_SliderFocusRect, option, widget);

            QStyleOptionFocusRect fropt;
            fropt.QStyleOption::operator=(*option);
            fropt.rect = focusRect;
            proxy()->drawPrimitive(PE_FrameFocusRect, &fropt, painter, widget);
        }

        // retrieve groove rect
        QRect grooveRect(subControlRect(CC_Slider, sliderOption, SC_SliderGroove, widget));

        // Style options
        StyleOptions styleOptions(palette, _variant);
        styleOptions.setPainter(painter);

        // base color
        QColor outline(Colors::buttonOutlineColor(styleOptions));
        QColor grooveColor(Colors::mix(outline, palette.color(QPalette::Window)));
        QColor highlightColor(palette.color(QPalette::Highlight));
        QColor highlightOutline(_dark ? Colors::darken(highlightColor, 0.3) : Colors::darken(highlightColor, 0.15));

        if (!enabled) {
            styleOptions.setRect(grooveRect);
            styleOptions.setColor(grooveColor);
            styleOptions.setOutlineColor(outline);
            Adwaita::Renderer::renderProgressBarGroove(styleOptions);
        } else {
            bool upsideDown(sliderOption->upsideDown);

            // handle rect
            QRect handleRect(subControlRect(CC_Slider, sliderOption, SC_SliderHandle, widget));

            if (sliderOption->orientation == Qt::Horizontal) {
                QRect leftRect(grooveRect);
                QRect rightRect(grooveRect);
                leftRect.setRight(handleRect.right() - Metrics::Slider_ControlThickness / 2);
                rightRect.setLeft(handleRect.left() + Metrics::Slider_ControlThickness / 2);

                if (upsideDown) {
                    styleOptions.setRect(leftRect);
                    styleOptions.setColor(grooveColor);
                    styleOptions.setOutlineColor(grooveColor);
                    Adwaita::Renderer::renderProgressBarGroove(styleOptions);
                    styleOptions.setRect(rightRect);
                    styleOptions.setColor(highlightColor);
                    styleOptions.setOutlineColor(highlightColor);
                    Adwaita::Renderer::renderProgressBarContents(styleOptions);
                } else {
                    styleOptions.setRect(leftRect);
                    styleOptions.setColor(highlightColor);
                    styleOptions.setOutlineColor(highlightColor);
                    Adwaita::Renderer::renderProgressBarContents(styleOptions);
                    styleOptions.setRect(rightRect);
                    styleOptions.setColor(grooveColor);
                    styleOptions.setOutlineColor(grooveColor);
                    Adwaita::Renderer::renderProgressBarGroove(styleOptions);
                }
            } else {
                QRect topRect(grooveRect);
                topRect.setBottom(handleRect.bottom() - Metrics::Slider_ControlThickness / 2);
                QRect bottomRect(grooveRect);
                bottomRect.setTop(handleRect.top() + Metrics::Slider_ControlThickness / 2);

                if (upsideDown) {
                    styleOptions.setRect(topRect);
                    styleOptions.setColor(grooveColor);
                    styleOptions.setOutlineColor(grooveColor);
                    Adwaita::Renderer::renderProgressBarGroove(styleOptions);
                    styleOptions.setRect(bottomRect);
                    styleOptions.setColor(highlightColor);
                    styleOptions.setOutlineColor(highlightColor);
                    Adwaita::Renderer::renderProgressBarContents(styleOptions);
                } else {
                    styleOptions.setRect(topRect);
                    styleOptions.setColor(highlightColor);
                    styleOptions.setOutlineColor(highlightColor);
                    Adwaita::Renderer::renderProgressBarContents(styleOptions);
                    styleOptions.setRect(bottomRect);
                    styleOptions.setColor(grooveColor);
                    styleOptions.setOutlineColor(grooveColor);
                    Adwaita::Renderer::renderProgressBarGroove(styleOptions);
                }
            }
        }
    }

    // handle
    if (sliderOption->subControls & SC_SliderHandle) {
        // get rect and center
        QRect handleRect(subControlRect(CC_Slider, sliderOption, SC_SliderHandle, widget));

        // handle state
        bool handleActive(sliderOption->activeSubControls & SC_SliderHandle);

        // animation state
        _animations->widgetStateEngine().updateState(widget, AnimationHover, handleActive && mouseOver);
        _animations->widgetStateEngine().updateState(widget, AnimationFocus, hasFocus);
        AnimationMode mode(_animations->widgetStateEngine().buttonAnimationMode(widget));
        qreal opacity(_animations->widgetStateEngine().buttonOpacity(widget));

        // Style options
        StyleOptions styleOptions(palette, _variant);
        styleOptions.setMouseOver(mouseOver);
        styleOptions.setOpacity(opacity);
        styleOptions.setAnimationMode(mode);

        // define colors
        QColor background(Colors::buttonBackgroundColor(styleOptions));

        styleOptions.setMouseOver(handleActive && mouseOver);

        QColor outline(Colors::buttonOutlineColor(styleOptions));
        QColor shadow(Colors::shadowColor(styleOptions));

        styleOptions.setPainter(painter);
        styleOptions.setRect(handleRect);
        styleOptions.setColor(background);
        styleOptions.setOutlineColor(outline);
        styleOptions.setActive(enabled && windowActive);

        // render
        Adwaita::Renderer::renderSliderHandle(styleOptions, tickSide);
    }

    return true;
}

//______________________________________________________________
bool Style::drawDialComplexControl(const QStyleOptionComplex *option, QPainter *painter, const QWidget *widget) const
{
    // cast option and check
    const QStyleOptionSlider *sliderOption(qstyleoption_cast<const QStyleOptionSlider *>(option));
    if (!sliderOption) {
        return true;
    }

    const QPalette &palette(option->palette);
    const State &state(option->state);
    bool enabled(state & State_Enabled);
    bool mouseOver((state & State_Active) && enabled && (state & State_MouseOver));
    bool hasFocus(enabled && (state & State_HasFocus));
    bool horizontal(sliderOption->orientation == Qt::Horizontal);
    Side tickSide { SideNone };
    if (horizontal && sliderOption->tickPosition == QSlider::TicksAbove) {
        tickSide = (Side)((int) tickSide | (int) SideTop);
    }
    if (horizontal && sliderOption->tickPosition == QSlider::TicksBelow) {
        tickSide = (Side)((int) tickSide | (int) SideBottom);
    }
    if (!horizontal && sliderOption->tickPosition == QSlider::TicksLeft) {
        tickSide = (Side)((int) tickSide | (int) SideLeft);
    }
    if (!horizontal && sliderOption->tickPosition == QSlider::TicksRight) {
        tickSide = (Side)((int) tickSide | (int) SideRight);
    }

    // do not render tickmarks
    if (sliderOption->subControls & SC_DialTickmarks)
    {}

    // groove
    if (sliderOption->subControls & SC_DialGroove) {
        // groove rect
        QRect grooveRect(subControlRect(CC_Dial, sliderOption, SC_SliderGroove, widget));

        // groove
        QColor grooveColor(Colors::mix(palette.color(QPalette::Window), palette.color(QPalette::WindowText), 0.3));

        StyleOptions styleOptions(painter, grooveRect);
        styleOptions.setColor(grooveColor);
        styleOptions.setColorVariant(_variant);

        // render groove
        Adwaita::Renderer::renderDialGroove(styleOptions);

        if (enabled) {
            // highlight
            QColor highlight(palette.color(QPalette::Highlight));

            // angles
            qreal first(dialAngle(sliderOption, sliderOption->minimum));
            qreal second(dialAngle(sliderOption, sliderOption->sliderPosition));

            StyleOptions styleOptions(painter, grooveRect);
            styleOptions.setColor(highlight);
            styleOptions.setColorVariant(_variant);

            // render contents
            Adwaita::Renderer::renderDialContents(styleOptions, first, second);
        }
    }

    // handle
    if (sliderOption->subControls & SC_DialHandle) {
        // get handle rect
        QRect handleRect(subControlRect(CC_Dial, sliderOption, SC_DialHandle, widget));
        handleRect = centerRect(handleRect, Metrics::Slider_ControlThickness, Metrics::Slider_ControlThickness);

        // handle state
        bool handleActive(mouseOver && handleRect.contains(_animations->dialEngine().position(widget)));
        bool sunken(state & (State_On | State_Sunken));

        // animation state
        _animations->dialEngine().setHandleRect(widget, handleRect);
        _animations->dialEngine().updateState(widget, AnimationHover, handleActive && mouseOver);
        _animations->dialEngine().updateState(widget, AnimationFocus, hasFocus);
        AnimationMode mode(_animations->dialEngine().buttonAnimationMode(widget));
        qreal opacity(_animations->dialEngine().buttonOpacity(widget));

        // Style options
        StyleOptions styleOptions(palette, _variant);
        styleOptions.setAnimationMode(mode);
        styleOptions.setMouseOver(handleActive && mouseOver);
        styleOptions.setHasFocus(hasFocus);
        styleOptions.setOpacity(opacity);

        // define colors
        QColor background(palette.color(QPalette::Button));
        QColor outline(Colors::sliderOutlineColor(styleOptions));
        QColor shadow(Colors::shadowColor(styleOptions));

        // render
        qreal angle = 270 - 180 * dialAngle(sliderOption, sliderOption->sliderPosition) / M_PI;

        styleOptions.setPainter(painter);
        styleOptions.setRect(handleRect);
        styleOptions.setColor(background);
        styleOptions.setOutlineColor(outline);
        styleOptions.setSunken(sunken);
        styleOptions.setActive(enabled);

        Adwaita::Renderer::renderSliderHandle(styleOptions, tickSide, angle);
    }

    return true;
}

//______________________________________________________________
bool Style::drawScrollBarComplexControl(const QStyleOptionComplex *option, QPainter *painter, const QWidget *widget) const
{
    //the animation for QStyle::SC_ScrollBarGroove is special: it will animate
    //the opacity of everything else as well, included slider and arrows
    bool enabled(option->state & State_Enabled);
    qreal opacity(_animations->scrollBarEngine().opacity(widget, QStyle::SC_ScrollBarGroove));
    bool animated(Adwaita::Config::ScrollBarShowOnMouseOver && _animations->scrollBarEngine().isAnimated(widget,  AnimationHover, QStyle::SC_ScrollBarGroove));
    bool mouseOver((option->state & State_Active) && option->state & State_MouseOver);

    if (opacity == AnimationData::OpacityInvalid) {
        opacity = 1;
    }

    // render full groove directly, rather than using the addPage and subPage control element methods
    if ((mouseOver || animated) && option->subControls & SC_ScrollBarGroove) {
        // retrieve groove rectangle
        QRect grooveRect(subControlRect(CC_ScrollBar, option, SC_ScrollBarGroove, widget));

        const QPalette &palette(option->palette);
        QColor color;
        if (_dark) {
            color = Colors::mix(palette.color(QPalette::Window), Colors::mix(palette.color(QPalette::Base), palette.color(QPalette::Window), 0.5), opacity);
        } else {
            color = Colors::mix(palette.color(QPalette::Window), Colors::mix(palette.color(QPalette::Window), palette.color(QPalette::Text), 0.2), opacity);
        }

        const State &state(option->state);
        bool horizontal(state & State_Horizontal);

        if (horizontal) {
            grooveRect = centerRect(grooveRect, grooveRect.width(), Metrics::ScrollBar_SliderWidth);
        } else {
            grooveRect = centerRect(grooveRect, Metrics::ScrollBar_SliderWidth, grooveRect.height());
        }

        // render
        if (enabled) {
            painter->setPen(Qt::NoPen);
            painter->setBrush(color);
            painter->drawRect(option->rect);
        }
    }

    // call base class primitive
    //ParentStyleClass::drawComplexControl( CC_ScrollBar, option, painter, widget );
    if (const QStyleOptionSlider *scrollbar = qstyleoption_cast<const QStyleOptionSlider *>(option)) {
        QStyleOptionSlider newScrollbar = *scrollbar;
        State saveFlags = scrollbar->state;

        if (scrollbar->subControls & SC_ScrollBarSlider) {
            newScrollbar.rect = scrollbar->rect;
            newScrollbar.state = saveFlags;
            newScrollbar.rect = proxy()->subControlRect(CC_ScrollBar, &newScrollbar, SC_ScrollBarSlider, widget);
            if (newScrollbar.rect.isValid()) {
                proxy()->drawControl(CE_ScrollBarSlider, &newScrollbar, painter, widget);

                if (scrollbar->state & State_HasFocus) {
                    QStyleOptionFocusRect fropt;
                    fropt.QStyleOption::operator=(newScrollbar);
                    fropt.rect.setRect(newScrollbar.rect.x() + 2, newScrollbar.rect.y() + 2,
                                       newScrollbar.rect.width() - 5,
                                       newScrollbar.rect.height() - 5);
                    proxy()->drawPrimitive(PE_FrameFocusRect, &fropt, painter, widget);
                }
            }
        }
    }

    return true;
}

//______________________________________________________________
bool Style::drawTitleBarComplexControl(const QStyleOptionComplex *option, QPainter *painter, const QWidget *widget) const
{
    // cast option and check
    const QStyleOptionTitleBar *titleBarOption(qstyleoption_cast<const QStyleOptionTitleBar *>(option));
    if (!titleBarOption) {
        return true;
    }

    // store palette and rect
    QPalette palette(option->palette);
    const QRect &rect(option->rect);

    const State &flags(option->state);
    bool enabled(flags & State_Enabled);
    bool active(enabled && (titleBarOption->titleBarState & Qt::WindowActive));

    // Style options
    StyleOptions styleOptions(palette, _variant);
    styleOptions.setActive(active);

    if (titleBarOption->subControls & SC_TitleBarLabel) {
        // render background
        painter->setClipRect(rect);
        QColor outline(Colors::frameOutlineColor(styleOptions));
        QColor background(Colors::titleBarColor(styleOptions));

        styleOptions.setPainter(painter);
        styleOptions.setRect(rect.adjusted(-1, -1, 1, 3));
        styleOptions.setColor(background);
        styleOptions.setOutlineColor(outline);

        Adwaita::Renderer::renderTabWidgetFrame(styleOptions, CornersTop);

        painter->setRenderHint(QPainter::Antialiasing, false);
        painter->setBrush(Qt::NoBrush);
        painter->setPen(outline);
        painter->drawLine(rect.bottomLeft(), rect.bottomRight());

        // render text
        palette.setColor(QPalette::WindowText, Colors::titleBarTextColor(styleOptions));
        QRect textRect(subControlRect(CC_TitleBar, option, SC_TitleBarLabel, widget));
        ParentStyleClass::drawItemText(painter, textRect, Qt::AlignCenter, palette, active, titleBarOption->text, QPalette::WindowText);
    }

    // buttons
    static const QList<SubControl> subControls = {
        SC_TitleBarMinButton,
        SC_TitleBarMaxButton,
        SC_TitleBarCloseButton,
        SC_TitleBarNormalButton,
        SC_TitleBarSysMenu
    };

    // loop over supported buttons
    foreach (const SubControl &subControl, subControls) {
        // skip if not requested
        if (!(titleBarOption->subControls & subControl)) {
            continue;
        }

        // find matching icon
        QIcon icon;
        switch (subControl) {
        case SC_TitleBarMinButton:
            icon = standardIcon(SP_TitleBarMinButton, option, widget);
            break;
        case SC_TitleBarMaxButton:
            icon = standardIcon(SP_TitleBarMaxButton, option, widget);
            break;
        case SC_TitleBarCloseButton:
            icon = standardIcon(SP_TitleBarCloseButton, option, widget);
            break;
        case SC_TitleBarNormalButton:
            icon = standardIcon(SP_TitleBarNormalButton, option, widget);
            break;
        case SC_TitleBarSysMenu:
            icon = titleBarOption->icon;
            break;
        default:
            break;
        }

        // check icon
        if (icon.isNull()) {
            continue;
        }

        // define icon rect
        QRect iconRect(subControlRect(CC_TitleBar, option, subControl, widget));
        if (iconRect.isEmpty()) {
            continue;
        }

        // active state
        bool subControlActive(titleBarOption->activeSubControls & subControl);

        // mouse over state
        const bool mouseOver(!subControlActive && widget && iconRect.translated(widget->mapToGlobal(QPoint(0, 0))).contains(QCursor::pos()));

        // adjust iconRect
        int iconWidth(pixelMetric(PM_SmallIconSize, option, widget));
        QSize iconSize(iconWidth, iconWidth);
        iconRect = centerRect(iconRect, iconSize);

        // set icon mode and state
        QIcon::Mode iconMode;
        QIcon::State iconState;

        if (!enabled) {
            iconMode = QIcon::Disabled;
            iconState = QIcon::Off;
        } else {
            if (mouseOver) {
                iconMode = QIcon::Active;
            } else {
                iconMode = QIcon::Normal;
            }

            iconState = subControlActive ? QIcon::On : QIcon::Off;
        }

        // get pixmap and render
        QPixmap pixmap = icon.pixmap(iconSize, iconMode, iconState);
        painter->drawPixmap(iconRect, pixmap);
    }

    return true;
}

//____________________________________________________________________________________________________
void Style::renderSpinBoxArrow(const SubControl &subControl, const QStyleOptionSpinBox *option, QPainter *painter, const QWidget *widget) const
{
    const QPalette &palette(option->palette);
    const State &state(option->state);

    // enable state
    bool hasFocus(state & State_HasFocus);
    bool enabled(state & State_Enabled);
    bool sunken(state & State_Sunken && option->activeSubControls & subControl);
    const QColor &outline = Colors::frameOutlineColor(StyleOptions(palette, _variant)).lighter(120);

    // check steps enable step
    const bool atLimit((subControl == SC_SpinBoxUp && !(option->stepEnabled & QAbstractSpinBox::StepUpEnabled))
                       || (subControl == SC_SpinBoxDown && !(option->stepEnabled & QAbstractSpinBox::StepDownEnabled)));

    // update enabled state accordingly
    enabled &= !atLimit;

    // update mouse-over effect
    bool mouseOver((state & State_Active) && enabled && (state & State_MouseOver));

    // check animation state
    bool subControlHover(enabled && (mouseOver) && (option->activeSubControls & subControl));
    bool subControlSunken(enabled && (sunken) && (option->activeSubControls & subControl));
    _animations->spinBoxEngine().updateState(widget, subControl, subControlHover, subControlSunken);

    qreal opacity(_animations->spinBoxEngine().opacity(widget, subControl));
    qreal pressedOpacity(_animations->spinBoxEngine().pressed(widget, subControl));

    // Style options
    StyleOptions styleOptions(palette, _variant);
    styleOptions.setColorRole(QPalette::Text);

    QColor color = Colors::arrowOutlineColor(styleOptions);
    if (atLimit) {
        styleOptions.setColorGroup(QPalette::Disabled);
        color = Colors::arrowOutlineColor(styleOptions);
    }

    // arrow orientation
    ArrowOrientation orientation((subControl == SC_SpinBoxUp) ? ArrowUp : ArrowDown);

    // arrow rect
    QRect arrowRect(subControlRect(CC_SpinBox, option, subControl, widget));

    if (subControl == SC_SpinBoxDown) {
        painter->setBrush(Qt::NoBrush);
        painter->setPen(outline);
        int highlight = hasFocus ? 1 : 0;
        painter->drawLine(arrowRect.left(), arrowRect.top() + 2 + highlight, arrowRect.left(), arrowRect.bottom() - 1 - highlight);
    }
    if (subControl == SC_SpinBoxUp) {
        painter->setBrush(Qt::NoBrush);
        painter->setPen(outline);
        int highlight = hasFocus ? 1 : 0;
        painter->drawLine(arrowRect.left(), arrowRect.top() + 2 + highlight, arrowRect.left(), arrowRect.bottom() - 1 - highlight);
    }

    if (true) {
        painter->setPen(Qt::NoPen);
        QColor background = Colors::mix(palette.base().color(), palette.text().color(), opacity * 0.1);
        background = Colors::mix(background, palette.dark().color(), pressedOpacity);
        painter->setBrush(background);
        if (hasFocus) {
            painter->drawRect(arrowRect.adjusted(1, 3, -1, -2));
        } else {
            painter->drawRect(arrowRect.adjusted(1, 2, -1, -1));
        }
    }

    // render
    styleOptions.setPainter(painter);
    styleOptions.setRect(arrowRect);
    styleOptions.setColor(color);
    styleOptions.setColorVariant(_variant);

    Adwaita::Renderer::renderSign(styleOptions, orientation == ArrowUp);

    return;
}

//______________________________________________________________________________
void Style::renderMenuTitle(const QStyleOptionToolButton *option, QPainter *painter, const QWidget *) const
{
    // render a separator at the bottom
    const QPalette &palette(option->palette);

    // Style options
    StyleOptions styleOptions(palette, _variant);
    styleOptions.setPainter(painter);
    styleOptions.setRect(QRect(option->rect.bottomLeft() - QPoint(0, Metrics::MenuItem_MarginWidth), QSize(option->rect.width(), 1)));
    styleOptions.setColor(Colors::separatorColor(styleOptions));

    Adwaita::Renderer::renderSeparator(styleOptions);

    // render text in the center of the rect
    // icon is discarded on purpose
    painter->setFont(option->font);
    QRect contentsRect = insideMargin(option->rect, Metrics::MenuItem_MarginWidth);
    drawItemText(painter, contentsRect, Qt::AlignCenter, palette, true, option->text, QPalette::WindowText);
}

//______________________________________________________________________________
qreal Style::dialAngle(const QStyleOptionSlider *sliderOption, int value) const
{
    // calculate angle at which handle needs to be drawn
    qreal angle(0);
    if (sliderOption->maximum == sliderOption->minimum) {
        angle = M_PI / 2;
    } else {
        qreal fraction(qreal(value - sliderOption->minimum) / qreal(sliderOption->maximum - sliderOption->minimum));
        if (!sliderOption->upsideDown) {
            fraction = 1 - fraction;
        }
        if (sliderOption->dialWrapping) {
            angle = 1.5 * M_PI - fraction * 2 * M_PI;
        } else {
            angle = (M_PI * 8 - fraction * 10 * M_PI) / 6;
        }
    }

    return angle;
}

//______________________________________________________________________________
const QWidget *Style::scrollBarParent(const QWidget *widget) const
{
    // check widget and parent
    if (!(widget && widget->parentWidget())) {
        return nullptr;
    }

    // try cast to scroll area. Must test both parent and grandparent
    QAbstractScrollArea *scrollArea;
    if (!(scrollArea = qobject_cast<QAbstractScrollArea *>(widget->parentWidget()))) {
        scrollArea = qobject_cast<QAbstractScrollArea *>(widget->parentWidget()->parentWidget());
    }

    // check scrollarea
    if (scrollArea && (widget == scrollArea->verticalScrollBar() || widget == scrollArea->horizontalScrollBar())) {
        return scrollArea;
    } else if (widget->parentWidget()->inherits("KTextEditor::View")) {
        return widget->parentWidget();
    } else {
        return nullptr;
    }
}

//______________________________________________________________________________
QColor Style::scrollBarArrowColor(const QStyleOptionSlider *option, const SubControl &control, const QWidget *widget) const
{
    const QRect &rect(option->rect);
    const QPalette &palette(option->palette);

    // Style options
    StyleOptions styleOptions(palette, _variant);
    styleOptions.setColorRole(QPalette::WindowText);

    QColor color(Colors::arrowOutlineColor(styleOptions));

    bool widgetMouseOver((option->state & State_MouseOver) && (option->state & State_MouseOver));
    if (widget) {
        widgetMouseOver = widget->underMouse();
    } else if (option->styleObject) { // in case this QStyle is used by QQuickControls QStyle wrapper
        widgetMouseOver = option->styleObject->property("hover").toBool();
    }

    // check enabled state
    bool enabled(option->state & State_Enabled);
    if (!enabled) {
        if (Adwaita::Config::ScrollBarShowOnMouseOver) {
            // finally, global opacity when ScrollBarShowOnMouseOver
            qreal globalOpacity(_animations->scrollBarEngine().opacity(widget, QStyle::SC_ScrollBarGroove));
            if (globalOpacity >= 0) {
                color.setAlphaF(globalOpacity);
            } else if (!widgetMouseOver) { // no mouse over and no animation in progress, don't draw arrows at all
                return Qt::transparent;
            }
        }
        return color;
    }

    if ((control == SC_ScrollBarSubLine && option->sliderValue == option->minimum)
            || (control == SC_ScrollBarAddLine && option->sliderValue == option->maximum)) {

        // Style options
        styleOptions.setColorGroup(QPalette::Disabled);

        // manually disable arrow, to indicate that scrollbar is at limit
        color = Colors::arrowOutlineColor(styleOptions);
        if (Adwaita::Config::ScrollBarShowOnMouseOver) {
            // finally, global opacity when ScrollBarShowOnMouseOver
            qreal globalOpacity(_animations->scrollBarEngine().opacity(widget, QStyle::SC_ScrollBarGroove));
            if (globalOpacity >= 0) {
                color.setAlphaF(globalOpacity);
            } else if (!widgetMouseOver) { // no mouse over and no animation in progress, don't draw arrows at all
                return Qt::transparent;
            }
        }
        return color;
    }

    bool mouseOver((option->state & State_Active) && _animations->scrollBarEngine().isHovered(widget, control));
    bool animated(_animations->scrollBarEngine().isAnimated(widget, AnimationHover, control));
    qreal opacity(_animations->scrollBarEngine().opacity(widget, control));

    // retrieve mouse position from engine
    QPoint position(mouseOver ? _animations->scrollBarEngine().position(widget) : QPoint(-1, -1));
    if (mouseOver && rect.contains(position)) {
        /*
         * need to update the arrow controlRect on fly because there is no
         * way to get it from the styles directly, outside of repaint events
         */
        _animations->scrollBarEngine().setSubControlRect(widget, control, rect);
    }

    if (rect.intersects(_animations->scrollBarEngine().subControlRect(widget, control))) {
        QColor highlight = Colors::hoverColor(StyleOptions(palette, _variant));
        if (animated) {
            color = Colors::mix(color, highlight, opacity);
        } else if (mouseOver) {
            color = highlight;
        }
    }

    if (Adwaita::Config::ScrollBarShowOnMouseOver) {
        // finally, global opacity when ScrollBarShowOnMouseOver
        qreal globalOpacity(_animations->scrollBarEngine().opacity(widget, QStyle::SC_ScrollBarGroove));
        if (globalOpacity >= 0) {
            color.setAlphaF(globalOpacity);
        } else if (!widgetMouseOver) { // no mouse over and no animation in progress, don't draw arrows at all
            return Qt::transparent;
        }
    }

    return color;
}

//____________________________________________________________________________________
void Style::setTranslucentBackground(QWidget *widget) const
{
    if (!_isKDE) {
        return;
    }
    widget->setAttribute(Qt::WA_TranslucentBackground);

#ifdef Q_WS_WIN
    // FramelessWindowHint is needed on windows to make WA_TranslucentBackground work properly
    widget->setWindowFlags(widget->windowFlags() | Qt::FramelessWindowHint);
#endif

}

//____________________________________________________________________________________
QStyleOptionToolButton Style::separatorMenuItemOption(const QStyleOptionMenuItem *menuItemOption, const QWidget *widget) const
{
    // separator can have a title and an icon
    // in that case they are rendered as sunken flat toolbuttons
    QStyleOptionToolButton toolButtonOption;
    toolButtonOption.initFrom(widget);
    toolButtonOption.rect = menuItemOption->rect;
    toolButtonOption.features = QStyleOptionToolButton::None;
    toolButtonOption.state = State_Enabled | State_AutoRaise;
    toolButtonOption.subControls = SC_ToolButton;
    toolButtonOption.icon =  QIcon();
    toolButtonOption.iconSize = QSize();
    toolButtonOption.text = menuItemOption->text;

    toolButtonOption.toolButtonStyle = Qt::ToolButtonTextBesideIcon;
    return toolButtonOption;
}

//____________________________________________________________________________________
QIcon Style::toolBarExtensionIcon(StandardPixmap standardPixmap, const QStyleOption *option, const QWidget *widget) const
{
    // store palette
    // due to Qt, it is not always safe to assume that either option, nor widget are defined
    QPalette palette;
    if (option) {
        palette = option->palette;
    } else if (widget) {
        palette = widget->palette();
    } else {
        palette = QApplication::palette();
    }

    // convenience class to map color to icon mode
    struct IconData {
        QColor _color;
        QIcon::Mode _mode;
        QIcon::State _state;
    };

    // map colors to icon states
    const QList<IconData> iconTypes = {
        { palette.color(QPalette::Active, QPalette::WindowText), QIcon::Normal, QIcon::Off },
        { palette.color(QPalette::Active, QPalette::WindowText), QIcon::Selected, QIcon::Off },
        { palette.color(QPalette::Active, QPalette::WindowText), QIcon::Active, QIcon::Off },
        { palette.color(QPalette::Disabled, QPalette::WindowText), QIcon::Disabled, QIcon::Off },

        { palette.color(QPalette::Active, QPalette::HighlightedText), QIcon::Normal, QIcon::On },
        { palette.color(QPalette::Active, QPalette::HighlightedText), QIcon::Selected, QIcon::On },
        { palette.color(QPalette::Active, QPalette::WindowText), QIcon::Active, QIcon::On },
        { palette.color(QPalette::Disabled, QPalette::WindowText), QIcon::Disabled, QIcon::On }
    };

    // default icon sizes
    static const QList<int> iconSizes = { 8, 16, 22, 32, 48 };

    // decide arrow orientation
    ArrowOrientation orientation(standardPixmap == SP_ToolBarHorizontalExtensionButton ? ArrowRight : ArrowDown);

    // create icon and fill
    QIcon icon;
    foreach (const IconData &iconData, iconTypes) {
        foreach (const int &iconSize, iconSizes) {
            // create pixmap
            QPixmap pixmap(iconSize, iconSize);
            pixmap.fill(Qt::transparent);

            // render
            QPainter painter(&pixmap);

            // icon size
            int fixedIconSize(pixelMetric(QStyle::PM_SmallIconSize, option, widget));
            QRect fixedRect(0, 0, fixedIconSize, fixedIconSize);

            painter.setWindow(fixedRect);
            painter.translate(standardPixmap == SP_ToolBarHorizontalExtensionButton ? QPoint(1, 0) : QPoint(0, 1));

            StyleOptions styleOptions(&painter, fixedRect);
            styleOptions.setColor(iconData._color);
            styleOptions.setColorVariant(_variant);

            Adwaita::Renderer::renderArrow(styleOptions, orientation);
            painter.end();

            // add to icon
            icon.addPixmap(pixmap, iconData._mode, iconData._state);
        }
    }

    return icon;
}

//____________________________________________________________________________________
QIcon Style::titleBarButtonIcon(StandardPixmap standardPixmap, const QStyleOption *option, const QWidget *widget) const
{
    // map standardPixmap to button type
    ButtonType buttonType;
    switch (standardPixmap) {
    case SP_TitleBarNormalButton:
        buttonType = ButtonRestore;
        break;
    case SP_TitleBarMinButton:
        buttonType = ButtonMinimize;
        break;
    case SP_TitleBarMaxButton:
        buttonType = ButtonMaximize;
        break;
    case SP_TitleBarCloseButton:
    case SP_DockWidgetCloseButton:
        buttonType = ButtonClose;
        break;

    default:
        return QIcon();
    }

    // store palette
    // due to Qt, it is not always safe to assume that either option, nor widget are defined
    QPalette palette;
    if (option) {
        palette = option->palette;
    } else if (widget) {
        palette = widget->palette();
    } else {
        palette = QApplication::palette();
    }

    bool isCloseButton(buttonType == ButtonClose && Adwaita::Config::OutlineCloseButton);

    palette.setCurrentColorGroup(QPalette::Active);
    QColor base(palette.color(QPalette::WindowText));
    QColor selected(palette.color(QPalette::HighlightedText));
    QColor negative(buttonType == ButtonClose ? Colors::negativeText(StyleOptions(palette, _variant)) : base);
    QColor negativeSelected(buttonType == ButtonClose ? Colors::negativeText(StyleOptions(palette, _variant)) : selected);

    bool invertNormalState(isCloseButton);

    // convenience class to map color to icon mode
    struct IconData {
        QColor _color;
        bool _inverted;
        QIcon::Mode _mode;
        QIcon::State _state;
    };

    // map colors to icon states
    const QList<IconData> iconTypes = {
        // state off icons
        { Colors::mix(palette.color(QPalette::Window), base,  0.5), invertNormalState, QIcon::Normal, QIcon::Off },
        { Colors::mix(palette.color(QPalette::Window), selected, 0.5), invertNormalState, QIcon::Selected, QIcon::Off },
        { Colors::mix(palette.color(QPalette::Window), negative, 0.5), true, QIcon::Active, QIcon::Off },
        { Colors::mix(palette.color(QPalette::Window), base, 0.2), invertNormalState, QIcon::Disabled, QIcon::Off },

        // state on icons
        { Colors::mix(palette.color(QPalette::Window), negative, 0.7), true, QIcon::Normal, QIcon::On },
        { Colors::mix(palette.color(QPalette::Window), negativeSelected, 0.7), true, QIcon::Selected, QIcon::On },
        { Colors::mix(palette.color(QPalette::Window), negative, 0.7), true, QIcon::Active, QIcon::On },
        { Colors::mix(palette.color(QPalette::Window), base, 0.2), invertNormalState, QIcon::Disabled, QIcon::On }
    };

    // default icon sizes
    static const QList<int> iconSizes = { 8, 16, 22, 32, 48 };

    // output icon
    QIcon icon;

    foreach (const IconData &iconData, iconTypes) {
        foreach (const int &iconSize, iconSizes) {
            // create pixmap
            QPixmap pixmap(iconSize, iconSize);
            pixmap.fill(Qt::transparent);

            // create painter and render
            QPainter painter(&pixmap);
            StyleOptions styleOptions(&painter, pixmap.rect());
            styleOptions.setColor(iconData._color);
            styleOptions.setColorVariant(_variant);

            Adwaita::Renderer::renderDecorationButton(styleOptions, buttonType);

            painter.end();

            // store
            icon.addPixmap(pixmap, iconData._mode, iconData._state);
        }
    }

    return icon;
}

//______________________________________________________________________________
const QAbstractItemView *Style::itemViewParent(const QWidget *widget) const
{
    const QAbstractItemView *itemView(nullptr);

    // check widget directly
    if ((itemView = qobject_cast<const QAbstractItemView *>(widget))) {
        return itemView;
    // check widget grand-parent
    } else if (widget && widget->parentWidget() && (itemView = qobject_cast<const QAbstractItemView *>(widget->parentWidget()->parentWidget()))
             && itemView->viewport() == widget->parentWidget()) {
        return itemView;
    } else {
        return nullptr;
    }
}

//____________________________________________________________________
bool Style::isSelectedItem(const QWidget *widget, const QPoint &localPosition) const
{
    // get relevant itemview parent and check
    const QAbstractItemView *itemView(itemViewParent(widget));
    if (!(itemView && itemView->hasFocus() && itemView->selectionModel())) {
        return false;
    }

    QPoint position = widget->mapTo(itemView, localPosition);

    // get matching QModelIndex and check
    QModelIndex index(itemView->indexAt(position));
    if (!index.isValid()) {
        return false;
    }

    // check whether index is selected
    return itemView->selectionModel()->isSelected(index);
}

//____________________________________________________________________
bool Style::isQtQuickControl(const QStyleOption *option, const QWidget *widget) const
{
    return (widget == nullptr) && option && option->styleObject && option->styleObject->inherits("QQuickItem");
}

//____________________________________________________________________
bool Style::showIconsInMenuItems(void) const
{
    return Adwaita::Settings::ShowIconsInMenuItems && !QCoreApplication::testAttribute(Qt::AA_DontShowIconsInMenus);
}

//____________________________________________________________________
bool Style::showIconsOnPushButtons(void) const
{
    return Adwaita::Settings::ShowIconsOnPushButtons;
}

//____________________________________________________________________
bool Style::isMenuTitle(const QWidget *widget) const
{
    // check widget
    if (!widget) {
        return false;
    }

    // check property
    QVariant property(widget->property(PropertyNames::menuTitle));
    if (property.isValid()) {
        return property.toBool();
    }

    // detect menu toolbuttons
    QWidget *parent = widget->parentWidget();
    if (qobject_cast<QMenu *>(parent)) {
        foreach (auto child, parent->findChildren<QWidgetAction *>()) {
            if (child->defaultWidget() != widget) {
                continue;
            }
            const_cast<QWidget *>(widget)->setProperty(PropertyNames::menuTitle, true);
            return true;
        }
    }

    const_cast<QWidget *>(widget)->setProperty(PropertyNames::menuTitle, false);
    return false;
}

//____________________________________________________________________
bool Style::hasAlteredBackground(const QWidget *widget) const
{
    // check widget
    if (!widget) {
        return false;
    }

    // check property
    QVariant property(widget->property(PropertyNames::alteredBackground));
    if (property.isValid()) {
        return property.toBool();
    }

    // check if widget is of relevant type
    bool hasAlteredBackground(false);
    if (const QGroupBox *groupBox = qobject_cast<const QGroupBox *>(widget)) {
        hasAlteredBackground = !groupBox->isFlat();
    } else if (const QTabWidget *tabWidget = qobject_cast<const QTabWidget *>(widget)) {
        hasAlteredBackground = !tabWidget->documentMode();
    } else if (qobject_cast<const QMenu *>(widget)) {
        hasAlteredBackground = true;
    } else if (Adwaita::Config::DockWidgetDrawFrame && qobject_cast<const QDockWidget *>(widget)) {
        hasAlteredBackground = true;
    }

    if (widget->parentWidget() && !hasAlteredBackground) {
        hasAlteredBackground = this->hasAlteredBackground(widget->parentWidget());
    }
    const_cast<QWidget *>(widget)->setProperty(PropertyNames::alteredBackground, hasAlteredBackground);
    return hasAlteredBackground;
}

//____________________________________________________________________
bool Style::hasCustomTextColors(const QPalette &palette) const
{
    const QPalette adwaitaPalette = Colors::palette(_dark ?  ColorVariant::AdwaitaDark : ColorVariant::Adwaita);

    const QColor activeTextColor = palette.color(QPalette::Active, QPalette::Text);
    const QColor inactiveTextColor = palette.color(QPalette::Inactive, QPalette::Text);
    const QColor adwaitaActiveTextColor = adwaitaPalette.color(QPalette::Active, QPalette::Text);
    const QColor adwaitaInactiveTextColor = adwaitaPalette.color(QPalette::Inactive, QPalette::Text);

    return activeTextColor != adwaitaActiveTextColor || inactiveTextColor != adwaitaInactiveTextColor;
}

}
