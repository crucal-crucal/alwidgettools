#pragma once

#include "e_basepage.hpp"

class E_MessageBar_Example final : public E_BasePage {
	Q_OBJECT

public:
	Q_INVOKABLE explicit E_MessageBar_Example(QWidget* parent = nullptr);
	~E_MessageBar_Example() override;
};
