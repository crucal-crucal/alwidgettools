#include "uvscrollpagearea.hpp"

#include <QPainter>

#include "uvscrollpagearea_p.hpp"
#include "althememanager.hpp"

/**
 * @brief \class CUVScrollPageAreaPrivate
 * Internal class for CUVScrollPageArea
 * @param q pointer to the public class
 * @param parent pointer to the parent class
 */
CUVScrollPageAreaPrivate::CUVScrollPageAreaPrivate(CUVScrollPageArea* q, QObject* parent): QObject(parent), q_ptr(q) {

}

CUVScrollPageAreaPrivate::~CUVScrollPageAreaPrivate() = default;

/**
 * @brief \class CUVScrollPageArea
 * @param parent pointer to the parent class
 */
CUVScrollPageArea::CUVScrollPageArea(QWidget* parent): QWidget(parent), d_ptr(new CUVScrollPageAreaPrivate(this, this)) {
	Q_D(CUVScrollPageArea);

	d->borderRadius = 6;
	setFixedHeight(75);
	d->themeMode = UVTheme->getThemeMode();
	connect(UVTheme, &CUVThemeManager::sigThemeModeChanged, this, [=](const ALThemeType::ThemeMode& mode) { d->themeMode = mode; });
}

CUVScrollPageArea::~CUVScrollPageArea() = default;

void CUVScrollPageArea::setBorderRadius(const int borderRadius) {
	d_func()->borderRadius = borderRadius;
	Q_EMIT sigBorderRadiusChanged();
}

int CUVScrollPageArea::getBorderRadius() const {
	return d_func()->borderRadius;
}

void CUVScrollPageArea::paintEvent(QPaintEvent* event) {
	Q_D(CUVScrollPageArea);

	QPainter painter(this);
	painter.save();
	painter.setRenderHint(QPainter::Antialiasing);
	painter.setPen(UVThemeColor(d->themeMode, ALThemeType::BasicBorder));
	painter.setBrush(UVThemeColor(d->themeMode, ALThemeType::BasicBaseAlpha));
	const QRect foregroundRect(1, 1, width() - 2, height() - 2);
	painter.drawRoundedRect(foregroundRect, d->borderRadius, d->borderRadius);
	painter.restore();
}
