#include "mainwindow.hpp"

#include <QPushButton>

#include "uvcontentdialog.hpp"

#include "page/e_basecomponents.hpp"
#include "page/e_settings.hpp"

MainWindow::MainWindow(QWidget* parent): CUVMainWindow(parent) {
	initWindow();
	initEdgeLayout();
	initContent();
	m_closeDialog = new CUVContentDialog(this);
	m_closeDialog->setShowPolicys(UVDialogPolicy::ShowMask | UVDialogPolicy::EnableResize);
	connect(m_closeDialog, &CUVContentDialog::sigCloseButtonClicked, this, &CUVMainWindow::closeWindow);
	connect(this, &MainWindow::sigCloseButtonClicked, m_closeDialog, &CUVContentDialog::exec);
	this->setIsDefaultClosed(false);
	moveToCenter();
}

MainWindow::~MainWindow() = default;

void MainWindow::initWindow() {
	resize(1200, 700);
	setWindowTitle("uvwidgettools");
	setWindowIcon(QIcon(":/image/crucal.png"));
}

void MainWindow::initEdgeLayout() {
}

void MainWindow::initContent() {
	m_baseComponents = new E_BaseComponents(this);
	addPageNode(m_baseComponents->windowTitle(), m_baseComponents, UVIcon::CUVAweSomeIcon::CabinetFiling);

	m_settings = new E_Settings(this);
	QString settingKey{};
	addFooterNode(m_settings->windowTitle(), m_settings, settingKey, 0, UVIcon::CUVAweSomeIcon::GearComplex);
}
