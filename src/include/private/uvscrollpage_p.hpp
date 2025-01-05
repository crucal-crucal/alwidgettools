#pragma once

#include <QObject>
#include <QMap>

class QStackedWidget;
class QVBoxLayout;
class QHBoxLayout;

class CUVScrollArea;
class CUVScrollPage;
class CUVText;
class CUVBreadcrumbBar;

class CUVScrollPagePrivate final : public QObject {
	Q_OBJECT
	Q_DISABLE_COPY(CUVScrollPagePrivate)
	Q_DECLARE_PUBLIC(CUVScrollPage)

public:
	explicit CUVScrollPagePrivate(CUVScrollPage* q, QObject* parent = nullptr);
	~CUVScrollPagePrivate() override;

	Q_INVOKABLE void invokableNavigationRouteBack(const QVariantMap& routeData);

protected:
	CUVScrollPage* const q_ptr{ nullptr };

private:
	QWidget* customWidget{ nullptr };
	QHBoxLayout* pageTitleHLayout{ nullptr };
	QVBoxLayout* mainVLayout{ nullptr };
	QStackedWidget* centralStackedWidget{ nullptr };
	CUVBreadcrumbBar* breadcrumbBar{ nullptr };

	QMap<QString, int> mapCentralWidget{};
	int navigationTargetIndex{ 0 };
	int pageTitleSpacing{ 0 };
	bool isGrabGesture{};

	void switchCentralStackIndex(int targetIndex, int lastIndex);
};
