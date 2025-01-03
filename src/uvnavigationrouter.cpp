#include "uvnavigationrouter.hpp"

#include "uvnavigationrouter_p.hpp"

/**
 * @brief \class CUVNavigationRouterPrivate
 * Internal class for CUVNavigationRouter
 * @param q pointer to the public class
 * @param parent pointer to the parent class
 */
CUVNavigationRouterPrivate::CUVNavigationRouterPrivate(CUVNavigationRouter* q, QObject* parent): QObject(parent), q_ptr(q) {

}

CUVNavigationRouterPrivate::~CUVNavigationRouterPrivate() = default;

/**
 * @brief \class CUVNavigationRouter
 * @return CUVNavigationRouter instance
 */
CUVNavigationRouter* CUVNavigationRouter::instance() {
	return CUVSingleton<CUVNavigationRouter>::instance();
}

UVNavigationRouterType::NavigationRouteType CUVNavigationRouter::navigationRoute(QObject* routeObject, const QString& routeFuncName, const QVariantMap& routeData, const Qt::ConnectionType& connectionType) {
	Q_D(CUVNavigationRouter);

	if (!routeObject) {
		return UVNavigationRouterType::ObjectInvalid;
	}
	if (routeFuncName.isEmpty()) {
		return UVNavigationRouterType::FunctionNameInvalid;
	}

	if (d->routeQueue.isEmpty()) {
		Q_EMIT sigNavigationRouterStateChanged(true);
	} else {
		if (d->routeQueue.count() >= d->maxRouteCount) {
			d->routeQueue.dequeue();
		}
	}
	QVariantMap savaData{};
	savaData.insert("CUVRouteObject", QVariant::fromValue<QObject*>(routeObject));
	savaData.insert("CUVRouteFuncName", routeFuncName);
	savaData.insert("CUVRouteData", routeData);
	savaData.insert("CUVRouteConnectionType", QVariant::fromValue<Qt::ConnectionType>(connectionType));
	d->routeQueue.enqueue(savaData);

	return UVNavigationRouterType::Success;
}

void CUVNavigationRouter::navigationRouteBack() {
	Q_D(CUVNavigationRouter);

	if (const int routeQueneCount = d->routeQueue.count(); routeQueneCount > 0) {
		if (routeQueneCount == 1) {
			Q_EMIT sigNavigationRouterStateChanged(false);
		}
		const QVariantMap routeSaveData = d->routeQueue.last();
		d->routeQueue.removeLast();
		const auto routeObject = routeSaveData.value("CUVRouteObject").value<QObject*>();
		const QString routeFuncName = routeSaveData.value("CUVRouteFuncName").toString();
		const QVariantMap routeData = routeSaveData.value("CUVRouteData").toMap();
		const auto connectionType = routeSaveData.value("CUVRouteConnectionType").value<Qt::ConnectionType>();
		QMetaObject::invokeMethod(routeObject, routeFuncName.toLocal8Bit().constData(), connectionType, Q_ARG(QVariantMap, routeData));
	}
}

void CUVNavigationRouter::setMaxRouteCount(const int maxRouteCount) {
	d_func()->maxRouteCount = maxRouteCount;
	Q_EMIT sigMaxRouteCountChanged();
}

int CUVNavigationRouter::getMaxRouteCount() const {
	return d_func()->maxRouteCount;
}

CUVNavigationRouter::CUVNavigationRouter(QObject* parent): QObject(parent), d_ptr(new CUVNavigationRouterPrivate(this, this)) {
	d_func()->maxRouteCount = 25;
}

CUVNavigationRouter::~CUVNavigationRouter() = default;
