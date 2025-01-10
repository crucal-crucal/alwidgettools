#pragma once

#include <QWidget>

#include "alwidgettoolsdef.hpp"

class CALMessageBarManager;
class CALMessageBarPrivate;

class CALWIDGETTOOLS_EXPORT CALMessageBar : public QWidget {
	Q_OBJECT
	Q_DISABLE_COPY(CALMessageBar)
	Q_DECLARE_PRIVATE(CALMessageBar)

public:
	explicit CALMessageBar(const ALMessageBarType::PositionPolicy& policy, const ALMessageBarType::MessageLevel& messageLevel,
	                       const QString& title, const QString& message, int displayMsec, QWidget* parent = nullptr);
	~CALMessageBar() override;

	/**
	 * success notification
	 * @param title 标题
	 * @param message 消息
	 * @param displayMsec 显示时间, 默认2000ms
	 * @param positionPolicy 显示位置, 默认右上角 \enum ALMessageBarType::PositionPolicy
	 * @param parent 父窗口
	 */
	static void success(const QString& title, const QString& message, int displayMsec = 2000,
	                    const ALMessageBarType::PositionPolicy& positionPolicy = ALMessageBarType::TopRight, QWidget* parent = nullptr);

	/**
	 * waring notification
	 * @param title 标题
	 * @param message 消息
	 * @param displayMsec 显示时间, 默认2000ms
	 * @param positionPolicy 显示位置, 默认左上角 \enum ALMessageBarType::PositionPolicy
	 * @param parent 父窗口
	 */
	static void warning(const QString& title, const QString& message, int displayMsec = 2000,
	                    const ALMessageBarType::PositionPolicy& positionPolicy = ALMessageBarType::TopLeft, QWidget* parent = nullptr);

	/**
	 * information notification
	 * @param title 标题
	 * @param message 消息
	 * @param displayMsec 显示时间, 默认2000ms
	 * @param positionPolicy 显示位置, 默认左下角 \enum ALMessageBarType::PositionPolicy
	 * @param parent 父窗口
	 */
	static void information(const QString& title, const QString& message, int displayMsec = 2000,
	                       const ALMessageBarType::PositionPolicy& positionPolicy = ALMessageBarType::BottomLeft, QWidget* parent = nullptr);

	/**
	 * error notification
	 * @param title 标题
	 * @param message 消息
	 * @param displayMsec 显示时间, 默认2000ms
	 * @param positionPolicy 显示位置, 默认右下角 \enum ALMessageBarType::PositionPolicy
	 * @param parent 父窗口
	 */
	static void error(const QString& title, const QString& message, int displayMsec = 2000,
	                  const ALMessageBarType::PositionPolicy& positionPolicy = ALMessageBarType::BottomRight, QWidget* parent = nullptr);

protected:
	const QScopedPointer<CALMessageBarPrivate> d_ptr{ nullptr };

	friend class CALMessageBarManager;

	void paintEvent(QPaintEvent* event) override;
	bool eventFilter(QObject* watched, QEvent* event) override;

	[[nodiscard]] WId getWinID() const;
#if 0
#ifdef Q_OS_WIN
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0);
	bool nativeEvent(const QByteArray& eventType, void* message, qintptr* result) override;
#else
	bool nativeEvent(const QByteArray& eventType, void* message, long* result) override;
#endif
#endif
#endif
};
