#include "altreeview.hpp"

#include "altreeview_p.hpp"
#include "altreeviewstyle.hpp"
#include "alscrollbar.hpp"

/**
 * @brief \namespace AL
 */
namespace AL {
/**
 * @brief \class CALTreeViewPrivate
 * Internal class for CALTreeView
 * @param q pointer to the public class
 * @param parent pointer to the parent class
 */
CALTreeViewPrivate::CALTreeViewPrivate(CALTreeView* q, QObject* parent): QObject(parent), q_ptr(q) {
}

CALTreeViewPrivate::~CALTreeViewPrivate() = default;

/**
 * @brief \class CALTreeView
 * @param parent pointer to the partent class
 */
CALTreeView::CALTreeView(QWidget* parent): QTreeView(parent), d_ptr(new CALTreeViewPrivate(this, this)) {
	Q_D(CALTreeView);

	setObjectName("CALTreeView");
	setStyleSheet("#CALTreeView { background-color: transparent; }"
		"QHeaderView { background-color: transparent; border: none; }");
	setAnimated(true);
	setMouseTracking(true);

	d->horizontalScrollBar = new CALScrollBar(this);
	d->horizontalScrollBar->setIsAnimation(true);
	connect(d->horizontalScrollBar, &CALScrollBar::sigRangeAnimationFinished, this, [=]() { doItemsLayout(); });
	setHorizontalScrollBar(d->horizontalScrollBar);
	setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
	d->verticalScrollBar = new CALScrollBar(this);
	d->verticalScrollBar->setIsAnimation(true);
	connect(d->verticalScrollBar, &CALScrollBar::sigRangeAnimationFinished, this, [=]() { doItemsLayout(); });
	setVerticalScrollBar(d->verticalScrollBar);
	setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
	d->style = new CALTreeViewStyle(style());
	setStyle(d->style);
}

CALTreeView::~CALTreeView() = default;

void CALTreeView::setItemHeight(const int height) {
	if (height > 0) {
		d_func()->style->setItemHeight(height);
		doItemsLayout();
	}
}

int CALTreeView::getItemHeight() const {
	return d_func()->style->getItemHeight();
}

void CALTreeView::setHeaderMargin(const int margin) {
	if (margin >= 0) {
		d_func()->style->setHeaderMargin(margin);
		doItemsLayout();
	}
}

int CALTreeView::getHeaderMargin() const {
	return d_func()->style->getHeaderMargin();
}
}
