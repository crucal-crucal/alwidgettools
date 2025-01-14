#include "altoggleswitch.hpp"

#include <QMouseEvent>
#include <QPainter>
#include <QPainterPath>
#include <QPropertyAnimation>

#include "althememanager.hpp"
#include "altoggleswitch_p.hpp"

/**
 * @brief \namespace AL
 */
namespace AL {
/**
 * @brief \class CALToggleSwitchPrivate
 * Internal class for CALToggleSwitch
 * @param q pointer to the public class
 * @param parent pointer to the parent class
 */
CALToggleSwitchPrivate::CALToggleSwitchPrivate(CALToggleSwitch* q, QObject* parent): QObject(parent), q_ptr(q) {
}

CALToggleSwitchPrivate::~CALToggleSwitchPrivate() = default;

void CALToggleSwitchPrivate::startPosAnimation(const qreal startX, const qreal endX, const bool isToggle) {
	Q_Q(CALToggleSwitch);

	const auto circleAnimation = new QPropertyAnimation(q, "circleCenterX");
	connect(circleAnimation, &QPropertyAnimation::valueChanged, q, [=](const QVariant& value) {
		this->circleCenterX = value.toReal();
		q->update();
	});
	circleAnimation->setStartValue(startX);
	circleAnimation->setEndValue(endX);
	circleAnimation->setEasingCurve(QEasingCurve::InOutSine);
	circleAnimation->start(QAbstractAnimation::DeleteWhenStopped);
	isToggled = isToggle;
	emit q->sigToggleChanged(isToggle);
}

void CALToggleSwitchPrivate::startRadiusAnimation(const qreal startRadius, const qreal endRadius) {
	Q_Q(CALToggleSwitch);

	const auto circleRadiusAnimation = new QPropertyAnimation(q, "circleRadius");
	connect(circleRadiusAnimation, &QPropertyAnimation::valueChanged, q, [=](const QVariant& value) {
		this->circleRadius = value.toReal();
		q->update();
	});
	circleRadiusAnimation->setStartValue(startRadius);
	circleRadiusAnimation->setEndValue(endRadius);
	circleRadiusAnimation->setEasingCurve(QEasingCurve::InOutSine);
	circleRadiusAnimation->start(QAbstractAnimation::DeleteWhenStopped);
}

void CALToggleSwitchPrivate::adjustCircleCenterX() {
	Q_Q(CALToggleSwitch);

	if (circleCenterX > q->width() - q->height() / 2.0 - margin * 2) {
		circleCenterX = q->width() - q->height() / 2.0 - margin * 2;
	}

	if (circleCenterX < q->height() / 2.0) {
		circleCenterX = q->height() / 2.0;
	}
}

/**
 * @brief \class CALToggleSwitch
 * @param parent pointer to the parent class
 */
CALToggleSwitch::CALToggleSwitch(QWidget* parent) : QWidget(parent), d_ptr(new CALToggleSwitchPrivate(this, this)) {
	Q_D(CALToggleSwitch);

	setMouseTracking(true);
	setFixedSize(44, 22);
	d->circleCenterX = -1;
	d->isToggled = false;
	d->themeMode = ALTheme->getThemeMode();
	connect(ALTheme, &CALThemeManager::sigThemeModeChanged, this, [=](const ALThemeType::ThemeMode& mode) { d->themeMode = mode; });
	setProperty("circleCenterX", 0.01);
	setProperty("circleRadius", 0.01);
}

CALToggleSwitch::~CALToggleSwitch() = default;

void CALToggleSwitch::setIsToggled(const bool isToggled) {
	Q_D(CALToggleSwitch);

	if (d->isToggled == isToggled) {
		return;
	}

	const qreal startX = d->isToggled ? width() - height() / 2.0 - d->margin * 2.0 : height() / 2.0;
	const qreal endX = d->isToggled ? height() / 2.0 : width() - height() / 2.0 - d->margin * 2.0;
	d->startPosAnimation(startX, endX, isToggled);
}

bool CALToggleSwitch::isToggled() const {
	Q_D(const CALToggleSwitch);

	return d->isToggled;
}

bool CALToggleSwitch::event(QEvent* event) {
	Q_D(CALToggleSwitch);

	switch (event->type()) {
		case QEvent::Enter: {
			if (isEnabled()) {
				d->startRadiusAnimation(height() * 0.3, height() * 0.35);
			}
			break;
		}
		case QEvent::Leave: {
			if (isEnabled()) {
				d->startRadiusAnimation(height() * 0.35, height() * 0.3);
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

void CALToggleSwitch::mousePressEvent(QMouseEvent* event) {
	Q_D(CALToggleSwitch);

	d->adjustCircleCenterX();
	d->isLeftButtonPress = true;
	d->lastMouseX = event->pos().x();
	d->startRadiusAnimation(d->circleRadius, height() * 0.25);

	QWidget::mousePressEvent(event);
}

void CALToggleSwitch::mouseReleaseEvent(QMouseEvent* event) {
	Q_D(CALToggleSwitch);

	d->isLeftButtonPress = false;
	QWidget::mouseReleaseEvent(event);

	if (d->isMousePressMove) {
		d->isMousePressMove = false;
		const qreal endx = (d->circleCenterX > width() / 2.0) ? (width() - height() / 2.0 - d->margin * 2.0) : (height() / 2.0);
		d->startPosAnimation(d->circleCenterX, endx, d->circleCenterX > width() / 2.0);
	} else {
		const qreal endx = d->isToggled ? (height() / 2.0) : (width() - height() / 2.0 - d->margin * 2.0);
		d->startPosAnimation(d->circleCenterX, endx, !d->isToggled);
	}

	d->startRadiusAnimation(height() * 0.25, height() * 0.35);
}

void CALToggleSwitch::mouseMoveEvent(QMouseEvent* event) {
	Q_D(CALToggleSwitch);

	if (d->isLeftButtonPress) {
		d->isMousePressMove = true;
		const int moveX = event->pos().x() - d->lastMouseX;
		d->lastMouseX = event->pos().x();;
		d->circleCenterX += moveX;
		d->adjustCircleCenterX();
	}

	QWidget::mouseMoveEvent(event);
}

void CALToggleSwitch::paintEvent(QPaintEvent* event) {
	Q_D(CALToggleSwitch);

	QPainter painter(this);
	painter.save();
	painter.setRenderHints(QPainter::SmoothPixmapTransform | QPainter::Antialiasing | QPainter::TextAntialiasing);
	// 背景绘制
	painter.setPen(QPen(ALThemeColor(d->themeMode, d->isToggled ? ALThemeType::BasicBorder : ALThemeType::BasicBorderDeep), 1.5));
	painter.setBrush(ALThemeColor(d->themeMode, isEnabled() ? d->isToggled ? ALThemeType::PrimaryNormal : underMouse() ? ALThemeType::BasicHover : ALThemeType::BasicBase : ALThemeType::BasicDisable));
	QPainterPath path;
	path.moveTo(width() - height() - d->margin, height() - d->margin);
	path.arcTo(QRectF(QPointF(width() - height() - d->margin, d->margin), QSize(height() - d->margin * 2, height() - d->margin * 2)), -90, 180);
	path.lineTo(height() / 2.0 + d->margin, d->margin);
	path.arcTo(QRectF(QPointF(d->margin, d->margin), QSize(height() - d->margin * 2, height() - d->margin * 2)), 90, 180);
	path.lineTo(width() - height() - d->margin, height() - d->margin);
	path.closeSubpath();
	painter.drawPath(path);

	// 圆心绘制
	painter.setPen(Qt::NoPen);
	painter.setBrush(ALThemeColor(d->themeMode, isEnabled() ? d->isToggled ? ALThemeType::BasicTextInvert : ALThemeType::ToggleSwitchNoToggledCenter : ALThemeType::BasicTextDisable));
	if (d->circleRadius == 0) {
		d->circleRadius = this->isEnabled() ? (underMouse() ? height() * 0.35 : height() * 0.3) : height() * 0.3;
	}
	if (d->isLeftButtonPress) {
		painter.drawEllipse(QPointF(d->circleCenterX, height() / 2.0), d->circleRadius, d->circleRadius);
	} else {
		if (d->circleCenterX == -1) {
			d->circleCenterX = d->isToggled ? width() - height() / 2 - d->margin * 2 : height() / 2;
		}
		painter.drawEllipse(QPointF(d->circleCenterX, height() / 2.0), d->circleRadius, d->circleRadius);
	}
	painter.restore();
}
}
