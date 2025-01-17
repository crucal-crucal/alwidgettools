#pragma once

#include <QAbstractTableModel>

class E_TableViewModel final : public QAbstractTableModel {
	Q_OBJECT

public:
	explicit E_TableViewModel(QObject* parent = nullptr);
	~E_TableViewModel() override;

	[[nodiscard]] int rowCount(const QModelIndex& parent) const override;
	[[nodiscard]] int columnCount(const QModelIndex& parent) const override;

	[[nodiscard]] QVariant data(const QModelIndex& index, int role) const override;
	[[nodiscard]] QVariant headerData(int section, Qt::Orientation orientation, int role) const override;

private:
	QStringList m_header{};
	QList<QStringList> m_dataList{};
	QList<QIcon> m_iconList{};
};
