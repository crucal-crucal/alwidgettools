#pragma once

#include "e_basepage.hpp"

class E_BaseComponents : public E_BasePage {
	Q_OBJECT

public:
	Q_INVOKABLE explicit E_BaseComponents(QWidget* parent = nullptr);
	~E_BaseComponents() override;

protected:
	void mouseReleaseEvent(QMouseEvent* event) override;
};
