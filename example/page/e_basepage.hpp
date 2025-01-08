#pragma once

#include "alscrollpage.hpp"

class QVBoxLayout;

class E_BasePage : public CALScrollPage {
	Q_OBJECT

public:
	explicit E_BasePage(QWidget* parent = nullptr);
	~E_BasePage() override;

protected:
	void createCustomWidget(const QString& desText);
	QVBoxLayout* mainVLayout{ nullptr };
};
