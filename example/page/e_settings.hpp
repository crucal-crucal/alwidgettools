#pragma once

#include "e_basepage.hpp"

class QVBoxLayout;
class CALMainWindow;

class E_Settings final : public E_BasePage {
	Q_OBJECT

public:
	Q_INVOKABLE explicit E_Settings(QWidget* parent = nullptr);
	~E_Settings() override;

private:
	QVBoxLayout* m_mainVLayout{ nullptr };
	CALMainWindow* m_mainWindow{ nullptr };

	void initThemeSwitchArea();
	void initMicaSwitchArea();
	void initNavigationDisplayModeArea();
};
