#pragma once

#include <QWidget>

#include "alwidgettoolsdef.hpp"

namespace AL {
class CALColorPreview final : public QWidget {
	Q_OBJECT

public:
	explicit CALColorPreview(QWidget* parent = nullptr);
	~CALColorPreview() override;

	void setBaseColor(const QColor& color);
	[[nodiscard]] QColor getBaseColor() const;

	void setThemeMode(const ALThemeType::ThemeMode& mode);
	[[nodiscard]] ALThemeType::ThemeMode getThemeMode() const;

protected:
	void paintEvent(QPaintEvent* event) override;

private:
	QColor m_baseColor{};
	ALThemeType::ThemeMode m_themeMode{};
};
} // namespace AL