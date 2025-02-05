#pragma once

#include <QObject>

#include "alsingleton.hpp"
#include "alwidgettoolsdef.hpp"

class QPainter;

/**
 * @brief \namespace AL
 */
namespace AL {
class CALMessageBar;
class CALIconButton;

enum WorkStatus {
	Idle                = 0x0000,
	CreateAnimation     = 0x0001,
	OtherEventAnimation = 0x0002,
};

class CALMessageBarManager final : public QObject {
	Q_OBJECT

public:
	static CALMessageBarManager* instance();

	/**
	 * @brief 请求事件堆栈调用
	 * @param messageBar 消息栏
	 */
	void requestMessageBarEvent(CALMessageBar* messageBar);

	/**
	 * @brief 发布创建事件
	 * @param messageBar 消息栏
	 */
	void postMessageBarCreateEvent(CALMessageBar* messageBar);

	/**
	 * @brief 发布终止事件
	 * @param messageBar 消息栏
	 */
	void postMessageBarEndEvent(CALMessageBar* messageBar);

	/**
	 * @brief 强制发布终止事件
	 * @param messageBar 消息栏
	 */
	void forcePostMessageBarEndEvent(CALMessageBar* messageBar);

	/**
	 * @brief 获取当前事件数量
	 * @param messageBar 消息栏
	 * @return 当前时间数量
	 */
	int getMessageBarEventCount(CALMessageBar* messageBar);

	/**
	 * @brief 更新活动序列
	 * @param messageBar 消息栏
	 * @param isActive 是否活动
	 */
	void updateActionMap(CALMessageBar* messageBar, bool isActive);

protected:
	friend class CALSingleton<CALMessageBarManager>;

private:
	explicit CALMessageBarManager(QObject* parent = nullptr);
	~CALMessageBarManager() override;

	QMap<CALMessageBar*, QList<QVariantMap>> mapMessageBarEvent{};
};

class CALMessageBarPrivate final : public QObject {
	Q_OBJECT
	Q_DISABLE_COPY(CALMessageBarPrivate)
	Q_DECLARE_PUBLIC(CALMessageBar)
	Q_PROPERTY(qreal opacity READ getOpacity WRITE setOpacity NOTIFY sigOpacityChanged)

public:
	explicit CALMessageBarPrivate(CALMessageBar* q, QObject* parent = nullptr);
	~CALMessageBarPrivate() override;

	void init();

	void tryToRequestMessageBarEvent();
	[[nodiscard]] WorkStatus getWorkStatus() const;
	Q_INVOKABLE void invokableMessageBarEnd(const QVariantMap& eventData);
	Q_INVOKABLE void invokableOtherMessageBarEnd(const QVariantMap& eventData);
	Q_SLOT void slotCloseButtonClicked();

	void setOpacity(qreal opacity);
	[[nodiscard]] qreal getOpacity() const;

	static void showMessageBar(const ALMessageBarType::PositionPolicy& positionPolicy, const ALMessageBarType::MessageLevel& messageLevel,
	                           const QString& title, const QString& message, int displayMsec, QWidget* parent = nullptr);


Q_SIGNALS:
	void sigOpacityChanged();

protected:
	friend class CALMessageBarManager;

	CALMessageBar* const q_ptr{ nullptr };

private:
	ALThemeType::ThemeMode themeMode{};
	ALMessageBarType::PositionPolicy policy{};
	ALMessageBarType::MessageLevel messageMode{};
	qint64 currentWinID{};
	qreal borderRadius{};
	qreal opacity{};
	qint64 createTime{};
	QString title{};
	QString text{};

	// 位置数据
	int leftPadding{};
	int titleLeftSpacing{};
	int textLeftSpacing{};
	int closeButtonLeftRightMargin{};
	int closeButtonWidth{};
	int messageBarHorizontalMargin{};
	int messageBarVerticalBottomMargin{};
	int messageBarVerticalTopMargin{};
	int messageBarSpacing{};
	int shadowBorderWidth{};

	// 逻辑数据
	bool isMessageBarCreateAnimationFinished{};
	bool isReadyToEnd{};
	bool isNormalDisplay{};
	bool isMessageBarEventAnimationStart{};
	CALIconButton* closeButton{ nullptr };

	Q_INVOKABLE void invokableMessageBarCreate(int displayMsec);
	void calculateInitialPos(int& startX, int& startY, int& endX, int& endY);
	QList<int> getOtherMessageBarTotalData(bool isJudgeCreateOrder = false);
	qreal calculateTargetPosY();
	bool judgeCreateDrder(CALMessageBar* otherMessageBar) const;

	void drawMessage(QPainter* painter, const QColor& backgroundColor, const QColor& iconColor, const QString& iconText, const QColor& textColor, const int& iconPixelSize, const int& iconX, const QColor& penColor = Qt::white);
	void drawSuccess(QPainter* painter);
	void drawError(QPainter* painter);
	void drawWarning(QPainter* painter);
	void drawInfo(QPainter* painter);
};
}
