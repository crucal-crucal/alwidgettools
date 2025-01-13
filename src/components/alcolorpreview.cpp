#include "alcolorpreview.hpp"

#include <QPainter>

#include "althememanager.hpp"

namespace AL {
/**
 * @brief \class CALColorPreview
 * @param parent pointer to the parent class
 */
CALColorPreview::CALColorPreview(QWidget* parent): QWidget(parent) {
	setFixedSize(50, 260);
	m_themeMode = ALTheme->getThemeMode();
}

CALColorPreview::~CALColorPreview() = default;

void CALColorPreview::setBaseColor(const QColor& color) {
	m_baseColor = color;
}

QColor CALColorPreview::getBaseColor() const {
	return m_baseColor;
}

void CALColorPreview::setThemeMode(const ALThemeType::ThemeMode& mode) {
	m_themeMode = mode;
}

ALThemeType::ThemeMode CALColorPreview::getThemeMode() const {
	return m_themeMode;
}

void CALColorPreview::paintEvent(QPaintEvent* event) {
	constexpr int penWidth = 3;
	constexpr qreal borderRadius = 5.0;

	QPainter painter(this);
	painter.save();
	painter.setRenderHint(QPainter::Antialiasing);
	QRect viewRect = rect();
	viewRect.adjust(penWidth, penWidth, -penWidth, -penWidth);
	painter.setPen(QPen(ALThemeColor(m_themeMode, ALThemeType::BasicBorder), penWidth));
	painter.setBrush(m_baseColor);
	painter.drawRoundedRect(viewRect, borderRadius, borderRadius);
	painter.restore();
}
} // namespace AL