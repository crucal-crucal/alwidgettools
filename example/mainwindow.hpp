#pragma once

#include "almainwindow.hpp"

class CALContentDialog;

class E_BaseComponents;
class E_Settings;
class E_Card;
class E_Popup;

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
	E_Card* m_card{ nullptr };
	E_Popup* m_popup{ nullptr };
};
