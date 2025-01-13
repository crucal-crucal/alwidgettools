#pragma once

#include <QQueue>
#include <QVariantMap>

#include "alwidgettoolsdef.hpp"

namespace AL {
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
} // namespace AL