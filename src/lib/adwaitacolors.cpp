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

#include "adwaitacolors.h"
#include "animations/adwaitaanimationdata.h"

#include <QGuiApplication>

#include <cmath>

namespace Adwaita
{

//* contrast for arrow and treeline rendering
static const qreal arrowShade = 0.15;

QColor Colors::alphaColor(QColor color, qreal alpha)
{
    if (alpha >= 0 && alpha < 1.0) {
        color.setAlphaF(alpha * color.alphaF());
    }
    return color;
}

QColor Colors::darken(const QColor &color, qreal amount)
{
    qreal h, s, l, a;
    color.getHslF(&h, &s, &l, &a);

    qreal lightness = l - amount;
    if (lightness < 0) {
        lightness = 0;
    }

    return QColor::fromHslF(h, s, lightness, a);
}

QColor Colors::desaturate(const QColor &color, qreal amount)
{
    qreal h, s, l, a;
    color.getHslF(&h, &s, &l, &a);

    qreal saturation = s - amount;
    if (saturation < 0) {
        saturation = 0;
    }
    return QColor::fromHslF(h, saturation, l, a);
}

QColor Colors::lighten(const QColor &color, qreal amount)
{
    qreal h, s, l, a;
    color.getHslF(&h, &s, &l, &a);

    qreal lightness = l + amount;
    if (lightness > 1) {
        lightness = 1;
    }
    return QColor::fromHslF(h, s, lightness, a);
}

QColor Colors::mix(const QColor &c1, const QColor &c2, qreal bias)
{
    auto mixQreal = [](qreal a, qreal b, qreal bias) {
        return a + (b - a) * bias;
    };

    if (bias <= 0.0) {
        return c1;
    }

    if (bias >= 1.0) {
        return c2;
    }

    if (std::isnan(bias)) {
        return c1;

    }

    qreal r = mixQreal(c1.redF(), c2.redF(), bias);
    qreal g = mixQreal(c1.greenF(), c2.greenF(), bias);
    qreal b = mixQreal(c1.blueF(), c2.blueF(), bias);
    qreal a = mixQreal(c1.alphaF(), c2.alphaF(), bias);

    return QColor::fromRgbF(r, g, b, a);
}

QColor Colors::transparentize(const QColor &color, qreal amount)
{
    qreal h, s, l, a;
    color.getHslF(&h, &s, &l, &a);

    qreal alpha = a - amount;
    if (alpha < 0) {
        alpha = 0;
    }

    return QColor::fromHslF(h, s, l, alpha);
}

static bool isDarkMode()
{
    const QColor textColor = QGuiApplication::palette().color(QPalette::Text);
    if ((textColor.redF() * 0.299 + textColor.greenF() * 0.587 + textColor.blueF() * 0.114) <= 186) {
        return true;
    }

    return false;
}

static QPalette paletteAdwaita()
{
    QPalette palette;

    // Colors defined in GTK adwaita style in _colors.scss
    QColor base_color = QColor("white");
    QColor text_color = QColor("black");
    QColor bg_color = QColor("#f6f5f4");
    QColor fg_color = QColor("#2e3436");
    QColor selected_bg_color = QColor("#3584e4");
    QColor selected_fg_color = QColor("white");
    QColor osd_text_color = QColor("white");
    QColor osd_bg_color = QColor("black");
    QColor shadow = Colors::transparentize(QColor("black"), 0.9);

    QColor backdrop_fg_color = Colors::mix(fg_color, bg_color);
    QColor backdrop_base_color = Colors::darken(base_color, 0.01);
    QColor backdrop_selected_fg_color = backdrop_base_color;

    // This is the color we use as initial color for the gradient in normal state
    // Defined in _drawing.scss button(normal)
    QColor button_base_color = Colors::darken(bg_color, 0.04);

    QColor link_color = Colors::darken(selected_bg_color, 0.1);
    QColor link_visited_color = Colors::darken(selected_bg_color, 0.2);

    palette.setColor(QPalette::All,      QPalette::Window,          bg_color);
    palette.setColor(QPalette::All,      QPalette::WindowText,      fg_color);
    palette.setColor(QPalette::All,      QPalette::Base,            base_color);
    palette.setColor(QPalette::All,      QPalette::AlternateBase,   base_color);
    palette.setColor(QPalette::All,      QPalette::ToolTipBase,     osd_bg_color);
    palette.setColor(QPalette::All,      QPalette::ToolTipText,     osd_text_color);
    palette.setColor(QPalette::All,      QPalette::Text,            fg_color);
    palette.setColor(QPalette::All,      QPalette::Button,          button_base_color);
    palette.setColor(QPalette::All,      QPalette::ButtonText,      fg_color);
    palette.setColor(QPalette::All,      QPalette::BrightText,      text_color);

    palette.setColor(QPalette::All,      QPalette::Light,           Colors::lighten(button_base_color));
    palette.setColor(QPalette::All,      QPalette::Midlight,        Colors::mix(Colors::lighten(button_base_color), button_base_color));
    palette.setColor(QPalette::All,      QPalette::Mid,             Colors::mix(Colors::darken(button_base_color), button_base_color));
    palette.setColor(QPalette::All,      QPalette::Dark,            Colors::darken(button_base_color));
    palette.setColor(QPalette::All,      QPalette::Shadow,          shadow);

    palette.setColor(QPalette::All,      QPalette::Highlight,       selected_bg_color);
    palette.setColor(QPalette::All,      QPalette::HighlightedText, selected_fg_color);

    palette.setColor(QPalette::All,      QPalette::Link,            link_color);
    palette.setColor(QPalette::All,      QPalette::LinkVisited,     link_visited_color);

    QColor insensitive_fg_color = Colors::mix(fg_color, bg_color);
    QColor insensitive_bg_color = Colors::mix(bg_color, base_color, 0.4);

    palette.setColor(QPalette::Disabled, QPalette::Window,          insensitive_bg_color);
    palette.setColor(QPalette::Disabled, QPalette::WindowText,      insensitive_fg_color);
    palette.setColor(QPalette::Disabled, QPalette::Base,            base_color);
    palette.setColor(QPalette::Disabled, QPalette::AlternateBase,   base_color);
    palette.setColor(QPalette::Disabled, QPalette::Text,            insensitive_fg_color);
    palette.setColor(QPalette::Disabled, QPalette::Button,          insensitive_bg_color);
    palette.setColor(QPalette::Disabled, QPalette::ButtonText,      insensitive_fg_color);
    palette.setColor(QPalette::Disabled, QPalette::BrightText,      text_color);

    palette.setColor(QPalette::Disabled, QPalette::Light,           Colors::lighten(insensitive_bg_color));
    palette.setColor(QPalette::Disabled, QPalette::Midlight,        Colors::mix(Colors::lighten(insensitive_bg_color), insensitive_bg_color));
    palette.setColor(QPalette::Disabled, QPalette::Mid,             Colors::mix(Colors::darken(insensitive_bg_color), insensitive_bg_color));
    palette.setColor(QPalette::Disabled, QPalette::Dark,            Colors::darken(insensitive_bg_color));
    palette.setColor(QPalette::Disabled, QPalette::Shadow,          shadow);

    palette.setColor(QPalette::Disabled, QPalette::Highlight,       selected_bg_color);
    palette.setColor(QPalette::Disabled, QPalette::HighlightedText, selected_fg_color);

    palette.setColor(QPalette::Disabled, QPalette::Link,            link_color);
    palette.setColor(QPalette::Disabled, QPalette::LinkVisited,     link_visited_color);


    palette.setColor(QPalette::Inactive, QPalette::Window,          bg_color);
    palette.setColor(QPalette::Inactive, QPalette::WindowText,      backdrop_fg_color);
    palette.setColor(QPalette::Inactive, QPalette::Base,            backdrop_base_color);
    palette.setColor(QPalette::Inactive, QPalette::AlternateBase,   backdrop_base_color);
    palette.setColor(QPalette::Inactive, QPalette::Text,            backdrop_fg_color);
    palette.setColor(QPalette::Inactive, QPalette::Button,          button_base_color);
    palette.setColor(QPalette::Inactive, QPalette::ButtonText,      backdrop_fg_color);
    palette.setColor(QPalette::Inactive, QPalette::BrightText,      text_color);

    palette.setColor(QPalette::Inactive, QPalette::Light,           Colors::lighten(insensitive_bg_color));
    palette.setColor(QPalette::Inactive, QPalette::Midlight,        Colors::mix(Colors::lighten(insensitive_bg_color), insensitive_bg_color));
    palette.setColor(QPalette::Inactive, QPalette::Mid,             Colors::mix(Colors::darken(insensitive_bg_color), insensitive_bg_color));
    palette.setColor(QPalette::Inactive, QPalette::Dark,            Colors::darken(insensitive_bg_color));
    palette.setColor(QPalette::Inactive, QPalette::Shadow,          shadow);

    palette.setColor(QPalette::Inactive, QPalette::Highlight,       selected_bg_color);
    palette.setColor(QPalette::Inactive, QPalette::HighlightedText, backdrop_selected_fg_color);

    palette.setColor(QPalette::Inactive, QPalette::Link,            link_color);
    palette.setColor(QPalette::Inactive, QPalette::LinkVisited,     link_visited_color);

    return palette;
}

// private
static QPalette paletteAdwaitaDark()
{
    QPalette palette;

    // Colors defined in GTK adwaita style in _colors.scss
    QColor base_color = Colors::lighten(Colors::desaturate(QColor("#241f31"), 1.0), 0.02);
    QColor text_color = QColor("white");
    QColor bg_color = Colors::darken(Colors::desaturate(QColor("#3d3846"), 1.0), 0.04);
    QColor fg_color = QColor("#eeeeec");
    QColor selected_bg_color = Colors::darken(QColor("#3584e4"), 0.2);
    QColor selected_fg_color = QColor("white");
    QColor osd_text_color = QColor("white");
    QColor osd_bg_color = QColor("black");
    QColor shadow = Colors::transparentize(QColor("black"), 0.9);

    QColor backdrop_fg_color = Colors::mix(fg_color, bg_color);
    QColor backdrop_base_color = Colors::lighten(base_color, 0.01);
    QColor backdrop_selected_fg_color = Colors::mix(text_color, backdrop_base_color, 0.2);

    // This is the color we use as initial color for the gradient in normal state
    // Defined in _drawing.scss button(normal)
    QColor button_base_color = Colors::darken(bg_color, 0.01);

    QColor link_color = Colors::lighten(selected_bg_color, 0.2);
    QColor link_visited_color = Colors::lighten(selected_bg_color, 0.1);

    palette.setColor(QPalette::All,      QPalette::Window,          bg_color);
    palette.setColor(QPalette::All,      QPalette::WindowText,      fg_color);
    palette.setColor(QPalette::All,      QPalette::Base,            base_color);
    palette.setColor(QPalette::All,      QPalette::AlternateBase,   base_color);
    palette.setColor(QPalette::All,      QPalette::ToolTipBase,     osd_bg_color);
    palette.setColor(QPalette::All,      QPalette::ToolTipText,     osd_text_color);
    palette.setColor(QPalette::All,      QPalette::Text,            fg_color);
    palette.setColor(QPalette::All,      QPalette::Button,          button_base_color);
    palette.setColor(QPalette::All,      QPalette::ButtonText,      fg_color);
    palette.setColor(QPalette::All,      QPalette::BrightText,      text_color);

    palette.setColor(QPalette::All,      QPalette::Light,           Colors::lighten(button_base_color));
    palette.setColor(QPalette::All,      QPalette::Midlight,        Colors::mix(Colors::lighten(button_base_color), button_base_color));
    palette.setColor(QPalette::All,      QPalette::Mid,             Colors::mix(Colors::darken(button_base_color), button_base_color));
    palette.setColor(QPalette::All,      QPalette::Dark,            Colors::darken(button_base_color));
    palette.setColor(QPalette::All,      QPalette::Shadow,          shadow);

    palette.setColor(QPalette::All,      QPalette::Highlight,       selected_bg_color);
    palette.setColor(QPalette::All,      QPalette::HighlightedText, selected_fg_color);

    palette.setColor(QPalette::All,      QPalette::Link,            link_color);
    palette.setColor(QPalette::All,      QPalette::LinkVisited,     link_visited_color);


    QColor insensitive_fg_color = Colors::mix(fg_color, bg_color);
    QColor insensitive_bg_color = Colors::mix(bg_color, base_color, 0.4);

    palette.setColor(QPalette::Disabled, QPalette::Window,          insensitive_bg_color);
    palette.setColor(QPalette::Disabled, QPalette::WindowText,      insensitive_fg_color);
    palette.setColor(QPalette::Disabled, QPalette::Base,            base_color);
    palette.setColor(QPalette::Disabled, QPalette::AlternateBase,   base_color);
    palette.setColor(QPalette::Disabled, QPalette::Text,            insensitive_fg_color);
    palette.setColor(QPalette::Disabled, QPalette::Button,          insensitive_bg_color);
    palette.setColor(QPalette::Disabled, QPalette::ButtonText,      insensitive_fg_color);
    palette.setColor(QPalette::Disabled, QPalette::BrightText,      text_color);

    palette.setColor(QPalette::Disabled, QPalette::Light,           Colors::lighten(insensitive_bg_color));
    palette.setColor(QPalette::Disabled, QPalette::Midlight,        Colors::mix(Colors::lighten(insensitive_bg_color), insensitive_bg_color));
    palette.setColor(QPalette::Disabled, QPalette::Mid,             Colors::mix(Colors::darken(insensitive_bg_color), insensitive_bg_color));
    palette.setColor(QPalette::Disabled, QPalette::Dark,            Colors::darken(insensitive_bg_color));
    palette.setColor(QPalette::Disabled, QPalette::Shadow,          shadow);

    palette.setColor(QPalette::Disabled, QPalette::Highlight,       selected_bg_color);
    palette.setColor(QPalette::Disabled, QPalette::HighlightedText, selected_fg_color);

    palette.setColor(QPalette::Disabled, QPalette::Link,            link_color);
    palette.setColor(QPalette::Disabled, QPalette::LinkVisited,     link_visited_color);


    palette.setColor(QPalette::Inactive, QPalette::Window,          bg_color);
    palette.setColor(QPalette::Inactive, QPalette::WindowText,      backdrop_fg_color);
    palette.setColor(QPalette::Inactive, QPalette::Base,            backdrop_base_color);
    palette.setColor(QPalette::Inactive, QPalette::AlternateBase,   backdrop_base_color);
    palette.setColor(QPalette::Inactive, QPalette::Text,            backdrop_fg_color);
    palette.setColor(QPalette::Inactive, QPalette::Button,          button_base_color);
    palette.setColor(QPalette::Inactive, QPalette::ButtonText,      backdrop_fg_color);
    palette.setColor(QPalette::Inactive, QPalette::BrightText,      text_color);

    palette.setColor(QPalette::Inactive, QPalette::Light,           Colors::lighten(insensitive_bg_color));
    palette.setColor(QPalette::Inactive, QPalette::Midlight,        Colors::mix(Colors::lighten(insensitive_bg_color), insensitive_bg_color));
    palette.setColor(QPalette::Inactive, QPalette::Mid,             Colors::mix(Colors::darken(insensitive_bg_color), insensitive_bg_color));
    palette.setColor(QPalette::Inactive, QPalette::Dark,            Colors::darken(insensitive_bg_color));
    palette.setColor(QPalette::Inactive, QPalette::Shadow,          shadow);

    palette.setColor(QPalette::Inactive, QPalette::Highlight,       selected_bg_color);
    palette.setColor(QPalette::Inactive, QPalette::HighlightedText, backdrop_selected_fg_color);

    palette.setColor(QPalette::Inactive, QPalette::Link,            link_color);
    palette.setColor(QPalette::Inactive, QPalette::LinkVisited,     link_visited_color);

    return palette;
}

// private
static QPalette paletteAdwaitaHighContrast()
{
    QPalette palette;

    return palette;
}

QPalette Colors::disabledPalette(const QPalette &source, qreal ratio)
{
    QPalette copy(source);

    const QList<QPalette::ColorRole> roles = { QPalette::Background, QPalette::Highlight, QPalette::WindowText, QPalette::ButtonText, QPalette::Text, QPalette::Button };
    foreach (const QPalette::ColorRole &role, roles) {
        copy.setColor(role, Colors::mix(source.color(QPalette::Active, role), source.color(QPalette::Disabled, role), 1.0 - ratio));
    }

    return copy;
}

QPalette Colors::palette(ColorVariant variant)
{
    if (variant == ColorVariant::Unknown) {
        return isDarkMode() ? paletteAdwaitaDark() : paletteAdwaita();
    } else if (variant == ColorVariant::Adwaita) {
        return paletteAdwaita();
    } else if (variant == ColorVariant::AdwaitaDark) {
        return paletteAdwaitaDark();
    } else {
        // TODO
        return paletteAdwaitaHighContrast();
    }
}

QColor Colors::hoverColor(const StyleOptions &options)
{
    return options.palette().highlight().color();
}

QColor Colors::focusColor(const StyleOptions &options)
{
    return options.palette().highlight().color();
}

QColor Colors::negativeText(const StyleOptions &options)
{
    Q_UNUSED(options)

    return Qt::red;
}

QColor Colors::shadowColor(const StyleOptions &options)
{
    return alphaColor(options.palette().color(QPalette::Shadow), 0.15);
}

QColor Colors::titleBarColor(const StyleOptions &options)
{
    return options.palette().color(options.active() ? QPalette::Active : QPalette::Inactive, QPalette::Window);
}

QColor Colors::titleBarTextColor(const StyleOptions &options)
{
    return options.palette().color(options.active() ? QPalette::Active : QPalette::Inactive, QPalette::WindowText);
}

QColor Colors::arrowOutlineColor(const StyleOptions &options)
{
    switch (options.colorRole()) {
    case QPalette::Text:
        return mix(options.palette().color(options.colorGroup(), QPalette::Text), options.palette().color(options.colorGroup(), QPalette::Base), arrowShade);
    case QPalette::WindowText:
        return mix(options.palette().color(options.colorGroup(), QPalette::WindowText), options.palette().color(options.colorGroup(), QPalette::Window), arrowShade);
    case QPalette::ButtonText:
        return mix(options.palette().color(options.colorGroup(), QPalette::ButtonText), options.palette().color(options.colorGroup(), QPalette::Button), arrowShade);
    default:
        return options.palette().text().color();
    }
}

QColor Colors::buttonOutlineColor(const StyleOptions &options)
{
    ColorVariant variant = options.colorVariant();

    if (variant == ColorVariant::Unknown) {
        variant = isDarkMode() ? ColorVariant::AdwaitaDark : ColorVariant::Adwaita;
    }

    if (variant == ColorVariant::AdwaitaDark) {
        return darken(options.palette().color(QPalette::Window), 0.1);
    } else {
        return darken(options.palette().color(QPalette::Window), 0.18);
    }
}

QColor Colors::indicatorOutlineColor(const StyleOptions &options)
{
    bool isDisabled = options.palette().currentColorGroup() == QPalette::Disabled;
    if (options.inMenu() || options.checkboxState() == CheckBoxState::CheckOff) {
        ColorVariant variant = options.colorVariant();

        if (variant == ColorVariant::Unknown) {
            variant = isDarkMode() ? ColorVariant::AdwaitaDark : ColorVariant::Adwaita;
        }

        if (isDisabled) {
            return buttonOutlineColor(options);
        }

        if (variant == ColorVariant::AdwaitaDark) {
            return darken(options.palette().color(QPalette::Window), 0.18);
        } else {
            return darken(options.palette().color(QPalette::Window), 0.24);
        }
    } else {
        return options.palette().color(QPalette::Highlight);
    }
}

QColor Colors::frameOutlineColor(const StyleOptions &options)
{
    return inputOutlineColor(options);
}

QColor Colors::inputOutlineColor(const StyleOptions &options)
{
    QColor outline(buttonOutlineColor(options));

    // focus takes precedence over hover
    if (options.animationMode() == AnimationFocus) {
        outline = mix(outline, focusColor(options));
    } else if (options.hasFocus()) {
        outline = focusColor(options);
    }

    return outline;
}

QColor Colors::sidePanelOutlineColor(const StyleOptions &options)
{
    QColor outline(options.palette().color(QPalette::Inactive, QPalette::Highlight));
    QColor focus(options.palette().color(QPalette::Active, QPalette::Highlight));

    if (options.animationMode() == AnimationFocus) {
        outline = mix(outline, focus, options.opacity());
    } else if (options.hasFocus()) {
        outline = focus;
    }

    return outline;
}

QColor Colors::sliderOutlineColor(const StyleOptions &options)
{
    QColor outline(mix(options.palette().color(QPalette::Window), options.palette().color(QPalette::Shadow), 0.5));

    // hover takes precedence over focus
    if (options.animationMode() == AnimationHover) {
        QColor hover(hoverColor(options));
        QColor focus(focusColor(options));
        if (options.hasFocus()) {
            outline = mix(focus, hover, options.opacity());
        } else {
            outline = mix(outline, hover, options.opacity());
        }
    } else if (options.mouseOver()) {
        outline = hoverColor(options);
    } else if (options.animationMode() == AnimationFocus) {
        QColor focus(focusColor(options));
        outline = mix(outline, focus, options.opacity());
    } else if (options.hasFocus()) {
        outline = focusColor(options);
    }

    return outline;
}

QColor Colors::buttonBackgroundColor(const StyleOptions &options)
{
    bool isDisabled = options.palette().currentColorGroup() == QPalette::Disabled;
    QColor buttonBackground(options.palette().color(QPalette::Button));
    QColor background(options.palette().color(QPalette::Window));

    ColorVariant variant = options.colorVariant();

    if (variant == ColorVariant::Unknown) {
        variant = isDarkMode() ? ColorVariant::AdwaitaDark : ColorVariant::Adwaita;
    }

    const bool darkMode = variant == ColorVariant::AdwaitaDark;
    const QPalette &palette = options.palette();

    if (isDisabled && (options.animationMode() == AnimationPressed || options.sunken())) {
        // Defined in drawing.css - insensitive-active button
        // if($variant == 'light', Colors::darken(Colors::mix($c, $base_color, 85%), 8%), Colors::darken(Colors::mix($c, $base_color, 85%), 6%));
        // FIXME: doesn't seem to be correct color
        return darkMode ? Colors::darken(Colors::mix(palette.color(QPalette::Active, QPalette::Window), palette.color(QPalette::Active, QPalette::Base), 0.15), 0.06) :
               Colors::darken(Colors::mix(palette.color(QPalette::Active, QPalette::Window), palette.color(QPalette::Active, QPalette::Base), 0.15), 0.08);
    }

    if (options.animationMode() == AnimationPressed) {
        if (darkMode) {
            // Active button for dark mode is Colors::darken(bg_color, 0.09)
            return Colors::mix(Colors::darken(background, 0.01), Colors::darken(background, 0.09), options.opacity());
        } else {
            // Active button for normal mode is Colors::darken(bg_color, 0.14)
            return Colors::mix(buttonBackground, Colors::darken(background, 0.14), options.opacity());
        }
    } else if (options.sunken()) {
        if (darkMode) {
            // Active button for dark mode is Colors::darken(bg_color, 0.09)
            return Colors::darken(background, 0.09);
        } else {
            // Active button for normal mode is Colors::darken(bg_color, 0.14)
            return Colors::darken(background, 0.14);
        }
    } else if (options.animationMode() == AnimationHover) {
        if (darkMode) {
            // Hovered button for dark mode is Colors::darken(bg_color, 0.01)
            return Colors::mix(buttonBackground, Colors::darken(background, 0.01), options.opacity());
        } else {
            // Hovered button for normal mode is bg_color
            return Colors::mix(buttonBackground, background, options.opacity());
        }
    } else if (options.mouseOver()) {
        if (darkMode) {
            // Hovered button for dark mode is Colors::darken(bg_color, 0.01)
            return Colors::darken(background, 0.01);
        } else {
            // Hovered button for normal mode is bg_color
            return background;
        }
    }

    return buttonBackground;
}

QColor Colors::checkBoxIndicatorColor(const StyleOptions &options)
{
    if (options.inMenu()) {
        return options.palette().color(QPalette::Text);
    } else {
        if (options.active()) {
            return options.palette().color(QPalette::HighlightedText);
        } else {
            return Colors::transparentize(options.palette().color(QPalette::ToolTipText), 0.2);
        }
    }
}

QColor Colors::headerTextColor(const StyleOptions &options)
{
    QColor col(options.palette().color(QPalette::WindowText));

    if (options.state() & QStyle::State_Enabled) {
        if (options.state() & QStyle::State_Sunken) {
            return Colors::alphaColor(col, 0.9);
        } else if (options.state() & QStyle::State_MouseOver) {
            return Colors::alphaColor(col, 0.7);
        }
    }
    return Colors::alphaColor(col, 0.5);
}

QColor Colors::indicatorBackgroundColor(const StyleOptions &options)
{
    ColorVariant variant = options.colorVariant();

    if (variant == ColorVariant::Unknown) {
        variant = isDarkMode() ? ColorVariant::AdwaitaDark : ColorVariant::Adwaita;
    }

    const QPalette &palette = options.palette();
    const bool darkMode = variant == ColorVariant::AdwaitaDark;

    bool isDisabled = palette.currentColorGroup() == QPalette::Disabled;
    QColor background(palette.color(QPalette::Window));
    // Normal-alt button for dark mode is Colors::darken(bg_color, 0.03)
    // Normal-alt button for normal mode is Colors::lighten(bg_color, 0.05)
    QColor indicatorColor(darkMode ? Colors::darken(background, 0.03) : Colors::lighten(background, 0.05));

    if (options.inMenu() || options.checkboxState() == CheckOff) {
        if (isDisabled) {
            // Defined in drawing.css - insensitive button
            // $insensitive_bg_color: Colors::mix($bg_color, $base_color, 60%);
            return Colors::mix(palette.color(QPalette::Active, QPalette::Window), palette.color(QPalette::Active, QPalette::Base), 0.6);
        }

        if (options.animationMode() == AnimationPressed) {
            if (darkMode) {
                // Active button for dark mode is Colors::darken(bg_color, 0.09)
                return Colors::mix(background, Colors::darken(background, 0.09), options.opacity());
            } else {
                // Active button for normal mode is Colors::darken(bg_color, 0.14)
                return Colors::mix(Colors::lighten(background, 0.0), Colors::darken(background, 0.14), options.opacity());
            }
        } else if (options.sunken()) {
            if (darkMode) {
                // Active button for dark mode is Colors::darken(bg_color, 0.09)
                return Colors::darken(background, 0.09);
            } else {
                // Active button for normal mode is Colors::darken(bg_color, 0.14)
                return Colors::darken(background, 0.14);
            }
        } else if (options.animationMode() == AnimationHover) {
            if (darkMode) {
                // Hovered-alt button for dark mode is bg_color
                return Colors::mix(indicatorColor, background, options.opacity());
            } else {
                // Hovered-alt button for normal mode is Colors::lighten(bg_color, 0.09)
                return Colors::mix(indicatorColor, Colors::lighten(background, 0.09), options.opacity());
            }
        } else if (options.mouseOver()) {
            if (darkMode) {
                // Hovered-alt button for dark mode is bg_color
                return background;
            } else {
                // Hovered-alt button for normal mode is Colors::lighten(bg_color, 0.09)
                return Colors::lighten(background, 0.09);
            }
        }
    } else {
        if (darkMode) {
            return Colors::lighten(palette.color(QPalette::Highlight));
        } else {
            return palette.color(QPalette::Highlight);
        }
    }

    return indicatorColor;
}

QColor Colors::frameBackgroundColor(const StyleOptions &options)
{
    return Colors::mix(options.palette().color(options.colorGroup(), QPalette::Window), options.palette().color(options.colorGroup(), QPalette::Base), 0.3);
}

QColor Colors::scrollBarHandleColor(const StyleOptions &options)
{
    ColorVariant variant = options.colorVariant();

    if (variant == ColorVariant::Unknown) {
        variant = isDarkMode() ? ColorVariant::AdwaitaDark : ColorVariant::Adwaita;
    }

    QColor fgColor = options.palette().color(QPalette::Text);
    QColor bgColor = options.palette().color(QPalette::Window);
    QColor selectedBgColor = options.palette().color(QPalette::Highlight);

    QColor color(Colors::mix(fgColor, bgColor, 0.4));
    QColor hoverColor(Colors::mix(fgColor, bgColor, 0.2));
    QColor activeColor(variant == ColorVariant::AdwaitaDark ? Colors::lighten(selectedBgColor, 0.1) : Colors::darken(selectedBgColor, 0.1));

    // hover takes precedence over focus
    if (options.animationMode() == AnimationPressed) {
        if (options.mouseOver()) {
            color = Colors::mix(hoverColor, activeColor, options.opacity());
        } else {
            color = Colors::mix(color, activeColor, options.opacity());
        }
    } else if (options.sunken()) {
        color = activeColor;
    } else if (options.animationMode() == AnimationHover) {
        color = Colors::mix(color, hoverColor, options.opacity());
    } else if (options.mouseOver()) {
        color = hoverColor;
    }

    return color;
}

QColor Colors::separatorColor(const StyleOptions &options)
{
    return buttonOutlineColor(options);
}

QColor Colors::toolButtonColor(const StyleOptions &options)
{
    if (options.sunken() || (options.animationMode() != AnimationNone && options.animationMode() != AnimationHover)) {
        return buttonBackgroundColor(options);
    }

    return Qt::transparent;
}

QColor Colors::tabBarColor(const StyleOptions &options)
{
    QColor background(Colors::mix(options.palette().window().color(), options.palette().shadow().color(), 0.15));
    if (!(options.state() & QStyle::State_Enabled)) {
        background = background.lighter(115);
    }

    if (!(options.state() & QStyle::State_Active)) {
        background = background.lighter(115);
    }
    return background;
}

} // namespace Adwaita
