#include "alnavigationmodel.hpp"

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

ALNavigationType::NodeOperateReturnType CALNavigationModel::addExpanderNode(const QString& expanderTitle, QString& expanderKey, const ALIcon::AweSomeIcon& awewomeIcon) {
	const auto node = new CALNavigationNode(expanderTitle, m_rootNode);
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

ALNavigationType::NodeOperateReturnType CALNavigationModel::addExpanderNode(const QString& expanderTitle, QString& expanderKey, const ALIcon::FluentIcon& fluentIcon) {
	const auto node = new CALNavigationNode(expanderTitle, m_rootNode);
	node->setDepth(1);
	node->setIsVisible(true);
	node->setIsExpanderNode(true);
	node->setFluentIcon(fluentIcon);
	beginInsertRows({}, m_rootNode->getChildrenNodes().count(), m_rootNode->getChildrenNodes().count());
	m_rootNode->addChildNode(node);
	m_mapNodes[node->getNodeKey()] = node;
	endInsertRows();
	expanderKey = node->getNodeKey();

	return ALNavigationType::Success;
}

ALNavigationType::NodeOperateReturnType CALNavigationModel::addExpanderNode(const QString& expanderTitle, QString& expanderKey, const QString& targetExpanderKey, const ALIcon::AweSomeIcon& awewomeIcon) {
	if (!m_mapNodes.contains(targetExpanderKey)) {
		return ALNavigationType::TargetNodeInvalid;
	}

	if (CALNavigationNode* parentNode = m_mapNodes.value(targetExpanderKey); parentNode->getIsExpanderNode()) {
		if (parentNode->getDepth() > NODE_MAX_DEPTHS) {
			return ALNavigationType::TargetNodeDepthLimit;
		}

		const auto node = new CALNavigationNode(expanderTitle, parentNode);
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

ALNavigationType::NodeOperateReturnType CALNavigationModel::addExpanderNode(const QString& expanderTitle, QString& expanderKey, const QString& targetExpanderKey, const ALIcon::FluentIcon& fluentIcon) {
	if (!m_mapNodes.contains(targetExpanderKey)) {
		return ALNavigationType::TargetNodeInvalid;
	}

	if (CALNavigationNode* parentNode = m_mapNodes.value(targetExpanderKey); parentNode->getIsExpanderNode()) {
		if (parentNode->getDepth() > NODE_MAX_DEPTHS) {
			return ALNavigationType::TargetNodeDepthLimit;
		}

		const auto node = new CALNavigationNode(expanderTitle, parentNode);
		node->setDepth(parentNode->getDepth() + 1);
		node->setIsExpanderNode(true);
		node->setFluentIcon(fluentIcon);
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

ALNavigationType::NodeOperateReturnType CALNavigationModel::addPageNode(const QString& pageTitle, QString& pageKey, const ALIcon::AweSomeIcon& awewomeIcon) {
	const auto node = new CALNavigationNode(pageTitle, m_rootNode);
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

ALNavigationType::NodeOperateReturnType CALNavigationModel::addPageNode(const QString& pageTitle, QString& pageKey, const ALIcon::FluentIcon& fluentIcon) {
	const auto node = new CALNavigationNode(pageTitle, m_rootNode);
	node->setDepth(1);
	node->setFluentIcon(fluentIcon);
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

ALNavigationType::NodeOperateReturnType CALNavigationModel::addPageNode(const QString& pageTitle, QString& pageKey, const QString& targetExpanderKey, const ALIcon::AweSomeIcon& awewomeIcon) {
	if (!m_mapNodes.contains(targetExpanderKey)) {
		return ALNavigationType::TargetNodeInvalid;
	}

	if (CALNavigationNode* parentNode = m_mapNodes.value(targetExpanderKey); parentNode->getIsExpanderNode()) {
		if (parentNode->getDepth() > NODE_MAX_DEPTHS) {
			return ALNavigationType::TargetNodeDepthLimit;
		}

		const auto node = new CALNavigationNode(pageTitle, parentNode);
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

ALNavigationType::NodeOperateReturnType CALNavigationModel::addPageNode(const QString& pageTitle, QString& pageKey, const QString& targetExpanderKey, const ALIcon::FluentIcon& fluentIcon) {
	if (!m_mapNodes.contains(targetExpanderKey)) {
		return ALNavigationType::TargetNodeInvalid;
	}

	if (CALNavigationNode* parentNode = m_mapNodes.value(targetExpanderKey); parentNode->getIsExpanderNode()) {
		if (parentNode->getDepth() > NODE_MAX_DEPTHS) {
			return ALNavigationType::TargetNodeDepthLimit;
		}

		const auto node = new CALNavigationNode(pageTitle, parentNode);
		node->setDepth(parentNode->getDepth() + 1);
		node->setFluentIcon(fluentIcon);
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

ALNavigationType::NodeOperateReturnType CALNavigationModel::addPageNode(const QString& pageTitle, QString& pageKey, const int keyPoints, const ALIcon::AweSomeIcon& awewomeIcon) {
	const auto node = new CALNavigationNode(pageTitle, m_rootNode);
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

ALNavigationType::NodeOperateReturnType CALNavigationModel::addPageNode(const QString& pageTitle, QString& pageKey, const int keyPoints, const ALIcon::FluentIcon& fluentIcon) {
	const auto node = new CALNavigationNode(pageTitle, m_rootNode);
	node->setDepth(1);
	node->setFluentIcon(fluentIcon);
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

ALNavigationType::NodeOperateReturnType CALNavigationModel::addPageNode(const QString& pageTitle, QString& pageKey, const QString& targetExpanderKey, const int keyPoints, const ALIcon::AweSomeIcon& awewomeIcon) {
	if (!m_mapNodes.contains(targetExpanderKey)) {
		return ALNavigationType::TargetNodeInvalid;
	}

	if (CALNavigationNode* parentNode = m_mapNodes.value(targetExpanderKey); parentNode->getIsExpanderNode()) {
		if (parentNode->getDepth() > NODE_MAX_DEPTHS) {
			return ALNavigationType::TargetNodeDepthLimit;
		}

		const auto node = new CALNavigationNode(pageTitle, parentNode);
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

ALNavigationType::NodeOperateReturnType CALNavigationModel::addPageNode(const QString& pageTitle, QString& pageKey, const QString& targetExpanderKey, const int keyPoints, const ALIcon::FluentIcon& fluentIcon) {
	if (!m_mapNodes.contains(targetExpanderKey)) {
		return ALNavigationType::TargetNodeInvalid;
	}

	if (CALNavigationNode* parentNode = m_mapNodes.value(targetExpanderKey); parentNode->getIsExpanderNode()) {
		if (parentNode->getDepth() > NODE_MAX_DEPTHS) {
			return ALNavigationType::TargetNodeDepthLimit;
		}

		const auto node = new CALNavigationNode(pageTitle, parentNode);
		node->setDepth(parentNode->getDepth() + 1);
		node->setFluentIcon(fluentIcon);
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
}
