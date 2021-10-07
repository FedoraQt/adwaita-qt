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

#include <QHash>

class QColor;

namespace Adwaita
{

class ColorsPrivate : public QObject
{
    Q_OBJECT
public:
    enum AdwaitaColor {
        invalid_color= 0,
        base_color,
        text_color,
        bg_color,
        fg_color,
        selected_fg_color,
        selected_bg_color,
        selected_borders_color,
        borders_color,
        alt_borders_color,
        borders_edge,
        link_color,
        link_visited_color,
        top_hilight,
        dark_fill,
        headerbar_bg_color,
        menu_color,
        menu_selected_color,
        scrollbar_bg_color,
        scrollbar_slider_color,
        scrollbar_slider_hover_color,
        scrollbar_slider_active_color,
        warning_color,
        error_color,
        success_color,
        destructive_color,
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
        backdrop_base_color,
        backdrop_text_color,
        backdrop_bg_color,
        backdrop_fg_color,
        backdrop_insensitive_color,
        backdrop_selected_fg_color,
        backdrop_borders_color,
        backdrop_dark_fill,
        suggested_bg_color,
        suggested_border_color,
        progress_bg_color,
        progress_border_color,
        checkradio_bg_color,
        checkradio_fg_color,
        checkradio_borders_color,
        switch_bg_color,
        switch_borders_color,
        focus_border_color,
        alt_focus_border_color
    };
    Q_ENUM(AdwaitaColor);

    ColorsPrivate();
    virtual ~ColorsPrivate();

    QColor adwaitaColor(AdwaitaColor color, ColorVariant variant);
    QColor adwaitaWidgetColor(const QString &color, ColorVariant variant);
    static bool isDarkMode();

private:
    QHash<AdwaitaColor, QHash<ColorVariant, QColor> > m_colors;
    QHash<QString, QMap<ColorVariant, QColor> > m_widgetColors;
};

} // namespace Adwaita

#endif // ADWAITA_COLORS_P_H


