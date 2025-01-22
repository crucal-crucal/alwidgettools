#pragma once

#include "alwidgettoolsdef.hpp"

/**
 * @brief \namespace AL
 */
namespace AL {
class CALDockWidget;
class CALDockWidgetTitleBar;

class CALDockWidgetPrivate final : public QObject {
	Q_OBJECT
	Q_DISABLE_COPY(CALDockWidgetPrivate)
	Q_DECLARE_PUBLIC(CALDockWidget)

public:
	explicit CALDockWidgetPrivate(CALDockWidget* q, QObject* parent = nullptr);
	~CALDockWidgetPrivate() override;

	Q_SLOT void slotThemeModeChanged(const ALThemeType::ThemeMode& themeMode);

protected:
	friend class CALDockWidgetTitleBar;

	CALDockWidget* const q_ptr{ nullptr };

private:
	ALThemeType::ThemeMode themeMode{};
	qint64 currentWinID{};
	int margins{};
	int shadowBorderWidth{};
	bool isEanbleMica{};
	CALDockWidgetTitleBar* titleBar{ nullptr };
};
}
