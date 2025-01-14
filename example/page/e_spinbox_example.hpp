#pragma once

#include "e_basepage.hpp"

class E_SpinBox_Example final : public E_BasePage {
	Q_OBJECT

public:
	Q_INVOKABLE explicit E_SpinBox_Example(QWidget* parent = nullptr);
	~E_SpinBox_Example() override;
};
