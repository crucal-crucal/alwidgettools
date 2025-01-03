#pragma once

#include <QImage>

#include "uvwidgettoolsdef.hpp"

class QHBoxLayout;
class QVBoxLayout;

class CUVEvent;
class CUVMainWindow;
class CUVNavigationBar;
class CUVAppBar;
class CUVCenterStackedWidget;
class CUVThemeAnimationWidget;

class CUVMainWindowPrivate final : public QObject {
	Q_OBJECT
	Q_DISABLE_COPY(CUVMainWindowPrivate)
	Q_DECLARE_PUBLIC(CUVMainWindow)

public:
	explicit CUVMainWindowPrivate(CUVMainWindow* q, QObject* parent = nullptr);
	~CUVMainWindowPrivate() override;

	Q_SLOT void slotNavigationButtonClicked();
	Q_SLOT void slotThemeReadyChange();
	Q_SLOT void slotDisplayModeChanged();
	Q_SLOT void slotThemeModeChanged(const UVThemeType::ThemeMode& mode);
	Q_SLOT void slotNavigationNodeClicked(const UVNavigationType::NavigationNodeType& nodeType, const QString& nodeKey);
	Q_SLOT void slotNavigationNodeAdded(const UVNavigationType::NavigationNodeType& nodeType, const QString& nodeKey, QWidget* page);
	Q_INVOKABLE void invokableWMWindowClickedEvent(const QVariantMap& data);

protected:
	CUVMainWindow* const q_ptr{ nullptr };

private:
	UVThemeType::ThemeMode themeMode{};
	UVNavigationType::NavigationDisplayMode navigationDisplayMode{};
	UVNavigationType::NavigationDisplayMode currentNavigationBarDisplayMode{};

	int themeChangeTime{};
	int contentsMargins{};
	int navigationTargetIndex{};
	bool isWindowClosing{};
	bool isInitFinished{};
	bool isNavigationDisplayModeChanged{};
	bool isNavigationEnable{};
	bool isNavigationBarExpanded{};
	bool isWMClickedAnimatinoFinished{};
	QImage lightBaseImage{};
	QImage darkBaseImage{};
	QMap<QString, int> routeMap{};

	CUVEvent* focusEvent{ nullptr };
	CUVNavigationBar* navigationBar{ nullptr };
	CUVCenterStackedWidget* centerStackedWidget{ nullptr };
	CUVAppBar* appBar{ nullptr };
	CUVThemeAnimationWidget* themeAnimationWidget{ nullptr };
	QHBoxLayout* centerHLayout{ nullptr };

	static qreal distance(const QPoint& point1, const QPoint& point2);
	void resetWindowLayout(bool isAnimation) const;
	void doNavigationDisplayModeChange();
};
