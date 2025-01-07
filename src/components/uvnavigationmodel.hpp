#pragma once

#include <QAbstractItemModel>

#include "alwidgettoolsdef.hpp"

class CUVNavigationNode;

class CUVNavigationModel final : public QAbstractItemModel {
	Q_OBJECT

public:
	explicit CUVNavigationModel(QObject* parent = nullptr);
	~CUVNavigationModel() override;

	QModelIndex parent(const QModelIndex& child) const override;
	QModelIndex index(int row, int column, const QModelIndex& parent) const override;
	int rowCount(const QModelIndex& parent) const override;
	int columnCount(const QModelIndex& parent) const override;
	QVariant data(const QModelIndex& index, int role) const override;

	ALNavigationType::NodeOperateReturnType addExpanderNode(const QString& expanderTitle, QString& expanderKey, const ALIcon::AweSomeIcon& awewomeIcon);
	ALNavigationType::NodeOperateReturnType addExpanderNode(const QString& expanderTitle, QString& expanderKey, const QString& targetExpanderKey, const ALIcon::AweSomeIcon& awewomeIcon);
	ALNavigationType::NodeOperateReturnType addPageNode(const QString& pageTitle, QString& pageKey, const ALIcon::AweSomeIcon& awewomeIcon);
	ALNavigationType::NodeOperateReturnType addPageNode(const QString& pageTitle, QString& pageKey, const QString& targetExpanderKey, const ALIcon::AweSomeIcon& awewomeIcon);
	ALNavigationType::NodeOperateReturnType addPageNode(const QString& pageTitle, QString& pageKey, int keyPoints, const ALIcon::AweSomeIcon& awewomeIcon);
	ALNavigationType::NodeOperateReturnType addPageNode(const QString& pageTitle, QString& pageKey, const QString& targetExpanderKey, int keyPoints, const ALIcon::AweSomeIcon& awewomeIcon);

	CUVNavigationNode* getNavigationNode(const QString& nodeKey) const;
	QList<CUVNavigationNode*> getRootExpanderNodes() const;
	QList<CUVNavigationNode*> getRootExpandedNodes() const;

	void setSelectedNode(CUVNavigationNode* node);
	[[nodiscard]] CUVNavigationNode* getSelectedNode() const;

	void setSelectedExpandedNode(CUVNavigationNode* node);
	[[nodiscard]] CUVNavigationNode* getSelectedExpandedNode() const;

private:
	mutable QMap<QString, CUVNavigationNode*> m_mapNodes{};
	CUVNavigationNode* m_rootNode{ nullptr };
	CUVNavigationNode* m_selectedNode{ nullptr };
	CUVNavigationNode* m_selectedExpandedNode{ nullptr };
};
