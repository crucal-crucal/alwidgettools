#pragma once

#include <QAbstractListModel>
#include <QMetaEnum>

#include "alwidgettoolsdef.hpp"

class E_IconModel final : public QAbstractListModel {
	Q_OBJECT
	Q_PROPERTY(bool isSearchMode READ getIsSearchMode WRITE setIsSearchMode NOTIFY sigIsSearchModeChanged)

public:
	explicit E_IconModel(const QMetaEnum& metaEnum, QObject* parent = nullptr);
	~E_IconModel() override;

	[[nodiscard]] int rowCount(const QModelIndex& parent) const override;
	[[nodiscard]] QVariant data(const QModelIndex& index, int role) const override;

	void setSearchKeyList(const QStringList& list);
	[[nodiscard]] QStringList getSearchKeyList() const;

	[[nodiscard]] QString getIconNameFromModelIndex(const QModelIndex& index) const;
	[[nodiscard]] QString getIconNamePrefix() const { return m_iconNamePrefix; }

	void setIsSearchMode(bool isSearchMode);
	[[nodiscard]] bool getIsSearchMode() const;

	[[nodiscard]] ALIcon::IconType getIconType() const { return m_iconType; }

Q_SIGNALS:
	void sigIsSearchModeChanged();

private:
	ALIcon::IconType m_iconType{};
	QMetaEnum m_metaEnum{};
	QStringList m_searchKeyList{};
	QString m_iconNamePrefix{};
	int m_rowCount{};
	bool m_isSearchMode{};
};
