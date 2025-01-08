#include "allcdnumber.hpp"

#include <QDateTime>
#include <QTimer>

#include "allcdnumber_p.hpp"
#include "allcdnumberstyle.hpp"
#include "althememanager.hpp"

/**
 * @brief \class CALLCDNumberPrivate
 * Internal class for CALLCDNumber
 * @param q pointer to the public class
 * @param parent pointer to the parent class
 */
CALLCDNumberPrivate::CALLCDNumberPrivate(CALLCDNumber* q, QObject* parent): QObject(parent), q_ptr(q) {
}

CALLCDNumberPrivate::~CALLCDNumberPrivate() = default;

void CALLCDNumberPrivate::slotThemeModeChanged(const ALThemeType::ThemeMode& mode) {
	Q_Q(CALLCDNumber);

	themeMode = mode;
	q->foregroundRole();
	QPalette palette = q->palette();
	palette.setColor(QPalette::WindowText, ALThemeColor(themeMode, ALThemeType::BasicText));
	q->setPalette(palette);
}

/**
 * @brief \class CALLCDNumber
 * @param parent pointer to the parent class
 */
CALLCDNumber::CALLCDNumber(QWidget* parent): QLCDNumber(parent), d_ptr(new CALLCDNumberPrivate(this, this)) {
	Q_D(CALLCDNumber);
	d->isUseAutoClock = false;
	d->autoClockFormat = "yyyy-MM-dd hh:mm:ss";
	setDigitCount(d->autoClockFormat.length());
	setSegmentStyle(QLCDNumber::Flat);
	setObjectName("CALLCDNumber");
	setStyleSheet("#CALLCDNumber { background-color: transparent; }");
	d->lcdNumberStyle = new CALLCDNumberStyle(style());
	setStyle(d->lcdNumberStyle);
	d->clockTimer = new QTimer(this);
	connect(d->clockTimer, &QTimer::timeout, this, [=]() { display(QDateTime::currentDateTime().toString(d->autoClockFormat)); });
	d->slotThemeModeChanged(ALTheme->getThemeMode());
	connect(ALTheme, &CALThemeManager::sigThemeModeChanged, d, &CALLCDNumberPrivate::slotThemeModeChanged);
}

CALLCDNumber::CALLCDNumber(const int numDigits, QWidget* parent): CALLCDNumber(parent) {
	setDigitCount(numDigits);
}

CALLCDNumber::~CALLCDNumber() = default;

void CALLCDNumber::setIsUseAutoClock(const bool isUseAutoClock) {
	Q_D(CALLCDNumber);

	d->isUseAutoClock = isUseAutoClock;
	if (isUseAutoClock) {
		setDigitCount(d->autoClockFormat.length());
		display(QDateTime::currentDateTime().toString(d->autoClockFormat));
		d->clockTimer->start(200);
	} else {
		d->clockTimer->stop();
		display("");
	}
	Q_EMIT sigIsUseAutoClockChanged();
}

bool CALLCDNumber::getIsUseAutoClock() const {
	return d_func()->isUseAutoClock;
}

void CALLCDNumber::setIsTransparent(const bool isTransparent) {
	d_func()->lcdNumberStyle->setIsTransparent(isTransparent);
	update();
	Q_EMIT sigIsTransparentChanged();
}

bool CALLCDNumber::getIsTransparent() const {
	return d_func()->lcdNumberStyle->getIsTransparent();
}

void CALLCDNumber::setAutoClockFormat(const QString& autoClockFormat) {
	d_func()->autoClockFormat = autoClockFormat;
	setDigitCount(autoClockFormat.length());
	Q_EMIT sigAutoClockFormatChanged();
}

QString CALLCDNumber::getAutoClockFormat() const {
	return d_func()->autoClockFormat;
}
