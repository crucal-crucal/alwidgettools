#pragma once

#include <QModelIndex>
#include <QProxyStyle>

#include "alwidgettoolsdef.hpp"

/**
 * @brief \namespace AL
 */
namespace AL {
class CALPovitStyle final : public QProxyStyle {
	Q_OBJECT

public:
	explicit CALPovitStyle(QStyle* style = nullptr);
	~CALPovitStyle() override;

	void drawPrimitive(PrimitiveElement pe, const QStyleOption* opt, QPainter* p, const QWidget* w) const override;
	void drawControl(ControlElement element, const QStyleOption* opt, QPainter* p, const QWidget* w) const override;
	int pixelMetric(PixelMetric metric, const QStyleOption* option, const QWidget* widget) const override;

	[[nodiscard]] const QColor& getMarkColor() const;

	void setPressIndex(const QModelIndex& index);
	[[nodiscard]] QModelIndex getPressIndex() const;

	void setCurrentIndex(int index);
	[[nodiscard]] int getCurrentIndex() const;

	void setPovitSpacing(int spacing);
	[[nodiscard]] int getPovitSpacing() const;

private:
	ALThemeType::ThemeMode m_themeMode{};
	QModelIndex m_pressIndex{};
	int m_currentIndex{};
	int m_povitSpacing{};
};
}
