#pragma once

#include "alwidget.hpp"

class E_About final : public CALWidget {
	Q_OBJECT

public:
	explicit E_About(QWidget* parent = nullptr);
	~E_About() override;
};
