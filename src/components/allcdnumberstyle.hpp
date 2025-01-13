#pragma once

#include <QProxyStyle>

#include "alwidgettoolsdef.hpp"

namespace AL {
class CALLCDNumberStyle final : public QProxyStyle {
	Q_OBJECT

public:
	explicit CALLCDNumberStyle(QStyle* parent = nullptr);
	~CALLCDNumberStyle() override;

	void drawControl(ControlElement element, const QStyleOption* opt, QPainter* p, const QWidget* w) const override;

	void setIsTransparent(bool istransparent);
	[[nodiscard]] bool getIsTransparent() const;

private:
	ALThemeType::ThemeMode m_themeMode{};
	bool m_isTransparent{};
	qreal m_borderRadius{};
};

} // namespace AL
