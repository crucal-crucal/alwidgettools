#pragma once

#include <QAbstractItemModel>

#include "alwidgettoolsdef.hpp"

/**
 * @brief \namespace AL
 */
namespace AL {
class CALNavigationNode;

class CALNavigationModel final : public QAbstractItemModel {
	Q_OBJECT

public:
	explicit CALNavigationModel(QObject* parent = nullptr);
	~CALNavigationModel() override;

	QModelIndex parent(const QModelIndex& child) const override;
	QModelIndex index(int row, int column, const QModelIndex& parent) const override;
	int rowCount(const QModelIndex& parent) const override;
	int columnCount(const QModelIndex& parent) const override;
	QVariant data(const QModelIndex& index, int role) const override;

	ALNavigationType::NodeOperateReturnType addExpanderNode(const QString& expanderTitle, QString& expanderKey, const ALIcon::AweSomeIcon& awewomeIcon);
	ALNavigationType::NodeOperateReturnType addExpanderNode(const QString& expanderTitle, QString& expanderKey, const ALIcon::FluentIcon& fluentIcon);
	ALNavigationType::NodeOperateReturnType addExpanderNode(const QString& expanderTitle, QString& expanderKey, const QString& targetExpanderKey, const ALIcon::AweSomeIcon& awewomeIcon);
	ALNavigationType::NodeOperateReturnType addExpanderNode(const QString& expanderTitle, QString& expanderKey, const QString& targetExpanderKey, const ALIcon::FluentIcon& fluentIcon);
	ALNavigationType::NodeOperateReturnType addPageNode(const QString& pageTitle, QString& pageKey, const ALIcon::AweSomeIcon& awewomeIcon);
	ALNavigationType::NodeOperateReturnType addPageNode(const QString& pageTitle, QString& pageKey, const ALIcon::FluentIcon& fluentIcon);
	ALNavigationType::NodeOperateReturnType addPageNode(const QString& pageTitle, QString& pageKey, const QString& targetExpanderKey, const ALIcon::AweSomeIcon& awewomeIcon);
	ALNavigationType::NodeOperateReturnType addPageNode(const QString& pageTitle, QString& pageKey, const QString& targetExpanderKey, const ALIcon::FluentIcon& fluentIcon);
	ALNavigationType::NodeOperateReturnType addPageNode(const QString& pageTitle, QString& pageKey, int keyPoints, const ALIcon::AweSomeIcon& awewomeIcon);
	ALNavigationType::NodeOperateReturnType addPageNode(const QString& pageTitle, QString& pageKey, int keyPoints, const ALIcon::FluentIcon& fluentIcon);
	ALNavigationType::NodeOperateReturnType addPageNode(const QString& pageTitle, QString& pageKey, const QString& targetExpanderKey, int keyPoints, const ALIcon::AweSomeIcon& awewomeIcon);
	ALNavigationType::NodeOperateReturnType addPageNode(const QString& pageTitle, QString& pageKey, const QString& targetExpanderKey, int keyPoints, const ALIcon::FluentIcon& fluentIcon);

	CALNavigationNode* getNavigationNode(const QString& nodeKey) const;
	QList<CALNavigationNode*> getRootExpanderNodes() const;
	QList<CALNavigationNode*> getRootExpandedNodes() const;

	void setSelectedNode(CALNavigationNode* node);
	[[nodiscard]] CALNavigationNode* getSelectedNode() const;

	void setSelectedExpandedNode(CALNavigationNode* node);
	[[nodiscard]] CALNavigationNode* getSelectedExpandedNode() const;

private:
	mutable QMap<QString, CALNavigationNode*> m_mapNodes{};
	CALNavigationNode* m_rootNode{ nullptr };
	CALNavigationNode* m_selectedNode{ nullptr };
	CALNavigationNode* m_selectedExpandedNode{ nullptr };
};
}
