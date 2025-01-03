#pragma once

#include "uvwidgettoolsdef.hpp"

class QAbstractButton;
class QLabel;
class QScreen;
class QHBoxLayout;
class QVBoxLayout;

class CUVText;
class CUVIconButton;
class CUVAwesomeToolButton;
class CUVAppBar;

class CUVAppBarPrivate final : public QObject {
	Q_OBJECT
	Q_DISABLE_COPY(CUVAppBarPrivate)
	Q_DECLARE_PUBLIC(CUVAppBar)

public:
	explicit CUVAppBarPrivate(CUVAppBar* q, QObject* parent = nullptr);
	~CUVAppBarPrivate() override;

	Q_SLOT void slotMinButtonClicked();
	Q_SLOT void slotMaxButtonClicked();
	Q_SLOT void slotCloseButtonClicked();
	Q_SLOT void slotStayTopButtonClicked() const;
	Q_SLOT void slotThemeModeChanged(const UVThemeType::ThemeMode& mode) const;

protected:
	CUVAppBar* const q_ptr{ nullptr };

private:
	bool isStayTop{};
	bool isFixedSize{};
	bool isDefaultClosed{};
	bool isOnlyAllowMinAndClose{};
	bool isHoverMaxButton{};
	int appBarHeight{};
	int customWidgetMaximumWidth{};
	qint64 currentWinID{};
	int lastMinTrackWidth{};
	quint64 clickTimer{};
	int edges{};
	int margins{ 8 };
	UVAppBarType::ButtonFlags buttonFlags{};
	QMap<UVAppBarType::ButtonFlag, QAbstractButton*> buttonMap{};
	QColor backgroundColor{};

	QWidget* customWidget{ nullptr };
	QHBoxLayout* mainHLayout{ nullptr };
	QVBoxLayout* iconLabelVLayout{ nullptr };
	QVBoxLayout* titleLabelVLayout{ nullptr };
	CUVAwesomeToolButton* routeBackButton{ nullptr };
	CUVAwesomeToolButton* navigationButton{ nullptr };
	CUVAwesomeToolButton* themeChangeButton{ nullptr };
	CUVAwesomeToolButton* stayTopButton{ nullptr };
	CUVAwesomeToolButton* minButton{ nullptr };
	CUVAwesomeToolButton* maxButton{ nullptr };
	CUVIconButton* closeButton{ nullptr };
	QScreen* lastScreen{ nullptr };
	CUVText* titleLabel{ nullptr };
	QLabel* iconLabel{ nullptr };

	void changeMaxButtonAwesome(bool isMaximized) const;
	void showSystemMenu(const QPoint& point);
	void updateCursor(int edges);
	bool containsCursorToItem(QWidget* item);
	int calculateMinimumWidth();
	QVBoxLayout* createVLayout(QWidget* widget) const;
	[[nodiscard]] QAbstractButton* getButtonByAppBarFlag(const UVAppBarType::ButtonFlag& flag) const;
};
