#pragma once

#include <QWidget>

#include "uvwidgettoolsdef.hpp"

class CUVBreadcrumbBarPrivate;

class CUVWIDGETTOOLS_EXPORT CUVBreadcrumbBar : public QWidget {
	Q_OBJECT
	Q_DECLARE_PRIVATE(CUVBreadcrumbBar)
	Q_PROPERTY(int textPixelSize READ getTextPixelSize WRITE setTextPixelSize NOTIFY sigTextPixelSizeChanged)
	Q_PROPERTY(bool isAutoRemove READ getIsAutoRemove WRITE setIsAutoRemove NOTIFY sigIsAutoRemoveChanged)

public:
	explicit CUVBreadcrumbBar(QWidget* parent = nullptr);
	~CUVBreadcrumbBar() override;

	void setBreadcrumbList(const QStringList& breadcrumbList);
	QStringList appendBreadcrumb(const QString& breadCrumb);
	QStringList removeBreadcrumb(const QString& breadCrumb);

	[[nodiscard]] int getBreadcrumbListCount() const;
	[[nodiscard]] QStringList getBreadcrumbList() const;

	void setTextPixelSize(int textPixelSize);
	[[nodiscard]] int getTextPixelSize() const;

	void setIsAutoRemove(bool isAutoRemove);
	[[nodiscard]] bool getIsAutoRemove() const;

Q_SIGNALS:
	Q_SIGNAL void sigTextPixelSizeChanged();
	Q_SIGNAL void sigIsAutoRemoveChanged();
	Q_SIGNAL void sigBreadcrumbClicked(const QString& breadcrumb, const QStringList& lastBreadcrumbList);

protected:
	const QScopedPointer<CUVBreadcrumbBarPrivate> d_ptr{ nullptr };
};
