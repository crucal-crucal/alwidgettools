#pragma once

#include <QMap>

#include "alwidgettoolsdef.hpp"

class QLayout;
class QVBoxLayout;
class QHBoxLayout;
class QLinearGradient;

class CUVMenu;
class CALNavigationBar;
class CUVNavigationNode;
class CUVNavigationModel;
class CUVNavigationView;
class CUVSuggestBox;
class CUVBaseListView;
class CUVInteractiveCard;
class CUVFooterModel;
class CUVFooterDelegate;
class CUVIconButton;
class CUVAwesomeToolButton;

class CUVNavigationBarPrivate final : public QObject {
	Q_OBJECT
	Q_DISABLE_COPY(CUVNavigationBarPrivate);
	Q_DECLARE_PUBLIC(CALNavigationBar)
	Q_PROPERTY(int navigationViewWidth READ getNavigationViewWidth WRITE setNavigationViewWidth NOTIFY sigNavigationViewWidthChanged)

public:
	explicit CUVNavigationBarPrivate(CALNavigationBar* q, QObject* parent = nullptr);
	~CUVNavigationBarPrivate() override;

	void setNavigationViewWidth(int width);
	[[nodiscard]] int getNavigationViewWidth() const;

	Q_SLOT void slotNavigationButtonClicked();
	Q_SLOT void slotNavigationOpenNewWindow(const QString& nodeKey);

	Q_INVOKABLE void invokableNavigationRouteBack(const QVariantMap& routeData);

	void slotTreeViewClicked(const QModelIndex& index, bool isLogRoute = true);
	void slotFooterViewClicked(const QModelIndex& index, bool isLogRoute = true);

Q_SIGNALS:
	Q_SIGNAL void sigNavigationViewWidthChanged();

protected:
	CALNavigationBar* const q_ptr{ nullptr };

private:
	bool isTransparent{};
	bool isShowUserCard{};
	int navigationViewWidth{};

	ALThemeType::ThemeMode themeMode{};
	ALNavigationType::NavigationDisplayMode currentDisplayMode{};
	QMap<QString, const QMetaObject*> mapPageMeta{};
	QMap<CUVNavigationNode*, CUVMenu*> mapCompactMenu{};
	QList<CUVNavigationNode*> listLastExpandedNodes{};

	QVBoxLayout* navigationButtonVLayout{ nullptr };
	QHBoxLayout* navigationSuggestBoxHLayout{ nullptr };
	QVBoxLayout* userButtonVLayout{ nullptr };

	CUVIconButton* userButton{ nullptr };
	CUVAwesomeToolButton* searchButton{ nullptr };
	CUVAwesomeToolButton* navigationButton{ nullptr };
	CUVNavigationModel* navigationModel{ nullptr };
	CUVNavigationView* navigationView{ nullptr };
	CUVBaseListView* footerView{ nullptr };
	CUVFooterModel* footerModel{ nullptr };
	CUVFooterDelegate* footerDelegate{ nullptr };
	CUVSuggestBox* navigationSuggestBox{ nullptr };
	CUVInteractiveCard* userInfoCard{ nullptr };

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
};
