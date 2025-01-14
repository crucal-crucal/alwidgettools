#include "allistview.hpp"

#include "allistviewstyle.hpp"
#include "allistview_p.hpp"
#include "alscrollbar.hpp"

/**
 * @brief \namespace AL
 */
namespace AL {
/**
 * \class CALListViewPrivate
 * Internal class for CALListView
 * @param q pointer to the public class
 * @param parent pointer to the parent class
 */
CALListViewPrivate::CALListViewPrivate(CALListView* q, QObject* parent): QObject(parent), q_ptr(q) {
}

CALListViewPrivate::~CALListViewPrivate() = default;

/**
 * \class CALListView
 * @param parent pointer to the parent class
 */
CALListView::CALListView(QWidget* parent): QListView(parent), d_ptr(new CALListViewPrivate(this, this)) {
	Q_D(CALListView);

	d->style = new CALListViewStyle(style());
	d->horizontalScrollBar = new CALScrollBar(this);
	d->verticalScrollBar = new CALScrollBar(this);

	setObjectName("CALListView");
	setStyleSheet("#CALListView { background-color: transparent; }");
	setStyle(d->style);
	setMouseTracking(true);
	setHorizontalScrollBar(d->horizontalScrollBar);
	setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
	setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);
	setVerticalScrollBar(d->verticalScrollBar);
	setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
	setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
}

CALListView::~CALListView() = default;

void CALListView::setItemHeight(const int itemHeight) {
	d_func()->style->setItemHeight(itemHeight);
}

int CALListView::getItemHeight() const {
	return d_func()->style->getItemHeight();
}

void CALListView::setIsTransparent(const bool isTransparent) {
	d_func()->style->setIsTransparent(isTransparent);
}

bool CALListView::getIsTransparent() const {
	return d_func()->style->getIsTransparent();
}
}
