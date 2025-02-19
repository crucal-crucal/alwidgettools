#pragma once

#include <QImage>

#include "alwidgettoolsdef.hpp"

class QHBoxLayout;
class QVBoxLayout;

/**
 * @brief \namespace AL
 */
namespace AL {
class CALEvent;
class CALMainWindow;
class CALNavigationBar;
class CALAppBar;
class CALCenterStackedWidget;
class CALThemeAnimationWidget;

class CALMainWindowPrivate final : public QObject {
	Q_OBJECT
	Q_DISABLE_COPY(CALMainWindowPrivate)
	Q_DECLARE_PUBLIC(CALMainWindow)

public:
	explicit CALMainWindowPrivate(CALMainWindow* q, QObject* parent = nullptr);
	~CALMainWindowPrivate() override;

	Q_SLOT void slotNavigationButtonClicked();
	Q_SLOT void slotThemeReadyChange();
	Q_SLOT void slotDisplayModeChanged();
	Q_SLOT void slotThemeModeChanged(const ALThemeType::ThemeMode& mode);
	Q_SLOT void slotNavigationNodeClicked(const ALNavigationType::NavigationNodeType& nodeType, const QString& nodeKey);
	Q_SLOT void slotNavigationNodeAdded(const ALNavigationType::NavigationNodeType& nodeType, const QString& nodeKey, QWidget* page);
	Q_SLOT void slotNavigationNodeRemoved(const ALNavigationType::NavigationNodeType& nodeType, const QString& nodeKey);
	Q_INVOKABLE void invokableWMWindowClickedEvent(const QVariantMap& data);

protected:
	CALMainWindow* const q_ptr{ nullptr };

private:
	ALThemeType::ThemeMode themeMode{};
	ALNavigationType::NavigationDisplayMode navigationDisplayMode{};
	ALNavigationType::NavigationDisplayMode currentNavigationBarDisplayMode{};

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
	QMap<QString, QWidget*> routeMap{};

	CALEvent* focusEvent{ nullptr };
	CALNavigationBar* navigationBar{ nullptr };
	CALCenterStackedWidget* centerStackedWidget{ nullptr };
	CALAppBar* appBar{ nullptr };
	CALThemeAnimationWidget* themeAnimationWidget{ nullptr };
	QHBoxLayout* centerHLayout{ nullptr };

	static qreal distance(const QPoint& point1, const QPoint& point2);
	void resetWindowLayout(bool isAnimation) const;
	void doNavigationDisplayModeChange();
};
}
