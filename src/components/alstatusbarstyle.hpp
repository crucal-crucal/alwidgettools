#pragma once

#include <QProxyStyle>

#include "alwidgettoolsdef.hpp"

namespace AL {
class CALStatusBarStyle final : public QProxyStyle {
	Q_OBJECT

public:
	explicit CALStatusBarStyle(QStyle* style = nullptr);
	~CALStatusBarStyle() override;

	void drawPrimitive(PrimitiveElement pe, const QStyleOption* opt, QPainter* p, const QWidget* w) const override;
	void drawControl(ControlElement element, const QStyleOption* opt, QPainter* p, const QWidget* w) const override;
	QSize sizeFromContents(ContentsType ct, const QStyleOption* opt, const QSize& contentsSize, const QWidget* w) const override;
	int pixelMetric(PixelMetric metric, const QStyleOption* option, const QWidget* widget) const override;

private:
	ALThemeType::ThemeMode m_themeMode{};
};

} // namespace AL
