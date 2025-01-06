#pragma once

#include "e_basepage.hpp"

class QVBoxLayout;

class E_BaseComponents : public E_BasePage {
	Q_OBJECT

public:
	Q_INVOKABLE explicit E_BaseComponents(QWidget* parent = nullptr);
	~E_BaseComponents() override;

protected:
	void mouseReleaseEvent(QMouseEvent* event) override;

private:
	QVBoxLayout* m_mainVLayout{ nullptr };

	void initToggleSwitchArea();
	void initMessageBarArea();
	void initCircularProgressArea();
	void initMultiSelectComboBoxArea();
	void initSliderArea();
	void initProgressBarArea();
	void initCheckBoxArea();
};
