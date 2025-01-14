#pragma once

#include "e_basepage.hpp"

class E_CircularProgress_Example final : public E_BasePage {
	Q_OBJECT

public:
	Q_INVOKABLE explicit E_CircularProgress_Example(QWidget* parent = nullptr);
	~E_CircularProgress_Example() override;
};
