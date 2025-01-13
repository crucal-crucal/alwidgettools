#pragma once

#include <QProxyStyle>

#include "alwidgettoolsdef.hpp"

namespace AL {
class CALLineEditStyle final : public QProxyStyle {
	Q_OBJECT

public:
	explicit CALLineEditStyle(QStyle* style = nullptr);
	~CALLineEditStyle() override;
	void drawPrimitive(PrimitiveElement pe, const QStyleOption* opt, QPainter* p, const QWidget* w) const override;

private:
	ALThemeType::ThemeMode m_themeMode{};
};

} // namespace AL
