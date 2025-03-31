#include "e_popup.hpp"

#include <QVBoxLayout>

#include "alcolordialog.hpp"
#include "alcalendar.hpp"
#include "alcalendarpicker.hpp"
#include "alcheckbox.hpp"
#include "aldrawerarea.hpp"
#include "alicon.hpp"
#include "alpushbutton.hpp"
#include "alroller.hpp"
#include "alscrollpagearea.hpp"
#include "altext.hpp"

using namespace AL;

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
	/// drawerArea
	initDrawerAreaArea();
	/// rollerArea
	initRollerArea();

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
	connect(colorDialog, &CALColorDialog::sigColorSelected, this, [colorDialogButton, colorDialog, colorText](const QColor& color) {
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

void E_Popup::initDrawerAreaArea() {
	const auto drawer = new CALDrawerArea(this);
	const auto drawerHeader = new QWidget(this);
	const auto drawerHeaderHLayout = new QHBoxLayout(drawerHeader);
	const auto drawerIcon = new CALText(this);
	drawerIcon->setAttribute(Qt::WA_TransparentForMouseEvents);
	drawerIcon->setTextPixelSize(15);
	drawerIcon->setALIcon(CALIconFactory::createIconType(ALIcon::FluentIcon::Message));
	drawerIcon->setFixedSize(25, 25);
	const auto drawerText = new CALText("CALDrawerArea", this);
	drawerText->setAttribute(Qt::WA_TransparentForMouseEvents);
	drawerText->setTextPixelSize(15);
	drawerHeaderHLayout->addWidget(drawerIcon);
	drawerHeaderHLayout->addWidget(drawerText);
	drawerHeaderHLayout->addStretch();
	drawer->setDrawerHeader(drawerHeader);

	for (int i = 0; i < 3; ++i) {
		const auto drawerWidget = new QWidget(this);
		drawerWidget->setFixedHeight(75);
		const auto drawerWidgetHLayout = new QHBoxLayout(drawerWidget);
		const auto drawerCheckBox = new CALCheckBox(QString("test interface %1").arg(i), this);
		drawerWidgetHLayout->addSpacing(60);
		drawerWidgetHLayout->addWidget(drawerCheckBox);
		drawer->addDrawer(drawerWidget);
	}

	mainVLayout->addWidget(drawer);
}

void E_Popup::initRollerArea() {
	const auto roller = new CALRoller(this);
	QStringList rollerItemList{};
	for (int i = 0; i < 100; ++i) {
		rollerItemList.append(QString::number(i));
	}
	roller->setItemList(rollerItemList);
	const auto rollerArea = new CALScrollPageArea(this);
	rollerArea->setFixedHeight(220);
	const auto rollerHLayout = new QHBoxLayout(rollerArea);
	const auto rollerText = new CALText("CALRoller", this);
	rollerText->setTextPixelSize(15);
	rollerHLayout->addWidget(rollerText);
	rollerHLayout->addWidget(roller);
	rollerHLayout->addStretch();

	mainVLayout->addWidget(rollerArea);
}
