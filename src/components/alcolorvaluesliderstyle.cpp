#include "alcolorvaluesliderstyle.hpp"

#include <QPainter>
#include <QPropertyAnimation>
#include <QStyledItemDelegate>
#include <QStyleOptionSlider>

#include "althememanager.hpp"

/**
 * @brief \namespace AL
 */
namespace AL {
/**
 * @brief \class CALColorValueSliderStyle
 * @param style pointer to the parent style
 */
CALColorValueSliderStyle::CALColorValueSliderStyle(QStyle* style) {
	m_baseGradient = new QLinearGradient(0, 0, 100, 100);
	setProperty("circleRadius", 0.01);
	m_themeMode = ALTheme->getThemeMode();
	connect(ALTheme, &CALThemeManager::sigThemeModeChanged, this, [this](const ALThemeType::ThemeMode& mode) { m_themeMode = mode; });
}

CALColorValueSliderStyle::~CALColorValueSliderStyle() = default;

void CALColorValueSliderStyle::setBaseColor(const QColor& color) {
	m_baseColor = color;
}

QColor CALColorValueSliderStyle::getBaseColor() const {
	return m_baseColor;
}

void CALColorValueSliderStyle::drawComplexControl(const ComplexControl cc, const QStyleOptionComplex* opt, QPainter* p, const QWidget* widget) const {
	switch (cc) {
		case QStyle::CC_Slider: {
			if (const auto sopt = qstyleoption_cast<const QStyleOptionSlider*>(opt)) {
				m_baseGradient->setFinalStop(widget->rect().bottomRight());
				QColor highValueColor = m_baseColor.toHsv();
				highValueColor.setHsv(highValueColor.hue(), highValueColor.saturation(), 255);
				QColor lowValueColor = m_baseColor.toHsv();
				lowValueColor.setHsv(lowValueColor.hue(), lowValueColor.saturation(), 0);
				m_baseGradient->setColorAt(0, highValueColor);
				m_baseGradient->setColorAt(1, lowValueColor);
				p->save();
				p->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
				QRectF sliderRact = sopt->rect;
				sliderRact.adjust(1.5, 0, -1.5, 0);
				QRect sliderHandleRect = subControlRect(cc, sopt, SC_SliderHandle, widget);
				sliderHandleRect.adjust(1, 1, -1, -1);
				/// 滑槽
				p->setPen(Qt::NoPen);
				p->setBrush(m_baseColor);
				// 渐变背景
				p->drawRoundedRect(QRectF(sliderRact.x(), sliderRact.y() + sliderRact.width() / 8, sliderRact.width(), sliderRact.height() - sliderRact.width() / 4), sliderRact.width() / 2, sliderRact.width() / 2);
				/// 滑块
				// 外圆
				p->setPen(ALThemeColor(m_themeMode, ALThemeType::BasicBorder));
				p->setBrush(ALThemeColor(m_themeMode, ALThemeType::BasicBase));
				p->drawEllipse(QPointF(sliderHandleRect.center().x() + 1, sliderHandleRect.center().y() + 1), static_cast<int>(sliderHandleRect.width() / 2), static_cast<int>(sliderHandleRect.height() / 2));
				// 内圆
				p->setPen(Qt::NoPen);
				p->setBrush(ALThemeColor(m_themeMode, ALThemeType::PrimaryNormal));
				if (m_lastState == 0) {
					m_lastState = sopt->state;
				}
				if (m_circleRadius == 0) {
					m_circleRadius = sliderHandleRect.width() / 3.8;
				}
				if (sopt->activeSubControls == SC_SliderHandle) {
					if (sopt->state.testFlag(QStyle::State_Sunken)) {
						if (sopt->state.testFlag(QStyle::State_MouseOver)) {
							if (!m_lastState.testFlag(QStyle::State_Sunken)) {
								startRadiusAnimation(sliderHandleRect.width() / 2.8, sliderHandleRect.width() / 4.5, const_cast<QWidget*>(widget));
								m_lastState = sopt->state;
							}
							p->drawEllipse(QPointF(sliderHandleRect.center().x() + 1, sliderHandleRect.center().y() + 1), m_circleRadius, m_circleRadius);
						}
					} else {
						if (sopt->state.testFlag(QStyle::State_MouseOver)) {
							if (!m_lastState.testFlag(QStyle::State_MouseOver)) {
								startRadiusAnimation(m_circleRadius, sliderHandleRect.width() / 2.8, const_cast<QWidget*>(widget));
								m_lastState = sopt->state;
							}
							if (m_lastState.testFlag(QStyle::State_Sunken)) {
								startRadiusAnimation(m_circleRadius, sliderHandleRect.width() / 2.8, const_cast<QWidget*>(widget));
								m_lastState = sopt->state;
							}
							p->drawEllipse(QPointF(sliderHandleRect.center().x() + 1, sliderHandleRect.center().y() + 1), m_circleRadius, m_circleRadius);
						}
					}
				} else {
					if (m_lastState.testFlag(QStyle::State_Sunken) || m_lastState.testFlag(QStyle::State_MouseOver)) {
						startRadiusAnimation(m_circleRadius, sliderHandleRect.width() / 3.8, const_cast<QWidget*>(widget));
						m_lastState &= ~QStyle::State_Sunken;
						m_lastState &= ~QStyle::State_MouseOver;
					}
					p->drawEllipse(QPointF(sliderHandleRect.center().x() + 1, sliderHandleRect.center().y() + 1), m_circleRadius, m_circleRadius);
				}
				p->restore();
				return;
			}
		}
		default: {
			break;
		}
	}

	QProxyStyle::drawComplexControl(cc, opt, p, widget);
}

int CALColorValueSliderStyle::pixelMetric(const PixelMetric metric, const QStyleOption* option, const QWidget* widget) const {
	switch (metric) {
		case QStyle::PM_SliderLength: {
			return 20;
		}
		case QStyle::PM_SliderThickness: {
			return 16;
		}
		default: {
			break;
		}
	}

	return QProxyStyle::pixelMetric(metric, option, widget);
}

int CALColorValueSliderStyle::styleHint(const StyleHint stylehint, const QStyleOption* opt, const QWidget* widget, QStyleHintReturn* returnData) const {
	if (stylehint == QStyle::SH_Slider_AbsoluteSetButtons) {
		return Qt::LeftButton;
	}

	return QProxyStyle::styleHint(stylehint, opt, widget, returnData);
}

void CALColorValueSliderStyle::startRadiusAnimation(const qreal startRadius, const qreal endRadius, QWidget* widget) const {
	const auto style = const_cast<CALColorValueSliderStyle*>(this);
	const auto circleRadiusAnimatino = new QPropertyAnimation(style, "circleRadius");
	connect(circleRadiusAnimatino, &QPropertyAnimation::valueChanged, style, [this, widget](const QVariant& value) {
		m_circleRadius = value.toReal();
		widget->update();
	});
	circleRadiusAnimatino->setEasingCurve(QEasingCurve::InOutSine);
	circleRadiusAnimatino->setStartValue(startRadius);
	circleRadiusAnimatino->setEndValue(endRadius);
	circleRadiusAnimatino->start(QAbstractAnimation::DeleteWhenStopped);
}
}
