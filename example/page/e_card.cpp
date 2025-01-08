#include "e_card.hpp"

#include <QVBoxLayout>

#include "allcdnumber.hpp"

E_Card::E_Card(QWidget* parent): E_BasePage(parent) {
	setWindowTitle("CALCard");
	createCustomWidget("some thing...");

	const auto centralWidget = new QWidget(this);
	centralWidget->setWindowTitle("CALCard");
	mainVLayout = new QVBoxLayout(centralWidget);
	mainVLayout->setContentsMargins(0, 0, 0, 0);
	mainVLayout->setSpacing(5);

	/// lcdNumberArea
	initLCDNumberArea();

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
