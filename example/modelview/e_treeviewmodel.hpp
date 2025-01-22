#pragma once

#include <QAbstractItemModel>

class E_TreeItem;

class E_TreeViewModel final : public QAbstractItemModel {
	Q_OBJECT

public:
	explicit E_TreeViewModel(QObject* parent = nullptr);
	~E_TreeViewModel() override;

	[[nodiscard]] QModelIndex parent(const QModelIndex& child) const override;
	[[nodiscard]] QModelIndex index(int row, int column, const QModelIndex& parent) const override;
	[[nodiscard]] int rowCount(const QModelIndex& parent) const override;
	[[nodiscard]] int columnCount(const QModelIndex& parent) const override;
	[[nodiscard]] QVariant data(const QModelIndex& index, int role) const override;
	bool setData(const QModelIndex& index, const QVariant& value, int role) override;
	[[nodiscard]] Qt::ItemFlags flags(const QModelIndex& index) const override;
	[[nodiscard]] QVariant headerData(int section, Qt::Orientation orientation, int role) const override;

	[[nodiscard]] int getItemCount() const;

private:
	QMap<QString, E_TreeItem*> m_mapItems;
	E_TreeItem* m_pRootItem{ nullptr };
};
