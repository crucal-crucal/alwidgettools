#pragma once

#include <QProxyStyle>

#include "alwidgettoolsdef.hpp"

/**
 * @brief \namespace AL
 */
namespace AL {
class CALMenuStyle final : public QProxyStyle {
	Q_OBJECT

	Q_PROPERTY(int MenuItemHeight READ getMenuItemHeight WRITE setMenuItemHeight NOTIFY sigMenuItemHeightChanged)

public:
	explicit CALMenuStyle(QStyle* style = nullptr);
	~CALMenuStyle() override;
	void drawPrimitive(PrimitiveElement element, const QStyleOption* option, QPainter* painter, const QWidget* widget) const override;
	void drawControl(ControlElement element, const QStyleOption* option, QPainter* painter, const QWidget* widget) const override;
	int pixelMetric(PixelMetric metric, const QStyleOption* option, const QWidget* widget) const override;
	QSize sizeFromContents(ContentsType type, const QStyleOption* option, const QSize& size, const QWidget* widget) const override;

	void setMenuItemHeight(int height);
	[[nodiscard]] int getMenuItemHeight() const;

Q_SIGNALS:
	 void sigMenuItemHeightChanged();

private:
	ALThemeType::ThemeMode m_themeMode{};
	mutable bool m_isAnyoneItemHasIcon{ false };
	int m_shadowBorderWidth{};
	int m_iconWidth{};
	int m_menuItemHeight{};
};
}
