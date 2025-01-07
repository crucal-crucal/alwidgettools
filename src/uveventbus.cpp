#include "uveventbus.hpp"

#include "uveventbus_p.hpp"

/**
 * @brief \class CUVEventPrivate
 * Internal class for CUVEvent
 * @param q pointer to the public class
 * @param parent pointer to the parent class
 */
CUVEventPrivate::CUVEventPrivate(CALEvent* q, QObject* parent): QObject(parent), q_ptr(q) {
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

ALEventBusType::EventBusReturnType CUVEventBusPrivate::registerEvent(CALEvent* event) {
	if (!event) {
		return ALEventBusType::EventInvalid;
	}

	const QString& eventName = event->getEventName();
	if (eventName.isEmpty()) {
		return ALEventBusType::EventNameInvalid;
	}

	auto& eventList = eventMap[eventName];
	if (eventList.contains(event)) {
		return ALEventBusType::EventInvalid;
	}
	eventList.append(event);

	return ALEventBusType::Success;
}

bool CUVEventBusPrivate::unRegisterEvent(const QString& eventName) {
	return unRegisterEvent(eventName, nullptr);
}

bool CUVEventBusPrivate::unRegisterEvent(CALEvent* event) {
	return event ? unRegisterEvent(event->getEventName(), event) : false;
}

bool CUVEventBusPrivate::unRegisterEvent(const QString& eventName, CALEvent* event) {
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
 * @brief \class CALEvent
 * @param parent pointer to the parent class
 */
CALEvent::CALEvent(QObject* parent): QObject(parent), d_ptr(new CUVEventPrivate(this, this)) {
	Q_D(CUVEvent);

	d->connectionType = Qt::AutoConnection;
	d->funcName = "";
	d->eventName = "";
}

CALEvent::CALEvent(const QString& eventName, const QString& funcName, QObject* parent): QObject(parent), d_ptr(new CUVEventPrivate(this, this)) {
	Q_D(CUVEvent);

	d->connectionType = Qt::AutoConnection;
	d->eventName = eventName;
	d->funcName = funcName;
}

CALEvent::~CALEvent() = default;

ALEventBusType::EventBusReturnType CALEvent::registerAndInit() {
	return CUVEventBus::instance()->d_func()->registerEvent(this);
}

void CALEvent::setEventName(const QString& eventName) {
	d_func()->eventName = eventName;
	Q_EMIT sigEventNameChanged();
}

QString CALEvent::getEventName() const {
	return d_func()->eventName;
}

void CALEvent::setFuncName(const QString& funcName) {
	d_func()->funcName = funcName;
	Q_EMIT sigFuncNameChanged();
}

QString CALEvent::getFuncName() const {
	return d_func()->funcName;
}

void CALEvent::setConnectionType(const Qt::ConnectionType& connectionType) {
	d_func()->connectionType = connectionType;
	Q_EMIT sigConnectionTypeChanged();
}

Qt::ConnectionType CALEvent::getConnectionType() const {
	return d_func()->connectionType;
}

/**
 * @brief \class CUVEventBus
 * @return pointer to the instance of the CUVEventBus
 */
CUVEventBus* CUVEventBus::instance() {
	return CUVSingleton<CUVEventBus>::instance();
}

ALEventBusType::EventBusReturnType CUVEventBus::post(const QString& eventName, const QVariantMap& data) {
	Q_D(CUVEventBus);

	if (eventName.isEmpty()) {
		return ALEventBusType::EventNameInvalid;
	}

	if (d->eventMap.contains(eventName)) {
		const auto& eventList = d->eventMap.value(eventName);
		for (auto& event : eventList) {
			if (event->parent()) {
				QMetaObject::invokeMethod(event->parent(), event->getFuncName().toLocal8Bit().constData(), event->getConnectionType(), Q_ARG(QVariantMap, data));
			}
		}
	}

	return ALEventBusType::Success;
}

QStringList CUVEventBus::getRegisteredEventsName() const {
	return d_func()->eventMap.isEmpty() ? QStringList{} : d_func()->eventMap.keys();
}

CUVEventBus::CUVEventBus(QObject* parent): QObject(parent), d_ptr(new CUVEventBusPrivate(this, this)) {
}

CUVEventBus::~CUVEventBus() = default;
