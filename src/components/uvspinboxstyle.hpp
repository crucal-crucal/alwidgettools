#pragma once

#include <QProxyStyle>

#include "alwidgettoolsdef.hpp"

class CUVSpinBoxStyle final : public QProxyStyle {
	Q_OBJECT

public:
	explicit CUVSpinBoxStyle(QStyle* style = nullptr);
	~CUVSpinBoxStyle() override;

	void drawComplexControl(ComplexControl cc, const QStyleOptionComplex* opt, QPainter* p, const QWidget* widget) const override;
	QRect subControlRect(ComplexControl cc, const QStyleOptionComplex* opt, SubControl sc, const QWidget* widget) const override;

private:
	ALThemeType::ThemeMode m_themeMode{};
};
