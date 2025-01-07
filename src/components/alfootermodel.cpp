#include "alfootermodel.hpp"

#include "alnavigationnode.hpp"

#define FOOTER_NODE_LIMIT 3

/**
 * @brief \class CALFooterModel
 * @param parent pointer to the parent class
 */
CALFooterModel::CALFooterModel(QObject* parent): QAbstractListModel(parent) {
}

CALFooterModel::~CALFooterModel() {
	qDeleteAll(m_listFooterNodes);
}

ALNavigationType::NodeOperateReturnType CALFooterModel::addFooterNode(const QString& footerTitle, QString& footerKey, const bool isHasFooterPage, const int keyPoints, const ALIcon::AweSomeIcon& awesome) {
	if (m_listFooterNodes.count() >= FOOTER_NODE_LIMIT) {
		return ALNavigationType::FooterUpperLimit;
	}

	const auto node = new CALNavigationNode(footerTitle);
	node->setKeyPoints(keyPoints);
	node->setIsFooterNode(true);
	node->setIsHasFooterNode(isHasFooterPage);
	node->setAwesomeIcon(awesome);
	beginResetModel();
	m_listFooterNodes.append(node);
	endResetModel();
	footerKey = node->getNodeKey();
	node->setModelIndex(this->index(m_listFooterNodes.count() - 1));

	return ALNavigationType::Success;
}

int CALFooterModel::getFooterNodeCount() const {
	return m_listFooterNodes.count();
}

CALNavigationNode* CALFooterModel::getNavigationNode(const QString& footerKey) {
	CALNavigationNode* resNode{ nullptr };

	for (const auto& node : m_listFooterNodes) {
		if (node->getNodeKey() == footerKey) {
			resNode = node;
			break;
		}
	}

	return resNode;
}

void CALFooterModel::setSelectedNode(CALNavigationNode* node) {
	m_selectedNode = node;
}

CALNavigationNode* CALFooterModel::getSelectedNode() const {
	return m_selectedNode;
}

int CALFooterModel::rowCount(const QModelIndex& parent) const {
	return m_listFooterNodes.count();
}

QVariant CALFooterModel::data(const QModelIndex& index, const int role) const {
	if (role == Qt::UserRole && index.row() < m_listFooterNodes.count()) {
		return QVariant::fromValue(m_listFooterNodes[index.row()]);
	}

	return {};
}
