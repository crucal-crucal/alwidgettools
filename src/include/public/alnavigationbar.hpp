#pragma once

#include <QWidget>

#include "alwidgettoolsdef.hpp"

class CALNavigationBarPrivate;

class CALWIDGETTOOLS_EXPORT CALNavigationBar : public QWidget {
	Q_OBJECT
	Q_DECLARE_PRIVATE(CALNavigationBar)
	Q_PROPERTY(bool isTransparent READ getIsTransparent WRITE setIsTransparent NOTIFY sigIsTransparentChanged)

public:
	explicit CALNavigationBar(QWidget* parent = nullptr);
	~CALNavigationBar() override;
	void setUserInfoCardVisible(bool isVisible);
	void setUserInfoCardPixmap(const QPixmap& pix);
	void setUserInfoCardTitle(const QString& title);
	void setUserInfoCardSubTitle(const QString& subTitle);

	ALNavigationType::NodeOperateReturnType addExpanderNode(const QString& expanderTitle, QString& expanderKey, const ALIcon::AweSomeIcon& awewomeIcon = ALIcon::AweSomeIcon::None);
	ALNavigationType::NodeOperateReturnType addExpanderNode(const QString& expanderTitle, QString& expanderKey, const QString& targetExpanderKey, const ALIcon::AweSomeIcon& awewomeIcon = ALIcon::AweSomeIcon::None);
	ALNavigationType::NodeOperateReturnType addPageNode(const QString& pageTitle, QWidget* page, const ALIcon::AweSomeIcon& awewomeIcon = ALIcon::AweSomeIcon::None);
	ALNavigationType::NodeOperateReturnType addPageNode(const QString& pageTitle, QWidget* page, const QString& targetExpanderKey, const ALIcon::AweSomeIcon& awewomeIcon = ALIcon::AweSomeIcon::None);
	ALNavigationType::NodeOperateReturnType addPageNode(const QString& pageTitle, QWidget* page, int keyPoints = 0, const ALIcon::AweSomeIcon& awewomeIcon = ALIcon::AweSomeIcon::None);
	ALNavigationType::NodeOperateReturnType addPageNode(const QString& pageTitle, QWidget* page, const QString& targetExpanderKey, int keyPoints = 0, const ALIcon::AweSomeIcon& awewomeIcon = ALIcon::AweSomeIcon::None);
	ALNavigationType::NodeOperateReturnType addFooterNode(const QString& footerTitle, QString& footerKey, int keyPoints = 0, const ALIcon::AweSomeIcon& awewomeIcon = ALIcon::AweSomeIcon::None);
	ALNavigationType::NodeOperateReturnType addFooterNode(const QString& footerTitle, QWidget* page, QString& footerKey, int keyPoints = 0, const ALIcon::AweSomeIcon& awewomeIcon = ALIcon::AweSomeIcon::None);

	void setIsTransparent(bool isTransparent);
	[[nodiscard]] bool getIsTransparent() const;

	void setNodeKeyPoints(const QString& nodeKey, int keyPoints);
	[[nodiscard]] int getNodeKeyPoints(const QString& nodeKey) const;

	void navigation(const QString& pageKey, bool isLogClicked = true);
	void setDisplayMode(const ALNavigationType::NavigationDisplayMode& displayMode, bool isAnimation = true);

Q_SIGNALS:
	Q_SIGNAL void sigIsTransparentChanged();
	Q_SIGNAL void sigUserInfoCardClicked();
	Q_SIGNAL void sigNavigationNodeClicked(const ALNavigationType::NavigationNodeType& nodeType, const QString& nodeKey);
	Q_SIGNAL void sigNavigationNodeAdded(const ALNavigationType::NavigationNodeType& nodeType, const QString& nodeKey, QWidget* page);

protected:
	const QScopedPointer<CALNavigationBarPrivate> d_ptr{ nullptr };

	void paintEvent(QPaintEvent* event) override;
};
