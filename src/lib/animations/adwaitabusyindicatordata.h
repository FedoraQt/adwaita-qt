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

#ifndef ADWAITA_BUSY_INDICATOR_DATA_
#define ADWAITA_BUSY_INDICATOR_DATA_

#include "adwaitaqt_export.h"

#include <QObject>

namespace Adwaita
{
class ADWAITAQT_EXPORT BusyIndicatorData : public QObject
{
    Q_OBJECT
public:
    //* constructor
    explicit BusyIndicatorData(QObject *parent)
        : QObject(parent)
        , _animated(false)
    {
    }

    //* destructor
    virtual ~BusyIndicatorData()
    {
    }

    //*@name accessors
    //@{

    //* animated
    bool isAnimated() const
    {
        return _animated;
    }

    //@}

    //*@name modifiers
    //@{

    //* enabled
    void setEnabled(bool)
    {
    }

    //* enabled
    void setDuration(int)
    {
    }

    //* animated
    void setAnimated(bool value)
    {
        _animated = value;
    }

    //@}

private:
    //* animated
    bool _animated;
};

} // namespace Adwaita

#endif // ADWAITA_BUSY_INDICATOR_DATA_
