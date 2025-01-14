#pragma once

#include <QAbstractListModel>

#include "alwidgettoolsdef.hpp"

/**
 * @brief \namespace AL
 */
namespace AL {
class CALBreadcrumbBarModel final : public QAbstractListModel {
	Q_OBJECT

public:
	explicit CALBreadcrumbBarModel(QObject* parent = nullptr);
	~CALBreadcrumbBarModel() override;

	void appendBreadcrumb(const QString& breadCrumb);
	void removeBreadcrumb(const QString& breadCrumb);
	void removeBreadcrumb(int index);

	void setBreadcrumbList(const QStringList& breadcrumbList);

	[[nodiscard]] int getBreadcrumbListCount() const;
	[[nodiscard]] QStringList getBreadcrumbList() const;

protected:
	[[nodiscard]] int rowCount(const QModelIndex& parent) const override;
	[[nodiscard]] QVariant data(const QModelIndex& index, int role) const override;

private:
	QStringList m_breadcrumbList{};
};
}
