#include "alnavigationnode.hpp"

#include <QUuid>

/**
 * @brief \namespace AL
 */
namespace AL {
/**
 * @brief \class CALNavigationNode
 * @param nodeTitle title of the node
 * @param parent pointer to the parent class
 */
CALNavigationNode::CALNavigationNode(const QString& nodeTitle, CALNavigationNode* parent): QObject(parent) {
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

CALNavigationNode::~CALNavigationNode() {
	qDeleteAll(m_childrenNodes);
}

void CALNavigationNode::setParentNode(CALNavigationNode* parentNode) {
	m_parentNode = parentNode;
}

CALNavigationNode* CALNavigationNode::getParentNode() const {
	return m_parentNode;
}

void CALNavigationNode::setChildrenNodes(const QList<CALNavigationNode*>& childrenNodes) {
	m_childrenNodes = childrenNodes;
	Q_EMIT sigChildrenNodesChanged();
}

QList<CALNavigationNode*> CALNavigationNode::getChildrenNodes() const {
	return m_childrenNodes;
}

void CALNavigationNode::setAwesomeIcon(const ALIcon::AweSomeIcon& awesomeIcon) {
	m_awesomeIcon = awesomeIcon;
	Q_EMIT sigAwesomeIconChanged();
}

ALIcon::AweSomeIcon CALNavigationNode::getAwesomeIcon() const {
	return m_awesomeIcon;
}

void CALNavigationNode::setModelIndex(const QModelIndex& modelIndex) {
	m_modelIndex = modelIndex;
	Q_EMIT sigModelIndexChanged();
}

QModelIndex CALNavigationNode::getModelIndex() const {
	return m_modelIndex;
}

void CALNavigationNode::setKeyPoints(const int keyPoints) {
	m_keyPoints = keyPoints;
	Q_EMIT sigKeyPointsChanged();
}

int CALNavigationNode::getKeyPoints() const {
	return m_keyPoints;
}

void CALNavigationNode::setDepth(const int depth) {
	m_depth = depth;
	Q_EMIT sigDepthChanged();
}

int CALNavigationNode::getDepth() const {
	return m_depth;
}

void CALNavigationNode::setIsRootNode(const bool isRootNode) {
	m_isRootNode = isRootNode;
	Q_EMIT sigIsRootNodeChanged();
}

bool CALNavigationNode::getIsRootNode() const {
	return m_isRootNode;
}

void CALNavigationNode::setIsFooterNode(const bool isFooterNode) {
	m_isFooterNode = isFooterNode;
	Q_EMIT sigIsFooterNodeChanged();
}

bool CALNavigationNode::getIsFooterNode() const {
	return m_isFooterNode;
}

void CALNavigationNode::setIsHasFooterNode(const bool isHasFooterNode) {
	m_isHasFooterNode = isHasFooterNode;
	Q_EMIT sigIsHasFooterNodeChanged();
}

bool CALNavigationNode::getIsHasFooterNode() const {
	return m_isHasFooterNode;
}

void CALNavigationNode::setIsExpanderNode(const bool isExpanderNode) {
	m_isExpanderNode = isExpanderNode;
	Q_EMIT sigIsExpanderNodeChanged();
}

bool CALNavigationNode::getIsExpanderNode() const {
	return m_isExpanderNode;
}

void CALNavigationNode::setIsVisible(const bool isVisible) {
	m_isVisible = isVisible;
	Q_EMIT sigIsVisibleChanged();
}

bool CALNavigationNode::getIsVisible() const {
	return m_isVisible;
}

QString CALNavigationNode::getNodeKey() const {
	return m_nodeKey;
}

QString CALNavigationNode::getNodeTitle() const {
	return m_nodeTitle;
}

void CALNavigationNode::setIsExpanded(const bool isExpanded) {
	m_isExpanded = isExpanded;
	setChildVisible(isExpanded);
}

bool CALNavigationNode::getIsExpanded() const {
	return m_isExpanded;
}

void CALNavigationNode::setChildVisible(const bool isVisible) { // NOLINT
	for (const auto& node : m_childrenNodes) {
		node->setIsVisible(isVisible);
		if (isVisible && node->getIsExpanderNode() && !node->getIsExpanded()) {
			continue;
		}
		node->setChildVisible(isVisible);
	}
}

bool CALNavigationNode::getIsHasChild() const {
	return m_childrenNodes.count() > 0;
}

bool CALNavigationNode::getIsHasPageChild() const { // NOLINT
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

void CALNavigationNode::addChildNode(CALNavigationNode* childNode) {
	if (m_isExpanderNode) {
		m_childrenNodes.append(childNode);
	}
}

bool CALNavigationNode::getIsChildHasKeyPoints() const { // NOLINT
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

CALNavigationNode* CALNavigationNode::getOriginalNode() {
	if (this->getParentNode()->getIsRootNode()) {
		return this;
	}

	CALNavigationNode* originalNode = this->getParentNode();
	while (!originalNode->getIsRootNode() && !originalNode->getParentNode()->getIsRootNode()) {
		originalNode = originalNode->getParentNode();
	}
	return originalNode;
}

bool CALNavigationNode::getIsChildNode(CALNavigationNode* node) { // NOLINT
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

int CALNavigationNode::getRow() const {
	return m_parentNode ? m_parentNode->getChildrenNodes().indexOf(const_cast<CALNavigationNode*>(this)) : 0;
}
}
