#pragma once

#include <QAbstractListModel>

#include "alwidgettoolsdef.hpp"

class CUVSuggestion;

class CUVSuggestModel final : public QAbstractListModel {
	Q_OBJECT

public:
	explicit CUVSuggestModel(QObject* parent = nullptr);
	~CUVSuggestModel() override;

	[[nodiscard]] int rowCount(const QModelIndex& parent) const override;
	[[nodiscard]] QVariant data(const QModelIndex& index, int role) const override;

	void setSearchSuggestion(const QVector<CUVSuggestion*>& suggestionVector);
	void clearSearchNode();

	CUVSuggestion* getSearchSuggestion(int row);

private:
	QVector<CUVSuggestion*> m_suggestionVector{}; // 符合搜索的节点
};
