#pragma once

#include <QProxyStyle>

#include "alwidgettoolsdef.hpp"

class CUVMainWindowStyle final : public QProxyStyle {
	Q_OBJECT

public:
	explicit CUVMainWindowStyle(QStyle* style = nullptr);
	~CUVMainWindowStyle() override;

	void drawPrimitive(PrimitiveElement pe, const QStyleOption* opt, QPainter* p, const QWidget* w) const override;
	void drawControl(ControlElement element, const QStyleOption* opt, QPainter* p, const QWidget* w) const override;

private:
	ALThemeType::ThemeMode m_themeMode{};
};
