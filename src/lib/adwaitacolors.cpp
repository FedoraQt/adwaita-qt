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

#include <cmath>

namespace Adwaita
{

class ColorRequestOptionsPrivate
{
public:
    explicit ColorRequestOptionsPrivate(const QPalette &palette)
        : m_palette(palette)
    { }
    virtual ~ColorRequestOptionsPrivate()
    { }

    QPalette m_palette;
    QPalette::ColorGroup m_colorGroup = QPalette::ColorGroup::Normal;
    QPalette::ColorRole m_colorRole = QPalette::ColorRole::Base;
    ColorRequestOptions::ColorVariant m_colorVariant = ColorRequestOptions::Adwaita;
    bool m_active;
};

ColorRequestOptions::ColorRequestOptions(const QPalette &palette)
    : d_ptr(new ColorRequestOptionsPrivate(palette))
{
}

ColorRequestOptions::~ColorRequestOptions()
{
}

QPalette ColorRequestOptions::palette() const
{
    Q_D(const ColorRequestOptions);

    return d->m_palette;
}

void ColorRequestOptions::setColorGroup(QPalette::ColorGroup group)
{
    Q_D(ColorRequestOptions);

    d->m_colorGroup = group;
}

QPalette::ColorGroup ColorRequestOptions::colorGroup() const
{
    Q_D(const ColorRequestOptions);

    return d->m_colorGroup;
}

void ColorRequestOptions::setColorRole(QPalette::ColorRole role)
{
    Q_D(ColorRequestOptions);

    d->m_colorRole = role;
}

QPalette::ColorRole ColorRequestOptions::colorRole() const
{
    Q_D(const ColorRequestOptions);

    return d->m_colorRole;
}

void ColorRequestOptions::setColorVariant(ColorRequestOptions::ColorVariant variant)
{
    Q_D(ColorRequestOptions);

    d->m_colorVariant = variant;
}

ColorRequestOptions::ColorVariant ColorRequestOptions::colorVariant() const
{
    Q_D(const ColorRequestOptions);

    return d->m_colorVariant;
}

void ColorRequestOptions::setActive(bool active)
{
    Q_D(ColorRequestOptions);

    d->m_active = active;
}

bool ColorRequestOptions::active() const
{
    Q_D(const ColorRequestOptions);

    return d->m_active;
}

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

QPalette Colors::palette(ColorRequestOptions::ColorVariant variant)
{
    if (variant == ColorRequestOptions::Adwaita) {
        return paletteAdwaita();
    } else if (variant == ColorRequestOptions::AdwaitaDark) {
        return paletteAdwaitaDark();
    } else {
        // TODO
        return paletteAdwaitaHighContrast();
    }
}

QColor Colors::hoverColor(const ColorRequestOptions &options)
{
    return options.palette().highlight().color();
}

QColor Colors::focusColor(const ColorRequestOptions &options)
{
    return options.palette().highlight().color();
}

QColor Colors::negativeText(const ColorRequestOptions &options)
{
    Q_UNUSED(options);

    return Qt::red;
}

QColor Colors::shadowColor(const ColorRequestOptions &options)
{
    return alphaColor(options.palette().color(QPalette::Shadow), 0.15);
}

QColor Colors::titleBarColor(const ColorRequestOptions &options)
{
    return options.palette().color(options.active() ? QPalette::Active : QPalette::Inactive, QPalette::Window);
}

QColor Colors::titleBarTextColor(const ColorRequestOptions &options)
{
    return options.palette().color(options.active() ? QPalette::Active : QPalette::Inactive, QPalette::WindowText);
}

} // namespace Adwaita
