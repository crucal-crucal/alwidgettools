#include "e_treeviewmodel.hpp"

#include <QIcon>

#include "e_treeitem.hpp"

#define ITEM_COUNT 20
#define SUB_ITEM_COUNT 6

E_TreeViewModel::E_TreeViewModel(QObject* parent): QAbstractItemModel(parent) {
	m_pRootItem = new E_TreeItem("root");
	for (int i = 0; i < ITEM_COUNT; ++i) {
		const auto level1 = new E_TreeItem(QString("level1_%1").arg(i), m_pRootItem);
		for (int j = 0; j < SUB_ITEM_COUNT; ++j) {
			const auto level2 = new E_TreeItem(QString("level2_%1").arg(j), level1);
			for (int k = 0; k < SUB_ITEM_COUNT; ++k) {
				const auto level3 = new E_TreeItem(QString("level3_%1").arg(k), level2);
				for (int l = 0; l < SUB_ITEM_COUNT; ++l) {
					const auto level4 = new E_TreeItem(QString("level4_%1").arg(l), level3);
					level3->appendChildItem(level4);
					m_mapItems.insert(level4->getItemKey(), level4);
				}
				level2->appendChildItem(level3);
				m_mapItems.insert(level3->getItemKey(), level3);
			}
			level1->appendChildItem(level2);
			m_mapItems.insert(level2->getItemKey(), level2);
		}
		m_pRootItem->appendChildItem(level1);
		m_mapItems.insert(level1->getItemKey(), level1);
	}
}

E_TreeViewModel::~E_TreeViewModel() = default;

QModelIndex E_TreeViewModel::parent(const QModelIndex& child) const {
	if (!child.isValid()) {
		return {};
	}

	const auto childItem = static_cast<E_TreeItem*>(child.internalPointer());
	const auto parentItem = childItem->getParentItem();
	if (parentItem == m_pRootItem || parentItem == nullptr) {
		return {};
	}

	return createIndex(parentItem->getRow(), 0, parentItem);
}

QModelIndex E_TreeViewModel::index(const int row, const int column, const QModelIndex& parent) const {
	if (!hasIndex(row, column, parent)) {
		return {};
	}
	E_TreeItem* parentItem;
	if (!parent.isValid()) {
		parentItem = m_pRootItem;
	} else {
		parentItem = static_cast<E_TreeItem*>(parent.internalPointer());
	}

	E_TreeItem* childItem{ nullptr };
	if (parentItem->getChildrenItems().count() > row) {
		childItem = parentItem->getChildrenItems().at(row);
	}

	if (childItem) {
		return createIndex(row, column, childItem);
	}

	return {};
}

int E_TreeViewModel::rowCount(const QModelIndex& parent) const {
	E_TreeItem* parentItem;
	if (parent.column() > 0) {
		return 0;
	}

	if (!parent.isValid()) {
		parentItem = m_pRootItem;
	} else {
		parentItem = static_cast<E_TreeItem*>(parent.internalPointer());
	}

	return parentItem->getChildrenItems().count();
}

int E_TreeViewModel::columnCount(const QModelIndex& parent) const {
	return 1;
}

QVariant E_TreeViewModel::data(const QModelIndex& index, const int role) const {
	if (role == Qt::DisplayRole) {
		return static_cast<E_TreeItem*>(index.internalPointer())->getItemTitle();
	} else if (role == Qt::DecorationRole) {
		return QIcon(":example/promotion/promotion4.jpg");
	} else if (role == Qt::CheckStateRole) {
		const auto item = static_cast<E_TreeItem*>(index.internalPointer());
		return item->getIsHasChild() ? item->getChildCheckState() : item->getIsChecked() ? Qt::Checked : Qt::Unchecked;
	}

	return {};
}

bool E_TreeViewModel::setData(const QModelIndex& index, const QVariant& value, const int role) {
	if (role == Qt::CheckStateRole) {
		const auto item = static_cast<E_TreeItem*>(index.internalPointer());
		item->setIsChecked(!item->getIsChecked());
		item->setChildChecked(item->getIsChecked());
		Q_EMIT dataChanged({}, {}, { role });
		return true;
	}

	return QAbstractItemModel::setData(index, value, role);
}

Qt::ItemFlags E_TreeViewModel::flags(const QModelIndex& index) const {
	Qt::ItemFlags flags = QAbstractItemModel::flags(index);
	flags |= Qt::ItemIsUserCheckable;
	return flags;
}

QVariant E_TreeViewModel::headerData(const int section, const Qt::Orientation orientation, const int role) const {
	if (orientation == Qt::Horizontal && role == Qt::DisplayRole) {
		return QString("CALTreeView-Example");
	}

	return QAbstractItemModel::headerData(section, orientation, role);
}

int E_TreeViewModel::getItemCount() const {
	return m_mapItems.count();
}
