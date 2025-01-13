#include "alcalendarpicker.hpp"

#include <QDate>
#include <QPainter>
#include <QPropertyAnimation>
#include <QVBoxLayout>

#include "alcalendar.hpp"
#include "alcalendarpickercontainer.hpp"
#include "alcalendarpicker_p.hpp"
#include "althememanager.hpp"

namespace AL {
/**
 * @brief \class CALCalendarPickerPrivate
 * Internal class for CalCalendarPicker
 * @param q pointer to the public class
 * @param parent pointer to the parent class
 */
CALCalendarPickerPrivate::CALCalendarPickerPrivate(CALCalendarPicker* q, QObject* parent): QObject(parent), q_ptr(q) {
}

CALCalendarPickerPrivate::~CALCalendarPickerPrivate() = default;

void CALCalendarPickerPrivate::slotCalendarPickerClicked() {
	Q_Q(CALCalendarPicker);

	const QPoint endPoint(q->mapToGlobal(QPoint((q->width() - calendarPickerContainer->width()) / 2, q->height() + 5)));
	const auto showAnimation = new QPropertyAnimation(calendarPickerContainer, "pos");
	showAnimation->setEasingCurve(QEasingCurve::OutCubic);
	showAnimation->setDuration(250);
	showAnimation->setStartValue(QPoint(endPoint.x(), endPoint.y() - 10));
	showAnimation->setEndValue(endPoint);
	calendarPickerContainer->show();
	showAnimation->start(QAbstractAnimation::DeleteWhenStopped);
}

void CALCalendarPickerPrivate::slotCanendarSelectedDateChanged() {
	Q_EMIT q_func()->sigSelectedDateChanged(calendar->getSelectedDate());
	if (calendarPickerContainer->isVisible()) {
		calendarPickerContainer->hide();
	}
}

/**
 * @brief \class CALCalendarPicker
 * @param parent pointer to the parent class
 */
CALCalendarPicker::CALCalendarPicker(QWidget* parent): QPushButton(parent), d_ptr(new CALCalendarPickerPrivate(this, this)) {
	Q_D(CALCalendarPicker);

	d->borderRadius = 3;
	d->calendarPickerContainer = new CALCalendarPickerContainer(this);
	d->calendarPickerContainer->resize(317, 352);
	d->calendar = new CALCalendar(d->calendarPickerContainer);
	connect(d->calendar, &CALCalendar::sigSelectedDateChanged, d, &CALCalendarPickerPrivate::slotCanendarSelectedDateChanged);
	d->calendarPickerContainer->hide();
	d->themeMode = ALTheme->getThemeMode();
	connect(ALTheme, &CALThemeManager::sigThemeModeChanged, this, [=](const ALThemeType::ThemeMode& mode) { d->themeMode = mode; });

	const auto containerVLayout = new QVBoxLayout(d->calendarPickerContainer);
	containerVLayout->setContentsMargins(6, 6, 6, 6);
	containerVLayout->addWidget(d->calendar);

	setFixedSize(120, 30);
	setObjectName("CALCalendarPicker");
	setMouseTracking(true);
	connect(this, &CALCalendarPicker::clicked, d, &CALCalendarPickerPrivate::slotCalendarPickerClicked);
}

CALCalendarPicker::~CALCalendarPicker() = default;

void CALCalendarPicker::setSelectedDate(const QDate& date) {
	d_func()->calendar->setSelectedDate(date);
	Q_EMIT sigSelectedDateChanged(date);
}

QDate CALCalendarPicker::getSelectedDate() const {
	return d_func()->calendar->getSelectedDate();
}

void CALCalendarPicker::setBorderRadius(const qreal borderRadius) {
	d_func()->borderRadius = borderRadius;
	Q_EMIT sigBorderRadiusChanged();
}

qreal CALCalendarPicker::getBorderRadius() const {
	return d_func()->borderRadius;
}

void CALCalendarPicker::paintEvent(QPaintEvent* event) {
	Q_D(CALCalendarPicker);

	QPainter painter(this);
	painter.save();
	painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);
	/// background & border
	QRect baseRect = rect();
	baseRect.adjust(1, 1, -1, -1);
	painter.setPen(ALThemeColor(d->themeMode, ALThemeType::BasicBorder));
	painter.setBrush(ALThemeColor(d->themeMode, underMouse() ? ALThemeType::BasicHover : ALThemeType::BasicBase));
	painter.drawRoundedRect(baseRect, d->borderRadius, d->borderRadius);
	/// date
	const QDate selectedDate = getSelectedDate();
	const QString date = QString("%1/%2/%3").arg(selectedDate.year()).arg(selectedDate.month()).arg(selectedDate.day());
	painter.setPen(ALThemeColor(d->themeMode, ALThemeType::BasicText));
	QRect textRect = baseRect;
	textRect.adjust(10, 0, 0, 0);
	painter.drawText(textRect, Qt::AlignVCenter | Qt::AlignLeft, date);
	// icon
	QFont iconFont("CALAwesome");
	iconFont.setPixelSize(17);
	painter.setFont(iconFont);
	painter.drawText(QRect(baseRect.right() - 25, 0, 15, height()), Qt::AlignVCenter | Qt::AlignRight, QChar(static_cast<unsigned short>(ALIcon::AweSomeIcon::CalendarRange)));
	painter.restore();
}

void CALCalendarPicker::showEvent(QShowEvent* event) {
	setSelectedDate(QDate::currentDate());
	QPushButton::showEvent(event);
}

} // namespace AL
