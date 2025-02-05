#pragma once

#include <QProxyStyle>

#include "alwidgettoolsdef.hpp"

/**
 * @brief \namespace AL
 */
namespace AL {
class CALTableViewStyle final : public QProxyStyle {
	Q_OBJECT
	Q_PROPERTY(int headerMargin MEMBER m_headerMargin NOTIFY sigHeaderMarginChanged)
	Q_PROPERTY(int currentHoverRow MEMBER m_currentHoverRow NOTIFY sigCurrentHoverRowChanged)

public:
	explicit CALTableViewStyle(QStyle* style = nullptr);
	~CALTableViewStyle() override;

	void drawPrimitive(PrimitiveElement pe, const QStyleOption* opt, QPainter* p, const QWidget* w) const override;
	void drawControl(ControlElement element, const QStyleOption* opt, QPainter* p, const QWidget* w) const override;
	int pixelMetric(PixelMetric metric, const QStyleOption* option, const QWidget* widget) const override;

	void setHeaderMargin(int margin);
	[[nodiscard]] int getHeaderMargin() const;

	void setCurrentHoverRow(int row);
	[[nodiscard]] int getCurrentHoverRow() const;

Q_SIGNALS:
	 void sigHeaderMarginChanged();
	 void sigCurrentHoverRowChanged();

private:
	ALThemeType::ThemeMode m_themeMode{};
	int m_headerMargin{};
	int m_currentHoverRow{};
	int m_horizontalPadding{};
};
}
