#pragma once

#include "e_basepage.hpp"

class E_Slider_Example final : public E_BasePage {
	Q_OBJECT

public:
	Q_INVOKABLE explicit E_Slider_Example(QWidget* parent = nullptr);
	~E_Slider_Example() override;
};
