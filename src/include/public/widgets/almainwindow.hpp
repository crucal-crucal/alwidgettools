#pragma once

#include <QMainWindow>

#include "alappbar.hpp"
#include "alwidgettoolsdef.hpp"

/**
 * @brief \namespace AL
 */
namespace AL {
class CALMainWindowPrivate;
class CALIconType;

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
	Q_PROPERTY(ALNavigationType::NavigationDisplayMode navigationDisplayMode READ getNavigationDisplayMode WRITE setNavigationDisplayMode NOTIFY sigNavigationDisplayModeChanged)
	AL_TAKEOVER_NATIVEEVENT_H

public:
	explicit CALMainWindow(QWidget* parent = nullptr);
	~CALMainWindow() override;

	/**
	 * @brief 将窗口移动到当前屏幕的中央
	 */
	void moveToCenter();

	/**
	 * @brief 为顶部 appBar 里添加自定义窗口
	 * @param customArea \enum ALAppBarType::CustomArea
	 * @param customWidget 自定义窗口
	 */
	void setCustomWidget(const ALAppBarType::CustomArea& customArea, QWidget* customWidget);
	[[nodiscard]] QWidget* getCustomWidget() const;

	/**
	 * @brief 设置是否启用导航栏
	 * @param isEnable 是否启用
	 */
	void setIsNavigationBarEnable(bool isEnable);
	[[nodiscard]] bool getIsNavigationBarEnable() const;

	/* 用户卡片接口 */
	void setUserInfoCardVisible(bool isVisible);
	void setUserInfoCardPixmap(const QPixmap& pix);
	void setUserInfoCardTitle(const QString& title);
	void setUserInfoCardSubTitle(const QString& subTitle);

	/**
	 * @brief 添加扩展节点
	 * @param expanderTitle 扩展节点 title
	 * @param expanderKey 扩展节点 key, 供给其他 node 使用的 targetExpanderKey
	 * @param icon_type 扩展节点图标类, 使用 CALIconFactory 创造
	 * @return \enum ALNavigationType::NodeOperateReturnType
	 */
	ALNavigationType::NodeOperateReturnType addExpanderNode(const QString& expanderTitle, QString& expanderKey, const std::shared_ptr<CALIconType>& icon_type = nullptr) const;

	/**
	 * @brief 添加扩展节点，并指定目标扩展节点键
	 * @param expanderTitle 扩展节点标题
	 * @param expanderKey 扩展节点键, 供给其他 node 使用的 targetExpanderKey
	 * @param targetExpanderKey 目标扩展节点键, ExpanderNode 的 expander Key
	 * @param icon_type 扩展节点图标类, 使用 CALIconFactory 创造
	 * @return \enum ALNavigationType::NodeOperateReturnType
	 */
	ALNavigationType::NodeOperateReturnType addExpanderNode(const QString& expanderTitle, QString& expanderKey, const QString& targetExpanderKey, const std::shared_ptr<CALIconType>& icon_type = nullptr) const;

	/**
	 * @brief 添加页面节点
	 * @param pageTitle 页面标题
	 * @param page 页面控件
	 * @param icon_type 扩展节点图标类, 使用 CALIconFactory 创造
	 * @return \enum ALNavigationType::NodeOperateReturnType
	 */
	ALNavigationType::NodeOperateReturnType addPageNode(const QString& pageTitle, QWidget* page, const std::shared_ptr<CALIconType>& icon_type = nullptr) const;

	/**
	 * @brief 添加页面节点，并指定目标扩展节点键
	 * @param pageTitle 页面标题
	 * @param page 页面控件
	 * @param targetExpanderKey 目标扩展节点键, ExpanderNode 的 expander Key
	 * @param icon_type 扩展节点图标类, 使用 CALIconFactory 创造
	 * @return \enum ALNavigationType::NodeOperateReturnType
	 */
	ALNavigationType::NodeOperateReturnType addPageNode(const QString& pageTitle, QWidget* page, const QString& targetExpanderKey, const std::shared_ptr<CALIconType>& icon_type = nullptr) const;

	/**
	 * @brief 添加页面节点，并设置关键点数
	 * @param pageTitle 页面标题
	 * @param page 页面控件
	 * @param keyPoints 节点关键点数, 小于等于 0 不会显示, 超过 99 则会显示 99+
	 * @param icon_type 扩展节点图标类, 使用 CALIconFactory 创造
	 * @return \enum ALNavigationType::NodeOperateReturnType
	 */
	ALNavigationType::NodeOperateReturnType addPageNode(const QString& pageTitle, QWidget* page, int keyPoints, const std::shared_ptr<CALIconType>& icon_type = nullptr) const;

	/**
	 * @brief 添加页面节点，并指定目标扩展节点键和关键点数
	 * @param pageTitle 页面标题
	 * @param page 页面控件
	 * @param targetExpanderKey 目标扩展节点键, ExpanderNode 的 expander Key
	 * @param keyPoints 节点关键点数, 小于等于 0 不会显示, 超过 99 则会显示 99+
	 * @param icon_type 扩展节点图标类, 使用 CALIconFactory 创造
	 * @return \enum ALNavigationType::NodeOperateReturnType
	 */
	ALNavigationType::NodeOperateReturnType addPageNode(const QString& pageTitle, QWidget* page, const QString& targetExpanderKey, int keyPoints = 0, const std::shared_ptr<CALIconType>& icon_type = nullptr) const;

	/**
	 * @brief 添加页脚节点
	 * @param footerTitle 页脚标题
	 * @param footerKey 页脚键
	 * @param keyPoints 节点关键点数, 小于等于 0 不会显示, 超过 99 则会显示 99+
	 * @param icon_type 扩展节点图标类, 使用 CALIconFactory 创造
	 * @return \enum ALNavigationType::NodeOperateReturnType
	 */
	ALNavigationType::NodeOperateReturnType addFooterNode(const QString& footerTitle, QString& footerKey, int keyPoints, const std::shared_ptr<CALIconType>& icon_type = nullptr) const;

	/**
	 * @brief 添加页脚节点，并指定页面控件
	 * @param footerTitle 页脚标题
	 * @param page 页面控件
	 * @param footerKey 页脚键
	 * @param keyPoints 节点关键点数, 小于等于 0 不会显示, 超过 99 则会显示 99+
	 * @param icon_type 扩展节点图标类, 使用 CALIconFactory 创造
	 * @return \enum ALNavigationType::NodeOperateReturnType
	 */
	ALNavigationType::NodeOperateReturnType addFooterNode(const QString& footerTitle, QWidget* page, QString& footerKey, int keyPoints, const std::shared_ptr<CALIconType>& icon_type = nullptr) const;

	/**
	 * @brief 删除节点
	 * @param nodeKey 添加节点时获取到的节点 key
	 * @return 是否删除成功
	 */
	bool removeNavigationNode(const QString& nodeKey);

	/**
	 * @brief 设置节点关键点数
	 * @param nodeKey 节点 key
	 * @param keyPoints 关键点数
	 */
	void setNodeKeyPoints(const QString& nodeKey, int keyPoints);
	[[nodiscard]] int getNodeKeyPoints(const QString& nodeKey) const;

	/**
	 * @brief 导航到指定页面
	 * @param pageKey 添加页面时的节点 key 或者 页面的 property("CALPageKey").toString()
	 */
	void navigation(const QString& pageKey);

	/**
	 * @brief 设置 AppBar 按钮是否启用
	 * @param buttonFlag \enum ALAppBarType::ButtonFlag
	 * @param isEnable 是否启用
	 */
	void setWindowButtonFlag(const ALAppBarType::ButtonFlag& buttonFlag, bool isEnable = true);
	void setWindowButtonFlags(const ALAppBarType::ButtonFlags& buttonFlags);
	[[nodiscard]] ALAppBarType::ButtonFlags getWindowButtonFlags() const;

	/**
	 * @brief 在 AppBar 的某一个按钮前面添加自定义界面
	 * @param widget 自定义界面
	 * @param flag \enum ALAppBarType::ButtonFlag
	 * @return 是否添加成功
	 */
	bool insertWidgetBeforeButtonInAppBar(QWidget* widget, const ALAppBarType::ButtonFlag& flag);

	/**
	 * @brief 在 AppBar 的某一个控件前面添加自定义界面
	 * @param widget 自定义界面
	 * @param targetWidget 目标控件
	 * @return 是否添加成功
	 */
	bool insertWidgetBeforeWidgetInAppBar(QWidget* widget, QWidget* targetWidget);

	/**
	 * @brief 在 AppBar 的某一个布局前面添加自定义界面
	 * @param widget 自定义界面
	 * @param targetLayout 目标布局
	 * @return 是否添加成功
	 */
	bool insertWidgetBeforeLayoutInAppBar(QWidget* widget, QLayout* targetLayout);

	/**
	 * @brief 在 AppBar 的某一个按钮前面添加一个自定义布局
	 * @param layout 自定义布局
	 * @param flag \enum ALAppBarType::ButtonFlag
	 * @return 是否添加成功
	 */
	bool insertLayoutBeforeButtonInAppBar(QLayout* layout, const ALAppBarType::ButtonFlag& flag);

	/**
	 * @brief 在 AppBar 的某一个 widget 前面添加一个自定义布局
	 * @param layout 自定义布局
	 * @param targetWidget 目标 widget
	 * @return 是否添加成功
	 */
	bool insertLayoutBeforeWidgetInAppBar(QLayout* layout, QWidget* targetWidget);

	/**
	 * @brief 在 AppBar 的某一个布局前面添加一个自定义布局
	 * @param layout 自定义布局
	 * @param targetLayout 目标布局
	 * @return 是否添加成功
	 */
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

	static void setThemeMode(const ALThemeType::ThemeMode& mode);
	[[nodiscard]] static ALThemeType::ThemeMode getThemeMode();

Q_SIGNALS:
	void sigUserInfoCardClicked();
	void sigCloseButtonClicked();
	void sigNavigationNodeClicked(const ALNavigationType::NavigationNodeType& nodeType, const QString& nodeKey);
	void sigCustomWidgetChanged();
	void sigIsStayTopChanged();
	void sigIsFixedSizeChanged();
	void sigIsDefaultClosedChanged();
	void sigIsCenterStackedWidgetTransparentChanged();
	void sigAppBarHeightChanged();
	void sigCustomWidgetMaximumWidthChanged();
	void sigThemeChangeTimeChanged();
	void sigNavigationDisplayModeChanged(const ALNavigationType::NavigationDisplayMode& displayMode);

protected:
	const QScopedPointer<CALMainWindowPrivate> d_ptr{ nullptr };

	bool eventFilter(QObject* watched, QEvent* event) override;
	QMenu* createPopupMenu() override;
};
}
