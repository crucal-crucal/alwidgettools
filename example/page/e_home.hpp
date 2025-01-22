#pragma once

#include "e_basepage.hpp"

class E_Home final : public E_BasePage {
	Q_OBJECT

public:
	Q_INVOKABLE explicit E_Home(QWidget* parent = nullptr);
	~E_Home() override;

Q_SIGNALS:
	Q_SIGNAL void sigToggleSwitchCardClicked();
	Q_SIGNAL void sigSpinBoxCardClicked();
	Q_SIGNAL void sigSliderCardClicked();
	Q_SIGNAL void sigRadioButtonCardClicked();
	Q_SIGNAL void sigProgressBarCardClicked();
	Q_SIGNAL void sigPlainTextEditCardClicked();
	Q_SIGNAL void sigMutiSelectComboBoxCardClicked();
	Q_SIGNAL void sigMessageBarCardClicked();
	Q_SIGNAL void sigCircularProgressCardClicked();
	Q_SIGNAL void sigCheckBoxCardClicked();
	Q_SIGNAL void sigTabWidgetCardClicked();
	Q_SIGNAL void sigListViewCardClicked();
	Q_SIGNAL void sigTableViewCardClicked();
	Q_SIGNAL void sigTreeViewCardClicked();

private:
	void initTitleCardArea();
	void initPopularArea();
};
