#pragma once

#include "e_basepage.hpp"

class E_MultiSelectCombobox_Example : public E_BasePage {
	Q_OBJECT

public:
	Q_INVOKABLE explicit E_MultiSelectCombobox_Example(QWidget* parent = nullptr);
	~E_MultiSelectCombobox_Example() override;
};
