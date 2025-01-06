#include "uvcolorpreview.hpp"

#include <QPainter>

#include "uvthememanager.hpp"

/**
 * @brief \class CUVColorPreview
 * @param parent pointer to the parent class
 */
CUVColorPreview::CUVColorPreview(QWidget* parent): QWidget(parent) {
	setFixedSize(50, 260);
	m_themeMode = UVTheme->getThemeMode();
}

CUVColorPreview::~CUVColorPreview() = default;

void CUVColorPreview::setBaseColor(const QColor& color) {
	m_baseColor = color;
}

QColor CUVColorPreview::getBaseColor() const {
	return m_baseColor;
}

void CUVColorPreview::setThemeMode(const UVThemeType::ThemeMode& mode) {
	m_themeMode = mode;
}

UVThemeType::ThemeMode CUVColorPreview::getThemeMode() const {
	return m_themeMode;
}

void CUVColorPreview::paintEvent(QPaintEvent* event) {
	constexpr int penWidth = 3;
	constexpr qreal borderRadius = 5.0;

	QPainter painter(this);
	painter.save();
	painter.setRenderHint(QPainter::Antialiasing);
	QRect viewRect = rect();
	viewRect.adjust(penWidth, penWidth, -penWidth, -penWidth);
	painter.setPen(QPen(UVThemeColor(m_themeMode, UVThemeType::BasicBorder), penWidth));
	painter.setBrush(m_baseColor);
	painter.drawRoundedRect(viewRect, borderRadius, borderRadius);
	painter.restore();
}
