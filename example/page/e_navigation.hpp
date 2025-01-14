#pragma once

#include "e_basepage.hpp"

class E_Navigation final : public E_BasePage {
	Q_OBJECT

public:
	Q_INVOKABLE explicit E_Navigation(QWidget* parent = nullptr);
	~E_Navigation() override;

private:
	void initBreadCrumbBarArea();
	void initPovitArea();
};
