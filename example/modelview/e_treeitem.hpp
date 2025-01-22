#pragma once

#include <QObject>

class E_TreeItem final : public QObject {
	Q_OBJECT
	Q_PROPERTY(QList<E_TreeItem*> childrenItems READ getChildrenItems WRITE setChildrenItems NOTIFY sigChildrenItemsChanged)
	Q_PROPERTY(bool isChecked READ getIsChecked WRITE setIsChecked NOTIFY sigIsCheckedChanged)

public:
	explicit E_TreeItem(const QString& itemTitle, E_TreeItem* parent = nullptr);
	~E_TreeItem() override;

	[[nodiscard]] QString getItemKey() const;
	[[nodiscard]] QString getItemTitle() const;

	void setChildChecked(bool isChecked);
	[[nodiscard]] Qt::CheckState getChildCheckState() const;

	void appendChildItem(E_TreeItem* childItem);

	[[nodiscard]] bool getIsHasChild() const;
	[[nodiscard]] int getRow() const;

	void setChildrenItems(const QList<E_TreeItem*>& childrenItems);
	[[nodiscard]] QList<E_TreeItem*> getChildrenItems() const;

	void setIsChecked(bool isChecked);
	[[nodiscard]] bool getIsChecked() const;

	void setParentItem(E_TreeItem* parentItem);
	[[nodiscard]] E_TreeItem* getParentItem() const;

Q_SIGNALS:
	Q_SIGNAL void sigChildrenItemsChanged();
	Q_SIGNAL void sigIsCheckedChanged();

private:
	QString m_itemKey{};
	QString m_itemTitle{};
	bool m_isChecked{};
	QList<E_TreeItem*> m_childrenItems{};
	E_TreeItem* m_parentItem{ nullptr };
};
