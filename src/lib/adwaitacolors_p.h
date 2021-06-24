/*************************************************************************
 * Copyright (C) 2019-2021 Jan Grulich <jgrulich@redhat.com>             *
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

#ifndef ADWAITA_COLORS_P_H
#define ADWAITA_COLORS_P_H

#include "adwaita.h"

#include <QMap>

class QColor;

namespace Adwaita
{

class ColorsPrivate : public QObject
{
    Q_OBJECT
public:
    enum AdwaitaColor {
        invalid_value = 0,
        light_fg,
        dark_fg,
        light_bg,
        dark_bg,
        accent_color,
        accent_text,
        destructive_color,
        destructive_text,
        success_color,
        warning_color,
        error_color,
        base_color,
        text_color,
        bg_color,
        fg_color,
        accent_standalone_color,
        destructive_standalone_color,
        borders_color,
        alt_borders_color,
        borders_edge,
        link_color,
        link_visited_color,
        dark_fill,
        headerbar_bg_color,
        menu_color,
        view_hover_color,
        view_active_color,
        view_selected_color,
        view_selected_hover_color,
        view_selected_active_color,
        trough_color,
        trough_hover_color,
        trough_active_color,
        button_color,
        button_hover_color,
        button_active_color,
        button_checked_color,
        button_checked_hover_color,
        button_checked_active_color,
        fill_color,
        fill_text_color,
        fill_hover_color,
        fill_active_color,
        slider_color,
        slider_hover_color,
        scrollbar_bg_color,
        scrollbar_slider_color,
        scrollbar_slider_hover_color,
        scrollbar_slider_active_color,
        osd_fg_color,
        osd_text_color,
        osd_bg_color,
        osd_insensitive_bg_color,
        osd_insensitive_fg_color,
        osd_borders_color,
        sidebar_bg_color,
        tooltip_borders_color,
        shadow_color,
        drop_target_color,
        insensitive_fg_color,
        insensitive_bg_color,
        insensitive_borders_color,
        focus_border_color,
        alt_focus_border_color
    };
    Q_ENUM(AdwaitaColor);

    ColorsPrivate();
    virtual ~ColorsPrivate();

    QColor adwaitaColor(AdwaitaColor color, ColorVariant variant, const QColor &currentColor = QColor());

private:
    QMap<AdwaitaColor, QMap<ColorVariant, std::function<QColor (const QColor&)> > > m_colors;
};

} // namespace Adwaita

#endif // ADWAITA_COLORS_P_H


