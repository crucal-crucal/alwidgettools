#include "e_basecomponents.hpp"

#include <QHBoxLayout>
#include <QMouseEvent>

#include "uvmessagebar.hpp"
#include "uvpushbutton.hpp"
#include "uvtoggleswitch.hpp"
#include "uvscrollpagearea.hpp"
#include "uvtext.hpp"

E_BaseComponents::E_BaseComponents(QWidget* parent): E_BasePage(parent) {
	setWindowTitle("CUVBaseComponents");
	createCustomWidget("some thing...");

	/// toggleSwitchArea
	const auto toggleSwitch = new CUVToggleSwitch(this);
	const auto toggleSwitchArea = new CUVScrollPageArea(this);
	const auto toggleSwitchHLayout = new QHBoxLayout(toggleSwitchArea);
	const auto toggleSwitchText = new CUVText("CUVToggleSwitch", this);
	toggleSwitchText->setTextPixelSize(15);
	toggleSwitchHLayout->addWidget(toggleSwitchText);
	toggleSwitchHLayout->addWidget(toggleSwitch);
	toggleSwitchHLayout->addStretch();
	const auto toggleSwitchDisable = new CUVToggleSwitch(this);
	const auto toggleSwitchDisableText = new CUVText("Disable", this);
	toggleSwitchDisableText->setTextPixelSize(15);
	connect(toggleSwitchDisable, &CUVToggleSwitch::sigToggleChanged, this, [=](const bool toggled) {
		toggleSwitch->setDisabled(toggled);
	});
	toggleSwitchHLayout->addWidget(toggleSwitchDisableText);
	toggleSwitchHLayout->addWidget(toggleSwitchDisable);
	toggleSwitchHLayout->addSpacing(10);

	/// messageBarArea
	const auto successButton = new CUVPushButton("Success", this);
	successButton->setFixedSize(80, 38);
	successButton->setButtonStyles(CUVPushButton::Shadow | CUVPushButton::Border);
	connect(successButton, &CUVPushButton::clicked, this, [=]() { CUVMessageBar::success("Success", "This is a success message"); });
	const auto warningButton = new CUVPushButton("Warning", this);
	warningButton->setFixedSize(80, 38);
	warningButton->setButtonStyles(CUVPushButton::Shadow | CUVPushButton::Border);
	connect(warningButton, &CUVPushButton::clicked, this, [=]() { CUVMessageBar::warning("Warning", "This is a warning message"); });
	const auto errorButton = new CUVPushButton("Error", this);
	errorButton->setFixedSize(80, 38);
	errorButton->setButtonStyles(CUVPushButton::Shadow | CUVPushButton::Border);
	connect(errorButton, &CUVPushButton::clicked, this, [=]() { CUVMessageBar::error("Error", "This is an error message"); });
	const auto infoButton = new CUVPushButton("Info", this);
	infoButton->setFixedSize(80, 38);
	infoButton->setButtonStyles(CUVPushButton::Shadow | CUVPushButton::Border);
	connect(infoButton, &CUVPushButton::clicked, this, [=]() { CUVMessageBar::information("Info", "This is an info message"); });
	const auto messageBarArea = new CUVScrollPageArea(this);
	const auto messageBarAreaHLayout = new QHBoxLayout(messageBarArea);
	const auto messageBarText = new CUVText("CUVMessageBar", this);
	messageBarText->setTextPixelSize(15);
	messageBarAreaHLayout->addWidget(messageBarText);
	messageBarAreaHLayout->addWidget(successButton);
	messageBarAreaHLayout->addWidget(warningButton);
	messageBarAreaHLayout->addWidget(errorButton);
	messageBarAreaHLayout->addWidget(infoButton);
	messageBarAreaHLayout->addStretch();
	const auto messageBarDisable = new CUVToggleSwitch(this);
	const auto messageBarDisableText = new CUVText("Disable", this);
	messageBarDisableText->setTextPixelSize(15);
	connect(messageBarDisable, &CUVToggleSwitch::sigToggleChanged, this, [=](const bool toggled) {
		successButton->setDisabled(toggled);
		warningButton->setDisabled(toggled);
		errorButton->setDisabled(toggled);
		infoButton->setDisabled(toggled);
	});
	messageBarAreaHLayout->addWidget(messageBarDisableText);
	messageBarAreaHLayout->addWidget(messageBarDisable);
	messageBarAreaHLayout->addSpacing(10);


	const auto centralWidget = new QWidget(this);
	centralWidget->setWindowTitle("CUVBaseComponents");
	const auto centralVLayout = new QVBoxLayout(centralWidget);
	centralVLayout->setContentsMargins(0, 0, 0, 0);
	centralVLayout->setSpacing(5);
	centralVLayout->addWidget(toggleSwitchArea);
	centralVLayout->addWidget(messageBarArea);
	centralVLayout->addStretch();
	addCentralWidget(centralWidget, true, true, 0);

	const auto homeStack1 = new CUVText("HomeStack1", this);
	QFont font = homeStack1->font();
	font.setPixelSize(32);
	homeStack1->setFont(font);
	homeStack1->setAlignment(Qt::AlignCenter);
	homeStack1->setWindowTitle("HomeStack1");
	addCentralWidget(homeStack1);
	const auto homeStack2 = new CUVText("HomeStack2", this);
	homeStack2->setFont(font);
	homeStack2->setAlignment(Qt::AlignCenter);
	homeStack2->setWindowTitle("HomeStack2");
	addCentralWidget(homeStack2);
}

E_BaseComponents::~E_BaseComponents() = default;

void E_BaseComponents::mouseReleaseEvent(QMouseEvent* event) {
	switch (event->button()) {
		case Qt::LeftButton: {
			break;
		}
		case Qt::BackButton: {
			this->navigation(0);
			break;
		}
		case Qt::ForwardButton: {
			this->navigation(1);
			break;
		}
		case Qt::MiddleButton: {
			this->navigation(2);
			break;
		}
		default: {
			break;
		}
	}

	E_BasePage::mouseReleaseEvent(event);
}
