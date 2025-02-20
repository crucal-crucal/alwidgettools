#include "e_circularprogress_example.hpp"

#include <QDebug>
#include <QHBoxLayout>
#include <QMetaEnum>

#include "alcircularprogress.hpp"
#include "alcolordialog.hpp"
#include "alcombobox.hpp"
#include "alpushbutton.hpp"
#include "alscrollpagearea.hpp"
#include "altext.hpp"
#include "altoggleswitch.hpp"

using namespace AL;

E_CircularProgress_Example::E_CircularProgress_Example(QWidget* parent): E_BasePage(parent) {
	setWindowTitle("CALCircularProgress");
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
	const auto circularProgress = new CALCircularProgress(this);
	circularProgress->setShowProgressText(false);
	circularProgress->setSize(30);
	circularProgress->setLineWidth(5.0);
	circularProgress->setColor(colorDialog->getCurrentColor());
	connect(colorDialog, &CALColorDialog::sigColorSelected, circularProgress, &CALCircularProgress::setColor);
	const auto circularProgressType = new CALComboBox(this);
	for (int i = 0; i < metaEnum.keyCount(); ++i) {
		circularProgressType->addItem(metaEnum.key(i));
	}
	circularProgressType->setCurrentText(QString(metaEnum.valueToKey(circularProgress->getProgressType())));
	connect(circularProgressType, &CALComboBox::currentTextChanged, this, [metaEnum, circularProgress](const QString& type) {
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
	mainVLayout->addStretch();
	addCentralWidget(centralWidget, true, true, 0);
}

E_CircularProgress_Example::~E_CircularProgress_Example() = default;
