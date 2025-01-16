#include "alflowlayout.hpp"

#include <QPropertyAnimation>
#include <QWidget>

#include "alflowlayout_p.hpp"

/**
 * @brief \namespace AL
 */
namespace AL {
/**
 * class CALFlowLayoutPrivate
 * Internal class for CALFlowLayout
 * @param parent pointer to the parent class
 * @param q pointer to the public class
 */
CALFlowLayoutPrivate::CALFlowLayoutPrivate(QObject* parent, CALFlowLayout* q): QObject(parent), q_ptr(q) {
}

CALFlowLayoutPrivate::~CALFlowLayoutPrivate() = default;

int CALFlowLayoutPrivate::doLayout(const QRect& rect, const bool testOnly) const {
	Q_Q(const CALFlowLayout);
	int left, top, right, bottom;
	q->getContentsMargins(&left, &top, &right, &bottom);
	const QRect effectiveRect = rect.adjusted(+left, +top, -right, -bottom);
	int x = effectiveRect.x();
	int y = effectiveRect.y();
	int lineHeight = 0;

	for (QLayoutItem* item : std::as_const(itemList)) {
		const QWidget* widget = item->widget();
		int spaceX = q->horizontalSpacing();
		if (spaceX == -1) {
			spaceX = widget->style()->layoutSpacing(QSizePolicy::PushButton, QSizePolicy::PushButton, Qt::Horizontal);
		}
		int spaceY = q->verticalSpacing();
		if (spaceY == -1) {
			spaceY = widget->style()->layoutSpacing(QSizePolicy::PushButton, QSizePolicy::PushButton, Qt::Vertical);
		}

		int nextX = x + item->sizeHint().width() + spaceX;
		if (nextX - spaceX > effectiveRect.right() && lineHeight > 0) {
			x = effectiveRect.x();
			y += lineHeight + spaceY;
			nextX = x + item->sizeHint().width() + spaceX;
			lineHeight = 0;
		}
		if (!lastGeometryMap.contains(item)) {
			lastGeometryMap.insert(item, QPoint(x, y));
		}
		if (!testOnly) {
			if (item->geometry().x() == 0 && item->geometry().y() == 0) {
				item->setGeometry(QRect(QPoint(x, y), item->sizeHint()));
			} else if (item->geometry().x() != x || item->geometry().y() != y) {
				if (isAnimation) {
					if (lastGeometryMap[item] == QPoint(x, y)) {
						lineHeight = qMax(lineHeight, item->sizeHint().height());
						return y + lineHeight - rect.y() + bottom;
					}
					const auto geometryAnimation = new QPropertyAnimation(item->widget(), "geometry");
					geometryAnimation->setStartValue(item->widget()->geometry());
					geometryAnimation->setEndValue(QRect(QPoint(x, y), item->sizeHint()));
					geometryAnimation->setDuration(200);
					geometryAnimation->setEasingCurve(QEasingCurve::InOutSine);
					geometryAnimation->start(QAbstractAnimation::DeleteWhenStopped);
					lastGeometryMap[item] = QPoint(x, y);
				} else {
					item->setGeometry((QRect(QPoint(x, y), item->sizeHint())));
				}
			}
		}
		x = nextX;
		lineHeight = qMax(lineHeight, item->sizeHint().height());
	}

	return y + lineHeight - rect.y() + bottom;
}

int CALFlowLayoutPrivate::smartSpacing(const QStyle::PixelMetric pm) const {
	Q_Q(const CALFlowLayout);

	if (QObject* parent = q->parent(); !parent) {
		return -1;
	} else if (parent->isWidgetType()) {
		const auto pw = qobject_cast<QWidget*>(parent);
		return pw->style()->pixelMetric(pm, nullptr, pw);
	} else {
		return qobject_cast<QLayout*>(parent)->spacing();
	}
}


/**
 * class CALFlowLayout
 * @param parent pointer to the parent class
 * @param margin 边距
 * @param hSpacing 水平间距
 * @param vSpacing 垂直间距
 */
CALFlowLayout::CALFlowLayout(QWidget* parent, const int margin, const int hSpacing, const int vSpacing): QLayout(parent), d_ptr(new CALFlowLayoutPrivate(this, this)) {
	Q_D(CALFlowLayout);

	d->hSpacing = hSpacing;
	d->vSpacing = vSpacing;
	d->lastHeightMap.insert(this, 0);
	setMargin(margin);
}

CALFlowLayout::CALFlowLayout(int margin, int hSpacing, int vSpacing): d_ptr(new CALFlowLayoutPrivate(this, this)) {
	Q_D(CALFlowLayout);

	d->hSpacing = hSpacing;
	d->vSpacing = vSpacing;
	d->lastHeightMap.insert(this, 0);
	setMargin(margin);
}

CALFlowLayout::~CALFlowLayout() {
	QLayoutItem* item;
	while ((item = CALFlowLayout::takeAt(0))) {
		delete item;
	}
}

void CALFlowLayout::addItem(QLayoutItem* item) {
	d_func()->itemList.append(item);
}

int CALFlowLayout::horizontalSpacing() const {
	return d_func()->hSpacing >= 0 ? d_func()->hSpacing : d_func()->smartSpacing(QStyle::PM_LayoutHorizontalSpacing);
}

int CALFlowLayout::verticalSpacing() const {
	return d_func()->vSpacing >= 0 ? d_func()->vSpacing : d_func()->smartSpacing(QStyle::PM_LayoutVerticalSpacing);
}

Qt::Orientations CALFlowLayout::expandingDirections() const {
	return {};
}

bool CALFlowLayout::hasHeightForWidth() const {
	return true;
}

int CALFlowLayout::heightForWidth(const int width) const {
	Q_D(const CALFlowLayout);

	const int height = d->doLayout(QRect(0, 0, width, 0), true);
	d->lastHeightMap[const_cast<CALFlowLayout*>(this)] = height;
	return height;
}

int CALFlowLayout::count() const {
	return d_func()->itemList.size();
}

QLayoutItem* CALFlowLayout::itemAt(const int index) const {
	return d_func()->itemList.value(index);
}

QSize CALFlowLayout::minimumSize() const {
	QSize size{};

	for (const QLayoutItem* item : std::as_const(d_func()->itemList)) {
		size = size.expandedTo(item->minimumSize());
	}

	const QMargins margins = this->contentsMargins();
	size += QSize(margins.left() + margins.right(), margins.top() + margins.bottom());
	return size;
}

void CALFlowLayout::setGeometry(const QRect& rect) {
	QLayout::setGeometry(rect);
	[[maybe_unused]] int height = d_func()->doLayout(rect, false);
}

QSize CALFlowLayout::sizeHint() const {
	return minimumSize();
}

QLayoutItem* CALFlowLayout::takeAt(const int index) {
	Q_D(CALFlowLayout);

	if (index >= 0 && index < d->itemList.size()) {
		return d->itemList.takeAt(index);
	}

	return nullptr;
}

void CALFlowLayout::setIsAnimation(const bool isAnimation) {
	d_func()->isAnimation = isAnimation;
}
}
