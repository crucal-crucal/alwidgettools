#include "uvbaselistview.hpp"

#include <QMouseEvent>

#include "uvscrollbar.hpp"

CUVBaseListView::CUVBaseListView(QWidget* parent): QListView(parent) {
	setObjectName("CUVBaseListView");
	setStyleSheet("#CUVBaseListView { background-color: transparent; border: 0px; }"
		"#CUVBaseListView::item { border: none; }");
	setAutoScroll(false);
	setFocusPolicy(Qt::NoFocus);
	setSelectionMode(QAbstractItemView::NoSelection);
	setMouseTracking(true);

	setVerticalScrollBar(new CUVScrollBar(this));
	setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
	setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);

	setHorizontalScrollBar(new CUVScrollBar(this));
	setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
	setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);
}

CUVBaseListView::~CUVBaseListView() = default;

void CUVBaseListView::mousePressEvent(QMouseEvent* event) {
	Q_EMIT sigMousePress(indexAt(event->pos()));

	QListView::mousePressEvent(event);
}

void CUVBaseListView::mouseReleaseEvent(QMouseEvent* event) {
	Q_EMIT sigMouseRelease(indexAt(event->pos()));

	QListView::mouseReleaseEvent(event);
}

void CUVBaseListView::mouseDoubleClickEvent(QMouseEvent* event) {
	Q_EMIT sigMouseDoubleClick(indexAt(event->pos()));

	QListView::mouseDoubleClickEvent(event);
}
