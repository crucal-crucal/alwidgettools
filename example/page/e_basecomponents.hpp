#pragma once

#include "e_basepage.hpp"

class E_BaseComponents final : public E_BasePage {
	Q_OBJECT

public:
	Q_INVOKABLE explicit E_BaseComponents(QWidget* parent = nullptr);
	~E_BaseComponents() override;

protected:
	void mouseReleaseEvent(QMouseEvent* event) override;

private:
	void initToggleSwitchArea();
	void initMessageBarArea();
	void initCircularProgressArea();
	void initMultiSelectComboBoxArea();
	void initSliderArea();
	void initProgressBarArea();
	void initCheckBoxArea();
	void initRadioButtonArea();
	void initSpinBoxArea();
	void initDoubleSpinBoxArea();
};
