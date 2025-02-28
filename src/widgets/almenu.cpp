#include "almenu.hpp"

#include <algorithm>
#include <QPainter>
#include <QPropertyAnimation>

#include "almenustyle.hpp"
#include "almenu_p.hpp"

/**
 * @brief \namespace AL
 */
namespace AL {
/**
 * @brief \class CALMenuPrivate
 * Internal class for CALMenu
 * @param q pointer to the public class
 * @param parent pointer to the parent class
 */
CALMenuPrivate::CALMenuPrivate(CALMenu* q, QObject* parent): QObject(parent), q_ptr(q) {
}

CALMenuPrivate::~CALMenuPrivate() = default;

void CALMenuPrivate::setAnimationImagePosY(const int pos) {
	animationImagePosY = pos;
	Q_EMIT sigAnimationImagePosYChanged();
}

int CALMenuPrivate::getAnimationImagePosY() const {
	return animationImagePosY;
}

/**
 * @brief \class CALMenu
 * @param parent pointer to the parent class
 */
CALMenu::CALMenu(QWidget* parent): QMenu(parent), d_ptr(new CALMenuPrivate(this, this)) {
	Q_D(CALMenu);

	setWindowFlags(Qt::Popup | Qt::FramelessWindowHint | Qt::NoDropShadowWindowHint);
	setAttribute(Qt::WA_TranslucentBackground);
	setObjectName("CALMenu");
	d->isCloseAnimation = false;
	d->menuStyle = new CALMenuStyle(style());
	setStyle(d->menuStyle);
	d->animationImagePosY = 0;
}

CALMenu::CALMenu(const QString& title, QWidget* parent): CALMenu(parent) {
	this->setTitle(title);
}

CALMenu::~CALMenu() = default;

void CALMenu::setMenuItemHeight(const int menuItemHeight) {
	Q_D(CALMenu);

	d->menuStyle->setMenuItemHeight(menuItemHeight);
}

int CALMenu::getMenuItemHeight() const {
	Q_D(const CALMenu);

	return d->menuStyle->getMenuItemHeight();
}

QAction* CALMenu::addMenu(QMenu* menu) {
	return QMenu::addMenu(menu);
}

CALMenu* CALMenu::addMenu(const QString& title) {
	const auto menu = new CALMenu(title, this);
	QMenu::addAction(menu->menuAction());
	return menu;
}

CALMenu* CALMenu::addMenu(const QIcon& icon, const QString& title) {
	const auto menu = new CALMenu(title, this);
	menu->setIcon(icon);
	QMenu::addAction(menu->menuAction());
	return menu;
}

QAction* CALMenu::addAction(const QString& text) {
	return QMenu::addAction(text);
}

QAction* CALMenu::addAction(const ALIcon::AweSomeIcon& awesomeicon, const QString& text) {
	const auto action = new QAction(text, this);
	if (awesomeicon != ALIcon::AweSomeIcon::None) {
		action->setProperty(ALIcon::iconProperty, QChar(static_cast<unsigned short>(awesomeicon)));
		action->setProperty(ALIcon::iconTypeProperty, ALIcon::Awesome);
	}
	QMenu::addAction(action);
	return action;
}

QAction* CALMenu::addAction(const ALIcon::FluentIcon& fluenticon, const QString& text) {
	const auto action = new QAction(text, this);
	if (fluenticon != ALIcon::FluentIcon::None) {
		action->setProperty(ALIcon::iconProperty, QChar(static_cast<unsigned short>(fluenticon)));
		action->setProperty(ALIcon::iconTypeProperty, ALIcon::Fluent);
	}
	QMenu::addAction(action);
	return action;
}

QAction* CALMenu::addAction(const QIcon& icon, const QString& text) {
	return QMenu::addAction(icon, text);
}

QAction* CALMenu::addAction(const ALIcon::AweSomeIcon& awesomeicon, const QString& text, const QKeySequence& shortcut) {
	const auto action = new QAction(text, this);
	action->setProperty(ALIcon::iconProperty, QChar(static_cast<unsigned short>(awesomeicon)));
	action->setProperty(ALIcon::iconTypeProperty, ALIcon::Awesome);
	action->setShortcut(shortcut);
	QMenu::addAction(action);
	return action;
}

QAction* CALMenu::addAction(const ALIcon::FluentIcon& fluenticon, const QString& text, const QKeySequence& shortcut) {
	const auto action = new QAction(text, this);
	action->setProperty(ALIcon::iconProperty, QChar(static_cast<unsigned short>(fluenticon)));
	action->setProperty(ALIcon::iconTypeProperty, ALIcon::Fluent);
	action->setShortcut(shortcut);
	QMenu::addAction(action);
	return action;
}

QAction* CALMenu::addAction(const QIcon& icon, const QString& text, const QKeySequence& shortcut) {
	const auto action = new QAction(text, this);
	action->setIcon(icon);
	action->setShortcut(shortcut);
	QMenu::addAction(action);
	return action;
}

QAction* CALMenu::addAction(const ALIcon::AweSomeIcon& awesomeicon, const QString& text, const QObject* receiver, const char* member, const QKeySequence& shortcut) {
	const auto action = new QAction(text, this);
	action->setProperty(ALIcon::iconProperty, QChar(static_cast<unsigned short>(awesomeicon)));
	action->setProperty(ALIcon::iconTypeProperty, ALIcon::Awesome);
	action->setShortcut(shortcut);
	QObject::connect(action, SIGNAL(triggered(bool)), receiver, member);
	QMenu::addAction(action);
	return action;
}

QAction* CALMenu::addAction(const ALIcon::FluentIcon& fluenticon, const QString& text, const QObject* receiver, const char* member, const QKeySequence& shortcut) {
	const auto action = new QAction(text, this);
	action->setProperty(ALIcon::iconProperty, QChar(static_cast<unsigned short>(fluenticon)));
	action->setProperty(ALIcon::iconTypeProperty, ALIcon::Fluent);
	action->setShortcut(shortcut);
	QObject::connect(action, SIGNAL(triggered(bool)), receiver, member);
	QMenu::addAction(action);
	return action;
}

QAction* CALMenu::addAction(const QIcon& icon, const QString& text, const QObject* receiver, const char* member, const QKeySequence& shortcut) {
	return QMenu::addAction(icon, text, receiver, member, shortcut);
}

bool CALMenu::isHasChildMenu() const {
	auto actionList = this->actions();
	const bool hasMenu = std::any_of(actionList.begin(), actionList.end(), [](const auto& action) {
		return !action->isSeparator() && action->menu();
	});

	return hasMenu;
}

bool CALMenu::isHasIcon() const {
	auto actionList = this->actions();
	const bool hasIcon = std::any_of(actionList.begin(), actionList.end(), [](const auto& action) {
		// Skip separators
		if (action->isSeparator()) {
			return false;
		}
		// Check menu for icons or property
		if (const QMenu* menu = action->menu(); menu && (!menu->icon().isNull() || !menu->property(ALIcon::iconProperty).toString().isEmpty())) {
			return true;
		}
		// Check action itself for icons or property
		return !action->icon().isNull() || !action->property(ALIcon::iconProperty).toString().isEmpty();
	});

	return hasIcon;
}

void CALMenu::showEvent(QShowEvent* event) {
	Q_EMIT sigMenuShow();
	Q_D(CALMenu);

	//消除阴影偏移
	move(this->pos().x() - 6, this->pos().y());
	if (!d->animationPix.isNull()) {
		d->animationPix = QPixmap();
	}
	d->animationPix = this->grab(this->rect());
	const auto posAnimation = new QPropertyAnimation(d, "animationImagePosY");
	connect(posAnimation, &QPropertyAnimation::finished, this, [this, d]() {
		d->animationPix = QPixmap();
		update();
	});
	connect(posAnimation, &QPropertyAnimation::valueChanged, this, [this](const QVariant&) { update(); });
	posAnimation->setEasingCurve(QEasingCurve::OutCubic);
	posAnimation->setDuration(400);
	int targetPosY = height();
	if (targetPosY > 160) {
		if (targetPosY < 320) {
			targetPosY = 160;
		} else {
			targetPosY /= 2;
		}
	}

	if (pos().y() + d->menuStyle->getMenuItemHeight() + 9 >= QCursor::pos().y()) {
		posAnimation->setStartValue(-targetPosY);
	} else {
		posAnimation->setStartValue(targetPosY);
	}

	posAnimation->setEndValue(0);
	posAnimation->start(QAbstractAnimation::DeleteWhenStopped);
	QMenu::showEvent(event);
}

void CALMenu::paintEvent(QPaintEvent* event) {
	Q_D(CALMenu);
	QPainter painter(this);
	painter.setRenderHints(QPainter::SmoothPixmapTransform | QPainter::Antialiasing);
	if (!d->animationPix.isNull()) {
		painter.save();
		painter.drawPixmap(QRect(0, d->animationImagePosY, width(), height()), d->animationPix);
		painter.restore();
	} else {
		QMenu::paintEvent(event);
	}
}
}
