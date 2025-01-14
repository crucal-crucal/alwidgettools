#include "e_navigation.hpp"

#include <QVBoxLayout>

#include "albreadcrumbbar.hpp"
#include "alpovit.hpp"
#include "alpushbutton.hpp"
#include "alscrollpagearea.hpp"
#include "altext.hpp"

using namespace AL;

E_Navigation::E_Navigation(QWidget* parent): E_BasePage(parent) {
	setWindowTitle("CALNavigation");
	createCustomWidget("some thing...");

	const auto centralWidget = new QWidget(this);
	centralWidget->setWindowTitle(this->windowTitle());
	mainVLayout = new QVBoxLayout(centralWidget);
	mainVLayout->setContentsMargins(0, 0, 0, 0);
	mainVLayout->setSpacing(5);

	/// BreadCrumbBarArea
	initBreadCrumbBarArea();
	/// PovitArea
	initPovitArea();

	mainVLayout->addStretch();
	addCentralWidget(centralWidget, true, true, 0);
}

E_Navigation::~E_Navigation() = default;

void E_Navigation::initBreadCrumbBarArea() {
	const auto breadcrumbBarText = new CALText("CALBreadcrumbBar", this);
	breadcrumbBarText->setTextPixelSize(18);
	const auto breadcrumbBar = new CALBreadcrumbBar(this);
	QStringList breadcrumbBarList;
	for (int i = 0; i < 20; ++i) {
		breadcrumbBarList.append(QString("item %1").arg(i));
	}
	breadcrumbBar->setBreadcrumbList(breadcrumbBarList);
	const auto resetButton = new CALPushButton(tr("reset"), this);
	resetButton->setFixedSize(60, 32);
	connect(resetButton, &CALPushButton::clicked, this, [=]() {
		breadcrumbBar->setBreadcrumbList(breadcrumbBarList);
	});

	const auto breadcrumbBarTextHLayout = new QHBoxLayout;
	breadcrumbBarTextHLayout->addWidget(breadcrumbBarText);
	breadcrumbBarTextHLayout->addSpacing(15);
	breadcrumbBarTextHLayout->addWidget(resetButton);
	breadcrumbBarTextHLayout->addStretch();
	const auto breadcrumbBarArea = new CALScrollPageArea(this);
	const auto breadcrumbBarVLayout = new QVBoxLayout(breadcrumbBarArea);
	breadcrumbBarVLayout->addWidget(breadcrumbBar);

	mainVLayout->addLayout(breadcrumbBarTextHLayout);
	mainVLayout->addSpacing(10);
	mainVLayout->addWidget(breadcrumbBarArea);
	mainVLayout->addSpacing(15);
}

void E_Navigation::initPovitArea() {
	const auto povitText = new CALText("CALPovit", this);
	povitText->setTextPixelSize(18);
	const auto povit = new CALPovit(this);
	povit->setPovitSpacing(8);
	povit->setMarkWidth(75);
	for (int i = 0; i < 7; ++i) {
		povit->appendPovit(QString("povit %1").arg(i));
	}
	povit->setCurrentIndex(0);
	const auto povitArea = new CALScrollPageArea(this);
	const auto povitVLayout = new QVBoxLayout(povitArea);
	povitVLayout->addWidget(povit);

	mainVLayout->addWidget(povitText);
	mainVLayout->addSpacing(10);
	mainVLayout->addWidget(povitArea);
}
