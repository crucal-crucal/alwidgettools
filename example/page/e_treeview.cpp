#include "e_treeview.hpp"

#include <QHBoxLayout>
#include <QHeaderView>

#include "alpushbutton.hpp"
#include "alscrollbar.hpp"
#include "alslider.hpp"
#include "altext.hpp"
#include "altreeview.hpp"
#include "e_treeviewmodel.hpp"

using namespace AL;

E_TreeView::E_TreeView(QWidget* parent): E_BasePage(parent) {
	setWindowTitle("CALTreeView");
	createCustomWidget("some thing...");

	const auto centralWidget = new QWidget(this);
	centralWidget->setWindowTitle("CALView");
	mainVLayout = new QVBoxLayout(centralWidget);
	mainVLayout->setContentsMargins(0, 0, 0, 0);
	mainVLayout->setSpacing(5);

	/// treeViewArea
	initTreeViewArea();

	mainVLayout->addStretch();
	addCentralWidget(centralWidget, true, false, 0);
}

E_TreeView::~E_TreeView() = default;

void E_TreeView::initTreeViewArea() {
	/// treeView
	const auto model = new E_TreeViewModel(this);
	const auto treeViewText = new CALText("CALTreeView", this);
	treeViewText->setTextPixelSize(18);
	const auto treeView = new CALTreeView(this);
	const auto treeViewFloatScrollBar = new CALScrollBar(treeView->verticalScrollBar(), treeView);
	treeViewFloatScrollBar->setIsAnimation(true);
	QFont headerFont = treeView->header()->font();
	headerFont.setPixelSize(16);
	treeView->header()->setFont(headerFont);
	treeView->setFixedHeight(390);
	treeView->setModel(model);

	/// tree settings
	const auto treeSettingWidget = new QWidget(this);
	const auto treeSettingVLayout = new QVBoxLayout(treeSettingWidget);
	treeSettingVLayout->setContentsMargins(0, 0, 0, 0);
	treeSettingVLayout->setSpacing(15);
	// data
	const auto dataText = new CALText(tr("number: ") + QString::number(model->getItemCount()), this);
	dataText->setTextPixelSize(15);
	const auto itemHeightText = new CALText(tr("ItemHeight"), this);
	itemHeightText->setTextPixelSize(15);
	const auto itemHeightSlider = new CALSlider(this);
	itemHeightSlider->setRange(200, 600);
	itemHeightSlider->setValue(350);
	const auto itemHeightValueText = new CALText("35", this);
	itemHeightValueText->setTextPixelSize(15);
	connect(itemHeightSlider, &CALSlider::valueChanged, this, [=](const int value) {
		itemHeightValueText->setText(QString::number(value / 10));
		treeView->setItemHeight(value / 10);
	});
	const auto itemHeightHLayout = new QHBoxLayout;
	itemHeightHLayout->setContentsMargins(0, 0, 0, 0);
	itemHeightHLayout->addWidget(itemHeightText);
	itemHeightHLayout->addWidget(itemHeightSlider);
	itemHeightHLayout->addWidget(itemHeightValueText);
	// HeaderMargin
	const auto headerMarginText = new CALText(tr("HeaderMargin"), this);
	headerMarginText->setTextPixelSize(15);
	const auto headerMarginSlider = new CALSlider(this);
	headerMarginSlider->setRange(0, 200);
	headerMarginSlider->setValue(50);
	const auto headerMarginValueText = new CALText("5", this);
	headerMarginValueText->setTextPixelSize(15);
	connect(headerMarginSlider, &CALSlider::valueChanged, this, [=](const int value) {
		headerMarginValueText->setText(QString::number(value / 10));
		treeView->setHeaderMargin(value / 10);
	});
	const auto headerMarginHLayout = new QHBoxLayout;
	headerMarginHLayout->setContentsMargins(0, 0, 0, 0);
	headerMarginHLayout->addWidget(headerMarginText);
	headerMarginHLayout->addWidget(headerMarginSlider);
	headerMarginHLayout->addWidget(headerMarginValueText);
	// Indentation
	const auto indentationText = new CALText(tr("Indentation"), this);
	indentationText->setTextPixelSize(15);
	const auto indentationSlider = new CALSlider(this);
	indentationSlider->setRange(200, 1000);
	indentationSlider->setValue(200);
	const auto indentationValueText = new CALText("20", this);
	indentationValueText->setTextPixelSize(15);
	connect(indentationSlider, &CALSlider::valueChanged, this, [=](const int value) {
		indentationValueText->setText(QString::number(value / 10));
		treeView->setIndentation(value / 10);
	});
	const auto indentationHLayout = new QHBoxLayout;
	indentationHLayout->setContentsMargins(0, 0, 0, 0);
	indentationHLayout->addWidget(indentationText);
	indentationHLayout->addWidget(indentationSlider);
	indentationHLayout->addWidget(indentationValueText);
	// expandAll
	const auto expandCollapseHLayout = new QHBoxLayout;
	expandCollapseHLayout->setContentsMargins(0, 0, 0, 0);
	const auto expandButton = new CALPushButton(tr("Expand All"), this);
	expandButton->setFixedWidth(80);
	connect(expandButton, &CALPushButton::clicked, this, [=]() { treeView->expandAll(); });
	// collapseAll
	const auto collapseButton = new CALPushButton(tr("Collapse All"), this);
	collapseButton->setFixedWidth(80);
	connect(collapseButton, &CALPushButton::clicked, this, [=]() { treeView->collapseAll(); });
	expandCollapseHLayout->addWidget(expandButton);
	expandCollapseHLayout->addWidget(collapseButton);
	expandCollapseHLayout->addStretch();

	treeSettingVLayout->addWidget(dataText);
	treeSettingVLayout->addLayout(itemHeightHLayout);
	treeSettingVLayout->addLayout(headerMarginHLayout);
	treeSettingVLayout->addLayout(indentationHLayout);
	treeSettingVLayout->addLayout(expandCollapseHLayout);
	treeSettingVLayout->addStretch();

	const auto treeHLayout = new QHBoxLayout;
	treeHLayout->setContentsMargins(0, 0, 10, 0);
	treeHLayout->addWidget(treeSettingWidget);
	treeHLayout->addWidget(treeView);

	mainVLayout->addWidget(treeViewText);
	mainVLayout->addSpacing(10);
	mainVLayout->addLayout(treeHLayout);
}
