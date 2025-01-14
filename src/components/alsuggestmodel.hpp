#pragma once

#include <QAbstractListModel>

#include "alwidgettoolsdef.hpp"

/**
 * @brief \namespace AL
 */
namespace AL {
class CALSuggestion;

class CALSuggestModel final : public QAbstractListModel {
	Q_OBJECT

public:
	explicit CALSuggestModel(QObject* parent = nullptr);
	~CALSuggestModel() override;

	[[nodiscard]] int rowCount(const QModelIndex& parent) const override;
	[[nodiscard]] QVariant data(const QModelIndex& index, int role) const override;

	void setSearchSuggestion(const QVector<CALSuggestion*>& suggestionVector);
	void clearSearchNode();

	CALSuggestion* getSearchSuggestion(int row);

private:
	QVector<CALSuggestion*> m_suggestionVector{}; // 符合搜索的节点
};
}
