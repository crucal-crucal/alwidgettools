#pragma once

#include "e_basepage.hpp"

class E_RadioButton_Example final : public E_BasePage {
	Q_OBJECT

public:
	Q_INVOKABLE explicit E_RadioButton_Example(QWidget* parent = nullptr);
	~E_RadioButton_Example() override;
};
