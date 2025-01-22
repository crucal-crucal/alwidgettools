#include "mainwindow.hpp"

#include <QVBoxLayout>

#include "alcontentdialog.hpp"
#include "aldockwidget.hpp"
#include "almenu.hpp"
#include "almenubar.hpp"
#include "alstatusbar.hpp"
#include "altext.hpp"
#include "altoolbar.hpp"
#include "altoolbutton.hpp"
#include "e_about.hpp"
#include "e_card.hpp"
#include "e_checkbox_example.hpp"
#include "e_circularprogress_example.hpp"
#include "e_home.hpp"
#include "e_icon.hpp"
#include "e_listview.hpp"
#include "e_messagebar_example.hpp"
#include "e_multiselectcombobox_example.hpp"
#include "e_navigation.hpp"
#include "e_plaintextedit_example.hpp"
#include "e_popup.hpp"
#include "e_progressbar_example.hpp"
#include "e_radiobutton_example.hpp"
#include "e_settings.hpp"
#include "e_slider_example.hpp"
#include "e_spinbox_example.hpp"
#include "e_tableview.hpp"
#include "e_tabwidget_example.hpp"
#include "e_toggleswitch_example.hpp"
#include "e_treeview.hpp"

using namespace AL;

MainWindow::MainWindow(QWidget* parent): CALMainWindow(parent) {
	initWindow();
	initEdgeLayout();
	initContent();
	m_closeDialog = new CALContentDialog(this);
	m_closeDialog->setShowPolicys(ALDialogPolicy::ShowMask | ALDialogPolicy::EnableResize);
	connect(m_closeDialog, &CALContentDialog::sigCloseButtonClicked, this, &CALMainWindow::closeWindow);
	connect(this, &MainWindow::sigCloseButtonClicked, m_closeDialog, &CALContentDialog::exec);
	this->setIsDefaultClosed(false);
	moveToCenter();
}

MainWindow::~MainWindow() = default;

void MainWindow::initWindow() {
	resize(1200, 700);
	setWindowTitle("alwidgettools");
	setWindowIcon(QIcon(":alwidgettools/image/crucal.png"));
}

void MainWindow::initEdgeLayout() {
	/// menuBar
	const auto menuBar = new CALMenuBar(this);
	menuBar->setFixedHeight(30);
	const auto customWidget = new QWidget(this);
	const auto customWidgetVLayout = new QVBoxLayout(customWidget);
	customWidgetVLayout->setContentsMargins(0, 0, 0, 0);
	customWidgetVLayout->setAlignment(Qt::AlignVCenter);
	customWidgetVLayout->addWidget(menuBar);
	this->setCustomWidget(ALAppBarType::MiddleArea, customWidget);
	this->setCustomWidgetMaximumWidth(500);

	menuBar->addAweSomeIconAction(ALIcon::AweSomeIcon::AtomSimple, tr("action menu"));
	const auto iconMenu = menuBar->addMenu(ALIcon::AweSomeIcon::Aperture, tr("icon menu"));
	iconMenu->setMenuItemHeight(27);
	iconMenu->addAction(ALIcon::AweSomeIcon::BoxCheck, tr("sort"), QKeySequence::SelectAll);
	iconMenu->addAction(ALIcon::FluentIcon::Airplane, tr("Airplane"), QKeySequence::SelectAll);

	/// dockWidget
	const auto logDockWidget = new CALDockWidget("log msg", this);
	logDockWidget->setWidget(new CALText("log", logDockWidget));
	this->addDockWidget(Qt::RightDockWidgetArea, logDockWidget);
	resizeDocks({ logDockWidget }, { 200 }, Qt::Horizontal);
	/// toolBar
	const auto toolBar = new CALToolBar(tr("Toolbar"), this);
	toolBar->setAllowedAreas(Qt::TopToolBarArea | Qt::BottomToolBarArea);
	toolBar->setToolBarSpacing(3);
	toolBar->setToolButtonStyle(Qt::ToolButtonIconOnly);
	toolBar->setIconSize(QSize(25, 25));
	const auto toolbutton1 = new CALToolButton(this);
	toolbutton1->setAweSomeIcon(ALIcon::AweSomeIcon::BadgeCheck);
	toolBar->addWidget(toolbutton1);
	const auto toolbutton2 = new CALToolButton(this);
	toolbutton2->setFluentIcon(ALIcon::FluentIcon::Calculator);
	toolBar->addWidget(toolbutton2);
	toolBar->addSeparator();
	const auto toolbutton3 = new CALToolButton(this);
	toolbutton3->setFluentIcon(ALIcon::FluentIcon::Wifi);
	toolbutton3->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
	toolbutton3->setText(tr("Wifi"));
	toolBar->addWidget(toolbutton3);
	this->addToolBar(Qt::TopToolBarArea, toolBar);

	/// status bar
	const auto statusBar = new CALStatusBar(this);
	const auto textLabel = new CALText(tr("status bar example"), statusBar);
	textLabel->setTextPixelSize(14);
	statusBar->addWidget(textLabel);
	this->setStatusBar(statusBar);
}

void MainWindow::initContent() {
	/// pageNode
	// home
	m_home = new E_Home(this);
	addPageNode(m_home->windowTitle(), m_home, ALIcon::FluentIcon::Home);
	// card
	m_card = new E_Card(this);
	addPageNode(m_card->windowTitle(), m_card, ALIcon::FluentIcon::QuickNote);
	// popup
	m_popup = new E_Popup(this);
	addPageNode(m_popup->windowTitle(), m_popup, ALIcon::AweSomeIcon::WindowMaximize);
	// navigation
	m_navigation = new E_Navigation(this);
	addPageNode(m_navigation->windowTitle(), m_navigation, ALIcon::AweSomeIcon::LocationArrow);

	/// expanderNode
	// icon
	QString iconExpanderKey{};
	addExpanderNode("CALIcon", iconExpanderKey, ALIcon::AweSomeIcon::CabinetFiling);
	m_fluenticon = new E_Icon(QMetaEnum::fromType<ALIcon::FluentIcon>(), this);
	m_fluenticon->setWindowTitle("CALFluentIcon");
	m_fluenticon->init();
	addPageNode(m_fluenticon->windowTitle(), m_fluenticon, iconExpanderKey, 199, ALIcon::AweSomeIcon::FontCase);
	m_awesomeicon = new E_Icon(QMetaEnum::fromType<ALIcon::AweSomeIcon>(), this);
	m_awesomeicon->setWindowTitle("CALIcon");
	m_awesomeicon->init();
	addPageNode(m_awesomeicon->windowTitle(), m_awesomeicon, iconExpanderKey, 99, ALIcon::AweSomeIcon::FontCase);
	// baseComponents
	QString baseExpanderKey{};
	addExpanderNode("CALBaseComponents", baseExpanderKey, ALIcon::AweSomeIcon::CabinetFiling);
	m_toggleSwitchExample = new E_ToggleSwitch_Example(this);
	addPageNode(m_toggleSwitchExample->windowTitle(), m_toggleSwitchExample, baseExpanderKey, ALIcon::FluentIcon::BulletedListMirrored);
	m_spinBoxExample = new E_SpinBox_Example(this);
	addPageNode(m_spinBoxExample->windowTitle(), m_spinBoxExample, baseExpanderKey, ALIcon::FluentIcon::BulletedListMirrored);
	m_sliderExample = new E_Slider_Example(this);
	addPageNode(m_sliderExample->windowTitle(), m_sliderExample, baseExpanderKey, ALIcon::FluentIcon::BulletedListMirrored);
	m_radioButtonExample = new E_RadioButton_Example(this);
	addPageNode(m_radioButtonExample->windowTitle(), m_radioButtonExample, baseExpanderKey, ALIcon::FluentIcon::BulletedListMirrored);
	m_progressBarExample = new E_ProgressBar_Example(this);
	addPageNode(m_progressBarExample->windowTitle(), m_progressBarExample, baseExpanderKey, ALIcon::FluentIcon::BulletedListMirrored);
	m_plainTextEditExample = new E_PlainTextEdit_Example(this);
	addPageNode(m_plainTextEditExample->windowTitle(), m_plainTextEditExample, baseExpanderKey, ALIcon::FluentIcon::BulletedListMirrored);
	m_multiSelectComboboxExample = new E_MultiSelectCombobox_Example(this);
	addPageNode(m_multiSelectComboboxExample->windowTitle(), m_multiSelectComboboxExample, baseExpanderKey, ALIcon::FluentIcon::BulletedListMirrored);
	m_messageBarExample = new E_MessageBar_Example(this);
	addPageNode(m_messageBarExample->windowTitle(), m_messageBarExample, baseExpanderKey, ALIcon::FluentIcon::BulletedListMirrored);
	m_circularProgressExample = new E_CircularProgress_Example(this);
	addPageNode(m_circularProgressExample->windowTitle(), m_circularProgressExample, baseExpanderKey, ALIcon::FluentIcon::BulletedListMirrored);
	m_checkboxExample = new E_CheckBox_Example(this);
	addPageNode(m_checkboxExample->windowTitle(), m_checkboxExample, baseExpanderKey, ALIcon::FluentIcon::BulletedListMirrored);
	m_tabWidgetExample = new E_TabWidget_Example(this);
	addPageNode(m_tabWidgetExample->windowTitle(), m_tabWidgetExample, baseExpanderKey, ALIcon::FluentIcon::BulletedListMirrored);
	// view
	QString viewExpanderKey{};
	addExpanderNode("CALView", viewExpanderKey, ALIcon::FluentIcon::List);
	m_listView = new E_ListView(this);
	addPageNode(m_listView->windowTitle(), m_listView, viewExpanderKey, ALIcon::FluentIcon::BulletedList);
	m_tableView = new E_TableView(this);
	addPageNode(m_tableView->windowTitle(), m_tableView, viewExpanderKey, ALIcon::FluentIcon::Tablet);
	m_treeView = new E_TreeView(this);
	addPageNode(m_treeView->windowTitle(), m_treeView, viewExpanderKey, ALIcon::FluentIcon::TreeFolderFolder);
	/// fotterNode
	// about
	m_about = new E_About;
	QString aboutKey{};
	addFooterNode(m_about->windowTitle(), nullptr, aboutKey, 0, ALIcon::AweSomeIcon::User);
	m_about->hide();
	connect(this, &MainWindow::sigNavigationNodeClicked, this, [=](const ALNavigationType::NavigationNodeType& nodeType, const QString& nodeKey) {
		if (nodeKey == aboutKey) {
			m_about->resize(400, 400);
			m_about->show();
		}
	});
	// settings
	m_settings = new E_Settings(this);
	QString settingKey{};
	addFooterNode(m_settings->windowTitle(), m_settings, settingKey, 0, ALIcon::AweSomeIcon::GearComplex);
	// home conntection
	connect(m_home, &E_Home::sigToggleSwitchCardClicked, this, [=]() { this->navigation(m_toggleSwitchExample->property("CALPageKey").toString()); });
	connect(m_home, &E_Home::sigSpinBoxCardClicked, this, [=]() { this->navigation(m_spinBoxExample->property("CALPageKey").toString()); });
	connect(m_home, &E_Home::sigSliderCardClicked, this, [=]() { this->navigation(m_sliderExample->property("CALPageKey").toString()); });
	connect(m_home, &E_Home::sigRadioButtonCardClicked, this, [=]() { this->navigation(m_radioButtonExample->property("CALPageKey").toString()); });
	connect(m_home, &E_Home::sigProgressBarCardClicked, this, [=]() { this->navigation(m_progressBarExample->property("CALPageKey").toString()); });
	connect(m_home, &E_Home::sigPlainTextEditCardClicked, this, [=]() { this->navigation(m_plainTextEditExample->property("CALPageKey").toString()); });
	connect(m_home, &E_Home::sigMutiSelectComboBoxCardClicked, this, [=]() { this->navigation(m_multiSelectComboboxExample->property("CALPageKey").toString()); });
	connect(m_home, &E_Home::sigMessageBarCardClicked, this, [=]() { this->navigation(m_messageBarExample->property("CALPageKey").toString()); });
	connect(m_home, &E_Home::sigCircularProgressCardClicked, this, [=]() { this->navigation(m_circularProgressExample->property("CALPageKey").toString()); });
	connect(m_home, &E_Home::sigCheckBoxCardClicked, this, [=]() { this->navigation(m_checkboxExample->property("CALPageKey").toString()); });
	connect(m_home, &E_Home::sigTabWidgetCardClicked, this, [=]() { this->navigation(m_tabWidgetExample->property("CALPageKey").toString()); });
	connect(m_home, &E_Home::sigListViewCardClicked, this, [=]() { this->navigation(m_listView->property("CALPageKey").toString()); });
	connect(m_home, &E_Home::sigTableViewCardClicked, this, [=]() { this->navigation(m_tableView->property("CALPageKey").toString()); });
	connect(m_home, &E_Home::sigTreeViewCardClicked, this, [=]() { this->navigation(m_treeView->property("CALPageKey").toString()); });
}

void MainWindow::showEvent(QShowEvent* event) {
	CALMainWindow::showEvent(event);
	setThemeMode(ALThemeType::Dark);
}
