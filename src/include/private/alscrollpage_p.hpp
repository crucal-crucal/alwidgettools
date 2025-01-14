#pragma once

#include <QMap>
#include <QObject>

class QStackedWidget;
class QVBoxLayout;
class QHBoxLayout;

/**
 * @brief \namespace AL
 */
namespace AL {
class CALScrollArea;
class CALScrollPage;
class CALText;
class CALBreadcrumbBar;

class CALScrollPagePrivate final : public QObject {
	Q_OBJECT
	Q_DISABLE_COPY(CALScrollPagePrivate)
	Q_DECLARE_PUBLIC(CALScrollPage)

public:
	explicit CALScrollPagePrivate(CALScrollPage* q, QObject* parent = nullptr);
	~CALScrollPagePrivate() override;

	Q_INVOKABLE void invokableNavigationRouteBack(const QVariantMap& routeData);

protected:
	CALScrollPage* const q_ptr{ nullptr };

private:
	QWidget* customWidget{ nullptr };
	QHBoxLayout* pageTitleHLayout{ nullptr };
	QVBoxLayout* mainVLayout{ nullptr };
	QStackedWidget* centralStackedWidget{ nullptr };
	CALBreadcrumbBar* breadcrumbBar{ nullptr };

	QMap<QString, int> mapCentralWidget{};
	int navigationTargetIndex{ 0 };
	int pageTitleSpacing{ 0 };
	bool isGrabGesture{};

	void switchCentralStackIndex(int targetIndex, int lastIndex);
};
}
