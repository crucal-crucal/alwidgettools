#include "e_home.hpp"

#include <QVBoxLayout>

#include "alacrylicurlcard.hpp"
#include "alflowlayout.hpp"
#include "alimagecard.hpp"
#include "almessagebar.hpp"
#include "alscrollarea.hpp"
#include "altext.hpp"

using namespace AL;

E_Home::E_Home(QWidget* parent): E_BasePage(parent) {
	setWindowTitle("Home");
	setTitleVisible(false);
	setContentsMargins(2, 2, 0, 0);

	const auto centralWidget = new QWidget(this);
	centralWidget->setWindowTitle(this->windowTitle());
	mainVLayout = new QVBoxLayout(centralWidget);
	mainVLayout->setContentsMargins(0, 0, 0, 0);
	mainVLayout->setSpacing(5);

	/// titleCardArea
	initTitleCardArea();
	/// popularCardArea
	initPopularArea();

	mainVLayout->addStretch();
	addCentralWidget(centralWidget);
}

E_Home::~E_Home() = default;

void E_Home::initTitleCardArea() {
	const auto desText = new CALText("QWidget", this);
	desText->setTextPixelSize(18);
	const auto titleText = new CALText("alwidgettools", this);
	titleText->setTextPixelSize(30);
	const auto titleVLayout = new QVBoxLayout;
	titleVLayout->setContentsMargins(30, 60, 0, 0);
	titleVLayout->addWidget(desText);
	titleVLayout->addWidget(titleText);
	const auto backgroundCard = new CALImageCard(this);
	backgroundCard->setBorderRadius(10);
	backgroundCard->setFixedHeight(400);
	backgroundCard->setMaximumAspectRatio(1.7);
	backgroundCard->setCardImage(QImage(":example/promotion/promotion4.jpg"));
	const auto urlCard1 = new CALAcrylicUrlCard(this);
	urlCard1->setCardPixmapSize({ 62, 62 });
	urlCard1->setFixedSize(195, 225);
	urlCard1->setTitlePixelSize(17);
	urlCard1->setTitleSpacing(25);
	urlCard1->setSubTitlePixelSize(13);
	urlCard1->setTitle("urlCard1");
	urlCard1->setSubTitle("A Qt Widget Library");
	urlCard1->setCardPixmap(QPixmap(":example/promotion/promotion2.jpg"));
	urlCard1->setUrl("https://github.com/Al-Wu/alwidgettools");
	connect(urlCard1, &CALAcrylicUrlCard::sigOpenError, this, [=](const QString& errMsg) { CALMessageBar::error(tr("open error"), errMsg, 2000, ALMessageBarType::Top, window()); });
	const auto urlCard2 = new CALAcrylicUrlCard(this);
	urlCard2->setCardPixmapSize({ 62, 62 });
	urlCard2->setFixedSize(195, 225);
	urlCard2->setTitlePixelSize(17);
	urlCard2->setTitleSpacing(25);
	urlCard2->setSubTitlePixelSize(13);
	urlCard2->setTitle("urlCard2");
	urlCard2->setSubTitle("A Qt Widget Library");
	urlCard2->setCardPixmap(QPixmap(":example/promotion/promotion3.jpg"));
	urlCard2->setUrl("https://github.com/Al-Wu/alwidgettools");
	connect(urlCard2, &CALAcrylicUrlCard::sigOpenError, this, [=](const QString& errMsg) { CALMessageBar::error(tr("open error"), errMsg, 2000, ALMessageBarType::Top, window()); });
	const auto cardScrollArea = new CALScrollArea(this);
	cardScrollArea->setWidgetResizable(true);
	cardScrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	cardScrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	cardScrollArea->setIsGrabGesture(true, 0);
	cardScrollArea->setIsOverShoot(Qt::Horizontal, true);
	const auto cardScrollAreaWidget = new QWidget(this);
	cardScrollAreaWidget->setStyleSheet("background-color: transparent;");
	cardScrollArea->setWidget(cardScrollAreaWidget);
	const auto urlCardHLayout = new QHBoxLayout;
	urlCardHLayout->setContentsMargins(30, 0, 0, 0);
	urlCardHLayout->setSpacing(15);
	urlCardHLayout->addWidget(urlCard1);
	urlCardHLayout->addWidget(urlCard2);
	urlCardHLayout->addStretch();
	const auto cardScrollAreaVLayout = new QVBoxLayout(cardScrollAreaWidget);
	cardScrollAreaVLayout->setContentsMargins(0, 0, 0, 0);
	cardScrollAreaVLayout->addStretch();
	cardScrollAreaVLayout->addLayout(urlCardHLayout);
	const auto backgroundVLayout = new QVBoxLayout(backgroundCard);
	backgroundVLayout->setContentsMargins(0, 0, 0, 0);
	backgroundVLayout->addLayout(titleVLayout);
	backgroundVLayout->addWidget(cardScrollArea);

	mainVLayout->addWidget(backgroundCard);
	mainVLayout->addSpacing(20);
}

void E_Home::initPopularArea() {
	const auto flowLayout = new CALFlowLayout(0, 5, 5);
	flowLayout->setContentsMargins(30, 0, 0, 0);
	flowLayout->setIsAnimation(true);

	for (int i = 0; i < 6; i++) {
		const auto btn = new QPushButton(QString("button %1").arg(i), this);
		btn->setFixedSize(300, 100);
		flowLayout->addWidget(btn);
	}

	mainVLayout->addLayout(flowLayout);
}
