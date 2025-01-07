#include "uvmaskwidget.hpp"

#include <QPainter>
#include <QPropertyAnimation>

/**
 * @brief \class CUVMaskWidget
 * @param parent pointer to the parent class
 */
CUVMaskWidget::CUVMaskWidget(QWidget* parent): QWidget(parent) {
	setObjectName("CUVMaskWidget");
	setStyleSheet("#CUVMaskWidget { background-color: transparent; }");
	m_MaskAlpha = 0;
}

CUVMaskWidget::~CUVMaskWidget() = default;

void CUVMaskWidget::doMaskAnimation(const int endValue) {
	const auto opacityAnimation = new QPropertyAnimation(this, "maskAlpha");
	connect(opacityAnimation, &QPropertyAnimation::valueChanged, this, [=](const QVariant& value) { this->update(); });
	connect(opacityAnimation, &QPropertyAnimation::finished, this, [=]() { this->setVisible(endValue != 0); });
	opacityAnimation->setEasingCurve(QEasingCurve::InOutSine);
	opacityAnimation->setDuration(250);
	opacityAnimation->setStartValue(m_MaskAlpha);
	opacityAnimation->setEndValue(endValue);
	opacityAnimation->start(QAbstractAnimation::DeleteWhenStopped);
}

void CUVMaskWidget::paintEvent(QPaintEvent* event) {
	QPainter painter(this);
	painter.save();
	painter.setPen(Qt::NoPen);
	painter.setRenderHint(QPainter::Antialiasing);
	painter.setBrush(QColor(0x00, 0x00, 0x00, m_MaskAlpha));
	painter.drawRect(rect());
	painter.restore();
}
