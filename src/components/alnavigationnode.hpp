#pragma once

#include <QModelIndex>

#include "alwidgettoolsdef.hpp"

namespace AL {
class CALNavigationNode final : public QObject {
	Q_OBJECT
	Q_PROPERTY(QList<CALNavigationNode*> childrenNodes READ getChildrenNodes WRITE setChildrenNodes NOTIFY sigChildrenNodesChanged)
	Q_PROPERTY(ALIcon::AweSomeIcon awesomeIcon READ getAwesomeIcon WRITE setAwesomeIcon NOTIFY sigAwesomeIconChanged)
	Q_PROPERTY(QModelIndex modelIndex READ getModelIndex WRITE setModelIndex NOTIFY sigModelIndexChanged)
	Q_PROPERTY(int keyPoints READ getKeyPoints WRITE setKeyPoints NOTIFY sigKeyPointsChanged)
	Q_PROPERTY(int depth READ getDepth WRITE setDepth NOTIFY sigDepthChanged)
	Q_PROPERTY(bool isRootNode READ getIsRootNode WRITE setIsRootNode NOTIFY sigIsRootNodeChanged)
	Q_PROPERTY(bool isFooterNode READ getIsFooterNode WRITE setIsFooterNode NOTIFY sigIsFooterNodeChanged)
	Q_PROPERTY(bool isHasFooterNode READ getIsHasFooterNode WRITE setIsHasFooterNode NOTIFY sigIsHasFooterNodeChanged)
	Q_PROPERTY(bool isExpanderNode READ getIsExpanderNode WRITE setIsExpanderNode NOTIFY sigIsExpanderNodeChanged)
	Q_PROPERTY(bool isVisible READ getIsVisible WRITE setIsVisible NOTIFY sigIsVisibleChanged)

public:
	explicit CALNavigationNode(const QString& nodeTitle, CALNavigationNode* parent = nullptr);
	~CALNavigationNode() override;

	void setParentNode(CALNavigationNode* parentNode);
	[[nodiscard]] CALNavigationNode* getParentNode() const;

	void setChildrenNodes(const QList<CALNavigationNode*>& childrenNodes);
	[[nodiscard]] QList<CALNavigationNode*> getChildrenNodes() const;

	void setAwesomeIcon(const ALIcon::AweSomeIcon& awesomeIcon);
	[[nodiscard]] ALIcon::AweSomeIcon getAwesomeIcon() const;

	void setModelIndex(const QModelIndex& modelIndex);
	[[nodiscard]] QModelIndex getModelIndex() const;

	void setKeyPoints(int keyPoints);
	[[nodiscard]] int getKeyPoints() const;

	void setDepth(int depth);
	[[nodiscard]] int getDepth() const;

	void setIsRootNode(bool isRootNode);
	[[nodiscard]] bool getIsRootNode() const;

	void setIsFooterNode(bool isFooterNode);
	[[nodiscard]] bool getIsFooterNode() const;

	void setIsHasFooterNode(bool isHasFooterNode);
	[[nodiscard]] bool getIsHasFooterNode() const;

	void setIsExpanderNode(bool isExpanderNode);
	[[nodiscard]] bool getIsExpanderNode() const;

	void setIsVisible(bool isVisible);
	[[nodiscard]] bool getIsVisible() const;

	[[nodiscard]] QString getNodeKey() const;
	[[nodiscard]] QString getNodeTitle() const;

	void setIsExpanded(bool isExpanded);
	[[nodiscard]] bool getIsExpanded() const;

	void setChildVisible(bool isVisible);
	[[nodiscard]] bool getIsHasChild() const;
	[[nodiscard]] bool getIsHasPageChild() const;

	void addChildNode(CALNavigationNode* childNode);

	[[nodiscard]] bool getIsChildHasKeyPoints() const;

	CALNavigationNode* getOriginalNode();
	bool getIsChildNode(CALNavigationNode* node);

	[[nodiscard]] int getRow() const;

Q_SIGNALS:
	Q_SIGNAL void sigChildrenNodesChanged();
	Q_SIGNAL void sigAwesomeIconChanged();
	Q_SIGNAL void sigModelIndexChanged();
	Q_SIGNAL void sigKeyPointsChanged();
	Q_SIGNAL void sigDepthChanged();
	Q_SIGNAL void sigIsRootNodeChanged();
	Q_SIGNAL void sigIsFooterNodeChanged();
	Q_SIGNAL void sigIsHasFooterNodeChanged();
	Q_SIGNAL void sigIsExpanderNodeChanged();
	Q_SIGNAL void sigIsVisibleChanged();

private:
	CALNavigationNode* m_parentNode{ nullptr };
	QList<CALNavigationNode*> m_childrenNodes{};
	ALIcon::AweSomeIcon m_awesomeIcon{};
	QModelIndex m_modelIndex{};
	int m_keyPoints{};
	int m_depth{};
	bool m_isRootNode{};
	bool m_isFooterNode{};
	bool m_isHasFooterNode{};
	bool m_isExpanderNode{};
	bool m_isVisible{};
	bool m_isExpanded{};
	QString m_nodeKey{};
	QString m_nodeTitle{};
};
} // namespace AL