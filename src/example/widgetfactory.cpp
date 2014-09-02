/*
 * Widget Showcase
 * Copyright (C) 2014 Martin Bříza <mbriza@redhat.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 */

#include <QtGui>
#include <QLayout>

#include "widgetfactory.h"
#include "widgetshowcase.h"

WidgetFactory::WidgetFactory(QWidget *parent)
    : QMainWindow(parent)
{
    setGeometry(0, 0, 1024, 768);

    setMenuBar(new QMenuBar);
    QMenu *file = new QMenu("File");
    file->addAction(new QAction("Load", file));
    QMenu *save = new QMenu("Save");
    save->addAction(new QAction("Save as", save));
    save->addAction(new QAction("Save", save));
    file->addMenu(save);
    file->addAction(new QAction("Quit", file));
    menuBar()->insertMenu(nullptr, file);
    menuBar()->insertMenu(nullptr, new QMenu("Edit"));
    menuBar()->insertMenu(nullptr, new QMenu("View"));
    menuBar()->insertMenu(nullptr, new QMenu("Help"));

    QToolBar *toolbar = new QToolBar();
    toolbar->setMovable(false);
    toolbar->setFloatable(false);
    toolbar->addAction(QIcon::fromTheme("document-save"), "");
    toolbar->addAction(QIcon::fromTheme("document-open"), "");
    toolbar->addSeparator();
    toolbar->addAction(QIcon::fromTheme("edit-find"), "");
    QWidget *spacer = new QWidget;
    spacer->setPalette(QPalette(Qt::transparent));
    spacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    toolbar->addWidget(spacer);
    toolbar->addWidget(new QLineEdit("search..."));
    addToolBar(Qt::TopToolBarArea, toolbar);

    setCentralWidget(new WidgetShowcase(this));

    setWindowTitle(tr("Qt/GTK+ Widget Factory"));
    QApplication::setStyle("adwaita");
}
