#include "mainwindow.hpp"

#include <QPushButton>

#include "uvcontentdialog.hpp"

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
}
