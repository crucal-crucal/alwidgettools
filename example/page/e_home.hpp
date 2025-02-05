#pragma once

#include "e_basepage.hpp"

class E_Home final : public E_BasePage {
	Q_OBJECT

public:
	Q_INVOKABLE explicit E_Home(QWidget* parent = nullptr);
	~E_Home() override;

Q_SIGNALS:
	 void sigToggleSwitchCardClicked();
	 void sigSpinBoxCardClicked();
	 void sigSliderCardClicked();
	 void sigRadioButtonCardClicked();
	 void sigProgressBarCardClicked();
	 void sigPlainTextEditCardClicked();
	 void sigMutiSelectComboBoxCardClicked();
	 void sigMessageBarCardClicked();
	 void sigCircularProgressCardClicked();
	 void sigCheckBoxCardClicked();
	 void sigTabWidgetCardClicked();
	 void sigListViewCardClicked();
	 void sigTableViewCardClicked();
	 void sigTreeViewCardClicked();

private:
	void initTitleCardArea();
	void initPopularArea();
};
