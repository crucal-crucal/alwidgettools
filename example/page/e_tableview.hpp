#pragma once

#include "e_basepage.hpp"

class E_TableView final : public E_BasePage {
	Q_OBJECT

public:
	Q_INVOKABLE explicit E_TableView(QWidget* parent = nullptr);
	~E_TableView() override;

private:
	void initTableViewArea();
};
