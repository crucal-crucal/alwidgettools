#pragma once

#include <QAbstractListModel>

/**
 * @brief \namespace AL
 */
namespace AL {
class CALPovitModel final : public QAbstractListModel {
	Q_OBJECT

public:
	explicit CALPovitModel(QObject* parent = nullptr);
	~CALPovitModel() override;

	[[nodiscard]] int appendPovit(const QString& povitTitle);
	bool removePovit(const QString& povitTitle);
	bool removePovit(int index);

	[[nodiscard]] int getPovitListCount() const;

protected:
	[[nodiscard]] int rowCount(const QModelIndex& parent) const override;
	[[nodiscard]] QVariant data(const QModelIndex& index, int role) const override;

private:
	QStringList m_povitList{};
};
}
