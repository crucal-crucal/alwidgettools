#pragma once

#include "almainwindow.hpp"

namespace AL {
class CALContentDialog;
}

class E_About;
class E_BaseComponents;
class E_Settings;
class E_Card;
class E_Popup;
class E_Icon;

class MainWindow final : public AL::CALMainWindow {
	Q_OBJECT

public:
	explicit MainWindow(QWidget* parent = nullptr);
	~MainWindow() override;

	void initWindow();
	void initEdgeLayout();
	void initContent();

private:
	AL::CALContentDialog* m_closeDialog{ nullptr };

	E_BaseComponents* m_baseComponents{ nullptr };
	E_Settings* m_settings{ nullptr };
	E_Card* m_card{ nullptr };
	E_Popup* m_popup{ nullptr };
	E_About* m_about{ nullptr };
	E_Icon* m_icon{ nullptr };
};
