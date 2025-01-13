#include "alcalendar.hpp"

#include <QApplication>
#include <QHBoxLayout>
#include <QPainter>
#include <QPropertyAnimation>

#include "alawesometoolbutton.hpp"
#include "albaselistview.hpp"
#include "alcalendardelegate.hpp"
#include "alcalendarmodel.hpp"
#include "alcalendartitledelegate.hpp"
#include "alcalendartitlemodel.hpp"
#include "alcalendar_p.hpp"
#include "alscrollbar.hpp"
#include "althememanager.hpp"

namespace AL {
/**
 * \class CALCalendarPrivate
 * Internal class for CALCalendar
 * @param q pointer to the public class
 * @param parent pointer to the parent class
 */
CALCalendarPrivate::CALCalendarPrivate(CALCalendar* q, QObject* parent): QObject(parent), q_ptr(q) {
}

CALCalendarPrivate::~CALCalendarPrivate() = default;

void CALCalendarPrivate::setZoomRatio(const qreal zoomRatio) {
	this->zoomRatio = zoomRatio;
	Q_EMIT sigZoomRatioChanged();
}

qreal CALCalendarPrivate::getZoomRatio() const {
	return zoomRatio;
}

void CALCalendarPrivate::setPixOpacity(const qreal pixOpacity) {
	this->pixOpacity = pixOpacity;
	Q_EMIT sigPixOpacityChanged();
}

qreal CALCalendarPrivate::getPixOpacity() const {
	return pixOpacity;
}

void CALCalendarPrivate::slotSwitchButtonClicked() {
	Q_Q(CALCalendar);

	if (!isSwitchAnimationFinished) {
		return;
	}
	if (const ALCalendarMode displayMode = calendarModel->getDisplayMode(); displayMode == ALCalendarMode::DayMode) {
		oldCalendarViewPix = q->grab(QRect(borderWidth, borderWidth + 45, q->width() - 2 * borderWidth, q->height() - 2 * borderWidth - 45)); // NOLINT
		calendarModel->setDisplayMode(ALCalendarMode::MonthMode);
		calendarTitleView->setVisible(false);
		QApplication::processEvents();
		calendarView->setSpacing(15);
		scrollToDate(selectedDate.isValid() ? selectedDate : QDate::currentDate());
		newCalendarViewPix = q->grab(QRect(borderWidth, borderWidth + 45, q->width() - 2 * borderWidth, q->height() - 2 * borderWidth - 45)); // NOLINT
		doSwitchAnimation(false);
	} else if (displayMode == ALCalendarMode::MonthMode) {
		oldCalendarViewPix = q->grab(QRect(borderWidth, borderWidth + 45, q->width() - 2 * borderWidth, q->height() - 2 * borderWidth - 45)); // NOLINT
		calendarModel->setDisplayMode(ALCalendarMode::YearMode);
		scrollToDate(selectedDate.isValid() ? selectedDate : QDate::currentDate());
		newCalendarViewPix = q->grab(QRect(borderWidth, borderWidth + 45, q->width() - 2 * borderWidth, q->height() - 2 * borderWidth - 45)); // NOLINT
		doSwitchAnimation(false);
	}
}

void CALCalendarPrivate::slotCalendarViewClicked(const QModelIndex& index) {
	Q_Q(CALCalendar);

	if (!isSwitchAnimationFinished) {
		return;
	}

	switch (calendarModel->getDisplayMode()) {
		case ALCalendarMode::DayMode: {
			if (const QDate date = calendarModel->getDateFromIndex(index); date.isValid()) {
				calendarView->clearSelection();
				q->setSelectedDate(date);
			}
			Q_EMIT q->sigClicked(selectedDate);
			break;
		}
		case ALCalendarMode::MonthMode: {
			oldCalendarViewPix = q->grab(QRect(borderWidth, borderWidth + 45, q->width() - 2 * borderWidth, q->height() - 2 * borderWidth - 45)); // NOLINT
			lastSelectedMonth = index.row() % 12 + 1;
			calendarModel->setDisplayMode(ALCalendarMode::DayMode);
			calendarTitleView->setVisible(true);
			calendarView->setSpacing(0);
			scrollToDate(QDate(lastSelectedYear, lastSelectedMonth, 15));
			calendarView->selectionModel()->setCurrentIndex(calendarModel->getIndexFromDate(selectedDate), QItemSelectionModel::Select);
			newCalendarViewPix = q->grab(QRect(borderWidth, borderWidth + 45, q->width() - 2 * borderWidth, q->height() - 2 * borderWidth - 45)); // NOLINT
			calendarTitleView->setVisible(false);
			doSwitchAnimation(true);
			break;
		}
		case ALCalendarMode::YearMode: {
			oldCalendarViewPix = q->grab(QRect(borderWidth, borderWidth + 45, q->width() - 2 * borderWidth, q->height() - 2 * borderWidth - 45)); // NOLINT
			lastSelectedYear = calendarModel->getMinimumDate().year() + index.row();
			calendarModel->setDisplayMode(ALCalendarMode::MonthMode);
			scrollToDate(QDate(lastSelectedYear, lastSelectedMonth, 15));
			newCalendarViewPix = q->grab(QRect(borderWidth, borderWidth + 45, q->width() - 2 * borderWidth, q->height() - 2 * borderWidth - 45)); // NOLINT
			doSwitchAnimation(true);
			break;
		}
		default: {
			break;
		}
	}
}

void CALCalendarPrivate::slotUpButtonClicked() const {
	QScrollBar* vScrollBar = calendarView->verticalScrollBar();
	const auto scrollAnimation = new QPropertyAnimation(vScrollBar, "value");
	scrollAnimation->setEasingCurve(QEasingCurve::OutSine);
	scrollAnimation->setDuration(300);
	scrollAnimation->setStartValue(vScrollBar->value());
	scrollAnimation->setEndValue(vScrollBar->value() - 150);
	scrollAnimation->start(QAbstractAnimation::DeleteWhenStopped);
}

void CALCalendarPrivate::slotDownButtonClicked() const {
	QScrollBar* vScrollBar = calendarView->verticalScrollBar();
	const auto scrollAnimation = new QPropertyAnimation(vScrollBar, "value");
	scrollAnimation->setEasingCurve(QEasingCurve::OutSine);
	scrollAnimation->setDuration(300);
	scrollAnimation->setStartValue(vScrollBar->value());
	scrollAnimation->setEndValue(vScrollBar->value() + 150);
	scrollAnimation->start(QAbstractAnimation::DeleteWhenStopped);
}

void CALCalendarPrivate::slotUpdateSwitchButtonText() const {
	const QModelIndex index = calendarView->indexAt(QPoint(calendarView->rect().center().x() - 20, calendarView->rect().center().y()));
	if (!index.isValid()) {
		return;
	}

	const auto data = calendarModel->data(index, Qt::UserRole).value<CALCalendarData>();
	switch (calendarModel->getDisplayMode()) {
		case ALCalendarMode::DayMode: {
			modeSwitchButton->setText(QString::number(data.year) + tr("year") + QString::number(data.month) + tr("month"));
			break;
		}
		case ALCalendarMode::MonthMode: {
			modeSwitchButton->setText(QString::number(data.year) + tr("year"));
			break;
		}
		case ALCalendarMode::YearMode: {
			modeSwitchButton->setText(QString("%1-%2").arg(data.year - 5).arg(data.year + 10));
			break;
		}
		default: {
			break;
		}
	}
}

void CALCalendarPrivate::scrollToDate(const QDate& date) const {
	const int index = calendarModel->getIndexFromDate(date).row();
	switch (calendarModel->getDisplayMode()) {
		case ALCalendarMode::DayMode: {
			calendarView->scrollTo(calendarModel->index(index - 14), QAbstractItemView::PositionAtTop);
			break;
		}
		case ALCalendarMode::MonthMode:
		case ALCalendarMode::YearMode: {
			calendarView->scrollTo(calendarModel->index(index - 4), QAbstractItemView::PositionAtTop);
			break;
		}
		default: {
			break;
		}
	}
}

void CALCalendarPrivate::doSwitchAnimation(const bool isZoomIn) {
	Q_Q(CALCalendar);

	if (!isSwitchAnimationFinished) {
		return;
	}
	isDrawNewPix = false;
	isSwitchAnimationFinished = false;
	calendarDelegate->setIsTransparent(true);
	const auto oldPixZoomAnimation = new QPropertyAnimation(this, "zoomRatio");
	connect(oldPixZoomAnimation, &QPropertyAnimation::valueChanged, this, [=]() { q->update(); });
	connect(oldPixZoomAnimation, &QPropertyAnimation::finished, this, [=]() {
		isDrawNewPix = true;
		const auto newPixZoomAnimation = new QPropertyAnimation(this, "zoomRatio");
		newPixZoomAnimation->setEasingCurve(QEasingCurve::OutCubic);
		connect(newPixZoomAnimation, &QPropertyAnimation::valueChanged, this, [=]() { q->update(); });
		connect(newPixZoomAnimation, &QPropertyAnimation::finished, this, [=]() {
			if (calendarModel->getDisplayMode() == ALCalendarMode::DayMode) {
				calendarTitleView->setVisible(true);
			}
			isSwitchAnimationFinished = true;
			calendarDelegate->setIsTransparent(false);
		});
		// 放大 | 缩小 年 - 月 - 日
		newPixZoomAnimation->setStartValue(isZoomIn ? 0.85 : 1.5);
		newPixZoomAnimation->setEndValue(1);
		newPixZoomAnimation->setDuration(300);
		newPixZoomAnimation->start(QAbstractAnimation::DeleteWhenStopped);
	});
	// 放大 | 缩小 年 - 月 - 日
	oldPixZoomAnimation->setStartValue(1);
	oldPixZoomAnimation->setEndValue(isZoomIn ? 1.15 : 0.85);
	oldPixZoomAnimation->setDuration(150);
	oldPixZoomAnimation->start(QAbstractAnimation::DeleteWhenStopped);

	const auto oldPixOpacityAnimation = new QPropertyAnimation(this, "pixOpacity");
	connect(oldPixOpacityAnimation, &QPropertyAnimation::finished, this, [=]() {
		const auto newPixOpacityAnimation = new QPropertyAnimation(this, "pixOpacity");
		newPixOpacityAnimation->setStartValue(0);
		newPixOpacityAnimation->setEndValue(1);
		newPixOpacityAnimation->setDuration(300);
		newPixOpacityAnimation->start(QAbstractAnimation::DeleteWhenStopped);
	});
	oldPixOpacityAnimation->setStartValue(1);
	oldPixOpacityAnimation->setEndValue(0);
	oldPixOpacityAnimation->setDuration(150);
	oldPixOpacityAnimation->start(QAbstractAnimation::DeleteWhenStopped);
}

/**
 * \class CALCalendar
 * @param parent pointer to the parent class
 */
CALCalendar::CALCalendar(QWidget* parent): QWidget(parent), d_ptr(new CALCalendarPrivate(this, this)) {
	Q_D(CALCalendar);

	setFixedSize(305, 340);
	setObjectName("CALCalendar");
	d->borderRadius = 5;
	d->borderWidth = 1.5;
	d->lastSelectedMonth = 1;
	d->lastSelectedYear = 0;
	d->pixOpacity = 1;
	d->zoomRatio = 1;
	d->isSwitchAnimationFinished = true;
	d->isDrawNewPix = false;
	d->themeMode = ALTheme->getThemeMode();
	connect(ALTheme, &CALThemeManager::sigThemeModeChanged, this, [=](const ALThemeType::ThemeMode& mode) { d->themeMode = mode; });
	/// title
	d->calendarTitleModel = new CALCalendarTitleModel(this);
	d->calendarTitleDelegate = new CALCalendarTitleDelegate(this);
	d->calendarTitleView = new CALBaseListView(this);
	d->calendarTitleView->setFlow(QListView::LeftToRight);
	d->calendarTitleView->setViewMode(QListView::IconMode);
	d->calendarTitleView->setResizeMode(QListView::Adjust);
	d->calendarTitleView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	d->calendarTitleView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	d->calendarTitleView->setModel(d->calendarTitleModel);
	d->calendarTitleView->setItemDelegate(d->calendarTitleDelegate);
	d->calendarTitleView->setFixedHeight(30);
	/// calendar content
	d->calendarModel = new CALCalendarModel(this);
	d->calendarDelegate = new CALCalendarDelegate(d->calendarModel, this);
	d->calendarView = new CALBaseListView(this);
	d->calendarView->setFlow(QListView::LeftToRight);
	d->calendarView->setViewMode(QListView::IconMode);
	d->calendarView->setResizeMode(QListView::Adjust);
	d->calendarView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	d->calendarView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	d->calendarView->setSelectionMode(QAbstractItemView::NoSelection);
	d->calendarView->setModel(d->calendarModel);
	d->calendarView->setItemDelegate(d->calendarDelegate);
	const auto vScrollBar = dynamic_cast<CALScrollBar*>(d->calendarView->verticalScrollBar());
	vScrollBar->setSpeedLimit(6);
	connect(d->calendarView, &CALBaseListView::clicked, d, &CALCalendarPrivate::slotCalendarViewClicked);
	/// mode switch button
	d->modeSwitchButton = new CALAwesomeToolButton(this);
	d->modeSwitchButton->setText(tr("January 1997"));
	QFont switchButtonFont = d->modeSwitchButton->font();
	switchButtonFont.setWeight(QFont::Bold);
	d->modeSwitchButton->setFont(switchButtonFont);
	d->modeSwitchButton->setToolButtonStyle(Qt::ToolButtonTextOnly);
	d->modeSwitchButton->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
	connect(d->modeSwitchButton, &CALAwesomeToolButton::clicked, d, &CALCalendarPrivate::slotSwitchButtonClicked);
	connect(d->calendarView->verticalScrollBar(), &QScrollBar::valueChanged, d, &CALCalendarPrivate::slotUpdateSwitchButtonText);
	/// up or down button
	d->upButton = new CALAwesomeToolButton(this);
	d->upButton->setFixedSize(36, 36);
	d->upButton->setAweSomeIcon(ALIcon::AweSomeIcon::CaretUp);
	connect(d->upButton, &CALAwesomeToolButton::clicked, d, &CALCalendarPrivate::slotUpButtonClicked);
	d->downButton = new CALAwesomeToolButton(this);
	d->downButton->setFixedSize(36, 36);
	d->downButton->setAweSomeIcon(ALIcon::AweSomeIcon::CaretDown);
	connect(d->downButton, &CALAwesomeToolButton::clicked, d, &CALCalendarPrivate::slotDownButtonClicked);
	/// layout
	const auto buttonHLayout = new QHBoxLayout;
	buttonHLayout->setContentsMargins(5, 5, 10, 0);
	buttonHLayout->addWidget(d->modeSwitchButton);
	buttonHLayout->addWidget(d->upButton);
	buttonHLayout->addWidget(d->downButton);
	const auto mainVLayout = new QVBoxLayout(this);
	mainVLayout->setContentsMargins(5, 0, 0, 0);
	mainVLayout->addLayout(buttonHLayout);
	mainVLayout->addWidget(d->calendarTitleView);
	mainVLayout->addWidget(d->calendarView);
}

CALCalendar::~CALCalendar() = default;

void CALCalendar::setBorderRadius(const qreal borderRadius) {
	d_func()->borderRadius = borderRadius;
	Q_EMIT sigBorderRadiusChanged();
}

qreal CALCalendar::getBorderRadius() const {
	return d_func()->borderRadius;
}

void CALCalendar::setSelectedDate(const QDate& selectedDate) {
	Q_D(CALCalendar);

	if (!selectedDate.isValid() || selectedDate.daysTo(d->calendarModel->getMaximumDate()) < 0 || selectedDate.daysTo(d->calendarModel->getMinimumDate()) > 0) {
		return;
	}
	d->selectedDate = selectedDate;
	d->calendarView->selectionModel()->setCurrentIndex(d->calendarModel->getIndexFromDate(selectedDate), QItemSelectionModel::Select);
	Q_EMIT sigSelectedDateChanged();
}

QDate CALCalendar::getSelectedDate() const {
	return d_func()->selectedDate;
}

void CALCalendar::setMinimumDate(const QDate& minimumDate) {
	Q_D(CALCalendar);

	if (!minimumDate.isValid() || minimumDate.daysTo(d->calendarModel->getMaximumDate()) > 0) {
		return;
	}
	d->calendarModel->setMinimumDate(minimumDate);
	Q_EMIT sigMinimumDateChanged();
}

QDate CALCalendar::getMinimumDate() const {
	return d_func()->calendarModel->getMinimumDate();
}

void CALCalendar::setMaximumDate(const QDate& maximumDate) {
	Q_D(CALCalendar);

	if (!maximumDate.isValid() || maximumDate.daysTo(d->calendarModel->getMinimumDate()) < 0) {
		return;
	}
	d->calendarModel->setMaximumDate(maximumDate);
	Q_EMIT sigMaximumDateChanged();
}

QDate CALCalendar::getMaximumDate() const {
	return d_func()->calendarModel->getMaximumDate();
}

void CALCalendar::paintEvent(QPaintEvent* event) {
	Q_D(CALCalendar);

	QPainter painter(this);
	painter.save();
	painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing | QPainter::SmoothPixmapTransform);
	QRect baseRect = rect();
	baseRect.adjust(d->borderWidth, d->borderWidth, -d->borderWidth, -d->borderWidth); // NOLINT
	painter.setPen(Qt::NoPen);
	painter.setBrush(ALThemeColor(d->themeMode, ALThemeType::DialogBase));
	painter.drawRoundedRect(baseRect, d->borderRadius, d->borderRadius);
	// 缩放动画
	if (!d->isSwitchAnimationFinished) {
		painter.save();
		QRect pixRect = QRect(baseRect.x(), d->borderWidth + 45, baseRect.width(), baseRect.height() - 45); // NOLINT
		painter.setOpacity(d->pixOpacity);
		painter.translate(pixRect.center());
		painter.scale(d->zoomRatio, d->zoomRatio);
		painter.translate(-pixRect.center());
		painter.drawPixmap(pixRect, d->isDrawNewPix ? d->newCalendarViewPix : d->oldCalendarViewPix);
		painter.restore();
	}
	// 分割线
	painter.setPen(QPen(ALThemeColor(d->themeMode, ALThemeType::BasicBorder), d->borderWidth));
	painter.setBrush(Qt::NoBrush);
	painter.drawRoundedRect(baseRect, d->borderRadius, d->borderRadius);
	painter.drawLine(QPointF(baseRect.x(), 45), QPointF(baseRect.right(), 45));
	painter.restore();
}

void CALCalendar::showEvent(QShowEvent* event) {
	Q_D(CALCalendar);
	const QDate currentDate = QDate::currentDate();
	d->lastSelectedYear = currentDate.year();
	d->lastSelectedMonth = currentDate.month();
	d->scrollToDate(currentDate);
	QWidget::showEvent(event);
}

} // namespace AL
