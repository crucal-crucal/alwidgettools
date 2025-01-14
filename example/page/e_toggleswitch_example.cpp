#include "e_toggleswitch_example.hpp"

#include <QDebug>
#include <QHBoxLayout>

#include "alscrollpagearea.hpp"
#include "altext.hpp"
#include "altoggleswitch.hpp"

using namespace AL;

E_ToggleSwitch_Example::E_ToggleSwitch_Example(QWidget* parent): E_BasePage(parent) {
	setWindowTitle("CALToggleSwitch");
	createCustomWidget("some thing...");

	const auto centralWidget = new QWidget(this);
	centralWidget->setWindowTitle(this->windowTitle());
	mainVLayout = new QVBoxLayout(centralWidget);
	mainVLayout->setContentsMargins(0, 0, 0, 0);
	mainVLayout->setSpacing(5);

	const auto toggleSwitch = new CALToggleSwitch(this);
	const auto toggleSwitchArea = new CALScrollPageArea(this);
	const auto toggleSwitchHLayout = new QHBoxLayout(toggleSwitchArea);
	const auto toggleSwitchText = new CALText("CALToggleSwitch", this);
	toggleSwitchText->setTextPixelSize(15);
	toggleSwitchHLayout->addWidget(toggleSwitchText);
	toggleSwitchHLayout->addWidget(toggleSwitch);
	toggleSwitchHLayout->addStretch();
	const auto toggleSwitchDisable = new CALToggleSwitch(this);
	const auto toggleSwitchDisableText = new CALText(tr("Disable"), this);
	toggleSwitchDisableText->setTextPixelSize(15);
	connect(toggleSwitchDisable, &CALToggleSwitch::sigToggleChanged, toggleSwitch, &CALToggleSwitch::setDisabled);
	toggleSwitchHLayout->addWidget(toggleSwitchDisableText);
	toggleSwitchHLayout->addWidget(toggleSwitchDisable);
	toggleSwitchHLayout->addSpacing(10);

	mainVLayout->addWidget(toggleSwitchArea);

	mainVLayout->addStretch();
	addCentralWidget(centralWidget, true, true, 0);
}

E_ToggleSwitch_Example::~E_ToggleSwitch_Example() = default;
