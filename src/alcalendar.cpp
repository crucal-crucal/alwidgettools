#include "alcalendar.hpp"

#include <QHBoxLayout>
#include <QPainter>

#include "alawesometoolbutton.hpp"
#include "albaselistview.hpp"
#include "alcalendardelegate.hpp"
#include "alcalendarmodel.hpp"
#include "alcalendartitledelegate.hpp"
#include "alcalendartitlemodel.hpp"
#include "alcalendar_p.hpp"
#include "alscrollbar.hpp"
#include "althememanager.hpp"

CALCalendar::CALCalendar(QWidget* parent): QWidget(parent), d_ptr(new CALCalendarPrivate(this, this)) {
	Q_D(CALCalendar);

	setFixedSize(305, 340);
	setObjectName("CALCalendar");
	d->borderRadius = 5;
	d->borderWidth = 1.5;
	d->lastSelectedMonth = 1;
	d->lastSelectedYear = 0;
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
	/// date
	const QDate currentDate = QDate::currentDate();
	d->lastSelectedYear = currentDate.year();
	d->lastSelectedMonth = currentDate.month();
	d->scrollToDate(currentDate);
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
	painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);
	QRect baseRect = rect();
	baseRect.adjust(d->borderWidth, d->borderWidth, -d->borderWidth, d->borderWidth);
	painter.setPen(Qt::NoPen);
	painter.setBrush(ALThemeColor(d->themeMode, ALThemeType::DialogBase));
	painter.drawRoundedRect(baseRect, d->borderRadius, d->borderRadius);
	// 缩放动画
	if (!d->isSwitchAnimationFinished) {
		painter.save();
		QRect pixRect = QRect(baseRect.x(), d->borderWidth + 45, baseRect.width(), baseRect.height() - 45);
		painter.setOpacity(d.pix)
	}
}
