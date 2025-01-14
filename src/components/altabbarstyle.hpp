#pragma once

#include <QProxyStyle>

#include "alwidgettoolsdef.hpp"

/**
 * @brief \namespace AL
 */
namespace AL {
class CALTabBarStyle final : public QProxyStyle {
	Q_OBJECT

public:
	explicit CALTabBarStyle(QStyle* style = nullptr);
	~CALTabBarStyle() override;

	void drawPrimitive(PrimitiveElement pe, const QStyleOption* opt, QPainter* p, const QWidget* w) const override;
	void drawControl(ControlElement element, const QStyleOption* opt, QPainter* p, const QWidget* w) const override;
	QSize sizeFromContents(ContentsType ct, const QStyleOption* opt, const QSize& contentsSize, const QWidget* w) const override;
	QRect subElementRect(SubElement subElement, const QStyleOption* option, const QWidget* widget) const override;

private:
	ALThemeType::ThemeMode m_themeMode{};
};
}
