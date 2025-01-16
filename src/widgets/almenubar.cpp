#include "almenubar.hpp"

#include <QToolButton>

#include "alicon.hpp"
#include "almenu.hpp"
#include "almenubarstyle.hpp"

/**
 * @brief \namespace AL
 */
namespace AL {
/**
 * \class CALMenuBar
 * @param parent pointer to the parent class
 */
CALMenuBar::CALMenuBar(QWidget* parent): QMenuBar(parent) {
	setMouseTracking(true);
	setObjectName("CALMenuBar");
	setStyle(new CALMenuBarStyle(style()));
	if (const auto toolbutton = this->findChild<QToolButton*>(); toolbutton->objectName() == "qt_menubar_ext_button") {
		QMenu* oldMenu = toolbutton->menu();
		const auto menu = new CALMenu(this);
		menu->setObjectName("CALExtendMenu");
		menu->setMenuItemHeight(27);
		if (oldMenu) {
			oldMenu->deleteLater();
		}
		toolbutton->setMenu(menu);
	}
}

CALMenuBar::~CALMenuBar() = default;

QAction* CALMenuBar::addMenu(QMenu* menu) {
	if (const auto calmenu = dynamic_cast<CALMenu*>(menu)) {
		calmenu->setMenuItemHeight(27);
	}
	return QMenuBar::addMenu(menu);
}

CALMenu* CALMenuBar::addMenu(const QString& title) {
	const auto menu = new CALMenu(title, this);
	menu->setMenuItemHeight(27);
	QMenuBar::addAction(menu->menuAction());
	return menu;
}

CALMenu* CALMenuBar::addMenu(const QIcon& icon, const QString& title) {
	const auto menu = new CALMenu(title, this);
	menu->setMenuItemHeight(27);
	menu->setIcon(icon);
	QMenuBar::addAction(menu->menuAction());
	return menu;
}

CALMenu* CALMenuBar::addMenu(const ALIcon::AweSomeIcon& awesomeicon, const QString& title) {
	const auto menu = new CALMenu(title, this);
	menu->setMenuItemHeight(27);
	menu->menuAction()->setProperty("CALIcon", QChar(static_cast<unsigned short>(awesomeicon)));
	menu->menuAction()->setProperty("CALIconType", ALIcon::Awesome);
	menu->menuAction()->setIcon(CALIcon::getQIconFromAwesomeIcon(ALIcon::AweSomeIcon::Broom, 1));
	QMenuBar::addAction(menu->menuAction());
	return menu;
}

CALMenu* CALMenuBar::addMenu(const ALIcon::FluentIcon& fluenticon, const QString& title) {
	const auto menu = new CALMenu(title, this);
	menu->setMenuItemHeight(27);
	menu->menuAction()->setProperty("CALIcon", QChar(static_cast<unsigned short>(fluenticon)));
	menu->menuAction()->setProperty("CALIconType", ALIcon::Fluent);
	menu->menuAction()->setIcon(CALIcon::getQIconFromFluentIcon(ALIcon::FluentIcon::Broom, 1));
	QMenuBar::addAction(menu->menuAction());
	return menu;
}

QAction* CALMenuBar::addAweSomeIconAction(const ALIcon::AweSomeIcon& awesomeicon) {
	const auto action = new QAction(this);
	action->setProperty("CALIcon", QChar(static_cast<unsigned short>(awesomeicon)));
	action->setProperty("CALIconType", ALIcon::Awesome);
	action->setIcon(CALIcon::getQIconFromAwesomeIcon(ALIcon::AweSomeIcon::Broom, 1));
	QMenuBar::addAction(action);
	return action;
}

QAction* CALMenuBar::addFluentIconAction(const ALIcon::FluentIcon& fluenticon) {
	const auto action = new QAction(this);
	action->setProperty("CALIcon", QChar(static_cast<unsigned short>(fluenticon)));
	action->setProperty("CALIconType", ALIcon::Fluent);
	action->setIcon(CALIcon::getQIconFromFluentIcon(ALIcon::FluentIcon::Broom, 1));
	QMenuBar::addAction(action);
	return action;
}

QAction* CALMenuBar::addAweSomeIconAction(const ALIcon::AweSomeIcon& awesomeicon, const QString& text) {
	const auto action = new QAction(text, this);
	action->setProperty("CALIcon", QChar(static_cast<unsigned short>(awesomeicon)));
	action->setProperty("CALIconType", ALIcon::Awesome);
	action->setIcon(CALIcon::getQIconFromAwesomeIcon(ALIcon::AweSomeIcon::Broom, 1));
	QMenuBar::addAction(action);
	return action;
}

QAction* CALMenuBar::addFluentIconAction(const ALIcon::FluentIcon& fluenticon, const QString& text) {
	const auto action = new QAction(text, this);
	action->setProperty("CALIcon", QChar(static_cast<unsigned short>(fluenticon)));
	action->setProperty("CALIconType", ALIcon::Fluent);
	action->setIcon(CALIcon::getQIconFromFluentIcon(ALIcon::FluentIcon::Broom, 1));
	QMenuBar::addAction(action);
	return action;
}

QAction* CALMenuBar::addAweSomeIconAction(const ALIcon::AweSomeIcon& awesomeicon, const QString& text, const QKeySequence& shortcut) {
	const auto action = new QAction(text, this);
	action->setShortcut(shortcut);
	action->setProperty("CALIcon", QChar(static_cast<unsigned short>(awesomeicon)));
	action->setProperty("CALIconType", ALIcon::Awesome);
	action->setIcon(CALIcon::getQIconFromAwesomeIcon(ALIcon::AweSomeIcon::Broom, 1));
	QMenuBar::addAction(action);
	return action;
}

QAction* CALMenuBar::addFluentIconAction(const ALIcon::FluentIcon& fluenticon, const QString& text, const QKeySequence& shortcut) {
	const auto action = new QAction(text, this);
	action->setShortcut(shortcut);
	action->setProperty("CALIcon", QChar(static_cast<unsigned short>(fluenticon)));
	action->setProperty("CALIconType", ALIcon::Fluent);
	action->setIcon(CALIcon::getQIconFromFluentIcon(ALIcon::FluentIcon::Broom, 1));
	QMenuBar::addAction(action);
	return action;
}
}
