#pragma once

#include <QMap>

#include "alnavigationmodel.hpp"
#include "alwidgettoolsdef.hpp"

class QLayout;
class QVBoxLayout;
class QHBoxLayout;
class QLinearGradient;

/**
 * @brief \namespace AL
 */
namespace AL {
class CALMenu;
class CALNavigationBar;
class CALNavigationNode;
class CALNavigationModel;
class CALNavigationView;
class CALSuggestBox;
class CALBaseListView;
class CALInteractiveCard;
class CALFooterModel;
class CALFooterDelegate;
class CALIconButton;
class CALToolButton;
class CALToolTip;

class CALNavigationBarPrivate final : public QObject {
	Q_OBJECT
	Q_DISABLE_COPY(CALNavigationBarPrivate);
	Q_DECLARE_PUBLIC(CALNavigationBar)
	Q_PROPERTY(int navigationViewWidth READ getNavigationViewWidth WRITE setNavigationViewWidth NOTIFY sigNavigationViewWidthChanged)

public:
	explicit CALNavigationBarPrivate(CALNavigationBar* q, QObject* parent = nullptr);
	~CALNavigationBarPrivate() override;

	void setNavigationViewWidth(int width);
	[[nodiscard]] int getNavigationViewWidth() const;

	Q_SLOT void slotNavigationButtonClicked();
	Q_SLOT void slotNavigationOpenNewWindow(const QString& nodeKey);

	Q_INVOKABLE void invokableNavigationRouteBack(const QVariantMap& routeData);

	void slotTreeViewClicked(const QModelIndex& index, bool isLogRoute = true);
	void slotFooterViewClicked(const QModelIndex& index, bool isLogRoute = true);

Q_SIGNALS:
	void sigNavigationViewWidthChanged();

protected:
	CALNavigationBar* const q_ptr{ nullptr };

private:
	bool isTransparent{};
	bool isShowUserCard{};
	int navigationViewWidth{};

	ALThemeType::ThemeMode themeMode{};
	ALNavigationType::NavigationDisplayMode currentDisplayMode{};
	QMap<QString, QString> mapSuggestKey{};
	QMap<QString, const QMetaObject*> mapPageMeta{};
	QMap<CALNavigationNode*, CALMenu*> mapCompactMenu{};
	QList<CALNavigationNode*> listLastExpandedNodes{};

	QVBoxLayout* navigationButtonVLayout{ nullptr };
	QHBoxLayout* navigationSuggestBoxHLayout{ nullptr };
	QVBoxLayout* userButtonVLayout{ nullptr };

	CALIconButton* userButton{ nullptr };
	CALToolButton* searchButton{ nullptr };
	CALToolButton* navigationButton{ nullptr };
	CALNavigationModel* navigationModel{ nullptr };
	CALNavigationView* navigationView{ nullptr };
	CALToolTip* footerToolTip{ nullptr };
	CALBaseListView* footerView{ nullptr };
	CALFooterModel* footerModel{ nullptr };
	CALFooterDelegate* footerDelegate{ nullptr };
	CALSuggestBox* navigationSuggestBox{ nullptr };
	CALInteractiveCard* userInfoCard{ nullptr };

	void resetNodeSelected() const;
	void expandSelectedNodeParent() const;
	void initNodeModelIndex(const QModelIndex& parentIndex);
	void addStackedPage(QWidget* page, const QString& pageKey);
	void addFooterPage(QWidget* page, const QString& footKey);
	void raiseNavigationBar();
	void doComponentAnimation(const ALNavigationType::NavigationDisplayMode& displayMode, bool isAnimation);
	void handleNavigationExpandState(bool isSave);
	void handleMaximalToCompactLayout() const;
	void handleCompactToMaximalLayout() const;
	void resetLayout() const;
	void doNavigationBarWidthAnimation(const ALNavigationType::NavigationDisplayMode& displayMode, bool isAnimation);
	void doNavigationViewWidthAnimation(bool isAnimation);
	void doNavigationButtonAnimation(bool isCompact, bool isAnimation);
	void doSearchButtonAnimation(bool isCompact, bool isAnimation) const;
	void doUserButtonAnimation(bool isCompact, bool isAnimation);
	void handleExpanderNodeInCompactMode(CALNavigationNode* node) const;                          // 处理紧凑模式下的展开节点
	void handleExpanderNodeInNormalMode(CALNavigationNode* node, const QModelIndex& index) const; // 处理普通模式下的展开节点
	void logNavigationRoute(const CALNavigationNode* selectedNode);                               // 记录导航路径
	void clearFooterSelection() const;                                                            // 清除页脚选择
	void updateNodeSelection(CALNavigationNode* node) const;                                      // 更新节点选择
	void handleCompactModeSelection(CALNavigationNode* node, QVariantMap& postData) const;        // 处理紧凑模式下的选择
	void handlePageNodeSelection(CALNavigationNode* node, bool isLogRoute);                       // 处理页面节点选择
};
}
