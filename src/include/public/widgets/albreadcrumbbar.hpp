#pragma once

#include <QWidget>

#include "alwidgettoolsdef.hpp"

/**
 * @brief \namespace AL
 */
namespace AL {
class CALBreadcrumbBarPrivate;

class CALWIDGETTOOLS_EXPORT CALBreadcrumbBar : public QWidget {
	Q_OBJECT
	Q_DECLARE_PRIVATE(CALBreadcrumbBar)
	Q_PROPERTY(int textPixelSize READ getTextPixelSize WRITE setTextPixelSize NOTIFY sigTextPixelSizeChanged)
	Q_PROPERTY(bool isAutoRemove READ getIsAutoRemove WRITE setIsAutoRemove NOTIFY sigIsAutoRemoveChanged)

public:
	explicit CALBreadcrumbBar(QWidget* parent = nullptr);
	~CALBreadcrumbBar() override;

	/**
	 * @brief 设置面包屑导航列表
	 * @param breadcrumbList 面包屑导航列表
	 */
	void setBreadcrumbList(const QStringList& breadcrumbList);

	/**
	 * @brief 添加新的面包屑到列表
	 * @param breadCrumb 新的面包屑项
	 * @return 当前面包屑列表
	 */
	QStringList appendBreadcrumb(const QString& breadCrumb);

	/**
	 * @brief 从当前面包屑列表中移除指定项
	 * @param breadCrumb 需要移除的项
	 * @return 当前面包屑列表
	 */
	QStringList removeBreadcrumb(const QString& breadCrumb);

	/**
	 * @brief 获取当前面包屑列表的项数
	 * @return 当前面包屑项数
	 */
	[[nodiscard]] int getBreadcrumbListCount() const;

	/**
	 * @brief 获取当前面包屑列表
	 * @return 当前面包屑列表
	 */
	[[nodiscard]] QStringList getBreadcrumbList() const;

	/**
	 * @brief 设置面包屑文字像素大小
	 * @param textPixelSize 文字像素大小
	 */
	void setTextPixelSize(int textPixelSize);
	[[nodiscard]] int getTextPixelSize() const;

	/**
	 * @brief 设置是否自动移除面包屑项
	 * @param isAutoRemove 是否自动移除
	 */
	void setIsAutoRemove(bool isAutoRemove);
	[[nodiscard]] bool getIsAutoRemove() const;

Q_SIGNALS:
	Q_SIGNAL void sigTextPixelSizeChanged();
	Q_SIGNAL void sigIsAutoRemoveChanged();
	Q_SIGNAL void sigBreadcrumbClicked(const QString& breadcrumb, const QStringList& lastBreadcrumbList);

protected:
	const QScopedPointer<CALBreadcrumbBarPrivate> d_ptr{ nullptr };
};
}
