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
    static bool guard = false;
    if (!guard) {
        guard = true;
        if (widget)
            widget->setStyleSheet(m_styleSheet);
        guard = false;
    }
}

void Adwaita::polish(QApplication* app)
{
    app->setStyleSheet(m_styleSheet);
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
        case PM_ButtonShiftVertical:
        case PM_ButtonShiftHorizontal:
        case PM_ButtonDefaultIndicator:
            return 0;
//         case PM_LayoutHorizontalSpacing:
        case PM_LayoutVerticalSpacing:
            return QCommonStyle::pixelMetric(metric, opt, widget) - 1;
        default:
            return QCommonStyle::pixelMetric(metric, opt, widget);
    }
}

int Adwaita::styleHint(StyleHint hint, const QStyleOption *opt, const QWidget *widget,
                       QStyleHintReturn *returnData) const
{
    switch (hint) {
        case QStyle::SH_Menu_SloppySubMenus:
        case QStyle::SH_Menu_MouseTracking:
        case QStyle::SH_MenuBar_MouseTracking:
            return 1;
        default:
            return QCommonStyle::styleHint(hint, opt, widget, returnData);
    }
}

void Adwaita::drawPrimitive(PrimitiveElement element, const QStyleOption *opt, QPainter *p,
                            const QWidget *widget) const
{
    switch(element) {
        case PE_PanelButtonBevel: {
            p->setPen(Qt::red);
            p->setBrush(Qt::blue);
            p->drawRect(opt->rect);
        }
        case PE_PanelButtonCommand: {
            QRect rect = opt->rect.adjusted(0, 3, -1, -1);
            QLinearGradient buttonGradient(0.0, rect.top(), 0.0, rect.bottom());
            if (opt->state & State_Enabled && !(opt->state & State_On)) {
                buttonGradient.setColorAt(0.0, QColor("#fafafa"));
                buttonGradient.setColorAt(1.0, QColor("#e0e0e0"));
            }
            else if (opt->state & State_Enabled && opt->state & State_On) {
                buttonGradient.setColorAt(0.0, QColor("#a8a8a8"));
                buttonGradient.setColorAt(0.05, QColor("#c0c0c0"));
                buttonGradient.setColorAt(0.15, QColor("#d6d6d6"));
            }
            else {
                buttonGradient.setColorAt(0.0, opt->palette.button().color());
            }
            QBrush buttonBrush(buttonGradient);
            p->setBrush(buttonBrush);
            p->setPen(QColor("#a1a1a1"));
            p->drawRoundedRect(rect, 3, 3);
            break;
        }
        default:
            QCommonStyle::drawPrimitive(element, opt, p, widget);
            break;
    }
}



void Adwaita::drawControl(ControlElement element, const QStyleOption *opt, QPainter *p,
                          const QWidget *widget) const
{
    switch(element) {
        case CE_ProgressBar: {
            p->setBrush(opt->palette.mid());
            QRect troughRect = opt->rect.translated(0, opt->rect.height() / 2 - 3);
            troughRect.setHeight(6);
            troughRect.setWidth(opt->rect.width() - 3);
            p->drawRoundedRect(troughRect, 2, 2);

            p->setBrush(opt->palette.highlight());
            QRect stateRect = opt->rect.translated(1, opt->rect.height() / 2 - 2);
            stateRect.setHeight(4);
            stateRect.setWidth(opt->rect.width() - 5);
            p->drawRoundedRect(stateRect, 1, 1);
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
        default:
            return QCommonStyle::subControlRect(cc, opt, sc, w);
    }
}

QRect Adwaita::subElementRect(QStyle::SubElement r, const QStyleOption* opt, const QWidget* widget) const
{
    switch(r) {
        case SE_PushButtonContents:
            return QCommonStyle::subElementRect(r, opt, widget).adjusted(0, 2, 0, 2);
        case SE_PushButtonLayoutItem:
        case SE_PushButtonFocusRect:
        case SE_ToolButtonLayoutItem:
//             return QCommonStyle::subElementRect(r, opt, widget).adjusted(0, 0, -10, -10);
        default:
            return QCommonStyle::subElementRect(r, opt, widget);
    }
}

QSize Adwaita::sizeFromContents(QStyle::ContentsType ct, const QStyleOption* opt, const QSize& contentsSize, const QWidget* widget) const
{
    switch(ct) {
        case CT_PushButton:
            return QCommonStyle::sizeFromContents(ct, opt, contentsSize, widget) + QSize(4, 5);
        default:
            return QCommonStyle::sizeFromContents(ct, opt, contentsSize, widget);
    }
}


