
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

#include "adwaitastyleplugin.h"
#include "adwaitastyle.h"
#include "adwaita.h"

#include <QApplication>

namespace Adwaita
{

QStyle *StylePlugin::create(const QString &key)
{
    if (key.toLower() == QStringLiteral("adwaita-auto")) {
        // Let this be autodetected
        return new Style();
    }

    if (key.toLower() == QStringLiteral("adwaita")) {
        return new Style(Adwaita::Adwaita);
    }

    if (key.toLower() == QStringLiteral("adwaita-dark")) {
        return new Style(Adwaita::AdwaitaDark);
    }

    if (key.toLower() == QStringLiteral("adwaita-highcontrast") ||
        key.toLower() == QStringLiteral("highcontrast")) {
        return new Style(Adwaita::AdwaitaHighcontrast);
    }

    if (key.toLower() == QStringLiteral("adwaita-highcontrastinverse") ||
        key.toLower() == QStringLiteral("highcontrastinverse")) {
        return new Style(Adwaita::AdwaitaHighcontrastInverse);
    }

    return nullptr;
}

StylePlugin::~StylePlugin()
{
}

QStringList StylePlugin::keys() const
{
    return QStringList() << QStringLiteral("Adwaita") << QStringLiteral("Adwaita-Dark") << QStringLiteral("Adwaita-Light")
                         << QStringLiteral("Adwaita-HighContrast") << QStringLiteral("Adwaita-HighContrastInverse")
                         << QStringLiteral("Adwaita-Auto");
}

} // namespace Adwaita
