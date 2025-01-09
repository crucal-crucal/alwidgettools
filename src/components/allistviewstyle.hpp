#pragma once

#include <QProxyStyle>

#include "alwidgettoolsdef.hpp"

class CALListViewStyle final : public QProxyStyle {
	Q_OBJECT
	Q_PROPERTY(int itemHeight MEMBER m_itemHeight NOTIFY sigItemHeightChanged)
	Q_PROPERTY(bool isTransparent MEMBER m_isTransparent NOTIFY sigIsTransparentChanged)

public:
	explicit CALListViewStyle(QStyle* style = nullptr);
	~CALListViewStyle() override;

	void drawPrimitive(PrimitiveElement pe, const QStyleOption* opt, QPainter* p, const QWidget* w) const override;
	void drawControl(ControlElement element, const QStyleOption* opt, QPainter* p, const QWidget* w) const override;
	QSize sizeFromContents(ContentsType ct, const QStyleOption* opt, const QSize& contentsSize, const QWidget* w) const override;

	void setItemHeight(int itemHeight);
	[[nodiscard]] int getItemHeight() const;

	void setIsTransparent(bool isTransparent);
	[[nodiscard]] bool getIsTransparent() const;

Q_SIGNALS:
	Q_SIGNAL void sigItemHeightChanged();
	Q_SIGNAL void sigIsTransparentChanged();

private:
	ALThemeType::ThemeMode m_themeMode{};
	int m_leftPadding{};
	int m_itemHeight{};
	bool m_isTransparent{};
};
