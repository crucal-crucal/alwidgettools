#pragma once

#include <QProxyStyle>

#include "alwidgettoolsdef.hpp"

class CUVLineEditStyle final : public QProxyStyle {
	Q_OBJECT

public:
	explicit CUVLineEditStyle(QStyle* style = nullptr);
	~CUVLineEditStyle() override;
	void drawPrimitive(PrimitiveElement pe, const QStyleOption* opt, QPainter* p, const QWidget* w) const override;

private:
	ALThemeType::ThemeMode m_themeMode{};
};
