#pragma once

#include "uvmainwindow.hpp"

class CALContentDialog;

class E_BaseComponents;
class E_Settings;

class MainWindow : public CALMainWindow {
	Q_OBJECT

public:
	explicit MainWindow(QWidget* parent = nullptr);
	~MainWindow() override;

	void initWindow();
	void initEdgeLayout();
	void initContent();

private:
	CALContentDialog* m_closeDialog{ nullptr };

	E_BaseComponents* m_baseComponents{ nullptr };
	E_Settings* m_settings{ nullptr };
};
