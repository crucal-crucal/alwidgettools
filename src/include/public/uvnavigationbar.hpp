#pragma once

#include <QWidget>

#include "uvwidgettoolsdef.hpp"

class CUVNavigationBarPrivate;

class CUVWIDGETTOOLS_EXPORT CUVNavigationBar : public QWidget {
	Q_OBJECT
	Q_DECLARE_PRIVATE(CUVNavigationBar)
	Q_PROPERTY(bool isTransparent READ getIsTransparent WRITE setIsTransparent NOTIFY sigIsTransparentChanged)

public:
	explicit CUVNavigationBar(QWidget* parent = nullptr);
	~CUVNavigationBar() override;
	void setUserInfoCardVisible(bool isVisible);
	void setUserInfoCardPixmap(const QPixmap& pix);
	void setUserInfoCardTitle(const QString& title);
	void setUserInfoCardSubTitle(const QString& subTitle);

	UVNavigationType::NodeOperateReturnType addExpanderNode(const QString& expanderTitle, QString& expanderKey, const UVIcon::CUVAweSomeIcon& awewomeIcon = UVIcon::CUVAweSomeIcon::None);
	UVNavigationType::NodeOperateReturnType addExpanderNode(const QString& expanderTitle, QString& expanderKey, const QString& targetExpanderKey, const UVIcon::CUVAweSomeIcon& awewomeIcon = UVIcon::CUVAweSomeIcon::None);
	UVNavigationType::NodeOperateReturnType addPageNode(const QString& pageTitle, QWidget* page, const UVIcon::CUVAweSomeIcon& awewomeIcon = UVIcon::CUVAweSomeIcon::None);
	UVNavigationType::NodeOperateReturnType addPageNode(const QString& pageTitle, QWidget* page, const QString& targetExpanderKey, const UVIcon::CUVAweSomeIcon& awewomeIcon = UVIcon::CUVAweSomeIcon::None);
	UVNavigationType::NodeOperateReturnType addPageNode(const QString& pageTitle, QWidget* page, int keyPoints = 0, const UVIcon::CUVAweSomeIcon& awewomeIcon = UVIcon::CUVAweSomeIcon::None);
	UVNavigationType::NodeOperateReturnType addPageNode(const QString& pageTitle, QWidget* page, const QString& targetExpanderKey, int keyPoints = 0, const UVIcon::CUVAweSomeIcon& awewomeIcon = UVIcon::CUVAweSomeIcon::None);
	UVNavigationType::NodeOperateReturnType addFooterNode(const QString& footerTitle, QString& footerKey, int keyPoints = 0, const UVIcon::CUVAweSomeIcon& awewomeIcon = UVIcon::CUVAweSomeIcon::None);
	UVNavigationType::NodeOperateReturnType addFooterNode(const QString& footerTitle, QWidget* page, QString& footerKey, int keyPoints = 0, const UVIcon::CUVAweSomeIcon& awewomeIcon = UVIcon::CUVAweSomeIcon::None);

	void setIsTransparent(bool isTransparent);
	[[nodiscard]] bool getIsTransparent() const;

	void setNodeKeyPoints(const QString& nodeKey, int keyPoints);
	[[nodiscard]] int getNodeKeyPoints(const QString& nodeKey) const;

	void navigation(const QString& pageKey, bool isLogClicked = true);
	void setDisplayMode(const UVNavigationType::NavigationDisplayMode& displayMode, bool isAnimation = true);

Q_SIGNALS:
	Q_SIGNAL void sigIsTransparentChanged();
	Q_SIGNAL void sigUserInfoCardClicked();
	Q_SIGNAL void sigNavigationNodeClicked(const UVNavigationType::NavigationNodeType& nodeType, const QString& nodeKey);
	Q_SIGNAL void sigNavigationNodeAdded(const UVNavigationType::NavigationNodeType& nodeType, const QString& nodeKey, QWidget* page);

protected:
	const QScopedPointer<CUVNavigationBarPrivate> d_ptr{ nullptr };

	void paintEvent(QPaintEvent* event) override;
};
