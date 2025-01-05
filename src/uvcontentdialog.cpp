#include "uvcontentdialog.hpp"

#include <QDebug>
#include <QKeyEvent>
#include <QVBoxLayout>

#include "uvcontentdialog_p.hpp"
#include "uvpushbutton.hpp"
#include "uvtext.hpp"
#include "uvthememanager.hpp"

/**
 * @brief \class CUVContentDialogPrivate
 * Internal class for CUVContentDialog
 * @param q pointer to the public class
 * @param parent pointer to the parent class
 */
CUVContentDialogPrivate::CUVContentDialogPrivate(CUVContentDialog* q, QObject* parent): QObject(parent), q_ptr(q) {
}

CUVContentDialogPrivate::~CUVContentDialogPrivate() = default;

/**
 * @brief \class CUVContentDialog
 * @param parent pointer to the parent class
 */
CUVContentDialog::CUVContentDialog(QWidget* parent): CUVBaseDialog(parent), d_ptr(new CUVContentDialogPrivate(this, this)) {
	Q_D(CUVContentDialog);

	resize(400, 100);
	setWindowModality(Qt::ApplicationModal);

#ifdef Q_OS_WIN
#if (QT_VERSION >= QT_VERSION_CHECK(6, 5, 3) && QT_VERSION <= QT_VERSION_CHECK(6, 6, 1))
    window()->setWindowFlags((window()->windowFlags()) | Qt::WindowMinimizeButtonHint | Qt::FramelessWindowHint);
#endif
#else
	window()->setWindowFlags((window()->windowFlags()) | Qt::FramelessWindowHint);
#endif

	d->closeButton = new CUVPushButton(tr("Close"), this);
	d->closeButton->setLightDefaultColor(UVThemeColor(UVThemeType::Light, UVThemeType::PrimaryNormal));
	d->closeButton->setDarkDefaultColor(UVThemeColor(UVThemeType::Dark, UVThemeType::PrimaryNormal));
	d->closeButton->setLightHoverColor(UVThemeColor(UVThemeType::Light, UVThemeType::PrimaryHover));
	d->closeButton->setDarkHoverColor(UVThemeColor(UVThemeType::Dark, UVThemeType::PrimaryHover));
	d->closeButton->setLightPressedColor(UVThemeColor(UVThemeType::Light, UVThemeType::PrimaryPress));
	d->closeButton->setDarkPressedColor(UVThemeColor(UVThemeType::Dark, UVThemeType::PrimaryPress));
	d->closeButton->setDarkTextColor(Qt::black);
	d->closeButton->setLightTextColor(Qt::white);
	d->closeButton->setMinimumSize(0, 0);
	d->closeButton->setMaximumSize(QSize(QWIDGETSIZE_MAX, QWIDGETSIZE_MAX));
	d->closeButton->setFixedHeight(38);
	d->closeButton->setBorderRadius(6);
	connect(d->closeButton, &CUVPushButton::clicked, this, [=]() {
		emit sigCloseButtonClicked();
		slotCloseButtonClicked();
		this->close();
	});

	d->cancelButton = new CUVPushButton(tr("Cancel"), this);
	d->cancelButton->setMinimumSize(0, 0);
	d->cancelButton->setMaximumSize(QSize(QWIDGETSIZE_MAX, QWIDGETSIZE_MAX));
	d->cancelButton->setFixedHeight(38);
	d->cancelButton->setBorderRadius(6);
	connect(d->cancelButton, &CUVPushButton::clicked, this, [=]() {
		emit sigCancelButtonClicked();
		slotCancelButtonClicked();
		this->close();
	});

	// const auto title = new QLabel(tr("exit"), this);
	// title->setObjectName("CUVContentDialog_title");
	const auto subTitle = new CUVText(tr("Are you sure you want to exit?"), this);
	subTitle->setTextStyle(UVTextType::Body);

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

CUVContentDialog::~CUVContentDialog() = default;

void CUVContentDialog::slotCloseButtonClicked() {
}

void CUVContentDialog::slotCancelButtonClicked() {
}

void CUVContentDialog::setCenterWidget(QWidget* centerWidget) {
	Q_D(CUVContentDialog);

	d->mainVLayout->takeAt(0);
	d->mainVLayout->takeAt(0);
	delete d->centerWidget;
	d->mainVLayout->addWidget(centerWidget);
	d->mainVLayout->addWidget(d->buttonWidget);
}

void CUVContentDialog::setCloseButtonText(const QString& text) {
	d_func()->closeButton->setText(text);
}

void CUVContentDialog::setCancelButtonText(const QString& text) {
	d_func()->cancelButton->setText(text);
}

void CUVContentDialog::keyPressEvent(QKeyEvent* event) {
	event->accept();
}
