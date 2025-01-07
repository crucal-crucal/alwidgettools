#pragma once

#include <QObject>
#include <QQueue>
#include <QVariantMap>

class CALNavigationRouter;

class CALNavigationRouterPrivate final : public QObject {
	Q_OBJECT
	Q_DISABLE_COPY(CALNavigationRouterPrivate)
	Q_DECLARE_PUBLIC(CALNavigationRouter)

public:
	explicit CALNavigationRouterPrivate(CALNavigationRouter* q, QObject* parent = nullptr);
	~CALNavigationRouterPrivate() override;

protected:
	CALNavigationRouter* const q_ptr{ nullptr };

private:
	int maxRouteCount{};
	QQueue<QVariantMap> routeQueue{};
};
