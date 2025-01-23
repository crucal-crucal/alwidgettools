#include "altabbar.hpp"

#include <QApplication>
#include <QDrag>
#include <QMimeData>
#include <QMouseEvent>

#include "altabbarstyle.hpp"
#include "altabbar_p.hpp"

/**
 * @brief \namespace AL
 */
namespace AL {
/**
 * @brief \class CALTabBarPrivate
 * Internal class for CALTabBar
 * @param q pointer to the public class
 * @param parent pointer to the parent class
 */
CALTabBarPrivate::CALTabBarPrivate(CALTabBar* q, QObject* parent): QObject(parent), q_ptr(q) {
}

CALTabBarPrivate::~CALTabBarPrivate() = default;

/**
 * @brief \class CALTabBar
 * @param parent pointer to the parent class
 */
CALTabBar::CALTabBar(QWidget* parent): QTabBar(parent), d_ptr(new CALTabBarPrivate(this, this)) {
	Q_D(CALTabBar);

	d->style = new CALTabBarStyle(style());
	setObjectName("CALTabBar");
	setStyleSheet("#CALTabBar { background-color: transparent; }");
	setTabsClosable(true);
	setMovable(true);
	setAcceptDrops(true);
	setStyle(d->style);
}

CALTabBar::~CALTabBar() = default;

void CALTabBar::mousePressEvent(QMouseEvent* event) {
	QTabBar::mousePressEvent(event);
	const int index = currentIndex();
	d_func()->lastDragPix = grab(tabRect(index));
	Q_EMIT sigTabBarPress(index);
}

void CALTabBar::mouseMoveEvent(QMouseEvent* event) {
	Q_D(CALTabBar);

	QRect moveRect = rect();
	moveRect.adjust(0, -height(), 0, height());
	if (const QPoint currentPos = event->pos(); currentPos.y() > moveRect.bottom() || currentPos.y() < moveRect.y()) {
		const auto drag = new QDrag(this);
		drag->setPixmap(d->lastDragPix);
		const qreal pixRatio = d->lastDragPix.devicePixelRatioF();
		const QPoint pixCenter = d->lastDragPix.rect().center();
		drag->setHotSpot(pixCenter / pixRatio);
		const auto data = new QMimeData;
		const int index = currentIndex();
		data->setProperty("DragType", "CALTabBarDrag");
		data->setProperty("TabIndex", index);
		data->setProperty("TabIcon", tabIcon(index));
		data->setProperty("TabText", tabText(index));
		data->setProperty("CALTabBarObject", QVariant::fromValue(this));
		data->setProperty("QDragObject", QVariant::fromValue(drag));
		drag->setMimeData(data);
		Q_EMIT sigTabDragCreate(drag);
	}

	QTabBar::mouseMoveEvent(event);
}

void CALTabBar::dragEnterEvent(QDragEnterEvent* event) {
	if (event->mimeData()->property("DragType").toString() == "CALTabBarDrag") {
		event->acceptProposedAction();
	}

	QTabBar::dragEnterEvent(event);
}

void CALTabBar::dropEvent(QDropEvent* event) {
	if (event->mimeData()->property("CALTabBarObject").value<CALTabBar*>() != this) {
		event->accept();
		const auto data = const_cast<QMimeData*>(event->mimeData());
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
		data->setProperty("TabDropIndex", tabAt(event->position().toPoint()));
#else
		data->setProperty("TabDropIndex", tabAt(event->pos()));
#endif
		Q_EMIT sigTabDragDrop(data);
	}

	QTabBar::dropEvent(event);
}
}
