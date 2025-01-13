#pragma once

#include <QProxyStyle>

#include "alwidgettoolsdef.hpp"

namespace AL {
class CALSliderStyle final : public QProxyStyle {
	Q_OBJECT

public:
	explicit CALSliderStyle(QStyle* style = nullptr);
	~CALSliderStyle() override;

	void drawComplexControl(ComplexControl cc, const QStyleOptionComplex* opt, QPainter* p, const QWidget* widget) const override;
	int pixelMetric(PixelMetric metric, const QStyleOption* option, const QWidget* widget) const override;
	int styleHint(StyleHint stylehint, const QStyleOption* opt, const QWidget* widget, QStyleHintReturn* returnData) const override;

	/// 滑槽
	void setSliderTrackColor(const QColor& color) { m_SliderTrackColor = color; }
	void setSliderTrackDisabledColor(const QColor& color) { m_SliderTrackDisabledColor = color; }
	void setSliderTrackHoverColor(const QColor& color) { m_SliderTrackHoverColor = color; }
	void setSliderTrackHoverDisabledColor(const QColor& color) { m_SliderTrackHoverDisabledColor = color; }

	/// 滑块
	void setSliderHandleBorderColor(const QColor& color) { m_SliderHandleBorderColor = color; }
	void setSliderHandleBorderDisabledColor(const QColor& color) { m_SliderHandleBorderDisabledColor = color; }
	void setSliderHandleInnerColor(const QColor& color) { m_SliderHandleInnerColor = color; }
	void setSliderHandleInnerDisabledColor(const QColor& color) { m_SliderHandleInnerDisabledColor = color; }
	void setSliderHandleOuterColor(const QColor& color) { m_SliderHandleOuterColor = color; }
	void setSliderHandleOuterDisabledColor(const QColor& color) { m_SliderHandleOuterDisabledColor = color; }

private:
	mutable QStyle::State m_lastState{};
	mutable qreal m_circleRadius{ 0 };

	/// 滑槽
	QColor m_SliderTrackColor{};              // 滑槽默认颜色
	QColor m_SliderTrackDisabledColor{};      // 滑槽禁用颜色
	QColor m_SliderTrackHoverColor{};         // 滑槽滑过颜色
	QColor m_SliderTrackHoverDisabledColor{}; // 滑槽滑过禁用颜色

	/// 滑块
	QColor m_SliderHandleBorderColor{};         // 滑块边框颜色
	QColor m_SliderHandleBorderDisabledColor{}; // 滑块边框禁用颜色
	QColor m_SliderHandleInnerColor{};          // 滑块内圆颜色
	QColor m_SliderHandleInnerDisabledColor{};  // 滑块内圆禁用颜色
	QColor m_SliderHandleOuterColor{};          // 滑块外圆颜色
	QColor m_SliderHandleOuterDisabledColor{};  // 滑块外圆禁用颜色

	void _startRadiusAnimation(qreal startRadius, qreal endRadius, QWidget* widget) const;
};

} // namespace AL
