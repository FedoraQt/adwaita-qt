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

#include <QFile>
#include <QMetaEnum>
#include <QRegularExpression>

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

static ColorsPrivate::AdwaitaColor colorNameToEnum(const QString &name)
{
    Q_ASSERT(!name.isEmpty());

    QMetaObject metaObject = ColorsPrivate::staticMetaObject;

    QMetaEnum metaEnum = metaObject.enumerator(metaObject.indexOfEnumerator("AdwaitaColor"));
    ColorsPrivate::AdwaitaColor value = static_cast<ColorsPrivate::AdwaitaColor>(metaEnum.keyToValue(name.toLatin1()));

    return (value <= ColorsPrivate::invalid_color || value > ColorsPrivate::alt_focus_border_color) ? ColorsPrivate::invalid_color : value;
}

static ColorsPrivate::AdwaitaWidgetColor widgetColorNameToEnum(const QString &name)
{
    Q_ASSERT(!name.isEmpty());

    QMetaObject metaObject = ColorsPrivate::staticMetaObject;

    QMetaEnum metaEnum = metaObject.enumerator(metaObject.indexOfEnumerator("AdwaitaWidgetColor"));
    ColorsPrivate::AdwaitaWidgetColor value = static_cast<ColorsPrivate::AdwaitaWidgetColor>(metaEnum.keyToValue(name.toLatin1()));

    return (value <= ColorsPrivate::invalid_widget_color || value > ColorsPrivate::checkradio_checked_disabled_color) ? ColorsPrivate::invalid_widget_color : value;
}

static QString enumToWidgetColorName(ColorsPrivate::AdwaitaWidgetColor color)
{
    Q_ASSERT(color != ColorsPrivate::invalid_widget_color);

    QMetaObject metaObject = ColorsPrivate::staticMetaObject;

    QMetaEnum metaEnum = metaObject.enumerator(metaObject.indexOfEnumerator("AdwaitaWidgetColor"));
    const QString value = metaEnum.valueToKey(color);

    return value;
}

static QColor colorFromText(const QString &name)
{
    if (name == QStringLiteral("white")) {
        return Qt::white;
    } else if (name == QStringLiteral("black")) {
        return Qt::black;
    } else if (name.startsWith(QStringLiteral("rgba"))) {
        const QRegularExpression reRgba("^rgba\\((\\d+)[,|\\ ]+(\\d+)[,|\\ ]+(\\d+)[,|\\ ]+([\\d|\\.]+)\\)$");
        const QRegularExpressionMatch reMatch = reRgba.match(name);

        if (reMatch.hasMatch()) {
            // Process captured color
            const int r = reMatch.captured(1).toInt();
            const int g = reMatch.captured(2).toInt();
            const int b = reMatch.captured(3).toInt();
            const double a = reMatch.captured(4).toDouble();
            return QColor(r, g, b, 255 * a);
        }
    } else if (name.startsWith(QStringLiteral("image"))) {
        const QRegularExpression reRgba("^image\\((.*)\\)$");
        const QRegularExpressionMatch reMatch = reRgba.match(name);

        if (reMatch.hasMatch()) {
            return colorFromText(reMatch.captured(1));
        }
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
    } else if (options.animationMode() == AnimationHover || options.mouseOver()) {
         result += QStringLiteral("_hover");
    }

    return result;
}

static QString checkRadioColorSuffixFromOptions(const StyleOptions &options)
{
    bool isDisabled = options.palette().currentColorGroup() == QPalette::Disabled;
    bool isInactive = options.palette().currentColorGroup() == QPalette::Inactive;
    QString result;

    // Checked button
    if (options.checkboxState() != CheckOff || options.radioButtonState() != RadioOff) {
        result += QStringLiteral("_checked");
    }

    if (isDisabled) {
        result += QStringLiteral("_disabled");
        return result;
    }

    if (options.animationMode() == AnimationPressed || options.sunken()) {
        result += QStringLiteral("_active");
    } else if (options.animationMode() == AnimationHover || options.mouseOver()) {
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
            QString line = in.readLine();

            if (line.isEmpty()) {
                continue;
            }

            if (line.startsWith(QStringLiteral("@define-color"))) {
                // @define-color color_name #ffffff;
                // @define-color color_name rgba(255, 255, 255, 0.1);
                const QRegularExpression reBase("^@define\\-color\\ ([a-z|_]+)\\ (.*);$");
                const QRegularExpressionMatch reMatch = reBase.match(line);

                const AdwaitaColor colorEnum = colorNameToEnum(reMatch.captured(1));
                const QColor color = colorFromText(reMatch.captured(2));

                auto map = m_colors.value(colorEnum);
                map.insert(colorVariant, color);
                m_colors.insert(colorEnum, map);
            } else {
                const QRegularExpression re("^(.*)\\ \\{(.*);\\ \\}");
                const QRegularExpressionMatch reMatch = re.match(line);

                const QString widgetColorName = reMatch.captured(1).replace(QLatin1Char(':'), QLatin1Char('_'));
                const QStringList properties = reMatch.captured(2).split(QLatin1Char(';'));

                for (const QString &property : properties) {
                    const QRegularExpression reBase("^(.*):\\ (.*)");
                    const QRegularExpressionMatch reMatch = reBase.match(property.trimmed());

                    if (reMatch.hasMatch()) {
                        const QString propertyName = reMatch.captured(1);
                        const QString propertyValue = reMatch.captured(2);

                        QString colorType;
                        if (propertyName == QStringLiteral("background-image") ||
                            propertyName == QStringLiteral("border-color") ||
                            propertyName == QStringLiteral("color")) {
                            const QString fullButtonColorName = QStringLiteral("%1_%2").arg(widgetColorName).arg(propertyName).replace(QLatin1Char('-'), QLatin1Char('_'));

                            // Special case with special treatment
                            if (propertyValue.startsWith(QStringLiteral("linear-gradient"))) {
                                // linear-gradient(to bottom, #5d9de9 10%, #478fe6 90%)
                                // linear-gradient(to top, #f6f5f4 2px, #fbfafa)
                                const QRegularExpression reBase("^linear-gradient\\((.[^,]*),\\ (#[a-z|0-9]{6}|white|black).*,\\ (#[a-z|0-9]{6}|white|black).*\\)$");
                                const QRegularExpressionMatch reMatch = reBase.match(propertyValue);

                                if (reMatch.hasMatch()) {
                                    const QColor firstColor = colorFromText(reMatch.captured(2));
                                    const QColor secondColor = colorFromText(reMatch.captured(3));
                                    const bool fromBottom = reMatch.captured(1) == QStringLiteral("to top");

                                    const QString gradientStart = QStringLiteral("%1_%2").arg(widgetColorName).arg(QStringLiteral("gradient_start"));
                                    const QString gradientStop = QStringLiteral("%1_%2").arg(widgetColorName).arg(QStringLiteral("gradient_stop"));

                                    const AdwaitaWidgetColor gradientStartEnum = widgetColorNameToEnum(gradientStart);
                                    const AdwaitaWidgetColor gradientStopEnum = widgetColorNameToEnum(gradientStop);

                                    auto gradientStartMap = m_widgetColors.value(gradientStartEnum);
                                    auto gradientStopMap = m_widgetColors.value(gradientStopEnum);

                                    gradientStartMap.insert(colorVariant, fromBottom ? firstColor : secondColor);
                                    gradientStopMap.insert(colorVariant, fromBottom ? secondColor : firstColor);

                                    m_widgetColors.insert(gradientStartEnum, gradientStartMap);
                                    m_widgetColors.insert(gradientStopEnum, gradientStopMap);
                                 } else {
                                    // qCDebug(ADWAITA) << "Unhandled property value" << propertyValue << " for " << propertyName << " in " << fullButtonColorName;
                                 }
                            } else {
                                const AdwaitaWidgetColor colorEnum = widgetColorNameToEnum(fullButtonColorName);
                                const QColor color = colorFromText(propertyValue);

                                auto map = m_widgetColors.value(colorEnum);
                                map.insert(colorVariant, color);
                                m_widgetColors.insert(colorEnum, map);
                            }

                        } else {
                            // qCDebug(ADWAITA) << "Unhandled property " << propertyName << " in " << line;
                        }
                    }
                }
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

QColor ColorsPrivate::adwaitaWidgetColor(AdwaitaWidgetColor color, ColorVariant variant)
{
    if (color == invalid_widget_color) {
        return QColor();
    }

    QColor returnValue = m_widgetColors.value(color).value(variant);

    if (!returnValue.isValid()) {
        QString colorName = enumToWidgetColorName(color);

        if (colorName.isEmpty()) {
            return QColor();
        }

        // We can only fallback to gradient in case of background_image or base color of any kind
        if (!colorName.endsWith(QStringLiteral("background_image")) && !colorName.endsWith(QStringLiteral("color"))) {
            return QColor();
        }

        // Requested color might use gradient
        QString gradientColorName = colorName;
        AdwaitaWidgetColor gradientColor = widgetColorNameToEnum(gradientColorName.replace(QStringLiteral("background_image"), QStringLiteral("gradient_start")));
        if (gradientColor != invalid_widget_color) {
            returnValue = m_widgetColors.value(gradientColor).value(variant);

            if (returnValue.isValid()) {
                return returnValue;
            }
        }

        // Derived state doesn't override all colors
        // E.g checkradio_checked_hover_color → checkradio_checked_color
        QString baseColorName = colorName;
        baseColorName.replace(QStringLiteral("_active"), QString()).replace(QStringLiteral("_hover"), QString()).replace(QStringLiteral("_disabled"), QString());

        // There is no base color we can try
        if (colorName == baseColorName) {
            return QColor();
        }

        AdwaitaWidgetColor baseColor = widgetColorNameToEnum(baseColorName);
        if (baseColor != invalid_widget_color) {
            // Recursive call to also cover gradient in case the base color uses one
            return adwaitaWidgetColor(baseColor, variant);
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

    QColor buttonColor = colorsGlobal->adwaitaWidgetColor(ColorsPrivate::button_background_image, variant);
    QColor disabledButtonColor = colorsGlobal->adwaitaWidgetColor(ColorsPrivate::button_backdrop_background_image, variant);

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
    return colorsGlobal->adwaitaColor(ColorsPrivate::focus_border_color, options.colorVariant());
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
    const QString colorName = QStringLiteral("button") + buttonColorSuffixFromOptions(options) + QStringLiteral("_border_color");
    return colorsGlobal->adwaitaWidgetColor(widgetColorNameToEnum(colorName), options.colorVariant());
}

QColor Colors::indicatorOutlineColor(const StyleOptions &options)
{
    bool isDisabled = options.palette().currentColorGroup() == QPalette::Disabled;
    if (options.inMenu()) {
        if (isDisabled) {
            return buttonOutlineColor(options);
        }

        if (options.colorVariant() == ColorVariant::AdwaitaDark || options.colorVariant() == AdwaitaHighcontrastDark) {
            return darken(options.palette().color(QPalette::Window), 0.18);
        } else {
            return darken(options.palette().color(QPalette::Window), 0.24);
        }
    } else {
        const QString colorName = QStringLiteral("checkradio") + checkRadioColorSuffixFromOptions(options) + QStringLiteral("_border_color");
        return colorsGlobal->adwaitaWidgetColor(widgetColorNameToEnum(colorName), options.colorVariant());
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

    if (isDisabled && (options.animationMode() == AnimationPressed || options.sunken())) {
        return colorsGlobal->adwaitaWidgetColor(ColorsPrivate::button_disabled_active_background_image, options.colorVariant());
    }

    if (options.animationMode() == AnimationPressed) {
        const ColorsPrivate::AdwaitaWidgetColor buttonColor = options.sunken() ? ColorsPrivate::button_checked_hover_background_image : ColorsPrivate::button_hover_background_image;
        const ColorsPrivate::AdwaitaWidgetColor buttonHoverColor = options.sunken() ? ColorsPrivate::button_checked_active_background_image : ColorsPrivate::button_checked_background_image;
        return Colors::mix(colorsGlobal->adwaitaWidgetColor(buttonColor, options.colorVariant()),
                           colorsGlobal->adwaitaWidgetColor(buttonHoverColor, options.colorVariant()), options.opacity());
    } else if (options.animationMode() == AnimationHover) {
        const ColorsPrivate::AdwaitaWidgetColor buttonColor = options.sunken() ? ColorsPrivate::button_checked_background_image : ColorsPrivate::button_background_image;
        const ColorsPrivate::AdwaitaWidgetColor buttonHoverColor = options.sunken() ? ColorsPrivate::button_checked_hover_background_image : ColorsPrivate::button_hover_background_image;
        return Colors::mix(colorsGlobal->adwaitaWidgetColor(buttonColor, options.colorVariant()),
                           colorsGlobal->adwaitaWidgetColor(buttonHoverColor, options.colorVariant()), options.opacity());
    }

    const QString colorName = QStringLiteral("button") + buttonColorSuffixFromOptions(options) + QStringLiteral("_background_image");
    return colorsGlobal->adwaitaWidgetColor(widgetColorNameToEnum(colorName), options.colorVariant());
}

QLinearGradient Colors::buttonBackgroundGradient(const StyleOptions &options)
{
    const QString gradientName = QStringLiteral("button") + buttonColorSuffixFromOptions(options) + QStringLiteral("_gradient_stop");
    QColor gradientStartColor = buttonBackgroundColor(options);
    QColor gradientStopColor = colorsGlobal->adwaitaWidgetColor(widgetColorNameToEnum(gradientName), options.colorVariant());

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
    if (options.inMenu()) {
        return Qt::transparent;
    }

    if (options.animationMode() == AnimationPressed) {
        const ColorsPrivate::AdwaitaWidgetColor checkradioColor = options.checkboxState() == CheckOff && options.radioButtonState() == RadioOff ? ColorsPrivate::checkradio_hover_background_image : ColorsPrivate::checkradio_checked_hover_background_image;
        const ColorsPrivate::AdwaitaWidgetColor checkradioHoverColor = options.checkboxState() == CheckOff && options.radioButtonState() == RadioOff ? ColorsPrivate::checkradio_checked_background_image : ColorsPrivate::checkradio_checked_active_background_image;
        return Colors::mix(colorsGlobal->adwaitaWidgetColor(checkradioColor, options.colorVariant()),
                           colorsGlobal->adwaitaWidgetColor(checkradioHoverColor, options.colorVariant()), options.opacity());
    } else if (options.animationMode() == AnimationHover) {
        const ColorsPrivate::AdwaitaWidgetColor checkradioColor = options.checkboxState() == CheckOff && options.radioButtonState() == RadioOff ? ColorsPrivate::checkradio_background_image : ColorsPrivate::checkradio_checked_background_image;
        const ColorsPrivate::AdwaitaWidgetColor checkradioHoverColor = options.checkboxState() == CheckOff && options.radioButtonState() == RadioOff ? ColorsPrivate::checkradio_hover_background_image : ColorsPrivate::checkradio_checked_hover_background_image;
        return Colors::mix(colorsGlobal->adwaitaWidgetColor(checkradioColor, options.colorVariant()),
                           colorsGlobal->adwaitaWidgetColor(checkradioHoverColor, options.colorVariant()), options.opacity());
    }

    const QString colorName = QStringLiteral("checkradio") + checkRadioColorSuffixFromOptions(options) + QStringLiteral("_background_image");
    return colorsGlobal->adwaitaWidgetColor(widgetColorNameToEnum(colorName), options.colorVariant());
}

QLinearGradient Colors::indicatorBackgroundGradient(const StyleOptions &options)
{
    const QString gradientName = QStringLiteral("checkradio") + checkRadioColorSuffixFromOptions(options) + QStringLiteral("_gradient_stop");
    QColor gradientStartColor = indicatorBackgroundColor(options);
    QColor gradientStopColor = colorsGlobal->adwaitaWidgetColor(widgetColorNameToEnum(gradientName), options.colorVariant());

    QLinearGradient gradient(options.rect().bottomLeft(), options.rect().topLeft());
    gradient.setColorAt(0, gradientStartColor);
    gradient.setColorAt(1, gradientStopColor.isValid() && !options.inMenu() ? gradientStopColor : gradientStartColor);

    return gradient;
}

QColor Colors::frameBackgroundColor(const StyleOptions &options)
{
    return Colors::mix(options.palette().color(options.colorGroup(), QPalette::Window), options.palette().color(options.colorGroup(), QPalette::Base), 0.3);
}

QColor Colors::scrollBarHandleColor(const StyleOptions &options)
{
    QColor fgColor = options.palette().color(QPalette::Text);
    QColor bgColor = options.palette().color(QPalette::Window);
    QColor selectedBgColor = options.palette().color(QPalette::Highlight);

    QColor color(Colors::mix(fgColor, bgColor, 0.4));
    QColor hoverColor(Colors::mix(fgColor, bgColor, 0.2));
    QColor activeColor(options.colorVariant() == ColorVariant::AdwaitaDark ? Colors::lighten(selectedBgColor, 0.1) : Colors::darken(selectedBgColor, 0.1));

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
