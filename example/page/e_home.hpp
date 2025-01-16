#pragma once

#include "e_basepage.hpp"

class E_Home final : public E_BasePage {
	Q_OBJECT

public:
	Q_INVOKABLE explicit E_Home(QWidget *parent = nullptr);
	~E_Home() override;

private:
	void initTitleCardArea();
	void initPopularArea();
};
