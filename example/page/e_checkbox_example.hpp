#pragma once

#include "e_basepage.hpp"

class E_CheckBox_Example final : public E_BasePage {
	Q_OBJECT

public:
	Q_INVOKABLE explicit E_CheckBox_Example(QWidget* parent = nullptr);
	~E_CheckBox_Example() override;
};
