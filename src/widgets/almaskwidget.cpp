#include "almaskwidget.hpp"

#include <QPainter>
#include <QPropertyAnimation>

namespace AL {
/**
 * @brief \class CALMaskWidget
 * @param parent pointer to the parent class
 */
CALMaskWidget::CALMaskWidget(QWidget* parent): QWidget(parent) {
	setObjectName("CALMaskWidget");
	setStyleSheet("#CALMaskWidget { background-color: transparent; }");
	m_MaskAlpha = 0;
}

CALMaskWidget::~CALMaskWidget() = default;

void CALMaskWidget::doMaskAnimation(const int endValue) {
	const auto opacityAnimation = new QPropertyAnimation(this, "maskAlpha");
	connect(opacityAnimation, &QPropertyAnimation::valueChanged, this, [=](const QVariant& value) { this->update(); });
	connect(opacityAnimation, &QPropertyAnimation::finished, this, [=]() { this->setVisible(endValue != 0); });
	opacityAnimation->setEasingCurve(QEasingCurve::InOutSine);
	opacityAnimation->setDuration(250);
	opacityAnimation->setStartValue(m_MaskAlpha);
	opacityAnimation->setEndValue(endValue);
	opacityAnimation->start(QAbstractAnimation::DeleteWhenStopped);
}

void CALMaskWidget::paintEvent(QPaintEvent* event) {
	QPainter painter(this);
	painter.save();
	painter.setPen(Qt::NoPen);
	painter.setRenderHint(QPainter::Antialiasing);
	painter.setBrush(QColor(0x00, 0x00, 0x00, m_MaskAlpha));
	painter.drawRect(rect());
	painter.restore();
}

} // namespace AL
