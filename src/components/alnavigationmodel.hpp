#pragma once

#include <memory>
#include <optional>
#include <QAbstractItemModel>
#include <variant>

#include "alnavigationbar_p.hpp"
#include "alwidgettoolsdef.hpp"

/**
 * @brief \namespace AL
 */
namespace AL {
class CALNavigationNode;
class CALIconType;

class CALNavigationModel final : public QAbstractItemModel {
	Q_OBJECT

public:
	explicit CALNavigationModel(QObject* parent = nullptr);
	~CALNavigationModel() override;

	[[nodiscard]] QModelIndex parent(const QModelIndex& child) const override;
	[[nodiscard]] QModelIndex index(int row, int column, const QModelIndex& parent) const override;
	[[nodiscard]] int rowCount(const QModelIndex& parent) const override;
	[[nodiscard]] int columnCount(const QModelIndex& parent) const override;
	[[nodiscard]] QVariant data(const QModelIndex& index, int role) const override;

	ALNavigationType::NodeOperateReturnType addExpanderNode(const QString& expanderTitle, QString& expanderKey, const std::shared_ptr<CALIconType>& icon_type);
	ALNavigationType::NodeOperateReturnType addExpanderNode(const QString& expanderTitle, QString& expanderKey, const QString& targetExpanderKey, const std::shared_ptr<CALIconType>& icon_type);
	ALNavigationType::NodeOperateReturnType addPageNode(const QString& pageTitle, QString& pageKey, const std::shared_ptr<CALIconType>& icon_type);
	ALNavigationType::NodeOperateReturnType addPageNode(const QString& pageTitle, QString& pageKey, const QString& targetExpanderKey, const std::shared_ptr<CALIconType>& icon_type);
	ALNavigationType::NodeOperateReturnType addPageNode(const QString& pageTitle, QString& pageKey, int keyPoints, const std::shared_ptr<CALIconType>& icon_type);
	ALNavigationType::NodeOperateReturnType addPageNode(const QString& pageTitle, QString& pageKey, const QString& targetExpanderKey, int keyPoints, const std::shared_ptr<CALIconType>& icon_type);

	QStringList removeNavigationNode(const QString& nodeKey);

	[[nodiscard]] CALNavigationNode* getNavigationNode(const QString& nodeKey) const;
	[[nodiscard]] QList<CALNavigationNode*> getRootExpanderNodes() const;
	[[nodiscard]] QList<CALNavigationNode*> getRootExpandedNodes() const;

	void setSelectedNode(CALNavigationNode* node);
	[[nodiscard]] CALNavigationNode* getSelectedNode() const;

	void setSelectedExpandedNode(CALNavigationNode* node);
	[[nodiscard]] CALNavigationNode* getSelectedExpandedNode() const;

private:
	QMap<QString, CALNavigationNode*> m_mapNodes{};
	CALNavigationNode* m_rootNode{ nullptr };
	CALNavigationNode* m_selectedNode{ nullptr };
	CALNavigationNode* m_selectedExpandedNode{ nullptr };

	ALNavigationType::NodeOperateReturnType addNodeInternal(CALNavigationNode* parentNode, const QString& title, QString& outKey, bool isExpander, const std::shared_ptr<CALIconType>& icon_type, int keyPoints = 0);
	void addNodeToModel(CALNavigationNode* node, CALNavigationNode* parentNode);
};
}
