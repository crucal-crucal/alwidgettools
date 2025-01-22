#pragma once

#include "e_basepage.hpp"

class E_TreeView final : public E_BasePage {
	Q_OBJECT

public:
	Q_INVOKABLE explicit E_TreeView(QWidget* parent = nullptr);
	~E_TreeView() override;

private:
	void initTreeViewArea();
};
