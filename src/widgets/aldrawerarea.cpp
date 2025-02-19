#include "aldrawerarea.hpp"

#include <QVBoxLayout>

#include "aldrawerarea_p.hpp"
#include "althememanager.hpp"

/**
 * @brief \namespace AL
 */
namespace AL {
/**
 * @brief \class CALDrawerAreaPrivate
 * Internal class for CALDrawerArea
 * @param q pointer to the public class
 * @param parent pointer to the parent class
 */
CALDrawerAreaPrivate::CALDrawerAreaPrivate(CALDrawerArea* q, QObject* parent): QObject(parent), q_ptr(q) {
}

CALDrawerAreaPrivate::~CALDrawerAreaPrivate() = default;

void CALDrawerAreaPrivate::slotDrawerHeaderClicked(const bool isExpand) {
	drawerContainer->doDrawerAnimation(isExpand);
	Q_EMIT q_func()->sigExpandStateChanged(isExpand);
}

void CALDrawerAreaPrivate::setExpandState(const bool expand) {
	drawerHeader->setIsExpand(expand);
	drawerHeader->doExpandOrCollpaseAnimation();
	drawerContainer->doDrawerAnimation(expand);
	Q_EMIT q_func()->sigExpandStateChanged(expand);
}

/**
 * @brief \class CALDrawerArea
 * @param parent pointer to the parent class
 */
CALDrawerArea::CALDrawerArea(QWidget* parent): QWidget(parent), d_ptr(new CALDrawerAreaPrivate(this, this)) {
	Q_D(CALDrawerArea);

	d->drawerHeader = new CALDrawerHeader(this);
	d->drawerContainer = new CALDrawerContainer(this);
	connect(d->drawerHeader, &CALDrawerHeader::sigDrawerHeaderClicked, d, &CALDrawerAreaPrivate::slotDrawerHeaderClicked);

	const auto mainVLayout = new QVBoxLayout(this);
	mainVLayout->setContentsMargins(0, 0, 0, 0);
	mainVLayout->setSpacing(0);
	mainVLayout->addWidget(d->drawerHeader);
	mainVLayout->addWidget(d->drawerContainer);

	d->themeMode = ALTheme->getThemeMode();
	connect(ALTheme, &CALThemeManager::sigThemeModeChanged, this, [d](const ALThemeType::ThemeMode& mode) { d->themeMode = mode; });
}

CALDrawerArea::~CALDrawerArea() = default;

void CALDrawerArea::setDrawerHeader(QWidget* widget) {
	d_func()->drawerHeader->setHeaderWidget(widget);
}

bool CALDrawerArea::addDrawer(QWidget* widget) {
	return d_func()->drawerContainer->addDrawer(widget);
}

bool CALDrawerArea::removeDrawer(QWidget* widget) {
	return d_func()->drawerContainer->removeDrawer(widget);
}

void CALDrawerArea::expand() {
	d_func()->setExpandState(true);
}

void CALDrawerArea::collpase() {
	d_func()->setExpandState(false);
}

bool CALDrawerArea::getIsExpand() const {
	return d_func()->drawerHeader->getIsExpand();
}

void CALDrawerArea::setBorderRadius(const int radius) {
	Q_D(CALDrawerArea);

	d->drawerHeader->setBorderRadius(radius);
	d->drawerContainer->setBorderRadius(radius);
	Q_EMIT sigBorderRadiusChanged();
}

int CALDrawerArea::getBorderRadius() const {
	return d_func()->drawerHeader->getBorderRadius();
}

void CALDrawerArea::setHeaderHeight(const int height) {
	d_func()->drawerHeader->setFixedHeight(height);
}

int CALDrawerArea::getHeaderHeight() const {
	return d_func()->drawerHeader->height();
}
}
