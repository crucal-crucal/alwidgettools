#pragma once

#include <QWidget>

#include "uvwidgettoolsdef.hpp"

class CUVColorPreview final : public QWidget {
	Q_OBJECT

public:
	explicit CUVColorPreview(QWidget* parent = nullptr);
	~CUVColorPreview() override;

	void setBaseColor(const QColor& color);
	[[nodiscard]] QColor getBaseColor() const;

	void setThemeMode(const UVThemeType::ThemeMode& mode);
	[[nodiscard]] UVThemeType::ThemeMode getThemeMode() const;

protected:
	void paintEvent(QPaintEvent* event) override;

private:
	QColor m_baseColor{};
	UVThemeType::ThemeMode m_themeMode{};
};
