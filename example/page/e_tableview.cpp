#include "e_tableview.hpp"

#include <QHeaderView>
#include <QVBoxLayout>

#include "altableview.hpp"
#include "altext.hpp"
#include "e_tableviewmodel.hpp"

using namespace AL;

E_TableView::E_TableView(QWidget* parent): E_BasePage(parent) {
	setWindowTitle("CALTableView");
	createCustomWidget("some thing...");

	const auto centralWidget = new QWidget(this);
	centralWidget->setWindowTitle("CALView");
	mainVLayout = new QVBoxLayout(centralWidget);
	mainVLayout->setContentsMargins(0, 0, 0, 0);
	mainVLayout->setSpacing(5);

	/// tableViewArea
	initTableViewArea();

	mainVLayout->addStretch();
	addCentralWidget(centralWidget, true, false, 0);
}

E_TableView::~E_TableView() = default;

void E_TableView::initTableViewArea() {
	const auto tableViewText = new CALText("CALTableView", this);
	tableViewText->setTextPixelSize(18);
	const auto tableView = new CALTableView(this);
	QFont tableHeaderFont = tableView->horizontalHeader()->font();
	tableHeaderFont.setPixelSize(16);
	tableView->horizontalHeader()->setFont(tableHeaderFont);
	tableView->setModel(new E_TableViewModel(this));
	tableView->setAlternatingRowColors(true);
	tableView->setIconSize(QSize(38, 38));
	tableView->verticalHeader()->setHidden(true);
	tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Interactive);
	tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
	tableView->horizontalHeader()->setMinimumSectionSize(64);
	tableView->verticalHeader()->setMinimumSectionSize(46);
	tableView->setFixedHeight(390);
	connect(tableView, &CALTableView::sigTableViewShow, this, [tableView]() {
		tableView->setColumnWidth(0, 110);
		tableView->setColumnWidth(1, 120);
		tableView->setColumnWidth(2, 130);
		tableView->setColumnWidth(3, 140);
		tableView->setColumnWidth(4, 150);
	});
	const auto tableViewHLayout = new QHBoxLayout;
	tableViewHLayout->setContentsMargins(0, 0, 10, 0);
	tableViewHLayout->addWidget(tableView);

	mainVLayout->addWidget(tableViewText);
	mainVLayout->addSpacing(10);
	mainVLayout->addLayout(tableViewHLayout);
}
