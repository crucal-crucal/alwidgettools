#include "e_basecomponents.hpp"

#include <QDebug>
#include <QHBoxLayout>
#include <QMetaEnum>
#include <QMouseEvent>

#include "alcolordialog.hpp"
#include "alcheckbox.hpp"
#include "alcircularprogress.hpp"
#include "alcombobox.hpp"
#include "aldoublespinbox.hpp"
#include "almessagebar.hpp"
#include "almultiselectcombobox.hpp"
#include "alprogressbar.hpp"
#include "alpushbutton.hpp"
#include "alradiobutton.hpp"
#include "alscrollpagearea.hpp"
#include "alslider.hpp"
#include "alspinbox.hpp"
#include "altext.hpp"
#include "altoggleswitch.hpp"

E_BaseComponents::E_BaseComponents(QWidget* parent): E_BasePage(parent) {
	setWindowTitle("CALBaseComponents");
	createCustomWidget("some thing...");

	const auto centralWidget = new QWidget(this);
	centralWidget->setWindowTitle(this->windowTitle());
	mainVLayout = new QVBoxLayout(centralWidget);
	mainVLayout->setContentsMargins(0, 0, 0, 0);
	mainVLayout->setSpacing(5);

	/// toggleSwitchArea
	initToggleSwitchArea();
	/// messageBarArea
	initMessageBarArea();
	/// circularProgressArea
	initCircularProgressArea();
	/// progressBarArea
	initProgressBarArea();
	/// multiSelectComboBoxArea
	initMultiSelectComboBoxArea();
	/// sliderArea
	initSliderArea();
	/// checkBoxArea
	initCheckBoxArea();
	/// radioButtonArea
	initRadioButtonArea();
	/// spinBoxArea
	initSpinBoxArea();
	/// doubleSpinBoxArea
	initDoubleSpinBoxArea();

	mainVLayout->addStretch();
	addCentralWidget(centralWidget, true, true, 0);

	const auto homeStack1 = new CALText("HomeStack1", this);
	QFont font = homeStack1->font();
	font.setPixelSize(32);
	homeStack1->setFont(font);
	homeStack1->setAlignment(Qt::AlignCenter);
	homeStack1->setWindowTitle("HomeStack1");
	addCentralWidget(homeStack1);
	const auto homeStack2 = new CALText("HomeStack2", this);
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

void E_BaseComponents::initToggleSwitchArea() {
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
}

void E_BaseComponents::initMessageBarArea() {
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
}

void E_BaseComponents::initCircularProgressArea() {
	const QMetaObject& metaObject = ALProgressType::staticMetaObject;
	const QMetaEnum metaEnum = metaObject.enumerator(metaObject.indexOfEnumerator("ProgressType"));
	if (!metaEnum.isValid()) {
		qWarning() << "Invalid ProgressType meta enum";
		return;
	}

	const auto colorDialog = new CALColorDialog(this);
	const auto circularProgress = new CALCircularProgress(this);
	circularProgress->setShowProgressText(false);
	circularProgress->setSize(30);
	circularProgress->setLineWidth(5.0);
	circularProgress->setColor(colorDialog->getCurrentColor());
	connect(colorDialog, &CALColorDialog::sigColorSelected, circularProgress, &CALCircularProgress::setColor);
	const auto circularProgressType = new CALComboBox(this);
	for (int i = 0; i < metaEnum.keyCount(); i++) {
		circularProgressType->addItem(metaEnum.key(i));
	}
	circularProgressType->setCurrentText(QString(metaEnum.valueToKey(circularProgress->getProgressType())));
	connect(circularProgressType, &CALComboBox::currentTextChanged, this, [=](const QString& type) {
		if (metaEnum.isValid()) {
			if (const int value = metaEnum.keyToValue(type.toLocal8Bit().constData()); value != -1) {
				circularProgress->setProgressType(static_cast<ALProgressType::ProgressType>(value));
			} else {
				qWarning() << "Invalid progress type: " << type;
			}
		}
	});
	const auto circularProgressArea = new CALScrollPageArea(this);
	const auto circularProgressHLayout = new QHBoxLayout(circularProgressArea);
	const auto circularProgressText = new CALText("CALCircularProgress", this);
	circularProgressText->setTextPixelSize(15);
	const auto circularProgerssColorButton = new CALPushButton("Color", this);
	circularProgerssColorButton->setFixedSize(80, 38);
	circularProgerssColorButton->setButtonStyles(CALPushButton::Shadow | CALPushButton::Border);
	connect(circularProgerssColorButton, &CALPushButton::clicked, colorDialog, &CALColorDialog::exec);
	circularProgressHLayout->addWidget(circularProgressText);
	circularProgressHLayout->addWidget(circularProgress);
	circularProgressHLayout->addWidget(circularProgerssColorButton);
	circularProgressHLayout->addWidget(circularProgressType);
	circularProgressHLayout->addStretch();
	const auto circularProgressDisable = new CALToggleSwitch(this);
	const auto circularProgressDisableText = new CALText(tr("Disable"), this);
	circularProgressDisableText->setTextPixelSize(15);
	connect(circularProgressDisable, &CALToggleSwitch::sigToggleChanged, circularProgress, &CALCircularProgress::setDisabled);
	circularProgressHLayout->addWidget(circularProgressDisableText);
	circularProgressHLayout->addWidget(circularProgressDisable);
	circularProgressHLayout->addSpacing(10);

	mainVLayout->addWidget(circularProgressArea);
}

void E_BaseComponents::initMultiSelectComboBoxArea() {
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
}

void E_BaseComponents::initSliderArea() {
	const auto slider = new CALSlider(this);
	const auto sliderArea = new CALScrollPageArea(this);
	const auto sliderHLayout = new QHBoxLayout(sliderArea);
	const auto sliderText = new CALText("CALSlider", this);
	sliderText->setTextPixelSize(15);
	sliderHLayout->addWidget(sliderText);
	sliderHLayout->addWidget(slider);
	sliderHLayout->addStretch();
	const auto sliderDisable = new CALToggleSwitch(this);
	const auto sliderDisableText = new CALText(tr("Disable"), this);
	sliderDisableText->setTextPixelSize(15);
	connect(sliderDisable, &CALToggleSwitch::sigToggleChanged, slider, &CALSlider::setDisabled);
	sliderHLayout->addWidget(sliderDisableText);
	sliderHLayout->addWidget(sliderDisable);
	sliderHLayout->addSpacing(10);

	mainVLayout->addWidget(sliderArea);
}

void E_BaseComponents::initProgressBarArea() {
	const QMetaObject& metaObject = ALProgressType::staticMetaObject;
	const QMetaEnum metaEnum = metaObject.enumerator(metaObject.indexOfEnumerator("ProgressType"));
	if (!metaEnum.isValid()) {
		qWarning() << "Invalid ProgressType meta enum";
		return;
	}

	const auto colorDialog = new CALColorDialog(this);
	const auto progressBar = new CALProgressBar(this);
	progressBar->setShowProgressText(false);
	progressBar->setProgressColor(colorDialog->getCurrentColor());
	connect(colorDialog, &CALColorDialog::sigColorSelected, progressBar, &CALProgressBar::setProgressColor);
	const auto progressBarType = new CALComboBox(this);
	progressBarType->addItem("DeterminateProgress");
	progressBarType->addItem("IndeterminateProgress");
	progressBarType->setCurrentText(QString(metaEnum.valueToKey(progressBar->getProgressType())));
	connect(progressBarType, &CALComboBox::currentTextChanged, this, [=](const QString& type) {
		if (metaEnum.isValid()) {
			if (const int value = metaEnum.keyToValue(type.toLocal8Bit().constData()); value != -1) {
				progressBar->setProgressType(static_cast<ALProgressType::ProgressType>(value));
			} else {
				qWarning() << "Invalid progress type: " << type;
			}
		}
	});
	const auto progressBarArea = new CALScrollPageArea(this);
	const auto progressBarHLayout = new QHBoxLayout(progressBarArea);
	const auto progressBarText = new CALText("CALProgressBar", this);
	progressBarText->setTextPixelSize(15);
	const auto progressBarProgerssColorButton = new CALPushButton("progress Color", this);
	progressBarProgerssColorButton->setFixedSize(120, 38);
	progressBarProgerssColorButton->setButtonStyles(CALPushButton::Shadow | CALPushButton::Border);
	connect(progressBarProgerssColorButton, &CALPushButton::clicked, colorDialog, &CALColorDialog::exec);
	progressBarHLayout->addWidget(progressBarText);
	progressBarHLayout->addWidget(progressBar);
	progressBarHLayout->addWidget(progressBarProgerssColorButton);
	progressBarHLayout->addWidget(progressBarType);
	progressBarHLayout->addStretch();
	const auto progressBarDisable = new CALToggleSwitch(this);
	const auto progressBarDisableText = new CALText(tr("Disable"), this);
	progressBarDisableText->setTextPixelSize(15);
	connect(progressBarDisable, &CALToggleSwitch::sigToggleChanged, progressBar, &CALProgressBar::setDisabled);
	progressBarHLayout->addWidget(progressBarDisableText);
	progressBarHLayout->addWidget(progressBarDisable);
	progressBarHLayout->addSpacing(10);

	mainVLayout->addWidget(progressBarArea);
}

void E_BaseComponents::initCheckBoxArea() {
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
}

void E_BaseComponents::initRadioButtonArea() {
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
}

void E_BaseComponents::initSpinBoxArea() {
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

	mainVLayout->addWidget(spinBoxArea);
}

void E_BaseComponents::initDoubleSpinBoxArea() {
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

	mainVLayout->addWidget(doubleSpinBoxArea);
}
