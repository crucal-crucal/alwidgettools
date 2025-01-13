#pragma once

#include "alwidgettoolsdef.hpp"

namespace AL {
class CALEvent;
class CALEventBus;

class CALEventPrivate final : public QObject {
	Q_OBJECT
	Q_DISABLE_COPY(CALEventPrivate)
	Q_DECLARE_PUBLIC(CALEvent)

public:
	explicit CALEventPrivate(CALEvent* q, QObject* parent = nullptr);
	~CALEventPrivate() override;

protected:
	CALEvent* const q_ptr{ nullptr };

private:
	QString eventName{};
	QString funcName{};
	Qt::ConnectionType connectionType{};
};

class CALEventBusPrivate final : public QObject {
	Q_OBJECT
	Q_DISABLE_COPY(CALEventBusPrivate)
	Q_DECLARE_PUBLIC(CALEventBus)

public:
	explicit CALEventBusPrivate(CALEventBus* q, QObject* parent = nullptr);
	~CALEventBusPrivate() override;

	[[nodiscard]] ALEventBusType::EventBusReturnType registerEvent(CALEvent* event);
	[[nodiscard]] bool unRegisterEvent(const QString& eventName);
	[[nodiscard]] bool unRegisterEvent(CALEvent* event);
	[[nodiscard]] bool unRegisterEvent(const QString& eventName, CALEvent* event);

protected:
	CALEventBus* const q_ptr{ nullptr };

private:
	QMap<QString, QList<CALEvent*>> eventMap{};
};

} // namespace AL
