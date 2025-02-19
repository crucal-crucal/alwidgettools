#include "alcontentdialog.hpp"

#include <QDebug>
#include <QKeyEvent>
#include <QVBoxLayout>

#include "alcontentdialog_p.hpp"
#include "alpushbutton.hpp"
#include "altext.hpp"
#include "althememanager.hpp"

/**
 * @brief \namespace AL
 */
namespace AL {
/**
 * @brief \class CALContentDialogPrivate
 * Internal class for CALContentDialog
 * @param q pointer to the public class
 * @param parent pointer to the parent class
 */
CALContentDialogPrivate::CALContentDialogPrivate(CALContentDialog* q, QObject* parent): QObject(parent), q_ptr(q) {
}

CALContentDialogPrivate::~CALContentDialogPrivate() = default;

/**
 * @brief \class CALContentDialog
 * @param parent pointer to the parent class
 */
CALContentDialog::CALContentDialog(QWidget* parent): CALBaseDialog(parent), d_ptr(new CALContentDialogPrivate(this, this)) {
	Q_D(CALContentDialog);

	resize(400, 100);
	setWindowModality(Qt::ApplicationModal);

#ifdef Q_OS_WIN
#if (QT_VERSION >= QT_VERSION_CHECK(6, 5, 3) && QT_VERSION <= QT_VERSION_CHECK(6, 6, 1))
	window()->setWindowFlags((window()->windowFlags()) | Qt::WindowMinimizeButtonHint | Qt::FramelessWindowHint);
#endif
#else
	window()->setWindowFlags((window()->windowFlags()) | Qt::FramelessWindowHint);
#endif

	d->closeButton = new CALPushButton(tr("Close"), this);
	d->closeButton->setLightDefaultColor(ALThemeColor(ALThemeType::Light, ALThemeType::PrimaryNormal));
	d->closeButton->setDarkDefaultColor(ALThemeColor(ALThemeType::Dark, ALThemeType::PrimaryNormal));
	d->closeButton->setLightHoverColor(ALThemeColor(ALThemeType::Light, ALThemeType::PrimaryHover));
	d->closeButton->setDarkHoverColor(ALThemeColor(ALThemeType::Dark, ALThemeType::PrimaryHover));
	d->closeButton->setLightPressedColor(ALThemeColor(ALThemeType::Light, ALThemeType::PrimaryPress));
	d->closeButton->setDarkPressedColor(ALThemeColor(ALThemeType::Dark, ALThemeType::PrimaryPress));
	d->closeButton->setDarkTextColor(Qt::black);
	d->closeButton->setLightTextColor(Qt::white);
	d->closeButton->setMinimumSize(0, 0);
	d->closeButton->setMaximumSize(QSize(QWIDGETSIZE_MAX, QWIDGETSIZE_MAX));
	d->closeButton->setFixedHeight(38);
	d->closeButton->setBorderRadius(6);
	connect(d->closeButton, &CALPushButton::clicked, this, [this]() {
		Q_EMIT sigCloseButtonClicked();
		slotCloseButtonClicked();
		this->close();
	});

	d->cancelButton = new CALPushButton(tr("Cancel"), this);
	d->cancelButton->setMinimumSize(0, 0);
	d->cancelButton->setMaximumSize(QSize(QWIDGETSIZE_MAX, QWIDGETSIZE_MAX));
	d->cancelButton->setFixedHeight(38);
	d->cancelButton->setBorderRadius(6);
	connect(d->cancelButton, &CALPushButton::clicked, this, [this]() {
		Q_EMIT sigCancelButtonClicked();
		slotCancelButtonClicked();
		this->close();
	});

	// const auto title = new CALText(tr("exit"), this);
	// title->setObjectName("CALContentDialog_title");
	const auto subTitle = new CALText(tr("Are you sure you want to exit?"), this);
	subTitle->setTextStyle(ALTextType::Body);

	const auto subTitleHLayout = new QHBoxLayout;
	subTitleHLayout->setContentsMargins(0, 0, 0, 0);
	subTitleHLayout->setSpacing(0);
	subTitleHLayout->addWidget(subTitle);
	subTitle->setAlignment(Qt::AlignCenter);

	d->centerWidget = new QWidget(this);
	const auto centralVLayout = new QVBoxLayout(d->centerWidget);
	centralVLayout->setContentsMargins(15, 25, 15, 10);
	centralVLayout->setSpacing(2);
	// centralVLayout->addWidget(title);
	centralVLayout->addLayout(subTitleHLayout);
	centralVLayout->addStretch();

	d->buttonWidget = new QWidget(this);
	d->buttonWidget->setFixedHeight(60);
	const auto buttonHLayout = new QHBoxLayout(d->buttonWidget);
	buttonHLayout->addWidget(d->closeButton);
	buttonHLayout->addWidget(d->cancelButton);

	d->mainVLayout = new QVBoxLayout(this);
	d->mainVLayout->setContentsMargins(0, 0, 0, 0);
	d->mainVLayout->addWidget(d->centerWidget);
	d->mainVLayout->addWidget(d->buttonWidget);
}

CALContentDialog::~CALContentDialog() = default;

void CALContentDialog::slotCloseButtonClicked() {
}

void CALContentDialog::slotCancelButtonClicked() {
}

void CALContentDialog::setCenterWidget(QWidget* centerWidget) {
	Q_D(CALContentDialog);

	d->mainVLayout->takeAt(0);
	d->mainVLayout->takeAt(0);
	delete d->centerWidget;
	d->mainVLayout->addWidget(centerWidget);
	d->mainVLayout->addWidget(d->buttonWidget);
}

void CALContentDialog::setCloseButtonText(const QString& text) {
	d_func()->closeButton->setText(text);
}

void CALContentDialog::setCancelButtonText(const QString& text) {
	d_func()->cancelButton->setText(text);
}

void CALContentDialog::keyPressEvent(QKeyEvent* event) {
	event->accept();
}
}
