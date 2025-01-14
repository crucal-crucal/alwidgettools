#include "albaselistview.hpp"

#include <QMouseEvent>

#include "alscrollbar.hpp"

/**
 * @brief \namespace AL
 */
namespace AL {
/**
 * @brief \class CALBaseListView
 * @param parent pointer to the parent class
 */
CALBaseListView::CALBaseListView(QWidget* parent): QListView(parent) {
	setObjectName("CALBaseListView");
	setStyleSheet("#CALBaseListView { background-color: transparent; border: 0px; }"
		"#CALBaseListView::item { border: none; }");
	setAutoScroll(false);
	setFocusPolicy(Qt::NoFocus);
	setSelectionMode(QAbstractItemView::NoSelection);
	setMouseTracking(true);

	setVerticalScrollBar(new CALScrollBar(this));
	setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
	setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);

	setHorizontalScrollBar(new CALScrollBar(this));
	setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
	setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);
}

CALBaseListView::~CALBaseListView() = default;

void CALBaseListView::mousePressEvent(QMouseEvent* event) {
	Q_EMIT sigMousePress(indexAt(event->pos()));

	QListView::mousePressEvent(event);
}

void CALBaseListView::mouseReleaseEvent(QMouseEvent* event) {
	Q_EMIT sigMouseRelease(indexAt(event->pos()));

	QListView::mouseReleaseEvent(event);
}

void CALBaseListView::mouseDoubleClickEvent(QMouseEvent* event) {
	Q_EMIT sigMouseDoubleClick(indexAt(event->pos()));

	QListView::mouseDoubleClickEvent(event);
}
}
