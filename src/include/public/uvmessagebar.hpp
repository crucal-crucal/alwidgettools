#pragma once

#include <QWidget>

#include "uvwidgettoolsdef.hpp"

class CUVMessageBarManager;
class CUVMessageBarPrivate;

class CUVWIDGETTOOLS_EXPORT CUVMessageBar : public QWidget {
	Q_OBJECT
	Q_DISABLE_COPY(CUVMessageBar)
	Q_DECLARE_PRIVATE(CUVMessageBar)

public:
	explicit CUVMessageBar(const UVMessageBarType::PositionPolicy& policy, const UVMessageBarType::MessageLevel& messageLevel,
	                       const QString& title, const QString& message, int displayMsec, QWidget* parent = nullptr);
	~CUVMessageBar() override;

	/**
	 * success notification
	 * @param title 标题
	 * @param message 消息
	 * @param displayMsec 显示时间, 默认2000ms
	 * @param positionPolicy 显示位置, 默认右上角 \enum UVMessageBarType::PositionPolicy
	 * @param parent 父窗口
	 */
	static void success(const QString& title, const QString& message, int displayMsec = 2000,
	                    const UVMessageBarType::PositionPolicy& positionPolicy = UVMessageBarType::TopRight, QWidget* parent = nullptr);

	/**
	 * waring notification
	 * @param title 标题
	 * @param message 消息
	 * @param displayMsec 显示时间, 默认2000ms
	 * @param positionPolicy 显示位置, 默认左上角 \enum UVMessageBarType::PositionPolicy
	 * @param parent 父窗口
	 */
	static void warning(const QString& title, const QString& message, int displayMsec = 2000,
	                    const UVMessageBarType::PositionPolicy& positionPolicy = UVMessageBarType::TopLeft, QWidget* parent = nullptr);

	/**
	 * information notification
	 * @param title 标题
	 * @param message 消息
	 * @param displayMsec 显示时间, 默认2000ms
	 * @param positionPolicy 显示位置, 默认左下角 \enum UVMessageBarType::PositionPolicy
	 * @param parent 父窗口
	 */
	static void information(const QString& title, const QString& message, int displayMsec = 2000,
	                       const UVMessageBarType::PositionPolicy& positionPolicy = UVMessageBarType::BottomLeft, QWidget* parent = nullptr);

	/**
	 * error notification
	 * @param title 标题
	 * @param message 消息
	 * @param displayMsec 显示时间, 默认2000ms
	 * @param positionPolicy 显示位置, 默认右下角 \enum UVMessageBarType::PositionPolicy
	 * @param parent 父窗口
	 */
	static void error(const QString& title, const QString& message, int displayMsec = 2000,
	                  const UVMessageBarType::PositionPolicy& positionPolicy = UVMessageBarType::BottomRight, QWidget* parent = nullptr);

protected:
	const QScopedPointer<CUVMessageBarPrivate> d_ptr{ nullptr };

	friend class CUVMessageBarManager;

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
