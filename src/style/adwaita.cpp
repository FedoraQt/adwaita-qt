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

#include <QtGui>
#include <QtGui/QPainter>

#include "adwaita.h"
#include "config.h"

Adwaita::Adwaita() : QCommonStyle() {
    QFile f(ADWAITA_CSS_FILE);
    f.open(QIODevice::ReadOnly);
    m_styleSheet = f.readAll();
}

void Adwaita::polish(QPalette &palette)
{
    // All, used especially for active elements in a focused window
    palette.setColor(QPalette::All,      QPalette::Window,          QColor("#ededed"));
    palette.setColor(QPalette::All,      QPalette::WindowText,      QColor("#2e3436"));
    palette.setColor(QPalette::All,      QPalette::Base,            QColor("white"));
    palette.setColor(QPalette::All,      QPalette::AlternateBase,   QColor("#ededed"));
    palette.setColor(QPalette::All,      QPalette::ToolTipBase,     QColor("#060606"));
    palette.setColor(QPalette::All,      QPalette::ToolTipText,     QColor("white"));
    palette.setColor(QPalette::All,      QPalette::Text,            QColor("#2e3436"));
    palette.setColor(QPalette::All,      QPalette::Button,          QColor("#e0e0e0"));
    palette.setColor(QPalette::All,      QPalette::ButtonText,      QColor("#2e3436"));
    palette.setColor(QPalette::All,      QPalette::BrightText,      QColor("white"));

    palette.setColor(QPalette::All,      QPalette::Light,           QColor("#fafafa"));
    palette.setColor(QPalette::All,      QPalette::Midlight,        QColor("#f3f3f3"));
    palette.setColor(QPalette::All,      QPalette::Dark,            QColor("#e0e0e0"));
    palette.setColor(QPalette::All,      QPalette::Mid,             QColor("#e6e6e6"));
    palette.setColor(QPalette::All,      QPalette::Shadow,          QColor("black"));

    palette.setColor(QPalette::All,      QPalette::Highlight,       QColor("#4a90d9"));
    palette.setColor(QPalette::All,      QPalette::HighlightedText, QColor("white"));

    palette.setColor(QPalette::All,      QPalette::Link,            QColor("#2a76c6"));
    palette.setColor(QPalette::All,      QPalette::LinkVisited,     QColor("#2a76c6"));

    // Exceptions for disabled elements in a focused window
    palette.setColor(QPalette::Disabled, QPalette::Window,          QColor("#f4f4f4"));
    palette.setColor(QPalette::Disabled, QPalette::WindowText,      QColor("#8d9091"));
    palette.setColor(QPalette::Disabled, QPalette::Base,            QColor("white"));
    palette.setColor(QPalette::Disabled, QPalette::AlternateBase,   QColor("#ededed"));
//     palette.setColor(QPalette::Disabled, QPalette::ToolTipBase,     QColor("#ff1234"));
//     palette.setColor(QPalette::Disabled, QPalette::ToolTipText,     QColor("#ff1234"));
    palette.setColor(QPalette::Disabled, QPalette::Text,            QColor("#8d9091"));
    palette.setColor(QPalette::Disabled, QPalette::Button,          QColor("#f4f4f4"));
    palette.setColor(QPalette::Disabled, QPalette::ButtonText,      QColor("#8d9091"));
    palette.setColor(QPalette::Disabled, QPalette::BrightText,      QColor("#ededed"));

    palette.setColor(QPalette::Disabled, QPalette::Light,           QColor("#f4f4f4"));
    palette.setColor(QPalette::Disabled, QPalette::Midlight,        QColor("#f4f4f4"));
    palette.setColor(QPalette::Disabled, QPalette::Dark,            QColor("#f4f4f4"));
    palette.setColor(QPalette::Disabled, QPalette::Mid,             QColor("#f4f4f4"));
    palette.setColor(QPalette::Disabled, QPalette::Shadow,          QColor("black"));

    palette.setColor(QPalette::Disabled, QPalette::Highlight,       QColor("#4a90d9"));
    palette.setColor(QPalette::Disabled, QPalette::HighlightedText, QColor("white"));

    palette.setColor(QPalette::Disabled, QPalette::Link,            QColor("#4a90d9"));
    palette.setColor(QPalette::Disabled, QPalette::LinkVisited,     QColor("#4a90d9"));


    // Exceptions for an unfocused window
    palette.setColor(QPalette::Inactive, QPalette::Window,          QColor("#ededed"));
    palette.setColor(QPalette::Inactive, QPalette::WindowText,      QColor("#54595a"));
    palette.setColor(QPalette::Inactive, QPalette::Base,            QColor("#fcfcfc"));
    palette.setColor(QPalette::Inactive, QPalette::AlternateBase,   QColor("#ededed"));
//     palette.setColor(QPalette::Inactive, QPalette::ToolTipBase,     QColor("#ff1234"));
//     palette.setColor(QPalette::Inactive, QPalette::ToolTipText,     QColor("#ff1234"));
    palette.setColor(QPalette::Inactive, QPalette::Text,            QColor("#54595a"));
    palette.setColor(QPalette::Inactive, QPalette::Button,          QColor("#ededed"));
    palette.setColor(QPalette::Inactive, QPalette::ButtonText,      QColor("#54595a"));
    palette.setColor(QPalette::Inactive, QPalette::BrightText,      QColor("#ededed"));

    palette.setColor(QPalette::Inactive, QPalette::Light,           QColor("#ededed"));
    palette.setColor(QPalette::Inactive, QPalette::Midlight,        QColor("#ededed"));
    palette.setColor(QPalette::Inactive, QPalette::Dark,            QColor("#ededed"));
    palette.setColor(QPalette::Inactive, QPalette::Mid,             QColor("#ededed"));
    palette.setColor(QPalette::Inactive, QPalette::Shadow,          QColor("black"));

    palette.setColor(QPalette::Inactive, QPalette::Highlight,       QColor("#4a90d9"));
    palette.setColor(QPalette::Inactive, QPalette::HighlightedText, QColor("white"));

    palette.setColor(QPalette::Inactive, QPalette::Link,            QColor("#4a90d9"));
    palette.setColor(QPalette::Inactive, QPalette::LinkVisited,     QColor("#4a90d9"));
}


void Adwaita::polish(QWidget *widget)
{
    static bool guard = false;
    if (!guard) {
        guard = true;
        if (widget)
            widget->setStyleSheet(m_styleSheet);
        guard = false;
    }
}

void Adwaita::polish(QApplication* app)
{
    app->setStyleSheet(m_styleSheet);
}


void Adwaita::unpolish(QWidget *widget)
{
}

void Adwaita::unpolish(QApplication* app)
{
}


int Adwaita::pixelMetric(PixelMetric metric, const QStyleOption *opt, const QWidget *widget) const
{
    switch(metric) {
        case PM_ButtonShiftVertical:
        case PM_ButtonShiftHorizontal:
        case PM_ButtonDefaultIndicator:
            return 0;
//         case PM_LayoutHorizontalSpacing:
        case PM_LayoutVerticalSpacing:
            return QCommonStyle::pixelMetric(metric, opt, widget) - 1;
        default:
            return QCommonStyle::pixelMetric(metric, opt, widget);
    }
}

int Adwaita::styleHint(StyleHint hint, const QStyleOption *opt, const QWidget *widget,
                       QStyleHintReturn *returnData) const
{
    switch (hint) {
        case QStyle::SH_Menu_SloppySubMenus:
        case QStyle::SH_Menu_MouseTracking:
        case QStyle::SH_MenuBar_MouseTracking:
            return 1;
        case QStyle::PM_ProgressBarChunkWidth:
            return 0;
        default:
            return QCommonStyle::styleHint(hint, opt, widget, returnData);
    }
}

void Adwaita::drawPrimitive(PrimitiveElement element, const QStyleOption *opt, QPainter *p,
                            const QWidget *widget) const
{
    switch(element) {
        case PE_PanelButtonCommand: {
            QRect rect = opt->rect.adjusted(0, 3, -1, -1);
            QLinearGradient buttonGradient(0.0, rect.top(), 0.0, rect.bottom());
            if (opt->state & State_Active && opt->state & State_Enabled) {
                if (opt->state & State_On) {
                    buttonGradient.setColorAt(0.0, QColor("#a8a8a8"));
                    buttonGradient.setColorAt(0.05, QColor("#c0c0c0"));
                    buttonGradient.setColorAt(0.15, QColor("#d6d6d6"));
                }
                else {
                    buttonGradient.setColorAt(0.0, QColor("#fafafa"));
                    buttonGradient.setColorAt(1.0, QColor("#e0e0e0"));
                }
            }
            else {
                buttonGradient.setColorAt(0.0, opt->palette.button().color());
            }
            QBrush buttonBrush(buttonGradient);
            p->save();
            p->setBrush(buttonBrush);
            p->setPen(QColor("#a1a1a1"));
            p->drawRoundedRect(rect, 3, 3);
            p->restore();
            break;
        }
        case PE_FrameFocusRect:
        case PE_IndicatorProgressChunk:
            break;
        default:
            QCommonStyle::drawPrimitive(element, opt, p, widget);
            break;
    }
}



void Adwaita::drawControl(ControlElement element, const QStyleOption *opt, QPainter *p,
                          const QWidget *widget) const
{
    switch(element) {
        case CE_ProgressBarGroove: {
            const QStyleOptionProgressBarV2 *pbopt = qstyleoption_cast<const QStyleOptionProgressBarV2*>(opt);
            QRect rect = pbopt->rect.adjusted(0, 0, -1, -1);
            p->save();
            QLinearGradient bgGrad;
            if (pbopt->orientation == Qt::Horizontal)
                bgGrad = QLinearGradient(0.0, rect.top()+1, 0.0, rect.bottom()+1);
            else
                bgGrad = QLinearGradient(rect.left()+1, 0.0, rect.right()+1, 0.0);
            bgGrad.setColorAt(0.0, QColor("#b2b2b2"));
            bgGrad.setColorAt(0.2, QColor("#d2d2d2"));
            bgGrad.setColorAt(0.9, QColor("#d2d2d2"));
            bgGrad.setColorAt(1.0, QColor("#b2b2b2"));
            p->setBrush(QBrush(bgGrad));
            p->setPen(QColor("#a1a1a1"));
            p->drawRoundedRect(rect, 3, 3);
            p->restore();
            break;
        }
        case CE_ProgressBarContents: {
            const QStyleOptionProgressBarV2 *pbopt = qstyleoption_cast<const QStyleOptionProgressBarV2*>(opt);
            QRect rect = pbopt->rect.adjusted(0, 0, -1, -1);
            p->save();
            QLinearGradient bgGrad;
            if (pbopt->orientation == Qt::Horizontal) {
                if (pbopt->progress >= 0) {
                    qreal ratio = (((qreal) pbopt->progress) - pbopt->minimum) / (((qreal) pbopt->maximum) - pbopt->minimum);
                    if (pbopt->invertedAppearance)
                        rect.adjust(rect.width() * ratio, 0, 0, 0);
                    else
                        rect.adjust(0, 0, -(rect.width() * ratio), 0);
                }
                bgGrad = QLinearGradient(0.0, rect.top()+1, 0.0, rect.bottom()+1);
            }
            else {
                if (pbopt->progress >= 0) {
                    qreal ratio = (((qreal) pbopt->progress) - pbopt->minimum) / (((qreal) pbopt->maximum) - pbopt->minimum);
                    if (pbopt->invertedAppearance)
                        rect.adjust(0, rect.height() * ratio, 0, 0);
                    else
                        rect.adjust(0, 0, 0, -(rect.height() * ratio));
                }
                bgGrad = QLinearGradient(rect.left()+1, 0.0, rect.right()+1, 0.0);
            }
            bgGrad.setColorAt(0.0, QColor("#4081C5"));
            if (pbopt->progress >= 0) {
                bgGrad.setColorAt(0.1, QColor("#4C91D9"));
                bgGrad.setColorAt(0.95, QColor("#4C91D9"));
            }
            else {
                bgGrad.setColorAt(0.1, QColor("#94CAFF"));
                bgGrad.setColorAt(0.95, QColor("#94CAFF"));
            }
            bgGrad.setColorAt(1.0, QColor("#4081C5"));
            p->setBrush(QBrush(bgGrad));
            p->setPen(QColor("black"));
            p->drawRoundedRect(rect, 2, 2);
            p->restore();
            break;
        }
        case CE_ProgressBarLabel: {
            const QStyleOptionProgressBarV2 *pbopt = qstyleoption_cast<const QStyleOptionProgressBarV2*>(opt);
            p->save();
            QFont font = p->font();
            font.setPointSize(8);
            p->setFont(font);
            p->setPen("#a8a8a8");
            p->setBrush(Qt::transparent);
            p->drawText(pbopt->rect, Qt::AlignHCenter | Qt::AlignBottom, pbopt->text);
            p->restore();
            break;
        }
        default:
            QCommonStyle::drawControl(element, opt, p, widget);
            break;
    }
}

void Adwaita::drawComplexControl(QStyle::ComplexControl control, const QStyleOptionComplex* opt,
                                 QPainter* p, const QWidget* widget) const
{
    switch(control) {
        case CC_Slider: {
            const QStyleOptionSlider *slOpt = qstyleoption_cast<const QStyleOptionSlider*>(opt);
            QRect handle = subControlRect(control, slOpt, SC_SliderHandle, widget);
            p->save();
            p->setPen("red");
            p->setBrush(Qt::blue);
            p->drawRect(slOpt->rect);
            p->setPen("green");
            p->setBrush(Qt::green);
            p->drawRect(handle);
            p->restore();
            break;
        }
        default:
            QCommonStyle::drawComplexControl(control, opt, p, widget);
            break;
    }
}

void Adwaita::drawItemPixmap(QPainter* painter, const QRect& rect, int alignment, const QPixmap& pixmap) const
{
    QCommonStyle::drawItemPixmap(painter, rect, alignment, pixmap);
}

void Adwaita::drawItemText(QPainter* painter, const QRect& rect, int alignment, const QPalette& pal,
                           bool enabled, const QString& text, QPalette::ColorRole textRole) const
{
    QCommonStyle::drawItemText(painter, rect, alignment, pal, enabled, text, textRole);
}

QRect Adwaita::subControlRect(QStyle::ComplexControl cc, const QStyleOptionComplex* opt, QStyle::SubControl sc, const QWidget* w) const
{
    switch(cc) {
        case CC_Slider: {
            switch (sc) {
                case SC_SliderHandle: {
                    const QStyleOptionSlider *slOpt = qstyleoption_cast<const QStyleOptionSlider*>(opt);
                    QRect rect(0, 0, 0, 0);
                    if (slOpt->orientation == Qt::Horizontal) {
                        qreal ratio = (((qreal) slOpt->sliderPosition) - slOpt->minimum) / (((qreal) slOpt->maximum) - slOpt->minimum);
                        rect.setWidth(slOpt->rect.height());
                        rect.setHeight(slOpt->rect.height());
                        if (!slOpt->upsideDown)
                            rect.translate((slOpt->rect.width() - rect.width()) * ratio, 0);
                        else
                            rect.translate((slOpt->rect.width() - rect.width())- (slOpt->rect.width() - rect.width()) * ratio, 0);
                    }
                    else {
                        qreal ratio = (((qreal) slOpt->sliderPosition) - slOpt->minimum) / (((qreal) slOpt->maximum) - slOpt->minimum);
                        rect.setWidth(slOpt->rect.width());
                        rect.setHeight(slOpt->rect.width());
                        if (!slOpt->upsideDown)
                            rect.translate(0, (slOpt->rect.height() - rect.height()) * ratio);
                        else
                            rect.translate(0, (slOpt->rect.height() - rect.height()) - (slOpt->rect.height() - rect.height()) * ratio);
                    }
                    return rect;
                }
            }
        }
    }
    return QCommonStyle::subControlRect(cc, opt, sc, w);
}

QRect Adwaita::subElementRect(QStyle::SubElement r, const QStyleOption* opt, const QWidget* widget) const
{
    switch(r) {
        case SE_ProgressBarGroove:
        case SE_ProgressBarContents: {
            const QStyleOptionProgressBarV2 *pbopt = qstyleoption_cast<const QStyleOptionProgressBarV2*>(opt);
            if (!pbopt) {
                qDebug() << "QStyleOptionProgressBarV2 cast failed!";
                return QCommonStyle::subElementRect(r, opt, widget).adjusted(0, 8, 0, -8);
            }
            if (pbopt->textVisible) {
                if (pbopt->orientation == Qt::Horizontal)
                    return opt->rect.adjusted(0, 13, 0, 0);
                else
                    return opt->rect.adjusted(13, 0, 0, 0);
            }
            else {
                if (pbopt->orientation == Qt::Horizontal)
                    return opt->rect.adjusted(0, 2, 0, -9);
                else
                    return opt->rect.adjusted(2, 0, -9, 0);
            }
        }
        case SE_ProgressBarLabel: {
            const QStyleOptionProgressBarV2 *pbopt = qstyleoption_cast<const QStyleOptionProgressBarV2*>(opt);
            if (!pbopt) {
                qDebug() << "QStyleOptionProgressBarV2 cast failed!";
                return QCommonStyle::subElementRect(r, opt, widget).adjusted(0, 8, 0, -8);
            }
            if (!pbopt->textVisible)
                return QRect(0, 0, 0, 0);
            if (pbopt->orientation == Qt::Horizontal)
                return opt->rect.adjusted(0, 0, 0, -5);
            else
                return opt->rect.adjusted(0, 0, -5, 0);
        }
        case SE_PushButtonContents:
            return QCommonStyle::subElementRect(r, opt, widget).adjusted(0, 2, 0, 2);
        case SE_PushButtonLayoutItem:
        case SE_PushButtonFocusRect:
        case SE_ToolButtonLayoutItem:
//             return QCommonStyle::subElementRect(r, opt, widget).adjusted(0, 0, -10, -10);
        default:
            return QCommonStyle::subElementRect(r, opt, widget);
    }
}

QSize Adwaita::sizeFromContents(QStyle::ContentsType ct, const QStyleOption* opt, const QSize& contentsSize, const QWidget* widget) const
{
    switch(ct) {
        case CT_ProgressBar: {
            if (qstyleoption_cast<const QStyleOptionProgressBarV2*>(opt)->textVisible)
                return QSize(19, 19);
            else
                return QSize(17, 17);
        }
        case CT_PushButton:
            return QCommonStyle::sizeFromContents(ct, opt, contentsSize, widget) + QSize(4, 5);
        default:
            return QCommonStyle::sizeFromContents(ct, opt, contentsSize, widget);
    }
}


