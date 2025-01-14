#include "e_multiselectcombobox_example.hpp"

#include <QDebug>
#include <QHBoxLayout>

#include "almultiselectcombobox.hpp"
#include "alscrollpagearea.hpp"
#include "altext.hpp"
#include "altoggleswitch.hpp"

using namespace AL;

E_MultiSelectCombobox_Example::E_MultiSelectCombobox_Example(QWidget* parent): E_BasePage(parent) {
	setWindowTitle("CALMultiSelectCombobox");
	createCustomWidget("some thing...");

	const auto centralWidget = new QWidget(this);
	centralWidget->setWindowTitle(this->windowTitle());
	mainVLayout = new QVBoxLayout(centralWidget);
	mainVLayout->setContentsMargins(0, 0, 0, 0);
	mainVLayout->setSpacing(5);

	const auto multiSelectComboBox = new CALMultiSelectComboBox(this);
	multiSelectComboBox->setFixedWidth(300);
	const auto multiSelectComboBoxArea = new CALScrollPageArea(this);
	const auto multiSelectComboBoxHLayout = new QHBoxLayout(multiSelectComboBoxArea);
	const auto multiSelectComboBoxText = new CALText("CALMultiSelectComboBox", this);
	multiSelectComboBoxText->setTextPixelSize(15);
	multiSelectComboBoxHLayout->addWidget(multiSelectComboBoxText);
	multiSelectComboBoxHLayout->addWidget(multiSelectComboBox);
	multiSelectComboBoxHLayout->addStretch();
	const auto multiSelectComboBoxDisable = new CALToggleSwitch(this);
	const auto multiSelectComboBoxDisableText = new CALText(tr("Disable"), this);
	multiSelectComboBoxDisableText->setTextPixelSize(15);
	connect(multiSelectComboBoxDisable, &CALToggleSwitch::sigToggleChanged, multiSelectComboBox, &CALMultiSelectComboBox::setDisabled);
	multiSelectComboBoxHLayout->addWidget(multiSelectComboBoxDisableText);
	multiSelectComboBoxHLayout->addWidget(multiSelectComboBoxDisable);
	multiSelectComboBoxHLayout->addSpacing(10);
	multiSelectComboBox->addItem("select all");
	for (int i = 0; i < 15; i++) {
		multiSelectComboBox->addItem(QString("test %1").arg(i));
	}
	multiSelectComboBox->setCurrentSelection(QList<int>{ 1, 2, 3, 4, 5 });

	mainVLayout->addWidget(multiSelectComboBoxArea);
	mainVLayout->addStretch();
	addCentralWidget(centralWidget, true, true, 0);
}

E_MultiSelectCombobox_Example::~E_MultiSelectCombobox_Example() = default;
