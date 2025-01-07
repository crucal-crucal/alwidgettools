#include "e_basecomponents.hpp"

#include <QDebug>
#include <QHBoxLayout>
#include <QMetaEnum>
#include <QMouseEvent>

#include "alcheckbox.hpp"
#include "alcircularprogress.hpp"
#include "alcolordialog.hpp"
#include "alcombobox.hpp"
#include "aldoublespinbox.hpp"
#include "uvmessagebar.hpp"
#include "uvmultiselectcombobox.hpp"
#include "uvprogressbar.hpp"
#include "uvpushbutton.hpp"
#include "uvradiobutton.hpp"
#include "uvscrollpagearea.hpp"
#include "uvslider.hpp"
#include "uvspinbox.hpp"
#include "uvtext.hpp"
#include "uvtoggleswitch.hpp"

E_BaseComponents::E_BaseComponents(QWidget* parent): E_BasePage(parent) {
	setWindowTitle("CUVBaseComponents");
	createCustomWidget("some thing...");

	const auto centralWidget = new QWidget(this);
	centralWidget->setWindowTitle("CUVBaseComponents");
	m_mainVLayout = new QVBoxLayout(centralWidget);
	m_mainVLayout->setContentsMargins(0, 0, 0, 0);
	m_mainVLayout->setSpacing(5);

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

	m_mainVLayout->addStretch();
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

void E_BaseComponents::initToggleSwitchArea() {
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
	connect(toggleSwitchDisable, &CUVToggleSwitch::sigToggleChanged, toggleSwitch, &CUVToggleSwitch::setDisabled);
	toggleSwitchHLayout->addWidget(toggleSwitchDisableText);
	toggleSwitchHLayout->addWidget(toggleSwitchDisable);
	toggleSwitchHLayout->addSpacing(10);

	m_mainVLayout->addWidget(toggleSwitchArea);
}

void E_BaseComponents::initMessageBarArea() {
	const auto successButton = new CALPushButton("Success", this);
	successButton->setFixedSize(80, 38);
	successButton->setButtonStyles(CALPushButton::Shadow | CALPushButton::Border);
	connect(successButton, &CALPushButton::clicked, this, [=]() { CUVMessageBar::success("Success", "This is a success message"); });
	const auto warningButton = new CALPushButton("Warning", this);
	warningButton->setFixedSize(80, 38);
	warningButton->setButtonStyles(CALPushButton::Shadow | CALPushButton::Border);
	connect(warningButton, &CALPushButton::clicked, this, [=]() { CUVMessageBar::warning("Warning", "This is a warning message"); });
	const auto errorButton = new CALPushButton("Error", this);
	errorButton->setFixedSize(80, 38);
	errorButton->setButtonStyles(CALPushButton::Shadow | CALPushButton::Border);
	connect(errorButton, &CALPushButton::clicked, this, [=]() { CUVMessageBar::error("Error", "This is an error message"); });
	const auto infoButton = new CALPushButton("Info", this);
	infoButton->setFixedSize(80, 38);
	infoButton->setButtonStyles(CALPushButton::Shadow | CALPushButton::Border);
	connect(infoButton, &CALPushButton::clicked, this, [=]() { CUVMessageBar::information("Info", "This is an info message"); });
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

	m_mainVLayout->addWidget(messageBarArea);
}

void E_BaseComponents::initCircularProgressArea() {
	const QMetaObject& metaObject = ALProgressType::staticMetaObject;
	const QMetaEnum metaEnum = metaObject.enumerator(metaObject.indexOfEnumerator("ProgressType"));
	if (!metaEnum.isValid()) {
		qWarning() << "Invalid ProgressType meta enum";
		return;
	}

	const auto colorDialog = new CUVColorDialog(this);
	const auto circularProgress = new CUVCircularProgress(this);
	circularProgress->setShowProgressText(false);
	circularProgress->setSize(30);
	circularProgress->setLineWidth(5.0);
	circularProgress->setColor(colorDialog->getCurrentColor());
	connect(colorDialog, &CUVColorDialog::sigColorSelected, circularProgress, &CUVCircularProgress::setColor);
	const auto circularProgressType = new CUVComboBox(this);
	for (int i = 0; i < metaEnum.keyCount(); i++) {
		circularProgressType->addItem(metaEnum.key(i));
	}
	circularProgressType->setCurrentText(QString(metaEnum.valueToKey(circularProgress->getProgressType())));
	connect(circularProgressType, &CUVComboBox::currentTextChanged, this, [=](const QString& type) {
		if (metaEnum.isValid()) {
			if (const int value = metaEnum.keyToValue(type.toLocal8Bit().constData()); value != -1) {
				circularProgress->setProgressType(static_cast<ALProgressType::ProgressType>(value));
			} else {
				qWarning() << "Invalid progress type: " << type;
			}
		}
	});
	const auto circularProgressArea = new CUVScrollPageArea(this);
	const auto circularProgressHLayout = new QHBoxLayout(circularProgressArea);
	const auto circularProgressText = new CUVText("CUVCircularProgress", this);
	circularProgressText->setTextPixelSize(15);
	const auto circularProgerssColorButton = new CALPushButton("Color", this);
	circularProgerssColorButton->setFixedSize(80, 38);
	circularProgerssColorButton->setButtonStyles(CALPushButton::Shadow | CALPushButton::Border);
	connect(circularProgerssColorButton, &CALPushButton::clicked, colorDialog, &CUVColorDialog::exec);
	circularProgressHLayout->addWidget(circularProgressText);
	circularProgressHLayout->addWidget(circularProgress);
	circularProgressHLayout->addWidget(circularProgerssColorButton);
	circularProgressHLayout->addWidget(circularProgressType);
	circularProgressHLayout->addStretch();
	const auto circularProgressDisable = new CUVToggleSwitch(this);
	const auto circularProgressDisableText = new CUVText("Disable", this);
	circularProgressDisableText->setTextPixelSize(15);
	connect(circularProgressDisable, &CUVToggleSwitch::sigToggleChanged, circularProgress, &CUVCircularProgress::setDisabled);
	circularProgressHLayout->addWidget(circularProgressDisableText);
	circularProgressHLayout->addWidget(circularProgressDisable);
	circularProgressHLayout->addSpacing(10);

	m_mainVLayout->addWidget(circularProgressArea);
}

void E_BaseComponents::initMultiSelectComboBoxArea() {
	const auto multiSelectComboBox = new CUVMultiSelectComboBox(this);
	multiSelectComboBox->setFixedWidth(300);
	const auto multiSelectComboBoxArea = new CUVScrollPageArea(this);
	const auto multiSelectComboBoxHLayout = new QHBoxLayout(multiSelectComboBoxArea);
	const auto multiSelectComboBoxText = new CUVText("CUVMultiSelectComboBox", this);
	multiSelectComboBoxText->setTextPixelSize(15);
	multiSelectComboBoxHLayout->addWidget(multiSelectComboBoxText);
	multiSelectComboBoxHLayout->addWidget(multiSelectComboBox);
	multiSelectComboBoxHLayout->addStretch();
	const auto multiSelectComboBoxDisable = new CUVToggleSwitch(this);
	const auto multiSelectComboBoxDisableText = new CUVText("Disable", this);
	multiSelectComboBoxDisableText->setTextPixelSize(15);
	connect(multiSelectComboBoxDisable, &CUVToggleSwitch::sigToggleChanged, multiSelectComboBox, &CUVMultiSelectComboBox::setDisabled);
	multiSelectComboBoxHLayout->addWidget(multiSelectComboBoxDisableText);
	multiSelectComboBoxHLayout->addWidget(multiSelectComboBoxDisable);
	multiSelectComboBoxHLayout->addSpacing(10);
	multiSelectComboBox->addItem("select all");
	for (int i = 0; i < 15; i++) {
		multiSelectComboBox->addItem(QString("test %1").arg(i));
	}
	multiSelectComboBox->setCurrentSelection(QList<int>{ 1, 2, 3, 4, 5 });

	m_mainVLayout->addWidget(multiSelectComboBoxArea);
}

void E_BaseComponents::initSliderArea() {
	const auto slider = new CUVSlider(this);
	const auto sliderArea = new CUVScrollPageArea(this);
	const auto sliderHLayout = new QHBoxLayout(sliderArea);
	const auto sliderText = new CUVText("CUVSlider", this);
	sliderText->setTextPixelSize(15);
	sliderHLayout->addWidget(sliderText);
	sliderHLayout->addWidget(slider);
	sliderHLayout->addStretch();
	const auto sliderDisable = new CUVToggleSwitch(this);
	const auto sliderDisableText = new CUVText("Disable", this);
	sliderDisableText->setTextPixelSize(15);
	connect(sliderDisable, &CUVToggleSwitch::sigToggleChanged, slider, &CUVSlider::setDisabled);
	sliderHLayout->addWidget(sliderDisableText);
	sliderHLayout->addWidget(sliderDisable);
	sliderHLayout->addSpacing(10);

	m_mainVLayout->addWidget(sliderArea);
}

void E_BaseComponents::initProgressBarArea() {
	const QMetaObject& metaObject = ALProgressType::staticMetaObject;
	const QMetaEnum metaEnum = metaObject.enumerator(metaObject.indexOfEnumerator("ProgressType"));
	if (!metaEnum.isValid()) {
		qWarning() << "Invalid ProgressType meta enum";
		return;
	}

	const auto colorDialog = new CUVColorDialog(this);
	const auto progressBar = new CUVProgressBar(this);
	progressBar->setProgressColor(colorDialog->getCurrentColor());
	connect(colorDialog, &CUVColorDialog::sigColorSelected, progressBar, &CUVProgressBar::setProgressColor);
	const auto progressBarType = new CUVComboBox(this);
	progressBarType->addItem("DeterminateProgress");
	progressBarType->addItem("IndeterminateProgress");
	progressBarType->setCurrentText(QString(metaEnum.valueToKey(progressBar->getProgressType())));
	connect(progressBarType, &CUVComboBox::currentTextChanged, this, [=](const QString& type) {
		if (metaEnum.isValid()) {
			if (const int value = metaEnum.keyToValue(type.toLocal8Bit().constData()); value != -1) {
				progressBar->setProgressType(static_cast<ALProgressType::ProgressType>(value));
			} else {
				qWarning() << "Invalid progress type: " << type;
			}
		}
	});
	const auto progressBarArea = new CUVScrollPageArea(this);
	const auto progressBarHLayout = new QHBoxLayout(progressBarArea);
	const auto progressBarText = new CUVText("CUVProgressBar", this);
	progressBarText->setTextPixelSize(15);
	const auto progressBarProgerssColorButton = new CALPushButton("progress Color", this);
	progressBarProgerssColorButton->setFixedSize(120, 38);
	progressBarProgerssColorButton->setButtonStyles(CALPushButton::Shadow | CALPushButton::Border);
	connect(progressBarProgerssColorButton, &CALPushButton::clicked, colorDialog, &CUVColorDialog::exec);
	progressBarHLayout->addWidget(progressBarText);
	progressBarHLayout->addWidget(progressBar);
	progressBarHLayout->addWidget(progressBarProgerssColorButton);
	progressBarHLayout->addWidget(progressBarType);
	progressBarHLayout->addStretch();
	const auto progressBarDisable = new CUVToggleSwitch(this);
	const auto progressBarDisableText = new CUVText("Disable", this);
	progressBarDisableText->setTextPixelSize(15);
	connect(progressBarDisable, &CUVToggleSwitch::sigToggleChanged, progressBar, &CUVProgressBar::setDisabled);
	progressBarHLayout->addWidget(progressBarDisableText);
	progressBarHLayout->addWidget(progressBarDisable);
	progressBarHLayout->addSpacing(10);

	m_mainVLayout->addWidget(progressBarArea);
}

void E_BaseComponents::initCheckBoxArea() {
	const auto checkBox = new CUVCheckBox("checkBox", this);
	const auto checkBoxArea = new CUVScrollPageArea(this);
	const auto checkBoxHLayout = new QHBoxLayout(checkBoxArea);
	const auto checkBoxText = new CUVText("CUVCheckBox", this);
	checkBoxText->setTextPixelSize(15);
	checkBoxHLayout->addWidget(checkBoxText);
	checkBoxHLayout->addWidget(checkBox);
	checkBoxHLayout->addStretch();
	const auto checkBoxDisable = new CUVToggleSwitch(this);
	const auto checkBoxDisableText = new CUVText("Disable", this);
	checkBoxDisableText->setTextPixelSize(15);
	connect(checkBoxDisable, &CUVToggleSwitch::sigToggleChanged, checkBox, &CUVSlider::setDisabled);
	checkBoxHLayout->addWidget(checkBoxDisableText);
	checkBoxHLayout->addWidget(checkBoxDisable);
	checkBoxHLayout->addSpacing(10);

	m_mainVLayout->addWidget(checkBoxArea);
}

void E_BaseComponents::initRadioButtonArea() {
	const auto radioButton = new CUVRadioButton("radioButton", this);
	const auto radioButtonArea = new CUVScrollPageArea(this);
	const auto radioButtonHLayout = new QHBoxLayout(radioButtonArea);
	const auto radioButtonText = new CUVText("CUVRadioButton", this);
	radioButtonText->setTextPixelSize(15);
	radioButtonHLayout->addWidget(radioButtonText);
	radioButtonHLayout->addWidget(radioButton);
	radioButtonHLayout->addStretch();
	const auto radioButtonDisable = new CUVToggleSwitch(this);
	const auto radioButtonDisableText = new CUVText("Disable", this);
	radioButtonDisableText->setTextPixelSize(15);
	connect(radioButtonDisable, &CUVToggleSwitch::sigToggleChanged, radioButton, &CUVRadioButton::setDisabled);
	radioButtonHLayout->addWidget(radioButtonDisableText);
	radioButtonHLayout->addWidget(radioButtonDisable);
	radioButtonHLayout->addSpacing(10);

	m_mainVLayout->addWidget(radioButtonArea);
}

void E_BaseComponents::initSpinBoxArea() {
	const auto spinBox = new CUVSpinBox(this);
	const auto spinBoxArea = new CUVScrollPageArea(this);
	const auto spinBoxHLayout = new QHBoxLayout(spinBoxArea);
	const auto spinBoxText = new CUVText("CUVSpinBox", this);
	spinBoxText->setTextPixelSize(15);
	spinBoxHLayout->addWidget(spinBoxText);
	spinBoxHLayout->addWidget(spinBox);
	spinBoxHLayout->addStretch();
	const auto spinBoxDisable = new CUVToggleSwitch(this);
	const auto spinBoxDisableText = new CUVText("Disable", this);
	spinBoxDisableText->setTextPixelSize(15);
	connect(spinBoxDisable, &CUVToggleSwitch::sigToggleChanged, spinBox, &CUVSpinBox::setDisabled);
	spinBoxHLayout->addWidget(spinBoxDisableText);
	spinBoxHLayout->addWidget(spinBoxDisable);
	spinBoxHLayout->addSpacing(10);

	m_mainVLayout->addWidget(spinBoxArea);
}

void E_BaseComponents::initDoubleSpinBoxArea() {
	const auto doubleSpinBox = new CUVDoubleSpinBox(this);
	const auto doubleSpinBoxArea = new CUVScrollPageArea(this);
	const auto doubleSpinBoxHLayout = new QHBoxLayout(doubleSpinBoxArea);
	const auto doubleSpinBoxText = new CUVText("CUVDoubleSpinBox", this);
	doubleSpinBoxText->setTextPixelSize(15);
	doubleSpinBoxHLayout->addWidget(doubleSpinBoxText);
	doubleSpinBoxHLayout->addWidget(doubleSpinBox);
	doubleSpinBoxHLayout->addStretch();
	const auto doubleSpinBoxDisable = new CUVToggleSwitch(this);
	const auto doubleSpinBoxDisableText = new CUVText("Disable", this);
	doubleSpinBoxDisableText->setTextPixelSize(15);
	connect(doubleSpinBoxDisable, &CUVToggleSwitch::sigToggleChanged, doubleSpinBox, &CUVDoubleSpinBox::setDisabled);
	doubleSpinBoxHLayout->addWidget(doubleSpinBoxDisableText);
	doubleSpinBoxHLayout->addWidget(doubleSpinBoxDisable);
	doubleSpinBoxHLayout->addSpacing(10);

	m_mainVLayout->addWidget(doubleSpinBoxArea);
}
