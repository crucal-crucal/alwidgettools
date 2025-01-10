#include "alnavigationrouter.hpp"

#include "alnavigationrouter_p.hpp"

/**
 * @brief \class CALNavigationRouterPrivate
 * Internal class for CALNavigationRouter
 * @param q pointer to the public class
 * @param parent pointer to the parent class
 */
CALNavigationRouterPrivate::CALNavigationRouterPrivate(CALNavigationRouter* q, QObject* parent): QObject(parent), q_ptr(q) {

}

CALNavigationRouterPrivate::~CALNavigationRouterPrivate() = default;

/**
 * @brief \class CALNavigationRouter
 * @return CALNavigationRouter instance
 */
CALNavigationRouter* CALNavigationRouter::instance() {
	return CALSingleton<CALNavigationRouter>::instance();
}

ALNavigationRouterType::NavigationRouteType CALNavigationRouter::navigationRoute(QObject* routeObject, const QString& routeFuncName, const QVariantMap& routeData, const Qt::ConnectionType& connectionType) {
	Q_D(CALNavigationRouter);

	if (!routeObject) {
		return ALNavigationRouterType::ObjectInvalid;
	}
	if (routeFuncName.isEmpty()) {
		return ALNavigationRouterType::FunctionNameInvalid;
	}

	if (d->routeQueue.isEmpty()) {
		Q_EMIT sigNavigationRouterStateChanged(true);
	} else {
		if (d->routeQueue.count() >= d->maxRouteCount) {
			d->routeQueue.dequeue();
		}
	}
	QVariantMap savaData{};
	savaData.insert("CALRouteObject", QVariant::fromValue<QObject*>(routeObject));
	savaData.insert("CALRouteFuncName", routeFuncName);
	savaData.insert("CALRouteData", routeData);
	savaData.insert("CALRouteConnectionType", QVariant::fromValue<Qt::ConnectionType>(connectionType));
	d->routeQueue.enqueue(savaData);

	return ALNavigationRouterType::Success;
}

void CALNavigationRouter::navigationRouteBack() {
	Q_D(CALNavigationRouter);

	if (const int routeQueneCount = d->routeQueue.count(); routeQueneCount > 0) {
		if (routeQueneCount == 1) {
			Q_EMIT sigNavigationRouterStateChanged(false);
		}
		const QVariantMap routeSaveData = d->routeQueue.last();
		d->routeQueue.removeLast();
		const auto routeObject = routeSaveData.value("CALRouteObject").value<QObject*>();
		const QString routeFuncName = routeSaveData.value("CALRouteFuncName").toString();
		const QVariantMap routeData = routeSaveData.value("CALRouteData").toMap();
		const auto connectionType = routeSaveData.value("CALRouteConnectionType").value<Qt::ConnectionType>();
		QMetaObject::invokeMethod(routeObject, routeFuncName.toLocal8Bit().constData(), connectionType, Q_ARG(QVariantMap, routeData));
	}
}

void CALNavigationRouter::setMaxRouteCount(const int maxRouteCount) {
	d_func()->maxRouteCount = maxRouteCount;
	Q_EMIT sigMaxRouteCountChanged();
}

int CALNavigationRouter::getMaxRouteCount() const {
	return d_func()->maxRouteCount;
}

CALNavigationRouter::CALNavigationRouter(QObject* parent): QObject(parent), d_ptr(new CALNavigationRouterPrivate(this, this)) {
	d_func()->maxRouteCount = 25;
}

CALNavigationRouter::~CALNavigationRouter() = default;
