#include "e_listview.hpp"

#include <QHBoxLayout>

#include "allistview.hpp"
#include "alscrollbar.hpp"
#include "altext.hpp"
#include "e_listviewmodel.hpp"

using namespace AL;

E_ListView::E_ListView(QWidget* parent): E_BasePage(parent) {
	setWindowTitle("CALListView");
	createCustomWidget("some thing...");

	const auto centralWidget = new QWidget(this);
	centralWidget->setWindowTitle("CALView");
	mainVLayout = new QVBoxLayout(centralWidget);
	mainVLayout->setContentsMargins(0, 0, 0, 0);
	mainVLayout->setSpacing(5);

	/// listViewArea
	initListViewArea();

	mainVLayout->addStretch();
	addCentralWidget(centralWidget, true, false, 0);
}

E_ListView::~E_ListView() = default;

void E_ListView::initListViewArea() {
	const auto listText = new CALText("CALListView", this);
	listText->setTextPixelSize(18);
	const auto listView = new CALListView(this);
	listView->setFixedHeight(390);
	listView->setModel(new E_ListViewModel(this));
	const auto listViewFloatScrollBar = new CALScrollBar(listView->verticalScrollBar(), listView);
	listViewFloatScrollBar->setIsAnimation(true);
	const auto listViewHLayout = new QHBoxLayout;
	listViewHLayout->setContentsMargins(0, 0, 10, 0);
	listViewHLayout->addWidget(listView);

	mainVLayout->addWidget(listText);
	mainVLayout->addSpacing(10);
	mainVLayout->addLayout(listViewHLayout);
}
