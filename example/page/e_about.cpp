#include "e_about.hpp"

#include <QIcon>
#include <QVBoxLayout>

#include "alimagecard.hpp"
#include "altext.hpp"

E_About::E_About(QWidget* parent): CALWidget(parent) {
	setWindowTitle("about...");
	setWindowIcon(QIcon(":/image/crucal.png"));
	setWindowModality(Qt::ApplicationModal);
	setIsStayTop(true);
	setWindowButtonFlags(ALAppBarType::CloseButtonHint);

	m_contentHLayout = new QHBoxLayout;
	m_contentHLayout->addSpacing(30);

	const auto mainVLayout = new QVBoxLayout(this);
	mainVLayout->setContentsMargins(0, 25, 0, 0);
	mainVLayout->addLayout(m_contentHLayout);

	initImageCardArea();
	initTextArea();
}

E_About::~E_About() = default;

void E_About::initImageCardArea() {
	const auto pixCard = new CALImageCard(this);
	pixCard->setFixedSize(60, 60);
	pixCard->setIsPreserveAspectCrop(false);
	pixCard->setCardImage(QImage(":/image/crucal.png"));
	const auto pixCardVLayout = new QVBoxLayout;
	pixCardVLayout->addWidget(pixCard);
	pixCardVLayout->addStretch();

	m_contentHLayout->addLayout(pixCardVLayout);
}

void E_About::initTextArea() {
	const auto versionText = new CALText("alwidgettools-LK-2024", this);
	QFont versionTextFont = versionText->font();
	versionTextFont.setWeight(QFont::Bold);
	versionText->setFont(versionTextFont);
	versionText->setWordWrap(false);
	versionText->setTextPixelSize(18);

	const auto supportText = new CALText("Windows 10/11, Linux, macOS", this);
	supportText->setWordWrap(false);
	supportText->setTextPixelSize(14);
	const auto contactText = new CALText("author: crucal", this);
	contactText->setWordWrap(false);
	contactText->setTextPixelSize(14);

	const auto textVLayout = new QVBoxLayout;
	textVLayout->addSpacing(15);
	textVLayout->addWidget(versionText);
	textVLayout->addWidget(supportText);
	textVLayout->addWidget(contactText);
	textVLayout->addStretch();

	m_contentHLayout->addLayout(textVLayout);
}
