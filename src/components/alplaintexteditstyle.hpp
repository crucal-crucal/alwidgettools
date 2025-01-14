#pragma once

#include <QProxyStyle>

#include "alwidgettoolsdef.hpp"

/**
 * @brief \namespace AL
 */
namespace AL {
class CALPlainTextEditStyle final : public QProxyStyle {
	Q_OBJECT
	Q_PROPERTY(qreal expandMarkWidth READ getExpandMarkWidth WRITE setExpandMarkWidth NOTIFY sigExpandMarkWidthChanged)

public:
	explicit CALPlainTextEditStyle(QStyle* style = nullptr);
	~CALPlainTextEditStyle() override;

	void drawControl(ControlElement element, const QStyleOption* opt, QPainter* p, const QWidget* w) const override;

	void setExpandMarkWidth(qreal expandMarkWidth);
	[[nodiscard]] qreal getExpandMarkWidth() const;

Q_SIGNALS:
	Q_SIGNAL void sigExpandMarkWidthChanged();

private:
	ALThemeType::ThemeMode m_themeMode{};
	qreal m_expandMarkWidth{};
	qreal m_borderRadius{};
};
}
