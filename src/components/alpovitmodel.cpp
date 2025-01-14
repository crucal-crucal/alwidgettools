#include "alpovitmodel.hpp"

/**
 * @brief \namespace AL
 */
namespace AL {
/**
 * @brief \class CALPovitModel
 * @param parent pointer to the parent class
 */
CALPovitModel::CALPovitModel(QObject* parent): QAbstractListModel(parent) {
}

CALPovitModel::~CALPovitModel() = default;

int CALPovitModel::appendPovit(const QString& povitTitle) {
	int nRet{ -1 };

	if (!povitTitle.isEmpty()) {
		beginInsertRows({}, m_povitList.count(), m_povitList.count());
		m_povitList.append(povitTitle);
		endInsertRows();
		nRet = m_povitList.count() - 1;
	}

	return nRet;
}

bool CALPovitModel::removePovit(const QString& povitTitle) {
	return removePovit(m_povitList.indexOf(povitTitle));
}

bool CALPovitModel::removePovit(const int index) {
	bool bRet{ false };

	if (index >= 0 && index < m_povitList.count()) {
		beginRemoveRows({}, index, index);
		m_povitList.removeAt(index);
		endRemoveRows();
		bRet = true;
	}

	return bRet;
}

int CALPovitModel::getPovitListCount() const {
	return m_povitList.count();
}

int CALPovitModel::rowCount(const QModelIndex& parent) const {
	return getPovitListCount();
}

QVariant CALPovitModel::data(const QModelIndex& index, const int role) const {
	if (role == Qt::DisplayRole) {
		return m_povitList.at(index.row());
	}
	return {};
}
}
