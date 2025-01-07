#pragma once

#include <QAbstractListModel>

class CALBreadcrumbBarModel final : public QAbstractListModel {
	Q_OBJECT

public:
	explicit CALBreadcrumbBarModel(QObject* parent = nullptr);
	~CALBreadcrumbBarModel() override;

	void appendBreadcrumb(const QString& breadCrumb);
	void removeBreadcrumb(const QString& breadCrumb);
	void removeBreadcrumb(int index);

	void setBreadcrumbList(const QStringList& breadcrumbList);

	int getBreadcrumbListCount() const;
	QStringList getBreadcrumbList() const;

protected:
	int rowCount(const QModelIndex& parent) const override;
	QVariant data(const QModelIndex& index, int role) const override;

private:
	QStringList m_breadcrumbList{};
};