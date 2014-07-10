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

#include "adwaita.h"


void Adwaita::polish(QPalette &palette)
{
    // All, used especially for active elements in a focused window
#ifdef ACTUAL_COLORS
    palette.setColor(QPalette::All,      QPalette::Window,          QColor("#ededed"));
    palette.setColor(QPalette::All,      QPalette::WindowText,      QColor("#2e3436"));
    palette.setColor(QPalette::All,      QPalette::Base,            QColor("white"));
    palette.setColor(QPalette::All,      QPalette::AlternateBase,   QColor("#2e3436"));
    palette.setColor(QPalette::All,      QPalette::ToolTipBase,     QColor("#060606"));
    palette.setColor(QPalette::All,      QPalette::ToolTipText,     QColor("white"));
    palette.setColor(QPalette::All,      QPalette::Text,            QColor("#2e3436"));
    palette.setColor(QPalette::All,      QPalette::Button,          QColor("#2e3436"));
    palette.setColor(QPalette::All,      QPalette::ButtonText,      QColor("#2e3436"));
    palette.setColor(QPalette::All,      QPalette::BrightText,      QColor("white"));

    palette.setColor(QPalette::All,      QPalette::Light,           QColor("#ff1234"));
    palette.setColor(QPalette::All,      QPalette::Midlight,        QColor("#ff1234"));
    palette.setColor(QPalette::All,      QPalette::Dark,            QColor("#ff1234"));
    palette.setColor(QPalette::All,      QPalette::Mid,             QColor("#ff1234"));
    palette.setColor(QPalette::All,      QPalette::Shadow,          QColor("#00000059"));

    palette.setColor(QPalette::All,      QPalette::Highlight,       QColor("#4a90d9"));
    palette.setColor(QPalette::All,      QPalette::HighlightedText, QColor("white"));

    palette.setColor(QPalette::All,      QPalette::Link,            QColor("#2a76c6"));
    palette.setColor(QPalette::All,      QPalette::LinkVisited,     QColor("#2a76c6"));

    // Exceptions for disabled elements in a focused window
    palette.setColor(QPalette::Disabled, QPalette::Window,          QColor("#f4f4f4"));
    palette.setColor(QPalette::Disabled, QPalette::WindowText,      QColor("#8d9091"));
    palette.setColor(QPalette::Disabled, QPalette::Base,            QColor("#ff1234"));
    palette.setColor(QPalette::Disabled, QPalette::AlternateBase,   QColor("#ff1234"));
//     palette.setColor(QPalette::Disabled, QPalette::ToolTipBase,     QColor("#ff1234"));
//     palette.setColor(QPalette::Disabled, QPalette::ToolTipText,     QColor("#ff1234"));
    palette.setColor(QPalette::Disabled, QPalette::Text,            QColor("#ff1234"));
    palette.setColor(QPalette::Disabled, QPalette::Button,          QColor("#ff1234"));
    palette.setColor(QPalette::Disabled, QPalette::ButtonText,      QColor("#ff1234"));
    palette.setColor(QPalette::Disabled, QPalette::BrightText,      QColor("#ff1234"));

    palette.setColor(QPalette::Disabled, QPalette::Light,           QColor("#ff1234"));
    palette.setColor(QPalette::Disabled, QPalette::Midlight,        QColor("#ff1234"));
    palette.setColor(QPalette::Disabled, QPalette::Dark,            QColor("#ff1234"));
    palette.setColor(QPalette::Disabled, QPalette::Mid,             QColor("#ff1234"));
    palette.setColor(QPalette::Disabled, QPalette::Shadow,          QColor("#ff1234"));

    palette.setColor(QPalette::Disabled, QPalette::Highlight,       QColor("#ff1234"));
    palette.setColor(QPalette::Disabled, QPalette::HighlightedText, QColor("#ff1234"));

    palette.setColor(QPalette::Disabled, QPalette::Link,            QColor("#ff1234"));
    palette.setColor(QPalette::Disabled, QPalette::LinkVisited,     QColor("#ff1234"));


    // Exceptions for an unfocused window
    palette.setColor(QPalette::Inactive, QPalette::Window,          QColor("#ededed"));
    palette.setColor(QPalette::Inactive, QPalette::WindowText,      QColor("#454f52"));
    palette.setColor(QPalette::Inactive, QPalette::Base,            QColor("#fcfcfc"));
    palette.setColor(QPalette::Inactive, QPalette::AlternateBase,   QColor("#ff1234"));
//     palette.setColor(QPalette::Inactive, QPalette::ToolTipBase,     QColor("#ff1234"));
//     palette.setColor(QPalette::Inactive, QPalette::ToolTipText,     QColor("#ff1234"));
    palette.setColor(QPalette::Inactive, QPalette::Text,            QColor("#54595a"));
    palette.setColor(QPalette::Inactive, QPalette::Button,          QColor("#ff1234"));
    palette.setColor(QPalette::Inactive, QPalette::ButtonText,      QColor("#ff1234"));
    palette.setColor(QPalette::Inactive, QPalette::BrightText,      QColor("#ff1234"));

    palette.setColor(QPalette::Inactive, QPalette::Light,           QColor("#ff1234"));
    palette.setColor(QPalette::Inactive, QPalette::Midlight,        QColor("#ff1234"));
    palette.setColor(QPalette::Inactive, QPalette::Dark,            QColor("#ff1234"));
    palette.setColor(QPalette::Inactive, QPalette::Mid,             QColor("#ff1234"));
    palette.setColor(QPalette::Inactive, QPalette::Shadow,          QColor("#ff1234"));

    palette.setColor(QPalette::Inactive, QPalette::Highlight,       QColor("#ff1234"));
    palette.setColor(QPalette::Inactive, QPalette::HighlightedText, QColor("#ff1234"));

    palette.setColor(QPalette::Inactive, QPalette::Link,            QColor("#4a90d9"));
    palette.setColor(QPalette::Inactive, QPalette::LinkVisited,     QColor("#4a90d9"));
#endif
    palette.setColor(QPalette::All,      QPalette::Window,          QColor("#ff1234"));
    palette.setColor(QPalette::All,      QPalette::WindowText,      QColor("#2e3436"));
    palette.setColor(QPalette::All,      QPalette::Base,            QColor("#ff1234"));
    palette.setColor(QPalette::All,      QPalette::AlternateBase,   QColor("#ff1234"));
    palette.setColor(QPalette::All,      QPalette::ToolTipBase,     QColor("#ff1234"));
    palette.setColor(QPalette::All,      QPalette::ToolTipText,     QColor("#ff1234"));
    palette.setColor(QPalette::All,      QPalette::Text,            QColor("#2e3436"));
    palette.setColor(QPalette::All,      QPalette::Button,          QColor("#ff1234"));
    palette.setColor(QPalette::All,      QPalette::ButtonText,      QColor("#2e3436"));
    palette.setColor(QPalette::All,      QPalette::BrightText,      QColor("#2e3436"));

    palette.setColor(QPalette::All,      QPalette::Light,           QColor("#ff1234"));
    palette.setColor(QPalette::All,      QPalette::Midlight,        QColor("#ff1234"));
    palette.setColor(QPalette::All,      QPalette::Dark,            QColor("#ff1234"));
    palette.setColor(QPalette::All,      QPalette::Mid,             QColor("#ff1234"));
    palette.setColor(QPalette::All,      QPalette::Shadow,          QColor("#ff1234"));

    palette.setColor(QPalette::All,      QPalette::Highlight,       QColor("#ff1234"));
    palette.setColor(QPalette::All,      QPalette::HighlightedText, QColor("#ffffff"));

    palette.setColor(QPalette::All,      QPalette::Link,            QColor("#ff1234"));
    palette.setColor(QPalette::All,      QPalette::LinkVisited,     QColor("#ff1234"));

    // Exceptions for disabled elements in a focused window
    palette.setColor(QPalette::Disabled, QPalette::Window,          QColor("#dd8888"));
    palette.setColor(QPalette::Disabled, QPalette::WindowText,      QColor("#8d9091"));
    palette.setColor(QPalette::Disabled, QPalette::Base,            QColor("#dd8888"));
    palette.setColor(QPalette::Disabled, QPalette::AlternateBase,   QColor("#dd8888"));
    palette.setColor(QPalette::Disabled, QPalette::Text,            QColor("#8d9091"));
    palette.setColor(QPalette::Disabled, QPalette::Button,          QColor("#dd8888"));
    palette.setColor(QPalette::Disabled, QPalette::ButtonText,      QColor("#8d9091"));
    palette.setColor(QPalette::Disabled, QPalette::BrightText,      QColor("#8d9091"));

    palette.setColor(QPalette::Disabled, QPalette::Light,           QColor("#dd8888"));
    palette.setColor(QPalette::Disabled, QPalette::Midlight,        QColor("#dd8888"));
    palette.setColor(QPalette::Disabled, QPalette::Dark,            QColor("#dd8888"));
    palette.setColor(QPalette::Disabled, QPalette::Mid,             QColor("#dd8888"));
    palette.setColor(QPalette::Disabled, QPalette::Shadow,          QColor("#dd8888"));

    palette.setColor(QPalette::Disabled, QPalette::Highlight,       QColor("#dd8888"));
    palette.setColor(QPalette::Disabled, QPalette::HighlightedText, QColor("#dd8888"));

    palette.setColor(QPalette::Disabled, QPalette::Link,            QColor("#dd8888"));
    palette.setColor(QPalette::Disabled, QPalette::LinkVisited,     QColor("#dd8888"));


    // Exceptions for an unfocused window
    palette.setColor(QPalette::Inactive, QPalette::Window,          QColor("#aa0212"));
    palette.setColor(QPalette::Inactive, QPalette::WindowText,      QColor("#54595a"));
    palette.setColor(QPalette::Inactive, QPalette::Base,            QColor("#aa0212"));
    palette.setColor(QPalette::Inactive, QPalette::AlternateBase,   QColor("#aa0212"));
    palette.setColor(QPalette::Inactive, QPalette::Text,            QColor("#54595a"));
    palette.setColor(QPalette::Inactive, QPalette::Button,          QColor("#aa0212"));
    palette.setColor(QPalette::Inactive, QPalette::ButtonText,      QColor("#54595a"));
    palette.setColor(QPalette::Inactive, QPalette::BrightText,      QColor("#54595a"));

    palette.setColor(QPalette::Inactive, QPalette::Light,           QColor("#aa0212"));
    palette.setColor(QPalette::Inactive, QPalette::Midlight,        QColor("#aa0212"));
    palette.setColor(QPalette::Inactive, QPalette::Dark,            QColor("#aa0212"));
    palette.setColor(QPalette::Inactive, QPalette::Mid,             QColor("#aa0212"));
    palette.setColor(QPalette::Inactive, QPalette::Shadow,          QColor("#aa0212"));

    palette.setColor(QPalette::Inactive, QPalette::Highlight,       QColor("#aa0212"));
    palette.setColor(QPalette::Inactive, QPalette::HighlightedText, QColor("#aa0212"));

    palette.setColor(QPalette::Inactive, QPalette::Link,            QColor("#aa0212"));
    palette.setColor(QPalette::Inactive, QPalette::LinkVisited,     QColor("#aa0212"));

}


void Adwaita::polish(QWidget *widget)
{
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


int Adwaita::pixelMetric(PixelMetric metric, const QStyleOption *option, const QWidget *widget) const
{
    return QCommonStyle::pixelMetric(metric, option, widget);
}

int Adwaita::styleHint(StyleHint hint, const QStyleOption *option, const QWidget *widget,
                       QStyleHintReturn *returnData) const
{
    return QCommonStyle::styleHint(hint, option, widget, returnData);
}

void Adwaita::drawPrimitive(PrimitiveElement element, const QStyleOption *option, QPainter *painter,
                            const QWidget *widget) const
{
    QCommonStyle::drawPrimitive(element, option, painter, widget);
}



void Adwaita::drawControl(ControlElement element, const QStyleOption *option, QPainter *painter,
                          const QWidget *widget) const
{
    QCommonStyle::drawControl(element, option, painter, widget);
}

void Adwaita::drawComplexControl(QStyle::ComplexControl control, const QStyleOptionComplex* option,
                                 QPainter* painter, const QWidget* widget) const
{
    QCommonStyle::drawComplexControl(control, option, painter, widget);
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

