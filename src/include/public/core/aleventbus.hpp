#pragma once

#include <QVariantMap>

#include "alsingleton.hpp"
#include "alwidgettoolsdef.hpp"

namespace AL {
class CALEventPrivate;

class CALWIDGETTOOLS_EXPORT CALEvent : public QObject {
	Q_OBJECT
	Q_DECLARE_PRIVATE(CALEvent)
	Q_PROPERTY(QString eventName READ getEventName WRITE setEventName NOTIFY sigEventNameChanged)
	Q_PROPERTY(QString funcName READ getFuncName WRITE setFuncName NOTIFY sigFuncNameChanged)
	Q_PROPERTY(Qt::ConnectionType connectionType READ getConnectionType WRITE setConnectionType NOTIFY sigConnectionTypeChanged)

public:
	explicit CALEvent(QObject* parent = nullptr);
	explicit CALEvent(const QString& eventName, const QString& funcName, QObject* parent = nullptr);
	~CALEvent() override;

	ALEventBusType::EventBusReturnType registerAndInit();

	void setEventName(const QString& eventName);
	[[nodiscard]] QString getEventName() const;

	void setFuncName(const QString& funcName);
	[[nodiscard]] QString getFuncName() const;

	void setConnectionType(const Qt::ConnectionType& connectionType);
	[[nodiscard]] Qt::ConnectionType getConnectionType() const;


Q_SIGNALS:
	Q_SIGNAL void sigEventNameChanged();
	Q_SIGNAL void sigFuncNameChanged();
	Q_SIGNAL void sigConnectionTypeChanged();

protected:
	const QScopedPointer<CALEventPrivate> d_ptr{ nullptr };
};

class CALEventBusPrivate;

class CALWIDGETTOOLS_EXPORT CALEventBus final : public QObject {
	Q_OBJECT
	Q_DECLARE_PRIVATE(CALEventBus)

public:
	static CALEventBus* instance();

	ALEventBusType::EventBusReturnType post(const QString& eventName, const QVariantMap& data = {});
	[[nodiscard]] QStringList getRegisteredEventsName() const;

protected:
	const QScopedPointer<CALEventBusPrivate> d_ptr{ nullptr };

	friend class CALSingleton<CALEventBus>;
	friend class CALEvent;

private:
	explicit CALEventBus(QObject* parent = nullptr);
	~CALEventBus() override;
};
} // namespace AL