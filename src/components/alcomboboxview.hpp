#pragma once

#include <QListView>

/**
 * @brief \namespace AL
 */
namespace AL {
class CALComboBoxView final : public QListView {
	Q_OBJECT

public:
	explicit CALComboBoxView(QWidget* parent = nullptr);
	~CALComboBoxView() override;

Q_SIGNALS:
	 void sigItemPressed(const QModelIndex& index);

protected:
	void mousePressEvent(QMouseEvent* event) override;
};
}
