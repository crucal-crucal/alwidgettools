#include "uvnavigationnode.hpp"

#include <QUuid>

/**
 * @brief \class CUVNavigationNode
 * @param nodeTitle title of the node
 * @param parent pointer to the parent class
 */
CUVNavigationNode::CUVNavigationNode(const QString& nodeTitle, CUVNavigationNode* parent): QObject(parent) {
	m_depth = 0;
	m_keyPoints = 0;
	m_nodeKey = QUuid::createUuid().toString().remove("{").remove("}").remove("-");
	m_nodeTitle = nodeTitle;
	m_isRootNode = false;
	m_isFooterNode = false;
	m_isHasFooterNode = false;
	m_isExpanderNode = false;
	m_isVisible = false;
	m_isExpanded = false;
	m_parentNode = parent;
}

CUVNavigationNode::~CUVNavigationNode() {
	qDeleteAll(m_childrenNodes);
}

void CUVNavigationNode::setParentNode(CUVNavigationNode* parentNode) {
	m_parentNode = parentNode;
}

CUVNavigationNode* CUVNavigationNode::getParentNode() const {
	return m_parentNode;
}

void CUVNavigationNode::setChildrenNodes(const QList<CUVNavigationNode*>& childrenNodes) {
	m_childrenNodes = childrenNodes;
	Q_EMIT sigChildrenNodesChanged();
}

QList<CUVNavigationNode*> CUVNavigationNode::getChildrenNodes() const {
	return m_childrenNodes;
}

void CUVNavigationNode::setAwesomeIcon(const ALIcon::AweSomeIcon& awesomeIcon) {
	m_awesomeIcon = awesomeIcon;
	Q_EMIT sigAwesomeIconChanged();
}

ALIcon::AweSomeIcon CUVNavigationNode::getAwesomeIcon() const {
	return m_awesomeIcon;
}

void CUVNavigationNode::setModelIndex(const QModelIndex& modelIndex) {
	m_modelIndex = modelIndex;
	Q_EMIT sigModelIndexChanged();
}

QModelIndex CUVNavigationNode::getModelIndex() const {
	return m_modelIndex;
}

void CUVNavigationNode::setKeyPoints(const int keyPoints) {
	m_keyPoints = keyPoints;
	Q_EMIT sigKeyPointsChanged();
}

int CUVNavigationNode::getKeyPoints() const {
	return m_keyPoints;
}

void CUVNavigationNode::setDepth(const int depth) {
	m_depth = depth;
	Q_EMIT sigDepthChanged();
}

int CUVNavigationNode::getDepth() const {
	return m_depth;
}

void CUVNavigationNode::setIsRootNode(const bool isRootNode) {
	m_isRootNode = isRootNode;
	Q_EMIT sigIsRootNodeChanged();
}

bool CUVNavigationNode::getIsRootNode() const {
	return m_isRootNode;
}

void CUVNavigationNode::setIsFooterNode(const bool isFooterNode) {
	m_isFooterNode = isFooterNode;
	Q_EMIT sigIsFooterNodeChanged();
}

bool CUVNavigationNode::getIsFooterNode() const {
	return m_isFooterNode;
}

void CUVNavigationNode::setIsHasFooterNode(const bool isHasFooterNode) {
	m_isHasFooterNode = isHasFooterNode;
	Q_EMIT sigIsHasFooterNodeChanged();
}

bool CUVNavigationNode::getIsHasFooterNode() const {
	return m_isHasFooterNode;
}

void CUVNavigationNode::setIsExpanderNode(const bool isExpanderNode) {
	m_isExpanderNode = isExpanderNode;
	Q_EMIT sigIsExpanderNodeChanged();
}

bool CUVNavigationNode::getIsExpanderNode() const {
	return m_isExpanderNode;
}

void CUVNavigationNode::setIsVisible(const bool isVisible) {
	m_isVisible = isVisible;
	Q_EMIT sigIsVisibleChanged();
}

bool CUVNavigationNode::getIsVisible() const {
	return m_isVisible;
}

QString CUVNavigationNode::getNodeKey() const {
	return m_nodeKey;
}

QString CUVNavigationNode::getNodeTitle() const {
	return m_nodeTitle;
}

void CUVNavigationNode::setIsExpanded(const bool isExpanded) {
	m_isExpanded = isExpanded;
	setChildVisible(isExpanded);
}

bool CUVNavigationNode::getIsExpanded() const {
	return m_isExpanded;
}

void CUVNavigationNode::setChildVisible(const bool isVisible) { // NOLINT
	for (const auto& node : m_childrenNodes) {
		node->setIsVisible(isVisible);
		if (isVisible && node->getIsExpanderNode() && !node->getIsExpanded()) {
			continue;
		}
		node->setChildVisible(isVisible);
	}
}

bool CUVNavigationNode::getIsHasChild() const {
	return m_childrenNodes.count() > 0;
}

bool CUVNavigationNode::getIsHasPageChild() const { // NOLINT
	if (m_childrenNodes.count() == 0) {
		return false;
	}

	for (const auto& node : m_childrenNodes) { // NOLINT
		if (!node->getIsExpanderNode()) {
			return true;
		}
		if (node->getIsHasPageChild()) {
			return true;
		}
	}

	return false;
}

void CUVNavigationNode::addChildNode(CUVNavigationNode* childNode) {
	if (m_isExpanderNode) {
		m_childrenNodes.append(childNode);
	}
}

bool CUVNavigationNode::getIsChildHasKeyPoints() const { // NOLINT
	for (const auto& node : m_childrenNodes) {           // NOLINT
		if (node->getKeyPoints()) {
			return true;
		}
		if (node->getIsChildHasKeyPoints()) {
			return true;
		}
	}

	return false;
}

CUVNavigationNode* CUVNavigationNode::getOriginalNode() {
	if (this->getParentNode()->getIsRootNode()) {
		return this;
	}

	CUVNavigationNode* originalNode = this->getParentNode();
	while (!originalNode->getIsRootNode() && !originalNode->getParentNode()->getIsRootNode()) {
		originalNode = originalNode->getParentNode();
	}
	return originalNode;
}

bool CUVNavigationNode::getIsChildNode(CUVNavigationNode* node) { // NOLINT
	if (m_childrenNodes.count() > 0) {
		if (m_childrenNodes.contains(node)) {
			return true;
		}
		for (const auto& childNode : m_childrenNodes) {
			if (childNode->getIsChildNode(node)) {
				return true;
			}
		}
	}

	return false;
}

int CUVNavigationNode::getRow() const {
	return m_parentNode ? m_parentNode->getChildrenNodes().indexOf(const_cast<CUVNavigationNode*>(this)) : 0;
}
