#pragma once

#include <QObject>

class CALBaseListView;
class CALBreadcrumbBarDelegate;
class CALBreadcrumbBarModel;
class CALBreadcrumbBar;

class CALBreadcrumbBarPrivate final : public QObject {
	Q_OBJECT
	Q_DISABLE_COPY(CALBreadcrumbBarPrivate)
	Q_DECLARE_PUBLIC(CALBreadcrumbBar)

public:
	explicit CALBreadcrumbBarPrivate(CALBreadcrumbBar* q, QObject* parent = nullptr);
	~CALBreadcrumbBarPrivate() override;

protected:
	CALBreadcrumbBar* const q_ptr{ nullptr };

private:
	int textPixelSize{};
	bool isAutoRemove{};

	CALBreadcrumbBarModel* listModel{ nullptr };
	CALBreadcrumbBarDelegate* listDelegate{ nullptr };
	CALBaseListView* listView{ nullptr };
};
