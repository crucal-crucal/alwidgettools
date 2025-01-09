#include "mainwindow.hpp"

#include <QVBoxLayout>

#include "alawesometoolbutton.hpp"
#include "alcontentdialog.hpp"
#include "almenu.hpp"
#include "almenubar.hpp"
#include "altoolbar.hpp"

#include "e_about.hpp"
#include "e_basecomponents.hpp"
#include "e_card.hpp"
#include "e_icon.hpp"
#include "e_popup.hpp"
#include "e_settings.hpp"

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
	setWindowIcon(QIcon(":/image/crucal.png"));
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

	menuBar->addAweSomeIconAction(ALIcon::AweSomeIcon::AtomSimple, "action menu");
	const auto iconMenu = menuBar->addMenu(ALIcon::AweSomeIcon::Aperture, "icon menu");
	iconMenu->setMenuItemHeight(27);
	iconMenu->addAction(ALIcon::AweSomeIcon::BoxCheck, "sort", QKeySequence::SelectAll);


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
	toolbutton3->setText("Bluetooth");
	toolBar->addWidget(toolbutton3);
	this->addToolBar(Qt::TopToolBarArea, toolBar);
}

void MainWindow::initContent() {
	m_baseComponents = new E_BaseComponents(this);
	addPageNode(m_baseComponents->windowTitle(), m_baseComponents, ALIcon::AweSomeIcon::CabinetFiling);
	m_card = new E_Card(this);
	addPageNode(m_card->windowTitle(), m_card, ALIcon::AweSomeIcon::Cards);
	m_popup = new E_Popup(this);
	addPageNode(m_popup->windowTitle(), m_popup, ALIcon::AweSomeIcon::WindowMaximize);
	m_icon = new E_Icon(this);
	addPageNode(m_icon->windowTitle(), m_icon, 99, ALIcon::AweSomeIcon::FontCase);

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
