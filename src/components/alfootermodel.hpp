#pragma once

#include <QAbstractListModel>

#include "alwidgettoolsdef.hpp"

/**
 * @brief \namespace AL
 */
namespace AL {
class CALNavigationNode;

class CALFooterModel final : public QAbstractListModel {
	Q_OBJECT

public:
	explicit CALFooterModel(QObject* parent = nullptr);
	~CALFooterModel() override;

	ALNavigationType::NodeOperateReturnType addFooterNode(const QString& footerTitle, QString& footerKey, bool isHasFooterPage, int keyPoints = 0, const ALIcon::AweSomeIcon& awesomeicon = ALIcon::AweSomeIcon::None);
	ALNavigationType::NodeOperateReturnType addFooterNode(const QString& footerTitle, QString& footerKey, bool isHasFooterPage, int keyPoints = 0, const ALIcon::FluentIcon& fluenticon = ALIcon::FluentIcon::None);
	[[nodiscard]] int getFooterNodeCount() const;
	CALNavigationNode* getNavigationNode(const QString& footerKey);
	CALNavigationNode* getNavigationNode(const QModelIndex& index);
	bool removeNavigationNode(const QString& footerKey);

	void setSelectedNode(CALNavigationNode* node);
	[[nodiscard]] CALNavigationNode* getSelectedNode() const;

protected:
	[[nodiscard]] int rowCount(const QModelIndex& parent) const override;;
	[[nodiscard]] QVariant data(const QModelIndex& index, int role) const override;

private:
	CALNavigationNode* m_selectedNode{ nullptr };
	QList<CALNavigationNode*> m_listFooterNodes;
};
}
