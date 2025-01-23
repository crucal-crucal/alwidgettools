#include "alsliderstyle.hpp"

#include <QPainter>
#include <QPropertyAnimation>

/**
 * @brief \namespace AL
 */
namespace AL {
/**
 * @brief \class CALSliderStyle
 * @param style pointer to the parent style
 */
CALSliderStyle::CALSliderStyle(QStyle* style) {
	setProperty("circleRadius", 0.01);

	m_lastState = QStyle::State_None;
	m_circleRadius = 0;
	// 默认颜色设置
	m_SliderTrackColor = QColor(217, 217, 217);                  // 滑槽默认颜色
	m_SliderTrackDisabledColor = QColor(235, 235, 235);          // 滑槽禁用颜色
	m_SliderTrackHoverColor = QColor(51, 145, 255);              // 滑槽滑过颜色
	m_SliderTrackHoverDisabledColor = QColor(51, 145, 255, 120); // 滑槽滑过禁用颜色

	m_SliderHandleBorderColor = QColor(200, 200, 200);             // 滑块边框颜色
	m_SliderHandleBorderDisabledColor = QColor(127, 127, 127);     // 滑块边框禁用颜色
	m_SliderHandleInnerColor = QColor(51, 145, 255);               // 滑块内圆颜色
	m_SliderHandleOuterColor = QColor(229, 229, 229);              // 滑块外圆颜色
	m_SliderHandleInnerDisabledColor = QColor(51, 145, 255, 60);   // 禁用状态下的内圆颜色
	m_SliderHandleOuterDisabledColor = QColor(229, 229, 229, 120); // 禁用状态下的外圆颜色
}

CALSliderStyle::~CALSliderStyle() = default;

void CALSliderStyle::drawComplexControl(const ComplexControl cc, const QStyleOptionComplex* opt, QPainter* p, const QWidget* widget) const {
	switch (cc) {
		case QStyle::CC_Slider: {
			if (const auto sopt = qstyleoption_cast<const QStyleOptionSlider*>(opt)) {
				p->save();
				p->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
				const QRect sliderRect = sopt->rect;
				QRect sliderHandleRect = subControlRect(cc, sopt, SC_SliderHandle, widget);
				sliderHandleRect.adjust(1, 1, -1, -1);
				const bool isEnabled = sopt->state.testFlag(QStyle::State_Enabled);
				/// 滑槽
				drawSliderTrack(p, sliderRect, sliderHandleRect, isEnabled, sopt);
				/// 滑块
				drawSliderHandle(p, sliderHandleRect, isEnabled, sopt, const_cast<QWidget*>(widget));
				p->restore();
			}
			return;
		}
		default: break;
	}

	QProxyStyle::drawComplexControl(cc, opt, p, widget);
}

int CALSliderStyle::pixelMetric(const PixelMetric metric, const QStyleOption* option, const QWidget* widget) const {
	switch (metric) {
		case QStyle::PM_SliderLength:
		case QStyle::PM_SliderThickness: {
			return 20;
		}
		default: break;
	}

	return QProxyStyle::pixelMetric(metric, option, widget);
}

int CALSliderStyle::styleHint(const StyleHint stylehint, const QStyleOption* opt, const QWidget* widget, QStyleHintReturn* returnData) const {
	if (stylehint == QStyle::SH_Slider_AbsoluteSetButtons) {
		return Qt::LeftButton;
	}

	return QProxyStyle::styleHint(stylehint, opt, widget, returnData);
}

void CALSliderStyle::startRadiusAnimation(const qreal startRadius, const qreal endRadius, QWidget* widget) const {
	const auto style = const_cast<CALSliderStyle*>(this);
	const auto circleRadiusAnimation = new QPropertyAnimation(style, "circleRadius");
	connect(circleRadiusAnimation, &QPropertyAnimation::valueChanged, style, [=](const QVariant& value) {
		this->m_circleRadius = value.toReal();
		widget->update();
	});
	circleRadiusAnimation->setEasingCurve(QEasingCurve::InOutSine);
	circleRadiusAnimation->setStartValue(startRadius);
	circleRadiusAnimation->setEndValue(endRadius);
	circleRadiusAnimation->start(QAbstractAnimation::DeleteWhenStopped);
}

void CALSliderStyle::drawSliderTrack(QPainter* p, const QRect& sliderRect, const QRect& sliderHandleRect, const bool isEnabled, const QStyleOptionSlider* sopt) const {
	p->setPen(Qt::NoPen);

	const QColor trackColor = isEnabled ? m_SliderTrackColor : m_SliderTrackDisabledColor;
	const QColor hoverColor = isEnabled ? m_SliderTrackHoverColor : m_SliderTrackHoverDisabledColor;
	p->setBrush(trackColor);
	if (sopt->orientation == Qt::Horizontal) {
		// 未滑过
		p->drawRoundedRect(QRect(sliderRect.x() + sliderRect.height() / 8, sliderRect.y() + static_cast<int>(sliderRect.height() * 0.375), sliderRect.width() - sliderRect.height() / 4, sliderRect.height() / 4), sliderRect.height() / 8.0, sliderRect.height() / 8.0);
		// 滑过
		p->setBrush(hoverColor);
		p->drawRoundedRect(QRect(sliderRect.x() + sliderRect.height() / 8, sliderRect.y() + static_cast<int>(sliderRect.height() * 0.375), sliderHandleRect.x(), sliderRect.height() / 4), sliderRect.height() / 8.0, sliderRect.height() / 8.0);
	} else {
		// 未滑过
		p->drawRoundedRect(QRect(sliderRect.x() + static_cast<int>(sliderRect.width() * 0.375), sliderRect.y() + sliderRect.width() / 8, sliderRect.width() / 4, sliderRect.height() - sliderRect.width() / 4), sliderRect.width() / 8.0, sliderRect.width() / 8.0);
		// 已滑过
		p->setBrush(hoverColor);
		p->drawRoundedRect(QRect(sliderRect.x() + static_cast<int>(sliderRect.width() * 0.375), sliderHandleRect.y(), sliderRect.width() / 4, sliderRect.height() - sliderRect.width() / 8 - sliderHandleRect.y()), sliderRect.width() / 8.0, sliderRect.width() / 8.0);
	}
}

void CALSliderStyle::drawSliderHandle(QPainter* p, const QRect& sliderHandleRect, const bool isEnabled, const QStyleOptionSlider* sopt, QWidget* widget) const {
	// 外圆
	p->setPen(isEnabled ? m_SliderHandleBorderColor : m_SliderHandleBorderDisabledColor);
	p->setBrush(isEnabled ? m_SliderHandleOuterColor : m_SliderHandleOuterDisabledColor);
	p->drawEllipse(QPointF(sliderHandleRect.center().x() + 1, sliderHandleRect.center().y() + 1), sliderHandleRect.width() / 2.0, sliderHandleRect.width() / 2.0);

	// 内圆
	if (isEnabled) {
		p->setPen(Qt::NoPen);
		p->setBrush(m_SliderHandleInnerColor);

		if (m_lastState.testFlag(QStyle::State_None)) {
			m_lastState = sopt->state;
		}

		if (m_circleRadius == 0) {
			m_circleRadius = sliderHandleRect.width() / 3.8;
		}

		if (sopt->activeSubControls == SC_SliderHandle) {
			if (sopt->state.testFlag(QStyle::State_Sunken)) {
				if (sopt->state.testFlag(QStyle::State_MouseOver)) {
					if (!m_lastState.testFlag(QStyle::State_Sunken)) {
						m_lastState = sopt->state;
					}
					p->drawEllipse(QPointF(sliderHandleRect.center().x() + 1, sliderHandleRect.center().y() + 1), m_circleRadius, m_circleRadius);
				}
			} else {
				if (sopt->state.testFlag(QStyle::State_MouseOver)) {
					if (!m_lastState.testFlag(QStyle::State_MouseOver) || m_lastState.testFlag(QStyle::State_Sunken)) {
						startRadiusAnimation(m_circleRadius, sliderHandleRect.width() / 2.4, widget);
						m_lastState = sopt->state;
					}
					p->drawEllipse(QPointF(sliderHandleRect.center().x() + 1, sliderHandleRect.center().y() + 1), m_circleRadius, m_circleRadius);
				}
			}
		} else {
			if (m_lastState.testFlag(QStyle::State_MouseOver) || m_lastState.testFlag(QStyle::State_Sunken)) {
				startRadiusAnimation(m_circleRadius, sliderHandleRect.width() / 3.8, widget);
				m_lastState &= ~QStyle::State_MouseOver;
				m_lastState &= ~QStyle::State_Sunken;
			}
			p->drawEllipse(QPointF(sliderHandleRect.center().x() + 1, sliderHandleRect.center().y() + 1), m_circleRadius, m_circleRadius);
		}
	} else {
		p->setPen(Qt::NoPen);
		p->setBrush(m_SliderHandleInnerDisabledColor);
		p->drawEllipse(QPointF(sliderHandleRect.center().x() + 1, sliderHandleRect.center().y() + 1), sliderHandleRect.width() / 3.8, sliderHandleRect.width() / 3.8);
	}
}
}
