#include "alscrollarea.hpp"

#include <QScroller>
#include <QDebug>
#include <QApplication>
#include "alscrollarea_p.hpp"
#include "alscrollbar.hpp"

/**
 * @brief \namespace AL
 */
namespace AL {
/**
 * @brief \class CALScrollAreaPrivate
 * Internal class for CALScrollArea
 * @param q pointer to the public class
 * @param parent pointer to the parent class
 */
CALScrollAreaPrivate::CALScrollAreaPrivate(CALScrollArea* q, QObject* parent): QObject(parent), q_ptr(q) {
}

CALScrollAreaPrivate::~CALScrollAreaPrivate() = default;

CALScrollBar* CALScrollAreaPrivate::getScrollBar(const Qt::Orientation& orientation) const {
	return orientation == Qt::Horizontal ? horizontalScrollBar : verticalScrollBar;
}

/**
 * @brief \class CALScrollArea
 * @param parent pointer to the parent class
 */
CALScrollArea::CALScrollArea(QWidget* parent): QScrollArea(parent), d_ptr(new CALScrollAreaPrivate(this, this)) {
	Q_D(CALScrollArea);

	setObjectName("CALScrollArea");
	setStyleSheet("#CALScrollArea { background-color: transparent; border: none; }");

	d->horizontalScrollBar = new CALScrollBar(this);
	d->verticalScrollBar = new CALScrollBar(this);
	setHorizontalScrollBar(d->horizontalScrollBar);
	setVerticalScrollBar(d->verticalScrollBar);
	QScrollArea::setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	QScrollArea::setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
}

CALScrollArea::~CALScrollArea() = default;

void CALScrollArea::setIsGrabGesture(const bool bIsEnable, const qreal mousePressEventDelay) const {
	if (bIsEnable) {
		QScroller::grabGesture(this->viewport(), QScroller::LeftMouseButtonGesture);
		QScrollerProperties properties = QScroller::scroller(this->viewport())->scrollerProperties();
		properties.setScrollMetric(QScrollerProperties::MousePressEventDelay, mousePressEventDelay);
		properties.setScrollMetric(QScrollerProperties::OvershootDragResistanceFactor, 0.35);
		properties.setScrollMetric(QScrollerProperties::OvershootScrollTime, 0.5);
		properties.setScrollMetric(QScrollerProperties::FrameRate, QScrollerProperties::Fps60);
		QScroller::scroller(this->viewport())->setScrollerProperties(properties);
	} else {
		QScroller::ungrabGesture(this->viewport());
	}
}

void CALScrollArea::setIsOverShoot(const Qt::Orientation& orientation, const bool bIsEnable) const {
	QScrollerProperties properties = QScroller::scroller(this->viewport())->scrollerProperties();
	properties.setScrollMetric(orientation == Qt::Horizontal ? QScrollerProperties::HorizontalOvershootPolicy : QScrollerProperties::VerticalOvershootPolicy,
	                           bIsEnable ? QScrollerProperties::OvershootAlwaysOn : QScrollerProperties::OvershootAlwaysOff);
	QScroller::scroller(this->viewport())->setScrollerProperties(properties);
}

bool CALScrollArea::getIsOverShoot(const Qt::Orientation& orientation) const {
	const QScrollerProperties properties = QScroller::scroller(this->viewport())->scrollerProperties();
	return properties.scrollMetric(orientation == Qt::Horizontal ? QScrollerProperties::HorizontalOvershootPolicy : QScrollerProperties::VerticalOvershootPolicy).toBool();
}

void CALScrollArea::setIsAnimation(const Qt::Orientation& orientation, const bool bIsAnimation) {
	d_func()->getScrollBar(orientation)->setIsAnimation(bIsAnimation);
}

bool CALScrollArea::getIsAnimation(const Qt::Orientation& orientation) const {
	return d_func()->getScrollBar(orientation)->getIsAnimation();
}

CALScrollArea::CALScrollArea(CALScrollAreaPrivate& dd, CALScrollArea* parent): QScrollArea(parent), d_ptr(&dd) {
}
}
