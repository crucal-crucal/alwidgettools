#pragma once

#include <QVariantMap>

#include "alwidgettoolsdef.hpp"
#include "alsingleton.hpp"

/**
 * @brief \namespace AL
 */
namespace AL {
class CALNavigationRouterPrivate;

class CALWIDGETTOOLS_EXPORT CALNavigationRouter : public QObject {
	Q_OBJECT
	Q_DECLARE_PRIVATE(CALNavigationRouter)
	Q_PROPERTY(int maxRouteCount READ getMaxRouteCount WRITE setMaxRouteCount NOTIFY sigMaxRouteCountChanged)

public:
	static CALNavigationRouter* instance();

	ALNavigationRouterType::NavigationRouteType navigationRoute(QObject* routeObject, const QString& routeFuncName, const QVariantMap& routeData = {}, const Qt::ConnectionType& connectionType = Qt::AutoConnection);
	void navigationRouteBack();

	void setMaxRouteCount(int maxRouteCount);
	[[nodiscard]] int getMaxRouteCount() const;

Q_SIGNALS:
	Q_SIGNAL void sigMaxRouteCountChanged();
	Q_SIGNAL void sigNavigationRouterStateChanged(bool state);

protected:
	const QScopedPointer<CALNavigationRouterPrivate> d_ptr{ nullptr };

	friend class CALSingleton<CALNavigationRouter>;

private:
	explicit CALNavigationRouter(QObject* parent = nullptr);
	~CALNavigationRouter() override;
};
}
