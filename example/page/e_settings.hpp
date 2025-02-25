#pragma once

#include "e_basepage.hpp"

/**
 * @brief \namespace AL
 */
namespace AL {
class CALMainWindow;
class CALRadioButton;
}

class E_Settings final : public E_BasePage {
	Q_OBJECT

public:
	Q_INVOKABLE explicit E_Settings(QWidget* parent = nullptr);
	~E_Settings() override;

	void setNavigationBarDisPlayMode(const ALNavigationType::NavigationDisplayMode& mode) const;

private:
	AL::CALMainWindow* m_mainWindow{ nullptr };
	QMap<ALNavigationType::NavigationDisplayMode, AL::CALRadioButton*> mapNavigationDisplayModeWithRadioButton{};

	void initThemeSwitchArea();
	void initMicaSwitchArea();
	void initLogSwitchArea();
	void initNavigationDisplayModeArea();
	void initNavigationBarEnabledArea();
};
