#include "uvscrollarea.hpp"

#include <QScroller>
#include <QDebug>
#include <QApplication>
#include "uvscrollarea_p.hpp"
#include "uvscrollbar.hpp"

/**
 * @brief \class CUVScrollAreaPrivate
 * Internal class for CUVScrollArea
 * @param q pointer to the public class
 * @param parent pointer to the parent class
 */
CUVScrollAreaPrivate::CUVScrollAreaPrivate(CUVScrollArea* q, QObject* parent): QObject(parent), q_ptr(q) {
}

CUVScrollAreaPrivate::~CUVScrollAreaPrivate() = default;

CUVScrollBar* CUVScrollAreaPrivate::getScrollBar(const Qt::Orientation& orientation) const {
	return orientation == Qt::Horizontal ? horizontalScrollBar : verticalScrollBar;
}

/**
 * @brief \class CUVScrollArea
 * @param parent pointer to the parent class
 */
CUVScrollArea::CUVScrollArea(QWidget* parent): QScrollArea(parent), d_ptr(new CUVScrollAreaPrivate(this, this)) {
	Q_D(CUVScrollArea);

	setObjectName("CUVScrollArea");
	setStyleSheet("#CUVScrollArea { background-color: transparent; border: none; }");

	d->horizontalScrollBar = new CUVScrollBar(this);
	d->verticalScrollBar = new CUVScrollBar(this);
	setHorizontalScrollBar(d->horizontalScrollBar);
	setVerticalScrollBar(d->verticalScrollBar);
	QScrollArea::setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	QScrollArea::setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
}

CUVScrollArea::~CUVScrollArea() = default;

void CUVScrollArea::setIsGrabGesture(const bool bIsEnable, const qreal mousePressEventDelay) const {
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

void CUVScrollArea::setIsOverShoot(const Qt::Orientation& orientation, const bool bIsEnable) const {
	QScrollerProperties properties = QScroller::scroller(this->viewport())->scrollerProperties();
	properties.setScrollMetric(orientation == Qt::Horizontal ? QScrollerProperties::HorizontalOvershootPolicy : QScrollerProperties::VerticalOvershootPolicy,
	                           bIsEnable ? QScrollerProperties::OvershootAlwaysOn : QScrollerProperties::OvershootAlwaysOff);
	QScroller::scroller(this->viewport())->setScrollerProperties(properties);
}

bool CUVScrollArea::getIsOverShoot(const Qt::Orientation& orientation) const {
	const QScrollerProperties properties = QScroller::scroller(this->viewport())->scrollerProperties();
	return properties.scrollMetric(orientation == Qt::Horizontal ? QScrollerProperties::HorizontalOvershootPolicy : QScrollerProperties::VerticalOvershootPolicy).toBool();
}

void CUVScrollArea::setIsAnimation(const Qt::Orientation& orientation, const bool bIsAnimation) {
	d_func()->getScrollBar(orientation)->setIsAnimation(bIsAnimation);
}

bool CUVScrollArea::getIsAnimation(const Qt::Orientation& orientation) const {
	return d_func()->getScrollBar(orientation)->getIsAnimation();
}

CUVScrollArea::CUVScrollArea(CUVScrollAreaPrivate& dd, CUVScrollArea* parent): QScrollArea(parent), d_ptr(&dd) {
}
