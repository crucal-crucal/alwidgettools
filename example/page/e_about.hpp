#pragma once

#include "alwidget.hpp"

class QHBoxLayout;

class E_About final : public CALWidget {
	Q_OBJECT

public:
	explicit E_About(QWidget* parent = nullptr);
	~E_About() override;

private:
	QHBoxLayout* m_contentHLayout{ nullptr };

	void initImageCardArea();
	void initTextArea();
};
