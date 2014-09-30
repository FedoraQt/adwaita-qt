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

#ifndef ADWAITA_H
#define ADWAITA_H

#include <QPalette>
#include <QCommonStyle>
#include <QApplication>
#include <QFile>
#include <QDebug>

class Adwaita : public QCommonStyle
{
    Q_OBJECT

public:
    Adwaita();

    void polish(QPalette &palette);
    void polish(QWidget *widget);
    void polish(QApplication* app);
    void unpolish(QWidget *widget);
    void unpolish(QApplication* app);
    int pixelMetric(PixelMetric metric, const QStyleOption *option,
                    const QWidget *widget) const;
    int styleHint(StyleHint hint, const QStyleOption *option,
                  const QWidget *widget, QStyleHintReturn *returnData) const;
    void drawPrimitive(PrimitiveElement element, const QStyleOption *option,
                       QPainter *painter, const QWidget *widget) const;
    void drawControl(ControlElement control, const QStyleOption *option,
                     QPainter *painter, const QWidget *widget) const;
    void drawItemPixmap(QPainter* painter, const QRect& rect, int alignment,
                        const QPixmap& pixmap) const;
    void drawItemText(QPainter* painter, const QRect& rect, int alignment,
                      const QPalette& pal, bool enabled, const QString& text,
                      QPalette::ColorRole textRole) const;
    void drawComplexControl(ComplexControl control,
                            const QStyleOptionComplex* option,
                            QPainter* painter, const QWidget* widget) const;
private:
    QString m_styleSheet;
};

#endif // ADWAITA_H