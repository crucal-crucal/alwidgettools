#pragma once

#include "e_basepage.hpp"

class E_ToggleSwitch_Example final : public E_BasePage {
	Q_OBJECT

public:
	Q_INVOKABLE explicit E_ToggleSwitch_Example(QWidget* parent = nullptr);
	~E_ToggleSwitch_Example() override;
};
