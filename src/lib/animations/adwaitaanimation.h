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

#ifndef ADWAITA_ANIMATION_H
#define ADWAITA_ANIMATION_H

#include "adwaita.h"
#include "adwaitaqt_export.h"

#include <QPropertyAnimation>
#include <QVariant>

namespace Adwaita
{
class ADWAITAQT_EXPORT Animation : public QPropertyAnimation
{
    Q_OBJECT
public:
    //* convenience
    using Pointer = WeakPointer<Animation>;

    //* constructor
    Animation(int duration, QObject *parent)
        : QPropertyAnimation(parent)
    {
        setDuration(duration);
    }

    //* destructor
    virtual ~Animation() = default;

    //* true if running
    bool isRunning() const
    {
        return state() == Animation::Running;
    }

    //* restart
    void restart()
    {
        if (isRunning()) {
            stop();
        }

        start();
    }
};

} // namespace Adwaita

#endif // ADWAITA_ANIMATION_H
