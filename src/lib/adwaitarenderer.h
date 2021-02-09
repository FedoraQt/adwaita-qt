/*************************************************************************
 * Copyright (C) 2014 by Hugo Pereira Da Costa <hugo.pereira@free.fr>    *
 * Copyright (C) 2014-2018 Martin Bříza <m@rtinbriza.cz>                 *
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

#ifndef ADWAITA_RENDERER_H
#define ADWAITA_RENDERER_H

#include "adwaita.h"
#include "adwaitaqt_export.h"

namespace Adwaita
{

class ADWAITAQT_EXPORT Renderer
{
public:
    static void renderDebugFrame(const StyleOptions &options);

    static void renderFocusRect(const StyleOptions &options, Sides sides = SideNone);

    static void renderFocusLine(const StyleOptions &options);

    static void renderFrame(const StyleOptions &options);

    static void renderSquareFrame(const StyleOptions &options);

    static void renderFlatFrame(const StyleOptions &options);

    static void renderSidePanelFrame(const StyleOptions &options, Side side);

    static void renderMenuFrame(const StyleOptions &options, bool roundCorners = true);

    static void renderButtonFrame(const StyleOptions &options);

    static void renderCheckBoxFrame(const StyleOptions &options);

    static void renderFlatButtonFrame(const StyleOptions &options);

    static void renderFlatRoundedButtonFrame(const StyleOptions &options);

    static void renderToolButtonFrame(const StyleOptions &options);

    static void renderToolBoxFrame(const StyleOptions &options, int tabWidth);

    static void renderTabWidgetFrame(const StyleOptions &options, Corners corners);

    static void renderSelection(const StyleOptions &options);

    static void renderSeparator(const StyleOptions &options, bool vertical = false);

    static void renderCheckBoxBackground(const StyleOptions &options);

    static void renderCheckBox(const StyleOptions &options, const QColor &tickColor, qreal animation = -1);

    static void renderRadioButtonBackground(const StyleOptions &options);

    static void renderRadioButton(const StyleOptions &options, const QColor &tickColor, qreal animation = -1);

    static void renderSliderGroove(const StyleOptions &options);

    static void renderSliderHandle(const StyleOptions &options, Side ticks, qreal angle = 0.0);

    static void renderDialGroove(const StyleOptions &options);

    static void renderDialContents(const StyleOptions &options, qreal first, qreal second);

    static void renderProgressBarGroove(const StyleOptions &options);

    static void renderProgressBarContents(const StyleOptions &options);

    static void renderProgressBarBusyContents(const StyleOptions &options, bool horizontal, bool reverse, int progress);

    static void renderScrollBarGroove(const StyleOptions &options);

    static void renderScrollBarHandle(const StyleOptions &options);

    static void renderToolBarHandle(const StyleOptions &options);

    static void renderTabBarTab(const StyleOptions &options, const QColor &background, Corners corners, bool renderFrame);

    static void renderArrow(const StyleOptions &options, ArrowOrientation arrowOrientation);

    static void renderSign(const StyleOptions &options, bool orientation);

    static void renderDecorationButton(const StyleOptions &options, ButtonType buttonType);
};

} // namespace Adwaita

#endif // ADWAITA_RENDERER_H


