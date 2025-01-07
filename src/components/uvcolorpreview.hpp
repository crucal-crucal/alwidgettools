#pragma once

#include <QWidget>

#include "alwidgettoolsdef.hpp"

class CUVColorPreview final : public QWidget {
	Q_OBJECT

public:
	explicit CUVColorPreview(QWidget* parent = nullptr);
	~CUVColorPreview() override;

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
