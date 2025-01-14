#include "e_radiobutton_example.hpp"

#include <QDebug>
#include <QHBoxLayout>

#include "alradiobutton.hpp"
#include "alscrollpagearea.hpp"
#include "altext.hpp"
#include "altoggleswitch.hpp"

using namespace AL;

E_RadioButton_Example::E_RadioButton_Example(QWidget* parent): E_BasePage(parent) {
	setWindowTitle("CALRadioButton");
	createCustomWidget("some thing...");

	const auto centralWidget = new QWidget(this);
	centralWidget->setWindowTitle(this->windowTitle());
	mainVLayout = new QVBoxLayout(centralWidget);
	mainVLayout->setContentsMargins(0, 0, 0, 0);
	mainVLayout->setSpacing(5);

	const auto radioButton = new CALRadioButton("radioButton", this);
	const auto radioButtonArea = new CALScrollPageArea(this);
	const auto radioButtonHLayout = new QHBoxLayout(radioButtonArea);
	const auto radioButtonText = new CALText("CALRadioButton", this);
	radioButtonText->setTextPixelSize(15);
	radioButtonHLayout->addWidget(radioButtonText);
	radioButtonHLayout->addWidget(radioButton);
	radioButtonHLayout->addStretch();
	const auto radioButtonDisable = new CALToggleSwitch(this);
	const auto radioButtonDisableText = new CALText(tr("Disable"), this);
	radioButtonDisableText->setTextPixelSize(15);
	connect(radioButtonDisable, &CALToggleSwitch::sigToggleChanged, radioButton, &CALRadioButton::setDisabled);
	radioButtonHLayout->addWidget(radioButtonDisableText);
	radioButtonHLayout->addWidget(radioButtonDisable);
	radioButtonHLayout->addSpacing(10);

	mainVLayout->addWidget(radioButtonArea);

	mainVLayout->addStretch();
	addCentralWidget(centralWidget, true, true, 0);
}

E_RadioButton_Example::~E_RadioButton_Example() = default;
