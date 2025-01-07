#include "albreadcrumbarmodel.hpp"

#include <QDebug>
/**
 * @brief \class CALBreadcrumbBarModel
 * @param parent pointer to the parent class
 */
CALBreadcrumbBarModel::CALBreadcrumbBarModel(QObject* parent): QAbstractListModel(parent) {
}

CALBreadcrumbBarModel::~CALBreadcrumbBarModel() = default;

void CALBreadcrumbBarModel::appendBreadcrumb(const QString& breadCrumb) {
	if (!breadCrumb.isEmpty()) {
		if (m_breadcrumbList.count() && m_breadcrumbList.last() == breadCrumb) {
			return;
		}

		if (m_breadcrumbList.count() >= 2 && m_breadcrumbList[m_breadcrumbList.count() - 2] == breadCrumb) {
			beginResetModel();
			m_breadcrumbList.removeLast();
			endResetModel();
			return;
		}
		beginResetModel();
		m_breadcrumbList.append(breadCrumb);
		endResetModel();
	}
}

void CALBreadcrumbBarModel::removeBreadcrumb(const QString& breadCrumb) {
	if (m_breadcrumbList.contains(breadCrumb)) {
		beginResetModel();
		m_breadcrumbList.removeAt(m_breadcrumbList.lastIndexOf(breadCrumb));
		endResetModel();
	}
}

void CALBreadcrumbBarModel::removeBreadcrumb(const int index) {
	if (index >= m_breadcrumbList.count()) {
		return;
	}
	beginResetModel();
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
		_breadcrumbList.remove(index, _breadcrumbList.count() - index);
#else
	for (int i = m_breadcrumbList.count() - 1; i >= 0; i--) {
		if (i < index) {
			break;
		}
		m_breadcrumbList.removeAt(i);
	}
#endif
	endResetModel();
}

void CALBreadcrumbBarModel::setBreadcrumbList(const QStringList& breadcrumbList) {
	beginResetModel();
	m_breadcrumbList = breadcrumbList;
	endResetModel();
}

int CALBreadcrumbBarModel::getBreadcrumbListCount() const {
	return m_breadcrumbList.count();
}

QStringList CALBreadcrumbBarModel::getBreadcrumbList() const {
	return m_breadcrumbList;
}

int CALBreadcrumbBarModel::rowCount(const QModelIndex& parent) const {
	return m_breadcrumbList.count() * 2 - 1;
}

QVariant CALBreadcrumbBarModel::data(const QModelIndex& index, const int role) const {
	if (role == Qt::DisplayRole) {
		return index.row() % 2 ? ">" : m_breadcrumbList[index.row() / 2];
	} else if (role == Qt::UserRole) {
		return index.row() == m_breadcrumbList.count() * 2 - 2 || index.row() == m_breadcrumbList.count() * 2 - 3 ? QString("LastBreadcrumb") : QVariant();
	}
	return {};
}
