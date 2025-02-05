#pragma once

#include "almainwindow.hpp"

class E_Home;
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
class E_TabWidget_Example;
class E_Navigation;
class E_ListView;
class E_TableView;
class E_TreeView;

class MainWindow final : public AL::CALMainWindow {
	Q_OBJECT

public:
	explicit MainWindow(QWidget* parent = nullptr);
	~MainWindow() override;

	void initWindow();
	void initEdgeLayout();
	void initContent();

protected:
	void showEvent(QShowEvent* event) override;

private:
	E_Home* m_home{ nullptr };
	E_Settings* m_settings{ nullptr };
	E_Card* m_card{ nullptr };
	E_Popup* m_popup{ nullptr };
	E_About* m_about{ nullptr };
	E_Icon* m_fluenticon{ nullptr };
	E_Icon* m_awesomeicon{ nullptr };
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
	E_TabWidget_Example* m_tabWidgetExample{ nullptr };
	E_Navigation* m_navigation{ nullptr };
	E_ListView* m_listView{ nullptr };
	E_TableView* m_tableView{ nullptr };
	E_TreeView* m_treeView{ nullptr };
};
