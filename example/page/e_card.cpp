#include "e_card.hpp"

#include <QVBoxLayout>

#include "allcdnumber.hpp"
#include "alpromotioncard.hpp"
#include "alpromotionview.hpp"

E_Card::E_Card(QWidget* parent): E_BasePage(parent) {
	setWindowTitle("CALCard");
	createCustomWidget("some thing...");

	const auto centralWidget = new QWidget(this);
	centralWidget->setWindowTitle(this->windowTitle());
	mainVLayout = new QVBoxLayout(centralWidget);
	mainVLayout->setContentsMargins(0, 0, 0, 0);
	mainVLayout->setSpacing(5);

	/// lcdNumberArea
	initLCDNumberArea();
	/// promotionArea
	initPromotionCardArea();
	/// promotionViewArea
	initPromotionViewArea();

	mainVLayout->addStretch();
	addCentralWidget(centralWidget, true, true, 0);
}

E_Card::~E_Card() = default;

void E_Card::initLCDNumberArea() {
	const auto lcdNumber = new CALLCDNumber(this);
	lcdNumber->setIsUseAutoClock(true);
	lcdNumber->setIsTransparent(false);
	lcdNumber->setFixedHeight(100);

	mainVLayout->addWidget(lcdNumber);
}

void E_Card::initPromotionCardArea() {
	const auto promotionCard = new CALPromotionCard(this);
	promotionCard->setFixedSize(600, 300);
	promotionCard->setCardPixmap(QPixmap(":example/promotion/promotion5.jpg"));
	promotionCard->setCardTitle("crucal");
	promotionCard->setPromotionTitle("crucal promotion");
	promotionCard->setTitle("crucal title");
	promotionCard->setSubTitle("crucal subtitle");

	mainVLayout->addSpacing(20);
	mainVLayout->addWidget(promotionCard);
}

void E_Card::initPromotionViewArea() {
	const auto promotionView = new CALPromotionView(this);

	for (int i = 1; i <= 6; ++i) {
		const auto promotionCard = new CALPromotionCard(this);
		const QString promotionCardName = QString("promotion%1").arg(i);
		promotionCard->setCardPixmap(QPixmap(QString(":example/promotion/") + promotionCardName + ".jpg"));
		promotionCard->setCardTitle(promotionCardName);
		promotionCard->setCardTitleColor(Qt::black);
		promotionCard->setPromotionTitle(promotionCardName + " promotion");
		promotionCard->setPromotionTitleColor(Qt::black);
		promotionCard->setTitle(promotionCardName + " title");
		promotionCard->setTitleColor(Qt::black);
		promotionCard->setSubTitle(promotionCardName + " subtitle");
		promotionCard->setSubTitleColor(Qt::black);
		promotionView->appendPromotionCard(promotionCard);
	}

	promotionView->setIsAutoScroll(true);

	mainVLayout->addSpacing(20);
	mainVLayout->addWidget(promotionView);
}
