#include "alsuggestmodel.hpp"

namespace AL {
/**
 * @brief \class CALSuggestModel
 * @param parent pointer to the parent class
 */
CALSuggestModel::CALSuggestModel(QObject* parent): QAbstractListModel(parent) {
}

CALSuggestModel::~CALSuggestModel() = default;

int CALSuggestModel::rowCount(const QModelIndex& parent) const {
	return m_suggestionVector.count();
}

QVariant CALSuggestModel::data(const QModelIndex& index, int role) const {
	return {};
}

void CALSuggestModel::setSearchSuggestion(const QVector<CALSuggestion*>& suggestionVector) {
	if (suggestionVector.isEmpty()) {
		return;
	}

	beginResetModel();
	m_suggestionVector = suggestionVector;
	endResetModel();
}

void CALSuggestModel::clearSearchNode() {
	m_suggestionVector.clear();
}

CALSuggestion* CALSuggestModel::getSearchSuggestion(const int row) {
	return row >= m_suggestionVector.count() ? nullptr : m_suggestionVector[row];
}
} // namespace AL