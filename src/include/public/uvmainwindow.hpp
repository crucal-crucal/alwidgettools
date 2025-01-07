#pragma once

#include <QMainWindow>

#include "alappbar.hpp"
#include "alwidgettoolsdef.hpp"

class CALMainWindowPrivate;

class CALWIDGETTOOLS_EXPORT CALMainWindow : public QMainWindow {
	Q_OBJECT
	Q_DECLARE_PRIVATE(CALMainWindow)
	Q_PROPERTY(bool isStayTop READ getIsStayTop WRITE setIsStayTop NOTIFY sigIsStayTopChanged)
	Q_PROPERTY(bool isFixedSize READ getIsFixedSize WRITE setIsFixedSize NOTIFY sigIsFixedSizeChanged)
	Q_PROPERTY(bool isDefaultClosed READ getIsDefaultClosed WRITE setIsDefaultClosed NOTIFY sigIsDefaultClosedChanged)
	Q_PROPERTY(bool isCenterStackedWidgetTransparent READ getIsCenterStackedWidgetTransparent WRITE setIsCenterStackedWidgetTransparent NOTIFY sigIsCenterStackedWidgetTransparentChanged)
	Q_PROPERTY(int appBarHeight READ getAppBarHeight WRITE setAppBarHeight NOTIFY sigAppBarHeightChanged)
	Q_PROPERTY(int customWidgetMaximumWidth READ getCustomWidgetMaximumWidth WRITE setCustomWidgetMaximumWidth NOTIFY sigCustomWidgetMaximumWidthChanged)
	Q_PROPERTY(int themeChangeTime READ getThemeChangeTime WRITE setThemeChangeTime NOTIFY sigThemeChangeTimeChanged)
	Q_PROPERTY(UVNavigationType::NavigationDisplayMode navigationDisplayMode READ getNavigationDisplayMode WRITE setNavigationDisplayMode NOTIFY sigNavigationDisplayModeChanged)
	Q_TAKEOVER_NATIVEEVENT_H

public:
	explicit CALMainWindow(QWidget* parent = nullptr);
	~CALMainWindow() override;

	void moveToCenter();

	void setCustomWidget(const ALAppBarType::CustomArea& customArea, QWidget* customWidget);
	[[nodiscard]] QWidget* getCustomWidget() const;

	void setIsNavigationBarEnable(bool isEnable);
	[[nodiscard]] bool getIsNavigationBarEnable() const;

	void setUserInfoCardVisible(bool isVisible);
	void setUserInfoCardPixmap(const QPixmap& pix);
	void setUserInfoCardTitle(const QString& title);
	void setUserInfoCardSubTitle(const QString& subTitle);

	ALNavigationType::NodeOperateReturnType addExpanderNode(const QString& expanderTitle, QString& expanderKey, const ALIcon::AweSomeIcon& awesomeIcon = ALIcon::AweSomeIcon::None) const;
	ALNavigationType::NodeOperateReturnType addExpanderNode(const QString& expanderTitle, QString& expanderKey, const QString& targetExpanderKey, const ALIcon::AweSomeIcon& awesomeIcon = ALIcon::AweSomeIcon::None) const;
	ALNavigationType::NodeOperateReturnType addPageNode(const QString& pageTitle, QWidget* page, const ALIcon::AweSomeIcon& awesomeIcon = ALIcon::AweSomeIcon::None) const;
	ALNavigationType::NodeOperateReturnType addPageNode(const QString& pageTitle, QWidget* page, const QString& targetExpanderKey, const ALIcon::AweSomeIcon& awesomeIcon = ALIcon::AweSomeIcon::None) const;
	ALNavigationType::NodeOperateReturnType addPageNode(const QString& pageTitle, QWidget* page, int keyPoints = 0, const ALIcon::AweSomeIcon& awesomeIcon = ALIcon::AweSomeIcon::None) const;
	ALNavigationType::NodeOperateReturnType addPageNode(const QString& pageTitle, QWidget* page, const QString& targetExpanderKey, int keyPoints = 0, const ALIcon::AweSomeIcon& awesomeIcon = ALIcon::AweSomeIcon::None) const;
	ALNavigationType::NodeOperateReturnType addFooterNode(const QString& footerTitle, QString& footerKey, int keyPoints = 0, const ALIcon::AweSomeIcon& awesomeIcon = ALIcon::AweSomeIcon::None) const;
	ALNavigationType::NodeOperateReturnType addFooterNode(const QString& footerTitle, QWidget* page, QString& footerKey, int keyPoints = 0, const ALIcon::AweSomeIcon& awesomeIcon = ALIcon::AweSomeIcon::None) const;

	void setNodeKeyPoints(const QString& nodeKey, int keyPoints);
	[[nodiscard]] int getNodeKeyPoints(const QString& nodeKey) const;

	void navigation(const QString& pageKey);
	void setWindowButtonFlag(const ALAppBarType::ButtonFlag& buttonFlag, bool isEnable = true);
	void setWindowButtonFlags(const ALAppBarType::ButtonFlags& buttonFlags);
	[[nodiscard]] ALAppBarType::ButtonFlags getWindowButtonFlags() const;

	bool insertWidgetBeforeButtonInAppBar(QWidget* widget, const ALAppBarType::ButtonFlag& flag);
	bool insertWidgetBeforeWidgetInAppBar(QWidget* widget, QWidget* targetWidget);
	bool insertWidgetBeforeLayoutInAppBar(QWidget* widget, QLayout* targetLayout);
	bool insertLayoutBeforeButtonInAppBar(QLayout* layout, const ALAppBarType::ButtonFlag& flag);
	bool insertLayoutBeforeWidgetInAppBar(QLayout* layout, QWidget* targetWidget);
	bool insertLayoutBeforeLayoutInAppBar(QLayout* layout, QLayout* targetLayout);

	virtual void closeWindow();

	void setIsStayTop(bool isStayTop);
	[[nodiscard]] bool getIsStayTop() const;

	void setIsFixedSize(bool isFixedSize);
	[[nodiscard]] bool getIsFixedSize() const;

	void setIsDefaultClosed(bool isDefaultClosed);
	[[nodiscard]] bool getIsDefaultClosed() const;

	void setIsCenterStackedWidgetTransparent(bool isTransparent);
	[[nodiscard]] bool getIsCenterStackedWidgetTransparent() const;

	void setAppBarHeight(int appBarHeight);
	[[nodiscard]] int getAppBarHeight() const;

	void setAppBarBackgroundColor(const QColor& appBarBackgroundColor);
	[[nodiscard]] QColor getAppBarBackgroundColor() const;

	void setCustomWidgetMaximumWidth(int width);
	[[nodiscard]] int getCustomWidgetMaximumWidth() const;

	void setThemeChangeTime(int time);
	[[nodiscard]] int getThemeChangeTime() const;

	void setNavigationDisplayMode(const ALNavigationType::NavigationDisplayMode& mode);
	[[nodiscard]] ALNavigationType::NavigationDisplayMode getNavigationDisplayMode() const;

Q_SIGNALS:
	Q_SIGNAL void sigUserInfoCardClicked();
	Q_SIGNAL void sigCloseButtonClicked();
	Q_SIGNAL void sigNavigationNodeClicked(const ALNavigationType::NavigationNodeType& nodeType, const QString& nodeKey);
	Q_SIGNAL void sigCustomWidgetChanged();
	Q_SIGNAL void sigIsStayTopChanged();
	Q_SIGNAL void sigIsFixedSizeChanged();
	Q_SIGNAL void sigIsDefaultClosedChanged();
	Q_SIGNAL void sigIsCenterStackedWidgetTransparentChanged();
	Q_SIGNAL void sigAppBarHeightChanged();
	Q_SIGNAL void sigCustomWidgetMaximumWidthChanged();
	Q_SIGNAL void sigThemeChangeTimeChanged();
	Q_SIGNAL void sigNavigationDisplayModeChanged();

protected:
	const QScopedPointer<CALMainWindowPrivate> d_ptr{ nullptr };

	bool eventFilter(QObject* watched, QEvent* event) override;
	QMenu* createPopupMenu() override;
};
