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
    void renderDebugFrame(const StyleOptions &options);

    void renderFocusRect(const StyleOptions &options, Sides sides = SideNone);

    void renderFocusLine(const StyleOptions &options);

    void renderFrame(const StyleOptions &options);

    void renderSquareFrame(const StyleOptions &options);

    void renderFlatFrame(const StyleOptions &options);

    void renderSidePanelFrame(const StyleOptions &options, Side side);

    void renderMenuFrame(const StyleOptions &options, bool roundCorners = true);

    void renderButtonFrame(const StyleOptions &options, const QColor &shadow);

    void renderCheckBoxFrame(const StyleOptions &options, const QColor &shadow);

    void renderFlatButtonFrame(const StyleOptions &options, const QColor &shadow);

    void renderToolButtonFrame(const StyleOptions &options);

    void renderToolBoxFrame(const StyleOptions &options, int tabWidth);

    void renderTabWidgetFrame(const StyleOptions &options, Corners corners);

    void renderSelection(const StyleOptions &options);

    void renderSeparator(const StyleOptions &options, bool vertical = false);

    void renderCheckBoxBackground(const StyleOptions &options);

    void renderCheckBox(const StyleOptions &options, const QColor &background, const QColor &tickColor, qreal animation);

    void renderRadioButtonBackground(const StyleOptions &options);

    void renderRadioButton(const StyleOptions &options, const QColor &background, const QColor &tickColor, qreal animation);

    void renderSliderGroove(const StyleOptions &options);

    void renderSliderHandle(const StyleOptions &options, const QColor &shadow, Side ticks, qreal angle = 0.0);

    void renderDialGroove(const StyleOptions &options);

    void renderDialContents(const StyleOptions &options, qreal first, qreal second);

    void renderProgressBarGroove(const StyleOptions &options);

    void renderProgressBarContents(const StyleOptions &options);

    void renderProgressBarBusyContents(const StyleOptions &options, bool horizontal, bool reverse, int progress);

    void renderScrollBarGroove(const StyleOptions &options);

    void renderScrollBarHandle(const StyleOptions &options);

    void renderToolBarHandle(const StyleOptions &options);

    void renderTabBarTab(const StyleOptions &options, const QColor &background, Corners corners, bool renderFrame);

    void renderArrow(const StyleOptions &options, ArrowOrientation arrowOrientation);

    void renderSign(const StyleOptions &options, bool orientation);

    void renderDecorationButton(const StyleOptions &options, ButtonType buttonType, bool inverted);
};

} // namespace Adwaita

#endif // ADWAITA_RENDERER_H


