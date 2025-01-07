#pragma once

#include <QWidget>

#include "alwidgettoolsdef.hpp"

class CALColorPicker final : public QWidget {
	Q_OBJECT

public:
	explicit CALColorPicker(QWidget* parent = nullptr);
	~CALColorPicker() override;

	void setThemeMode(const ALThemeType::ThemeMode& mode);
	[[nodiscard]] ALThemeType::ThemeMode getThemeMode() const;

	void setSelectedColor(const QColor& color);
	[[nodiscard]] QColor getSelectedColor() const;

Q_SIGNALS:
	Q_SIGNAL void sigSelectedColorChanged(const QColor& color);

protected:
	void mousePressEvent(QMouseEvent* event) override;
	void mouseReleaseEvent(QMouseEvent* event) override;
	void mouseMoveEvent(QMouseEvent* event) override;
	void paintEvent(QPaintEvent* event) override;

private:
	ALThemeType::ThemeMode m_themeMode{};

	QImage m_colorPickerImage{};
	QPoint m_selectedPoint{};
	QColor m_selectedColor{};

	QPoint adjustPointLimit(QPoint point) const;
};
