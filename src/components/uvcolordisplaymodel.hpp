#pragma once

#include <QAbstractListModel>
#include <QColor>

class CUVColorDisplayModel final : public QAbstractListModel {
	Q_OBJECT

public:
	explicit CUVColorDisplayModel(QObject* parent = nullptr);
	~CUVColorDisplayModel() override;

	void appendDisplayColor(const QList<QColor>& colorList);
	void appendDisplayColor(const QColor& color);
	void removeDisplayColor(int index);
	void replaceDisplayColor(const QColor& color, int index);

	[[nodiscard]] QList<QColor> getDisplayColorList() const;
	[[nodiscard]] QColor getDisplayColor(int index) const;

protected:
	[[nodiscard]] int rowCount(const QModelIndex& parent) const override;
	[[nodiscard]] QVariant data(const QModelIndex& index, int role) const override;

private:
	QList<QColor> m_displayColorList{};
};
