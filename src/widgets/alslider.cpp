#include "alslider.hpp"

#include "alslider_p.hpp"

#include "alsliderstyle.hpp"

/**
 * @brief \namespace AL
 */
namespace AL {
/**
 * @brief \class CALSliderPrivate
 * Internal class for CALSlider
 * @param q pointer to the public class
 * @param parent pointer to the parent class
 */
CALSliderPrivate::CALSliderPrivate(CALSlider* q, QObject* parent): QObject(parent), q_ptr(q) {
	style = new CALSliderStyle(q->style());
	q->setStyle(style);
}

CALSliderPrivate::~CALSliderPrivate() = default;

/**
 * @brief \class CALSlider
 * @param parent pointer to the parent class
 */
CALSlider::CALSlider(QWidget* parent): QSlider(parent), d_ptr(new CALSliderPrivate(this, this)) {
	setOrientation(Qt::Horizontal);
}

CALSlider::CALSlider(const Qt::Orientation orientation, QWidget* parent): QSlider(orientation, parent), d_ptr(new CALSliderPrivate(this, this)) {
}

CALSlider::~CALSlider() = default;

void CALSlider::setSliderTrackColor(const QColor& color) {
	d_func()->style->setSliderTrackColor(color);
}

void CALSlider::setSliderTrackDisabledColor(const QColor& color) {
	d_func()->style->setSliderTrackDisabledColor(color);
}

void CALSlider::setSliderTrackHoverColor(const QColor& color) {
	d_func()->style->setSliderTrackHoverColor(color);
}

void CALSlider::setSliderTrackHoverDisabledColor(const QColor& color) {
	d_func()->style->setSliderTrackHoverDisabledColor(color);
}

void CALSlider::setSliderHandleBorderColor(const QColor& color) {
	d_func()->style->setSliderHandleBorderColor(color);
}

void CALSlider::setSliderHandleBorderDisabledColor(const QColor& color) {
	d_func()->style->setSliderHandleBorderDisabledColor(color);
}

void CALSlider::setSliderHandleInnerColor(const QColor& color) {
	d_func()->style->setSliderHandleInnerColor(color);
}

void CALSlider::setSliderHandleInnerDisabledColor(const QColor& color) {
	d_func()->style->setSliderHandleInnerDisabledColor(color);
}

void CALSlider::setSliderHandleOuterColor(const QColor& color) {
	d_func()->style->setSliderHandleOuterColor(color);
}

void CALSlider::setSliderHandleOuterDisabledColor(const QColor& color) {
	d_func()->style->setSliderHandleOuterDisabledColor(color);
}

void CALSlider::mousePressEvent(QMouseEvent* event) {
	QSlider::mousePressEvent(event);
	Q_EMIT sigLeftMousePressed();
}

void CALSlider::mouseMoveEvent(QMouseEvent* event) {
	QSlider::mouseMoveEvent(event);
	Q_EMIT sigLeftMouseMoved();
}

void CALSlider::mouseReleaseEvent(QMouseEvent* event) {
	QSlider::mouseReleaseEvent(event);
	Q_EMIT sigLeftMouseReleased();
}
}
