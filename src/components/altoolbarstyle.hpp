#pragma once

#include <QProxyStyle>

#include "alwidgettoolsdef.hpp"

class QStyleOptionToolButton;

class CALToolBarStyle final : public QProxyStyle {
	Q_OBJECT

public:
	explicit CALToolBarStyle(QStyle* style = nullptr);
	~CALToolBarStyle() override;

	void drawPrimitive(PrimitiveElement pe, const QStyleOption* opt, QPainter* p, const QWidget* w) const override;
	void drawControl(ControlElement element, const QStyleOption* opt, QPainter* p, const QWidget* w) const override;
	int pixelMetric(PixelMetric metric, const QStyleOption* option, const QWidget* widget) const override;

private:
	ALThemeType::ThemeMode m_themeMode{};

	void drawIndicator(QPainter* painter, const QStyleOptionToolButton* bopt, const QWidget* widget) const;
	void drawIcon(QPainter* painter, const QRect& iconRect, const QStyleOptionToolButton* bopt, const QWidget* widget) const;
	void drawText(QPainter* painter, const QRect& contentRect, const QStyleOptionToolButton* bopt) const;
};
