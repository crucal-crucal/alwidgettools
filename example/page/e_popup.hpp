#pragma once

#include "e_basepage.hpp"

class E_Popup final : public E_BasePage {
	Q_OBJECT

public:
	Q_INVOKABLE explicit E_Popup(QWidget* parent = nullptr);
	~E_Popup() override;

private:
	void initCalendarArea();
};
