#pragma once

#include "uvmainwindow.hpp"

class CUVContentDialog;

class MainWindow : public CUVMainWindow {
	Q_OBJECT

public:
	explicit MainWindow(QWidget* parent = nullptr);
	~MainWindow() override;

	void initWindow();
	void initEdgeLayout();
	void initContent();

private:
	CUVContentDialog* m_closeDialog{ nullptr };
};
