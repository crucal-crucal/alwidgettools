#include "e_messagebar_example.hpp"

#include <QDebug>
#include <QHBoxLayout>

#include "almessagebar.hpp"
#include "alpushbutton.hpp"
#include "alscrollpagearea.hpp"
#include "altext.hpp"
#include "altoggleswitch.hpp"

using namespace AL;

E_MessageBar_Example::E_MessageBar_Example(QWidget* parent): E_BasePage(parent) {
	setWindowTitle("CALMessageBar");
	createCustomWidget("some thing...");

	const auto centralWidget = new QWidget(this);
	centralWidget->setWindowTitle(this->windowTitle());
	mainVLayout = new QVBoxLayout(centralWidget);
	mainVLayout->setContentsMargins(0, 0, 0, 0);
	mainVLayout->setSpacing(5);

	const auto successButton = new CALPushButton("Success", this);
	successButton->setFixedSize(80, 38);
	successButton->setButtonStyles(CALPushButton::Shadow | CALPushButton::Border);
	connect(successButton, &CALPushButton::clicked, this, [=]() { CALMessageBar::success("Success", "This is a success message"); });
	const auto warningButton = new CALPushButton("Warning", this);
	warningButton->setFixedSize(80, 38);
	warningButton->setButtonStyles(CALPushButton::Shadow | CALPushButton::Border);
	connect(warningButton, &CALPushButton::clicked, this, [=]() { CALMessageBar::warning("Warning", "This is a warning message"); });
	const auto errorButton = new CALPushButton("Error", this);
	errorButton->setFixedSize(80, 38);
	errorButton->setButtonStyles(CALPushButton::Shadow | CALPushButton::Border);
	connect(errorButton, &CALPushButton::clicked, this, [=]() { CALMessageBar::error("Error", "This is an error message"); });
	const auto infoButton = new CALPushButton("Info", this);
	infoButton->setFixedSize(80, 38);
	infoButton->setButtonStyles(CALPushButton::Shadow | CALPushButton::Border);
	connect(infoButton, &CALPushButton::clicked, this, [=]() { CALMessageBar::information("Info", "This is an info message"); });
	const auto messageBarArea = new CALScrollPageArea(this);
	const auto messageBarAreaHLayout = new QHBoxLayout(messageBarArea);
	const auto messageBarText = new CALText("CALMessageBar", this);
	messageBarText->setTextPixelSize(15);
	messageBarAreaHLayout->addWidget(messageBarText);
	messageBarAreaHLayout->addWidget(successButton);
	messageBarAreaHLayout->addWidget(warningButton);
	messageBarAreaHLayout->addWidget(errorButton);
	messageBarAreaHLayout->addWidget(infoButton);
	messageBarAreaHLayout->addStretch();
	const auto messageBarDisable = new CALToggleSwitch(this);
	const auto messageBarDisableText = new CALText(tr("Disable"), this);
	messageBarDisableText->setTextPixelSize(15);
	connect(messageBarDisable, &CALToggleSwitch::sigToggleChanged, this, [=](const bool toggled) {
		successButton->setDisabled(toggled);
		warningButton->setDisabled(toggled);
		errorButton->setDisabled(toggled);
		infoButton->setDisabled(toggled);
	});
	messageBarAreaHLayout->addWidget(messageBarDisableText);
	messageBarAreaHLayout->addWidget(messageBarDisable);
	messageBarAreaHLayout->addSpacing(10);

	mainVLayout->addWidget(messageBarArea);

	mainVLayout->addStretch();
	addCentralWidget(centralWidget, true, true, 0);
}

E_MessageBar_Example::~E_MessageBar_Example() = default;
