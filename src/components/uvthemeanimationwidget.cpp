#include "uvthemeanimationwidget.hpp"

#include <QPainter>
#include <QPainterPath>
#include <QPropertyAnimation>

/**
 * @brief \class CUVThemeAnimationWidget
 * @param parent pointer to the parent class
 */
CUVThemeAnimationWidget::CUVThemeAnimationWidget(QWidget* parent): QWidget(parent) {
	m_endRadius = 0.01;
}

CUVThemeAnimationWidget::~CUVThemeAnimationWidget() = default;

void CUVThemeAnimationWidget::startAnimation(const int mesc) {
	const auto themeChangeAnimation = new QPropertyAnimation(this, "radius");
	themeChangeAnimation->setDuration(mesc);
	themeChangeAnimation->setEasingCurve(QEasingCurve::InOutSine);
	connect(themeChangeAnimation, &QPropertyAnimation::finished, this, [=]() {
		Q_EMIT sigAnimationFinished();
		this->deleteLater();
	});
	connect(themeChangeAnimation, &QPropertyAnimation::valueChanged, this, [=]() { this->update(); });
	themeChangeAnimation->setStartValue(0.0);
	themeChangeAnimation->setEndValue(m_endRadius);
	themeChangeAnimation->start(QAbstractAnimation::DeleteWhenStopped);
}

void CUVThemeAnimationWidget::setRadius(const qreal radius) {
	m_radius = radius;
	Q_EMIT sigRadiusChanged();
}

qreal CUVThemeAnimationWidget::getRadius() const {
	return m_radius;
}

void CUVThemeAnimationWidget::setEndRadius(const qreal endRadius) {
	m_endRadius = endRadius;
	Q_EMIT sigEndRadiusChanged();
}

qreal CUVThemeAnimationWidget::getEndRadius() const {
	return m_endRadius;
}

void CUVThemeAnimationWidget::setCenter(const QPoint& center) {
	m_center = center;
	Q_EMIT sigCenterChanged();
}

QPoint CUVThemeAnimationWidget::getCenter() const {
	return m_center;
}

void CUVThemeAnimationWidget::setOldWindowImage(const QImage& oldWindowImage) {
	m_oldWindowImage = oldWindowImage;
	Q_EMIT sigOldWindowImageChanged();
}

QImage CUVThemeAnimationWidget::getOldWindowImage() const {
	return m_oldWindowImage;
}

void CUVThemeAnimationWidget::setNewWindowImage(const QImage& newWindowImage) {
	m_newWindowImage = newWindowImage;
	Q_EMIT sigNewWindowImageChanged();
}

QImage CUVThemeAnimationWidget::getNewWindowImage() const {
	return m_newWindowImage;
}

void CUVThemeAnimationWidget::paintEvent(QPaintEvent* event) {
	QPainter painter(this);
	painter.save();
	painter.setRenderHint(QPainter::Antialiasing);
	painter.setPen(Qt::NoPen);
	painter.drawImage(rect(), m_oldWindowImage);
	QPainterPath path;
	path.moveTo(m_center.x(), m_center.y());
	path.addEllipse(QPointF(m_center.x(), m_center.y()), m_radius, m_radius);
	painter.setClipPath(path);
	painter.drawImage(rect(), m_newWindowImage);
	painter.restore();
}
