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
#include <QtGui/QRgb>

#include "adwaita.h"
#include "config.h"

Adwaita::Adwaita() : QCommonStyle() {
    QFile f(ADWAITA_CSS_FILE);
    f.open(QIODevice::ReadOnly);
    m_styleSheet = f.readAll();
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
}

void Adwaita::polish(QApplication* app)
{
//     app->setStyleSheet(m_styleSheet);
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
            return 16;
        case PM_ButtonShiftVertical:
        case PM_ButtonShiftHorizontal:
        case PM_ButtonDefaultIndicator:
            return 0;
        case PM_SliderThickness:
        case PM_SliderControlThickness:
            return 20;
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
        case PE_FrameTabBarBase: {
            p->save();
            p->setBrush(QColor("#d6d6d6"));
            p->drawRect(opt->rect.adjusted(0,0,-1,-1));
            p->restore();
            break;
        }
        case PE_FrameTabWidget: {
            const QStyleOptionTabWidgetFrameV2 *twOpt = qstyleoption_cast<const QStyleOptionTabWidgetFrameV2 *>(opt);
            const int stripWidth = 32;
            QRect north(twOpt->rect.left(), twOpt->rect.top() - stripWidth + 1, twOpt->rect.width() - 1, stripWidth - 1);
            QRect south(twOpt->rect.left(), twOpt->rect.bottom(), twOpt->rect.width() - 1, stripWidth - 1);
            QRect east(twOpt->rect.left() - stripWidth + 1, twOpt->rect.top(), stripWidth - 1, twOpt->rect.height() - 1);
            QRect west(twOpt->rect.right(), twOpt->rect.top(), stripWidth - 1, twOpt->rect.height() - 1);
            p->save();
            p->setPen(QColor("#a1a1a1"));
            p->setBrush(Qt::transparent);
            p->drawRect(twOpt->rect.adjusted(0,0,-1,-1));
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
        case PE_Frame:
        case PE_FrameDefaultButton:
        case PE_PanelButtonTool:
        case PE_FrameButtonTool:
        case PE_FrameMenu: {
            break;
        }
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
            p->setBrush(Qt::white);
            p->setPen(QColor("#a1a1a1"));
            p->drawRoundedRect(rect, 3, 3);
            p->restore();
            break;
        }
        case PE_FrameFocusRect:
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
        case CE_ToolButtonLabel:
            break;
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
            p->save();
            p->setPen(Qt::transparent);
            p->setBrush(Qt::white);
            p->drawRect(opt->rect);
            p->restore();
            break;
        }
        case CE_MenuItem: {
            const QStyleOptionMenuItem *miopt = qstyleoption_cast<const QStyleOptionMenuItem*>(opt);
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
        case CC_ToolButton: {
            const QStyleOptionToolButton *tbOpt = qstyleoption_cast<const QStyleOptionToolButton*>(opt);
            QRect button = subControlRect(control, opt, SC_ToolButton, widget);
            p->save();
            if (opt->state & (State_MouseOver)) {
                p->setPen(Qt::NoPen);
                QLinearGradient buttonGradient(0.0, button.top(), 0.0, button.bottom());
                if (opt->state & State_On || opt->state & State_Sunken) {
                    buttonGradient.setColorAt(0.0, QColor("#a8a8a8"));
                    buttonGradient.setColorAt(0.05, QColor("#c0c0c0"));
                    buttonGradient.setColorAt(0.15, QColor("#d6d6d6"));
                }
                else {
                    buttonGradient.setColorAt(0.0, QColor("#fafafa"));
                    buttonGradient.setColorAt(1.0, QColor("#e0e0e0"));
                }
                p->setBrush(QBrush(buttonGradient));
                p->drawRect(button);
            }

            p->drawPixmap(button.topLeft() + QPoint(button.width() < tbOpt->iconSize.width() + 20 ? ((button.width() - tbOpt->iconSize.width()) / 2) : 12, (button.height() - tbOpt->iconSize.height()) / 2), tbOpt->icon.pixmap(tbOpt->iconSize));
            p->setPen(opt->palette.windowText().color());
            p->drawText(button.adjusted(tbOpt->iconSize.width() + 16, 0, 0, 0), Qt::AlignVCenter, tbOpt->text);
            p->restore();
            break;
        }
        case CC_SpinBox: {
            const QStyleOptionSpinBox *sbOpt = qstyleoption_cast<const QStyleOptionSpinBox*>(opt);
            QRect frame = subControlRect(control, sbOpt, SC_SpinBoxFrame).adjusted(0, 0, -1, -1);
            QRect up = subControlRect(control, sbOpt, SC_SpinBoxUp).adjusted(0, 0, -1, -1);
            QRect down = subControlRect(control, sbOpt, SC_SpinBoxDown).adjusted(0, 0, -1, -1);
            QRect edit = subControlRect(control, sbOpt, SC_SpinBoxEditField).adjusted(0, 0, -1, -1);
            p->save();
            p->setPen("#a8a8a8");
            p->setBrush(Qt::white);
            p->drawRoundedRect(frame, 3, 3);
            p->setPen("#d6d6d6");
            p->drawLine(up.topLeft() + QPoint(0, 1), up.bottomLeft());
            p->drawLine(down.topLeft() + QPoint(0, 1), down.bottomLeft());
            p->setPen(QColor("#383e40"));
            p->setBrush(QColor("#383e40"));
            p->drawRect(QRect(down.center() - QPoint(3, -1), down.center() + QPoint(5, 1)));
            p->drawRect(QRect(up.center() - QPoint(3, -1), up.center() + QPoint(5, 1)));
            p->drawRect(QRect(up.center() - QPoint(-1, 3), up.center() + QPoint(1, 5)));
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
                else {
                    buttonGradient.setColorAt(0.0, QColor("#fafafa"));
                    buttonGradient.setColorAt(1.0, QColor("#e0e0e0"));
                }
            }
            else {
                buttonGradient.setColorAt(0.0, opt->palette.button().color());
            }
            QBrush buttonBrush(buttonGradient);

            p->save();/*
            p->setBrush(Qt::yellow);
            p->drawRect(popup);*/
            p->setPen(QColor("#a1a1a1"));
            p->setBrush(buttonBrush);
            p->drawRoundedRect(frame, 3, 3);
            if (cbOpt->editable) {
                p->drawLine(arrow.topLeft(), arrow.bottomLeft());
            }
            //p->drawRoundedRect(arrow, 3, 3);
            p->setBrush(QColor("2e3436"));
            p->setPen(QColor("#2e3436"));
            QPolygon triangle;
            triangle.append(arrow.center() + QPoint(-4, -1));
            triangle.append(arrow.center() + QPoint( 4, -1));
            triangle.append(arrow.center() + QPoint( 0,  3));
//             triangle.append(arrow.center() + QPoint( 1,  2));
            p->setRenderHint(QPainter::Antialiasing, false);
            p->drawPolygon(triangle, Qt::WindingFill);
            p->setRenderHint(QPainter::Antialiasing, false);
            if (cbOpt->editable) {
                p->setBrush(Qt::white);
                p->setPen(Qt::transparent);
                p->drawRect(editField.adjusted(2, 1, 0, -1));
                p->drawRect(editField.adjusted(1, 2, 0, -2));
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
            bgGrad.setColorAt(0.0, QColor("#b2b2b2"));
            bgGrad.setColorAt(0.2, QColor("#d2d2d2"));
            bgGrad.setColorAt(0.9, QColor("#d2d2d2"));
            bgGrad.setColorAt(1.0, QColor("#b2b2b2"));
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

            // handle
            QLinearGradient buttonGradient(0.0, handle.top(), 0.0, handle.bottom());
            if (opt->state & State_Active && opt->state & State_Enabled) {
                if (opt->state & State_On) {
                    buttonGradient.setColorAt(0.0, QColor("#a8a8a8"));
                    buttonGradient.setColorAt(0.05, QColor("#c0c0c0"));
                    buttonGradient.setColorAt(0.15, QColor("#d6d6d6"));
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
        case CC_ToolButton: {
            const QStyleOptionToolButton *tbOpt = qstyleoption_cast<const QStyleOptionToolButton*>(opt);
            switch (sc) {
                case SC_ToolButton:
                case SC_ToolButtonMenu:
                    break;
            }
            break;
        }
        case CC_SpinBox: {
            const QStyleOptionSpinBox *cbOpt = qstyleoption_cast<const QStyleOptionSpinBox*>(opt);
            switch (sc) {
                case SC_SpinBoxUp: {
                    return QRect(opt->rect.right() - opt->rect.height() * 1.2 - 1, opt->rect.top(), opt->rect.height() * 1.2 + 1, opt->rect.height());
                }
                case SC_SpinBoxDown: {
                    return QRect(opt->rect.right() - 2 * (opt->rect.height() * 1.2 + 1), opt->rect.top(), opt->rect.height() * 1.2 + 1, opt->rect.height());
                }
                case SC_SpinBoxEditField: {
                    return QRect(opt->rect.left(), opt->rect.top(), opt->rect.width() - 2 * (opt->rect.height() * 1.2) + 1, opt->rect.height());
                }
            }
            break;
        }
        case CC_ComboBox: {
            const QStyleOptionComboBox *cbOpt = qstyleoption_cast<const QStyleOptionComboBox*>(opt);
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
            return QCommonStyle::sizeFromContents(ct, opt, contentsSize, widget) + QSize(8, 10);
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
            return QCommonStyle::sizeFromContents(ct, opt, contentsSize, widget) + QSize(8, 0);
        }
        case CT_PushButton:
            return QCommonStyle::sizeFromContents(ct, opt, contentsSize, widget) + QSize(8, 6);
        case CT_LineEdit:
            return QCommonStyle::sizeFromContents(ct, opt, contentsSize, widget) + QSize(8, 10);
        default:
            return QCommonStyle::sizeFromContents(ct, opt, contentsSize, widget);
    }
}


