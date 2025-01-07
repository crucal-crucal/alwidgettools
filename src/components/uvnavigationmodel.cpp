#include "uvnavigationmodel.hpp"

#include "uvnavigationnode.hpp"

#define NODE_MAX_DEPTHS 10

/**
 * @brief \class CUVNavigationModel
 * @param parent pointer to the parent class
 */
CUVNavigationModel::CUVNavigationModel(QObject* parent): QAbstractItemModel(parent) {
	m_rootNode = new CUVNavigationNode("root");
	m_rootNode->setIsRootNode(true);
	m_rootNode->setIsExpanderNode(true);
}

CUVNavigationModel::~CUVNavigationModel() {
	SAFE_DELETE(m_rootNode)
}

QModelIndex CUVNavigationModel::parent(const QModelIndex& child) const {
	if (!child.isValid()) {
		return {};
	}

	const auto childNode = static_cast<CUVNavigationNode*>(child.internalPointer());
	CUVNavigationNode* parentNode = childNode->getParentNode();
	if (parentNode == m_rootNode || parentNode == nullptr) {
		return {};
	}

	return createIndex(parentNode->getRow(), 0, parentNode);
}

QModelIndex CUVNavigationModel::index(const int row, const int column, const QModelIndex& parent) const {
	if (!hasIndex(row, column, parent)) {
		return {};
	}

	const CUVNavigationNode* parentNode;
	if (!parent.isValid()) {
		parentNode = m_rootNode;
	} else {
		parentNode = static_cast<CUVNavigationNode*>(parent.internalPointer());
	}

	CUVNavigationNode* childNode{ nullptr };
	if (parentNode->getChildrenNodes().count() > row) {
		childNode = parentNode->getChildrenNodes().at(row);
	}

	if (childNode) {
		return createIndex(row, column, childNode);
	}

	return {};
}

int CUVNavigationModel::rowCount(const QModelIndex& parent) const {
	CUVNavigationNode* parentNode;

	if (parent.column() > 0) {
		return 0;
	}

	if (!parent.isValid()) {
		parentNode = m_rootNode;
	} else {
		parentNode = static_cast<CUVNavigationNode*>(parent.internalPointer());
	}

	return parentNode->getChildrenNodes().count();
}

int CUVNavigationModel::columnCount(const QModelIndex& parent) const {
	return 1;
}

QVariant CUVNavigationModel::data(const QModelIndex& index, int role) const {
	return {};
}

ALNavigationType::NodeOperateReturnType CUVNavigationModel::addExpanderNode(const QString& expanderTitle, QString& expanderKey, const ALIcon::AweSomeIcon& awewomeIcon) {
	const auto node = new CUVNavigationNode(expanderTitle, m_rootNode);
	node->setDepth(1);
	node->setIsVisible(true);
	node->setIsExpanderNode(true);
	node->setAwesomeIcon(awewomeIcon);
	beginInsertRows({}, m_rootNode->getChildrenNodes().count(), m_rootNode->getChildrenNodes().count());
	m_rootNode->addChildNode(node);
	m_mapNodes[node->getNodeKey()] = node;
	endInsertRows();
	expanderKey = node->getNodeKey();

	return ALNavigationType::Success;
}

ALNavigationType::NodeOperateReturnType CUVNavigationModel::addExpanderNode(const QString& expanderTitle, QString& expanderKey, const QString& targetExpanderKey, const ALIcon::AweSomeIcon& awewomeIcon) {
	if (!m_mapNodes.contains(targetExpanderKey)) {
		return ALNavigationType::TargetNodeInvalid;
	}

	if (CUVNavigationNode* parentNode = m_mapNodes.value(targetExpanderKey); parentNode->getIsExpanderNode()) {
		if (parentNode->getDepth() > NODE_MAX_DEPTHS) {
			return ALNavigationType::TargetNodeDepthLimit;
		}

		const auto node = new CUVNavigationNode(expanderTitle, parentNode);
		node->setDepth(parentNode->getDepth() + 1);
		node->setIsExpanderNode(true);
		node->setAwesomeIcon(awewomeIcon);
		node->setIsVisible(parentNode->getIsVisible() && parentNode->getIsExpanded());
		beginInsertRows(parentNode->getModelIndex(), parentNode->getChildrenNodes().count(), parentNode->getChildrenNodes().count());
		parentNode->addChildNode(node);
		m_mapNodes[node->getNodeKey()] = node;
		endInsertRows();
		expanderKey = node->getNodeKey();

		return ALNavigationType::Success;
	}

	return ALNavigationType::TargetNodeTypeError;
}

ALNavigationType::NodeOperateReturnType CUVNavigationModel::addPageNode(const QString& pageTitle, QString& pageKey, const ALIcon::AweSomeIcon& awewomeIcon) {
	const auto node = new CUVNavigationNode(pageTitle, m_rootNode);
	node->setDepth(1);
	node->setAwesomeIcon(awewomeIcon);
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

ALNavigationType::NodeOperateReturnType CUVNavigationModel::addPageNode(const QString& pageTitle, QString& pageKey, const QString& targetExpanderKey, const ALIcon::AweSomeIcon& awewomeIcon) {
	if (!m_mapNodes.contains(targetExpanderKey)) {
		return ALNavigationType::TargetNodeInvalid;
	}

	if (CUVNavigationNode* parentNode = m_mapNodes.value(targetExpanderKey); parentNode->getIsExpanderNode()) {
		if (parentNode->getDepth() > NODE_MAX_DEPTHS) {
			return ALNavigationType::TargetNodeDepthLimit;
		}

		const auto node = new CUVNavigationNode(pageTitle, parentNode);
		node->setDepth(parentNode->getDepth() + 1);
		node->setAwesomeIcon(awewomeIcon);
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

ALNavigationType::NodeOperateReturnType CUVNavigationModel::addPageNode(const QString& pageTitle, QString& pageKey, const int keyPoints, const ALIcon::AweSomeIcon& awewomeIcon) {
	const auto node = new CUVNavigationNode(pageTitle, m_rootNode);
	node->setDepth(1);
	node->setAwesomeIcon(awewomeIcon);
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

ALNavigationType::NodeOperateReturnType CUVNavigationModel::addPageNode(const QString& pageTitle, QString& pageKey, const QString& targetExpanderKey, const int keyPoints, const ALIcon::AweSomeIcon& awewomeIcon) {
	if (!m_mapNodes.contains(targetExpanderKey)) {
		return ALNavigationType::TargetNodeInvalid;
	}

	if (CUVNavigationNode* parentNode = m_mapNodes.value(targetExpanderKey); parentNode->getIsExpanderNode()) {
		if (parentNode->getDepth() > NODE_MAX_DEPTHS) {
			return ALNavigationType::TargetNodeDepthLimit;
		}

		const auto node = new CUVNavigationNode(pageTitle, parentNode);
		node->setDepth(parentNode->getDepth() + 1);
		node->setAwesomeIcon(awewomeIcon);
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

CUVNavigationNode* CUVNavigationModel::getNavigationNode(const QString& nodeKey) const {
	return m_mapNodes.value(nodeKey, nullptr);
}

QList<CUVNavigationNode*> CUVNavigationModel::getRootExpanderNodes() const {
	QList<CUVNavigationNode*> expanderNodeList{};
	for (const auto& node : m_rootNode->getChildrenNodes()) {
		if (node->getIsExpanderNode()) {
			expanderNodeList.append(node);
		}
	}

	return expanderNodeList;
}

QList<CUVNavigationNode*> CUVNavigationModel::getRootExpandedNodes() const {
	QList<CUVNavigationNode*> expandedNodeList{};
	for (const auto& node : m_rootNode->getChildrenNodes()) {
		if (node->getIsExpanderNode() && node->getIsExpanded()) {
			expandedNodeList.append(node);
		}
	}

	return expandedNodeList;
}

void CUVNavigationModel::setSelectedNode(CUVNavigationNode* node) {
	m_selectedNode = node;
}

CUVNavigationNode* CUVNavigationModel::getSelectedNode() const {
	return  m_selectedNode;
}

void CUVNavigationModel::setSelectedExpandedNode(CUVNavigationNode* node) {
	m_selectedExpandedNode = node;
}

CUVNavigationNode* CUVNavigationModel::getSelectedExpandedNode() const {
	return m_selectedExpandedNode;
}
