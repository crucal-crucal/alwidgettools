#pragma once

#include "almainwindow.hpp"

/**
 * @brief \namespace AL
 */
namespace AL {
class CALContentDialog;
}

class E_About;
class E_Settings;
class E_Card;
class E_Popup;
class E_Icon;
class E_ToggleSwitch_Example;
class E_SpinBox_Example;
class E_Slider_Example;
class E_RadioButton_Example;
class E_ProgressBar_Example;
class E_PlainTextEdit_Example;
class E_MultiSelectCombobox_Example;
class E_MessageBar_Example;
class E_CircularProgress_Example;
class E_CheckBox_Example;
class E_Navigation;

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

	E_Settings* m_settings{ nullptr };
	E_Card* m_card{ nullptr };
	E_Popup* m_popup{ nullptr };
	E_About* m_about{ nullptr };
	E_Icon* m_icon{ nullptr };
	E_ToggleSwitch_Example* m_toggleSwitchExample{ nullptr };
	E_SpinBox_Example* m_spinBoxExample{ nullptr };
	E_Slider_Example* m_sliderExample{ nullptr };
	E_RadioButton_Example* m_radioButtonExample{ nullptr };
	E_ProgressBar_Example* m_progressBarExample{ nullptr };
	E_PlainTextEdit_Example* m_plainTextEditExample{ nullptr };
	E_MultiSelectCombobox_Example* m_multiSelectComboboxExample{ nullptr };
	E_MessageBar_Example* m_messageBarExample{ nullptr };
	E_CircularProgress_Example* m_circularProgressExample{ nullptr };
	E_CheckBox_Example* m_checkboxExample{ nullptr };
	E_Navigation* m_navigation{ nullptr };
};
