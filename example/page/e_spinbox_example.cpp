#include "e_spinbox_example.hpp"

#include <QDebug>
#include <QHBoxLayout>

#include "alspinbox.hpp"
#include "aldoublespinbox.hpp"
#include "alscrollpagearea.hpp"
#include "altext.hpp"
#include "altoggleswitch.hpp"

using namespace AL;

E_SpinBox_Example::E_SpinBox_Example(QWidget* parent): E_BasePage(parent) {
	setWindowTitle("CALSpinBox");
	createCustomWidget("some thing...");

	const auto centralWidget = new QWidget(this);
	centralWidget->setWindowTitle(this->windowTitle());
	mainVLayout = new QVBoxLayout(centralWidget);
	mainVLayout->setContentsMargins(0, 0, 0, 0);
	mainVLayout->setSpacing(5);

	const auto spinBox = new CALSpinBox(this);
	const auto spinBoxArea = new CALScrollPageArea(this);
	const auto spinBoxHLayout = new QHBoxLayout(spinBoxArea);
	const auto spinBoxText = new CALText("CALSpinBox", this);
	spinBoxText->setTextPixelSize(15);
	spinBoxHLayout->addWidget(spinBoxText);
	spinBoxHLayout->addWidget(spinBox);
	spinBoxHLayout->addStretch();
	const auto spinBoxDisable = new CALToggleSwitch(this);
	const auto spinBoxDisableText = new CALText(tr("Disable"), this);
	spinBoxDisableText->setTextPixelSize(15);
	connect(spinBoxDisable, &CALToggleSwitch::sigToggleChanged, spinBox, &CALSpinBox::setDisabled);
	spinBoxHLayout->addWidget(spinBoxDisableText);
	spinBoxHLayout->addWidget(spinBoxDisable);
	spinBoxHLayout->addSpacing(10);

	const auto doubleSpinBox = new CALDoubleSpinBox(this);
	const auto doubleSpinBoxArea = new CALScrollPageArea(this);
	const auto doubleSpinBoxHLayout = new QHBoxLayout(doubleSpinBoxArea);
	const auto doubleSpinBoxText = new CALText("CALDoubleSpinBox", this);
	doubleSpinBoxText->setTextPixelSize(15);
	doubleSpinBoxHLayout->addWidget(doubleSpinBoxText);
	doubleSpinBoxHLayout->addWidget(doubleSpinBox);
	doubleSpinBoxHLayout->addStretch();
	const auto doubleSpinBoxDisable = new CALToggleSwitch(this);
	const auto doubleSpinBoxDisableText = new CALText(tr("Disable"), this);
	doubleSpinBoxDisableText->setTextPixelSize(15);
	connect(doubleSpinBoxDisable, &CALToggleSwitch::sigToggleChanged, doubleSpinBox, &CALDoubleSpinBox::setDisabled);
	doubleSpinBoxHLayout->addWidget(doubleSpinBoxDisableText);
	doubleSpinBoxHLayout->addWidget(doubleSpinBoxDisable);
	doubleSpinBoxHLayout->addSpacing(10);

	mainVLayout->addWidget(spinBoxArea);
	mainVLayout->addWidget(doubleSpinBoxArea);

	mainVLayout->addStretch();
	addCentralWidget(centralWidget, true, true, 0);
}

E_SpinBox_Example::~E_SpinBox_Example() = default;
