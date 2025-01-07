#include "alcomboboxview.hpp"

#include <QMouseEvent>

/**
 * @brief \class CALComboBoxView
 * @param parent pointer to the parent class
 */
CALComboBoxView::CALComboBoxView(QWidget* parent): QListView(parent) {
}

CALComboBoxView::~CALComboBoxView() = default;

void CALComboBoxView::mousePressEvent(QMouseEvent* event) {
	QModelIndex index = indexAt(event->pos());
	if (index.isValid()) {
		emit sigItemPressed(index);
	}
	event->ignore();
}
