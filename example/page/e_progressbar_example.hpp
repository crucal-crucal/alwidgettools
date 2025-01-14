#pragma once

#include "e_basepage.hpp"

class E_ProgressBar_Example final : public E_BasePage {
	Q_OBJECT

public:
	Q_INVOKABLE explicit E_ProgressBar_Example(QWidget* parent = nullptr);
	~E_ProgressBar_Example() override;
};
