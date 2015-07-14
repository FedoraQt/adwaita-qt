/*
 * Copyright (C) 2014  Martin Bříza <mbriza@redhat.com>
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
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 *
 */

#include "styleplugin.h"
#include "adwaita.h"

StylePlugin::StylePlugin(QObject* parent)
        : QStylePlugin(parent) {
}

QStyle* StylePlugin::create(const QString& key) {
    if (key.toLower() == "adwaita")
        return new Adwaita;
    return nullptr;
}

QStringList StylePlugin::keys() const {
    return QStringList() << "adwaita";
}

#if QT_VERSION < 0x050000
    Q_EXPORT_PLUGIN2(adwaita, StylePlugin);
#endif
