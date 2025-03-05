#include "alnavigationmodel.hpp"

#include <QDebug>

#include "alnavigationnode.hpp"

#define NODE_MAX_DEPTHS 10

/**
 * @brief \namespace AL
 */
namespace AL {
/**
 * @brief \class CALNavigationModel
 * @param parent pointer to the parent class
 */
CALNavigationModel::CALNavigationModel(QObject* parent): QAbstractItemModel(parent) {
	m_rootNode = new CALNavigationNode("root");
	m_rootNode->setIsRootNode(true);
	m_rootNode->setIsExpanderNode(true);
}

CALNavigationModel::~CALNavigationModel() {
	SAFE_DELETE(m_rootNode)
}

QModelIndex CALNavigationModel::parent(const QModelIndex& child) const {
	if (!child.isValid()) {
		return {};
	}

	const auto childNode = static_cast<CALNavigationNode*>(child.internalPointer());
	CALNavigationNode* parentNode = childNode->getParentNode();
	if (parentNode == m_rootNode || parentNode == nullptr) {
		return {};
	}

	return createIndex(parentNode->getRow(), 0, parentNode);
}

QModelIndex CALNavigationModel::index(const int row, const int column, const QModelIndex& parent) const {
	if (!hasIndex(row, column, parent)) {
		return {};
	}

	const CALNavigationNode* parentNode;
	if (!parent.isValid()) {
		parentNode = m_rootNode;
	} else {
		parentNode = static_cast<CALNavigationNode*>(parent.internalPointer());
	}

	CALNavigationNode* childNode{ nullptr };
	if (parentNode->getChildrenNodes().count() > row) {
		childNode = parentNode->getChildrenNodes().at(row);
	}

	if (childNode) {
		return createIndex(row, column, childNode);
	}

	return {};
}

int CALNavigationModel::rowCount(const QModelIndex& parent) const {
	CALNavigationNode* parentNode;

	if (parent.column() > 0) {
		return 0;
	}

	if (!parent.isValid()) {
		parentNode = m_rootNode;
	} else {
		parentNode = static_cast<CALNavigationNode*>(parent.internalPointer());
	}

	return parentNode->getChildrenNodes().count();
}

int CALNavigationModel::columnCount(const QModelIndex& parent) const {
	return 1;
}

QVariant CALNavigationModel::data(const QModelIndex& index, int role) const {
	return {};
}

ALNavigationType::NodeOperateReturnType CALNavigationModel::addExpanderNode(const QString& expanderTitle, QString& expanderKey, const std::shared_ptr<CALIconType>& icon_type) {
#if 0
	const auto node = new CALNavigationNode(expanderTitle, m_rootNode);
	node->setDepth(1);
	node->setIsVisible(true);
	node->setIsExpanderNode(true);
	node->setALIcon(icon_type);
	beginInsertRows({}, m_rootNode->getChildrenNodes().count(), m_rootNode->getChildrenNodes().count());
	m_rootNode->addChildNode(node);
	m_mapNodes[node->getNodeKey()] = node;
	endInsertRows();
	expanderKey = node->getNodeKey();

	return ALNavigationType::Success;
#endif
	return addNodeInternal(m_rootNode, expanderTitle, expanderKey, true, icon_type);
}

ALNavigationType::NodeOperateReturnType CALNavigationModel::addExpanderNode(const QString& expanderTitle, QString& expanderKey, const QString& targetExpanderKey, const std::shared_ptr<CALIconType>& icon_type) {
	if (!m_mapNodes.contains(targetExpanderKey)) {
		return ALNavigationType::TargetNodeInvalid;
	}

	if (CALNavigationNode* parentNode = m_mapNodes.value(targetExpanderKey); parentNode->getIsExpanderNode()) {
		if (parentNode->getDepth() > NODE_MAX_DEPTHS) {
			return ALNavigationType::TargetNodeDepthLimit;
		}

#if 0
		const auto node = new CALNavigationNode(expanderTitle, parentNode);
		node->setDepth(parentNode->getDepth() + 1);
		node->setIsExpanderNode(true);
		node->setALIcon(icon_type);
		node->setIsVisible(parentNode->getIsVisible() && parentNode->getIsExpanded());
		beginInsertRows(parentNode->getModelIndex(), parentNode->getChildrenNodes().count(), parentNode->getChildrenNodes().count());
		parentNode->addChildNode(node);
		m_mapNodes[node->getNodeKey()] = node;
		endInsertRows();
		expanderKey = node->getNodeKey();

		return ALNavigationType::Success;
#endif

		return addNodeInternal(parentNode, expanderTitle, expanderKey, true, icon_type);
	}

	return ALNavigationType::TargetNodeTypeError;
}

ALNavigationType::NodeOperateReturnType CALNavigationModel::addPageNode(const QString& pageTitle, QString& pageKey, const std::shared_ptr<CALIconType>& icon_type) {
	const auto node = new CALNavigationNode(pageTitle, m_rootNode);
	node->setDepth(1);
	node->setALIcon(icon_type);
	node->setIsVisible(true);
	beginInsertRows({}, m_rootNode->getChildrenNodes().count(), m_rootNode->getChildrenNodes().count());
	m_rootNode->addChildNode(node);
	m_mapNodes[node->getNodeKey()] = node;
	endInsertRows();
	pageKey = node->getNodeKey();

	if (!m_selectedNode) {
		m_selectedNode = node;
	}

	return ALNavigationType::Success;
}

ALNavigationType::NodeOperateReturnType CALNavigationModel::addPageNode(const QString& pageTitle, QString& pageKey, const QString& targetExpanderKey, const std::shared_ptr<CALIconType>& icon_type) {
	if (!m_mapNodes.contains(targetExpanderKey)) {
		return ALNavigationType::TargetNodeInvalid;
	}

	if (CALNavigationNode* parentNode = m_mapNodes.value(targetExpanderKey); parentNode->getIsExpanderNode()) {
		if (parentNode->getDepth() > NODE_MAX_DEPTHS) {
			return ALNavigationType::TargetNodeDepthLimit;
		}

		const auto node = new CALNavigationNode(pageTitle, parentNode);
		node->setDepth(parentNode->getDepth() + 1);
		node->setALIcon(icon_type);
		node->setIsVisible(parentNode->getIsVisible() && parentNode->getIsExpanded());
		beginInsertRows(parentNode->getModelIndex(), parentNode->getChildrenNodes().count(), parentNode->getChildrenNodes().count());
		parentNode->addChildNode(node);
		m_mapNodes[node->getNodeKey()] = node;
		endInsertRows();
		pageKey = node->getNodeKey();
		if (!m_selectedNode) {
			m_selectedNode = node;
		}

		return ALNavigationType::Success;
	}

	return ALNavigationType::TargetNodeTypeError;
}

ALNavigationType::NodeOperateReturnType CALNavigationModel::addPageNode(const QString& pageTitle, QString& pageKey, const int keyPoints, const std::shared_ptr<CALIconType>& icon_type) {
	const auto node = new CALNavigationNode(pageTitle, m_rootNode);
	node->setDepth(1);
	node->setALIcon(icon_type);
	node->setIsVisible(true);
	node->setKeyPoints(keyPoints);
	beginInsertRows({}, m_rootNode->getChildrenNodes().count(), m_rootNode->getChildrenNodes().count());
	m_rootNode->addChildNode(node);
	m_mapNodes[node->getNodeKey()] = node;
	endInsertRows();
	pageKey = node->getNodeKey();
	if (!m_selectedNode) {
		m_selectedNode = node;
	}

	return ALNavigationType::Success;
}

ALNavigationType::NodeOperateReturnType CALNavigationModel::addPageNode(const QString& pageTitle, QString& pageKey, const QString& targetExpanderKey, const int keyPoints, const std::shared_ptr<CALIconType>& icon_type) {
	if (!m_mapNodes.contains(targetExpanderKey)) {
		return ALNavigationType::TargetNodeInvalid;
	}

	if (CALNavigationNode* parentNode = m_mapNodes.value(targetExpanderKey); parentNode->getIsExpanderNode()) {
		if (parentNode->getDepth() > NODE_MAX_DEPTHS) {
			return ALNavigationType::TargetNodeDepthLimit;
		}

		const auto node = new CALNavigationNode(pageTitle, parentNode);
		node->setDepth(parentNode->getDepth() + 1);
		node->setALIcon(icon_type);
		node->setKeyPoints(keyPoints);
		node->setIsVisible(parentNode->getIsVisible() && parentNode->getIsExpanded());
		beginInsertRows(parentNode->getModelIndex(), parentNode->getChildrenNodes().count(), parentNode->getChildrenNodes().count());
		parentNode->addChildNode(node);
		m_mapNodes[node->getNodeKey()] = node;
		endInsertRows();
		pageKey = node->getNodeKey();
		if (!m_selectedNode) {
			m_selectedNode = node;
		}

		return ALNavigationType::Success;
	}

	return ALNavigationType::TargetNodeTypeError;
}

QStringList CALNavigationModel::removeNavigationNode(const QString& nodeKey) { // NOLINT
	QStringList removeKeyList{};

	if (!m_mapNodes.contains(nodeKey)) {
		return removeKeyList;
	}

	CALNavigationNode* node = m_mapNodes.value(nodeKey);
	CALNavigationNode* parentNode = node->getParentNode();
	if (node->getIsExpanderNode()) {
		QList<CALNavigationNode*> childNodeList = node->getChildrenNodes();
		for (const auto& childNode : childNodeList) {
			QList<QString> childRemoveKeyList = removeNavigationNode(childNode->getNodeKey());
			removeKeyList.append(childRemoveKeyList);
		}
	} else {
		removeKeyList.append(node->getNodeKey());
	}
	beginRemoveRows(parentNode->getModelIndex(), parentNode->getChildrenNodes().count() - 1, parentNode->getChildrenNodes().count() - 1);
	parentNode->removeChildNode(node);
	m_mapNodes.remove(node->getNodeKey());
	endRemoveRows();

	return removeKeyList;
}

CALNavigationNode* CALNavigationModel::getNavigationNode(const QString& nodeKey) const {
	return m_mapNodes.value(nodeKey, nullptr);
}

QList<CALNavigationNode*> CALNavigationModel::getRootExpanderNodes() const {
	QList<CALNavigationNode*> expanderNodeList{};
	for (const auto& node : m_rootNode->getChildrenNodes()) {
		if (node->getIsExpanderNode()) {
			expanderNodeList.append(node);
		}
	}

	return expanderNodeList;
}

QList<CALNavigationNode*> CALNavigationModel::getRootExpandedNodes() const {
	QList<CALNavigationNode*> expandedNodeList{};
	for (const auto& node : m_rootNode->getChildrenNodes()) {
		if (node->getIsExpanderNode() && node->getIsExpanded()) {
			expandedNodeList.append(node);
		}
	}

	return expandedNodeList;
}

void CALNavigationModel::setSelectedNode(CALNavigationNode* node) {
	m_selectedNode = node;
}

CALNavigationNode* CALNavigationModel::getSelectedNode() const {
	return m_selectedNode;
}

void CALNavigationModel::setSelectedExpandedNode(CALNavigationNode* node) {
	m_selectedExpandedNode = node;
}

CALNavigationNode* CALNavigationModel::getSelectedExpandedNode() const {
	return m_selectedExpandedNode;
}

ALNavigationType::NodeOperateReturnType CALNavigationModel::addNodeInternal(CALNavigationNode* parentNode, const QString& title, QString& outKey, const bool isExpander, const std::shared_ptr<CALIconType>& icon_type, const int keyPoints) {
	try {
		const auto node = new CALNavigationNode(title, parentNode);
		node->setDepth(parentNode->getDepth() + 1);
		node->setIsExpanderNode(isExpander);
		node->setIsVisible(parentNode->getIsVisible() && parentNode->getIsExpanded());
		outKey = node->getNodeKey();
		node->setALIcon(icon_type);
		if (!isExpander && keyPoints > 0) {
			node->setKeyPoints(keyPoints);
		}
		addNodeToModel(node, parentNode);
		if (!isExpander && !m_selectedNode) {
			m_selectedNode = node;
		}
		return ALNavigationType::NodeOperateReturnType::Success;
	} catch (const std::exception& e) {
		qDebug() << QString(e.what());
		return ALNavigationType::NodeOperateReturnType::TargetNodeTypeError;
	}
}

void CALNavigationModel::addNodeToModel(CALNavigationNode* node, CALNavigationNode* parentNode) {
	const auto index = parentNode->getModelIndex();
	const auto count = parentNode->getChildrenNodes().count();
	beginInsertRows(index, count, count);
	parentNode->addChildNode(node);
	m_mapNodes[node->getNodeKey()] = node;
	endInsertRows();
}
}
