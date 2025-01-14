#include "alcolordisplaymodel.hpp"

/**
 * @brief \namespace AL
 */
namespace AL {
/**
 * @brief \class CALColorDisplayModel
 * @param parent pointer to the parent class
 */
CALColorDisplayModel::CALColorDisplayModel(QObject* parent): QAbstractListModel(parent) {
}

CALColorDisplayModel::~CALColorDisplayModel() = default;

void CALColorDisplayModel::appendDisplayColor(const QList<QColor>& colorList) {
	beginResetModel();
	m_displayColorList.append(colorList);
	endResetModel();
}

void CALColorDisplayModel::appendDisplayColor(const QColor& color) {
	beginResetModel();
	m_displayColorList.append(color);
	endResetModel();
}

void CALColorDisplayModel::removeDisplayColor(const int index) {
	if (index < 0 || index >= m_displayColorList.count()) {
		return;
	}
	beginResetModel();
	m_displayColorList.removeAt(index);
	endResetModel();
}

void CALColorDisplayModel::replaceDisplayColor(const QColor& color, const int index) {
	if (index < 0 || index >= m_displayColorList.count()) {
		return;
	}

	beginResetModel();
	m_displayColorList.replace(index, color);
	endResetModel();
}

QList<QColor> CALColorDisplayModel::getDisplayColorList() const {
	return m_displayColorList;
}

QColor CALColorDisplayModel::getDisplayColor(const int index) const {
	if (index < 0 || index >= m_displayColorList.count()) {
		return {};
	}

	return m_displayColorList.at(index);
}

int CALColorDisplayModel::rowCount(const QModelIndex& parent) const {
	return m_displayColorList.count();
}

QVariant CALColorDisplayModel::data(const QModelIndex& index, const int role) const {
	return role == Qt::UserRole ? QVariant::fromValue(m_displayColorList.at(index.row())) : QVariant();
}
}
