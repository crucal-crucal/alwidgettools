#pragma once

#include <QProxyStyle>

#include "alwidgettoolsdef.hpp"

namespace AL {
class CALRadioButtonStyle final : public QProxyStyle {
	Q_OBJECT

public:
	explicit CALRadioButtonStyle(QStyle* style = nullptr);
	~CALRadioButtonStyle() override;

	void drawPrimitive(PrimitiveElement pe, const QStyleOption* opt, QPainter* p, const QWidget* w) const override;
	int pixelMetric(PixelMetric metric, const QStyleOption* option, const QWidget* widget) const override;

private:
	ALThemeType::ThemeMode m_themeMode{};
};

} // namespace AL
