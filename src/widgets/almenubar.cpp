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

CALMenu* CALMenuBar::addMenu(const std::shared_ptr<CALIconType>& icon_type, const QString& title) {
	const auto menu = new CALMenu(title, this);
	menu->setMenuItemHeight(27);
	menu->menuAction()->setProperty(ALIcon::iconProperty, QChar(icon_type->value()));
	menu->menuAction()->setProperty(ALIcon::iconTypeProperty, icon_type->iconType());
	if (icon_type->iconType() == ALIcon::IconType::Awesome) {
		menu->menuAction()->setIcon(CALIcon::getQIconFromALIcon(ALIcon::AweSomeIcon::Broom, 1));
	} else if (icon_type->iconType() == ALIcon::IconType::Fluent) {
		menu->menuAction()->setIcon(CALIcon::getQIconFromALIcon(ALIcon::FluentIcon::Broom, 1));
	}
	QMenuBar::addAction(menu->menuAction());
	return menu;
}

QAction* CALMenuBar::addALIconAction(const std::shared_ptr<CALIconType>& icon_type) {
	const auto action = new QAction(this);
	action->setProperty(ALIcon::iconProperty, QChar(icon_type->value()));
	action->setProperty(ALIcon::iconTypeProperty, icon_type->iconType());
	if (icon_type->iconType() == ALIcon::IconType::Awesome) {
		action->setIcon(CALIcon::getQIconFromALIcon(ALIcon::AweSomeIcon::Broom, 1));
	} else if (icon_type->iconType() == ALIcon::IconType::Fluent) {
		action->setIcon(CALIcon::getQIconFromALIcon(ALIcon::FluentIcon::Broom, 1));
	}
	QMenuBar::addAction(action);
	return action;
}

QAction* CALMenuBar::addALIconAction(const std::shared_ptr<CALIconType>& icon_type, const QString& text) {
	const auto action = new QAction(text, this);
	action->setProperty(ALIcon::iconProperty, QChar(icon_type->value()));
	action->setProperty(ALIcon::iconTypeProperty, icon_type->iconType());
	if (icon_type->iconType() == ALIcon::IconType::Awesome) {
		action->setIcon(CALIcon::getQIconFromALIcon(ALIcon::AweSomeIcon::Broom, 1));
	} else if (icon_type->iconType() == ALIcon::IconType::Fluent) {
		action->setIcon(CALIcon::getQIconFromALIcon(ALIcon::FluentIcon::Broom, 1));
	}
	QMenuBar::addAction(action);
	return action;
}

QAction* CALMenuBar::addALIconAction(const std::shared_ptr<CALIconType>& icon_type, const QString& text, const QKeySequence& shortcut) {
	const auto action = new QAction(text, this);
	action->setShortcut(shortcut);
	action->setProperty(ALIcon::iconProperty, QChar(icon_type->value()));
	action->setProperty(ALIcon::iconTypeProperty, icon_type->iconType());
	if (icon_type->iconType() == ALIcon::IconType::Awesome) {
		action->setIcon(CALIcon::getQIconFromALIcon(ALIcon::AweSomeIcon::Broom, 1));
	} else if (icon_type->iconType() == ALIcon::IconType::Fluent) {
		action->setIcon(CALIcon::getQIconFromALIcon(ALIcon::FluentIcon::Broom, 1));
	}
	QMenuBar::addAction(action);
	return action;
}
}
