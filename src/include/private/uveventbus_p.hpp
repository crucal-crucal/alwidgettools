#pragma once

#include "alwidgettoolsdef.hpp"

class CALEvent;

class CUVEventPrivate final : public QObject {
	Q_OBJECT
	Q_DISABLE_COPY(CUVEventPrivate)
	Q_DECLARE_PUBLIC(CALEvent)

public:
	explicit CUVEventPrivate(CALEvent* q, QObject* parent = nullptr);
	~CUVEventPrivate() override;

protected:
	CALEvent* const q_ptr{ nullptr };

private:
	QString eventName{};
	QString funcName{};
	Qt::ConnectionType connectionType{};
};

class CUVEventBus;

class CUVEventBusPrivate final : public QObject {
	Q_OBJECT
	Q_DISABLE_COPY(CUVEventBusPrivate)
	Q_DECLARE_PUBLIC(CUVEventBus)

public:
	explicit CUVEventBusPrivate(CUVEventBus* q, QObject* parent = nullptr);
	~CUVEventBusPrivate() override;

	[[nodiscard]] ALEventBusType::EventBusReturnType registerEvent(CALEvent* event);
	[[nodiscard]] bool unRegisterEvent(const QString& eventName);
	[[nodiscard]] bool unRegisterEvent(CALEvent* event);
	[[nodiscard]] bool unRegisterEvent(const QString& eventName, CALEvent* event);

protected:
	CUVEventBus* const q_ptr{ nullptr };

private:
	QMap<QString, QList<CALEvent*>> eventMap{};
};
