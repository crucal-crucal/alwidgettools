#pragma once

#include <QWidget>

#include "alwidgettoolsdef.hpp"

#ifdef Q_OS_WIN
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
#define Q_TAKEOVER_NATIVEEVENT_H bool nativeEvent(const QByteArray& eventType, void* message, qintptr* result) override;
#else
#define AL_TAKEOVER_NATIVEEVENT_H bool nativeEvent(const QByteArray& eventType, void* message, long* result) override;
#endif
#else
#define AL_TAKEOVER_NATIVEEVENT_H
#endif

#ifdef Q_OS_WIN
/**
 * @brief 定义处理本地事件，允许通过 takeOverNativeEvent 接管事件
 * @param CALAppBar appBar
 */
#define CALAPPBAR_HANDLE(CALAppBar)                                           \
	if (CALAppBar) {                                                          \
		int ret = (CALAppBar)->takeOverNativeEvent(eventType, message, result); \
		if (ret == -1) {                                                      \
			return QWidget::nativeEvent(eventType, message, result);          \
		}                                                                     \
		return static_cast<bool>(ret);										  \
	}                                                                         \
	return QWidget::nativeEvent(eventType, message, result);
#endif

#ifdef Q_OS_WIN
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
#define Q_TAKEOVER_NATIVEEVENT_CPP(CLASS, CALAppBar)                                       \
    bool CLASS::nativeEvent(const QByteArray& eventType, void* message, qintptr* result) { \
        CALAPPBAR_HANDLE(CALAppBar)                                                        \
    }
#else
#define AL_TAKEOVER_NATIVEEVENT_CPP(CLASS, CALAppBar)                                    \
    bool CLASS::nativeEvent(const QByteArray& eventType, void* message, long* result) { \
        CALAPPBAR_HANDLE(CALAppBar)                                                     \
    }
#endif
#else
#define AL_TAKEOVER_NATIVEEVENT_CPP(CLASS, CALAppBar)
#endif

/**
 * @brief \namespace AL
 */
namespace AL {
class CALAppBarPrivate;

class CALWIDGETTOOLS_EXPORT CALAppBar : public QWidget {
	Q_OBJECT
	Q_DECLARE_PRIVATE(CALAppBar)
	Q_PROPERTY(bool isStayTop READ getIsStayTop WRITE setIsStayTop NOTIFY sigIsStayTopChanged)
	Q_PROPERTY(bool isFixedSize READ getIsFixedSize WRITE setIsFixedSize NOTIFY sigIsFixedSizeChanged)
	Q_PROPERTY(bool iSDefaultClosed READ getIsDefaultClosed WRITE setIsDefaultClosed NOTIFY sigIsDefaultClosedChanged)
	Q_PROPERTY(bool isOnlyAllowMinAndClose READ getIsOnlyAllowMinAndClose WRITE setIsOnlyAllowMinAndClose NOTIFY sigIsOnlyAllowMinAndCloseChanged)
	Q_PROPERTY(int appBarHeight READ getAppBarHeight WRITE setAppBarHeight NOTIFY sigAppBarHeightChanged)
	Q_PROPERTY(int customWidgetMaximumWidth READ getCustomWidgetMaximumWidth WRITE setCustomWidgetMaximumWidth NOTIFY sigCustomWidgetMaximumWidthChanged)

public:
	explicit CALAppBar(QWidget* parent);
	~CALAppBar() override;

	void setIsStayTop(bool isStayTop);
	[[nodiscard]] bool getIsStayTop() const;

	void setIsFixedSize(bool isFixedSize);
	[[nodiscard]] bool getIsFixedSize() const;

	void setIsDefaultClosed(bool isDefaultClosed);
	[[nodiscard]] bool getIsDefaultClosed() const;

	void setIsOnlyAllowMinAndClose(bool isOnlyAllowMinAndClose);
	[[nodiscard]] bool getIsOnlyAllowMinAndClose() const;

	void setAppBarHeight(int appBarHeight);
	[[nodiscard]] int getAppBarHeight() const;

	void setCustomWidgetMaximumWidth(int customWidgetMaximumWidth);
	[[nodiscard]] int getCustomWidgetMaximumWidth() const;

	void setBackgroundColor(const QColor& color);
	[[nodiscard]] QColor getBackgroundColor() const;

	void setCustomWidget(const ALAppBarType::CustomArea& customArea, QWidget* widget);
	[[nodiscard]] QWidget* getCustomWidget() const;

	void setWindowButtonFlag(const ALAppBarType::ButtonFlag& buttonFlag, bool isEnable = true);
	void setWindowButtonFlags(const ALAppBarType::ButtonFlags& buttonFlags);
	[[nodiscard]] ALAppBarType::ButtonFlags getWindowButtonFlags() const;

	bool insertWidgetBeforeButton(QWidget* widget, const ALAppBarType::ButtonFlag& flag);
	bool insertWidgetBeforeWidget(QWidget* widget, QWidget* targetWidget);
	bool insertWidgetBeforeLayout(QWidget* widget, QLayout* targetLayout);
	bool insertLayoutBeforeButton(QLayout* layout, const ALAppBarType::ButtonFlag& flag);
	bool insertLayoutBeforeWidget(QLayout* layout, QWidget* targetWidget);
	bool insertLayoutBeforeLayout(QLayout* layout, QLayout* targetLayout);

	void setRouteBackButtonEnable(bool isEnable);

	void closeWindow();
#ifdef Q_OS_WIN
	// 接管本地事件
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
	int takeOverNativeEvent(const QByteArray& eventType, void* message, qintptr* result);
#else
	int takeOverNativeEvent(const QByteArray& eventType, const void* message, long* result);
#endif
#endif

Q_SIGNALS:
	Q_SIGNAL void sigIsStayTopChanged();
	Q_SIGNAL void sigIsFixedSizeChanged();
	Q_SIGNAL void sigIsDefaultClosedChanged();
	Q_SIGNAL void sigIsOnlyAllowMinAndCloseChanged();
	Q_SIGNAL void sigAppBarHeightChanged();
	Q_SIGNAL void sigCustomWidgetMaximumWidthChanged();
	Q_SIGNAL void sigRouteBackButtonClicked();
	Q_SIGNAL void sigNavigationButtonClicked();
	Q_SIGNAL void sigThemeChangeButtonClicked();
	Q_SIGNAL void sigCloseButtonClicked();
	Q_SIGNAL void sigCustomWidgetChanged();

protected:
	const QScopedPointer<CALAppBarPrivate> d_ptr{ nullptr };

	void paintEvent(QPaintEvent* event) override;
	bool eventFilter(QObject* watched, QEvent* event) override;
};
}
