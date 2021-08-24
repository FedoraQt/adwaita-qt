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

#include "adwaitarenderer.h"
#include "adwaitacolors.h"

#include <QPainter>
#include <QPainterPath>
#include <QRect>

#include <cmath>

#ifndef M_PI
    #define M_PI 3.14159265358979323846
#endif

namespace Adwaita
{

static qreal frameRadius(qreal bias = 0)
{
    return qMax(qreal(Metrics::Frame_FrameRadius) - 0.5 + bias, 0.0);
}

static QPainterPath roundedPath(const QRectF &rect, Corners corners, qreal radius)
{
    QPainterPath path;

    // simple cases
    if (corners == 0) {
        path.addRect(rect);
        return path;
    }

    if (corners == AllCorners) {
        path.addRoundedRect(rect, radius, radius);
        return path;
    }

    QSizeF cornerSize(2 * radius, 2 * radius);

    // rotate counterclockwise
    // top left corner
    if (corners & CornerTopLeft) {
        path.moveTo(rect.topLeft() + QPointF(radius, 0));
        path.arcTo(QRectF(rect.topLeft(), cornerSize), 90, 90);
    } else {
        path.moveTo(rect.topLeft());
    }

    // bottom left corner
    if (corners & CornerBottomLeft) {
        path.lineTo(rect.bottomLeft() - QPointF(0, radius));
        path.arcTo(QRectF(rect.bottomLeft() - QPointF(0, 2 * radius), cornerSize), 180, 90);
    } else {
        path.lineTo(rect.bottomLeft());
    }

    // bottom right corner
    if (corners & CornerBottomRight) {
        path.lineTo(rect.bottomRight() - QPointF(radius, 0));
        path.arcTo(QRectF(rect.bottomRight() - QPointF(2 * radius, 2 * radius), cornerSize), 270, 90);
    } else {
        path.lineTo(rect.bottomRight());
    }

    // top right corner
    if (corners & CornerTopRight) {
        path.lineTo(rect.topRight() + QPointF(0, radius));
        path.arcTo(QRectF(rect.topRight() - QPointF(2 * radius, 0), cornerSize), 0, 90);
    } else {
        path.lineTo(rect.topRight());
    }

    path.closeSubpath();
    return path;
}

void Renderer::renderDebugFrame(const StyleOptions &options)
{
    if (!options.painter()) {
        return;
    }

    options.painter()->save();
    options.painter()->setRenderHints(QPainter::Antialiasing);
    options.painter()->setBrush(Qt::NoBrush);
    options.painter()->setPen(Qt::red);
    options.painter()->drawRect(QRectF(options.rect()).adjusted(0.5, 0.5, -0.5, -0.5));
    options.painter()->restore();
}

void Renderer::renderFocusRect(const StyleOptions &options, Sides sides)
{
    if (!options.painter()) {
        return;
    }

    if (!options.color().isValid()) {
        return;
    }

    options.painter()->save();
    options.painter()->setRenderHints(QPainter::Antialiasing);
    options.painter()->setBrush(options.color());

    if (!(options.outlineColor().isValid() && sides)) {
        options.painter()->setPen(Qt::NoPen);
        options.painter()->drawRect(options.rect());
    } else {
        options.painter()->setClipRect(options.rect());

        QRectF copy(options.rect());
        copy.adjust(0.5, 0.5, -0.5, -0.5);

        qreal radius(frameRadius(-1.0));
        if (!(sides & SideTop)) {
            copy.adjust(0, -radius, 0, 0);
        }
        if (!(sides & SideBottom)) {
            copy.adjust(0, 0, 0, radius);
        }
        if (!(sides & SideLeft)) {
            copy.adjust(-radius, 0, 0, 0);
        }
        if (!(sides & SideRight)) {
            copy.adjust(0, 0, radius, 0);
        }

        options.painter()->setPen(options.outlineColor());
        // options.painter()->setBrush( Qt::NoBrush );
        options.painter()->drawRoundedRect(copy, radius, radius);
    }

    options.painter()->restore();
}

void Renderer::renderFocusLine(const StyleOptions &options)
{
    if (!options.painter()) {
        return;
    }

    if (!options.color().isValid()) {
        return;
    }

    options.painter()->save();

    QPen pen(options.color(), 1);
    pen.setStyle(Qt::DotLine);

    options.painter()->setRenderHint(QPainter::Antialiasing, false);
    options.painter()->setPen(pen);
    options.painter()->setBrush(Qt::NoBrush);

    options.painter()->drawRoundedRect(options.rect(), 1, 1);

    options.painter()->restore();
}

void Renderer::renderFrame(const StyleOptions &options)
{
    if (!options.painter()) {
        return;
    }

    options.painter()->save();
    options.painter()->setRenderHint(QPainter::Antialiasing);

    QRectF frameRect(options.rect().adjusted(1, 1, -1, -1));
    qreal radius(frameRadius());

    // set pen
    if (options.outlineColor().isValid()) {
        if (options.hasFocus()) {
            options.painter()->setPen(QPen(options.outlineColor(), 2));
            frameRect.adjust(0.5, 0.5, -0.5, -0.5);
        } else {
            options.painter()->setPen(options.outlineColor());
        }
        frameRect.adjust(0.5, 0.5, -0.5, -0.5);
        radius = qMax(radius - 1, qreal(0.0));

    } else {
        options.painter()->setPen(Qt::NoPen);
    }

    // set brush
    if (options.color().isValid()) {
        options.painter()->setBrush(options.color());
    } else {
        options.painter()->setBrush(Qt::NoBrush);
    }

    // render
    options.painter()->drawRoundedRect(frameRect, radius, radius);
    options.painter()->restore();
}

void Renderer::renderSquareFrame(const StyleOptions &options)
{
    if (!options.painter()) {
        return;
    }

    options.painter()->save();
    options.painter()->setPen(options.color());
    options.painter()->drawRect(options.rect().adjusted(1, 1, -2, -2));
    if (options.hasFocus()) {
        options.color().setAlphaF(0.5);
        options.painter()->setPen(options.color());
        options.painter()->drawRect(options.rect().adjusted(0, 0, -1, -1));
    }
    options.painter()->restore();
}

void Renderer::renderFlatFrame(const StyleOptions &options)
{
    if (!options.painter()) {
        return;
    }

    options.painter()->save();
    options.painter()->setRenderHint(QPainter::Antialiasing);

    QRectF frameRect(options.rect().adjusted(1, 1, -1, -1));
    qreal radius(frameRadius());

    // set pen
    if (options.outlineColor().isValid()) {
        if (options.hasFocus()) {
            options.painter()->setPen(QPen(options.outlineColor(), 2));
            frameRect.adjust(0.5, 0.5, -0.5, -0.5);
        } else {
            options.painter()->setPen(options.outlineColor());
        }
        frameRect.adjust(0.5, 0.5, -0.5, -0.5);
        radius = qMax(radius - 1, qreal(0.0));
    } else {
        options.painter()->setPen(Qt::NoPen);
    }

    // set brush
    if (options.color().isValid()) {
        options.painter()->setBrush(options.color());
    } else {
        options.painter()->setBrush(Qt::NoBrush);
    }

    QPainterPath path;
    path.setFillRule(Qt::WindingFill);
    path.addRect(frameRect.adjusted(2 * radius, 0, 0, 0));
    path.addRoundedRect(frameRect.adjusted(0, 0, - 2 * radius, 0), radius, radius);

    options.painter()->drawPath(path.simplified());
    options.painter()->restore();

    // render
    //options.painter()->drawRoundedRect( frameRect, radius, radius );
}


void Renderer::renderFlatRoundedButtonFrame(const StyleOptions &options)
{
    if (!options.painter()) {
        return;
    }

    options.painter()->save();
    // setup options.painter()
    options.painter()->setRenderHint(QPainter::Antialiasing, true);

    // copy options.rect()
    QRectF frameRect(options.rect());
    frameRect.adjust(1, 1, -1, -1);
    qreal radius(frameRadius());

    if (options.outlineColor().isValid()) {
        options.painter()->setPen(QPen(options.outlineColor(), 1.0));

        frameRect.adjust(0.5, 0.5, -0.5, -0.5);
        radius = qMax(radius - 1, qreal(0.0));
    } else {
        options.painter()->setPen(Qt::NoPen);
    }

    // content
        if (options.color().isValid() && options.active()) {
        options.painter()->setBrush(Colors::buttonBackgroundGradient(options));
    } else if (!options.active()) {
        options.painter()->setBrush(options.color());
    } else {
        options.painter()->setBrush(Qt::NoBrush);
    }

    // render
    options.painter()->drawEllipse(frameRect);
    options.painter()->restore();
}

void Renderer::renderSidePanelFrame(const StyleOptions &options, Side side)
{
    if (!options.painter()) {
        return;
    }

    // check color
    if (!options.outlineColor().isValid()) {
        return;
    }

    // adjust options.rect()
    QRectF frameRect(options.rect().adjusted(1, 1, -1, -1));
    frameRect.adjust(0.5, 0.5, -0.5, -0.5);

    // setup options.painter()
    options.painter()->save();
    options.painter()->setRenderHint(QPainter::Antialiasing);
    options.painter()->setPen(options.outlineColor());

    // render
    switch (side) {
    case SideLeft:
        frameRect.adjust(0, 1, 0, -1);
        options.painter()->drawLine(frameRect.topRight(), frameRect.bottomRight());
        break;
    case SideTop:
        frameRect.adjust(1, 0, -1, 0);
        options.painter()->drawLine(frameRect.topLeft(), frameRect.topRight());
        break;
    case SideRight:
        frameRect.adjust(0, 1, 0, -1);
        options.painter()->drawLine(frameRect.topLeft(), frameRect.bottomLeft());
        break;
    case SideBottom:
        frameRect.adjust(1, 0, -1, 0);
        options.painter()->drawLine(frameRect.bottomLeft(), frameRect.bottomRight());
        break;
    case AllSides: {
        qreal radius(frameRadius(-1.0));
        options.painter()->drawRoundedRect(frameRect, radius, radius);
        break;
    }
    default:
        break;
    }

    options.painter()->restore();
}

void Renderer::renderMenuFrame(const StyleOptions &options, bool roundCorners)
{
    if (!options.painter()) {
        return;
    }

    options.painter()->save();

    // set brush
    if (options.color().isValid()) {
        options.painter()->setBrush(options.color());
    } else {
        options.painter()->setBrush(Qt::NoBrush);
    }

    options.painter()->setRenderHint(QPainter::Antialiasing, false);
    QRectF frameRect(options.rect());
    if (options.outlineColor().isValid()) {
        options.painter()->setPen(options.outlineColor());
        frameRect.adjust(0.5, 0.5, -0.5, -0.5);
    } else {
        options.painter()->setPen(Qt::NoPen);
    }

    options.painter()->drawRect(frameRect);
    options.painter()->restore();
}

void Renderer::renderButtonFrame(const StyleOptions &options)
{
    if (!options.painter()) {
        return;
    }

    // setup options.painter()
    options.painter()->save();
    options.painter()->setRenderHint(QPainter::Antialiasing, true);

    // copy options.rect()
    QRectF frameRect(options.rect());
    frameRect.adjust(1, 1, -1, -1);
    qreal radius(frameRadius());

    if (options.outlineColor().isValid()) {
        options.painter()->setPen(QPen(options.outlineColor(), 1.0));

        frameRect.adjust(0.5, 0.5, -0.5, -0.5);
        radius = qMax(radius - 1, qreal(0.0));
    } else {
        options.painter()->setPen(Qt::NoPen);
    }

    // content
    if (options.color().isValid() && options.active()) {
        options.painter()->setBrush(Colors::buttonBackgroundGradient(options));
    } else if (!options.active()) {
        options.painter()->setBrush(options.color());
    } else {
        options.painter()->setBrush(Qt::NoBrush);
    }

    // render
    options.painter()->drawRoundedRect(frameRect, radius, radius);

    if (!options.sunken() && options.active() && options.color().isValid()) {
        options.painter()->setPen(options.color().lighter(140));
        options.painter()->drawLine(frameRect.topLeft() + QPoint(3, 1), frameRect.topRight() + QPoint(-3, 1));
        options.painter()->setPen(options.outlineColor().darker(114));
        options.painter()->drawLine(frameRect.bottomLeft() + QPointF(2.7, 0), frameRect.bottomRight() + QPointF(-2.7, 0));
    }

    options.painter()->restore();
}

void Renderer::renderCheckBoxFrame(const StyleOptions &options)
{
    if (!options.painter()) {
        return;
    }

    // setup options.painter()
    options.painter()->save();
    options.painter()->setRenderHint(QPainter::Antialiasing, true);

    // copy options.rect()
    QRectF frameRect(options.rect());
    frameRect.adjust(1, 1, -1, -1);
    qreal radius(frameRadius());

    if (options.outlineColor().isValid()) {
        options.painter()->setPen(QPen(options.outlineColor(), 1.0));

        frameRect.adjust(0.5, 0.5, -0.5, -0.5);
        radius = qMax(radius - 1, qreal(0.0));
    } else {
        options.painter()->setPen(Qt::NoPen);
    }

    if (options.color().isValid() && options.active()) {
        options.painter()->setBrush(Colors::indicatorBackgroundGradient(options));
    } else if (!options.active()) {
        options.painter()->setBrush(options.color());
    } else {
        options.painter()->setBrush(Qt::NoBrush);
    }

    // render
    options.painter()->drawRoundedRect(frameRect, radius, radius);
    options.painter()->restore();
}

void Renderer::renderFlatButtonFrame(const StyleOptions &options)
{
    if (!options.painter()) {
        return;
    }

    // setup options.painter()
    options.painter()->save();
    options.painter()->setRenderHint(QPainter::Antialiasing, true);

    // copy options.rect()
    QRectF frameRect(options.rect());
    frameRect.adjust(1, 1, -1, -1);
    qreal radius(frameRadius());

    if (options.outlineColor().isValid()) {
        options.painter()->setPen(QPen(options.outlineColor(), 1.0));

        frameRect.adjust(0.5, 0.5, -0.5, -0.5);
        radius = qMax(radius - 1, qreal(0.0));
    } else {
        options.painter()->setPen(Qt::NoPen);
    }

    // content
    if (options.color().isValid()) {
        QLinearGradient gradient(frameRect.topLeft(), frameRect.bottomLeft());
        //gradient.setColorAt( 0, options.color().darker( sunken ? 110 : (options.hasFocus()|mouseOver) ? 85 : 100 ) );
        //gradient.setColorAt( 1, options.color().darker( sunken ? 130 : (options.hasFocus()|mouseOver) ? 95 : 110 ) );

        if (!options.active()) {
            gradient.setColorAt(0, options.color());
        } else if (options.sunken()) {
            gradient.setColorAt(0, options.color());
        } else {
            gradient.setColorAt(0, Colors::mix(options.color(), Qt::white, 0.07));
            gradient.setColorAt(1, Colors::mix(options.color(), Qt::black, 0.1));
        }
        options.painter()->setBrush(gradient);
    } else {
        options.painter()->setBrush(Qt::NoBrush);
    }

    QPainterPath path;
    path.setFillRule(Qt::WindingFill);
    path.addRoundedRect(frameRect.adjusted(2 * radius, 0, 0, 0), radius, radius);
    path.addRect(frameRect.adjusted(0, 0, -2 * radius, 0));
    options.painter()->drawPath(path.simplified());

    if (!options.sunken() && options.active()) {
        options.painter()->setPen(options.color().lighter(140));
        options.painter()->drawLine(frameRect.topLeft() + QPoint(1, 1), frameRect.topRight() + QPoint(-3, 1));
        options.painter()->setPen(options.outlineColor().darker(114));
        options.painter()->drawLine(frameRect.bottomLeft() + QPointF(0.7, 0), frameRect.bottomRight() + QPointF(-2.7, 0));
    }

    // render
    //options.painter()->drawRoundedRect( frameRect, radius, radius );

    options.painter()->restore();
}

void Renderer::renderToolButtonFrame(const StyleOptions &options)
{
    if (!options.painter()) {
        return;
    }

    // do nothing for invalid color
    if (!options.color().isValid())
        return;

    // setup options.painter()
    options.painter()->save();
    options.painter()->setRenderHints(QPainter::Antialiasing);

    QRectF baseRect(options.rect());

    if (options.sunken()) {
        qreal radius(frameRadius());

        options.painter()->setPen(Qt::NoPen);
        options.painter()->setBrush(options.color());

        QRectF contentRect(baseRect.adjusted(1, 1, -1, -1));
        options.painter()->drawRoundedRect(contentRect, radius, radius);
    } else {
        qreal radius(frameRadius(-0.5));

        options.painter()->setPen(options.color());
        options.painter()->setBrush(Qt::NoBrush);
        QRectF outlineRect(baseRect.adjusted(1.5, 1.5, -1.5, -1.5));
        options.painter()->drawRoundedRect(outlineRect, radius, radius);
    }
    options.painter()->restore();
}

void Renderer::renderToolBoxFrame(const StyleOptions &options, int tabWidth)
{
    if (!options.painter()) {
        return;
    }

    if (!options.outlineColor().isValid()) {
        return;
    }

    // round radius
    qreal radius(frameRadius());
    QSizeF cornerSize(2 * radius, 2 * radius);

    // if options.rect() - tabwidth is even, need to increase tabWidth by 1 unit
    // for anti aliasing
    if (!((options.rect().width() - tabWidth) % 2)) {
        ++tabWidth;
    }

    // adjust options.rect() for antialiasing
    QRectF baseRect(options.rect());
    baseRect.adjust(0.5, 0.5, -0.5, -0.5);

    // create path
    QPainterPath path;
    path.moveTo(0, baseRect.height() - 1);
    path.lineTo((baseRect.width() - tabWidth) / 2 - radius, baseRect.height() - 1);
    path.arcTo(QRectF(QPointF((baseRect.width() - tabWidth) / 2 - 2 * radius, baseRect.height() - 1 - 2 * radius), cornerSize), 270, 90);
    path.lineTo((baseRect.width() - tabWidth) / 2, radius);
    path.arcTo(QRectF(QPointF((baseRect.width() - tabWidth) / 2, 0), cornerSize), 180, -90);
    path.lineTo((baseRect.width() + tabWidth) / 2 - 1 - radius, 0);
    path.arcTo(QRectF(QPointF((baseRect.width() + tabWidth) / 2  - 1 - 2 * radius, 0), cornerSize), 90, -90);
    path.lineTo((baseRect.width() + tabWidth) / 2 - 1, baseRect.height() - 1 - radius);
    path.arcTo(QRectF(QPointF((baseRect.width() + tabWidth) / 2 - 1, baseRect.height() - 1 - 2 * radius), cornerSize), 180, 90);
    path.lineTo(baseRect.width() - 1, baseRect.height() - 1);

    // render
    options.painter()->save();
    options.painter()->setRenderHints(QPainter::Antialiasing);
    options.painter()->setBrush(Qt::NoBrush);
    options.painter()->setPen(options.outlineColor());
    options.painter()->translate(baseRect.topLeft());
    options.painter()->drawPath(path);
    options.painter()->restore();

    return;
}

void Renderer::renderTabWidgetFrame(const StyleOptions &options, Corners corners)
{
    if (!options.painter()) {
        return;
    }

    options.painter()->setRenderHint(QPainter::Antialiasing);

    QRectF frameRect(options.rect().adjusted(1, 1, -1, -1));
    qreal radius(frameRadius());

    options.painter()->save();

    // set pen
    if (options.outlineColor().isValid()) {
        options.painter()->setPen(options.outlineColor());
        frameRect.adjust(0.5, 0.5, -0.5, -0.5);
        radius = qMax(radius - 1, qreal(0.0));
    } else {
        options.painter()->setPen(Qt::NoPen);
    }

    // set brush
    if (options.color().isValid()) {
        options.painter()->setBrush(options.color());
    } else {
        options.painter()->setBrush(Qt::NoBrush);
    }

    // render
    QPainterPath path(roundedPath(frameRect, corners, radius));
    options.painter()->drawPath(path);
    options.painter()->restore();
}

void Renderer::renderSelection(const StyleOptions &options)
{
    if (!options.painter()) {
        return;
    }

    options.painter()->save();
    options.painter()->setRenderHint(QPainter::Antialiasing);
    options.painter()->setPen(Qt::NoPen);
    options.painter()->setBrush(options.color());
    options.painter()->drawRect(options.rect());
    options.painter()->restore();
}

void Renderer::renderSeparator(const StyleOptions &options, bool vertical)
{
    if (!options.painter()) {
        return;
    }

    options.painter()->save();
    options.painter()->setRenderHint(QPainter::Antialiasing, false);
    options.painter()->setBrush(Qt::NoBrush);
    options.painter()->setPen(options.color());

    if (vertical) {
        options.painter()->translate(options.rect().width() / 2, 0);
        options.painter()->drawLine(options.rect().topLeft(), options.rect().bottomLeft());
    } else {
        options.painter()->translate(0, options.rect().height() / 2);
        options.painter()->drawLine(options.rect().topLeft(), options.rect().topRight());
    }

    options.painter()->restore();
}

void Renderer::renderCheckBoxBackground(const StyleOptions &options)
{
    if (!options.painter()) {
        return;
    }

    // setup options.painter()
    options.painter()->save();
    options.painter()->setRenderHint(QPainter::Antialiasing, true);

    // copy options.rect() and radius
    QRectF frameRect(options.rect());
    frameRect.adjust(3, 3, -3, -3);

    options.painter()->setPen(options.outlineColor());
    options.painter()->setBrush(options.color());
    options.painter()->drawRect(frameRect);
    options.painter()->restore();
}

void Renderer::renderCheckBox(const StyleOptions &options, const QColor &tickColor, qreal animation)
{
    if (!options.painter()) {
        return;
    }

    // setup options.painter()
    options.painter()->save();
    options.painter()->setRenderHint(QPainter::Antialiasing, true);

    // copy options.rect() and radius
    QRectF frameRect(options.rect());
    frameRect.adjust(2, 2, -2, -2);
    qreal radius(frameRadius());

    // content
    {
        StyleOptions tmpStyleOptions(options.painter(), options.rect());
        tmpStyleOptions.setColor(options.color());
        tmpStyleOptions.setOutlineColor(options.outlineColor());
        tmpStyleOptions.setHasFocus(false);
        tmpStyleOptions.setSunken(options.sunken());
        tmpStyleOptions.setMouseOver(options.mouseOver());
        tmpStyleOptions.setActive(options.active());
        tmpStyleOptions.setCheckboxState(options.checkboxState());
        tmpStyleOptions.setColorVariant(options.colorVariant());
        tmpStyleOptions.setInMenu(options.inMenu());
        renderCheckBoxFrame(tmpStyleOptions);
    }

    // mark
    if (options.checkboxState() == CheckOn) {
        options.painter()->save();
        options.painter()->setRenderHint(QPainter::Antialiasing);
        options.painter()->setBrush(Qt::NoBrush);
        QPen pen(tickColor, 3);
        pen.setJoinStyle(Qt::MiterJoin);
        options.painter()->setPen(pen);

        QRectF markerRect(frameRect);

        QPainterPath path;
        path.moveTo(markerRect.right() - markerRect.width() / 4, markerRect.top() + markerRect.height() / 3);
        path.lineTo(markerRect.center().x(), markerRect.bottom() - markerRect.height() / 3.0);
        path.lineTo(markerRect.left() + markerRect.width() / 4, markerRect.center().y());

        options.painter()->setClipRect(markerRect);
        options.painter()->drawPath(path);
        options.painter()->restore();
    } else if (options.checkboxState() == CheckPartial) {
        QPen pen(tickColor, 4);
        pen.setCapStyle(Qt::RoundCap);
        options.painter()->setPen(pen);

        QRectF markerRect(frameRect.adjusted(4, 4, -4, -4));

        options.painter()->drawLine(markerRect.center() - QPoint(3, 0), markerRect.center() + QPoint(3, 0));
    } else if (options.checkboxState() == CheckAnimated) {
        options.painter()->save();
        options.painter()->setRenderHint(QPainter::Antialiasing);
        options.painter()->setBrush(Qt::NoBrush);
        QPen pen(tickColor, 3);
        pen.setJoinStyle(Qt::MiterJoin);
        options.painter()->setPen(pen);

        QRectF markerRect(frameRect);

        QPainterPath path;
        path.moveTo(markerRect.right() - markerRect.width() / 4, markerRect.top() + markerRect.height() / 3);
        path.lineTo(markerRect.center().x(), markerRect.bottom() - markerRect.height() / 3.0);
        path.lineTo(markerRect.left() + markerRect.width() / 4, markerRect.center().y());
        path.translate(-markerRect.right(), -markerRect.top());

        options.painter()->setClipRect(markerRect.adjusted(1, 1, -1, -1));
        options.painter()->translate(markerRect.right(), markerRect.top());
        options.painter()->scale(animation, 0.5 + 0.5 * animation);
        options.painter()->drawPath(path);
        options.painter()->restore();
    }

    options.painter()->restore();
}

void Renderer::renderRadioButtonBackground(const StyleOptions &options)
{
    if (!options.painter()) {
        return;
    }

    // setup options.painter()
    options.painter()->save();
    options.painter()->setRenderHint(QPainter::Antialiasing, true);

    // copy options.rect()
    QRectF frameRect(options.rect());
    frameRect.adjust(3, 3, -3, -3);
    if (options.sunken()) {
        frameRect.translate(1, 1);
    }

    options.painter()->setPen(options.outlineColor());
    options.painter()->setBrush(options.color());
    options.painter()->drawEllipse(frameRect);
    options.painter()->restore();
}

void Renderer::renderRadioButton(const StyleOptions &options, const QColor &tickColor, qreal animation)
{
    if (!options.painter()) {
        return;
    }

    // setup options.painter()
    options.painter()->save();
    options.painter()->setRenderHint(QPainter::Antialiasing, true);

    // copy options.rect()
    QRectF frameRect(options.rect());
    frameRect.adjust(2, 2, -2, -2);

    if (options.color().isValid() && options.active()) {
        options.painter()->setBrush(Colors::indicatorBackgroundGradient(options));
    } else if (!options.active()) {
        options.painter()->setBrush(options.color());
    } else {
        options.painter()->setBrush(Qt::NoBrush);
    }

    options.painter()->setPen(QPen(options.outlineColor(), 1));
    QRectF contentRect(frameRect.adjusted(0.5, 0.5, -0.5, -0.5));
    options.painter()->drawEllipse(contentRect);

    // mark
    if (options.radioButtonState() == RadioOn) {
        options.painter()->setBrush(tickColor);
        options.painter()->setPen(Qt::NoPen);

        QRectF markerRect(frameRect.adjusted(5, 5, -5, -5));
        options.painter()->drawEllipse(markerRect);
    } else if (options.radioButtonState() == RadioAnimated) {
        options.painter()->setBrush(tickColor);
        options.painter()->setPen(Qt::NoPen);
        QRectF markerRect(frameRect.adjusted(5, 5, -5, -5));
        qreal remaining = markerRect.width() / 2.0 * (1.0 - animation);
        markerRect.adjust(remaining, remaining, -remaining, -remaining);

        options.painter()->drawEllipse(markerRect);
    }

    options.painter()->restore();
}

void Renderer::renderSliderGroove(const StyleOptions &options)
{
    if (!options.painter()) {
        return;
    }

    // setup options.painter()
    options.painter()->save();
    options.painter()->setRenderHint(QPainter::Antialiasing, true);

    QRectF baseRect(options.rect());
    qreal radius(0.5 * Metrics::Slider_GrooveThickness);

    // content
    if (options.color().isValid()) {
        options.painter()->setPen(Qt::NoPen);
        options.painter()->setBrush(options.color());
        options.painter()->drawRoundedRect(baseRect, radius, radius);
    }

    options.painter()->restore();
}

void Renderer::renderSliderHandle(const StyleOptions &options, Side ticks, qreal angle)
{
    if (!options.painter()) {
        return;
    }

    // setup options.painter()
    options.painter()->save();
    options.painter()->setRenderHint(QPainter::Antialiasing, true);

    // copy options.rect()
    QRectF frameRect(options.rect());
    frameRect.adjust(1, 1, -1, -1);

    // set pen
    if (options.outlineColor().isValid()) {
        QPen pen(options.outlineColor());
        pen.setCapStyle(Qt::FlatCap);
        pen.setJoinStyle(Qt::MiterJoin);
        options.painter()->setPen(pen);

        frameRect.adjust(0.5, 0.5, -0.5, -0.5);
    } else
        options.painter()->setPen(Qt::NoPen);

    // set brush
    if (options.color().isValid() && options.active()) {
        options.painter()->setBrush(Colors::buttonBackgroundGradient(options));
    }  else if (!options.active()) {
        options.painter()->setBrush(options.color());
    } else {
        options.painter()->setBrush(Qt::NoBrush);
    }

    QRect r(options.rect().right() - options.rect().height(), options.rect().top(), options.rect().height(), options.rect().height());
    r.adjust(4.5, 3.5, -2.5, -3.5);

    QPainterPath circle;
    circle.addEllipse(r);
    circle.closeSubpath();

    if (ticks & SideBottom) {
        QPainterPath triangle(r.center());
        triangle.moveTo(r.left() + 1.5, r.center().y() + 5.5);
        triangle.lineTo(r.center().x() + 1, r.bottom() + 4.5);
        triangle.lineTo(r.right() - 0.5, r.center().y() + 5.5);
        triangle.closeSubpath();
        circle = circle.united(triangle);
    } else if (ticks & SideTop) {
        QPainterPath triangle(r.center());
        triangle.moveTo(r.left() + 1.5, r.center().y() - 3.5);
        triangle.lineTo(r.center().x() + 1, r.top() - 2.5);
        triangle.lineTo(r.right() - 0.5, r.center().y() - 3.5);
        triangle.closeSubpath();
        circle = circle.united(triangle);
    } else if (ticks & SideLeft) {
        QPainterPath triangle(r.center());
        triangle.moveTo(r.center().x() - 3.5, r.top() + 1.5);
        triangle.lineTo(r.left() - 2.5, r.center().y() + 1);
        triangle.lineTo(r.center().x() - 3.5, r.bottom() - 0.5);
        triangle.closeSubpath();
        circle = circle.united(triangle);
    } else if (ticks & SideRight) {
        QPainterPath triangle(r.center());
        triangle.moveTo(r.center().x() + 3.5, r.top() + 1.5);
        triangle.lineTo(r.right() + 2.5, r.center().y() + 1);
        triangle.lineTo(r.center().x() + 3.5, r.bottom() - 0.5);
        triangle.closeSubpath();
        circle = circle.united(triangle);
    }

    QTransform rotate;
    rotate.translate(frameRect.center().x(), frameRect.center().y());
    rotate.rotate(angle);
    rotate.translate(-frameRect.center().x(), -frameRect.center().y());
    options.painter()->drawPolygon(circle.toFillPolygon(rotate));
    options.painter()->restore();
}

void Renderer::renderDialGroove(const StyleOptions &options)
{
    if (!options.painter()) {
        return;
    }

    // setup options.painter()
    options.painter()->save();
    options.painter()->setRenderHint(QPainter::Antialiasing, true);

    QRectF baseRect(options.rect());

    // content
    if (options.color().isValid()) {
        qreal penWidth(Metrics::Slider_GrooveThickness);
        QRectF grooveRect(options.rect().adjusted(penWidth / 2, penWidth / 2, -penWidth / 2, -penWidth / 2));

        options.painter()->setPen(QPen(options.color(), penWidth));
        options.painter()->setBrush(Qt::NoBrush);
        options.painter()->drawEllipse(grooveRect);
    }

    return;
}

void Renderer::renderDialContents(const StyleOptions &options, qreal first, qreal second)
{
    if (!options.painter()) {
        return;
    }

    // setup options.painter()
    options.painter()->setRenderHint(QPainter::Antialiasing, true);

    QRectF baseRect(options.rect());

    // content
    if (options.color().isValid()) {
        // setup groove options.rect()
        qreal penWidth(Metrics::Slider_GrooveThickness);
        QRectF grooveRect(options.rect().adjusted(penWidth / 2, penWidth / 2, -penWidth / 2, -penWidth / 2));

        // setup angles
        int angleStart(first * 180 * 16 / M_PI);
        int angleSpan((second - first) * 180 * 16 / M_PI);

        // setup pen
        if (angleSpan != 0) {
            QPen pen(options.color(), penWidth);
            pen.setCapStyle(Qt::RoundCap);
            options.painter()->setPen(pen);
            options.painter()->setBrush(Qt::NoBrush);
            options.painter()->drawArc(grooveRect, angleStart, angleSpan);
        }
    }

    options.painter()->restore();
}

void Renderer::renderProgressBarGroove(const StyleOptions &options)
{
    if (!options.painter()) {
        return;
    }

    // setup options.painter()
    options.painter()->save();
    options.painter()->setRenderHint(QPainter::Antialiasing, true);
    options.painter()->setRenderHint(QPainter::SmoothPixmapTransform, true);

    QRectF baseRect(options.rect());
    qreal radius(0.5);

    // content
    if (options.color().isValid()) {
        options.painter()->setBrush(options.color());

    }
    if (options.outlineColor().isValid()) {
        options.painter()->setPen(options.outlineColor());
    }
    options.painter()->drawRoundedRect(baseRect.translated(0.5, 0.5), radius, radius);

    options.painter()->restore();
}

void Renderer::renderProgressBarContents(const StyleOptions &options)
{
    return renderProgressBarGroove(options);
}

void Renderer::renderProgressBarBusyContents(const StyleOptions &options, bool horizontal, bool reverse, int progress)
{
    Q_UNUSED(reverse)

    if (!options.painter()) {
        return;
    }

    // setup options.painter()
    options.painter()->save();
    options.painter()->setRenderHint(QPainter::Antialiasing, true);

    QRectF baseRect(options.rect());
    qreal radius(0.25 * Metrics::ProgressBar_Thickness);
    QRectF contentRect;
    if (horizontal) {
        contentRect = QRect(baseRect.left(), baseRect.top(), Metrics::ProgressBar_BusyIndicatorSize, baseRect.height());
        contentRect.translate(fabs(progress - 50) / 50.0 * (baseRect.width() - contentRect.width()), 0);
    } else {
        contentRect = QRect(baseRect.left(), baseRect.top(), baseRect.width(), Metrics::ProgressBar_BusyIndicatorSize);
        contentRect.translate(0, fabs(progress - 50) / 50.0 * (baseRect.height() - contentRect.height()));
    }

    options.painter()->setBrush(options.color());
    options.painter()->setPen(options.outlineColor());
    options.painter()->drawRoundedRect(contentRect.translated(0.5, 0.5), radius, radius);
    options.painter()->restore();
}

void Renderer::renderScrollBarGroove(const StyleOptions &options)
{
    return renderScrollBarHandle(options);
}

void Renderer::renderScrollBarHandle(const StyleOptions &options)
{
    if (!options.painter()) {
        return;
    }

    // setup options.painter()
    options.painter()->save();
    options.painter()->setRenderHint(QPainter::Antialiasing, true);

    QRectF baseRect(options.rect());
    qreal metric(options.rect().width() < options.rect().height() ? options.rect().width() : options.rect().height());
    qreal radius(0.5 * metric);

    // content
    if (options.color().isValid()) {
        options.painter()->setPen(Qt::NoPen);
        options.painter()->setBrush(options.color());
        options.painter()->drawRoundedRect(baseRect, radius, radius);
    }

    options.painter()->restore();
}

void Renderer::renderToolBarHandle(const StyleOptions &options)
{
    return renderProgressBarGroove(options);
}

void Renderer::renderTabBarTab(const StyleOptions &options, const QColor &background, Corners corners, bool renderFrame)
{
    if (!options.painter()) {
        return;
    }

    // setup options.painter()
    options.painter()->save();
    options.painter()->setRenderHint(QPainter::Antialiasing, false);

    QRectF frameRect(options.rect());
    qreal adjustment;

    // pen
    if (options.outlineColor().isValid()) {
        options.painter()->setPen(options.outlineColor());
        frameRect.adjust(1.0, 1.0, -1.0, -1.0);
        adjustment = 0;

        options.painter()->setBrush(background);

        // render
        options.painter()->drawRect(frameRect);
    } else if (!renderFrame) {
        adjustment = 9;
    }

    options.painter()->setPen(QPen(options.color(), 6));

    switch (corners) {
    case CornersTop:
        options.painter()->drawLine(frameRect.left() + adjustment, frameRect.bottom(), frameRect.right() - adjustment, frameRect.bottom());
        break;
    case CornersBottom:
        options.painter()->drawLine(frameRect.left() + adjustment, frameRect.top(), frameRect.right() - adjustment, frameRect.top());
        break;
    case CornersLeft:
        options.painter()->drawLine(frameRect.right(), frameRect.top() + adjustment, frameRect.right(), frameRect.bottom() - adjustment);
        break;
    case CornersRight:
        options.painter()->drawLine(frameRect.left(), frameRect.top() + adjustment, frameRect.left(), frameRect.bottom() - adjustment);
        break;
    }

    options.painter()->restore();
}

void Renderer::renderArrow(const StyleOptions &options, ArrowOrientation arrowOrientation)
{
    if (!options.painter()) {
        return;
    }

    // define polygon
    QPolygonF arrow;
    switch (arrowOrientation) {
    case ArrowUp:
        arrow << QPointF(-4, 2) << QPointF(0, -2) << QPointF(4, 2);
        break;
    case ArrowDown:
        arrow << QPointF(-4, -2) << QPointF(0, 2) << QPointF(4, -2);
        break;
    case ArrowLeft:
        arrow << QPointF(2, -4) << QPointF(-2, 0) << QPointF(2, 4);
        break;
    case ArrowRight:
        arrow << QPointF(-2, -4) << QPointF(2, 0) << QPointF(-2, 4);
        break;
    default:
        break;
    }

    QPen pen(options.color(), 1.2);
    pen.setCapStyle(Qt::FlatCap);
    pen.setJoinStyle(Qt::MiterJoin);

    options.painter()->save();
    options.painter()->setRenderHints(QPainter::Antialiasing);
    options.painter()->translate(QRectF(options.rect()).center());
    options.painter()->setBrush(options.color());
    options.painter()->setPen(pen);
    options.painter()->drawPolygon(arrow);

    options.painter()->restore();
}

void Renderer::renderSign(const StyleOptions &options, bool orientation)
{
    if (!options.painter()) {
        return;
    }

    QPen pen(options.color(), 2);
    pen.setCapStyle(Qt::FlatCap);

    QRect r = options.rect().adjusted(1, 2, 0, 0);

    options.painter()->save();
    options.painter()->setPen(pen);
    options.painter()->drawLine(r.center() - QPointF(5, 0), r.center() + QPointF(5, 0));
    if (orientation) {
        options.painter()->drawLine(r.center() - QPointF(0, 5), r.center() + QPointF(0, 5));
    }
    options.painter()->restore();
}

void Renderer::renderDecorationButton(const StyleOptions &options, ButtonType buttonType)
{
    if (!options.painter()) {
        return;
    }

    options.painter()->save();
    options.painter()->setViewport(options.rect());
    options.painter()->setWindow(0, 0, 18, 18);
    options.painter()->setRenderHints(QPainter::Antialiasing, false);

    // initialize pen
    QPen pen;
    pen.setCapStyle(Qt::RoundCap);
    pen.setJoinStyle(Qt::MiterJoin);

    options.painter()->setBrush(Qt::NoBrush);

    pen.setColor(options.color());
    pen.setCapStyle(Qt::RoundCap);
    pen.setJoinStyle(Qt::MiterJoin);
    pen.setWidthF(2.0 * qMax(1.0, 18.0 / options.rect().width()));
    options.painter()->setPen(pen);

    switch (buttonType) {
    case ButtonClose: {
        options.painter()->setRenderHints(QPainter::Antialiasing, true);
        options.painter()->drawLine(QPointF(5, 5), QPointF(13, 13));
        options.painter()->drawLine(13, 5, 5, 13);
        break;
    }
    case ButtonMaximize: {
        options.painter()->drawRect(5, 5, 8, 8);
        break;
    }
    case ButtonMinimize: {
        options.painter()->drawPolyline(QPolygonF()
                              << QPointF(5, 13)
                              << QPointF(13, 13));
        break;
    }
    case ButtonRestore: {
        options.painter()->drawRect(6, 6, 6, 6);
        break;
    }
    default:
        break;
    }

    options.painter()->restore();
}

} // namespace Adwaita
