#pragma once

#include <QProxyStyle>

#include "alwidgettoolsdef.hpp"

namespace AL {
class CALComboBoxStyle final : public QProxyStyle {
	Q_OBJECT

	Q_PROPERTY(qreal expandIconRotate READ getExpandIconRotate WRITE setExpandIconRotate NOTIFY sigExpandIconRotateChanged)
	Q_PROPERTY(qreal expandMarkWidth READ getExpandMarkWidth WRITE setExpandMarkWidth NOTIFY sigExpandMarkWidthChanged)

public:
	explicit CALComboBoxStyle(QStyle* style = nullptr);
	~CALComboBoxStyle() override;

	void drawPrimitive(PrimitiveElement element, const QStyleOption* option, QPainter* painter, const QWidget* widget) const override;
	void drawControl(ControlElement element, const QStyleOption* option, QPainter* painter, const QWidget* widget) const override;
	void drawComplexControl(ComplexControl control, const QStyleOptionComplex* option, QPainter* painter, const QWidget* widget) const override;

	QRect subControlRect(ComplexControl cc, const QStyleOptionComplex* opt, SubControl sc, const QWidget* widget) const override;
	QSize sizeFromContents(ContentsType type, const QStyleOption* option, const QSize& size, const QWidget* widget) const override;

	void setExpandIconRotate(qreal ExpandIconRotate);
	[[nodiscard]] qreal getExpandIconRotate() const;

	void setExpandMarkWidth(qreal ExpandMarkWidth);
	[[nodiscard]] qreal getExpandMarkWidth() const;

signals:
	void sigExpandIconRotateChanged();
	void sigExpandMarkWidthChanged();

private:
	ALThemeType::ThemeMode m_themeMode{};
	qreal m_expandIconRotate{};
	qreal m_expandMarkWidth{};
	int m_shadowBorderWidth{};
};
} // namespace AL