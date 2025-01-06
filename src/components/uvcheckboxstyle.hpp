#pragma once

#include <QProxyStyle>

#include "uvwidgettoolsdef.hpp"

class CUVCheckBoxStyle final : public QProxyStyle {
	Q_OBJECT

public:
	explicit CUVCheckBoxStyle(QStyle* style = nullptr);
	~CUVCheckBoxStyle() override;

	void drawControl(ControlElement element, const QStyleOption* opt, QPainter* p, const QWidget* w) const override;
	int pixelMetric(PixelMetric metric, const QStyleOption* option, const QWidget* widget) const override;

	void setCheckIndicatorWidth(int width);
	[[nodiscard]] int getCheckIndicatorWidth() const;

private:
	UVThemeType::ThemeMode m_themeMode{};
	int m_checkIndicatorWidth{};
	qreal m_borderRadius{};
};
