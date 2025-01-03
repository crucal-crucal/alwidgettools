#include "uvfootermodel.hpp"

#include "uvnavigationnode.hpp"

#define FOOTER_NODE_LIMIT 3

/**
 * @brief \class CUVFooterModel
 * @param parent pointer to the parent class
 */
CUVFooterModel::CUVFooterModel(QObject* parent): QAbstractListModel(parent) {
}

CUVFooterModel::~CUVFooterModel() {
	qDeleteAll(m_listFooterNodes);
}

UVNavigationType::NodeOperateReturnType CUVFooterModel::addFooterNode(const QString& footerTitle, QString& footerKey, const bool isHasFooterPage, const int keyPoints, const UVIcon::CUVAweSomeIcon& awesome) {
	if (m_listFooterNodes.count() >= FOOTER_NODE_LIMIT) {
		return UVNavigationType::FooterUpperLimit;
	}

	const auto node = new CUVNavigationNode(footerTitle);
	node->setKeyPoints(keyPoints);
	node->setIsFooterNode(true);
	node->setIsHasFooterNode(isHasFooterPage);
	node->setAwesomeIcon(awesome);
	beginResetModel();
	m_listFooterNodes.append(node);
	endResetModel();
	footerKey = node->getNodeKey();
	node->setModelIndex(this->index(m_listFooterNodes.count() - 1));

	return UVNavigationType::Success;
}

int CUVFooterModel::getFooterNodeCount() const {
	return m_listFooterNodes.count();
}

CUVNavigationNode* CUVFooterModel::getNavigationNode(const QString& footerKey) {
	CUVNavigationNode* resNode{ nullptr };

	for (const auto& node : m_listFooterNodes) {
		if (node->getNodeKey() == footerKey) {
			resNode = node;
			break;
		}
	}

	return resNode;
}

void CUVFooterModel::setSelectedNode(CUVNavigationNode* node) {
	m_selectedNode = node;
}

CUVNavigationNode* CUVFooterModel::getSelectedNode() const {
	return m_selectedNode;
}

int CUVFooterModel::rowCount(const QModelIndex& parent) const {
	return m_listFooterNodes.count();
}

QVariant CUVFooterModel::data(const QModelIndex& index, const int role) const {
	if (role == Qt::UserRole && index.row() < m_listFooterNodes.count()) {
		return QVariant::fromValue(m_listFooterNodes[index.row()]);
	}

	return {};
}
