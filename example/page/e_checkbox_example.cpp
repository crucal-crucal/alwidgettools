#include "e_checkbox_example.hpp"

#include <QDebug>
#include <QHBoxLayout>

#include "alcheckbox.hpp"
#include "alscrollpagearea.hpp"
#include "alslider.hpp"
#include "altext.hpp"
#include "altoggleswitch.hpp"

using namespace AL;

E_CheckBox_Example::E_CheckBox_Example(QWidget* parent): E_BasePage(parent) {
	setWindowTitle("CALCheckBox");
	createCustomWidget("some thing...");

	const auto centralWidget = new QWidget(this);
	centralWidget->setWindowTitle(this->windowTitle());
	mainVLayout = new QVBoxLayout(centralWidget);
	mainVLayout->setContentsMargins(0, 0, 0, 0);
	mainVLayout->setSpacing(5);

	const auto checkBox = new CALCheckBox("checkBox", this);
	const auto checkBoxArea = new CALScrollPageArea(this);
	const auto checkBoxHLayout = new QHBoxLayout(checkBoxArea);
	const auto checkBoxText = new CALText("CALCheckBox", this);
	checkBoxText->setTextPixelSize(15);
	checkBoxHLayout->addWidget(checkBoxText);
	checkBoxHLayout->addWidget(checkBox);
	checkBoxHLayout->addStretch();
	const auto checkBoxDisable = new CALToggleSwitch(this);
	const auto checkBoxDisableText = new CALText(tr("Disable"), this);
	checkBoxDisableText->setTextPixelSize(15);
	connect(checkBoxDisable, &CALToggleSwitch::sigToggleChanged, checkBox, &CALSlider::setDisabled);
	checkBoxHLayout->addWidget(checkBoxDisableText);
	checkBoxHLayout->addWidget(checkBoxDisable);
	checkBoxHLayout->addSpacing(10);

	mainVLayout->addWidget(checkBoxArea);
	mainVLayout->addStretch();
	addCentralWidget(centralWidget, true, true, 0);
}

E_CheckBox_Example::~E_CheckBox_Example() = default;
