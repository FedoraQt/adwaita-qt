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
    palette.setColor(QPalette::All,      QPalette::Button,          QColor("#2e3436"));
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
    palette.setColor(QPalette::Disabled, QPalette::Base,            QColor("#ff1234"));
    palette.setColor(QPalette::Disabled, QPalette::AlternateBase,   QColor("#ff1234"));
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

