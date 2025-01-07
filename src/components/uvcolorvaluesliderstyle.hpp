#pragma once

#include <QProxyStyle>

#include "alwidgettoolsdef.hpp"

class QLinearGradient;

class CUVColorValueSliderStyle final : public QProxyStyle {
	Q_OBJECT

public:
	explicit CUVColorValueSliderStyle(QStyle* style = nullptr);
	~CUVColorValueSliderStyle() override;

	void setBaseColor(const QColor& color);
	[[nodiscard]] QColor getBaseColor() const;

	void drawComplexControl(ComplexControl cc, const QStyleOptionComplex* opt, QPainter* p, const QWidget* widget) const override;
	int pixelMetric(PixelMetric metric, const QStyleOption* option, const QWidget* widget) const override;
	int styleHint(StyleHint stylehint, const QStyleOption* opt, const QWidget* widget, QStyleHintReturn* returnData) const override;

private:
	ALThemeType::ThemeMode m_themeMode{};
	QColor m_baseColor{};
	mutable QStyle::State m_lastState{};
	mutable qreal m_circleRadius{};
	QLinearGradient* m_baseGradient{ nullptr };

	void startRadiusAnimation(qreal startRadius, qreal endRadius, QWidget* widget) const;
};
