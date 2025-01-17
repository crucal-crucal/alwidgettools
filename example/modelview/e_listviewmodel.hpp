#pragma once

#include <QAbstractListModel>

class E_ListViewModel final : public QAbstractListModel {
	Q_OBJECT

public:
	explicit E_ListViewModel(QObject* parent = nullptr);
	~E_ListViewModel() override;

protected:
	[[nodiscard]] int rowCount(const QModelIndex& parent) const override;
	[[nodiscard]] QVariant data(const QModelIndex& index, int role) const override;

private:
	QStringList m_dataList{};
};
