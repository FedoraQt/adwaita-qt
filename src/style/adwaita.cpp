/*
 * Adwaita Qt Theme
 * Copyright (C) 2014 Martin Bříza <mbriza@redhat.com>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.

 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301 USA
 *
 */

#include <QtGui>
#include <QtGui/QPainter>
#include <QtGui/QTransform>
#include <QtGui/QRgb>
#include <QtGui/QSizePolicy>

#include "adwaita.h"
#include "config.h"


Adwaita::Adwaita() : QCommonStyle() {
    m_iconMap[IT_CheckBox][0]
            = QPixmap(QString("%1/%2").arg(ADWAITA_ASSET_DIR).arg("checkbox-unchecked-backdrop-insensitive.png"));
    m_iconMap[IT_CheckBox][State_Active]
            = QPixmap(QString("%1/%2").arg(ADWAITA_ASSET_DIR).arg("checkbox-unchecked-insensitive.png"));
    m_iconMap[IT_CheckBox][State_Active | State_Enabled]
            = QPixmap(QString("%1/%2").arg(ADWAITA_ASSET_DIR).arg("checkbox-unchecked.png"));
    m_iconMap[IT_CheckBox][               State_Enabled]
            = QPixmap(QString("%1/%2").arg(ADWAITA_ASSET_DIR).arg("checkbox-unchecked-backdrop.png"));
    m_iconMap[IT_CheckBox][State_Active | State_Enabled | State_Sunken]
            = QPixmap(QString("%1/%2").arg(ADWAITA_ASSET_DIR).arg("checkbox-unchecked-active.png"));
    m_iconMap[IT_CheckBox][                                              State_On]
            = QPixmap(QString("%1/%2").arg(ADWAITA_ASSET_DIR).arg("checkbox-checked-backdrop-insensitive.png"));
    m_iconMap[IT_CheckBox][State_Active                                | State_On]
            = QPixmap(QString("%1/%2").arg(ADWAITA_ASSET_DIR).arg("checkbox-checked-insensitive.png"));
    m_iconMap[IT_CheckBox][State_Active | State_Enabled                | State_On]
            = QPixmap(QString("%1/%2").arg(ADWAITA_ASSET_DIR).arg("checkbox-checked.png"));
    m_iconMap[IT_CheckBox][               State_Enabled                | State_On]
            = QPixmap(QString("%1/%2").arg(ADWAITA_ASSET_DIR).arg("checkbox-checked-backdrop.png"));
    m_iconMap[IT_CheckBox][State_Active | State_Enabled | State_Sunken | State_On]
            = QPixmap(QString("%1/%2").arg(ADWAITA_ASSET_DIR).arg("checkbox-checked-active.png"));
    m_iconMap[IT_CheckBox][                                              State_NoChange]
            = QPixmap(QString("%1/%2").arg(ADWAITA_ASSET_DIR).arg("checkbox-mixed-backdrop-insensitive.png"));
    m_iconMap[IT_CheckBox][State_Active                                | State_NoChange]
            = QPixmap(QString("%1/%2").arg(ADWAITA_ASSET_DIR).arg("checkbox-mixed-insensitive.png"));
    m_iconMap[IT_CheckBox][State_Active | State_Enabled                | State_NoChange]
            = QPixmap(QString("%1/%2").arg(ADWAITA_ASSET_DIR).arg("checkbox-mixed.png"));
    m_iconMap[IT_CheckBox][               State_Enabled                | State_NoChange]
            = QPixmap(QString("%1/%2").arg(ADWAITA_ASSET_DIR).arg("checkbox-mixed-backdrop.png"));
    m_iconMap[IT_CheckBox][State_Active | State_Enabled | State_Sunken | State_NoChange]
            = QPixmap(QString("%1/%2").arg(ADWAITA_ASSET_DIR).arg("checkbox-checked-active.png"));

    m_iconMap[IT_RadioButton][0]
            = QPixmap(QString("%1/%2").arg(ADWAITA_ASSET_DIR).arg("radio-unchecked-backdrop-insensitive.png"));
    m_iconMap[IT_RadioButton][State_Active]
            = QPixmap(QString("%1/%2").arg(ADWAITA_ASSET_DIR).arg("radio-unchecked-insensitive.png"));
    m_iconMap[IT_RadioButton][State_Active | State_Enabled]
            = QPixmap(QString("%1/%2").arg(ADWAITA_ASSET_DIR).arg("radio-unchecked.png"));
    m_iconMap[IT_RadioButton][               State_Enabled]
            = QPixmap(QString("%1/%2").arg(ADWAITA_ASSET_DIR).arg("radio-unchecked-backdrop.png"));
    m_iconMap[IT_RadioButton][State_Active | State_Enabled | State_Sunken]
            = QPixmap(QString("%1/%2").arg(ADWAITA_ASSET_DIR).arg("radio-unchecked-active.png"));
    m_iconMap[IT_RadioButton][                                              State_On]
            = QPixmap(QString("%1/%2").arg(ADWAITA_ASSET_DIR).arg("radio-checked-backdrop-insensitive.png"));
    m_iconMap[IT_RadioButton][State_Active                                | State_On]
            = QPixmap(QString("%1/%2").arg(ADWAITA_ASSET_DIR).arg("radio-checked-insensitive.png"));
    m_iconMap[IT_RadioButton][State_Active | State_Enabled                | State_On]
            = QPixmap(QString("%1/%2").arg(ADWAITA_ASSET_DIR).arg("radio-checked.png"));
    m_iconMap[IT_RadioButton][               State_Enabled                | State_On]
            = QPixmap(QString("%1/%2").arg(ADWAITA_ASSET_DIR).arg("radio-checked-backdrop.png"));
    m_iconMap[IT_RadioButton][State_Active | State_Enabled | State_Sunken | State_On]
            = QPixmap(QString("%1/%2").arg(ADWAITA_ASSET_DIR).arg("radio-checked-active.png"));
    m_iconMap[IT_RadioButton][                                              State_NoChange]
            = QPixmap(QString("%1/%2").arg(ADWAITA_ASSET_DIR).arg("radio-mixed-backdrop-insensitive.png"));
    m_iconMap[IT_RadioButton][State_Active                                | State_NoChange]
            = QPixmap(QString("%1/%2").arg(ADWAITA_ASSET_DIR).arg("radio-mixed-insensitive.png"));
    m_iconMap[IT_RadioButton][State_Active | State_Enabled                | State_NoChange]
            = QPixmap(QString("%1/%2").arg(ADWAITA_ASSET_DIR).arg("radio-mixed.png"));
    m_iconMap[IT_RadioButton][               State_Enabled                | State_NoChange]
            = QPixmap(QString("%1/%2").arg(ADWAITA_ASSET_DIR).arg("radio-mixed-backdrop.png"));
    m_iconMap[IT_RadioButton][State_Active | State_Enabled | State_Sunken | State_NoChange]
            = QPixmap(QString("%1/%2").arg(ADWAITA_ASSET_DIR).arg("radio-checked-active.png"));
}

void Adwaita::polish(QPalette &palette)
{
    // All, used especially for active elements in a focused window
    palette.setColor(QPalette::All,      QPalette::Window,          QColor("#ededed"));
    palette.setColor(QPalette::All,      QPalette::WindowText,      QColor("#2e3436"));
    palette.setColor(QPalette::All,      QPalette::Base,            QColor("white"));
    palette.setColor(QPalette::All,      QPalette::AlternateBase,   QColor("#ededed"));
    palette.setColor(QPalette::All,      QPalette::ToolTipBase,     QColor("#060606"));
    palette.setColor(QPalette::All,      QPalette::ToolTipText,     QColor("white"));
    palette.setColor(QPalette::All,      QPalette::Text,            QColor("#2e3436"));
    palette.setColor(QPalette::All,      QPalette::Button,          QColor("#e0e0e0"));
    palette.setColor(QPalette::All,      QPalette::ButtonText,      QColor("#2e3436"));
    palette.setColor(QPalette::All,      QPalette::BrightText,      QColor("white"));

    palette.setColor(QPalette::All,      QPalette::Light,           QColor("#fafafa"));
    palette.setColor(QPalette::All,      QPalette::Midlight,        QColor("#f3f3f3"));
    palette.setColor(QPalette::All,      QPalette::Dark,            QColor("#e0e0e0"));
    palette.setColor(QPalette::All,      QPalette::Mid,             QColor("#e6e6e6"));
    palette.setColor(QPalette::All,      QPalette::Shadow,          QColor("black"));

    palette.setColor(QPalette::All,      QPalette::Highlight,       QColor("#4a90d9"));
    palette.setColor(QPalette::All,      QPalette::HighlightedText, QColor("white"));

    palette.setColor(QPalette::All,      QPalette::Link,            QColor("#2a76c6"));
    palette.setColor(QPalette::All,      QPalette::LinkVisited,     QColor("#2a76c6"));

    // Exceptions for disabled elements in a focused window
    palette.setColor(QPalette::Disabled, QPalette::Window,          QColor("#f4f4f4"));
    palette.setColor(QPalette::Disabled, QPalette::WindowText,      QColor("#8d9091"));
    palette.setColor(QPalette::Disabled, QPalette::Base,            QColor("white"));
    palette.setColor(QPalette::Disabled, QPalette::AlternateBase,   QColor("#ededed"));
//     palette.setColor(QPalette::Disabled, QPalette::ToolTipBase,     QColor("#ff1234"));
//     palette.setColor(QPalette::Disabled, QPalette::ToolTipText,     QColor("#ff1234"));
    palette.setColor(QPalette::Disabled, QPalette::Text,            QColor("#8d9091"));
    palette.setColor(QPalette::Disabled, QPalette::Button,          QColor("#f4f4f4"));
    palette.setColor(QPalette::Disabled, QPalette::ButtonText,      QColor("#8d9091"));
    palette.setColor(QPalette::Disabled, QPalette::BrightText,      QColor("#ededed"));

    palette.setColor(QPalette::Disabled, QPalette::Light,           QColor("#f4f4f4"));
    palette.setColor(QPalette::Disabled, QPalette::Midlight,        QColor("#f4f4f4"));
    palette.setColor(QPalette::Disabled, QPalette::Dark,            QColor("#f4f4f4"));
    palette.setColor(QPalette::Disabled, QPalette::Mid,             QColor("#f4f4f4"));
    palette.setColor(QPalette::Disabled, QPalette::Shadow,          QColor("black"));

    palette.setColor(QPalette::Disabled, QPalette::Highlight,       QColor("#4a90d9"));
    palette.setColor(QPalette::Disabled, QPalette::HighlightedText, QColor("white"));

    palette.setColor(QPalette::Disabled, QPalette::Link,            QColor("#4a90d9"));
    palette.setColor(QPalette::Disabled, QPalette::LinkVisited,     QColor("#4a90d9"));


    // Exceptions for an unfocused window
    palette.setColor(QPalette::Inactive, QPalette::Window,          QColor("#ededed"));
    palette.setColor(QPalette::Inactive, QPalette::WindowText,      QColor("#54595a"));
    palette.setColor(QPalette::Inactive, QPalette::Base,            QColor("#fcfcfc"));
    palette.setColor(QPalette::Inactive, QPalette::AlternateBase,   QColor("#ededed"));
//     palette.setColor(QPalette::Inactive, QPalette::ToolTipBase,     QColor("#ff1234"));
//     palette.setColor(QPalette::Inactive, QPalette::ToolTipText,     QColor("#ff1234"));
    palette.setColor(QPalette::Inactive, QPalette::Text,            QColor("#54595a"));
    palette.setColor(QPalette::Inactive, QPalette::Button,          QColor("#ededed"));
    palette.setColor(QPalette::Inactive, QPalette::ButtonText,      QColor("#54595a"));
    palette.setColor(QPalette::Inactive, QPalette::BrightText,      QColor("#ededed"));

    palette.setColor(QPalette::Inactive, QPalette::Light,           QColor("#ededed"));
    palette.setColor(QPalette::Inactive, QPalette::Midlight,        QColor("#ededed"));
    palette.setColor(QPalette::Inactive, QPalette::Dark,            QColor("#ededed"));
    palette.setColor(QPalette::Inactive, QPalette::Mid,             QColor("#ededed"));
    palette.setColor(QPalette::Inactive, QPalette::Shadow,          QColor("black"));

    palette.setColor(QPalette::Inactive, QPalette::Highlight,       QColor("#4a90d9"));
    palette.setColor(QPalette::Inactive, QPalette::HighlightedText, QColor("white"));

    palette.setColor(QPalette::Inactive, QPalette::Link,            QColor("#4a90d9"));
    palette.setColor(QPalette::Inactive, QPalette::LinkVisited,     QColor("#4a90d9"));
}


void Adwaita::polish(QWidget *widget)
{
    widget->setAttribute( Qt::WA_Hover );
    if (widget->inherits("QTabBar")) {
        qobject_cast<QTabBar*>(widget)->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    }
}

void Adwaita::polish(QApplication* app)
{
}


void Adwaita::unpolish(QWidget *widget)
{
}

void Adwaita::unpolish(QApplication* app)
{
}


int Adwaita::pixelMetric(PixelMetric metric, const QStyleOption *opt, const QWidget *widget) const
{
    switch(metric) {
        case PM_DefaultFrameWidth:
            return 2;
        case PM_ToolBarFrameWidth:
            return 0;
        case PM_ToolBarItemMargin:
        case PM_ToolBarItemSpacing:
            return 0;
        case PM_MenuBarHMargin:
        case PM_MenuBarVMargin:
        case PM_MenuHMargin:
        case PM_MenuVMargin:
        case PM_MenuDesktopFrameWidth:
        case PM_MenuBarPanelWidth:
            return 0;
        case PM_TabBarTabHSpace:
        case PM_TabBarTabVSpace:
        case PM_TabBarBaseHeight:
            return 19;
        case PM_TabBarTabShiftHorizontal:
        case PM_TabBarTabShiftVertical:
        case PM_TabBarBaseOverlap:
            return 0;
        case PM_ButtonShiftVertical:
        case PM_ButtonShiftHorizontal:
        case PM_ButtonDefaultIndicator:
            return 0;
        case PM_SliderThickness:
        case PM_SliderControlThickness:
            return 20;
        case PM_ScrollBarExtent:
            return 11;
        case PM_SplitterWidth:
            return 6;
        default:
            return QCommonStyle::pixelMetric(metric, opt, widget);
    }
}

int Adwaita::styleHint(StyleHint hint, const QStyleOption *opt, const QWidget *widget,
                       QStyleHintReturn *returnData) const
{
    switch (hint) {
        case QStyle::SH_EtchDisabledText:
        case QStyle::SH_Menu_SloppySubMenus:
        case QStyle::SH_Menu_MouseTracking:
        case QStyle::SH_MenuBar_MouseTracking:
            return 1;
        case QStyle::PM_ProgressBarChunkWidth:
            return 0;
        default:
            return QCommonStyle::styleHint(hint, opt, widget, returnData);
    }
}

void Adwaita::drawPrimitive(PrimitiveElement element, const QStyleOption *opt, QPainter *p,
                            const QWidget *widget) const
{
    switch(element) {
        case PE_FrameStatusBarItem:
            break;
        case PE_IndicatorDockWidgetResizeHandle: {
            QStyleOption optCopy(*opt);
            if (opt->rect.height() > opt->rect.width())
                optCopy.state |= State_Horizontal;
            else
                optCopy.state &= ~State_Horizontal;
            drawControl(CE_Splitter, &optCopy, p, widget);
            break;
        }
        case PE_FrameDockWidget:
        case PE_FrameGroupBox: {
            p->save();
            p->setPen(QColor("#a1a1a1"));
            p->setBrush(Qt::transparent);
            p->drawRect(opt->rect.adjusted(0, 0, -1, -1));
            p->restore();
            break;
        }
        case PE_IndicatorSpinMinus:
        case PE_IndicatorSpinDown:
        case PE_IndicatorSpinPlus:
        case PE_IndicatorSpinUp: {
            p->save();
            p->setPen(opt->palette.windowText().color());
            p->drawRect(QRect(opt->rect.center() - QPoint(3, -1), opt->rect.center() + QPoint(5, 1)));
            if (element == PE_IndicatorSpinPlus || element == PE_IndicatorSpinUp)
                p->drawRect(QRect(opt->rect.center() - QPoint(-1, 3), opt->rect.center() + QPoint(1, 5)));
            p->restore();
            break;
        }
        case PE_IndicatorCheckBox: {
            p->save();
            p->drawPixmap(opt->rect, m_iconMap[IT_CheckBox][opt->state & (State_Active | State_Enabled | State_Sunken | State_NoChange | State_On)]);
            p->restore();
            break;
        }
        case PE_IndicatorRadioButton: {
            p->save();
            p->drawPixmap(opt->rect, m_iconMap[IT_RadioButton][opt->state & (State_Active | State_Enabled | State_Sunken | State_NoChange | State_On)]);
            p->restore();
            break;
        }
        case PE_FrameTabBarBase: {
            const QStyleOptionTabBarBase *tbbOpt = qstyleoption_cast<const QStyleOptionTabBarBase *>(opt);
            if (0 && widget && widget->parentWidget() && widget->parentWidget()->inherits("QTabWidget")) {
                qDebug() << widget->parentWidget()->metaObject()->className();
                return;
            }
            QRect rect = tbbOpt->tabBarRect.width() * tbbOpt->tabBarRect.height() > tbbOpt->rect.height() * tbbOpt->rect.width() ? tbbOpt->tabBarRect : tbbOpt->rect;
            p->save();
            QLinearGradient shadowGradient(0.0, 0.0, 0.0, 1.0);
            shadowGradient.setCoordinateMode(QGradient::ObjectBoundingMode);
            shadowGradient.setColorAt(0.0, QColor("#b0b0b0"));
            shadowGradient.setColorAt(1.0/(rect.height()+1)*4, Qt::transparent);
            p->setPen(QColor("#a1a1a1"));
            p->setBrush(QColor("#d6d6d6"));
            p->drawRect(rect.adjusted(1,1,-1,-1));
            p->setBrush(QBrush(shadowGradient));
            p->drawRect(rect.adjusted(1,1,-1,-1));
            p->restore();
            break;
        }
        case PE_FrameTabWidget: {
            const QStyleOptionTabWidgetFrame *twOpt = qstyleoption_cast<const QStyleOptionTabWidgetFrame*>(twOpt);
            const int stripWidth = 35;
            QRect north(opt->rect.left(), opt->rect.top() - stripWidth + 1, opt->rect.width() - 1, stripWidth - 1);
            QRect south(opt->rect.left(), opt->rect.bottom(), opt->rect.width() - 1, stripWidth - 1);
            QRect east(opt->rect.left() - stripWidth + 1, opt->rect.top(), stripWidth - 1, opt->rect.height() - 1);
            QRect west(opt->rect.right(), opt->rect.top(), stripWidth - 1, opt->rect.height() - 1);
            p->save();
            p->setPen(QColor("#a1a1a1"));
            p->setBrush(Qt::transparent);
            p->drawRect(opt->rect.adjusted(0,0,-1,-1));
            p->setBrush(QColor("#d6d6d6"));
            p->drawRect(north);
            p->drawRect(south);
            p->drawRect(west);
            p->drawRect(east);
            QLinearGradient shadowGradient(0.0, 0.0, 0.0, 1.0);
            shadowGradient.setCoordinateMode(QGradient::ObjectBoundingMode);
            shadowGradient.setColorAt(0.0, QColor("#b0b0b0"));
            shadowGradient.setColorAt(1.0/(north.height()+1)*4, Qt::transparent);
            p->setBrush(QBrush(shadowGradient));
            p->drawRect(north);
            p->drawRect(south);
            shadowGradient.setColorAt(0.0, QColor("#b0b0b0"));
            shadowGradient.setColorAt(1.0/(east.height()+1)*4, Qt::transparent);
            p->setBrush(QBrush(shadowGradient));
            p->drawRect(east);
            p->drawRect(west);
            p->restore();
            break;
        }
        case PE_FrameDefaultButton:
        case PE_Frame:
        case PE_FrameButtonTool: {
            p->save();
            p->setPen(QColor("#a1a1a1"));
            p->setBrush(Qt::transparent);
            p->drawRect(opt->rect.adjusted(0, 0, -1, -1));
            p->restore();
            break;
        }
        case PE_IndicatorButtonDropDown: {
            p->save();
            p->setPen(Qt::black);
            p->setBrush(Qt::black);
            QPolygon triangle;
            triangle.append(opt->rect.center() + QPoint(-4, -1));
            triangle.append(opt->rect.center() + QPoint( 4, -1));
            triangle.append(opt->rect.center() + QPoint( 0,  3));
            p->drawPolygon(triangle, Qt::WindingFill);
            p->restore();
            break;
        }
        case PE_IndicatorToolBarHandle: {
            p->save();
            p->setPen(Qt::NoPen);
            p->setBrush(QBrush(QColor("#a1a1a1"), Qt::Dense7Pattern));
            p->drawRect(opt->rect.adjusted(0,0,-1,-1));
            p->restore();
        }
        case PE_IndicatorToolBarSeparator: {
            p->save();
            p->setPen("#d6d6d6");
            p->drawLine(opt->rect.center().x(), opt->rect.top() + 4, opt->rect.center().x(), opt->rect.bottom() - 4);
            p->restore();
            break;
        }
        case PE_PanelButtonTool: {
            p->save();
            p->setPen("#a8a8a8");
            bool visible = widget && widget->parentWidget() && widget->parentWidget()->inherits("QTabBar");
            QLinearGradient buttonGradient(0.0, opt->rect.top(), 0.0, opt->rect.bottom());
            if (visible) {
                buttonGradient.setColorAt(0.0, QColor("#fafafa"));
                buttonGradient.setColorAt(1.0, QColor("#e0e0e0"));
                p->setBrush(QBrush(buttonGradient));
                p->drawRoundedRect(opt->rect.adjusted(1, 1, -2, -2), 3, 3);
            } else if(opt->state & State_On || opt->state & State_Sunken) {
                buttonGradient.setColorAt(0.0, QColor("#a8a8a8"));
                buttonGradient.setColorAt(0.05, QColor("#c0c0c0"));
                buttonGradient.setColorAt(0.15, QColor("#d6d6d6"));
                p->setBrush(QBrush(buttonGradient));
                p->drawRoundedRect(opt->rect.adjusted(1, 1, -2, -2), 3, 3);
            }
            else if (opt->state & State_MouseOver) {
                buttonGradient.setColorAt(0.0, QColor("white"));
                buttonGradient.setColorAt(0.4, QColor("#f7f7f7"));
                buttonGradient.setColorAt(1.0, QColor("#ededed"));
                p->setBrush(QBrush(buttonGradient));
                p->drawRoundedRect(opt->rect.adjusted(1, 1, -2, -2), 3, 3);
            }
            p->restore();
            break;
        }
        case PE_FrameMenu:
            break;
        case PE_PanelMenuBar: {
            p->save();
            p->setPen(Qt::transparent);
            p->setBrush(opt->palette.window());
            p->drawRect(opt->rect);
            p->restore();
            break;
        }
        case PE_IndicatorBranch: {
            p->save();
            if (opt->state & State_Children) {
                p->setPen(QColor("#a1a1a1"));
                p->setBrush(QColor("#a1a1a1"));
                QPolygon triangle;
                if (opt->state & State_Open) {
                    triangle.append(opt->rect.center() + QPoint(-4, -1));
                    triangle.append(opt->rect.center() + QPoint( 4, -1));
                    triangle.append(opt->rect.center() + QPoint( 0,  3));
                }
                else {
                    triangle.append(opt->rect.center() + QPoint(-1, -4));
                    triangle.append(opt->rect.center() + QPoint(-1,  4));
                    triangle.append(opt->rect.center() + QPoint( 3,  0));
                }
                p->setRenderHint(QPainter::Antialiasing, false);
                p->drawPolygon(triangle, Qt::WindingFill);
                p->setRenderHint(QPainter::Antialiasing, false);
            }
            else {
                p->setPen(QColor("#d6d6d6"));
                if (opt->state & State_Item) {
                    p->drawLine(opt->rect.center().x(), opt->rect.top(), opt->rect.center().x(), opt->rect.center().y());
                    p->drawLine(opt->rect.center().x(), opt->rect.center().y(), opt->rect.right(), opt->rect.center().y());
                }
                if (opt->state & State_Sibling) {
                    p->drawLine(opt->rect.center().x(), opt->rect.top(), opt->rect.center().x(), opt->rect.bottom());
                }
            }
            p->restore();
            break;
        }
        case PE_PanelButtonCommand: {
            QRect rect = opt->rect.adjusted(0, 0, -1, -1);
            QLinearGradient buttonGradient(0.0, rect.top(), 0.0, rect.bottom());
            if (opt->state & State_Active && opt->state & State_Enabled) {
                if (opt->state & State_On || opt->state & State_Sunken) {
                    buttonGradient.setColorAt(0.0, QColor("#a8a8a8"));
                    buttonGradient.setColorAt(0.05, QColor("#c0c0c0"));
                    buttonGradient.setColorAt(0.15, QColor("#d6d6d6"));
                }
                else if (opt->state & State_MouseOver) {
                    buttonGradient.setColorAt(0.0, QColor("white"));
                    buttonGradient.setColorAt(0.4, QColor("#f7f7f7"));
                    buttonGradient.setColorAt(1.0, QColor("#ededed"));
                }
                else {
                    buttonGradient.setColorAt(0.0, QColor("#fafafa"));
                    buttonGradient.setColorAt(1.0, QColor("#e0e0e0"));
                }
            }
            else {
                buttonGradient.setColorAt(0.0, opt->palette.button().color());
            }
            QBrush buttonBrush(buttonGradient);
            p->save();
            p->setBrush(buttonBrush);
            p->setPen(QColor("#a1a1a1"));
            p->drawRoundedRect(rect, 3, 3);
            p->restore();
            break;
        }
        case PE_PanelLineEdit: {
            if (widget && widget->parentWidget() && (widget->parentWidget()->inherits("QComboBox") || widget->parentWidget()->inherits("QAbstractSpinBox"))) {
                break;
            }
            QRect rect = opt->rect.adjusted(0, 0, -1, -1);
            p->save();
            QLinearGradient shadowGradient(0.0, 0.0, 0.0, 1.0);
            QLinearGradient backgroundGradient(0.0, 0.0, 0.0, 1.0);
            shadowGradient.setCoordinateMode(QGradient::ObjectBoundingMode);
            shadowGradient.setColorAt(0.0, QColor("#d4d4d4"));
            shadowGradient.setColorAt(1.0/(rect.height()+1)*4, Qt::transparent);
            backgroundGradient.setCoordinateMode(QGradient::ObjectBoundingMode);
            backgroundGradient.setColorAt(0.0, QColor("#f3f3f3"));
            backgroundGradient.setColorAt(1.0/(rect.height()+1)*16, Qt::white);
            p->setBrush(QBrush(backgroundGradient));
            if (opt->state & State_HasFocus) {
                p->setPen(opt->palette.highlight().color());
            }
            else {
                p->setPen(QColor("#a1a1a1"));
            }
            p->drawRoundedRect(rect, 3, 3);
            if (opt->state & State_Active && opt->state & State_Enabled) {
                p->setBrush(QBrush(shadowGradient));
                p->drawRoundedRect(rect, 3, 3);
            }
            else if (!(opt->state & State_Enabled)) {
                p->setBrush(opt->palette.button());
                p->drawRoundedRect(rect, 3, 3);
            }
            p->restore();
            break;
        }
        case PE_FrameFocusRect: {
            p->save();
            p->setBrush(Qt::transparent);
            QPen dotPen(QBrush(QColor("#a1a1a1")), 1, Qt::DotLine);
            p->setPen(dotPen);
            p->drawRoundedRect(opt->rect.adjusted(0, 0, -1, -1), 2, 2);
            p->restore();
            break;
        }
        case PE_IndicatorProgressChunk:
            break;
        default:
            QCommonStyle::drawPrimitive(element, opt, p, widget);
            break;
    }
}



void Adwaita::drawControl(ControlElement element, const QStyleOption *opt, QPainter *p,
                          const QWidget *widget) const
{
    switch(element) {
/*
        case CE_ShapedFrame: {
            const QStyleOptionFrameV3* frameOpt = qstyleoption_cast<const QStyleOptionFrameV3*>(opt);
            if(!frameOpt) {
                break;
            }
            switch(frameOpt->frameShape) {
                case QFrame::Box:
                case QFrame::Panel:
                case QFrame::StyledPanel:
                case QFrame::WinPanel: {
                    p->setPen(QPen(QColor("#a1a1a1"), frameOpt->lineWidth));
                    p->setBrush(Qt::transparent);
                    p->drawRect(opt->rect.adjusted(0, 0, -1, -1));
                    break;
                }
                case QFrame::HLine:
                    p->setPen(QPen(QColor("#a1a1a1"), frameOpt->lineWidth));
                    p->drawLine(opt->rect.topLeft(), opt->rect.topRight());
                    break;
                case QFrame::VLine:
                    p->setPen(QPen(QColor("#a1a1a1"), frameOpt->lineWidth));
                    p->drawLine(opt->rect.topLeft(), opt->rect.bottomLeft());
                    break;
                default:
                    break;
            }
            break;
*/
        }
        case CE_DockWidgetTitle: {
            // cast option and check
            const QStyleOptionDockWidgetV2* dwOpt = ::qstyleoption_cast<const QStyleOptionDockWidgetV2*>( opt );
            if (!dwOpt) {
                QCommonStyle::drawControl(element, opt, p, widget);
                return;
            }
            QRect rect = opt->rect;
            p->save();
            p->setPen(Qt::NoPen);
            p->setBrush(QBrush(QColor("#a1a1a1"), Qt::Dense7Pattern));
            p->drawRect(rect.adjusted(0,0,-1,-1));
            if (dwOpt && dwOpt->verticalTitleBar) {
                p->translate(-rect.left(), -rect.top()); // move the coordinate system to 0,0
                p->rotate(270); // rotate by 270°
                p->translate(-rect.height(), 0);
                p->setBrush(Qt::red);
                rect = QRect(rect.left(), rect.top(), rect.height(), rect.width());
            }
            drawItemText(p, rect, Qt::AlignCenter | Qt::AlignVCenter | Qt::TextShowMnemonic, opt->palette, opt->state & State_Enabled, dwOpt->title, QPalette::WindowText);
            p->restore();
            break;
        }
        case CE_TabBarTabShape: {
            const QStyleOptionTab *tbOpt = qstyleoption_cast<const QStyleOptionTab *>(opt);
            if (!tbOpt) {
                QCommonStyle::drawControl(element, opt, p, widget);
                return;
            }
            p->save();

            QPen underline;
            underline.setWidth(3);
            if (opt->state & State_Selected)
                underline.setColor(QColor("#4a90d9"));
            else if (opt->state & State_MouseOver)
                underline.setColor(QColor("#a1a1a1"));
            else
                underline.setColor(Qt::transparent);
            p->setPen(underline);

            switch (tbOpt->shape) {
                case QTabBar::RoundedNorth:
                case QTabBar::TriangularNorth:
                    p->drawLine(opt->rect.bottomLeft() + QPoint(6, -1), opt->rect.bottomRight() - QPoint(6, 1));
                    break;
                case QTabBar::RoundedEast:
                case QTabBar::TriangularEast:
                    p->drawLine(opt->rect.topLeft() + QPoint(1, 6), opt->rect.bottomLeft() - QPoint(-1, 6));
                    break;
                case QTabBar::RoundedWest:
                case QTabBar::TriangularWest:
                    p->drawLine(opt->rect.topRight() + QPoint(-1, 6), opt->rect.bottomRight() - QPoint(1, 6));
                    break;
                case QTabBar::RoundedSouth:
                case QTabBar::TriangularSouth:
                    p->drawLine(opt->rect.topLeft() + QPoint(6, 1), opt->rect.topRight() - QPoint(6, -1));
                    break;
                default:
                    break;
            }
            p->restore();
            break;
        }
        case CE_Splitter: {
            p->save();
            p->setPen(Qt::NoPen);
            p->setBrush(QColor("#c3c3c3"));
            p->drawRect(opt->rect);
            p->setPen(QColor("#a1a1a1"));
            if (opt->state & State_Horizontal) {
                p->drawLine(opt->rect.left() + 1, opt->rect.center().y() + 2, opt->rect.right() - 1, opt->rect.center().y() + 2);
                p->drawLine(opt->rect.left() + 1, opt->rect.center().y() - 2, opt->rect.right() - 1, opt->rect.center().y() - 2);
                p->drawLine(opt->rect.left() + 1, opt->rect.center().y(), opt->rect.right() - 1, opt->rect.center().y());
            }
            else {
                p->drawLine(opt->rect.center().x() + 2, opt->rect.top() + 1, opt->rect.center().x() + 2, opt->rect.bottom() - 1);
                p->drawLine(opt->rect.center().x() - 2, opt->rect.top() + 1, opt->rect.center().x() - 2, opt->rect.bottom() - 1);
                p->drawLine(opt->rect.center().x(), opt->rect.top() + 1, opt->rect.center().x(), opt->rect.bottom() - 1);
            }
            p->restore();
            break;
        }
        case CE_HeaderLabel:
        case CE_HeaderSection: {
            p->save();
            p->setBrush(Qt::red);
            p->setPen(Qt::NoPen);
            p->drawRect(opt->rect);
            p->restore();
            break;
        }
        case CE_Header: {
            p->save();
            const QStyleOptionHeader *hopt = qstyleoption_cast<const QStyleOptionHeader *>(opt);
            if (!hopt) {
                QCommonStyle::drawControl(element, opt, p, widget);
                return;
            }
            p->setBrush(opt->palette.base());
            p->setPen(QColor("#ededed"));
            p->drawRect(opt->rect.adjusted(-1, -1, -1, -1));
            p->setPen(QColor("#c3c3c3"));
            p->drawText(opt->rect, Qt::AlignCenter | Qt::TextShowMnemonic, hopt->text);
            p->restore();
            break;
        }
        case CE_HeaderEmptyArea: {
            p->save();
            p->setBrush(Qt::white);
            p->setPen(Qt::NoPen);
            p->drawRect(opt->rect);
            p->restore();
            break;
        }
        case CE_ToolButtonLabel: {
            const QStyleOptionToolButton *tbOpt = qstyleoption_cast<const QStyleOptionToolButton*>(opt);
            if (!tbOpt) {
                QCommonStyle::drawControl(element, opt, p, widget);
                return;
            }
            QStyleOptionToolButton tbOptTmp(*tbOpt);
            if (tbOptTmp.rect.height() > tbOptTmp.rect.width())
                tbOptTmp.rect.adjust(0, 5, 0, -5);
            else
                tbOptTmp.rect.adjust(5, 0, -5, 0);
            QCommonStyle::drawControl(element, &tbOptTmp, p, widget);
            break;
        }
        case CE_ToolBar: {
            p->save();
            p->setPen(Qt::NoPen);
            p->setBrush(opt->palette.window());
            p->drawRect(opt->rect);
            p->restore();
            break;
        }
        case CE_MenuBarEmptyArea: {
            p->save();
            p->setPen(QColor("#d6d6d6"));
            p->drawLine(opt->rect.bottomLeft(), opt->rect.bottomRight());
            p->restore();
            break;
        }
        case CE_MenuBarItem: {
            const QStyleOptionMenuItem *miopt = qstyleoption_cast<const QStyleOptionMenuItem*>(opt);
            if (!miopt) {
                QCommonStyle::drawControl(element, opt, p, widget);
                return;
            }

            p->save();
            p->setPen(Qt::NoPen);
            p->setBrush(opt->palette.window());
            p->drawRect(opt->rect);

            p->setPen(QColor("#d6d6d6"));
            p->drawLine(opt->rect.bottomLeft(), opt->rect.bottomRight());

            if (miopt->state & State_Sunken) {
                p->setBrush(QColor("#4a90d9"));
                p->drawRect(miopt->rect.left(), miopt->rect.bottom() - 2, miopt->rect.right(), miopt->rect.bottom());
                p->setPen(QColor("#4a90d9"));
            }
            else
                p->setPen(opt->palette.windowText().color());

            drawItemText(p,
                         miopt->rect,
                         Qt::AlignCenter | Qt::AlignVCenter | Qt::TextShowMnemonic | Qt::TextDontClip | Qt::TextSingleLine,
                         miopt->palette,
                         miopt->state & State_Enabled,
                         miopt->text,
                         QPalette::NoRole);
            p->restore();
            break;
        }
        case CE_MenuEmptyArea: {
            const QStyleOptionMenuItem *miopt = qstyleoption_cast<const QStyleOptionMenuItem*>(opt);
            if (!miopt) {
                QCommonStyle::drawControl(element, opt, p, widget);
                return;
            }
            p->save();
            p->setPen(Qt::transparent);
            p->setBrush(Qt::white);
            p->drawRect(opt->rect);
            p->restore();
            break;
        }
        case CE_MenuItem: {
            const QStyleOptionMenuItem *miopt = qstyleoption_cast<const QStyleOptionMenuItem*>(opt);
            if (!miopt) {
                QCommonStyle::drawControl(element, opt, p, widget);
                return;
            }
            if (miopt->menuItemType == QStyleOptionMenuItem::Separator) {
                p->save();
                p->setPen(Qt::NoPen);
                p->setBrush(Qt::white);
                p->drawRect(miopt->rect);
                p->setPen(QColor("#d6d6d6"));
                p->drawLine(miopt->rect.left() + 8, miopt->rect.center().y(), miopt->rect.right() - 8, miopt->rect.center().y());
                p->restore();
                break;
            }
            QRect rect = miopt->rect;
            p->save();
            if (miopt->state & State_Selected)
                p->setBrush(opt->palette.highlight());
            else
                p->setBrush(Qt::white);
            p->setPen(Qt::transparent);
            p->drawRect(rect);
            if (miopt->state & State_Selected)
                p->setPen(Qt::white);
            else
                p->setPen(opt->palette.windowText().color());
            if (miopt->menuItemType & QStyleOptionMenuItem::SubMenu) {
                if (miopt->state & State_Selected) {
                    p->setPen(Qt::white);
                    p->setBrush(Qt::white);
                }
                else {
                    p->setBrush(QColor("2e3436"));
                    p->setPen(QColor("#2e3436"));
                }
                QPolygon triangle;
                triangle.append(QPoint(rect.right() - 7, rect.center().y() - 3));
                triangle.append(QPoint(rect.right() - 7, rect.center().y() + 3));
                triangle.append(QPoint(rect.right() - 4, rect.center().y()));
                p->setRenderHint(QPainter::Antialiasing, false);
                p->drawPolygon(triangle, Qt::WindingFill);
                p->setRenderHint(QPainter::Antialiasing, false);
            }
            if (miopt->maxIconWidth) {
                QPixmap icon;
                // TODO: improve performance - change colors only once
                if (miopt->checkType == QStyleOptionMenuItem::QStyleOptionMenuItem::NonExclusive) {
                    if (miopt->checked)
                        icon = QIcon::fromTheme("checkbox-checked-symbolic", QIcon(":/checkbox-checked-symbolic")).pixmap(16, 16);
                    else
                        icon = QIcon::fromTheme("checkbox-symbolic", QIcon(":/checkbox-symbolic")).pixmap(16, 16);

                    if (miopt->state & State_Enabled) {
                        QImage iconImage = icon.toImage();
                        for (int i = 0; i < icon.height(); i++) {
                            for (int j = 0; j < icon.width(); j++) {
                                uint8_t red = qRed(iconImage.pixel(j, i));
                                uint8_t green = qGreen(iconImage.pixel(j, i));
                                uint8_t blue = qBlue(iconImage.pixel(j, i));
                                uint8_t alpha = qAlpha(iconImage.pixel(j, i));
                                red = red < 200 ? red >> 1 : red;
                                green = green < 200 ? green >> 1 : green;
                                blue = blue < 200 ? blue >> 1 : blue;
                                iconImage.setPixel(j, i, qRgba(red, green, blue, alpha));
                            }
                        }
                        icon = QPixmap::fromImage(iconImage);
                    }
                }
                else if (miopt->checkType == QStyleOptionMenuItem::QStyleOptionMenuItem::Exclusive) {
                    if (miopt->checked)
                        icon = QIcon::fromTheme("radio-checked-symbolic", QIcon(":/radio-checked-symbolic")).pixmap(16, 16);
                    else
                        icon = QIcon::fromTheme("radio-symbolic", QIcon(":/radio-symbolic")).pixmap(16, 16);

                    if (miopt->state & State_Enabled) {
                        QImage iconImage = icon.toImage();
                        for (int i = 0; i < icon.height(); i++) {
                            for (int j = 0; j < icon.width(); j++) {
                                uint8_t red = qRed(iconImage.pixel(j, i));
                                uint8_t green = qGreen(iconImage.pixel(j, i));
                                uint8_t blue = qBlue(iconImage.pixel(j, i));
                                uint8_t alpha = qAlpha(iconImage.pixel(j, i));
                                red = red < 200 ? red >> 1 : red;
                                green = green < 200 ? green >> 1 : green;
                                blue = blue < 200 ? blue >> 1 : blue;
                                iconImage.setPixel(j, i, qRgba(red, green, blue, alpha));
                            }
                        }
                        icon = QPixmap::fromImage(iconImage);
                    }
                }
                else {
                    icon = miopt->icon.pixmap(16, 16);
                }
                drawItemPixmap(p, QRect(4, opt->rect.center().y() - 8, 16, 16), Qt::AlignCenter, icon);

                QStringList split = miopt->text.split('\t');
                drawItemText(p,
                            rect.adjusted(24,0,-8,0),
                            Qt::AlignLeft | Qt::AlignVCenter | Qt::TextShowMnemonic | Qt::TextDontClip | Qt::TextSingleLine,
                            miopt->palette,
                            miopt->state & State_Enabled,
                            split[0],
                            QPalette::NoRole);
                if (split.count() > 1) {
                    p->setPen(Qt::gray);
                    drawItemText(p,
                                 rect.adjusted(24,0,-8,0),
                                 Qt::AlignRight | Qt::AlignVCenter | Qt::TextShowMnemonic | Qt::TextDontClip | Qt::TextSingleLine,
                                 miopt->palette,
                                 miopt->state & State_Enabled,
                                 split[1],
                                 QPalette::NoRole);
                }
            }
            else {
                drawItemText(p,
                            rect.adjusted(16,0,-8,0),
                            Qt::AlignLeft | Qt::AlignVCenter | Qt::TextShowMnemonic | Qt::TextDontClip | Qt::TextSingleLine,
                            miopt->palette,
                            miopt->state & State_Enabled,
                            miopt->text,
                            QPalette::NoRole);
            }
            p->restore();
            break;
        }
        case CE_ProgressBarGroove: {
            const QStyleOptionProgressBarV2 *pbopt = qstyleoption_cast<const QStyleOptionProgressBarV2*>(opt);
            if (!pbopt) {
                QCommonStyle::drawControl(element, opt, p, widget);
                return;
            }
            QRect rect = pbopt->rect.adjusted(0, 0, -1, -1);
            p->save();
            QLinearGradient bgGrad;
            if (pbopt->orientation == Qt::Horizontal)
                bgGrad = QLinearGradient(0.0, rect.top()+1, 0.0, rect.bottom()+1);
            else
                bgGrad = QLinearGradient(rect.left()+1, 0.0, rect.right()+1, 0.0);
            bgGrad.setColorAt(0.0, QColor("#b2b2b2"));
            bgGrad.setColorAt(0.2, QColor("#d2d2d2"));
            bgGrad.setColorAt(0.9, QColor("#d2d2d2"));
            bgGrad.setColorAt(1.0, QColor("#b2b2b2"));
            p->setBrush(QBrush(bgGrad));
            p->setPen(QColor("#a1a1a1"));
            p->drawRoundedRect(rect, 3, 3);
            p->restore();
            break;
        }
        case CE_ProgressBarContents: {
            const QStyleOptionProgressBarV2 *pbopt = qstyleoption_cast<const QStyleOptionProgressBarV2*>(opt);
            if (!pbopt) {
                QCommonStyle::drawControl(element, opt, p, widget);
                return;
            }
            QRect rect = pbopt->rect.adjusted(0, 0, -1, -1);
            p->save();
            QLinearGradient bgGrad;
            if (pbopt->orientation == Qt::Horizontal) {
                if (pbopt->progress >= 0) {
                    qreal ratio = (((qreal) pbopt->progress) - pbopt->minimum) / (((qreal) pbopt->maximum) - pbopt->minimum);
                    if (pbopt->invertedAppearance)
                        rect.adjust(rect.width() * ratio, 0, 0, 0);
                    else
                        rect.adjust(0, 0, -(rect.width() * ratio), 0);
                }
                bgGrad = QLinearGradient(0.0, rect.top()+1, 0.0, rect.bottom()+1);
            }
            else {
                if (pbopt->progress >= 0) {
                    qreal ratio = (((qreal) pbopt->progress) - pbopt->minimum) / (((qreal) pbopt->maximum) - pbopt->minimum);
                    if (pbopt->invertedAppearance)
                        rect.adjust(0, rect.height() * ratio, 0, 0);
                    else
                        rect.adjust(0, 0, 0, -(rect.height() * ratio));
                }
                bgGrad = QLinearGradient(rect.left()+1, 0.0, rect.right()+1, 0.0);
            }
            bgGrad.setColorAt(0.0, QColor("#4081C5"));
            if (pbopt->progress >= 0) {
                bgGrad.setColorAt(0.1, QColor("#4C91D9"));
                bgGrad.setColorAt(0.95, QColor("#4C91D9"));
            }
            else {
                bgGrad.setColorAt(0.1, QColor("#94CAFF"));
                bgGrad.setColorAt(0.95, QColor("#94CAFF"));
            }
            bgGrad.setColorAt(1.0, QColor("#4081C5"));
            p->setBrush(QBrush(bgGrad));
            p->setPen(QColor("black"));
            p->drawRoundedRect(rect, 2, 2);
            p->restore();
            break;
        }
        case CE_ProgressBarLabel: {
            const QStyleOptionProgressBarV2 *pbopt = qstyleoption_cast<const QStyleOptionProgressBarV2*>(opt);
            if (!pbopt) {
                QCommonStyle::drawControl(element, opt, p, widget);
                return;
            }
            p->save();
            QFont font = p->font();
            font.setPointSize(8);
            p->setFont(font);
            p->setPen("#a8a8a8");
            p->setBrush(Qt::transparent);
            p->drawText(pbopt->rect, Qt::AlignHCenter | Qt::AlignBottom, pbopt->text);
            p->restore();
            break;
        }
        default:
            QCommonStyle::drawControl(element, opt, p, widget);
            break;
    }
}

void Adwaita::drawComplexControl(QStyle::ComplexControl control, const QStyleOptionComplex* opt,
                                 QPainter* p, const QWidget* widget) const
{
    switch(control) {
        case CC_ScrollBar: {
            const QStyleOptionSlider *slOpt = qstyleoption_cast<const QStyleOptionSlider *>(opt);
            QRect slider = subControlRect(CC_ScrollBar, slOpt, SC_ScrollBarSlider).adjusted(2, 2, -2, -2);
            p->save();
            p->setPen(Qt::NoPen);
            p->setBrush(slOpt->palette.dark());
            p->drawRect(slOpt->rect);
            p->setBrush(QColor("#b3b5b6"));
            p->setRenderHint(QPainter::Antialiasing, true);
            p->drawRoundedRect(slider, 3, 3);
            p->setRenderHint(QPainter::Antialiasing, false);
            p->restore();
            break;
        }
        case CC_ToolButton: {
            QCommonStyle::drawComplexControl(control, opt, p, widget);
            break;
        }
        case CC_SpinBox: {
            const QStyleOptionSpinBox *sbOpt = qstyleoption_cast<const QStyleOptionSpinBox*>(opt);
            QRect frame = subControlRect(control, sbOpt, SC_SpinBoxFrame).adjusted(0, 0, -1, -1);
            QRect up = subControlRect(control, sbOpt, SC_SpinBoxUp).adjusted(0, 0, -1, -1);
            QRect down = subControlRect(control, sbOpt, SC_SpinBoxDown).adjusted(0, 0, -1, -1);
            QRect edit = subControlRect(control, sbOpt, SC_SpinBoxEditField).adjusted(0, 0, -1, -1);
            p->save();
            if (sbOpt->state & State_HasFocus)
                p->setPen(sbOpt->palette.highlight().color());
            else
                p->setPen("#a8a8a8");
            QLinearGradient backgroundGradient(0.0, 0.0, 0.0, 1.0);
            backgroundGradient.setCoordinateMode(QGradient::ObjectBoundingMode);
            backgroundGradient.setColorAt(0.0, QColor("#f3f3f3"));
            backgroundGradient.setColorAt(1.0/(frame.height()+1)*16, Qt::white);
            p->setBrush(QBrush(backgroundGradient));
            p->drawRoundedRect(frame, 3, 3);
            if (opt->state & State_Enabled && opt->state & State_Active) {
                QLinearGradient shadowGradient(0.0, 0.0, 0.0, 1.0);
                shadowGradient.setCoordinateMode(QGradient::ObjectBoundingMode);
                shadowGradient.setColorAt(0.0, QColor("#d4d4d4"));
                shadowGradient.setColorAt(1.0/(frame.height()+1)*4, Qt::transparent);
                p->setBrush(QBrush(shadowGradient));
                p->drawRoundedRect(frame, 3, 3);
            }
            p->setPen("#d6d6d6");
            if (sbOpt->subControls & (SC_SpinBoxEditField | SC_SpinBoxDown )&& sbOpt->subControls & SC_SpinBoxUp)
                p->drawLine(up.topLeft() + QPoint(0, 1), up.bottomLeft());
            if (sbOpt->subControls & (SC_SpinBoxEditField) && sbOpt->subControls & SC_SpinBoxDown)
            p->drawLine(down.topLeft() + QPoint(0, 1), down.bottomLeft());

            p->setPen(Qt::NoPen);
            if (opt->state & State_MouseOver)
                p->setBrush(QColor("#f0f0f0"));
            if (opt->state & State_Sunken)
                p->setBrush(QColor("#e1e1e1"));
            if (opt->state & State_Enabled && opt->activeSubControls == SC_SpinBoxDown && sbOpt->stepEnabled & QAbstractSpinBox::StepDownEnabled) {
                p->drawRect(down.adjusted(1, 1, 1, 0));
            }
            if (opt->state & State_Enabled && opt->activeSubControls == SC_SpinBoxUp && sbOpt->stepEnabled & QAbstractSpinBox::StepUpEnabled) {
                QPainterPath path;
                path.setFillRule(Qt::WindingFill);
                path.addRoundedRect(up.adjusted(1, 1, 2, 0), 3, 3);
                path.addRect(up.adjusted(1, 1, -1, -1));
                p->drawPath(path);
            }

            QStyleOptionSpinBox optCopy(*sbOpt);
            optCopy.rect = up;
            if (!(sbOpt->stepEnabled & QAbstractSpinBox::StepUpEnabled))
                optCopy.palette.setCurrentColorGroup(QPalette::Disabled);
            drawPrimitive(PE_IndicatorSpinUp, &optCopy, p, widget);
            optCopy.rect = down;
            optCopy.palette.setCurrentColorGroup(opt->palette.currentColorGroup());
            if (!(sbOpt->stepEnabled & QAbstractSpinBox::StepDownEnabled))
                optCopy.palette.setCurrentColorGroup(QPalette::Disabled);
            drawPrimitive(PE_IndicatorSpinDown, &optCopy, p, widget);
            p->restore();
            break;
        }
        case CC_ComboBox: {
            const QStyleOptionComboBox *cbOpt = qstyleoption_cast<const QStyleOptionComboBox*>(opt);
            QRect frame = subControlRect(control, cbOpt, SC_ComboBoxFrame).adjusted(0, 0, -1, -1);
            QRect arrow = subControlRect(control, cbOpt, SC_ComboBoxArrow).adjusted(-1, 0, -1, -1);
            QRect editField = subControlRect(control, cbOpt, SC_ComboBoxEditField).adjusted(0, 0, -1, 0);
            QRect popup = subControlRect(control, cbOpt, SC_ComboBoxListBoxPopup).adjusted(0, 0, -1, -1);

            QLinearGradient buttonGradient(0.0, frame.top(), 0.0, frame.bottom());
            if (opt->state & State_Active && opt->state & State_Enabled) {
                if (opt->state & State_On || opt->state & State_Sunken) {
                    buttonGradient.setColorAt(0.0, QColor("#a8a8a8"));
                    buttonGradient.setColorAt(0.05, QColor("#c0c0c0"));
                    buttonGradient.setColorAt(0.15, QColor("#d6d6d6"));
                }
                else if (opt->state & State_MouseOver) {
                    buttonGradient.setColorAt(0.0, QColor("white"));
                    buttonGradient.setColorAt(0.4, QColor("#f7f7f7"));
                    buttonGradient.setColorAt(1.0, QColor("#ededed"));
                }
                else {
                    buttonGradient.setColorAt(0.0, QColor("#fafafa"));
                    buttonGradient.setColorAt(1.0, QColor("#e0e0e0"));
                }
            }
            else {
                buttonGradient.setColorAt(0.0, opt->palette.button().color());
            }
            QBrush buttonBrush(buttonGradient);

            p->save();
            p->setPen(QColor("#a1a1a1"));
            p->setBrush(buttonBrush);
            p->drawRoundedRect(frame, 3, 3);
            if (cbOpt->editable) {
                p->drawLine(arrow.topLeft(), arrow.bottomLeft());
            }
            else if (cbOpt->state & State_HasFocus) {
                QStyleOption copyOpt(*cbOpt);
                copyOpt.rect = cbOpt->rect.adjusted(3, 3, -3, -3);
                drawPrimitive(QStyle::PE_FrameFocusRect, &copyOpt, p, widget);
            }
            p->setBrush(QColor("2e3436"));
            p->setPen(QColor("#2e3436"));
            QPolygon triangle;
            triangle.append(arrow.center() + QPoint(-4, -1));
            triangle.append(arrow.center() + QPoint( 4, -1));
            triangle.append(arrow.center() + QPoint( 0,  3));
            p->drawPolygon(triangle, Qt::WindingFill);
            if (cbOpt->editable) {
                QLinearGradient shadowGradient(0.0, 0.0, 0.0, 1.0);
                QLinearGradient backgroundGradient(0.0, 0.0, 0.0, 1.0);
                shadowGradient.setCoordinateMode(QGradient::ObjectBoundingMode);
                shadowGradient.setColorAt(0.0, QColor("#d4d4d4"));
                shadowGradient.setColorAt(1.0/(editField.height()+1)*4, Qt::transparent);
                backgroundGradient.setCoordinateMode(QGradient::ObjectBoundingMode);
                backgroundGradient.setColorAt(0.0, QColor("#f3f3f3"));
                backgroundGradient.setColorAt(1.0/(editField.height()+1)*16, Qt::white);
                if (!(opt->state & State_Enabled))
                    p->setBrush(opt->palette.button());
                else
                    p->setBrush(backgroundGradient);
                QPainterPath path;
                path.setFillRule(Qt::WindingFill);
                path.addRoundedRect(editField.adjusted(0, 0, 0, -1), 3, 3);
                path.addRect(editField.adjusted(3, 0, 0, -1));
                if (opt->state & State_HasFocus) {
                    p->setPen(opt->palette.highlight().color());
                }
                else {
                    p->setPen(QColor("#a1a1a1"));
                }
                p->drawPath(path.simplified());
                if (opt->state & State_Active && opt->state & State_Enabled) {
                    p->setBrush(QBrush(shadowGradient));
                    p->drawPath(path.simplified());
                }
            }
            p->restore();
            break;
        }
        case CC_Slider: {
            const QStyleOptionSlider *slOpt = qstyleoption_cast<const QStyleOptionSlider*>(opt);
            QRect handle = subControlRect(control, slOpt, SC_SliderHandle, widget).adjusted(1, 1, -1, -1);
            QRect groove = subControlRect(control, slOpt, SC_SliderGroove, widget).adjusted(0, 0, 0, 0);
            QRect grooveFill(groove);
            QRect tick = subControlRect(control, slOpt, SC_SliderTickmarks, widget);
            p->save();

            // groove
            QLinearGradient bgGrad;
            if (slOpt->orientation == Qt::Horizontal)
                bgGrad = QLinearGradient(0.0, groove.top()+1, 0.0, groove.bottom()+1);
            else
                bgGrad = QLinearGradient(groove.left()+1, 0.0, groove.right()+1, 0.0);
            if (slOpt->state & State_Enabled) {
                bgGrad.setColorAt(0.0, QColor("#b2b2b2"));
                bgGrad.setColorAt(0.2, QColor("#d2d2d2"));
                bgGrad.setColorAt(0.9, QColor("#d2d2d2"));
                bgGrad.setColorAt(1.0, QColor("#b2b2b2"));
            }
            else {
                bgGrad.setColorAt(0.0, slOpt->palette.window().color());
                bgGrad.setColorAt(0.0, slOpt->palette.window().color());
            }
            p->setBrush(QBrush(bgGrad));
            p->setPen(QColor("#a1a1a1"));
            p->drawRoundedRect(groove, 2, 2);

            QLinearGradient fgGrad;
            if (slOpt->orientation == Qt::Horizontal)
                fgGrad = QLinearGradient(0.0, grooveFill.top()+1, 0.0, grooveFill.bottom()+1);
            else
                fgGrad = QLinearGradient(grooveFill.left()+1, 0.0, grooveFill.right()+1, 0.0);
            fgGrad.setColorAt(0.0, QColor("#4081C5"));
            fgGrad.setColorAt(0.1, QColor("#4C91D9"));
            fgGrad.setColorAt(0.6, QColor("#4C91D9"));
            fgGrad.setColorAt(0.8, QColor("#4081C5"));
            if (slOpt->orientation == Qt::Horizontal)
                if (!slOpt->upsideDown)
                    grooveFill.setWidth(grooveFill.width() * ((qreal) slOpt->sliderPosition - slOpt->minimum) / (slOpt->maximum - slOpt->minimum));
                else
                    grooveFill.adjust(grooveFill.width() - grooveFill.width() * ((qreal) slOpt->sliderPosition - slOpt->minimum) / (slOpt->maximum - slOpt->minimum), 0, 0, 0);
            else
                if (!slOpt->upsideDown)
                    grooveFill.setHeight(grooveFill.height() * ((qreal) slOpt->sliderPosition - slOpt->minimum) / (slOpt->maximum - slOpt->minimum));
                else
                    grooveFill.adjust(0, grooveFill.height() - grooveFill.height() * ((qreal) slOpt->sliderPosition - slOpt->minimum) / (slOpt->maximum - slOpt->minimum), 0, 0);
            p->setBrush(QBrush(fgGrad));
            p->drawRoundedRect(grooveFill, 2, 2);

            // tickmarks
            if (slOpt->tickInterval > 0) {
                p->setPen(Qt::transparent);
                p->setBrush(QColor("#a1a1a1"));
                qreal tickStep = 0.0;
                qreal offset = 0.0;
                if (slOpt->maximum > 0 && slOpt->maximum > slOpt->minimum) {
                    if (slOpt->orientation == Qt::Horizontal) {
                        tickStep = (((qreal) groove.width() - groove.height() + 1) / ((qreal) slOpt->maximum - slOpt->minimum));
                        offset = tick.x();
                    }
                    else {
                        tickStep = (((qreal) groove.height() - groove.width() + 1) / ((qreal) slOpt->maximum - slOpt->minimum));
                        offset = tick.y();
                    }
                }
                while (tickStep > 0.0 && tick.right() < (slOpt->rect.right() - handle.width() / 2) && tick.bottom() < (slOpt->rect.bottom() - handle.height() / 2)) {
                    if (slOpt->orientation == Qt::Horizontal)
                        tick.moveLeft(offset);
                    else
                        tick.moveTop(offset);
                    p->drawRect(tick);
                    offset += tickStep;
                }
            }

            if (slOpt->state & State_HasFocus) {
                QStyleOption copyOpt(*slOpt);
                copyOpt.rect = groove.adjusted(-2, -2, 3, 3);
                drawPrimitive(QStyle::PE_FrameFocusRect, &copyOpt, p, widget);
            }

            // handle
            QLinearGradient buttonGradient(0.0, handle.top(), 0.0, handle.bottom());
            if (opt->state & State_Active && opt->state & State_Enabled) {
                if (opt->state & State_On) {
                    buttonGradient.setColorAt(0.0, QColor("#a8a8a8"));
                    buttonGradient.setColorAt(0.05, QColor("#c0c0c0"));
                    buttonGradient.setColorAt(0.15, QColor("#d6d6d6"));
                }
                else if (opt->state & State_MouseOver) {
                    buttonGradient.setColorAt(0.0, QColor("white"));
                    buttonGradient.setColorAt(0.4, QColor("#f7f7f7"));
                    buttonGradient.setColorAt(1.0, QColor("#ededed"));
                }
                else {
                    buttonGradient.setColorAt(0.0, QColor("#fafafa"));
                    buttonGradient.setColorAt(1.0, QColor("#e0e0e0"));
                }
            }
            else {
                buttonGradient.setColorAt(0.0, opt->palette.button().color());
            }
            QBrush buttonBrush(buttonGradient);
            if (slOpt->state & State_Selected)
                p->setPen(QColor("#4a90d9"));
            else
                p->setPen(QColor("#a1a1a1"));
            p->setBrush(buttonBrush);
            p->setRenderHint(QPainter::Antialiasing, true);
            p->drawEllipse(handle);
            p->setRenderHint(QPainter::Antialiasing, false);

            p->restore();
            break;
        }
        default:
            QCommonStyle::drawComplexControl(control, opt, p, widget);
            break;
    }
}

void Adwaita::drawItemPixmap(QPainter* painter, const QRect& rect, int alignment, const QPixmap& pixmap) const
{
    QCommonStyle::drawItemPixmap(painter, rect, alignment, pixmap);
}

void Adwaita::drawItemText(QPainter* painter, const QRect& rect, int alignment, const QPalette& pal,
                           bool enabled, const QString& text, QPalette::ColorRole textRole) const
{
    QCommonStyle::drawItemText(painter, rect, alignment, pal, enabled, text, textRole);
}

QRect Adwaita::subControlRect(QStyle::ComplexControl cc, const QStyleOptionComplex* opt, QStyle::SubControl sc, const QWidget* w) const
{
    switch(cc) {
        case CC_ScrollBar: {
            const QStyleOptionSlider *slOpt = qstyleoption_cast<const QStyleOptionSlider *>(opt);
            if (!slOpt) {
                return QCommonStyle::subControlRect(cc, opt, sc, w);
            }
            const QRect scrollBarRect = slOpt->rect;
            int maxlen = ((slOpt->orientation == Qt::Horizontal) ?
                          scrollBarRect.width() : scrollBarRect.height());
            int sliderlen;

            // calculate slider length
            if (slOpt->maximum != slOpt->minimum) {
                uint range = slOpt->maximum - slOpt->minimum;
                sliderlen = (qint64(slOpt->pageStep) * maxlen) / (range + slOpt->pageStep);

                int slidermin = proxy()->pixelMetric(PM_ScrollBarSliderMin, slOpt, w);
                if (sliderlen < slidermin || range > INT_MAX / 2)
                    sliderlen = slidermin;
                if (sliderlen > maxlen)
                    sliderlen = maxlen;
            } else {
                sliderlen = maxlen;
            }

            int sliderstart = sliderPositionFromValue(slOpt->minimum,
                                                                 slOpt->maximum,
                                                                 slOpt->sliderPosition,
                                                                 maxlen - sliderlen,
                                                                 slOpt->upsideDown);

            switch (sc) {
            case SC_ScrollBarSubLine:            // top/left button
                return QRect(0, 0, 0, 0);
            case SC_ScrollBarAddLine:            // bottom/right button
                return QRect(0, 0, 0, 0);
            case SC_ScrollBarSubPage:            // between top/left button and slider
                if (slOpt->orientation == Qt::Horizontal)
                    return QRect(0, 0, sliderstart, scrollBarRect.height());
                else
                    return QRect(0, 0, scrollBarRect.width(), sliderstart );
            case SC_ScrollBarAddPage:            // between bottom/right button and slider
                if (slOpt->orientation == Qt::Horizontal)
                    return QRect(sliderstart + sliderlen, 0,
                                maxlen - sliderstart - sliderlen + 0, scrollBarRect.height());
                else
                    return QRect(0, sliderstart + sliderlen, scrollBarRect.width(),
                                maxlen - sliderstart - sliderlen + 0);
            case SC_ScrollBarGroove:
                if (slOpt->orientation == Qt::Horizontal)
                    return QRect(0, 0, scrollBarRect.width(),
                                scrollBarRect.height());
                else
                    return QRect(0, 0, scrollBarRect.width(),
                                scrollBarRect.height() );
            case SC_ScrollBarSlider:
                if (slOpt->orientation == Qt::Horizontal)
                    return QRect(sliderstart, 0, sliderlen, scrollBarRect.height());
                else
                    return QRect(0, sliderstart, scrollBarRect.width(), sliderlen);
            default:
                break;
            }
            break;
        }
        case CC_SpinBox: {
            const QStyleOptionSpinBox *cbOpt = qstyleoption_cast<const QStyleOptionSpinBox*>(opt);
            if (!cbOpt) {
                return QCommonStyle::subControlRect(cc, opt, sc, w);
            }
            switch (sc) {
                case SC_SpinBoxUp: {
                    if (cbOpt->subControls & SC_SpinBoxEditField)
                        return QRect(opt->rect.right() - opt->rect.height() * 1.2 - 1, opt->rect.top(), opt->rect.height() * 1.2 + 1, opt->rect.height());
                    else
                        return QRect(opt->rect.right() - opt->rect.height() * 1.2 - 1, opt->rect.top(), opt->rect.height() * 1.2 - 4, opt->rect.height());
                }
                case SC_SpinBoxDown: {
                    if (cbOpt->subControls & SC_SpinBoxEditField)
                        return QRect(opt->rect.right() - 2 * (opt->rect.height() * 1.2 + 1), opt->rect.top(), opt->rect.height() * 1.2 + 1, opt->rect.height());
                    else
                        return QRect(4 + opt->rect.right() - 2 * (opt->rect.height() * 1.2 + 1), opt->rect.top(), opt->rect.height() * 1.2 - 4, opt->rect.height());
                }
                case SC_SpinBoxEditField: {
                    return QRect(opt->rect.left(), opt->rect.top(), opt->rect.width() - 2 * (opt->rect.height() * 1.2) + 1, opt->rect.height());
                }
                case SC_SpinBoxFrame: {
                    if (cbOpt->subControls & SC_SpinBoxEditField)
                        return opt->rect;
                    else
                        return QRect(4 + opt->rect.right() - 2 * (opt->rect.height() * 1.2 + 1), opt->rect.top(), 2 * (opt->rect.height() * 1.2 - 2), opt->rect.height());
                }
            }
            break;
        }
        case CC_ComboBox: {
            const QStyleOptionComboBox *cbOpt = qstyleoption_cast<const QStyleOptionComboBox*>(opt);
            if (!cbOpt) {
                return QCommonStyle::subControlRect(cc, opt, sc, w);
            }
            switch (sc) {
                case SC_ComboBoxArrow: {
                    return QRect(cbOpt->rect.right() - cbOpt->rect.height() * 1.2 + 1, cbOpt->rect.top(), cbOpt->rect.height() * 1.2 + 1, cbOpt->rect.height());
                }
                case SC_ComboBoxEditField: {
                    QRect full = opt->rect;
                    full.setRight(cbOpt->rect.right() - cbOpt->rect.height() * 1.2);
                    if (!cbOpt->editable)
                        full.setLeft(10);
                    return full;
                }
            }
            break;
        }
        case CC_Slider: {
            const QStyleOptionSlider *slOpt = qstyleoption_cast<const QStyleOptionSlider*>(opt);
            if (!slOpt) {
                return QCommonStyle::subControlRect(cc, opt, sc, w);
            }
            switch (sc) {
                case SC_SliderHandle: {
                    QRect handle(0, 0, 0, 0);
                    if (slOpt->orientation == Qt::Horizontal) {
                        qreal ratio = ((qreal) slOpt->sliderPosition - slOpt->minimum) / ((qreal) slOpt->maximum - slOpt->minimum);
                        handle.setWidth(slOpt->rect.height());
                        handle.setHeight(slOpt->rect.height());
                        if (!slOpt->upsideDown)
                            handle.translate((slOpt->rect.width() - handle.width()) * ratio, 0);
                        else
                            handle.translate((slOpt->rect.width() - handle.width()) - (slOpt->rect.width() - handle.width()) * ratio, 0);
                    }
                    else {
                        qreal ratio = ((qreal) slOpt->sliderPosition - slOpt->minimum) / ((qreal) slOpt->maximum - slOpt->minimum);
                        handle.setWidth(slOpt->rect.width());
                        handle.setHeight(slOpt->rect.width());
                        if (!slOpt->upsideDown)
                            handle.translate(0, (slOpt->rect.height() - handle.height()) * ratio);
                        else
                            handle.translate(0, (slOpt->rect.height() - handle.height()) - (slOpt->rect.height() - handle.height()) * ratio);
                    }
                    return handle;
                }
                case SC_SliderGroove: {
                    QRect groove(0, 0, 0, 0);
                    if (slOpt->orientation == Qt::Horizontal) {
                        groove.setHeight(3);
                        groove.setWidth(slOpt->rect.width() - slOpt->rect.height() + 4);
                        groove.translate(slOpt->rect.height() / 2 - 2, (slOpt->rect.height() - groove.height()) / 2);
                    }
                    else {
                        groove.setWidth(3);
                        groove.setHeight(slOpt->rect.height() - slOpt->rect.width() + 4);
                        groove.translate((slOpt->rect.width() - groove.width()) / 2, slOpt->rect.width() / 2 - 2);
                    }
                    return groove;
                }
                case SC_SliderTickmarks: {
                    QRect tick(0, 0, 0, 0);
                    if (slOpt->orientation == Qt::Horizontal) {
                        tick.setHeight(4);
                        tick.setWidth(1);
                        tick.translate((slOpt->rect.height() - tick.width()) / 2, 0);
                    }
                    else {
                        tick.setHeight(1);
                        tick.setWidth(4);
                        tick.translate(0, (slOpt->rect.width() - tick.height()) / 2);
                    }
                    return tick;
                }
            }
            break;
        }
    }
    return QCommonStyle::subControlRect(cc, opt, sc, w);
}

QRect Adwaita::subElementRect(QStyle::SubElement r, const QStyleOption* opt, const QWidget* widget) const
{
    switch(r) {
        case SE_CheckBoxContents:
        case SE_RadioButtonContents: {
            return opt->rect.translated(22, -1);
        }
        case SE_RadioButtonIndicator:
        case SE_CheckBoxIndicator: {
            return QRect(opt->rect.left() + 2, opt->rect.center().y() - 8, 16, 16);
        }
        case SE_ProgressBarGroove:
        case SE_ProgressBarContents: {
            const QStyleOptionProgressBarV2 *pbopt = qstyleoption_cast<const QStyleOptionProgressBarV2*>(opt);
            if (!pbopt) {
                qDebug() << "QStyleOptionProgressBarV2 cast failed!";
                return QCommonStyle::subElementRect(r, opt, widget).adjusted(0, 8, 0, -8);
            }
            if (pbopt->textVisible) {
                if (pbopt->orientation == Qt::Horizontal)
                    return opt->rect.adjusted(0, 13, 0, 0);
                else
                    return opt->rect.adjusted(13, 0, 0, 0);
            }
            else {
                if (pbopt->orientation == Qt::Horizontal)
                    return opt->rect.adjusted(0, 5, 0, -6);
                else
                    return opt->rect.adjusted(5, 0, -6, 0);
            }
        }
        case SE_ProgressBarLabel: {
            const QStyleOptionProgressBarV2 *pbopt = qstyleoption_cast<const QStyleOptionProgressBarV2*>(opt);
            if (!pbopt) {
                qDebug() << "QStyleOptionProgressBarV2 cast failed!";
                return QCommonStyle::subElementRect(r, opt, widget).adjusted(0, 8, 0, -8);
            }
            if (!pbopt->textVisible)
                return QRect(0, 0, 0, 0);
            if (pbopt->orientation == Qt::Horizontal)
                return opt->rect.adjusted(0, 0, 0, -5);
            else
                return opt->rect.adjusted(0, 0, -5, 0);
        }
        case SE_LineEditContents: {
            return opt->rect.adjusted(6, 1, -6, -1);
        }
        default:
            return QCommonStyle::subElementRect(r, opt, widget);
    }
}

QSize Adwaita::sizeFromContents(QStyle::ContentsType ct, const QStyleOption* opt, const QSize& contentsSize, const QWidget* widget) const
{
    switch(ct) {
        case CT_HeaderSection: {
            const QStyleOptionHeader *hopt = qstyleoption_cast<const QStyleOptionHeader *>(opt);
            if (hopt && hopt->text.isEmpty())
                return QSize(0, 0);
            else
                return QCommonStyle::sizeFromContents(ct, opt, contentsSize, widget);
        }
        case CT_ToolButton: {
            return QCommonStyle::sizeFromContents(ct, opt, contentsSize, widget) + QSize(10, 12);
        }
        case CT_MenuBarItem: {
            //const QStyleOptionMenuItem *miopt = qstyleoption_cast<const QStyleOptionMenuItem*>(opt);
            return QSize(QCommonStyle::sizeFromContents(ct, opt, contentsSize, widget).width() + 16, 23);
        }
        case CT_MenuItem: {
            return QCommonStyle::sizeFromContents(ct, opt, contentsSize, widget) + QSize(24, 0);
        }
        case CT_MenuBar: {
            return QSize(23, 23);
        }
        case CT_ComboBox: {
            return QCommonStyle::sizeFromContents(ct, opt, contentsSize, widget) + QSize(4, 6);
        }
        case CT_ProgressBar: {
            if (qstyleoption_cast<const QStyleOptionProgressBarV2*>(opt)->textVisible)
                return QSize(19, 19);
            else
                return QSize(1, 1);
        }
        case CT_Slider: {
            return QSize(20, 20);
        }
        case CT_SpinBox: {
            return QCommonStyle::sizeFromContents(ct, opt, contentsSize, widget) + QSize(12, 2);
        }
        case CT_PushButton:
            return QCommonStyle::sizeFromContents(ct, opt, contentsSize, widget) + QSize(4, 2);
        case CT_LineEdit:
            return QCommonStyle::sizeFromContents(ct, opt, contentsSize, widget) + QSize(6, 8);
        default:
            return QCommonStyle::sizeFromContents(ct, opt, contentsSize, widget);
    }
}

