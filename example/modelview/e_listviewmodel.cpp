#include "e_listviewmodel.hpp"

E_ListViewModel::E_ListViewModel(QObject* parent): QAbstractListModel(parent) {
	for (int i = 0; i < 1e3; ++i) {
		m_dataList.append(QString("list tiem %1").arg(i));
	}
}

E_ListViewModel::~E_ListViewModel() = default;

int E_ListViewModel::rowCount(const QModelIndex& parent) const {
	return m_dataList.count();
}

QVariant E_ListViewModel::data(const QModelIndex& index, const int role) const {
	if (role == Qt::DisplayRole) {
		return m_dataList.at(index.row());
	}
	return {};
}
