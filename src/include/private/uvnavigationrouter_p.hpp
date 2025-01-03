#pragma once

#include <QObject>
#include <QQueue>
#include <QVariantMap>

class CUVNavigationRouter;

class CUVNavigationRouterPrivate final : public QObject {
	Q_OBJECT
	Q_DISABLE_COPY(CUVNavigationRouterPrivate)
	Q_DECLARE_PUBLIC(CUVNavigationRouter)

public:
	explicit CUVNavigationRouterPrivate(CUVNavigationRouter* q, QObject* parent = nullptr);
	~CUVNavigationRouterPrivate() override;

protected:
	CUVNavigationRouter* const q_ptr{ nullptr };

private:
	int maxRouteCount{};
	QQueue<QVariantMap> routeQueue{};
};
