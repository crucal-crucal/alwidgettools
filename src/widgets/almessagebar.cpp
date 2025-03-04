#include "almessagebar.hpp"

#include <QApplication>
#include <QDateTime>
#include <QGraphicsOpacityEffect>
#include <QHBoxLayout>
#include <QPainter>
#include <QPainterPath>
#include <QPropertyAnimation>
#include <QResizeEvent>
#include <QTimer>
#include <QVariant>

#include "alicon.hpp"
#include "aliconbutton.hpp"
#include "almessagebar_p.hpp"
#include "althememanager.hpp"

#ifdef Q_OS_WIN
#include <dwmapi.h>
#include <windowsx.h>
#endif

/**
 * @brief \namespace AL
 */
namespace AL {
QMap<ALMessageBarType::PositionPolicy, QList<CALMessageBar*>*> mapMessageBarActive;

/**
 * @brief \class CALMessageBarManager
 * @return CALMessageBarManager instance
 */
CALMessageBarManager* CALMessageBarManager::instance() {
	return CALSingleton<CALMessageBarManager>::instance();
}

void CALMessageBarManager::requestMessageBarEvent(CALMessageBar* messageBar) {
	if (!messageBar) {
		return;
	}

	if (mapMessageBarEvent.contains(messageBar)) {
		QList<QVariantMap> eventList = mapMessageBarEvent.value(messageBar);
		const QVariantMap eventData = eventList.last();
		eventList.removeLast();
		if (eventList.isEmpty()) {
			mapMessageBarEvent.remove(messageBar);
		} else {
			mapMessageBarEvent.insert(messageBar, eventList);
		}
		// 触发事件
		const QString funcName = eventData.value("EventFuncName").toString();
		const QVariantMap funcData = eventData.value("EventFuncData").toMap();
		QMetaObject::invokeMethod(messageBar->d_func(), funcName.toLocal8Bit().constData(), Qt::AutoConnection, Q_ARG(QVariantMap, funcData));
	}
}

void CALMessageBarManager::postMessageBarCreateEvent(CALMessageBar* messageBar) {
	if (!messageBar) {
		return;
	}

	updateActionMap(messageBar, true); // 计算坐标前增加
	if (!mapMessageBarEvent.contains(messageBar)) {
		QList<QVariantMap> eventList{};
		QVariantMap eventData{};
		eventData.insert("EventFuncName", "invokableMessageBarEnd");
		eventList.append(eventData);
		mapMessageBarEvent.insert(messageBar, eventList);
	}
}

void CALMessageBarManager::postMessageBarEndEvent(CALMessageBar* messageBar) {
	if (!messageBar) {
		return;
	}

	updateActionMap(messageBar, false);
	// Other MessageBar 事件入栈, 记录同一策略事件
	const ALMessageBarType::PositionPolicy policy = messageBar->d_func()->policy;
	for (const auto& otherMessageBar : *mapMessageBarActive.value(policy)) {
		if (otherMessageBar->d_func()->judgeCreateDrder(messageBar)) {
			QList<QVariantMap> eventList = mapMessageBarEvent[otherMessageBar];
			// 优先执行先触发的事件, end 事件保持首位
			QVariantMap eventData{};
			eventData.insert("EventFuncName", "invokableOtherMessageBarEnd");
			QVariantMap funcData{};
			funcData.insert("TargetPosY", otherMessageBar->d_func()->calculateTargetPosY());
			eventData.insert("EventFuncData", funcData);
			// 若处于创建动画阶段, 则合并事件动画
			if (otherMessageBar->d_func()->getWorkStatus() == WorkStatus::CreateAnimation) {
				while (eventList.count() > 1) {
					eventList.removeLast();
				}
			}
			eventList.insert(1, eventData);
			mapMessageBarEvent[otherMessageBar] = eventList;
			otherMessageBar->d_func()->tryToRequestMessageBarEvent();
		}
	}
}

void CALMessageBarManager::forcePostMessageBarEndEvent(CALMessageBar* messageBar) {
	if (!messageBar) {
		return;
	}

	// 清楚事件堆栈记录
	mapMessageBarEvent.remove(messageBar);
	// 发布终止事件
	postMessageBarEndEvent(messageBar);
}

int CALMessageBarManager::getMessageBarEventCount(CALMessageBar* messageBar) {
	if (!messageBar || !mapMessageBarEvent.contains(messageBar)) {
		return -1;
	}

	return mapMessageBarEvent[messageBar].count();
}

void CALMessageBarManager::updateActionMap(CALMessageBar* messageBar, const bool isActive) { // NOLINT
	if (!messageBar) {
		return;
	}

	const ALMessageBarType::PositionPolicy policy = messageBar->d_func()->policy;
	if (isActive) {
		if (mapMessageBarActive.contains(policy)) {
			mapMessageBarActive[policy]->append(messageBar);
		} else {
			const auto messageBarList = new QList<CALMessageBar*>();
			messageBarList->append(messageBar);
			mapMessageBarActive.insert(policy, messageBarList);
		}
	} else {
		if (mapMessageBarActive.contains(policy) && mapMessageBarActive[policy]->count() > 0) {
			mapMessageBarActive[policy]->removeOne(messageBar);
		}
	}
}

CALMessageBarManager::CALMessageBarManager(QObject* parent): QObject(parent) {
}

CALMessageBarManager::~CALMessageBarManager() = default;

/**
 * @brief \class CALMessageBarPrivate
 * Internal class for CALMessageBar
 * @param q pointer to the public class
 * @param parent pointer to the parent class
 */
CALMessageBarPrivate::CALMessageBarPrivate(CALMessageBar* q, QObject* parent): QObject(parent), q_ptr(q) {
	this->setProperty("MessageBarCloseY", 0);
	this->setProperty("MessageBarFinishY", 0);
	createTime = QDateTime::currentMSecsSinceEpoch();
}

CALMessageBarPrivate::~CALMessageBarPrivate() = default;

void CALMessageBarPrivate::init() {
	borderRadius = 6;
	title = "";
	text = "";
	leftPadding = 20;
	titleLeftSpacing = 30;
	textLeftSpacing = 15;
	closeButtonLeftRightMargin = 20;
	closeButtonWidth = 30;
	messageBarHorizontalMargin = 20;
	messageBarVerticalBottomMargin = 20;
	messageBarVerticalTopMargin = 50;
	messageBarSpacing = 15;
	shadowBorderWidth = 6;

	isMessageBarCreateAnimationFinished = false;
	isReadyToEnd = false;
	isNormalDisplay = false;
	isMessageBarEventAnimationStart = false;
}

void CALMessageBarPrivate::tryToRequestMessageBarEvent() {
	if (!isMessageBarCreateAnimationFinished || isMessageBarEventAnimationStart) {
		return;
	}

	CALMessageBarManager::instance()->requestMessageBarEvent(q_func());
}

WorkStatus CALMessageBarPrivate::getWorkStatus() const {
	if (!isMessageBarCreateAnimationFinished) {
		return WorkStatus::CreateAnimation;
	}
	if (isMessageBarEventAnimationStart) {
		return WorkStatus::OtherEventAnimation;
	}
	return WorkStatus::Idle;
}

void CALMessageBarPrivate::invokableMessageBarEnd(const QVariantMap& eventData) {
	Q_Q(CALMessageBar);

	CALMessageBarManager::instance()->postMessageBarEndEvent(q);
	const auto barFinishedOpacityAnimation = new QPropertyAnimation(this, "opacity");
	connect(barFinishedOpacityAnimation, &QPropertyAnimation::valueChanged, this, [this, q]() {
		closeButton->setOpacity(opacity);
		q->update();
	});
	connect(barFinishedOpacityAnimation, &QPropertyAnimation::finished, this, [q]() { q->deleteLater(); });
	barFinishedOpacityAnimation->setDuration(300);
	barFinishedOpacityAnimation->setEasingCurve(QEasingCurve::InOutSine);
	barFinishedOpacityAnimation->setStartValue(1);
	barFinishedOpacityAnimation->setEndValue(0);
	barFinishedOpacityAnimation->start(QAbstractAnimation::DeleteWhenStopped);
}

void CALMessageBarPrivate::invokableOtherMessageBarEnd(const QVariantMap& eventData) {
	Q_Q(CALMessageBar);

	isMessageBarEventAnimationStart = true;
	const qreal targetPosY = eventData.value("TargetPosY").toReal();
	const auto closePosAnimation = new QPropertyAnimation(this, "MessageBarCloseY");
	connect(closePosAnimation, &QPropertyAnimation::valueChanged, this, [q](const QVariant& value) { q->move(q->pos().x(), value.toInt()); });
	connect(closePosAnimation, &QPropertyAnimation::finished, this, [this, q]() {
		isMessageBarEventAnimationStart = false;
		if (CALMessageBarManager::instance()->getMessageBarEventCount(q) > 1) {
			CALMessageBarManager::instance()->requestMessageBarEvent(q);
		}
		if (isReadyToEnd) {
			CALMessageBarManager::instance()->requestMessageBarEvent(q);
		}
	});
	closePosAnimation->setEasingCurve(QEasingCurve::InOutSine);
	closePosAnimation->setDuration(200);
	closePosAnimation->setStartValue(q->pos().y());
	closePosAnimation->setEndValue(targetPosY);
	closePosAnimation->start(QAbstractAnimation::DeleteWhenStopped);
}

void CALMessageBarPrivate::slotCloseButtonClicked() {
	Q_Q(CALMessageBar);

	if (isReadyToEnd) {
		return;
	}

	isReadyToEnd = true;
	isNormalDisplay = false;
	CALMessageBarManager::instance()->forcePostMessageBarEndEvent(q);
	const auto opacityAnimation = new QPropertyAnimation(this, "opacity");
	connect(opacityAnimation, &QPropertyAnimation::valueChanged, this, [this, q]() {
		closeButton->setOpacity(opacity);
		q->update();
	});
	connect(opacityAnimation, &QPropertyAnimation::finished, this, [q]() { q->deleteLater(); });
	opacityAnimation->setEasingCurve(QEasingCurve::InOutSine);
	opacityAnimation->setStartValue(opacity);
	opacityAnimation->setEndValue(0);
	opacityAnimation->setDuration(220);
	opacityAnimation->start(QAbstractAnimation::DeleteWhenStopped);
}

void CALMessageBarPrivate::invokableMessageBarCreate(const int displayMsec) {
	Q_Q(CALMessageBar);

	q->show();
	QFont font = q->font();
	font.setPixelSize(16);
	font.setWeight(QFont::Bold);
	q->setFont(font);
	const int titleWidth = q->fontMetrics().horizontalAdvance(title);
	font.setPixelSize(14);
	font.setWeight(QFont::Medium);
	q->setFont(font);
	const int textWidth = q->fontMetrics().horizontalAdvance(text);
	const int fixedWidth = closeButtonLeftRightMargin + leftPadding + titleLeftSpacing + closeButtonWidth + titleWidth + textWidth + 2 * shadowBorderWidth;
	q->setFixedWidth(qMin(500, fixedWidth));
	CALMessageBarManager::instance()->postMessageBarCreateEvent(q);
	int startX = 0, startY = 0, endX = 0, endY = 0;
	calculateInitialPos(startX, startY, endX, endY);
	// 划入动画
	const auto barPosAnimation = new QPropertyAnimation(q, "pos");
	connect(barPosAnimation, &QPropertyAnimation::finished, q, [this, q, displayMsec]() {
		isNormalDisplay = true;
		isMessageBarCreateAnimationFinished = true;
		if (CALMessageBarManager::instance()->getMessageBarEventCount(q) > 1) {
			CALMessageBarManager::instance()->requestMessageBarEvent(q);
		}
		QTimer::singleShot(displayMsec, q, [this, q]() {
			isReadyToEnd = true;
			CALMessageBarManager::instance()->requestMessageBarEvent(q);
		});
	});
	switch (policy) {
		case ALMessageBarType::Top:
		case ALMessageBarType::Bottom: {
			barPosAnimation->setDuration(250);
			break;
		}
		default: {
			barPosAnimation->setDuration(450);
			break;
		}
	}
	barPosAnimation->setEasingCurve(QEasingCurve::InOutSine);
	barPosAnimation->setStartValue(QPoint(startX, startY));
	barPosAnimation->setEndValue(QPoint(endX, endY));
	barPosAnimation->start(QAbstractAnimation::DeleteWhenStopped);
}

void CALMessageBarPrivate::calculateInitialPos(int& startX, int& startY, int& endX, int& endY) {
	Q_Q(CALMessageBar);

	QList<int> resultList = getOtherMessageBarTotalData();
	const int minimumHeightTotal = resultList[0];
	const int indexLessCount = resultList[1];
	switch (this->policy) {
		case ALMessageBarType::Top: {
			// 25动画距离
			startX = q->parentWidget()->width() / 2 - q->minimumWidth() / 2;
			startY = minimumHeightTotal + messageBarSpacing * indexLessCount + messageBarVerticalTopMargin - 25;
			endX = startX;
			endY = minimumHeightTotal + messageBarSpacing * indexLessCount + messageBarVerticalTopMargin;
			break;
		}
		case ALMessageBarType::Left: {
			startX = -q->minimumWidth();
			startY = minimumHeightTotal + messageBarSpacing * indexLessCount + q->parentWidget()->height() / 2;
			endX = messageBarHorizontalMargin;
			endY = startY;
			break;
		}
		case ALMessageBarType::Bottom: {
			startX = q->parentWidget()->width() / 2 - q->minimumWidth() / 2;
			startY = q->parentWidget()->height() - q->minimumHeight() - minimumHeightTotal - messageBarSpacing * indexLessCount - messageBarVerticalBottomMargin - 25;
			endX = startX;
			endY = q->parentWidget()->height() - q->minimumHeight() - minimumHeightTotal - messageBarSpacing * indexLessCount - messageBarVerticalBottomMargin;
			break;
		}
		case ALMessageBarType::Right: {
			startX = q->parentWidget()->width();
			startY = minimumHeightTotal + messageBarSpacing * indexLessCount + q->parentWidget()->height() / 2;
			endX = q->parentWidget()->width() - q->minimumWidth() - messageBarHorizontalMargin;
			endY = startY;
			break;
		}
		case ALMessageBarType::TopRight: {
			startX = q->parentWidget()->width();
			startY = minimumHeightTotal + messageBarSpacing * indexLessCount + messageBarVerticalTopMargin;
			endX = q->parentWidget()->width() - q->minimumWidth() - messageBarHorizontalMargin;
			endY = startY;
			break;
		}
		case ALMessageBarType::TopLeft: {
			startX = -q->minimumWidth();
			startY = minimumHeightTotal + messageBarSpacing * indexLessCount + messageBarVerticalTopMargin;
			endX = this->messageBarHorizontalMargin;
			endY = startY;
			break;
		}
		case ALMessageBarType::BottomRight: {
			startX = q->parentWidget()->width();
			startY = q->parentWidget()->height() - q->minimumHeight() - minimumHeightTotal - messageBarSpacing * indexLessCount - messageBarVerticalBottomMargin;
			endX = q->parentWidget()->width() - q->minimumWidth() - this->messageBarHorizontalMargin;
			endY = startY;
			break;
		}
		case ALMessageBarType::BottomLeft: {
			startX = -q->minimumWidth();
			startY = q->parentWidget()->height() - q->minimumHeight() - minimumHeightTotal - messageBarSpacing * indexLessCount - messageBarVerticalBottomMargin;
			endX = this->messageBarHorizontalMargin;
			endY = startY;
			break;
		}
		default: {
			break;
		}
	}

	if (endY < messageBarVerticalTopMargin || endY > q->parentWidget()->height() - messageBarVerticalBottomMargin - q->minimumHeight()) {
		CALMessageBarManager::instance()->updateActionMap(q, false);
		q->deleteLater();
	}
}

QList<int> CALMessageBarPrivate::getOtherMessageBarTotalData(const bool isJudgeCreateOrder) {
	Q_Q(CALMessageBar);

	int minimumHeightTotal{ 0 };
	int indexLessCount{ 0 };
	QList<CALMessageBar*>* messageBarList = mapMessageBarActive[policy];
	for (const auto& messageBar : *messageBarList) {
		if (messageBar == q) {
			continue;
		}
		if (!isJudgeCreateOrder || judgeCreateDrder(messageBar)) {
			++indexLessCount;
			minimumHeightTotal += messageBar->minimumHeight();
		}
	}

	return { minimumHeightTotal, indexLessCount };
}

qreal CALMessageBarPrivate::calculateTargetPosY() {
	Q_Q(CALMessageBar);

	const QList<int> resultList = getOtherMessageBarTotalData(true);
	const int minimumHeightTotal = resultList.at(0);
	const int indexLessCount = resultList.at(1);
	switch (policy) {
		case ALMessageBarType::Top:
		case ALMessageBarType::TopRight:
		case ALMessageBarType::TopLeft: {
			return minimumHeightTotal + messageBarSpacing * indexLessCount + messageBarVerticalTopMargin;
		}
		case ALMessageBarType::Left:
		case ALMessageBarType::Right: {
			return minimumHeightTotal + messageBarSpacing * indexLessCount + q->parentWidget()->height() / 2.0;
		}
		case ALMessageBarType::Bottom:
		case ALMessageBarType::BottomLeft:
		case ALMessageBarType::BottomRight: {
			return q->parentWidget()->height() - q->minimumHeight() - minimumHeightTotal - messageBarSpacing * indexLessCount - messageBarVerticalBottomMargin;
		}
	}
	return 0;
}

bool CALMessageBarPrivate::judgeCreateDrder(CALMessageBar* otherMessageBar) const {
	return otherMessageBar->d_func()->createTime < createTime; // otherMessageBar 先创建
}

void CALMessageBarPrivate::drawMessage(QPainter* painter, const QColor& backgroundColor, const QColor& iconColor, const QString& iconText,
                                       const QColor& textColor, const int& iconPixelSize, const int& iconX, const QColor& penColor) {
	Q_Q(CALMessageBar);
	// 背景颜色
	painter->setBrush(backgroundColor);
	const QRect foregroundRect(shadowBorderWidth, shadowBorderWidth, q->width() - 2 * shadowBorderWidth, q->height() - 2 * shadowBorderWidth);
	painter->drawRoundedRect(foregroundRect, borderRadius, borderRadius);
	// 图标绘制
	painter->save();
	painter->setPen(penColor);
	QPainterPath textPath;
	textPath.addEllipse(QPoint(leftPadding + 6, q->height() / 2), 9, 9);
	painter->setClipPath(textPath);
	painter->fillPath(textPath, iconColor);
	QFont iconFont(ALIcon::getEnumTypeFontName(ALIcon::Awesome));
	iconFont.setPixelSize(iconPixelSize);
	painter->setFont(iconFont);
	painter->drawText(iconX, 0, q->width(), q->height(), Qt::AlignVCenter, iconText);
	painter->restore();
	// 文字颜色
	painter->setPen(textColor);
}

void CALMessageBarPrivate::drawSuccess(QPainter* painter) {
	drawMessage(painter, QColor(0xE0, 0xF6, 0xDD), QColor(0x11, 0x77, 0x10), QChar(static_cast<ushort>(ALIcon::AweSomeIcon::Check)),
	            Qt::black, 12, leftPadding);
}

void CALMessageBarPrivate::drawError(QPainter* painter) {
	drawMessage(painter, QColor(0xFE, 0xE7, 0xEA), QColor(0xBA, 0x2D, 0x20), QChar(static_cast<ushort>(ALIcon::AweSomeIcon::Close)),
	            Qt::black, 13, leftPadding + 1);
}

void CALMessageBarPrivate::drawWarning(QPainter* painter) {
	drawMessage(painter, QColor(0x6B, 0x56, 0x27), QColor(0xF8, 0xE2, 0x23), QString("!"),
	            QColor(0xFA, 0xFA, 0xFA), 12, leftPadding + 4, Qt::black);
}

void CALMessageBarPrivate::drawInfo(QPainter* painter) {
	drawMessage(painter, QColor(0xF4, 0xF4, 0xF4), QColor(0x00, 0x66, 0xB4), QString("i"),
	            Qt::black, 12, leftPadding + 4);
}

void CALMessageBarPrivate::setOpacity(const qreal opacity) {
	this->opacity = opacity;
	Q_EMIT sigOpacityChanged();
}

qreal CALMessageBarPrivate::getOpacity() const {
	return opacity;
}

void CALMessageBarPrivate::showMessageBar(const ALMessageBarType::PositionPolicy& positionPolicy, const ALMessageBarType::MessageLevel& messageLevel, const QString& title, const QString& message, const int displayMsec, QWidget* parent) {
	if (!parent) {
		QList<QWidget*> widgetList = QApplication::topLevelWidgets();
		for (const auto& widget : widgetList) {
			if (widget->property("CALBaseClassName").toString() == "CALMainWindow") {
				parent = widget;
				break;
			}
		}
		if (!parent) {
			return;
		}
	}

	const auto bar = new CALMessageBar(positionPolicy, messageLevel, title, message, displayMsec, parent);
#ifdef Q_OS_WIN
	// 显示置顶
	const auto hwnd = reinterpret_cast<HWND>(bar->getWinID());
	::SetWindowPos(hwnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
#else
	bar->setWindowFlags(bar->windowFlags() | Qt::WindowStaysOnTopHint);
	if (bar->isVisible()) {
		bar->show();
	}
#endif
}

/**
 * @brief \class CALMessageBar
 * @param policy 显示位置
 * @param messageLevel 消息类型
 * @param title 标题
 * @param message 消息
 * @param displayMsec 显示时间
 * @param parent pointer to the parent class
 */
CALMessageBar::CALMessageBar(const ALMessageBarType::PositionPolicy& policy, const ALMessageBarType::MessageLevel& messageLevel,
                             const QString& title, const QString& message, const int displayMsec, QWidget* parent)
: QWidget(parent), d_ptr(new CALMessageBarPrivate(this)) {
	Q_D(CALMessageBar);

	setObjectName("CALMessageBar");
	setStyleSheet("#CALMessageBar{ background-color: transparent; }");

	d->init();
	d->title = title;
	d->text = message;
	d->policy = policy;
	d->messageMode = messageLevel;
	d->opacity = 1;
	d->themeMode = ALTheme->getThemeMode();
	setFixedHeight(60);
	setMouseTracking(true);
	parent->installEventFilter(this);
	d->closeButton = new CALIconButton(CALIconFactory::createIconType(ALIcon::FluentIcon::ChromeClose), 10, d->closeButtonWidth, 30, this);
	switch (d->messageMode) {
		case ALMessageBarType::Success: {
			d->closeButton->setLightHoverColor(QColor(0xE6, 0xFC, 0xE3));
			d->closeButton->setDarkHoverColor(QColor(0xE6, 0xFC, 0xE3));
			d->closeButton->setDarkIconColor(Qt::white);
			break;
		}
		case ALMessageBarType::Warning: {
			d->closeButton->setLightHoverColor(QColor(0x5E, 0x4C, 0x22));
			d->closeButton->setDarkHoverColor(QColor(0x5E, 0x4C, 0x22));
			d->closeButton->setLightIconColor(Qt::white);
			d->closeButton->setDarkIconColor(Qt::white);
			break;
		}
		case ALMessageBarType::Info: {
			d->closeButton->setLightHoverColor(QColor(0xEB, 0xEB, 0xEB));
			d->closeButton->setDarkHoverColor(QColor(0xEB, 0xEB, 0xEB));
			d->closeButton->setDarkIconColor(Qt::black);
			break;
		}
		case ALMessageBarType::Error: {
			d->closeButton->setLightHoverColor(QColor(0xF7, 0xE1, 0xE4));
			d->closeButton->setDarkHoverColor(QColor(0xF7, 0xE1, 0xE4));
			d->closeButton->setDarkIconColor(Qt::black);
			break;
		}
		default: {
			break;
		}
	}
	d->closeButton->setBorderRadius(5);
	connect(d->closeButton, &CALIconButton::clicked, d, &CALMessageBarPrivate::slotCloseButtonClicked);
	const auto mainLayout = new QHBoxLayout(this);
	mainLayout->setContentsMargins(0, 0, 10, 0);
	mainLayout->addStretch();
	mainLayout->addWidget(d->closeButton);

	d->invokableMessageBarCreate(displayMsec);
}

CALMessageBar::~CALMessageBar() = default;

void CALMessageBar::success(const QString& title, const QString& message, const int displayMsec, const ALMessageBarType::PositionPolicy& positionPolicy, QWidget* parent) {
	CALMessageBarPrivate::showMessageBar(positionPolicy, ALMessageBarType::Success, title, message, displayMsec, parent);
}

void CALMessageBar::warning(const QString& title, const QString& message, const int displayMsec, const ALMessageBarType::PositionPolicy& positionPolicy, QWidget* parent) {
	CALMessageBarPrivate::showMessageBar(positionPolicy, ALMessageBarType::Warning, title, message, displayMsec, parent);
}

void CALMessageBar::information(const QString& title, const QString& message, const int displayMsec, const ALMessageBarType::PositionPolicy& positionPolicy, QWidget* parent) {
	CALMessageBarPrivate::showMessageBar(positionPolicy, ALMessageBarType::Info, title, message, displayMsec, parent);
}

void CALMessageBar::error(const QString& title, const QString& message, const int displayMsec, const ALMessageBarType::PositionPolicy& positionPolicy, QWidget* parent) {
	CALMessageBarPrivate::showMessageBar(positionPolicy, ALMessageBarType::Error, title, message, displayMsec, parent);
}

void CALMessageBar::paintEvent(QPaintEvent* event) {
	Q_D(CALMessageBar);

	QPainter painter(this);
	painter.setOpacity(d->opacity);
	painter.setRenderHints(QPainter::SmoothPixmapTransform | QPainter::Antialiasing | QPainter::TextAntialiasing);
	// 阴影
	ALTheme->drawEffectShadow(&painter, rect(), d->shadowBorderWidth, d->borderRadius);
	// 背景和图标绘制
	painter.save();
	painter.setPen(d->themeMode == ALThemeType::Light ? QColor(0xBE, 0xBA, 0xBE) : QColor(0x52, 0x50, 0x52));
	switch (d->messageMode) {
		case ALMessageBarType::Success: {
			d->drawSuccess(&painter);
			break;
		}
		case ALMessageBarType::Warning: {
			d->drawWarning(&painter);
			break;
		}
		case ALMessageBarType::Error: {
			d->drawError(&painter);
			break;
		}
		case ALMessageBarType::Info: {
			d->drawInfo(&painter);
			break;
		}
		default: break;
	}
	// 标题
	QFont font = this->font();
	font.setWeight(400);
	font.setPixelSize(16);
	painter.setFont(font);
	const int titleTextWidth = qMin(painter.fontMetrics().horizontalAdvance(d->title) + 1, 100);
	constexpr int textFlags = Qt::AlignLeft | Qt::AlignVCenter | Qt::TextWordWrap | Qt::TextWrapAnywhere;
	painter.drawText(QRect(d->leftPadding + d->titleLeftSpacing, -1, titleTextWidth, height()), textFlags, d->title);
	// 正文
	font.setWeight(QFont::Light);
	font.setPixelSize(15);
	painter.setFont(font);
	painter.drawText(QRect(d->leftPadding + d->titleLeftSpacing + titleTextWidth + d->textLeftSpacing, 0,
	                       width() - (d->leftPadding + d->titleLeftSpacing + titleTextWidth + d->textLeftSpacing + d->closeButtonWidth +
		                       d->closeButtonLeftRightMargin / 2), height()), textFlags, d->text);
	if (const int textHeight = painter.fontMetrics().boundingRect(
		QRect(d->leftPadding + d->titleLeftSpacing + titleTextWidth + d->textLeftSpacing, 0,
		      width() - (d->leftPadding + d->titleLeftSpacing + titleTextWidth + d->textLeftSpacing + d->closeButtonWidth + d->closeButtonLeftRightMargin),
		      height()), textFlags, d->text).height(); textHeight >= minimumHeight() - 20) {
		setMinimumHeight(textHeight + 20);
	}

	painter.restore();
}

bool CALMessageBar::eventFilter(QObject* watched, QEvent* event) {
	Q_D(CALMessageBar);

	if (watched == parentWidget()) {
		switch (event->type()) {
			case QEvent::Resize: {
				const auto resizeEvent = dynamic_cast<QResizeEvent*>(event);
				const QSize offsetSize = parentWidget()->size() - resizeEvent->oldSize();
				if (d->isNormalDisplay) {
					const int parentWidth = parentWidget()->width();
					switch (d->policy) {
						case ALMessageBarType::Top: {
							this->move(parentWidth / 2 - minimumWidth() / 2, this->y());
							break;
						}
						case ALMessageBarType::Bottom: {
							this->move(parentWidth / 2 - minimumWidth() / 2, this->pos().y() + offsetSize.height());
							break;
						}
						case ALMessageBarType::Left:
						case ALMessageBarType::TopLeft: {
							this->move(d->messageBarHorizontalMargin, this->pos().y());
							break;
						}
						case ALMessageBarType::BottomLeft: {
							this->move(d->messageBarHorizontalMargin, this->pos().y() + offsetSize.height());
							break;
						}
						case ALMessageBarType::Right:
						case ALMessageBarType::TopRight: {
							this->move(parentWidth - minimumWidth() - d->messageBarHorizontalMargin, this->y());
							break;
						}
						case ALMessageBarType::BottomRight: {
							this->move(parentWidth - minimumWidth() - d->messageBarHorizontalMargin,
							           this->pos().y() + offsetSize.height());
							break;
						}
						default: break;
					}
				}
				break;
			}
			default: {
				break;
			}
		}
	}

	return QWidget::eventFilter(watched, event);
}

WId CALMessageBar::getWinID() const {
#if 0
	return static_cast<WId>(d_func()->currentWinID);
#else
	return this->winId();
#endif
}

#if 0
#ifdef Q_OS_WIN
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
bool CALMessageBar::nativeEvent(const QByteArray& eventType, void* message, qintptr* result)
#else
bool CALMessageBar::nativeEvent(const QByteArray& eventType, void* message, long* result)
#endif
{
	Q_D(CALMessageBar);

	// 验证消息类型和指针有效性
	if ((eventType != "windows_generic_MSG") || !message) {
		return false;
	}

	const auto msg = static_cast<const MSG*>(message);
	HWND hwnd = msg->hwnd;
	if (!hwnd) {
		return false;
	}

	d->currentWinID = reinterpret_cast<qint64>(hwnd);
	const UINT uMsg = msg->message;
	const WPARAM wParam = msg->wParam;
	const LPARAM lParam = msg->lParam;

	switch (uMsg) {
		case WM_WINDOWPOSCHANGING: {
			if (const auto wp = reinterpret_cast<WINDOWPOS*>(lParam); wp != nullptr && (wp->flags & SWP_NOSIZE) == 0) {
				wp->flags |= SWP_NOCOPYBITS; // 防止复制位图（即窗口重绘时产生闪烁）

				if (const LRESULT defProcResult = ::DefWindowProcW(hwnd, uMsg, wParam, lParam);
					defProcResult <= LONG_MAX && defProcResult >= LONG_MIN) {
					*result = static_cast<long>(defProcResult);
				} else {
					return false;
				}
				return true;
			}
			return false;
		}
		case WM_NCACTIVATE: {
			*result = true; // 启用非客户区域的激活状态
			return true;
		}
		case WM_NCCALCSIZE: { // 计算客户区大小，决定窗口的边框或阴影
#if (QT_VERSION >= QT_VERSION_CHECK(6, 5, 3) && QT_VERSION <= QT_VERSION_CHECK(6, 6, 1))
			if (wParam == FALSE) {
				return false;
			}

			// 根据窗口状态调整内容边距
			if (::IsZoomed(hwnd)) {
				setContentsMargins(8, 8, 8, 8);
			} else {
				setContentsMargins(0, 0, 0, 0);
			}
			*result = 0;
			return true;
#else
			if (wParam == FALSE) {
				return false;
			}

			RECT* clientRect = &reinterpret_cast<NCCALCSIZE_PARAMS*>(lParam)->rgrc[0];
			if (!::IsZoomed(hwnd)) {
				clientRect->top -= 1;
				clientRect->bottom -= 1;
			}
			*result = WVR_REDRAW;
			return true;
#endif
		}
#if 0
		case WM_NCHITTEST: {
			POINT nativeLocalPos{ GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };
			::ScreenToClient(hwnd, &nativeLocalPos);
			RECT clientRect{ 0, 0, 0, 0 };
			::GetClientRect(hwnd, &clientRect);
			const auto clientWidth = clientRect.right - clientRect.left;
			const auto clientHeight = clientRect.bottom - clientRect.top;
			const bool left = nativeLocalPos.x < 8;
			const bool right = nativeLocalPos.x > clientWidth - 8;
			const bool top = nativeLocalPos.y < 8;
			const bool bottom = nativeLocalPos.y > clientHeight - 8;
			*result = 0;
			if (!window()->isFullScreen() && !window()->isMaximized()) {
				if (left && top) {
					*result = HTTOPLEFT;
				} else if (left && bottom) {
					*result = HTBOTTOMLEFT;
				} else if (right && top) {
					*result = HTTOPRIGHT;
				} else if (right && bottom) {
					*result = HTBOTTOMRIGHT;
				} else if (left) {
					*result = HTLEFT;
				} else if (right) {
					*result = HTRIGHT;
				} else if (top) {
					*result = HTTOP;
				} else if (bottom) {
					*result = HTBOTTOM;
				}
			}
			if (0 != *result) {
				return true;
			}
			// 默认情况下是客户区
			*result = HTCLIENT;
			return true;
		}
#endif
		default: break;
	}
	return QWidget::nativeEvent(eventType, message, result);
}
#endif
#endif
}
