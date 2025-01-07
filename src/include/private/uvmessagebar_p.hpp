#pragma once

#include <QObject>

#include "alsingleton.hpp"

class CUVMessageBar;
class CUVIconButton;
class QPainter;

enum WorkStatus {
	Idle                = 0x0000,
	CreateAnimation     = 0x0001,
	OtherEventAnimation = 0x0002,
};

class CUVMessageBarManager final : public QObject {
	Q_OBJECT

public:
	static CUVMessageBarManager* instance();

	/**
	 * @brief 请求事件堆栈调用
	 * @param messageBar 消息栏
	 */
	void requestMessageBarEvent(CUVMessageBar* messageBar);

	/**
	 * @brief 发布创建事件
	 * @param messageBar 消息栏
	 */
	void postMessageBarCreateEvent(CUVMessageBar* messageBar);

	/**
	 * @brief 发布终止事件
	 * @param messageBar 消息栏
	 */
	void postMessageBarEndEvent(CUVMessageBar* messageBar);

	/**
	 * @brief 强制发布终止事件
	 * @param messageBar 消息栏
	 */
	void forcePostMessageBarEndEvent(CUVMessageBar* messageBar);

	/**
	 * @brief 获取当前事件数量
	 * @param messageBar 消息栏
	 * @return 当前时间数量
	 */
	int getMessageBarEventCount(CUVMessageBar* messageBar);

	/**
	 * @brief 更新活动序列
	 * @param messageBar 消息栏
	 * @param isActive 是否活动
	 */
	void updateActionMap(CUVMessageBar* messageBar, bool isActive);

protected:
	friend class CUVSingleton<CUVMessageBarManager>;

private:
	explicit CUVMessageBarManager(QObject* parent = nullptr);
	~CUVMessageBarManager() override;

	QMap<CUVMessageBar*, QList<QVariantMap>> mapMessageBarEvent{};
};

class CUVMessageBarPrivate final : public QObject {
	Q_OBJECT
	Q_DISABLE_COPY(CUVMessageBarPrivate)
	Q_DECLARE_PUBLIC(CUVMessageBar)
	Q_PROPERTY(qreal opacity READ getOpacity WRITE setOpacity NOTIFY sigOpacityChanged)

public:
	explicit CUVMessageBarPrivate(CUVMessageBar* q, QObject* parent = nullptr);
	~CUVMessageBarPrivate() override;

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
	Q_SIGNAL void sigOpacityChanged();

protected:
	friend class CUVMessageBarManager;

	CUVMessageBar* const q_ptr{ nullptr };

private:
	ALThemeType::ThemeMode themeMode{};
	ALMessageBarType::PositionPolicy policy{};
	ALMessageBarType::MessageLevel messageMode{};
	qint64 currentWinID{};
	int borderRadius{};
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
	CUVIconButton* closeButton{ nullptr };

	Q_INVOKABLE void invokableMessageBarCreate(int displayMsec);
	void calculateInitialPos(int& startX, int& startY, int& endX, int& endY);
	QList<int> getOtherMessageBarTotalData(bool isJudgeCreateOrder = false);
	qreal calculateTargetPosY();
	bool judgeCreateDrder(CUVMessageBar* otherMessageBar) const;

	void drawMessage(QPainter* painter, const QColor& backgroundColor, const QColor& iconColor, const QString& iconText, const QColor& textColor, const int& iconPixelSize, const int& iconX, const QColor& penColor = Qt::white);
	void drawSuccess(QPainter* painter);
	void drawError(QPainter* painter);
	void drawWarning(QPainter* painter);
	void drawInfo(QPainter* painter);
};
