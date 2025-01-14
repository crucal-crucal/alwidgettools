#include "alscrollpage.hpp"

#include <QDebug>
#include <QHBoxLayout>
#include <QPropertyAnimation>
#include <QStackedWidget>
#include <QVariant>

#include "albreadcrumbbar.hpp"
#include "alnavigationrouter.hpp"
#include "alscrollarea.hpp"
#include "alscrollbar.hpp"
#include "alscrollpage_p.hpp"

/**
 * @brief \namespace AL
 */
namespace AL {
/**
 * @brief \class CALScrollPagePrivate
 * Internal class for CALScrollPage
 * @param q pointer to the public class
 * @param parent pointer to the parent class
 */
CALScrollPagePrivate::CALScrollPagePrivate(CALScrollPage* q, QObject* parent): QObject(parent), q_ptr(q) {
	navigationTargetIndex = 0;
	pageTitleSpacing = 0;
	isGrabGesture = false;
}

CALScrollPagePrivate::~CALScrollPagePrivate() = default;

void CALScrollPagePrivate::invokableNavigationRouteBack(const QVariantMap& routeData) {
	// 面包屑
	Q_Q(CALScrollPage);

	if (const QString pageCheckSumKey = routeData.value("CALScrollPageCheckSumKey").toString(); pageCheckSumKey == "Navigation") {
		const QString pageTitle = routeData.value("CALPageTitle").toString();
		q->navigation(mapCentralWidget.value(pageTitle), false);
	} else if (pageCheckSumKey == "sigBreadcrumbClicked") {
		const QStringList lastBreadcrumbList = routeData.value("LastBreadcrumbList").toStringList();
		const int widgetIndex = mapCentralWidget.value(lastBreadcrumbList.last());
		switchCentralStackIndex(widgetIndex, navigationTargetIndex);
		navigationTargetIndex = widgetIndex;
		breadcrumbBar->setBreadcrumbList(lastBreadcrumbList);
	}
}

void CALScrollPagePrivate::switchCentralStackIndex(const int targetIndex, const int lastIndex) {
	QWidget* lastWidget = centralStackedWidget->widget(lastIndex);
	QWidget* targetWidget = centralStackedWidget->widget(targetIndex);
	targetWidget->resize(lastWidget->size());
	targetWidget->setVisible(true);

	const auto lastWidgetAnimation = new QPropertyAnimation(lastWidget, "pos");
	lastWidgetAnimation->setEasingCurve(QEasingCurve::InQuart);
	lastWidgetAnimation->setDuration(280);

	const auto targetWidgetAnimation = new QPropertyAnimation(targetWidget, "pos");
	connect(targetWidgetAnimation, &QPropertyAnimation::finished, this, [=]() { centralStackedWidget->setCurrentIndex(targetIndex); });
	targetWidgetAnimation->setEasingCurve(QEasingCurve::InQuart);
	targetWidgetAnimation->setDuration(280);

	// 左滑 & 右滑
	const bool isLeftSlide = targetIndex > lastIndex;
	lastWidgetAnimation->setStartValue(lastWidget->pos());
	lastWidgetAnimation->setEndValue(QPoint(isLeftSlide ? -centralStackedWidget->width() : centralStackedWidget->width(), 0));
	targetWidgetAnimation->setStartValue(QPoint(isLeftSlide ? centralStackedWidget->width() : -centralStackedWidget->width(), 0));
	targetWidgetAnimation->setEndValue(QPoint(0, 0));
	lastWidgetAnimation->start(QAbstractAnimation::DeleteWhenStopped);
	targetWidgetAnimation->start(QAbstractAnimation::DeleteWhenStopped);
}

/**
 * @brief \class CALScrollPage
 * @param parent pointer to the parent class
 */
CALScrollPage::CALScrollPage(QWidget* parent): QWidget(parent), d_ptr(new CALScrollPagePrivate(this, this)) {
	Q_D(CALScrollPage);
	setProperty("CALBaseClassName", "CALScrollPage");
	d->breadcrumbBar = new CALBreadcrumbBar(this);
	d->breadcrumbBar->setTextPixelSize(28);
	connect(d->breadcrumbBar, &CALBreadcrumbBar::sigBreadcrumbClicked, this, [=](const QString& breadcrumb, const QStringList& lastBreadcrumbList) {
		if (d->mapCentralWidget.contains(breadcrumb)) {
			const int widgetIndex = d->mapCentralWidget.value(breadcrumb);
			d->switchCentralStackIndex(widgetIndex, d->navigationTargetIndex);
			d->navigationTargetIndex = widgetIndex;
			QVariantMap routeData{};
			routeData.insert("CALScrollPageCheckSumKey", "sigBreadcrumbClicked");
			routeData.insert("LastBreadcrumbList", lastBreadcrumbList);
			CALNavigationRouter::instance()->navigationRoute(d, "invokableNavigationRouteBack", routeData);
		}
	});
	d->pageTitleHLayout = new QHBoxLayout;
	d->pageTitleHLayout->setContentsMargins(0, 0, 0, 0);
	d->pageTitleHLayout->addWidget(d->breadcrumbBar);

	d->centralStackedWidget = new QStackedWidget(this);
	d->centralStackedWidget->setContentsMargins(0, 0, 0, 0);

	d->mainVLayout = new QVBoxLayout(this);
	d->mainVLayout->setContentsMargins(0, 0, 0, 0);
	d->mainVLayout->addLayout(d->pageTitleHLayout);
	d->mainVLayout->addWidget(d->centralStackedWidget);
	setContentsMargins(20, 20, 0, 0);
}

CALScrollPage::~CALScrollPage() = default;

void CALScrollPage::setCustomWidget(QWidget* widget) {
	Q_D(CALScrollPage);

	if (!widget || widget == this) {
		return;
	}

	if (d->customWidget) {
		d->mainVLayout->removeWidget(d->customWidget);
	}
	d->mainVLayout->insertWidget(1, widget);
	d->customWidget = widget;
	Q_EMIT sigCustomWidgetChanged();
}

QWidget* CALScrollPage::getCustomWidget() const {
	return d_func()->customWidget;
}

void CALScrollPage::addCentralWidget(QWidget* widget, const bool isWidgetResizeable, const bool isVerticalGrabGesture, const qreal mousePressEventDelay) {
	Q_D(CALScrollPage);

	if (!widget) {
		return;
	}

	if (widget->windowTitle().isEmpty()) {
		widget->setWindowTitle(QString("page_%1").arg(d->centralStackedWidget->count()));
	}
	if (d->centralStackedWidget->count() == 0) {
		d->breadcrumbBar->appendBreadcrumb(widget->windowTitle());
	}

	const auto scrollArea = new CALScrollArea(this);
	scrollArea->setMouseTracking(true);
	scrollArea->setIsAnimation(Qt::Vertical, true);
	scrollArea->setWidgetResizable(isWidgetResizeable);
	scrollArea->setIsGrabGesture(isVerticalGrabGesture, mousePressEventDelay);
	scrollArea->setIsOverShoot(Qt::Vertical, true);
	scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	const auto floatScrollBar = new CALScrollBar(scrollArea->verticalScrollBar(), scrollArea);
	floatScrollBar->setIsAnimation(true);
	scrollArea->setWidget(widget);
	widget->setObjectName("CALScrollPage_centralPage");
	widget->setStyleSheet("#CALScrollPage_centralPage { background-color: transparent; }");
	d->mapCentralWidget.insert(widget->windowTitle(), d->centralStackedWidget->count());
	d->centralStackedWidget->addWidget(scrollArea);
}

void CALScrollPage::navigation(const int widgetIndex, const bool isLogRoute) {
	Q_D(CALScrollPage);

	if (widgetIndex >= d->centralStackedWidget->count() || d->navigationTargetIndex == widgetIndex) {
		return;
	}

	d->switchCentralStackIndex(widgetIndex, d->navigationTargetIndex);
	d->navigationTargetIndex = widgetIndex;
	if (isLogRoute) {
		QVariantMap routeData{};
		routeData.insert("CALScrollPageCheckSumKey", "Navigation");
		const QStringList breadcrumbList = d->breadcrumbBar->getBreadcrumbList();
		routeData.insert("CALPageTitle", breadcrumbList.last());
		CALNavigationRouter::instance()->navigationRoute(d, "invokableNavigationRouteBack", routeData);
	}
	d->breadcrumbBar->appendBreadcrumb(d->mapCentralWidget.key(widgetIndex));
}

void CALScrollPage::setPageTitleSpacing(const int spacing) {
	Q_D(CALScrollPage);

	d->pageTitleSpacing = spacing;
	d->pageTitleHLayout->takeAt(0);
	d->pageTitleHLayout->addSpacing(spacing);
	d->pageTitleHLayout->addWidget(d->breadcrumbBar);
}

int CALScrollPage::getPageTitleSpacing() const {
	return d_func()->pageTitleSpacing;
}

void CALScrollPage::setTitleVisible(const bool isVisible) {
	d_func()->breadcrumbBar->setVisible(isVisible);
}
}
