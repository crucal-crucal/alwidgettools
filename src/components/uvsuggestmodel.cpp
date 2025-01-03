#include "uvsuggestmodel.hpp"

CUVSuggestModel::CUVSuggestModel(QObject* parent): QAbstractListModel(parent) {
}

CUVSuggestModel::~CUVSuggestModel() = default;

int CUVSuggestModel::rowCount(const QModelIndex& parent) const {
	return m_suggestionVector.count();
}

QVariant CUVSuggestModel::data(const QModelIndex& index, int role) const {
	return {};
}

void CUVSuggestModel::setSearchSuggestion(const QVector<CUVSuggestion*>& suggestionVector) {
	if (suggestionVector.isEmpty()) {
		return;
	}

	beginResetModel();
	m_suggestionVector = suggestionVector;
	endResetModel();
}

void CUVSuggestModel::clearSearchNode() {
	m_suggestionVector.clear();
}

CUVSuggestion* CUVSuggestModel::getSearchSuggestion(const int row) {
	return row >= m_suggestionVector.count() ? nullptr : m_suggestionVector[row];
}
