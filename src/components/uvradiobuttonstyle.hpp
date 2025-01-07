#pragma once

#include <QProxyStyle>

#include "alwidgettoolsdef.hpp"

class CUVRadioButtonStyle final : public QProxyStyle {
	Q_OBJECT

public:
	explicit CUVRadioButtonStyle(QStyle* style = nullptr);
	~CUVRadioButtonStyle() override;

	void drawPrimitive(PrimitiveElement pe, const QStyleOption* opt, QPainter* p, const QWidget* w) const override;
	int pixelMetric(PixelMetric metric, const QStyleOption* option, const QWidget* widget) const override;

private:
	ALThemeType::ThemeMode m_themeMode{};
};
