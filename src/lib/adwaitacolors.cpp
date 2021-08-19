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

#include "adwaitacolors.h"
#include "adwaitacolors_p.h"
#include "adwaitadebug.h"
#include "animations/adwaitaanimationdata.h"

#include <QGuiApplication>
#include <QFile>
#include <QMetaEnum>
#include <QRegularExpression>

#include <QtMath>

Q_LOGGING_CATEGORY(ADWAITA, "adwaita.colors")

namespace Adwaita
{

Q_GLOBAL_STATIC(ColorsPrivate, colorsGlobal)

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
    if (qSqrt(((textColor.red() * textColor.red()) * 0.299) +
              ((textColor.green() * textColor.green()) * 0.587) +
              ((textColor.blue() * textColor.blue()) * 0.114)) > 128) {
        return true;
    }

    return false;
}

static ColorsPrivate::AdwaitaColor colorNameToEnum(const QString &name)
{
    Q_ASSERT(!name.isEmpty());

    QMetaObject metaObject = ColorsPrivate::staticMetaObject;

    QMetaEnum metaEnum = metaObject.enumerator(metaObject.indexOfEnumerator("AdwaitaColor"));
    ColorsPrivate::AdwaitaColor value = static_cast<ColorsPrivate::AdwaitaColor>(metaEnum.keyToValue(name.toLatin1()));

    return (value <= ColorsPrivate::invalid_color || value > ColorsPrivate::alt_focus_border_color) ? ColorsPrivate::invalid_color : value;
}

static ColorsPrivate::AdwaitaButtonColor buttonColorNameToEnum(const QString &name)
{
    Q_ASSERT(!name.isEmpty());

    QMetaObject metaObject = ColorsPrivate::staticMetaObject;

    QMetaEnum metaEnum = metaObject.enumerator(metaObject.indexOfEnumerator("AdwaitaButtonColor"));
    ColorsPrivate::AdwaitaButtonColor value = static_cast<ColorsPrivate::AdwaitaButtonColor>(metaEnum.keyToValue(name.toLatin1()));

    return (value <= ColorsPrivate::invalid_button_color || value > ColorsPrivate::button_disabled_active_text_color) ? ColorsPrivate::invalid_button_color : value;
}

static QColor colorFromText(const QString &name)
{
    if (name == QStringLiteral("white")) {
        return Qt::white;
    } else if (name == QStringLiteral("black")) {
        return Qt::black;
    }

    return QColor(name);
}

static QString buttonColorSuffixFromOptions(const StyleOptions &options)
{
    bool isDisabled = options.palette().currentColorGroup() == QPalette::Disabled;
    bool isInactive = options.palette().currentColorGroup() == QPalette::Inactive;
    QString result;

    // Checked button
    if (options.sunken()) {
        result += QStringLiteral("_checked");
    } else if (isInactive && isDisabled) {
        result += QStringLiteral("_backdrop_insensitive");
    } else if (isInactive) {
        result += QStringLiteral("_backdrop");
    } else if (isDisabled) {
        result += QStringLiteral("_disabled");
    }

    if (options.animationMode() == AnimationPressed) {
        // button_active_color doesn't exist
        if (result.isEmpty()) {
            result += QStringLiteral("_checked");
        } else {
            result += QStringLiteral("_active");
        }
    } else if (options.animationMode() == AnimationHover) {
         result += QStringLiteral("_hover");
    } else if (options.mouseOver()) {
         result += QStringLiteral("_hover");
    }

    return result;
}

ColorsPrivate::ColorsPrivate()
{
    const QStringList variants = { QStringLiteral("light"), QStringLiteral("dark"),
                                   QStringLiteral("hc"), QStringLiteral("hc-dark") };
    for (const QString &variant : variants) {
        ColorVariant colorVariant;
        if (variant == QStringLiteral("light")) {
            colorVariant = Adwaita;
        } else if (variant == QStringLiteral("dark")) {
            colorVariant = AdwaitaDark;
        } else if (variant == QStringLiteral("hc")) {
            colorVariant = AdwaitaHighcontrast;
        } else {
            colorVariant = AdwaitaHighcontrastDark;
        }

        const QString filename = QStringLiteral(":/stylesheet/Adwaita-%1.css").arg(variant);
        QFile file(filename);

        if (!file.open(QIODevice::ReadOnly)) {
            continue;
        }

        QTextStream in(&file);
        while (!in.atEnd()) {
            const QString line = in.readLine();

            if (line.isEmpty()) {
                continue;
            }

            // E.g. light_fg #2e3436;
            const QRegularExpression reBase("^@define\\-color\\ ([a-z|_]+)\\ (#[a-z|0-9]{6}|white|black);$");
            // E.g. borders_edge rgba(255, 255, 255, 0.8);
            const QRegularExpression reRgba("^@define\\-color\\ ([a-z|_]+)\\ rgba\\((\\d+)[,|\\ ]+(\\d+)[,|\\ ]+(\\d+)[,|\\ ]+([\\d|\\.]+)\\);$");
            // E.g. button:hover { color: #2e3436; border-color: #cdc7c2; background-image: linear-gradient(to top, #d6d1cd, #e8e6e3 1px); box-shadow: 0 1px 2px rgba(0, 0, 0, 0.07); }
            const QRegularExpression reButtonColor("^(button[:|a-z]*)\\ \\{\\ color:\\ (#[a-z|0-9]{6}|white|black);\\ (outline-color: (.*);\\ )*border-color: (#[a-z|0-9]{6}|white|black);\\ background-image: (linear-gradient|image)\\((.[^\\)]*)\\);\\ (box-shadow:\\ .*;\\ )*\\}");

            const QRegularExpressionMatch reBaseMatch = reBase.match(line);
            const QRegularExpressionMatch reRgbaMatch = reRgba.match(line);
            const QRegularExpressionMatch reButtonColorMatch = reButtonColor.match(line);

            if (reBaseMatch.hasMatch()) {
                // qWarning() << reBaseMatch.captured(0);
                const AdwaitaColor color = colorNameToEnum(reBaseMatch.captured(1));
                auto map = m_colors.value(color);

                // Process captured color
                const QColor capturedColor = colorFromText(reBaseMatch.captured(2));
                map.insert(colorVariant, capturedColor);
                m_colors.insert(color, map);
            } else if (reRgbaMatch.hasMatch()) {
                // qWarning() << reRgbaMatch.captured(0);
                const AdwaitaColor color = colorNameToEnum(reRgbaMatch.captured(1));
                auto map = m_colors.value(color);

                // Process captured color
                const int r = reRgbaMatch.captured(2).toInt();
                const int g = reRgbaMatch.captured(3).toInt();
                const int b = reRgbaMatch.captured(4).toInt();
                const double a = reRgbaMatch.captured(5).toDouble();
                const QColor capturedColor(r, g, b, 255 * a);
                map.insert(colorVariant, capturedColor);

                m_colors.insert(color, map);
            } else if (reButtonColorMatch.hasMatch()) {
                // qWarning() << reBaseMatch.captured(0);

                const QString buttonColorName = reButtonColorMatch.captured(1).replace(QLatin1Char(':'), QLatin1Char('_'));

                // Outline color
                if (!reButtonColorMatch.captured(3).isEmpty()) {
                    const QString outlineColor = reButtonColorMatch.captured(4);
                    const QRegularExpression reOutlineColor("^rgba\\((\\d+)[,|\\ ]+(\\d+)[,|\\ ]+(\\d+)[,|\\ ]+([\\d|\\.]+)\\)$");
                    const QRegularExpressionMatch outlineColorMatch = reOutlineColor.match(outlineColor);
                    auto outlineColorMap = m_buttonColors.value(button_outline_color);

                    const QColor capturedOutlineColor(outlineColorMatch.captured(1).toInt(),
                                                      outlineColorMatch.captured(2).toInt(),
                                                      outlineColorMatch.captured(3).toInt(),
                                                      255 * outlineColorMatch.captured(4).toDouble());
                    outlineColorMap.insert(colorVariant, capturedOutlineColor);
                    m_buttonColors.insert(button_outline_color, outlineColorMap);
                }

                // Border color
                const AdwaitaButtonColor borderColor = buttonColorNameToEnum(buttonColorName + QStringLiteral("_border_color"));
                auto borderColorMap = m_buttonColors.value(borderColor);

                const QColor capturedBorderColor = colorFromText(reButtonColorMatch.captured(5));
                borderColorMap.insert(colorVariant, capturedBorderColor);
                m_buttonColors.insert(borderColor, borderColorMap);

                // Text color
                const AdwaitaButtonColor textColor = buttonColorNameToEnum(buttonColorName + QStringLiteral("_text_color"));
                auto textColorMap = m_buttonColors.value(textColor);

                const QColor capturedTextColor = colorFromText(reButtonColorMatch.captured(2));
                textColorMap.insert(colorVariant, capturedTextColor);
                m_buttonColors.insert(textColor, textColorMap);

                //  Background color
                const QString backgroundType = reButtonColorMatch.captured(6);
                if (backgroundType == QStringLiteral("image")) {
                    const AdwaitaButtonColor bgColor = buttonColorNameToEnum(buttonColorName + QStringLiteral("_color"));
                    auto bgColormap = m_buttonColors.value(bgColor);

                    const QColor capturedBgColor(colorFromText(reButtonColorMatch.captured(7)));
                    bgColormap.insert(colorVariant, capturedBgColor);
                    m_buttonColors.insert(bgColor, bgColormap);
                } else if (backgroundType == QStringLiteral("linear-gradient")) {
                    const QString capturedGradient = reButtonColorMatch.captured(7);
                    // E.g. "to top, #2b2b2b 20%, #2d2d2d 90%"
                    const QRegularExpression reLinerGradient(".[^#]*(#[a-z|0-9]{6}|white|black).[^#]*(#[a-z|0-9]{6}|white|black).*");
                    const QRegularExpressionMatch linearGradientMatch = reLinerGradient.match(capturedGradient);

                    const AdwaitaButtonColor gradientStart = buttonColorNameToEnum(buttonColorName + QStringLiteral("_gradient_start"));
                    const AdwaitaButtonColor gradientStop = buttonColorNameToEnum(buttonColorName + QStringLiteral("_gradient_stop"));

                    auto gradientStartMap = m_buttonColors.value(gradientStart);
                    auto gradientStopMap = m_buttonColors.value(gradientStop);

                    const QColor gradientStartColor = colorFromText(linearGradientMatch.captured(1));
                    const QColor gradientStopColor = colorFromText(linearGradientMatch.captured(2));

                    gradientStartMap.insert(colorVariant, gradientStartColor);
                    gradientStopMap.insert(colorVariant, gradientStopColor);

                    m_buttonColors.insert(gradientStart, gradientStartMap);
                    m_buttonColors.insert(gradientStop, gradientStopMap);
                } else {
                        qCDebug(ADWAITA) << "Unable to process " << buttonColorName << " background color.";
                }
            } else {
                qCDebug(ADWAITA) << "Line: " << line << " cannot be processed.";
            }
        }
    }
}

ColorsPrivate::~ColorsPrivate()
{
}

QColor ColorsPrivate::adwaitaColor(AdwaitaColor color, ColorVariant variant)
{
    return m_colors.value(color).value(variant);
}

QColor ColorsPrivate::adwaitaButtonColor(AdwaitaButtonColor color, ColorVariant variant)
{
    QColor returnValue = m_buttonColors.value(color).value(variant);
    if (!returnValue.isValid()) {
        if (color == button_color) {
            return m_buttonColors.value(button_gradient_start).value(variant);
        } else if (color == button_hover_color) {
            return m_buttonColors.value(button_hover_gradient_start).value(variant);
        }
    }

    return returnValue;
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
    QPalette palette;

    QColor buttonColor = colorsGlobal->adwaitaButtonColor(ColorsPrivate::button_color, variant);
    QColor disabledButtonColor = colorsGlobal->adwaitaButtonColor(ColorsPrivate::button_backdrop_color, variant);

    palette.setColor(QPalette::All,      QPalette::Window,          colorsGlobal->adwaitaColor(ColorsPrivate::bg_color, variant));
    palette.setColor(QPalette::All,      QPalette::WindowText,      colorsGlobal->adwaitaColor(ColorsPrivate::fg_color, variant));
    palette.setColor(QPalette::All,      QPalette::Base,            colorsGlobal->adwaitaColor(ColorsPrivate::base_color, variant));
    palette.setColor(QPalette::All,      QPalette::AlternateBase,   colorsGlobal->adwaitaColor(ColorsPrivate::base_color, variant));
    palette.setColor(QPalette::All,      QPalette::ToolTipBase,     colorsGlobal->adwaitaColor(ColorsPrivate::osd_bg_color, variant));
    palette.setColor(QPalette::All,      QPalette::ToolTipText,     colorsGlobal->adwaitaColor(ColorsPrivate::osd_text_color, variant));
    palette.setColor(QPalette::All,      QPalette::Text,            colorsGlobal->adwaitaColor(ColorsPrivate::fg_color, variant));
    palette.setColor(QPalette::All,      QPalette::Button,          buttonColor);
    palette.setColor(QPalette::All,      QPalette::ButtonText,      colorsGlobal->adwaitaColor(ColorsPrivate::fg_color, variant));
    palette.setColor(QPalette::All,      QPalette::BrightText,      colorsGlobal->adwaitaColor(ColorsPrivate::text_color, variant));

    palette.setColor(QPalette::All,      QPalette::Light,           Colors::lighten(buttonColor));
    palette.setColor(QPalette::All,      QPalette::Midlight,        Colors::mix(Colors::lighten(buttonColor), buttonColor));
    palette.setColor(QPalette::All,      QPalette::Mid,             Colors::mix(Colors::darken(buttonColor), buttonColor));
    palette.setColor(QPalette::All,      QPalette::Dark,            Colors::darken(buttonColor));
    palette.setColor(QPalette::All,      QPalette::Shadow,          colorsGlobal->adwaitaColor(ColorsPrivate::shadow_color, variant));

    palette.setColor(QPalette::All,      QPalette::Highlight,       colorsGlobal->adwaitaColor(ColorsPrivate::selected_bg_color, variant));
    palette.setColor(QPalette::All,      QPalette::HighlightedText, colorsGlobal->adwaitaColor(ColorsPrivate::selected_fg_color, variant));

    palette.setColor(QPalette::All,      QPalette::Link,            colorsGlobal->adwaitaColor(ColorsPrivate::link_color, variant));
    palette.setColor(QPalette::All,      QPalette::LinkVisited,     colorsGlobal->adwaitaColor(ColorsPrivate::link_visited_color, variant));

    palette.setColor(QPalette::Disabled, QPalette::Window,          colorsGlobal->adwaitaColor(ColorsPrivate::insensitive_bg_color, variant));
    palette.setColor(QPalette::Disabled, QPalette::WindowText,      colorsGlobal->adwaitaColor(ColorsPrivate::insensitive_fg_color, variant));
    palette.setColor(QPalette::Disabled, QPalette::Base,            colorsGlobal->adwaitaColor(ColorsPrivate::base_color, variant));
    palette.setColor(QPalette::Disabled, QPalette::AlternateBase,   colorsGlobal->adwaitaColor(ColorsPrivate::base_color, variant));
    palette.setColor(QPalette::Disabled, QPalette::Text,            colorsGlobal->adwaitaColor(ColorsPrivate::insensitive_fg_color, variant));
    palette.setColor(QPalette::Disabled, QPalette::Button,          colorsGlobal->adwaitaColor(ColorsPrivate::insensitive_bg_color, variant));
    palette.setColor(QPalette::Disabled, QPalette::ButtonText,      colorsGlobal->adwaitaColor(ColorsPrivate::insensitive_fg_color, variant));
    palette.setColor(QPalette::Disabled, QPalette::BrightText,      colorsGlobal->adwaitaColor(ColorsPrivate::text_color, variant));

    palette.setColor(QPalette::Disabled, QPalette::Light,           Colors::lighten(buttonColor));
    palette.setColor(QPalette::Disabled, QPalette::Midlight,        Colors::mix(Colors::lighten(buttonColor), buttonColor));
    palette.setColor(QPalette::Disabled, QPalette::Mid,             Colors::mix(Colors::darken(buttonColor), buttonColor));
    palette.setColor(QPalette::Disabled, QPalette::Dark,            Colors::darken(buttonColor));
    palette.setColor(QPalette::Disabled, QPalette::Shadow,          colorsGlobal->adwaitaColor(ColorsPrivate::shadow_color, variant));

    palette.setColor(QPalette::Disabled, QPalette::Highlight,       colorsGlobal->adwaitaColor(ColorsPrivate::selected_bg_color, variant));
    palette.setColor(QPalette::Disabled, QPalette::HighlightedText, colorsGlobal->adwaitaColor(ColorsPrivate::selected_fg_color, variant));

    palette.setColor(QPalette::Disabled, QPalette::Link,            colorsGlobal->adwaitaColor(ColorsPrivate::link_color, variant));
    palette.setColor(QPalette::Disabled, QPalette::LinkVisited,     colorsGlobal->adwaitaColor(ColorsPrivate::link_visited_color, variant));

    palette.setColor(QPalette::Inactive, QPalette::Window,          colorsGlobal->adwaitaColor(ColorsPrivate::backdrop_bg_color, variant));
    palette.setColor(QPalette::Inactive, QPalette::WindowText,      colorsGlobal->adwaitaColor(ColorsPrivate::backdrop_fg_color, variant));
    palette.setColor(QPalette::Inactive, QPalette::Base,            colorsGlobal->adwaitaColor(ColorsPrivate::backdrop_base_color, variant));
    palette.setColor(QPalette::Inactive, QPalette::AlternateBase,   colorsGlobal->adwaitaColor(ColorsPrivate::backdrop_base_color, variant));
    palette.setColor(QPalette::Inactive, QPalette::ToolTipBase,     colorsGlobal->adwaitaColor(ColorsPrivate::osd_bg_color, variant));
    palette.setColor(QPalette::Inactive, QPalette::ToolTipText,     colorsGlobal->adwaitaColor(ColorsPrivate::osd_text_color, variant));
    palette.setColor(QPalette::Inactive, QPalette::Text,            colorsGlobal->adwaitaColor(ColorsPrivate::backdrop_fg_color, variant));
    palette.setColor(QPalette::Inactive, QPalette::Button,          disabledButtonColor);
    palette.setColor(QPalette::Inactive, QPalette::ButtonText,      colorsGlobal->adwaitaColor(ColorsPrivate::backdrop_fg_color, variant));
    palette.setColor(QPalette::Inactive, QPalette::BrightText,      colorsGlobal->adwaitaColor(ColorsPrivate::backdrop_text_color, variant));

    palette.setColor(QPalette::Inactive, QPalette::Light,           Colors::lighten(buttonColor));
    palette.setColor(QPalette::Inactive, QPalette::Midlight,        Colors::mix(Colors::lighten(buttonColor), buttonColor));
    palette.setColor(QPalette::Inactive, QPalette::Mid,             Colors::mix(Colors::darken(buttonColor), buttonColor));
    palette.setColor(QPalette::Inactive, QPalette::Dark,            Colors::darken(buttonColor));
    palette.setColor(QPalette::Inactive, QPalette::Shadow,          colorsGlobal->adwaitaColor(ColorsPrivate::shadow_color, variant));

    palette.setColor(QPalette::Inactive, QPalette::Highlight,       colorsGlobal->adwaitaColor(ColorsPrivate::selected_bg_color, variant));
    palette.setColor(QPalette::Inactive, QPalette::HighlightedText, colorsGlobal->adwaitaColor(ColorsPrivate::selected_fg_color, variant));

    palette.setColor(QPalette::Inactive, QPalette::Link,            colorsGlobal->adwaitaColor(ColorsPrivate::link_color, variant));
    palette.setColor(QPalette::Inactive, QPalette::LinkVisited,     colorsGlobal->adwaitaColor(ColorsPrivate::link_visited_color, variant));

    return palette;
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

    const QString colorName = QStringLiteral("button") + buttonColorSuffixFromOptions(options) + QStringLiteral("_border_color");
    return colorsGlobal->adwaitaButtonColor(buttonColorNameToEnum(colorName), variant);
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

    ColorVariant variant = options.colorVariant();

    if (variant == ColorVariant::Unknown) {
        variant = isDarkMode() ? ColorVariant::AdwaitaDark : ColorVariant::Adwaita;
    }

    if (isDisabled && (options.animationMode() == AnimationPressed || options.sunken())) {
        return colorsGlobal->adwaitaButtonColor(ColorsPrivate::button_disabled_active_color, variant);
    }

    if (options.animationMode() == AnimationPressed) {
        const ColorsPrivate::AdwaitaButtonColor buttonColor = options.sunken() ? ColorsPrivate::button_checked_hover_color : ColorsPrivate::button_hover_color;
        const ColorsPrivate::AdwaitaButtonColor buttonHoverColor = options.sunken() ? ColorsPrivate::button_checked_active_color : ColorsPrivate::button_checked_color;
        return Colors::mix(colorsGlobal->adwaitaButtonColor(buttonColor, variant),
                           colorsGlobal->adwaitaButtonColor(buttonHoverColor, variant), options.opacity());
    } else if (options.animationMode() == AnimationHover) {
        const ColorsPrivate::AdwaitaButtonColor buttonColor = options.sunken() ? ColorsPrivate::button_checked_color : ColorsPrivate::button_color;
        const ColorsPrivate::AdwaitaButtonColor buttonHoverColor = options.sunken() ? ColorsPrivate::button_checked_hover_color : ColorsPrivate::button_hover_color;
        return Colors::mix(colorsGlobal->adwaitaButtonColor(buttonColor, variant),
                           colorsGlobal->adwaitaButtonColor(buttonHoverColor, variant), options.opacity());
    }

    const QString colorName = QStringLiteral("button") + buttonColorSuffixFromOptions(options) + QStringLiteral("_color");
    return colorsGlobal->adwaitaButtonColor(buttonColorNameToEnum(colorName), variant);
}

QLinearGradient Colors::buttonBackgroundGradient(const StyleOptions &options)
{
    ColorVariant variant = options.colorVariant();

    if (variant == ColorVariant::Unknown) {
        variant = isDarkMode() ? ColorVariant::AdwaitaDark : ColorVariant::Adwaita;
    }

    const QString gradientName = QStringLiteral("button") + buttonColorSuffixFromOptions(options) + QStringLiteral("_gradient_stop");
    QColor gradientStartColor = buttonBackgroundColor(options);
    QColor gradientStopColor = colorsGlobal->adwaitaButtonColor(buttonColorNameToEnum(gradientName), variant);

    QLinearGradient gradient(options.rect().bottomLeft(), options.rect().topLeft());
    gradient.setColorAt(0, gradientStartColor);
    gradient.setColorAt(1, gradientStopColor.isValid() ? gradientStopColor : gradientStartColor);

    return gradient;
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
