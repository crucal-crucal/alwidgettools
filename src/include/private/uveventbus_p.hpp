#pragma once

#include "uvwidgettoolsdef.hpp"

class CUVEvent;

class CUVEventPrivate final : public QObject {
	Q_OBJECT
	Q_DISABLE_COPY(CUVEventPrivate)
	Q_DECLARE_PUBLIC(CUVEvent)

public:
	explicit CUVEventPrivate(CUVEvent* q, QObject* parent = nullptr);
	~CUVEventPrivate() override;

protected:
	CUVEvent* const q_ptr{ nullptr };

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

	[[nodiscard]] UVEventBusType::EventBusReturnType registerEvent(CUVEvent* event);
	[[nodiscard]] bool unRegisterEvent(const QString& eventName);
	[[nodiscard]] bool unRegisterEvent(CUVEvent* event);
	[[nodiscard]] bool unRegisterEvent(const QString& eventName, CUVEvent* event);

protected:
	CUVEventBus* const q_ptr{ nullptr };

private:
	QMap<QString, QList<CUVEvent*>> eventMap{};
};
