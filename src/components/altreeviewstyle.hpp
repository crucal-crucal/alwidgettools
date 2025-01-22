#pragma once

#include <QProxyStyle>

#include "alwidgettoolsdef.hpp"

/**
 * @brief \namespace AL
 */
namespace AL {
class CALTreeViewStyle : public QProxyStyle {
	Q_OBJECT
	Q_PROPERTY(int itemHeight READ getItemHeight WRITE setItemHeight NOTIFY sigItemHeightChanged)
	Q_PROPERTY(int headerMargin READ getHeaderMargin WRITE setHeaderMargin NOTIFY sigHeaderMarginChanged)

public:
	explicit CALTreeViewStyle(QStyle* style = nullptr);
	~CALTreeViewStyle() override;

	void drawPrimitive(PrimitiveElement pe, const QStyleOption* opt, QPainter* p, const QWidget* w) const override;
	void drawControl(ControlElement element, const QStyleOption* opt, QPainter* p, const QWidget* w) const override;
	QSize sizeFromContents(ContentsType ct, const QStyleOption* opt, const QSize& contentsSize, const QWidget* w) const override;
	int pixelMetric(PixelMetric metric, const QStyleOption* option, const QWidget* widget) const override;
	QRect subElementRect(SubElement subElement, const QStyleOption* option, const QWidget* widget) const override;

	void setItemHeight(int itemHeight);
	[[nodiscard]] int getItemHeight() const;

	void setHeaderMargin(int headerMargin);
	[[nodiscard]] int getHeaderMargin() const;

Q_SIGNALS:
	Q_SIGNAL void sigItemHeightChanged();
	Q_SIGNAL void sigHeaderMarginChanged();

private:
	ALThemeType::ThemeMode m_themeMode{};
	int m_leftPadding{};
	int m_itemHeight{};
	int m_headerMargin{};
};
}
