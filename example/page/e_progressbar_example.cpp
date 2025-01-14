#include "e_progressbar_example.hpp"

#include <QDebug>
#include <QHBoxLayout>
#include <QMetaEnum>

#include "alcolordialog.hpp"
#include "alcombobox.hpp"
#include "alprogressbar.hpp"
#include "alpushbutton.hpp"
#include "alscrollpagearea.hpp"
#include "altext.hpp"
#include "altoggleswitch.hpp"

using namespace AL;

E_ProgressBar_Example::E_ProgressBar_Example(QWidget* parent): E_BasePage(parent) {
	setWindowTitle("CALProgressBar");
	createCustomWidget("some thing...");

	const auto centralWidget = new QWidget(this);
	centralWidget->setWindowTitle(this->windowTitle());
	mainVLayout = new QVBoxLayout(centralWidget);
	mainVLayout->setContentsMargins(0, 0, 0, 0);
	mainVLayout->setSpacing(5);
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

	mainVLayout->addStretch();
	addCentralWidget(centralWidget, true, true, 0);
}

E_ProgressBar_Example::~E_ProgressBar_Example() = default;
