#include "alfootermodel.hpp"

#include "alnavigationnode.hpp"

#define FOOTER_NODE_LIMIT 3

/**
 * @brief \namespace AL
 */
namespace AL {
/**
 * @brief \class CALFooterModel
 * @param parent pointer to the parent class
 */
CALFooterModel::CALFooterModel(QObject* parent): QAbstractListModel(parent) {
}

CALFooterModel::~CALFooterModel() {
	qDeleteAll(m_listFooterNodes);
}

ALNavigationType::NodeOperateReturnType CALFooterModel::addFooterNode(const QString& footerTitle, QString& footerKey, const bool isHasFooterPage, const int keyPoints, const ALIcon::AweSomeIcon& awesomeicon) {
	if (m_listFooterNodes.count() >= FOOTER_NODE_LIMIT) {
		return ALNavigationType::FooterUpperLimit;
	}

	const auto node = new CALNavigationNode(footerTitle);
	node->setKeyPoints(keyPoints);
	node->setIsFooterNode(true);
	node->setIsHasFooterNode(isHasFooterPage);
	node->setAwesomeIcon(awesomeicon);
	beginResetModel();
	m_listFooterNodes.append(node);
	endResetModel();
	footerKey = node->getNodeKey();
	node->setModelIndex(this->index(m_listFooterNodes.count() - 1));

	return ALNavigationType::Success;
}

ALNavigationType::NodeOperateReturnType CALFooterModel::addFooterNode(const QString& footerTitle, QString& footerKey, const bool isHasFooterPage, const int keyPoints, const ALIcon::FluentIcon& fluenticon) {
	if (m_listFooterNodes.count() >= FOOTER_NODE_LIMIT) {
		return ALNavigationType::FooterUpperLimit;
	}

	const auto node = new CALNavigationNode(footerTitle);
	node->setKeyPoints(keyPoints);
	node->setIsFooterNode(true);
	node->setIsHasFooterNode(isHasFooterPage);
	node->setFluentIcon(fluenticon);
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

CALNavigationNode* CALFooterModel::getNavigationNode(const QModelIndex& index) {
	if (!index.isValid()) {
		return nullptr;
	}

	const int row = index.row();
	if (row < 0 || row >= m_listFooterNodes.count()) {
		return nullptr;
	}

	return m_listFooterNodes.at(row);
}

bool CALFooterModel::removeNavigationNode(const QString& footerKey) {
	bool bRet{ false };

	for (auto& node : m_listFooterNodes) {
		if (node->getNodeKey() == footerKey) {
			bRet = m_listFooterNodes.removeOne(node);
			break;
		}
	}

	return bRet;
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
}
