#include "e_popup.hpp"

#include <QVBoxLayout>

#include "alcalendar.hpp"

E_Popup::E_Popup(QWidget* parent): E_BasePage(parent) {
	setWindowTitle("CALPopup");
	createCustomWidget("some thing...");

	const auto centralWidget = new QWidget(this);
	centralWidget->setWindowTitle("CALPopup");
	mainVLayout = new QVBoxLayout(centralWidget);
	mainVLayout->setContentsMargins(0, 0, 0, 0);
	mainVLayout->setSpacing(5);

	/// calendar
	initCalendarArea();

	mainVLayout->addStretch();
	addCentralWidget(centralWidget, true, true, 0);
}

E_Popup::~E_Popup() = default;

void E_Popup::initCalendarArea() {
	const auto calendar = new CALCalendar(this);
	mainVLayout->addWidget(calendar);
}
