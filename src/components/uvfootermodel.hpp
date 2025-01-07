#pragma once

#include <QAbstractListModel>

#include "alwidgettoolsdef.hpp"

class CUVNavigationNode;

class CUVFooterModel final : public QAbstractListModel {
	Q_OBJECT

public:
	explicit CUVFooterModel(QObject* parent = nullptr);
	~CUVFooterModel() override;

	ALNavigationType::NodeOperateReturnType addFooterNode(const QString& footerTitle, QString& footerKey, bool isHasFooterPage, int keyPoints = 0, const ALIcon::AweSomeIcon& awesome = ALIcon::AweSomeIcon::None);
	[[nodiscard]] int getFooterNodeCount() const;
	CUVNavigationNode* getNavigationNode(const QString& footerKey);

	void setSelectedNode(CUVNavigationNode* node);
	[[nodiscard]] CUVNavigationNode* getSelectedNode() const;

protected:
	[[nodiscard]] int rowCount(const QModelIndex& parent) const override;;
	[[nodiscard]] QVariant data(const QModelIndex& index, int role) const override;

private:
	CUVNavigationNode* m_selectedNode{ nullptr };
	QList<CUVNavigationNode*> m_listFooterNodes;
};
