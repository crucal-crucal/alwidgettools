#pragma once

#include "e_basepage.hpp"

/**
 * @brief \namespace AL
 */
namespace AL {
class CALMainWindow;
}

class E_Settings final : public E_BasePage {
	Q_OBJECT

public:
	Q_INVOKABLE explicit E_Settings(QWidget* parent = nullptr);
	~E_Settings() override;

private:
	AL::CALMainWindow* m_mainWindow{ nullptr };

	void initThemeSwitchArea();
	void initMicaSwitchArea();
	void initNavigationDisplayModeArea();
};
