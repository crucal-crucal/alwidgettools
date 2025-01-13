#pragma once

#include <QListView>

#include "alwidgettoolsdef.hpp"

namespace AL {
class CALComboBoxView final : public QListView {
	Q_OBJECT

public:
	explicit CALComboBoxView(QWidget* parent = nullptr);
	~CALComboBoxView() override;

signals:
	void sigItemPressed(const QModelIndex& index);

protected:
	void mousePressEvent(QMouseEvent* event) override;
};
} // namespace AL