#include "uveventbus.hpp"

#include "uveventbus_p.hpp"

/**
 * @brief \class CUVEventPrivate
 * Internal class for CUVEvent
 * @param q pointer to the public class
 * @param parent pointer to the parent class
 */
CUVEventPrivate::CUVEventPrivate(CUVEvent* q, QObject* parent): QObject(parent), q_ptr(q) {
}

CUVEventPrivate::~CUVEventPrivate() = default;

/**
 * @brief \class CUVEventBusPrivate
 * Internal class for CUVEventBus
 * @param q pointer to the public class
 * @param parent pointer to the parent class
 */
CUVEventBusPrivate::CUVEventBusPrivate(CUVEventBus* q, QObject* parent): QObject(parent), q_ptr(q) {
}

CUVEventBusPrivate::~CUVEventBusPrivate() = default;

UVEventBusType::EventBusReturnType CUVEventBusPrivate::registerEvent(CUVEvent* event) {
	if (!event) {
		return UVEventBusType::EventInvalid;
	}

	const QString& eventName = event->getEventName();
	if (eventName.isEmpty()) {
		return UVEventBusType::EventNameInvalid;
	}

	auto& eventList = eventMap[eventName];
	if (eventList.contains(event)) {
		return UVEventBusType::EventInvalid;
	}
	eventList.append(event);

	return UVEventBusType::Success;
}

bool CUVEventBusPrivate::unRegisterEvent(const QString& eventName) {
	return unRegisterEvent(eventName, nullptr);
}

bool CUVEventBusPrivate::unRegisterEvent(CUVEvent* event) {
	return event ? unRegisterEvent(event->getEventName(), event) : false;
}

bool CUVEventBusPrivate::unRegisterEvent(const QString& eventName, CUVEvent* event) {
	if (eventName.isEmpty() || !eventMap.contains(eventName)) {
		return false;
	}

	auto& eventList = eventMap[eventName];
	eventList.removeOne(event);

	if (eventList.isEmpty()) {
		eventMap.remove(eventName);
	}

	return true;
}

/**
 * @brief \class CUVEvent
 * @param parent pointer to the parent class
 */
CUVEvent::CUVEvent(QObject* parent): QObject(parent), d_ptr(new CUVEventPrivate(this, this)) {
	Q_D(CUVEvent);

	d->connectionType = Qt::AutoConnection;
	d->funcName = "";
	d->eventName = "";
}

CUVEvent::CUVEvent(const QString& eventName, const QString& funcName, QObject* parent): QObject(parent), d_ptr(new CUVEventPrivate(this, this)) {
	Q_D(CUVEvent);

	d->connectionType = Qt::AutoConnection;
	d->eventName = eventName;
	d->funcName = funcName;
}

CUVEvent::~CUVEvent() = default;

UVEventBusType::EventBusReturnType CUVEvent::registerAndInit() {
	return CUVEventBus::instance()->d_func()->registerEvent(this);
}

void CUVEvent::setEventName(const QString& eventName) {
	d_func()->eventName = eventName;
	Q_EMIT sigEventNameChanged();
}

QString CUVEvent::getEventName() const {
	return d_func()->eventName;
}

void CUVEvent::setFuncName(const QString& funcName) {
	d_func()->funcName = funcName;
	Q_EMIT sigFuncNameChanged();
}

QString CUVEvent::getFuncName() const {
	return d_func()->funcName;
}

void CUVEvent::setConnectionType(const Qt::ConnectionType& connectionType) {
	d_func()->connectionType = connectionType;
	Q_EMIT sigConnectionTypeChanged();
}

Qt::ConnectionType CUVEvent::getConnectionType() const {
	return d_func()->connectionType;
}

/**
 * @brief \class CUVEventBus
 * @return pointer to the instance of the CUVEventBus
 */
CUVEventBus* CUVEventBus::instance() {
	return CUVSingleton<CUVEventBus>::instance();
}

UVEventBusType::EventBusReturnType CUVEventBus::post(const QString& eventName, const QVariantMap& data) {
	Q_D(CUVEventBus);

	if (eventName.isEmpty()) {
		return UVEventBusType::EventNameInvalid;
	}

	if (d->eventMap.contains(eventName)) {
		const auto& eventList = d->eventMap.value(eventName);
		for (auto& event : eventList) {
			if (event->parent()) {
				QMetaObject::invokeMethod(event->parent(), event->getFuncName().toLocal8Bit().constData(), event->getConnectionType(), Q_ARG(QVariantMap, data));
			}
		}
	}

	return UVEventBusType::Success;
}

QStringList CUVEventBus::getRegisteredEventsName() const {
	return d_func()->eventMap.isEmpty() ? QStringList{} : d_func()->eventMap.keys();
}

CUVEventBus::CUVEventBus(QObject* parent): QObject(parent), d_ptr(new CUVEventBusPrivate(this, this)) {
}

CUVEventBus::~CUVEventBus() = default;
