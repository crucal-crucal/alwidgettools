#include "uvtoggleswitch.hpp"

#include <QMouseEvent>
#include <QPainter>
#include <QPainterPath>
#include <QPropertyAnimation>

#include "uvtoggleswitch_p.hpp"

/**
 * @brief \class CUVToggleSwitchPrivate
 * Internal class for CUVToggleSwitch
 * @param q pointer to the public class
 * @param parent pointer to the parent class
 */
CUVToggleSwitchPrivate::CUVToggleSwitchPrivate(CUVToggleSwitch* q, QObject* parent): QObject(parent), q_ptr(q) {
}

CUVToggleSwitchPrivate::~CUVToggleSwitchPrivate() = default;

void CUVToggleSwitchPrivate::_startPosAnimation(const qreal startX, const qreal endX, const bool isToggle) {
	Q_Q(CUVToggleSwitch);

	const auto circleAnimation = new QPropertyAnimation(q, "circleCenterX");
	connect(circleAnimation, &QPropertyAnimation::valueChanged, q, [=](const QVariant& value) {
		this->_circleCenterX = value.toReal();
		q->update();
	});
	circleAnimation->setStartValue(startX);
	circleAnimation->setEndValue(endX);
	circleAnimation->setEasingCurve(QEasingCurve::InOutSine);
	circleAnimation->start(QAbstractAnimation::DeleteWhenStopped);
	_isToggled = isToggle;
	emit q->sigToggleChanged(isToggle);
}

void CUVToggleSwitchPrivate::_startRadiusAnimation(const qreal startRadius, const qreal endRadius) {
	Q_Q(CUVToggleSwitch);

	const auto circleRadiusAnimation = new QPropertyAnimation(q, "circleRadius");
	connect(circleRadiusAnimation, &QPropertyAnimation::valueChanged, q, [=](const QVariant& value) {
		this->_circleRadius = value.toReal();
		q->update();
	});
	circleRadiusAnimation->setStartValue(startRadius);
	circleRadiusAnimation->setEndValue(endRadius);
	circleRadiusAnimation->setEasingCurve(QEasingCurve::InOutSine);
	circleRadiusAnimation->start(QAbstractAnimation::DeleteWhenStopped);
}

void CUVToggleSwitchPrivate::_adjustCircleCenterX() {
	Q_Q(CUVToggleSwitch);

	if (_circleCenterX > q->width() - q->height() / 2 - _margin * 2) {
		_circleCenterX = q->width() - q->height() / 2 - _margin * 2;
	}

	if (_circleCenterX < q->height() / 2) {
		_circleCenterX = q->height() / 2;
	}
}

/**
 * @brief \class CUVToggleSwitch
 * @param parent pointer to the parent class
 */
CUVToggleSwitch::CUVToggleSwitch(QWidget* parent) : QWidget(parent), d_ptr(new CUVToggleSwitchPrivate(this, this)) {
	setMouseTracking(true);
	setFixedSize(44, 22);
	d_func()->_circleCenterX = -1;
	d_func()->_isToggled = false;
	setProperty("circleCenterX", 0.01);
	setProperty("circleRadius", 0.01);
}

CUVToggleSwitch::~CUVToggleSwitch() = default;

void CUVToggleSwitch::setIsToggled(const bool isToggled) {
	Q_D(CUVToggleSwitch);

	if (d->_isToggled == isToggled) {
		return;
	}

	const qreal startX = d->_isToggled ? width() - height() / 2.0 - d->_margin * 2.0 : height() / 2.0;
	const qreal endX = d->_isToggled ? height() / 2.0 : width() - height() / 2.0 - d->_margin * 2.0;
	d->_startPosAnimation(startX, endX, isToggled);
}

bool CUVToggleSwitch::isToggled() const {
	Q_D(const CUVToggleSwitch);

	return d->_isToggled;
}

bool CUVToggleSwitch::event(QEvent* event) {
	Q_D(CUVToggleSwitch);

	switch (event->type()) {
		case QEvent::Enter: {
			if (isEnabled()) {
				d->_startRadiusAnimation(height() * 0.3, height() * 0.35);
			}
			break;
		}
		case QEvent::Leave: {
			if (isEnabled()) {
				d->_startRadiusAnimation(height() * 0.35, height() * 0.3);
			}
			break;
		}
		case QEvent::MouseMove: {
			update();
			break;
		}
		default: break;
	}

	return QWidget::event(event);
}

void CUVToggleSwitch::mousePressEvent(QMouseEvent* event) {
	Q_D(CUVToggleSwitch);

	d->_adjustCircleCenterX();
	d->_isLeftButtonPress = true;
	d->_lastMouseX = event->pos().x();
	d->_startRadiusAnimation(d->_circleRadius, height() * 0.25);

	// QWidget::mousePressEvent(event);
}

void CUVToggleSwitch::mouseReleaseEvent(QMouseEvent* event) {
	Q_D(CUVToggleSwitch);

	d->_isLeftButtonPress = false;
	// QWidget::mouseReleaseEvent(event);

	if (d->_isMousePressMove) {
		d->_isMousePressMove = false;
		const qreal endx = (d->_circleCenterX > width() / 2.0) ? (width() - height() / 2.0 - d->_margin * 2.0) : (height() / 2.0);
		d->_startPosAnimation(d->_circleCenterX, endx, d->_circleCenterX > width() / 2.0);
	} else {
		const qreal endx = d->_isToggled ? (height() / 2.0) : (width() - height() / 2.0 - d->_margin * 2.0);
		d->_startPosAnimation(d->_circleCenterX, endx, !d->_isToggled);
	}

	d->_startRadiusAnimation(height() * 0.25, height() * 0.35);
}

void CUVToggleSwitch::mouseMoveEvent(QMouseEvent* event) {
	Q_D(CUVToggleSwitch);

	if (d->_isLeftButtonPress) {
		d->_isMousePressMove = true;
		const int moveX = event->pos().x() - d->_lastMouseX;
		d->_lastMouseX = event->pos().x();;
		d->_circleCenterX += moveX;
		d->_adjustCircleCenterX();
	}

	// QWidget::mouseMoveEvent(event);
}

void CUVToggleSwitch::paintEvent(QPaintEvent* event) {
	Q_D(CUVToggleSwitch);

	QPainter painter(this);
	painter.save();
	painter.setRenderHints(QPainter::SmoothPixmapTransform | QPainter::Antialiasing | QPainter::TextAntialiasing);
	// 背景绘制
	painter.setPen(d->_isToggled ? QPen(QColor(0x4B, 0x4B, 0x4B), 1.5) : QPen(QColor(0x5C, 0x5C, 0x5C), 1.5));
	painter.setBrush(isEnabled() ? d->_isToggled ? qRgb(82, 164, 255) : (underMouse() ? QColor(0x40, 0x40, 0x40) : QColor(0x34, 0x34, 0x34)) : QColor(0x2A, 0x2A, 0x2A));
	QPainterPath path;
	path.moveTo(width() - height() - d->_margin, height() - d->_margin);
	path.arcTo(QRectF(QPointF(width() - height() - d->_margin, d->_margin), QSize(height() - d->_margin * 2, height() - d->_margin * 2)), -90, 180);
	path.lineTo(height() / 2 + d->_margin, d->_margin);
	path.arcTo(QRectF(QPointF(d->_margin, d->_margin), QSize(height() - d->_margin * 2, height() - d->_margin * 2)), 90, 180);
	path.lineTo(width() - height() - d->_margin, height() - d->_margin);
	path.closeSubpath();
	painter.drawPath(path);

	// 圆心绘制
	painter.setPen(Qt::NoPen);
	painter.setBrush(isEnabled() ? d->_isToggled ? Qt::white : QColor(0xD0, 0xD0, 0xD0) : QColor(0xA7, 0xA7, 0xA7));
	if (d->_circleRadius == 0) {
		d->_circleRadius = this->isEnabled() ? (underMouse() ? height() * 0.35 : height() * 0.3) : height() * 0.3;
	}
	if (d->_isLeftButtonPress) {
		painter.drawEllipse(QPointF(d->_circleCenterX, height() / 2), d->_circleRadius, d->_circleRadius);
	} else {
		if (d->_circleCenterX == -1) {
			d->_circleCenterX = d->_isToggled ? width() - height() / 2 - d->_margin * 2 : height() / 2;
		}
		painter.drawEllipse(QPointF(d->_circleCenterX, height() / 2), d->_circleRadius, d->_circleRadius);
	}
	painter.restore();
}
