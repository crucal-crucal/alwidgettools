#include "uvslider.hpp"

#include "uvslider_p.hpp"

#include "uvsliderstyle.hpp"

/**
 * @brief \class CUVSliderPrivate
 * Internal class for CUVSlider
 * @param q pointer to the public class
 * @param parent pointer to the parent class
 */
CUVSliderPrivate::CUVSliderPrivate(CUVSlider* q, QObject* parent): QObject(parent), q_ptr(q) {
	style = new CUVSliderStyle(q->style());
	q->setStyle(style);
}

CUVSliderPrivate::~CUVSliderPrivate() = default;

/**
 * @brief \class CUVSlider
 * @param parent pointer to the parent class
 */
CUVSlider::CUVSlider(QWidget* parent): QSlider(parent), d_ptr(new CUVSliderPrivate(this, this)) {
	setOrientation(Qt::Horizontal);
}

CUVSlider::CUVSlider(const Qt::Orientation orientation, QWidget* parent): QSlider(orientation, parent), d_ptr(new CUVSliderPrivate(this, this)) {
}

CUVSlider::~CUVSlider() = default;

void CUVSlider::setSliderTrackColor(const QColor& color) {
	d_func()->style->setSliderTrackColor(color);
}

void CUVSlider::setSliderTrackDisabledColor(const QColor& color) {
	d_func()->style->setSliderTrackDisabledColor(color);
}

void CUVSlider::setSliderTrackHoverColor(const QColor& color) {
	d_func()->style->setSliderTrackHoverColor(color);
}

void CUVSlider::setSliderTrackHoverDisabledColor(const QColor& color) {
	d_func()->style->setSliderTrackHoverDisabledColor(color);
}

void CUVSlider::setSliderHandleBorderColor(const QColor& color) {
	d_func()->style->setSliderHandleBorderColor(color);
}

void CUVSlider::setSliderHandleBorderDisabledColor(const QColor& color) {
	d_func()->style->setSliderHandleBorderDisabledColor(color);
}

void CUVSlider::setSliderHandleInnerColor(const QColor& color) {
	d_func()->style->setSliderHandleInnerColor(color);
}

void CUVSlider::setSliderHandleInnerDisabledColor(const QColor& color) {
	d_func()->style->setSliderHandleInnerDisabledColor(color);
}

void CUVSlider::setSliderHandleOuterColor(const QColor& color) {
	d_func()->style->setSliderHandleOuterColor(color);
}

void CUVSlider::setSliderHandleOuterDisabledColor(const QColor& color) {
	d_func()->style->setSliderHandleOuterDisabledColor(color);
}

void CUVSlider::mousePressEvent(QMouseEvent* event) {
	QSlider::mousePressEvent(event);
	emit sigLeftMousePressed();
}

void CUVSlider::mouseMoveEvent(QMouseEvent* event) {
	QSlider::mouseMoveEvent(event);
	emit sigLeftMouseMoved();
}

void CUVSlider::mouseReleaseEvent(QMouseEvent* event) {
	QSlider::mouseReleaseEvent(event);
	emit sigLeftMouseReleased();
}
