#pragma once

#include "e_basepage.hpp"

class E_ListView final : public E_BasePage {
	Q_OBJECT

public:
	Q_INVOKABLE explicit E_ListView(QWidget* parent = nullptr);
	~E_ListView() override;

private:
	void initListViewArea();
};
