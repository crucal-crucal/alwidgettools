#pragma once

#include "e_basepage.hpp"

class QVBoxLayout;

class E_Card final : public E_BasePage {
	Q_OBJECT

public:
	Q_INVOKABLE explicit E_Card(QWidget* parent = nullptr);
	~E_Card() override;

private:
	QVBoxLayout* m_mainVLayout{ nullptr };

	void initLCDNumberArea();
};
