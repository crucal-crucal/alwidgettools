#pragma once

#include "uvscrollpage.hpp"

class E_BasePage : public CUVScrollPage {
	Q_OBJECT

public:
	explicit E_BasePage(QWidget* parent = nullptr);
	~E_BasePage() override;

protected:
	void createCustomWidget(const QString& desText);
};
