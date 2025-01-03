#pragma once

#include <QVariantMap>

#include "uvsingleton.hpp"
#include "uvwidgettoolsdef.hpp"

class CUVEventPrivate;

class CUVWIDGETTOOLS_EXPORT CUVEvent : public QObject {
	Q_OBJECT
	Q_DECLARE_PRIVATE(CUVEvent)
	Q_PROPERTY(QString eventName READ getEventName WRITE setEventName NOTIFY sigEventNameChanged)
	Q_PROPERTY(QString funcName READ getFuncName WRITE setFuncName NOTIFY sigFuncNameChanged)
	Q_PROPERTY(Qt::ConnectionType connectionType READ getConnectionType WRITE setConnectionType NOTIFY sigConnectionTypeChanged)

public:
	explicit CUVEvent(QObject* parent = nullptr);
	explicit CUVEvent(const QString& eventName, const QString& funcName, QObject* parent = nullptr);
	~CUVEvent() override;

	UVEventBusType::EventBusReturnType registerAndInit();

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
	const QScopedPointer<CUVEventPrivate> d_ptr{ nullptr };
};

class CUVEventBusPrivate;

class CUVWIDGETTOOLS_EXPORT CUVEventBus final : public QObject {
	Q_OBJECT
	Q_DECLARE_PRIVATE(CUVEventBus)

public:
	static CUVEventBus* instance();

	UVEventBusType::EventBusReturnType post(const QString& eventName, const QVariantMap& data = {});
	[[nodiscard]] QStringList getRegisteredEventsName() const;

protected:
	const QScopedPointer<CUVEventBusPrivate> d_ptr{ nullptr };

	friend class CUVSingleton<CUVEventBus>;
	friend class CUVEvent;

private:
	explicit CUVEventBus(QObject* parent = nullptr);
	~CUVEventBus() override;
};
