#pragma once

#include <QVariantMap>

#include "alsingleton.hpp"
#include "alwidgettoolsdef.hpp"

/**
 * @brief \namespace AL
 */
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

	/**
	 * @brief 注册并初始化事件到事件总线
	 * @return \enum ALEventBusType::EventBusReturnType
	 */
	ALEventBusType::EventBusReturnType registerAndInit();

	/**
	 * @brief 设置事件名称
	 * @param eventName 事件名称
	 */
	void setEventName(const QString& eventName);
	[[nodiscard]] QString getEventName() const;

	/**
	 * @brief 设置函数名称
	 * @param funcName 函数名称
	 */
	void setFuncName(const QString& funcName);
	[[nodiscard]] QString getFuncName() const;

	/**
	 * @brief 设置事件连接类型
	 * @param connectionType 事件连接类型
	 */
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

	/**
	 * @brief 发布事件到事件总线
	 * @param eventName 事件名称
	 * @param data 事件数据
	 * @return \enum ALEventBusType::EventBusReturnType
	 */
	ALEventBusType::EventBusReturnType post(const QString& eventName, const QVariantMap& data = {});

	/**
	 * @brief 获取已注册的事件名称列表
	 * @return 注册列表
	 */
	[[nodiscard]] QStringList getRegisteredEventsName() const;

protected:
	const QScopedPointer<CALEventBusPrivate> d_ptr{ nullptr };

	friend class CALSingleton<CALEventBus>;
	friend class CALEvent;

private:
	explicit CALEventBus(QObject* parent = nullptr);
	~CALEventBus() override;
};
}
