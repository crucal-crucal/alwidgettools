#include "uvcomboboxview.hpp"

#include <QMouseEvent>

/**
 * @brief \class CUVComboBoxView
 * @param parent pointer to the parent class
 */
CUVComboBoxView::CUVComboBoxView(QWidget* parent): QListView(parent) {
}

CUVComboBoxView::~CUVComboBoxView() = default;

void CUVComboBoxView::mousePressEvent(QMouseEvent* event) {
	QModelIndex index = indexAt(event->pos());
	if (index.isValid()) {
		emit sigItemPressed(index);
	}
	event->ignore();
}
