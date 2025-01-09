#pragma once

#include <QAbstractListModel>
#include <QMetaEnum>

class E_IconModel final : public QAbstractListModel {
	Q_OBJECT
	Q_PROPERTY(bool isSearchMode READ getIsSearchMode WRITE setIsSearchMode NOTIFY sigIsSearchModeChanged)

public:
	explicit E_IconModel(QObject* parent = nullptr);
	~E_IconModel() override;

	[[nodiscard]] int rowCount(const QModelIndex& parent) const override;
	[[nodiscard]] QVariant data(const QModelIndex& index, int role) const override;

	void setSearchKeyList(const QStringList& list);
	[[nodiscard]] QStringList getSearchKeyList() const;

	[[nodiscard]] QString getIconNameFromModelIndex(const QModelIndex& index) const;

	void setIsSearchMode(bool isSearchMode);
	[[nodiscard]] bool getIsSearchMode() const;

Q_SIGNALS:
	Q_SIGNAL void sigIsSearchModeChanged();

private:
	QMetaEnum m_metaEnum{};
	QStringList m_searchKeyList{};
	int m_rowCount{};
	bool m_isSearchMode{};
};
