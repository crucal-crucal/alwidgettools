#include "uvcolordisplaymodel.hpp"

/**
 * @brief \class CUVColorDisplayModel
 * @param parent pointer to the parent class
 */
CUVColorDisplayModel::CUVColorDisplayModel(QObject* parent): QAbstractListModel(parent) {
}

CUVColorDisplayModel::~CUVColorDisplayModel() = default;

void CUVColorDisplayModel::appendDisplayColor(const QList<QColor>& colorList) {
	beginResetModel();
	m_displayColorList.append(colorList);
	endResetModel();
}

void CUVColorDisplayModel::appendDisplayColor(const QColor& color) {
	beginResetModel();
	m_displayColorList.append(color);
	endResetModel();
}

void CUVColorDisplayModel::removeDisplayColor(const int index) {
	if (index < 0 || index >= m_displayColorList.count()) {
		return;
	}
	beginResetModel();
	m_displayColorList.removeAt(index);
	endResetModel();
}

void CUVColorDisplayModel::replaceDisplayColor(const QColor& color, const int index) {
	if (index < 0 || index >= m_displayColorList.count()) {
		return;
	}

	beginResetModel();
	m_displayColorList.replace(index, color);
	endResetModel();
}

QList<QColor> CUVColorDisplayModel::getDisplayColorList() const {
	return m_displayColorList;
}

QColor CUVColorDisplayModel::getDisplayColor(const int index) const {
	if (index < 0 || index >= m_displayColorList.count()) {
		return {};
	}

	return m_displayColorList.at(index);
}

int CUVColorDisplayModel::rowCount(const QModelIndex& parent) const {
	return m_displayColorList.count();
}

QVariant CUVColorDisplayModel::data(const QModelIndex& index, const int role) const {
	return role == Qt::UserRole ? QVariant::fromValue(m_displayColorList.at(index.row())) : QVariant();
}
