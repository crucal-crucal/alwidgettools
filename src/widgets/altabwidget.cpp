#include "altabwidget.hpp"

#include <QApplication>
#include <QMimeData>
#include <QMouseEvent>
#include <QPainter>

#include "alcustomtabwidget.hpp"
#include "altabbar.hpp"
#include "altabwidget_p.hpp"

/**
 * @brief \namespace AL
 */
namespace AL {
CALTabWidgetPrivate::CALTabWidgetPrivate(CALTabWidget* q, QObject* parent): QObject(parent), q_ptr(q) {
}

CALTabWidgetPrivate::~CALTabWidgetPrivate() = default;

void CALTabWidgetPrivate::slotTabBarPress(const int index) {
}

void CALTabWidgetPrivate::slotTabDragCreate(QDrag* drag) {
	Q_Q(CALTabWidget);

	QMimeData* mimeData = drag->mimeData();
	mimeData->setProperty("CALTabWidgetObject", QVariant::fromValue(q));
	const int index = mimeData->property("TabIndex").toInt();
	QString tabText = q->tabText(index);
	QIcon tabIcon = q->tabIcon(index);
	QWidget* dragWidget = q->widget(index);
	QVariant originTabWidget = dragWidget->property("CALOriginTabWidget");
	if (!originTabWidget.isValid()) {
		dragWidget->setProperty("CALOriginTabWidget", QVariant::fromValue<CALTabWidget*>(q));
	}
	mimeData->setProperty("DragWidget", QVariant::fromValue(dragWidget));
	drag->setMimeData(mimeData);
	QMouseEvent releaseEvent(QEvent::MouseButtonRelease, QPoint(-1, -1), QPoint(-1, -1), Qt::LeftButton, Qt::LeftButton, Qt::NoModifier);
	QApplication::sendEvent(mimeData->property("CALTabBarObject").value<CALTabBar*>(), &releaseEvent);
	if (drag->exec() == Qt::IgnoreAction) {
		/// new widget
		const auto originCustomTabBar = mimeData->property("CALTabBarObject").value<CALTabBar*>();
		if (originCustomTabBar && originCustomTabBar->objectName() == "CALCustomTabBar") {
			originCustomTabBar->removeTab(index);
		}
		const auto t_originTabWidget = dragWidget->property("CALOriginTabWidget").value<CALTabWidget*>();
		const auto floatWidget = new CALCustomTabWidget(t_originTabWidget);
		CALTabBar* customTabBar = floatWidget->getCustomTabBar();
		dragWidget->setProperty("CurrentCustomBar", QVariant::fromValue<CALTabBar*>(customTabBar));
		CALTabWidget* customTabWidget = floatWidget->getCustomTabWidget();
		customTabWidget->d_func()->customTabBar = customTabBar;
		connect(customTabBar, &CALTabBar::sigTabBarPress, customTabWidget->d_func(), &CALTabWidgetPrivate::slotTabBarPress);
		connect(customTabBar, &CALTabBar::sigTabDragCreate, customTabWidget->d_func(), &CALTabWidgetPrivate::slotTabDragCreate);
		connect(customTabBar, &CALTabBar::sigTabDragDrop, customTabWidget->d_func(), &CALTabWidgetPrivate::slotTabDragDrop);
		floatWidget->addTab(dragWidget, tabIcon, tabText);
		floatWidget->show();
		QPoint cursorPoint = QCursor::pos();
		floatWidget->move(cursorPoint.x() - floatWidget->width() / 2, cursorPoint.y() - 40);
	}
}

void CALTabWidgetPrivate::slotTabDragDrop(const QMimeData* mimeData) {
}

void CALTabWidgetPrivate::slotTabCloseRequested(int index) {
}

/**
 * @brief \class CALTabWidget
 * @param parent pointer to the parent class
 */
CALTabWidget::CALTabWidget(QWidget* parent): QTabWidget(parent), d_ptr(new CALTabWidgetPrivate(this, this)) {
	Q_D(CALTabWidget);

	d->isTransparent = false;
	setObjectName("CALTabWidget");
	setAcceptDrops(true);
	const auto tabBar = new CALTabBar(this);
	setTabBar(tabBar);
	connect(tabBar, &CALTabBar::sigTabBarPress, d, &CALTabWidgetPrivate::slotTabBarPress);
	connect(tabBar, &CALTabBar::sigTabDragCreate, d, &CALTabWidgetPrivate::slotTabDragCreate);
	connect(tabBar, &CALTabBar::sigTabDragDrop, d, &CALTabWidgetPrivate::slotTabDragDrop);
	connect(tabBar, &CALTabBar::tabCloseRequested, d, &CALTabWidgetPrivate::slotTabCloseRequested);
}

CALTabWidget::~CALTabWidget() = default;

void CALTabWidget::setTabPosition(const TabPosition& position) {
	if (position == QTabWidget::North || position == QTabWidget::South) {
		QTabWidget::setTabPosition(position);
	}
}

void CALTabWidget::setIsTransparent(const bool isTransparent) {
	d_func()->isTransparent = isTransparent;
	Q_EMIT sigIsTransparentChanged();
}

bool CALTabWidget::getIsTransparent() const {
	return d_func()->isTransparent;
}

void CALTabWidget::paintEvent(QPaintEvent* event) {
	if (!d_func()->isTransparent) {
		QTabWidget::paintEvent(event);
	}
}

void CALTabWidget::dragEnterEvent(QDragEnterEvent* event) {
	if (event->mimeData()->property("Dragtype").toString() == "CALTabBarDrag") {
		event->acceptProposedAction();
	}

	QTabWidget::dragEnterEvent(event);
}

void CALTabWidget::dropEvent(QDropEvent* event) {
	if (event->mimeData()->property("CALTabWidgetObject").value<CALTabWidget*>() != this) {
		const auto data = const_cast<QMimeData*>(event->mimeData());
		data->setProperty("TabDropIndex", count());
		d_func()->slotTabDragDrop(event->mimeData());
		event->accept();
	}

	QTabWidget::dropEvent(event);
}
}
