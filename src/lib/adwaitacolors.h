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

#ifndef ADWAITA_COLORS_H
#define ADWAITA_COLORS_H

#include "adwaitaqt_export.h"

#include <QColor>
#include <QPalette>
#include <QScopedPointer>

namespace Adwaita
{

class ColorRequestOptionsPrivate;

class ADWAITAQT_EXPORT ColorRequestOptions
{
public:
    enum ColorVariant {
        Adwaita,
        AdwaitaDark,
        AdwaitaHighContrast
    };

    explicit ColorRequestOptions(const QPalette &palette);
    virtual ~ColorRequestOptions();

    QPalette palette() const;

    void setColorGroup(QPalette::ColorGroup group);
    QPalette::ColorGroup colorGroup() const;

    void setColorRole(QPalette::ColorRole role);
    QPalette::ColorRole colorRole() const;

    void setColorVariant(ColorVariant variant);
    ColorVariant colorVariant() const;

    void setActive(bool active);
    bool active() const;

private:
    Q_DECLARE_PRIVATE(ColorRequestOptions)

    const QScopedPointer<ColorRequestOptionsPrivate> d_ptr;
};

class ADWAITAQT_EXPORT Colors
{
public:
    // Color adjustments
    static QColor alphaColor(QColor color, qreal alpha);
    static QColor darken(const QColor &color, qreal amount = 0.1);
    static QColor desaturate(const QColor &color, qreal amount = 0.1);
    static QColor lighten(const QColor &color, qreal amount = 0.1);
    static QColor mix(const QColor &c1, const QColor &c2, qreal bias = 0.5);
    static QColor transparentize(const QColor &color, qreal amount = 0.1);

    // Color paletty for each Adwaita variant
    static QPalette palette(ColorRequestOptions::ColorVariant variant = ColorRequestOptions::Adwaita);

    // Generic colors
    static QColor hoverColor(const ColorRequestOptions &options);
    static QColor focusColor(const ColorRequestOptions &options);
    static QColor negativeText(const ColorRequestOptions &options);
    static QColor shadowColor(const ColorRequestOptions &options);
    static QColor titleBarColor(const ColorRequestOptions &options);
    static QColor titleBarTextColor(const ColorRequestOptions &options);

    // Outline colors
//     static QColor arrowOutlineColor(const ColorRequestOptions &options);
//                              const QPalette &palette,
//                              QPalette::ColorGroup,
//                              QPalette::ColorRole

//     static QColor arrowOutlineColor(const ColorRequestOptions &options);
//                             const QPalette &palette,
//                             QPalette::ColorRole role);

//     static QColor arrowOutlineColor(const ColorRequestOptions &options);
//                              const QPalette &palette,
//                              bool mouseOver,
//                              bool hasFocus, qreal opacity = AnimationData::OpacityInvalid,
//                              AnimationMode = AnimationNone);

//     static QColor buttonOutlineColor(const ColorRequestOptions &options);
//                                      const QPalette &palette,
//                                      bool mouseOver,
//                                      bool hasFocus,
//                                      qreal opacity = AnimationData::OpacityInvalid,
//                                      AnimationMode = AnimationNone,
//                                      bool darkMode = false);

//     static QColor indicatorOutlineColor(const ColorRequestOptions &options);
//                                         const QPalette &palette,
//                                         bool mouseOver = false,
//                                         bool hasFocus = false,
//                                         qreal opacity = AnimationData::OpacityInvalid,
//                                         AnimationMode = AnimationNone,
//                                         CheckBoxState state = CheckOff,
//                                         bool inMenu = false,
//                                         ColorVariant variant = Adwaita);

//     static QColor frameOutlineColor(const ColorRequestOptions &options);
//                                     const QPalette &palette,
//                                     bool mouseOver = false,
//                                     bool hasFocus = false,
//                                     qreal opacity = AnimationData::OpacityInvalid,
//                                     AnimationMode = AnimationNone,
//                                     ColorVariant variant = Adwaita);

//     static QColor inputOutlineColor(const ColorRequestOptions &options);
//                                     const QPalette &palette,
//                                     bool mouseOver = false,
//                                     bool hasFocus = false,
//                                     qreal opacity = AnimationData::OpacityInvalid,
//                                     AnimationMode = AnimationNone,
//                                     ColorVariant variant = Adwaita);

//     static QColor sidePanelOutlineColor(const ColorRequestOptions &options);
//                                         const QPalette &palette,
//                                         bool hasFocus = false,
//                                         qreal opacity = AnimationData::OpacityInvalid,
//                                         AnimationMode = AnimationNone);
//     static QColor sliderOutlineColor(const ColorRequestOptions &options);
//                                      const QPalette &palette,
//                                      bool mouseOver,
//                                      bool hasFocus,
//                                      qreal opacity = AnimationData::OpacityInvalid,
//                                      AnimationMode = AnimationNone);


//     // Background colors
//     QColor frameBackgroundColor(const QPalette &palette);
//
//     QColor frameBackgroundColor(const QPalette &palette, QPalette::ColorGroup);
//
//     //* arrow outline color
//     QColor arrowColor(const QPalette &palette, QPalette::ColorGroup, QPalette::ColorRole);
//
//     //* arrow outline color
//     QColor arrowColor(const QPalette &palette, QPalette::ColorRole role);
//
//     //* arrow outline color, using animations
//     QColor arrowColor(const QPalette &palette, bool mouseOver, bool hasFocus, qreal opacity = AnimationData::OpacityInvalid, AnimationMode = AnimationNone);
//
//     //* button outline color, using animations
//
//     //* button panel color, using animations
//     QColor buttonBackgroundColor(const QPalette &palette, bool mouseOver, bool hasFocus, bool sunken, qreal opacity = AnimationData::OpacityInvalid, AnimationMode = AnimationNone, bool darkMode = false);
//
//     //* checkbox/radiobutton color, using animations
//     QColor indicatorBackgroundColor(const QPalette &palette,
//                                     bool mouseOver,
//                                     bool hasFocus,
//                                     bool sunken,
//                                     qreal opacity = AnimationData::OpacityInvalid,
//                                     AnimationMode = AnimationNone,
//                                     CheckBoxState state = CheckOff,
//                                     bool darkMode = false,
//                                     bool inMenu = false);
//
//     //* tool button color
//     QColor toolButtonColor(const QPalette &palette, bool mouseOver, bool hasFocus, bool sunken, qreal opacity = AnimationData::OpacityInvalid, AnimationMode = AnimationNone);
//
//     //* slider outline color, using animations
//
//     //* scrollbar handle color, using animations
//     QColor scrollBarHandleColor(const QPalette &palette, bool mouseOver, bool hasFocus, bool sunken, qreal opacity = AnimationData::OpacityInvalid, AnimationMode = AnimationNone, bool darkMode = false);
//
//     //* checkbox indicator, using animations
//     QColor checkBoxIndicatorColor(const QPalette &palette, bool mouseOver, bool active, qreal opacity = AnimationData::OpacityInvalid, AnimationMode = AnimationNone, bool darkMode = false, bool inMenu = false);
//
//     //* separator color
//     QColor separatorColor(const QPalette &palette, bool darkMode = false);
//
//     //* TreeView header text color
//     QColor headerTextColor(const QPalette &palette, const QStyle::State state);
//
//     //* TabBar background color
//     QColor tabBarColor(const QPalette &palette, const QStyle::State state);
//
//     //* merge active and inactive palettes based on ratio, for smooth enable state change transition
//     QPalette disabledPalette(const QPalette &palette, qreal ratio);
};

} // namespace Adwaita

#endif // ADWAITA_COLORS_H

