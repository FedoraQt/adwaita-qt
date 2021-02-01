/*************************************************************************
 * Copyright (C) 2014 by Hugo Pereira Da Costa <hugo.pereira@free.fr>    *
 * Copyright (C) 2014-2018 Martin Bříza <m@rtinbriza.cz>                 *
 * Copyright (C) 2019-2020 Jan Grulich <jgrulich@redhat.com>             *
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
#include "adwaitacolors.h"
#include "adwaitahelper.h"

#include <QApplication>
#include <QPainter>
#include <QLibrary>

#if ADWAITA_HAVE_X11
#include <X11/Xlib-xcb.h>
#endif

namespace Adwaita
{

Helper::Helper()
{
    init();
}

// static
bool Helper::isWindowActive(const QWidget *widget)
{
    const QWindow *win = widget ? widget->window()->windowHandle() : nullptr;
    if (win) {
        return win->isActive();
    }
    return false;
}

bool Helper::isX11()
{
    static const bool s_isX11 = qApp->platformName() == QLatin1String("xcb");
    return s_isX11;
}

bool Helper::isWayland()
{
    static const bool s_isWayland = qApp->platformName().startsWith(QLatin1String("wayland"));
    return s_isWayland;
}

bool Helper::compositingActive(void) const
{
#if ADWAITA_HAVE_X11
    if (isX11()) {
        // direct call to X
        xcb_get_selection_owner_cookie_t cookie(xcb_get_selection_owner(connection(), _compositingManagerAtom));
        ScopedPointer<xcb_get_selection_owner_reply_t> reply(xcb_get_selection_owner_reply(connection(), cookie, nullptr));
        return reply && reply->owner;

    }
#endif

    // use KWindowSystem
    //return KWindowSystem::compositingActive();
    return false;
}

bool Helper::hasAlphaChannel(const QWidget *widget) const
{
    return compositingActive() && widget && widget->testAttribute(Qt::WA_TranslucentBackground);
}

QPixmap Helper::highDpiPixmap(int width, int height) const
{
    qreal dpiRatio(qApp->devicePixelRatio());
    QPixmap pixmap(width * dpiRatio, height * dpiRatio);
    pixmap.setDevicePixelRatio(dpiRatio);
    return pixmap;
}

qreal Helper::devicePixelRatio(const QPixmap &pixmap) const
{
    Q_UNUSED(pixmap);
    return 1;
}

#if ADWAITA_HAVE_X11

xcb_connection_t *Helper::connection()
{

    return QX11Info::connection();
}

xcb_atom_t Helper::createAtom(const QString &name) const
{
    if (isX11()) {
        xcb_connection_t *connection(Helper::connection());
        xcb_intern_atom_cookie_t cookie(xcb_intern_atom(connection, false, name.size(), qPrintable(name)));
        ScopedPointer<xcb_intern_atom_reply_t> reply(xcb_intern_atom_reply(connection, cookie, nullptr));
        return reply ? reply->atom : 0;
    } else
        return 0;
}

#endif

void Helper::init()
{
#if ADWAITA_HAVE_X11
    if (isX11()) {
        // create compositing screen
        QString atomName(QStringLiteral("_NET_WM_CM_S%1").arg(QX11Info::appScreen()));
        _compositingManagerAtom = createAtom(atomName);
    }
#endif
}

void Helper::setVariant(QWidget *widget, const QByteArray &variant)
{
#if ADWAITA_HAVE_X11
    if (isX11() && widget) { //  && !widget->testAttribute(Qt::WA_)) {
        static const char *_GTK_THEME_VARIANT = "_GTK_THEME_VARIANT";

        // Check if already set
        QVariant var = widget->property("_GTK_THEME_VARIANT");
        if (var.isValid() && var.toByteArray() == variant) {
            return;
        }

        // Typedef's from xcb/xcb.h - copied so that there is no
        // direct xcb dependency
        typedef quint32 XcbAtom;

        struct XcbInternAtomCookie {
            unsigned int sequence;
        };

        struct XcbInternAtomReply {
            quint8  response_type;
            quint8  pad0;
            quint16 sequence;
            quint32 length;
            XcbAtom atom;
        };

        struct XcbVoidCookie {
            unsigned int sequence;
        };

        typedef void *(*XcbConnectFn)(const char *, int *);
        typedef XcbInternAtomCookie(*XcbInternAtomFn)(void *, quint8, quint16, const char *);
        typedef XcbInternAtomReply * (*XcbInternAtomReplyFn)(void *, XcbInternAtomCookie, void *);
        typedef XcbVoidCookie(*XcbChangePropertyFn)(void *, quint8, quint32, XcbAtom, XcbAtom, quint8, quint32, const void *);
        typedef int (*XcbFlushFn)(void *);

        static QLibrary *lib = 0;
        static XcbAtom variantAtom = 0;
        static XcbAtom utf8TypeAtom = 0;
        static void *xcbConn = 0;
        static XcbChangePropertyFn XcbChangePropertyFnPtr = 0;
        static XcbFlushFn XcbFlushFnPtr = 0;

        if (!lib) {
            lib = new QLibrary("libxcb", qApp);

            if (lib->load()) {
                XcbConnectFn XcbConnectFnPtr = (XcbConnectFn)lib->resolve("xcb_connect");
                XcbInternAtomFn XcbInternAtomFnPtr = (XcbInternAtomFn)lib->resolve("xcb_intern_atom");
                XcbInternAtomReplyFn XcbInternAtomReplyFnPtr = (XcbInternAtomReplyFn)lib->resolve("xcb_intern_atom_reply");

                XcbChangePropertyFnPtr = (XcbChangePropertyFn)lib->resolve("xcb_change_property");
                XcbFlushFnPtr = (XcbFlushFn)lib->resolve("xcb_flush");
                if (XcbConnectFnPtr && XcbInternAtomFnPtr && XcbInternAtomReplyFnPtr && XcbChangePropertyFnPtr && XcbFlushFnPtr) {
                    xcbConn = (*XcbConnectFnPtr)(0, 0);
                    if (xcbConn) {
                        XcbInternAtomReply *typeReply = (*XcbInternAtomReplyFnPtr)(xcbConn, (*XcbInternAtomFnPtr)(xcbConn, 0, 11, "UTF8_STRING"), 0);

                        if (typeReply) {
                            XcbInternAtomReply *gtkVarReply = (*XcbInternAtomReplyFnPtr)(xcbConn,
                                                              (*XcbInternAtomFnPtr)(xcbConn, 0, strlen(_GTK_THEME_VARIANT),
                                                                      _GTK_THEME_VARIANT), 0);
                            if (gtkVarReply) {
                                utf8TypeAtom = typeReply->atom;
                                variantAtom = gtkVarReply->atom;
                                free(gtkVarReply);
                            }
                            free(typeReply);
                        }
                    }
                }
            }
        }

        if (0 != variantAtom) {
            (*XcbChangePropertyFnPtr)(xcbConn, 0, widget->effectiveWinId(), variantAtom, utf8TypeAtom, 8,
                                      variant.length(), (const void *)variant.constData());
            (*XcbFlushFnPtr)(xcbConn);
            widget->setProperty(_GTK_THEME_VARIANT, variant);
        }
    }
#endif
}

}
