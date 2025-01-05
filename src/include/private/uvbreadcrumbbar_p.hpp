#pragma once

#include <QObject>

class CUVBaseListView;
class CUVBreadcrumbBarDelegate;
class CUVBreadcrumbBarModel;
class CUVBreadcrumbBar;

class CUVBreadcrumbBarPrivate final : public QObject {
	Q_OBJECT
	Q_DISABLE_COPY(CUVBreadcrumbBarPrivate)
	Q_DECLARE_PUBLIC(CUVBreadcrumbBar)

public:
	explicit CUVBreadcrumbBarPrivate(CUVBreadcrumbBar* q, QObject* parent = nullptr);
	~CUVBreadcrumbBarPrivate() override;

protected:
	CUVBreadcrumbBar* const q_ptr{ nullptr };

private:
	int textPixelSize{};
	bool isAutoRemove{};

	CUVBreadcrumbBarModel* listModel{ nullptr };
	CUVBreadcrumbBarDelegate* listDelegate{ nullptr };
	CUVBaseListView* listView{ nullptr };
};
