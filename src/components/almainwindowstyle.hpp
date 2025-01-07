#pragma once

#include <QProxyStyle>

#include "alwidgettoolsdef.hpp"

class CALMainWindowStyle final : public QProxyStyle {
	Q_OBJECT

public:
	explicit CALMainWindowStyle(QStyle* style = nullptr);
	~CALMainWindowStyle() override;

	void drawPrimitive(PrimitiveElement pe, const QStyleOption* opt, QPainter* p, const QWidget* w) const override;
	void drawControl(ControlElement element, const QStyleOption* opt, QPainter* p, const QWidget* w) const override;

private:
	ALThemeType::ThemeMode m_themeMode{};
};
