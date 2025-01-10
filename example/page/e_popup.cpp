#include "e_popup.hpp"

#include <QVBoxLayout>

#include "alcalendar.hpp"
#include "alcalendarpicker.hpp"
#include "alcolordialog.hpp"
#include "alpushbutton.hpp"
#include "alscrollpagearea.hpp"
#include "altext.hpp"

E_Popup::E_Popup(QWidget* parent): E_BasePage(parent) {
	setWindowTitle("CALPopup");
	createCustomWidget("some thing...");

	const auto centralWidget = new QWidget(this);
	centralWidget->setWindowTitle(this->windowTitle());
	mainVLayout = new QVBoxLayout(centralWidget);
	mainVLayout->setContentsMargins(0, 0, 0, 0);
	mainVLayout->setSpacing(5);

	/// colorDialogArea
	initColorDialogArea();
	/// calendarPickerArea
	initCalendarPickerArea();
	/// calendar
	initCalendarArea();

	mainVLayout->addStretch();
	addCentralWidget(centralWidget, true, true, 0);
}

E_Popup::~E_Popup() = default;

void E_Popup::initColorDialogArea() {
	const auto colorDialog = new CALColorDialog(this);
	const auto colorText = new CALText(colorDialog->getCurrentColorRgb(), this);
	colorText->setTextPixelSize(15);
	const auto colorDialogButton = new CALPushButton(this);
	colorDialogButton->setFixedSize(35, 35);
	colorDialogButton->setLightDefaultColor(colorDialog->getCurrentColorRgb());
	colorDialogButton->setDarkDefaultColor(colorDialog->getCurrentColorRgb());
	colorDialogButton->setLightHoverColor(colorDialog->getCurrentColorRgb());
	colorDialogButton->setDarkHoverColor(colorDialog->getCurrentColorRgb());
	colorDialogButton->setLightPressedColor(colorDialog->getCurrentColorRgb());
	colorDialogButton->setDarkPressedColor(colorDialog->getCurrentColorRgb());
	connect(colorDialogButton, &CALPushButton::clicked, colorDialog, &CALColorDialog::exec);
	connect(colorDialog, &CALColorDialog::sigColorSelected, this, [=](const QColor& color) {
		colorDialogButton->setLightDefaultColor(color);
		colorDialogButton->setDarkDefaultColor(color);
		colorDialogButton->setLightHoverColor(color);
		colorDialogButton->setDarkHoverColor(color);
		colorDialogButton->setLightPressedColor(color);
		colorDialogButton->setDarkPressedColor(color);
		colorText->setText(colorDialog->getCurrentColorRgb());
	});
	const auto colorDialogArea = new CALScrollPageArea(this);
	const auto colorDialogHLayout = new QHBoxLayout(colorDialogArea);
	const auto colorDialogText = new CALText("CALColorDialog", this);
	colorDialogText->setTextPixelSize(15);
	colorDialogHLayout->addWidget(colorDialogText);
	colorDialogHLayout->addWidget(colorDialogButton);
	colorDialogHLayout->addWidget(colorText);
	colorDialogHLayout->addStretch();

	mainVLayout->addWidget(colorDialogArea);
}

void E_Popup::initCalendarPickerArea() {
	const auto calendarPicker = new CALCalendarPicker(this);
	const auto calendarPickerArea = new CALScrollPageArea(this);
	const auto calendarPickerHLayout = new QHBoxLayout(calendarPickerArea);
	const auto calendarPickerText = new CALText("CALCalendarPicker", this);
	calendarPickerText->setTextPixelSize(15);
	calendarPickerHLayout->addWidget(calendarPickerText);
	calendarPickerHLayout->addWidget(calendarPicker);
	calendarPickerHLayout->addStretch();

	mainVLayout->addWidget(calendarPickerArea);
}

void E_Popup::initCalendarArea() {
	const auto calendar = new CALCalendar(this);
	mainVLayout->addWidget(calendar);
}
