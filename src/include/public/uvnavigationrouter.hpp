#pragma once

#include <QVariantMap>

#include "uvwidgettoolsdef.hpp"
#include "uvsingleton.hpp"

class CUVNavigationRouterPrivate;

class CUVWIDGETTOOLS_EXPORT CUVNavigationRouter : public QObject {
	Q_OBJECT
	Q_DECLARE_PRIVATE(CUVNavigationRouter)
	Q_PROPERTY(int maxRouteCount READ getMaxRouteCount WRITE setMaxRouteCount NOTIFY sigMaxRouteCountChanged)

public:
	static CUVNavigationRouter* instance();

	UVNavigationRouterType::NavigationRouteType navigationRoute(QObject* routeObject, const QString& routeFuncName, const QVariantMap& routeData = {}, const Qt::ConnectionType& connectionType = Qt::AutoConnection);
	void navigationRouteBack();

	void setMaxRouteCount(int maxRouteCount);
	[[nodiscard]] int getMaxRouteCount() const;

Q_SIGNALS:
	Q_SIGNAL void sigMaxRouteCountChanged();
	Q_SIGNAL void sigNavigationRouterStateChanged(bool state);

protected:
	const QScopedPointer<CUVNavigationRouterPrivate> d_ptr{ nullptr };

	friend class CUVSingleton<CUVNavigationRouter>;

private:
	explicit CUVNavigationRouter(QObject* parent = nullptr);
	~CUVNavigationRouter() override;
};
