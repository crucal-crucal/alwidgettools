#include "e_tabwidget_example.hpp"

#include <QVBoxLayout>

#include "altabwidget.hpp"
#include "altext.hpp"

using namespace AL;

E_TabWidget_Example::E_TabWidget_Example(QWidget* parent): E_BasePage(parent) {
	setWindowTitle("CALTabWidget");
	createCustomWidget("some thing...");

	const auto centralWidget = new QWidget(this);
	centralWidget->setWindowTitle(this->windowTitle());
	mainVLayout = new QVBoxLayout(centralWidget);
	mainVLayout->setContentsMargins(0, 0, 0, 0);
	mainVLayout->setSpacing(5);

	const auto tabWidgetText = new CALText("CALTabWidget", this);
	tabWidgetText->setTextPixelSize(18);
	const auto tabWidget = new CALTabWidget(this);
	tabWidget->setFixedHeight(500);

	for (int i = 0; i < 5; ++i) {
		const auto page = new QLabel(QString("page %1").arg(i), this);
		page->setAlignment(Qt::AlignCenter);
		QFont font = page->font();
		font.setPixelSize(32);
		page->setFont(font);
		tabWidget->addTab(page, QIcon(":alwidgettools/image/crucal.png"), QString("tab %1").arg(i));
	}

	mainVLayout->addWidget(tabWidgetText);
	mainVLayout->addWidget(tabWidget);
	addCentralWidget(centralWidget, true, false, 0);
}

E_TabWidget_Example::~E_TabWidget_Example() = default;
