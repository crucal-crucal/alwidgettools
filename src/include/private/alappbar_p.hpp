#pragma once

#include "alwidgettoolsdef.hpp"

class QAbstractButton;
class QLabel;
class QScreen;
class QHBoxLayout;
class QVBoxLayout;

/**
 * @brief \namespace AL
 */
namespace AL {
class CALText;
class CALIconButton;
class CALToolButton;
class CALAppBar;

class CALAppBarPrivate final : public QObject {
	Q_OBJECT
	Q_DISABLE_COPY(CALAppBarPrivate)
	Q_DECLARE_PUBLIC(CALAppBar)

public:
	explicit CALAppBarPrivate(CALAppBar* q, QObject* parent = nullptr);
	~CALAppBarPrivate() override;

	Q_SLOT void slotMinButtonClicked();
	Q_SLOT void slotMaxButtonClicked();
	Q_SLOT void slotCloseButtonClicked();
	Q_SLOT void slotStayTopButtonClicked() const;
	Q_SLOT void slotThemeModeChanged(const ALThemeType::ThemeMode& mode) const;

protected:
	CALAppBar* const q_ptr{ nullptr };

private:
	bool isStayTop{};
	bool isFixedSize{};
	bool isDefaultClosed{};
	bool isOnlyAllowMinAndClose{};
	bool isHoverMaxButton{};
	int appBarHeight{};
	int customWidgetMaximumWidth{};
	int lastMinTrackWidth{};
	int edges{};
	int margins{};
	qint64 currentWinID{};
	quint64 clickTimer{};
	ALAppBarType::ButtonFlags buttonFlags{};
	QMap<ALAppBarType::ButtonFlag, QAbstractButton*> buttonMap{};
	QColor backgroundColor{};

	QWidget* customWidget{ nullptr };
	QHBoxLayout* mainHLayout{ nullptr };
	QVBoxLayout* iconLabelVLayout{ nullptr };
	QVBoxLayout* titleLabelVLayout{ nullptr };
	CALToolButton* routeBackButton{ nullptr };
	CALToolButton* navigationButton{ nullptr };
	CALToolButton* themeChangeButton{ nullptr };
	CALToolButton* stayTopButton{ nullptr };
	CALToolButton* minButton{ nullptr };
	CALToolButton* maxButton{ nullptr };
	CALIconButton* closeButton{ nullptr };
	QScreen* lastScreen{ nullptr };
	CALText* titleLabel{ nullptr };
	QLabel* iconLabel{ nullptr };

	void changeMaxButtonIcon(bool isMaximized) const;
	void showSystemMenu(const QPoint& point);
	void updateCursor(int edges);
	bool containsCursorToItem(QWidget* item);
	int calculateMinimumWidth();
	QVBoxLayout* createVLayout(QWidget* widget) const;
	[[nodiscard]] QAbstractButton* getButtonByAppBarFlag(const ALAppBarType::ButtonFlag& flag) const;
};
}
