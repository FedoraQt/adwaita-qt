/*
 * Widget Showcase
 * Copyright (C) 2014-2018 Martin Bříza <mbriza@redhat.com>
 * Copyright (C) 2019 Jan Grulich <jgrulich@redhat.com>
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

#include <QLayout>

#include "widgetfactory.h"
#include "ui_widgetfactory.h"

#include <QAction>
#include <QLineEdit>
#include <QMenu>
#include <QMenuBar>
#include <QToolBar>

class WidgetFactory::Private
{
public:
    Private() { }
    Ui_WidgetFactory ui;
};

WidgetFactory::WidgetFactory(QWidget *parent)
    : QMainWindow(parent)
    , d(new WidgetFactory::Private())
{
    setGeometry(0, 0, 1440, 800);

    QWidget *mainWidget = new QWidget(this);
    d->ui.setupUi(mainWidget);

    setCentralWidget(mainWidget);

    // QLineEdit with the icon inside
    QAction *lineEditAction = new QAction(QIcon::fromTheme(QLatin1String("view-refresh")), QString());
    connect(lineEditAction, &QAction::triggered, this, [this] (bool checked) {
        if (d->ui.linedit->echoMode() == QLineEdit::Normal) {
            d->ui.linedit->setEchoMode(QLineEdit::Password);
        } else {
            d->ui.linedit->setEchoMode(QLineEdit::Normal);
        }
    });
    d->ui.linedit->addAction(lineEditAction, QLineEdit::TrailingPosition);

    // QLineEdit with the widget on the right side
    connect(d->ui.button, &QPushButton::pressed, this, [this] () {
        d->ui.lineedit2->clear();
    });

    // Checkboxes
    d->ui.checkbox3->setCheckState(Qt::PartiallyChecked);
    d->ui.checkbox6->setCheckState(Qt::PartiallyChecked);

    // Buttons
    d->ui.pushbutton3->setDown(true);
    d->ui.pushbutton4->setDown(true);

    // Sliders and progress bars
    connect(d->ui.horizontalslider3, &QSlider::valueChanged, this, [this] (int value) {
        d->ui.progressbar_horizontal->setValue(value);
        d->ui.progressbar_horizontal2->setValue(value);
        d->ui.progressbar_vertical->setValue(value);
        d->ui.progressbar_vertical2->setValue(value);
    });

    connect(d->ui.horizontalslider, &QSlider::valueChanged, this, [this] (int value) {
        d->ui.horizontalslider2->setValue(value);
        d->ui.verticalslider->setValue(value);
        d->ui.verticalslider2->setValue(value);
    });
    connect(d->ui.verticalslider, &QSlider::valueChanged, this, [this] (int value) {
        d->ui.horizontalslider->setValue(value);
        d->ui.horizontalslider2->setValue(value);
        d->ui.verticalslider2->setValue(value);
    });

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

    setWindowTitle(tr("Qt/GTK+ Widget Factory"));
}

WidgetFactory::~WidgetFactory()
{
    delete d;
}
