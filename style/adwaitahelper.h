#ifndef adwaita_helper_h
#define adwaita_helper_h

/*************************************************************************
 * Copyright (C) 2014 by Hugo Pereira Da Costa <hugo.pereira@free.fr>    *
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


#include "adwaita.h"
#include "adwaitaanimationdata.h"
#include "config-adwaita.h"

#include <QPainterPath>
#include <QWidget>
#include <QStyle>

#if ADWAITA_HAVE_X11
#include <QX11Info>
#include <xcb/xcb.h>
#endif

#include <cmath>

namespace Adwaita
{

    //* adwaita style helper class.
    /** contains utility functions used at multiple places in both adwaita style and adwaita window decoration */
    class Helper
    {
        public:

        //* constructor
        explicit Helper( );

        #if ADWAITA_USE_KDE4
        //* constructor
        explicit Helper( const QByteArray& );
        #endif

        //* destructor
        virtual ~Helper()
        {}

        //*@name color utilities
        //@{

        // Borrowed from the KColorUtils code
        static QColor mix(const QColor &c1, const QColor &c2, qreal bias = 0.5)
        {
            auto mixQreal = [](qreal a, qreal b, qreal bias) { return a + (b - a) * bias; };

            if (bias <= 0.0) return c1;
            if (bias >= 1.0) return c2;
            if (std::isnan(bias)) return c1;

            qreal r = mixQreal(c1.redF(),   c2.redF(),   bias);
            qreal g = mixQreal(c1.greenF(), c2.greenF(), bias);
            qreal b = mixQreal(c1.blueF(),  c2.blueF(),  bias);
            qreal a = mixQreal(c1.alphaF(), c2.alphaF(), bias);

            return QColor::fromRgbF(r, g, b, a);
        }

        //* add alpha channel multiplier to color
        QColor alphaColor( QColor color, qreal alpha ) const;

        // ADWAITA TODO

        //* mouse over color
        QColor hoverColor( const QPalette& palette ) const
        { return palette.highlight().color(); }

        //* focus color
        QColor focusColor( const QPalette& palette ) const
        { return palette.highlight().color(); }

        //* negative text color (used for close button)
        QColor negativeText( const QPalette& palette ) const
        // { return _viewNegativeTextBrush.brush( palette ).color(); }
        { Q_UNUSED(palette); return Qt::red; }

        //* shadow
        QColor shadowColor( const QPalette& palette ) const
        { return alphaColor( palette.color( QPalette::Shadow ), 0.15 ); }

        //* titlebar color
        QColor titleBarColor( const QPalette& palette, bool active ) const
        { return palette.color(active ? QPalette::Active : QPalette::Inactive, QPalette::Window); }

        //* titlebar text color
        QColor titleBarTextColor( const QPalette& palette, bool active ) const
        { return palette.color(active ? QPalette::Active : QPalette::Inactive, QPalette::WindowText); }

        //* frame outline color, using animations
        QColor frameOutlineColor( const QPalette&, bool mouseOver = false, bool hasFocus = false, qreal opacity = AnimationData::OpacityInvalid, AnimationMode = AnimationNone ) const;

        //* focus outline color, using animations
        QColor focusOutlineColor( const QPalette& ) const;

        //* hover outline color, using animations
        QColor hoverOutlineColor( const QPalette& ) const;

        //* focus outline color, using animations
        QColor buttonFocusOutlineColor( const QPalette& ) const;

        //* hover outline color, using animations
        QColor buttonHoverOutlineColor( const QPalette& ) const;

        //* side panel outline color, using animations
        QColor sidePanelOutlineColor( const QPalette&, bool hasFocus = false, qreal opacity = AnimationData::OpacityInvalid, AnimationMode = AnimationNone ) const;

        //* frame background color
        QColor frameBackgroundColor( const QPalette& palette ) const
        { return frameBackgroundColor( palette, palette.currentColorGroup() ); }

        //* frame background color
        QColor frameBackgroundColor( const QPalette&, QPalette::ColorGroup ) const;

        //* arrow outline color
        QColor arrowColor( const QPalette&, QPalette::ColorGroup, QPalette::ColorRole ) const;

        //* arrow outline color
        QColor arrowColor( const QPalette& palette, QPalette::ColorRole role ) const
        { return arrowColor( palette, palette.currentColorGroup(), role ); }

        //* arrow outline color, using animations
        QColor arrowColor( const QPalette&, bool mouseOver, bool hasFocus, qreal opacity = AnimationData::OpacityInvalid, AnimationMode = AnimationNone ) const;

        //* button outline color, using animations
        QColor buttonOutlineColor( const QPalette&, bool mouseOver, bool hasFocus, qreal opacity = AnimationData::OpacityInvalid, AnimationMode = AnimationNone ) const;

        //* button panel color, using animations
        QColor buttonBackgroundColor( const QPalette&, bool mouseOver, bool hasFocus, bool sunken, qreal opacity = AnimationData::OpacityInvalid, AnimationMode = AnimationNone ) const;

        //* tool button color
        QColor toolButtonColor( const QPalette&, bool mouseOver, bool hasFocus, bool sunken, qreal opacity = AnimationData::OpacityInvalid, AnimationMode = AnimationNone ) const;

        //* slider outline color, using animations
        QColor sliderOutlineColor( const QPalette&, bool mouseOver, bool hasFocus, qreal opacity = AnimationData::OpacityInvalid, AnimationMode = AnimationNone ) const;

        //* scrollbar handle color, using animations
        QColor scrollBarHandleColor( const QPalette&, bool mouseOver, bool hasFocus, qreal opacity = AnimationData::OpacityInvalid, AnimationMode = AnimationNone ) const;

        //* checkbox indicator, using animations
        QColor checkBoxIndicatorColor( const QPalette&, bool mouseOver, bool active, qreal opacity = AnimationData::OpacityInvalid, AnimationMode = AnimationNone ) const;

        //* separator color
        QColor separatorColor( const QPalette& ) const;

        // TreeView header text color
        QColor headerTextColor( const QPalette& palette, const QStyle::State state ) const;

        //* merge active and inactive palettes based on ratio, for smooth enable state change transition
        QPalette disabledPalette( const QPalette&, qreal ratio ) const;

        //@}

        //*@name rendering utilities
        //@{

        //* debug frame
        void renderDebugFrame( QPainter*, const QRect& ) const;

        //* focus rect
        void renderFocusRect( QPainter*, const QRect&, const QColor&, const QColor& outline = QColor(), Sides = 0 ) const;

        //* focus line
        void renderFocusLine( QPainter*, const QRect&, const QColor& ) const;

        //* generic frame
        void renderFrame( QPainter*, const QRect&, const QColor& color, const QColor& outline = QColor(), bool hasFocus = false ) const;

        //* generic frame flat on right side
        void renderFlatFrame( QPainter*, const QRect&, const QColor& color, const QColor& outline = QColor(), bool hasFocus = false ) const;

        //* side panel frame
        void renderSidePanelFrame( QPainter*, const QRect&, const QColor& outline, Side ) const;

        //* menu frame
        void renderMenuFrame( QPainter*, const QRect&, const QColor& color, const QColor& outline, bool roundCorners = true ) const;

        //* button frame
        void renderButtonFrame(QPainter*, const QRect&, const QColor& color, const QColor& outline, const QColor& shadow, bool focus, bool sunken, bool mouseOver , bool active) const;

        //* button frame
        void renderFlatButtonFrame(QPainter*, const QRect&, const QColor& color, const QColor& outline, const QColor& shadow, bool focus, bool sunken, bool mouseOver , bool active) const;

        //* toolbutton frame
        void renderToolButtonFrame( QPainter*, const QRect&, const QColor& color, bool sunken ) const;

        //* toolbutton frame
        void renderToolBoxFrame( QPainter*, const QRect&, int tabWidth, const QColor& color ) const;

        //* tab widget frame
        void renderTabWidgetFrame( QPainter*, const QRect&, const QColor& color, const QColor& outline, Corners ) const;

        //* selection frame
        void renderSelection( QPainter*, const QRect&, const QColor& ) const;

        //* separator
        void renderSeparator( QPainter*, const QRect&, const QColor&, bool vertical = false ) const;

        //* checkbox
        void renderCheckBoxBackground( QPainter*, const QRect&, const QColor& color, const QColor& outline, bool sunken ) const;

        //* checkbox
        void renderCheckBox( QPainter*, const QRect&, const QColor& background, const QColor& outline, const QColor& tickColor, bool sunken, CheckBoxState state, qreal animation = AnimationData::OpacityInvalid, bool active = true ) const;

        //* radio button
        void renderRadioButtonBackground( QPainter*, const QRect&, const QColor& color, const QColor& outline, bool sunken ) const;

        //* radio button
        void renderRadioButton(QPainter*, const QRect&, const QColor& background, const QColor& outline, const QColor& tickColor, bool sunken, bool enabled, RadioButtonState state, qreal animation = AnimationData::OpacityInvalid ) const;

        //* slider groove
        void renderSliderGroove( QPainter*, const QRect&, const QColor& ) const;

        //* slider handle
        void renderSliderHandle( QPainter*, const QRect&, const QColor&, const QColor& outline, const QColor& shadow, bool sunken, bool enabled, Side ticks) const;

        //* dial groove
        void renderDialGroove( QPainter*, const QRect&, const QColor& ) const;

        //* dial groove
        void renderDialContents( QPainter*, const QRect&, const QColor&, qreal first, qreal second ) const;

        //* progress bar groove
        void renderProgressBarGroove( QPainter*, const QRect&, const QColor&, const QColor& ) const;

        //* progress bar contents
        void renderProgressBarContents( QPainter* painter, const QRect& rect, const QColor& color, const QColor& outline ) const
        { return renderProgressBarGroove( painter, rect, color, outline ); }

        //* progress bar contents (animated)
        void renderProgressBarBusyContents( QPainter* painter, const QRect& rect, const QColor& color, const QColor& outline, bool horizontal, bool reverse, int progress  ) const;

        //* scrollbar groove
        void renderScrollBarGroove( QPainter* painter, const QRect& rect, const QColor& color ) const
        { return renderScrollBarHandle( painter, rect, color ); }

        //* scrollbar handle
        void renderScrollBarHandle( QPainter*, const QRect&, const QColor& ) const;

        //* toolbar handle
        void renderToolBarHandle( QPainter* painter, const QRect& rect, const QColor& color ) const
        { return renderProgressBarGroove( painter, rect, color, Qt::transparent ); }

        //* tabbar tab
        void renderTabBarTab( QPainter*, const QRect&, const QColor& color, const QColor& outline, Corners, bool renderFrame ) const;

        //* generic arrow
        void renderArrow( QPainter*, const QRect&, const QColor&, ArrowOrientation ) const;

        //* generic sign (+-)
        void renderSign(QPainter*painter, const QRect&rect, const QColor&color, bool orientation ) const;

        //* generic button (for mdi decorations, tabs and dock widgets)
        void renderDecorationButton( QPainter*, const QRect&, const QColor&, ButtonType, bool inverted ) const;

        //@}

        //*@name compositing utilities
        //@{

        //* true if style was compiled for and is running on X11
        static bool isX11( void );

        //* true if running on platform Wayland
        static bool isWayland( void );

        //* returns true if compositing is active
        bool compositingActive( void ) const;

        //* returns true if a given widget supports alpha channel
        bool hasAlphaChannel( const QWidget* ) const;

        //@}

        //@name high dpi utility functions
        //@{

        //* return dpi-aware pixmap of given size
        virtual QPixmap highDpiPixmap( const QSize& size ) const
        { return highDpiPixmap( size.width(), size.height() ); }

        //* return dpi-aware pixmap of given size
        virtual QPixmap highDpiPixmap( int width ) const
        { return highDpiPixmap( width, width ); }

        //* return dpi-aware pixmap of given size
        virtual QPixmap highDpiPixmap( int width, int height ) const;

        //* return device pixel ratio for a given pixmap
        virtual qreal devicePixelRatio( const QPixmap& ) const;

        //@}

        //*@name X11 utilities
        //@{

        #if ADWAITA_HAVE_X11

        //* get xcb connection
        static xcb_connection_t* connection( void );

        //* create xcb atom
        xcb_atom_t createAtom( const QString& ) const;

        #endif

        //@}

        //* frame radius
        qreal frameRadius( qreal bias = 0 ) const
        { return qMax( qreal( Metrics::Frame_FrameRadius ) - 0.5 + bias, 0.0 ); }

        protected:

        //* initialize
        void init( void );

        //* return rectangle for widgets shadow, offset depending on light source
        QRectF shadowRect( const QRectF& ) const;

        //* return rounded path in a given rect, with only selected corners rounded, and for a given radius
        QPainterPath roundedPath( const QRectF&, Corners, qreal ) const;

        private:

        #if ADWAITA_HAVE_X11

        //* atom used for compositing manager
        xcb_atom_t _compositingManagerAtom;

        #endif

    };

}

#endif
