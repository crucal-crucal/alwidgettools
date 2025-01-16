#pragma once

#include "e_basepage.hpp"

class E_TabWidget_Example final : public E_BasePage {
	Q_OBJECT

public:
	Q_INVOKABLE explicit E_TabWidget_Example(QWidget* parent = nullptr);
	~E_TabWidget_Example() override;
};
