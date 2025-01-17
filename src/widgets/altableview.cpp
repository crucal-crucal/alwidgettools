#include "altableview.hpp"

#include <QMouseEvent>

#include "alscrollbar.hpp"
#include "altableviewstyle.hpp"
#include "altableview_p.hpp"

/**
 * @brief \namespace AL
 */
namespace AL {
/**
 * @brief \class CALTableViewPrivate
 * Internal class for CALTableView
 * @param q pointer to the public class
 * @param parent pointer to the parent class
 */
CALTableViewPrivate::CALTableViewPrivate(CALTableView* q, QObject* parent): QObject(parent), q_ptr(q) {
}

CALTableViewPrivate::~CALTableViewPrivate() = default;

/**
 * @brief \class CALTableView
 * @param parent pointer to the parent class
 */
CALTableView::CALTableView(QWidget* parent): QTableView(parent), d_ptr(new CALTableViewPrivate(this, this)) {
	Q_D(CALTableView);
	d->style = new CALTableViewStyle(style());
	d->horizontalScrollBar = new CALScrollBar(this);
	d->verticalScrollBar = new CALScrollBar(this);

	setMouseTracking(true);
	setObjectName("CALTableView");
	setStyleSheet("QTableView { background-color: transparent; }"
		"QHeaderView { background-color: transparent; border: none; }");
	setShowGrid(false);
	setVerticalScrollBar(d->verticalScrollBar);
	setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
	setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
	setHorizontalScrollBar(d->horizontalScrollBar);
	setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
	setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);
	setStyle(d->style);
}

CALTableView::~CALTableView() = default;

void CALTableView::setHeaderMargin(const int margin) {
	if (margin >= 0) {
		d_func()->style->setHeaderMargin(margin);
		doItemsLayout();
		Q_EMIT sigHeaderMarginChanged();
	}
}

int CALTableView::getHeaderMargin() const {
	return d_func()->style->getHeaderMargin();
}

void CALTableView::showEvent(QShowEvent* event) {
	Q_EMIT sigTableViewShow();
	QTableView::showEvent(event);
}

void CALTableView::hideEvent(QHideEvent* event) {
	Q_EMIT sigTableViewHide();
	QTableView::hideEvent(event);
}

void CALTableView::mouseMoveEvent(QMouseEvent* event) {
	if (selectionBehavior() == QAbstractItemView::SelectRows) {
		d_func()->style->setCurrentHoverRow(indexAt(event->pos()).row());
		update();
	}

	QTableView::mouseMoveEvent(event);
}

void CALTableView::leaveEvent(QEvent* event) {
	if (selectionBehavior() == QAbstractItemView::SelectRows) {
		d_func()->style->setCurrentHoverRow(-1);
		update();
	}

	QTableView::leaveEvent(event);
}
}
