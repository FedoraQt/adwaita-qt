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
#include <QActionGroup>
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

    QMenuBar *menubar = new QMenuBar(this);
    setMenuBar(menubar);
    // Inspired by Kondike (a gtk game)
    QMenu *game = new QMenu("Game", menubar);
    QAction *newGame = new QAction("New game");
    newGame->setShortcut(QKeySequence::New);
    game->addAction(newGame);
    QAction *restart = new QAction("Restart");
    restart->setDisabled(true);
    game->addAction(restart);
    QAction *statistics = new QAction("Statistics");
    game->addAction(statistics);
    QAction *selectGame = new QAction("Select game...");
    selectGame->setShortcut(QKeySequence::Open);
    game->addAction(selectGame);
    QMenu *recentlyPlayed = new QMenu("Recently played");
    recentlyPlayed->addAction(new QAction("Game 1", recentlyPlayed));
    recentlyPlayed->addAction(new QAction("Game 1", recentlyPlayed));
    game->addMenu(recentlyPlayed);
    game->addSeparator();
    QAction *close = new QAction("Close");
    close->setShortcut(QKeySequence::Quit);
    game->addAction(close);
    menuBar()->insertMenu(nullptr, game);

    QMenu *edit = new QMenu("Edit", menubar);
    QAction *act1 = new QAction("Pick me");
    act1->setCheckable(true);
    act1->setChecked(true);
    QAction *act2 = new QAction("No, pick me instead");
    act2->setCheckable(true);
    QAction *act3 = new QAction("Don't pick me");
    act3->setCheckable(true);
    act3->setDisabled(true);
    edit->addAction(act1);
    edit->addAction(act2);
    edit->addAction(act3);
    QActionGroup *actionGroup = new QActionGroup(edit);
    actionGroup->setExclusive(true);
    actionGroup->addAction(act1);
    actionGroup->addAction(act2);
    actionGroup->addAction(act3);
    menuBar()->insertMenu(nullptr, edit);

    menuBar()->insertMenu(nullptr, new QMenu("View", menubar));
    QAction *testAction = new QAction(QStringLiteral("Test 1"));
    testAction->setCheckable(true);

    edit->addAction(testAction);
    menuBar()->insertMenu(nullptr, edit);
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
