#include "mainwindow.hpp"

#include <QPushButton>

#include "alcontentdialog.hpp"

#include "page/e_basecomponents.hpp"
#include "page/e_card.hpp"
#include "page/e_popup.hpp"
#include "page/e_settings.hpp"

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
}

void MainWindow::initContent() {
	m_baseComponents = new E_BaseComponents(this);
	addPageNode(m_baseComponents->windowTitle(), m_baseComponents, ALIcon::AweSomeIcon::CabinetFiling);
	m_card = new E_Card(this);
	addPageNode(m_card->windowTitle(), m_card, ALIcon::AweSomeIcon::Cards);
	m_popup = new E_Popup(this);
	addPageNode(m_popup->windowTitle(), m_popup, ALIcon::AweSomeIcon::WindowMaximize);

	m_settings = new E_Settings(this);
	QString settingKey{};
	addFooterNode(m_settings->windowTitle(), m_settings, settingKey, 0, ALIcon::AweSomeIcon::GearComplex);
}
