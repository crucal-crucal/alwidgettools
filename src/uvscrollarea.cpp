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
	horizontalScrollBar = new CUVScrollBar(Qt::Horizontal, q);
	verticalScrollBar = new CUVScrollBar(Qt::Vertical, q);
	q->setHorizontalScrollBar(horizontalScrollBar);
	q->setVerticalScrollBar(verticalScrollBar);

	//this->setFocusPolicy(Qt::StrongFocus);
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

void CUVScrollArea::setWheelToScrollBarEnabled(const bool bEnabled) {
	d_func()->bWheelToScrollBarEnabled = bEnabled;
}

bool CUVScrollArea::getWheelToScrollBarEnabled() const {
	return d_func()->bWheelToScrollBarEnabled;
}

CUVScrollBar* CUVScrollArea::getHorizontalScrollBar() const {
	return d_func()->horizontalScrollBar;
}

CUVScrollBar* CUVScrollArea::getVerticalScrollBar() const {
	return d_func()->verticalScrollBar;
}

void CUVScrollArea::mousePressEvent(QMouseEvent* ev) {
	Q_D(CUVScrollArea);

	if (QApplication::keyboardModifiers() == Qt::CTRL && ev->buttons() & Qt::LeftButton) {
		this->setCursor(Qt::ClosedHandCursor);
		d->moveStartPosition = ev->globalPos(); /*- this->pos();*/
		d->bStartMove = true;
		return;
	}
	this->setCursor(Qt::ArrowCursor);
}

void CUVScrollArea::mouseMoveEvent(QMouseEvent* ev) {
	Q_D(CUVScrollArea);

	if (QApplication::keyboardModifiers() == Qt::CTRL && d->bStartMove) {
		this->setCursor(Qt::ClosedHandCursor);
		QScrollBar* scrollBarx = horizontalScrollBar();
		QScrollBar* scrollBary = verticalScrollBar();

		QPoint p = ev->globalPos();
		int offsetx = p.x() - d->moveStartPosition.x();
		int offsety = p.y() - d->moveStartPosition.y();

		scrollBarx->setValue(scrollBarx->value() - offsetx);
		scrollBary->setValue(scrollBary->value() - offsety);
		d->moveStartPosition = p;

		return;
	}
	this->setCursor(Qt::ArrowCursor);
}

void CUVScrollArea::mouseReleaseEvent(QMouseEvent* ev) {
	Q_D(CUVScrollArea);

	d->bStartMove = false;
	if (QApplication::keyboardModifiers() == Qt::CTRL)
		this->setCursor(Qt::OpenHandCursor);
	else
		this->setCursor(Qt::ArrowCursor);
}

void CUVScrollArea::wheelEvent(QWheelEvent* ev) {
	Q_D(CUVScrollArea);

	if (d->bWheelToScrollBarEnabled) {
		return QScrollArea::wheelEvent(ev);
	}

	QPoint pnt1 = ev->pos();
	QPoint pnt2 = ev->globalPos();
	qDebug() << "CUVScrollArea<<<<<<< ev->pos()" << pnt1 << "ev->globalPos()" << pnt2;
	d->wheelStartPosition = ev->pos();
	emit sigWheelDeltaValueChangedWithoutKey(ev->delta(), pnt2);
}

void CUVScrollArea::resizeEvent(QResizeEvent* event) {
	Q_D(CUVScrollArea);
	QScrollArea::resizeEvent(event);
}

void CUVScrollArea::enterEvent(QEvent* event) {
	Q_D(CUVScrollArea);

	if (d->horizontalScrollBar->maximum() > 0 && m_bScrollBarVisible) {
		d->horizontalScrollBar->show();
		this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
	}

	if (d->verticalScrollBar->maximum() > 0 && m_bScrollBarVisible) {
		d->verticalScrollBar->show();
		this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
	}
	this->update();

	QScrollArea::enterEvent(event);
}

void CUVScrollArea::leaveEvent(QEvent* event) {
	Q_D(CUVScrollArea);

	d->horizontalScrollBar->hide();
	this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	d->verticalScrollBar->hide();
	this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	this->update();

	QScrollArea::leaveEvent(event);
}

CUVScrollArea::CUVScrollArea(CUVScrollAreaPrivate& dd, CUVScrollArea* parent): QScrollArea(parent), d_ptr(&dd) {
}
