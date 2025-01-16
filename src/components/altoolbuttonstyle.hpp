#pragma once

#include <QProxyStyle>

#include "alwidgettoolsdef.hpp"

class QStyleOptionToolButton;

/**
 * @brief \namespace AL
 */
namespace AL {
class CALToolButtonStyle final : public QProxyStyle {
	Q_OBJECT
	Q_PROPERTY(qreal expandIconRotate MEMBER m_expandIconRotate NOTIFY sigExpandIconRotateChanged)

public:
	explicit CALToolButtonStyle(QStyle* style = nullptr);
	~CALToolButtonStyle() override;

	void drawComplexControl(ComplexControl cc, const QStyleOptionComplex* opt, QPainter* p, const QWidget* widget) const override;
	QSize sizeFromContents(ContentsType ct, const QStyleOption* opt, const QSize& contentsSize, const QWidget* w) const override;

	void setIsTransparent(const bool isTransparent) { m_isTransparent = isTransparent; }
	[[nodiscard]] bool getIsTransparent() const { return m_isTransparent; }

	void setBorderRadius(const int borderRadius) { m_borderRadius = borderRadius; }
	[[nodiscard]] int getBorderRadius() const { return m_borderRadius; }

	void setExpandIconRotate(const qreal expandIconRotate) { m_expandIconRotate = expandIconRotate; }
	[[nodiscard]] qreal getExpandIconRotate() const { return m_expandIconRotate; }

	void setIsSelected(const bool isSelected) { m_isSelected = isSelected; }
	[[nodiscard]] bool getIsSelected() const { return m_isSelected; }

	void setALIconType(const ALIcon::IconType& iconType) { m_iconType = iconType; }
	[[nodiscard]] ALIcon::IconType getALIconType() const { return m_iconType; }

signals:
	void sigExpandIconRotateChanged();

private:
	ALThemeType::ThemeMode m_themeMode{};
	ALIcon::IconType m_iconType{};
	bool m_isTransparent{};
	int m_borderRadius{};
	qreal m_expandIconRotate{};
	bool m_isSelected{};
	int m_contentMargin{ 6 };
	int m_hasMenuIndicatorWidth{ 30 };

	void drawIndicator(QPainter* painter, const QStyleOptionToolButton* bopt, const QWidget* widget) const;
	void drawIcon(QPainter* painter, QRect iconRect, const QStyleOptionToolButton* bopt, const QWidget* widget) const;
	void drawText(QPainter* painter, QRect contentRect, const QStyleOptionToolButton* bopt) const;
	static qreal calculateExpandIndicatorWidth(const QStyleOptionToolButton* bopt, QPainter* painter);
};
}
