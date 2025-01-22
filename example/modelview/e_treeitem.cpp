#include "e_treeitem.hpp"

#include <QUuid>

E_TreeItem::E_TreeItem(const QString& itemTitle, E_TreeItem* parent): QObject(parent) {
	m_itemKey = QUuid::createUuid().toString().remove("{").remove("}").remove("-");
	m_itemTitle = itemTitle;
	m_parentItem = parent;
	m_isChecked = false;
}

E_TreeItem::~E_TreeItem() {
	qDeleteAll(m_childrenItems);
}

QString E_TreeItem::getItemKey() const {
	return m_itemKey;
}

QString E_TreeItem::getItemTitle() const {
	return m_itemTitle;
}

void E_TreeItem::setChildChecked(const bool isChecked) { // NOLINT
	for (const auto& node : m_childrenItems) {
		isChecked ? node->setIsChecked(isChecked) : node->setChildChecked(isChecked);
		isChecked ? node->setChildChecked(isChecked) : node->setIsChecked(isChecked);
	}
}

Qt::CheckState E_TreeItem::getChildCheckState() const { // NOLINT
	bool bIsAllChecked{ true };
	bool bIsAnyChecked{ false };

	for (const auto node : m_childrenItems) {
		bIsAnyChecked = node->getIsChecked();
		if (const Qt::CheckState childState = node->getChildCheckState(); childState == Qt::PartiallyChecked) {
			bIsAllChecked = false;
			bIsAnyChecked = true;
			break;
		} else if (childState == Qt::Unchecked) {
			bIsAllChecked = false;
		}
	}

	if (m_childrenItems.count() > 0) {
		return bIsAllChecked ? Qt::Checked : bIsAnyChecked ? Qt::PartiallyChecked : Qt::Unchecked;
	}

	return Qt::Checked;
}

void E_TreeItem::appendChildItem(E_TreeItem* childItem) {
	m_childrenItems.append(childItem);
}

bool E_TreeItem::getIsHasChild() const {
	return m_childrenItems.count() > 0;
}

int E_TreeItem::getRow() const {
	return m_parentItem ? m_parentItem->getChildrenItems().indexOf(const_cast<E_TreeItem*>(this)) : 0;
}

void E_TreeItem::setChildrenItems(const QList<E_TreeItem*>& childrenItems) {
	m_childrenItems = childrenItems;
	Q_EMIT sigChildrenItemsChanged();
}

QList<E_TreeItem*> E_TreeItem::getChildrenItems() const {
	return m_childrenItems;
}

void E_TreeItem::setIsChecked(const bool isChecked) {
	m_isChecked = isChecked;
	Q_EMIT sigIsCheckedChanged();
}

bool E_TreeItem::getIsChecked() const {
	return m_isChecked;
}

void E_TreeItem::setParentItem(E_TreeItem* parentItem) {
	m_parentItem = parentItem;
}

E_TreeItem* E_TreeItem::getParentItem() const {
	return m_parentItem;
}
