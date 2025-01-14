#include "mainwindow.hpp"

#include <QVBoxLayout>

#include "alcontentdialog.hpp"
#include "alawesometoolbutton.hpp"
#include "almenu.hpp"
#include "almenubar.hpp"
#include "alstatusbar.hpp"
#include "altext.hpp"
#include "altoolbar.hpp"

#include "e_about.hpp"
#include "e_card.hpp"
#include "e_icon.hpp"
#include "e_popup.hpp"
#include "e_settings.hpp"
#include "e_toggleswitch_example.hpp"
#include "e_slider_example.hpp"
#include "e_radiobutton_example.hpp"
#include "e_spinbox_example.hpp"
#include "e_progressbar_example.hpp"
#include "e_plaintextedit_example.hpp"
#include "e_multiselectcombobox_example.hpp"
#include "e_messagebar_example.hpp"
#include "e_circularprogress_example.hpp"
#include "e_checkbox_example.hpp"
#include "e_navigation.hpp"

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


	/// toolBar
	const auto toolBar = new CALToolBar(tr("Toolbar"), this);
	toolBar->setAllowedAreas(Qt::TopToolBarArea | Qt::BottomToolBarArea);
	toolBar->setToolBarSpacing(3);
	toolBar->setToolButtonStyle(Qt::ToolButtonIconOnly);
	toolBar->setIconSize(QSize(25, 25));
	const auto toolbutton1 = new CALAwesomeToolButton(this);
	toolbutton1->setAweSomeIcon(ALIcon::AweSomeIcon::BadgeCheck);
	toolBar->addWidget(toolbutton1);
	const auto toolbutton2 = new CALAwesomeToolButton(this);
	toolbutton2->setAweSomeIcon(ALIcon::AweSomeIcon::ChartUser);
	toolBar->addWidget(toolbutton2);
	toolBar->addSeparator();
	const auto toolbutton3 = new CALAwesomeToolButton(this);
	toolbutton3->setAweSomeIcon(ALIcon::AweSomeIcon::Bluetooth);
	toolbutton3->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
	toolbutton3->setText(tr("Bluetooth"));
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
	m_card = new E_Card(this);
	addPageNode(m_card->windowTitle(), m_card, ALIcon::AweSomeIcon::Cards);
	m_popup = new E_Popup(this);
	addPageNode(m_popup->windowTitle(), m_popup, ALIcon::AweSomeIcon::WindowMaximize);
	m_icon = new E_Icon(this);
	addPageNode(m_icon->windowTitle(), m_icon, 99, ALIcon::AweSomeIcon::FontCase);
	m_navigation = new E_Navigation(this);
	addPageNode(m_navigation->windowTitle(), m_navigation, ALIcon::AweSomeIcon::LocationArrow);

	/// expanderNode
	QString expanderKey{};
	addExpanderNode("CALBaseComponents", expanderKey, ALIcon::AweSomeIcon::CabinetFiling);
	m_toggleSwitchExample = new E_ToggleSwitch_Example(this);
	addPageNode(m_toggleSwitchExample->windowTitle(), m_toggleSwitchExample, expanderKey, ALIcon::AweSomeIcon::ListCheck);
	m_spinBoxExample = new E_SpinBox_Example(this);
	addPageNode(m_spinBoxExample->windowTitle(), m_spinBoxExample, expanderKey, ALIcon::AweSomeIcon::ListCheck);
	m_sliderExample = new E_Slider_Example(this);
	addPageNode(m_sliderExample->windowTitle(), m_sliderExample, expanderKey, ALIcon::AweSomeIcon::ListCheck);
	m_radioButtonExample = new E_RadioButton_Example(this);
	addPageNode(m_radioButtonExample->windowTitle(), m_radioButtonExample, expanderKey, ALIcon::AweSomeIcon::ListCheck);
	m_progressBarExample = new E_ProgressBar_Example(this);
	addPageNode(m_progressBarExample->windowTitle(), m_progressBarExample, expanderKey, ALIcon::AweSomeIcon::ListCheck);
	m_plainTextEditExample = new E_PlainTextEdit_Example(this);
	addPageNode(m_plainTextEditExample->windowTitle(), m_plainTextEditExample, expanderKey, ALIcon::AweSomeIcon::ListCheck);
	m_multiSelectComboboxExample = new E_MultiSelectCombobox_Example(this);
	addPageNode(m_multiSelectComboboxExample->windowTitle(), m_multiSelectComboboxExample, expanderKey, ALIcon::AweSomeIcon::ListCheck);
	m_messageBarExample = new E_MessageBar_Example(this);
	addPageNode(m_messageBarExample->windowTitle(), m_messageBarExample, expanderKey, ALIcon::AweSomeIcon::ListCheck);
	m_circularProgressExample = new E_CircularProgress_Example(this);
	addPageNode(m_circularProgressExample->windowTitle(), m_circularProgressExample, expanderKey, ALIcon::AweSomeIcon::ListCheck);
	m_checkboxExample = new E_CheckBox_Example(this);
	addPageNode(m_checkboxExample->windowTitle(), m_checkboxExample, expanderKey, ALIcon::AweSomeIcon::ListCheck);


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

	m_settings = new E_Settings(this);
	QString settingKey{};
	addFooterNode(m_settings->windowTitle(), m_settings, settingKey, 0, ALIcon::AweSomeIcon::GearComplex);
}
