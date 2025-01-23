#include "alscrollpagearea.hpp"

#include <QPainter>

#include "alscrollpagearea_p.hpp"
#include "althememanager.hpp"

/**
 * @brief \namespace AL
 */
namespace AL {
/**
 * @brief \class CALScrollPageAreaPrivate
 * Internal class for CALScrollPageArea
 * @param q pointer to the public class
 * @param parent pointer to the parent class
 */
CALScrollPageAreaPrivate::CALScrollPageAreaPrivate(CALScrollPageArea* q, QObject* parent): QObject(parent), q_ptr(q) {
}

CALScrollPageAreaPrivate::~CALScrollPageAreaPrivate() = default;

/**
 * @brief \class CALScrollPageArea
 * @param parent pointer to the parent class
 */
CALScrollPageArea::CALScrollPageArea(QWidget* parent): QWidget(parent), d_ptr(new CALScrollPageAreaPrivate(this, this)) {
	Q_D(CALScrollPageArea);

	d->borderRadius = 6;
	setFixedHeight(75);
	d->themeMode = ALTheme->getThemeMode();
	connect(ALTheme, &CALThemeManager::sigThemeModeChanged, this, [=](const ALThemeType::ThemeMode& mode) { d->themeMode = mode; });
}

CALScrollPageArea::~CALScrollPageArea() = default;

void CALScrollPageArea::setBorderRadius(const qreal borderRadius) {
	d_func()->borderRadius = borderRadius;
	Q_EMIT sigBorderRadiusChanged();
}

qreal CALScrollPageArea::getBorderRadius() const {
	return d_func()->borderRadius;
}

void CALScrollPageArea::paintEvent(QPaintEvent* event) {
	Q_D(CALScrollPageArea);

	QPainter painter(this);
	painter.save();
	painter.setRenderHint(QPainter::Antialiasing);
	painter.setPen(ALThemeColor(d->themeMode, ALThemeType::BasicBorder));
	painter.setBrush(ALThemeColor(d->themeMode, ALThemeType::BasicBaseAlpha));
	const QRect foregroundRect(1, 1, width() - 2, height() - 2);
	painter.drawRoundedRect(foregroundRect, d->borderRadius, d->borderRadius);
	painter.restore();
}
}
