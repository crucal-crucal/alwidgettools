#include "aleventbus.hpp"

#include "aleventbus_p.hpp"

/**
 * @brief \namespace AL
 */
namespace AL {
/**
 * @brief \class CALEventPrivate
 * Internal class for CALEvent
 * @param q pointer to the public class
 * @param parent pointer to the parent class
 */
CALEventPrivate::CALEventPrivate(CALEvent* q, QObject* parent): QObject(parent), q_ptr(q) {
}

CALEventPrivate::~CALEventPrivate() = default;

/**
 * @brief \class CALEventBusPrivate
 * Internal class for CALEventBus
 * @param q pointer to the public class
 * @param parent pointer to the parent class
 */
CALEventBusPrivate::CALEventBusPrivate(CALEventBus* q, QObject* parent): QObject(parent), q_ptr(q) {
}

CALEventBusPrivate::~CALEventBusPrivate() = default;

ALEventBusType::EventBusReturnType CALEventBusPrivate::registerEvent(CALEvent* event) {
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

bool CALEventBusPrivate::unRegisterEvent(const QString& eventName) {
	return unRegisterEvent(eventName, nullptr);
}

bool CALEventBusPrivate::unRegisterEvent(CALEvent* event) {
	return event ? unRegisterEvent(event->getEventName(), event) : false;
}

bool CALEventBusPrivate::unRegisterEvent(const QString& eventName, CALEvent* event) {
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
CALEvent::CALEvent(QObject* parent): QObject(parent), d_ptr(new CALEventPrivate(this, this)) {
	Q_D(CALEvent);

	d->connectionType = Qt::AutoConnection;
	d->funcName = "";
	d->eventName = "";
}

CALEvent::CALEvent(const QString& eventName, const QString& funcName, QObject* parent): QObject(parent), d_ptr(new CALEventPrivate(this, this)) {
	Q_D(CALEvent);

	d->connectionType = Qt::AutoConnection;
	d->eventName = eventName;
	d->funcName = funcName;
}

CALEvent::~CALEvent() = default;

ALEventBusType::EventBusReturnType CALEvent::registerAndInit() {
	return CALEventBus::instance()->d_func()->registerEvent(this);
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
 * @brief \class CALEventBus
 * @return pointer to the instance of the CALEventBus
 */
CALEventBus* CALEventBus::instance() {
	return CALSingleton<CALEventBus>::instance();
}

ALEventBusType::EventBusReturnType CALEventBus::post(const QString& eventName, const QVariantMap& data) {
	Q_D(CALEventBus);

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

QStringList CALEventBus::getRegisteredEventsName() const {
	return d_func()->eventMap.isEmpty() ? QStringList{} : d_func()->eventMap.keys();
}

CALEventBus::CALEventBus(QObject* parent): QObject(parent), d_ptr(new CALEventBusPrivate(this, this)) {
}

CALEventBus::~CALEventBus() = default;
}
