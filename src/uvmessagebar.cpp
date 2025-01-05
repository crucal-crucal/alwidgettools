#include "uvmessagebar.hpp"

#include <QApplication>
#include <QGraphicsOpacityEffect>
#include <QHBoxLayout>
#include <QPainter>
#include <QPainterPath>
#include <QPropertyAnimation>
#include <QResizeEvent>
#include <QTimer>
#include <QVariant>

#include "uvawesomebutton.hpp"
#include "uvmessagebar_p.hpp"
#include "uvthememanager.hpp"

#ifdef Q_OS_WIN
#include <dwmapi.h>
#include <windowsx.h>
#endif

using namespace UVIcon;

QMap<UVMessageBarType::PositionPolicy, QList<CUVMessageBar*>*> CUVMessageBarPrivate::messageBarActiveMap{};

/**
 * @brief \class CUVMessageBarPrivate
 * Internal class for CUVMessageBar
 * @param q pointer to the public class
 * @param parent pointer to the parent class
 */
CUVMessageBarPrivate::CUVMessageBarPrivate(CUVMessageBar* q, QObject* parent): QObject(parent), q_ptr(q) {
	this->setProperty("MessagebarCloseY", 0);
	this->setProperty("MessageBarFinishY", 0);
}

CUVMessageBarPrivate::~CUVMessageBarPrivate() = default;

void CUVMessageBarPrivate::init() {
	borderRadius = 5;
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
	shadowBorderWidth = 0;

	messageBarIndex = 0;
	isMessageBarStartAnimationFinished = false;
	isMessageBarEventAnimationInStartAnimation = false;
	isCloseAnimationStart = false;
	isNormalDisplay = false;
	isMessageBarEventAnimationStart = false;
}

void CUVMessageBarPrivate::messageBarStartAnimation(const int displayMsec) {
	Q_Q(CUVMessageBar);

	q->show();
	QFont font = q->font();
	font.setPixelSize(16);
	font.setWeight(QFont::Bold);
	q->setFont(font);
	const int titleWidth = q->fontMetrics().horizontalAdvance(this->title);
	font.setPixelSize(14);
	font.setWeight(QFont::Medium);
	q->setFont(font);
	const int textWidth = q->fontMetrics().horizontalAdvance(this->text);
	const int fixedWidth = this->closeButtonLeftRightMargin + this->leftPadding + this->titleLeftSpacing * 2 + this->closeButtonWidth
	                       + titleWidth + textWidth + 2 * this->shadowBorderWidth;
	q->setFixedWidth(fixedWidth > 600 ? 600 : fixedWidth);
	updateActiveMap(true); // 计算坐标前增加
	int startX, startY, endX, endY;
	calculatePos(startX, startY, endX, endY);
	// 划入动画
	const auto barPosAnimation = new QPropertyAnimation(q, "pos");
	connect(barPosAnimation, &QPropertyAnimation::finished, q, [=]() {
		this->isNormalDisplay = true;
		this->isMessageBarStartAnimationFinished = true;
		if (this->isMessageBarEventAnimationInStartAnimation) {
			messageBarEventAnimation();
		}
		QTimer::singleShot(displayMsec, q, [=]() {
			this->isCloseAnimationStart = true;
			if (!this->isMessageBarEventAnimationStart) {
				messageBarFinishAnimation();
			}
		});
	});

	switch (this->policy) {
		case UVMessageBarType::Top:
		case UVMessageBarType::Bottom: {
			barPosAnimation->setDuration(200);
			break;
		}
		case UVMessageBarType::Left:
		case UVMessageBarType::Right:
		case UVMessageBarType::TopRight:
		case UVMessageBarType::TopLeft:
		case UVMessageBarType::BottomRight:
		case UVMessageBarType::BottomLeft: {
			barPosAnimation->setDuration(250);
			break;
		}
		default: break;
	}

	barPosAnimation->setStartValue(QPoint(startX, startY));
	barPosAnimation->setEndValue(QPoint(endX, endY));
	barPosAnimation->setEasingCurve(QEasingCurve::InOutSine);
	barPosAnimation->start(QAbstractAnimation::DeleteWhenStopped);
}

void CUVMessageBarPrivate::messageBarFinishAnimation() {
	Q_Q(CUVMessageBar);

	const auto barFinishedOpacityAnimation = new QPropertyAnimation(q->graphicsEffect(), "opacity");
	connect(barFinishedOpacityAnimation, &QPropertyAnimation::finished, this, [=]() {
		q->deleteLater();
	});
	barFinishedOpacityAnimation->setDuration(200);
	barFinishedOpacityAnimation->setEasingCurve(QEasingCurve::InOutSine);
	barFinishedOpacityAnimation->setStartValue(1);
	barFinishedOpacityAnimation->setEndValue(0);
	barFinishedOpacityAnimation->start(QAbstractAnimation::DeleteWhenStopped);
	updateActiveMap(false);
	emit sigMessageBarClosed(policy, messageBarIndex);
}

void CUVMessageBarPrivate::messageBarEventAnimation() {
	Q_Q(CUVMessageBar);

	this->isMessageBarEventAnimationStart = true;
	const auto closePosAnimation = new QPropertyAnimation(this, "MessagebarCloseY");
	connect(closePosAnimation, &QPropertyAnimation::valueChanged, this, [=](const QVariant& value) {
		q->move(q->pos().x(), value.toInt());
	});
	connect(closePosAnimation, &QPropertyAnimation::finished, this, [=]() {
		if (this->isCloseAnimationStart) {
			messageBarFinishAnimation();
		} else {
			this->isMessageBarEventAnimationStart = false;
		}
	});

	closePosAnimation->setEasingCurve(QEasingCurve::InOutSine);
	closePosAnimation->setDuration(200);
	closePosAnimation->setStartValue(q->pos().y());
	const int minimumHeightTotal = calculateMinimumHeightTotal(true);
	const int spacing = this->messageBarSpacing * (this->messageBarIndex - 1);
	const int parentHeight = q->parentWidget()->height();
	const int minimumHeight = q->minimumHeight();
	switch (this->policy) {
		case UVMessageBarType::Top: {
			closePosAnimation->setEndValue(minimumHeightTotal + spacing + this->messageBarVerticalTopMargin);
			break;
		}
		case UVMessageBarType::Left: {
			closePosAnimation->setEndValue(minimumHeightTotal + spacing + parentHeight / 2);
			break;
		}
		case UVMessageBarType::Bottom: {
			closePosAnimation->setEndValue(parentHeight - minimumHeight - minimumHeightTotal - spacing - this->messageBarVerticalBottomMargin);
			break;
		}
		case UVMessageBarType::Right: {
			closePosAnimation->setEndValue(minimumHeightTotal + spacing + parentHeight / 2);
			break;
		}
		case UVMessageBarType::TopRight:
		case UVMessageBarType::TopLeft: {
			closePosAnimation->setEndValue(minimumHeightTotal + spacing + this->messageBarVerticalTopMargin);
			break;
		}
		case UVMessageBarType::BottomRight:
		case UVMessageBarType::BottomLeft: {
			closePosAnimation->setEndValue(parentHeight - minimumHeight - minimumHeightTotal - spacing - messageBarVerticalBottomMargin);
			break;
		}
		default: {
			break;
		}
	}

	closePosAnimation->start(QAbstractAnimation::DeleteWhenStopped);
}

void CUVMessageBarPrivate::updateActiveMap(const bool isActive) {
	Q_Q(CUVMessageBar);

	if (isActive) {
		if (messageBarActiveMap.contains(this->policy)) {
			messageBarActiveMap[this->policy]->append(q);
		} else {
			const auto messageBarList = new QList<CUVMessageBar*>();
			messageBarList->append(q);
			messageBarActiveMap.insert(this->policy, messageBarList);
		}
		this->messageBarIndex = messageBarActiveMap[this->policy]->count();
	} else {
		if (messageBarActiveMap.contains(this->policy)) {
			if (!messageBarActiveMap[this->policy]->isEmpty()) {
				messageBarActiveMap[this->policy]->removeOne(q);
			}
		}
	}
}

void CUVMessageBarPrivate::calculatePos(int& startX, int& startY, int& endX, int& endY) {
	Q_Q(CUVMessageBar);

	const int minimumHeightTotal = this->calculateMinimumHeightTotal();
	const int spacing = this->messageBarSpacing * (this->messageBarIndex - 1);
	switch (this->policy) {
		case UVMessageBarType::Top: {
			// 25动画距离
			startX = q->parentWidget()->width() / 2 - q->minimumWidth() / 2;
			startY = minimumHeightTotal + spacing + this->messageBarVerticalTopMargin - 25;
			endX = startX;
			endY = minimumHeightTotal + spacing + this->messageBarVerticalTopMargin;
			break;
		}
		case UVMessageBarType::Left: {
			startX = -q->minimumWidth();
			startY = minimumHeightTotal + spacing + q->parentWidget()->height() / 2;
			endX = this->messageBarHorizontalMargin;
			endY = startY;
			break;
		}
		case UVMessageBarType::Bottom: {
			startX = q->parentWidget()->width() / 2 - q->minimumWidth() / 2;
			startY = q->parentWidget()->height() - q->minimumHeight() - minimumHeightTotal - spacing - this->messageBarVerticalBottomMargin - 25;
			endX = startX;
			endY = q->parentWidget()->height() - q->minimumHeight() - minimumHeightTotal - spacing - this->messageBarVerticalBottomMargin;
			break;
		}
		case UVMessageBarType::Right: {
			startX = q->parentWidget()->width();
			startY = minimumHeightTotal + spacing + q->parentWidget()->height() / 2;
			endX = q->parentWidget()->width() - q->minimumWidth() - this->messageBarHorizontalMargin;
			endY = startY;
			break;
		}
		case UVMessageBarType::TopRight: {
			startX = q->parentWidget()->width();
			startY = minimumHeightTotal + spacing + this->messageBarVerticalTopMargin;
			endX = q->parentWidget()->width() - q->minimumWidth() - this->messageBarHorizontalMargin;
			endY = startY;
			break;
		}
		case UVMessageBarType::TopLeft: {
			startX = -q->minimumWidth();
			startY = minimumHeightTotal + spacing + this->messageBarVerticalTopMargin;
			endX = this->messageBarHorizontalMargin;
			endY = startY;
			break;
		}
		case UVMessageBarType::BottomRight: {
			startX = q->parentWidget()->width();
			startY = q->parentWidget()->height() - q->minimumHeight() - minimumHeightTotal - spacing - this->messageBarVerticalBottomMargin;
			endX = q->parentWidget()->width() - q->minimumWidth() - this->messageBarHorizontalMargin;
			endY = startY;
			break;
		}
		case UVMessageBarType::BottomLeft: {
			startX = -q->minimumWidth();
			startY = q->parentWidget()->height() - q->minimumHeight() - minimumHeightTotal - spacing - this->messageBarVerticalBottomMargin;
			endX = this->messageBarHorizontalMargin;
			endY = startY;
			break;
		}
		default: {
			break;
		}
	}

	if (endY < this->messageBarVerticalTopMargin || endY > q->parentWidget()->height() - this->messageBarVerticalBottomMargin - q->minimumHeight()) {
		updateActiveMap(false);
		q->deleteLater();
	}
}

int CUVMessageBarPrivate::calculateMinimumHeightTotal(const bool isJudgeIndex) {
	Q_Q(CUVMessageBar);

	int minimumHeightTotal = 0;
	const auto messageBarList = CUVMessageBarPrivate::messageBarActiveMap[this->policy];
	if (isJudgeIndex) {
		for (const auto& messageBar: *messageBarList) {
			if (messageBar == q) {
				continue;
			}
			if (messageBar->d_ptr->messageBarIndex < messageBarIndex) {
				minimumHeightTotal += messageBar->minimumHeight();
			}
		}
	} else {
		for (const auto& messageBar: *messageBarList) {
			if (messageBar == q) {
				continue;
			}
			minimumHeightTotal += messageBar->minimumHeight();
		}
	}

	return minimumHeightTotal;
}

void CUVMessageBarPrivate::drawMessage(QPainter* painter, const QColor& backgroundColor, const QColor& iconColor, const QString& iconText,
                                       const QColor& textColor, const int iconPixelSize, const int iconX, const QColor& penColor) {
	Q_Q(CUVMessageBar);
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

	auto iconFont = QFont("CUVAwesome");
	iconFont.setPixelSize(iconPixelSize);
	painter->setFont(iconFont);
	painter->drawText(iconX, 0, q->width(), q->height(), Qt::AlignVCenter, iconText);
	painter->restore();

	// 文字颜色
	painter->setPen(textColor);
}

void CUVMessageBarPrivate::drawSuccess(QPainter* painter) {
	drawMessage(painter, QColor(45, 45, 45), QColor(0x11, 0x77, 0x10), QChar(static_cast<ushort>(CUVAweSomeIcon::Check)),
	            Qt::white, 12, leftPadding);
}

void CUVMessageBarPrivate::drawError(QPainter* painter) {
	drawMessage(painter, QColor(0xFE, 0xE7, 0xEA), QColor(0xBA, 0x2D, 0x20), QChar(static_cast<ushort>(CUVAweSomeIcon::Close)),
	            Qt::white, 13, leftPadding + 1);
}

void CUVMessageBarPrivate::drawWarning(QPainter* painter) {
	drawMessage(painter, QColor(0x6B, 0x56, 0x27), QColor(0xF8, 0xE2, 0x23), QString("!"),
	            QColor(0xFA, 0xFA, 0xFA), 12, leftPadding + 4, Qt::black);
}

void CUVMessageBarPrivate::drawInfo(QPainter* painter) {
	drawMessage(painter, QColor(45, 45, 45), QColor(0x00, 0x66, 0xB4), QString("i"),
	            Qt::white, 12, leftPadding + 4);
}

void CUVMessageBarPrivate::showMessageBar(const UVMessageBarType::PositionPolicy& positionPolicy, const UVMessageBarType::MessageLevel& messageMode, const QString& title, const QString& message,
                                          const int displayMsec, QWidget* parent) {
	if (!parent) {
		const auto widgetList = QApplication::topLevelWidgets();
		for (const auto& widget: widgetList) {
			if (widget->property("CUVBaseClassName").toString() == "CUVWindow") {
				parent = widget;
			}
		}
		if (!parent) {
			return;
		}
	}

	const auto bar = new CUVMessageBar(positionPolicy, messageMode, title, message, displayMsec, parent);
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

void CUVMessageBarPrivate::slotCloseButtonClicked() {
	Q_Q(CUVMessageBar);

	if (this->isCloseAnimationStart) {
		return;
	}
	this->isCloseAnimationStart = true;
	const auto opacityAnimation = new QPropertyAnimation(q->graphicsEffect(), "opacity");
	connect(opacityAnimation, &QPropertyAnimation::finished, q, [=]() { q->deleteLater(); });
	opacityAnimation->setStartValue(dynamic_cast<QGraphicsOpacityEffect*>(q->graphicsEffect())->opacity());
	opacityAnimation->setEndValue(0);
	opacityAnimation->setDuration(220);
	opacityAnimation->setEasingCurve(QEasingCurve::InOutSine);
	opacityAnimation->start(QAbstractAnimation::DeleteWhenStopped);
	this->isNormalDisplay = false;
	updateActiveMap(false);
	emit sigMessageBarClosed(this->policy, this->messageBarIndex);
}

void CUVMessageBarPrivate::slotOtherMessageBarClosed(const UVMessageBarType::PositionPolicy positionPolicy, const int barIndex) {
	if (this->policy == positionPolicy && !isCloseAnimationStart && this->messageBarIndex > barIndex) {
		this->messageBarIndex -= 1;
		if (!this->isMessageBarStartAnimationFinished) {
			this->isMessageBarEventAnimationInStartAnimation = true;
			return;
		}
		messageBarEventAnimation();
	}
}

/**
 * @brief \class CUVMessageBar
 * @param policy 显示位置
 * @param messageLevel 消息类型
 * @param title 标题
 * @param message 消息
 * @param displayMsec 显示时间
 * @param parent pointer to the parent class
 */
CUVMessageBar::CUVMessageBar(const UVMessageBarType::PositionPolicy& policy, const UVMessageBarType::MessageLevel& messageLevel,
                             const QString& title, const QString& message, const int displayMsec, QWidget* parent)
: QWidget(parent), d_ptr(new CUVMessageBarPrivate(this)) {
	d_func()->init();
	Q_D(CUVMessageBar);

	d->title = title;
	d->text = message;
	d->policy = policy;
	d->messageMode = messageLevel;
	setFixedHeight(60);
	setMouseTracking(true);
	const auto effect = new QGraphicsOpacityEffect(this);
	effect->setOpacity(1);
	setGraphicsEffect(effect);
	setFont(QFont("Source Han Sans SC Normal"));
	parent->installEventFilter(this);
	d->closeButton = new CUVAwesomeButton(CUVAweSomeIcon::Close, 17, d->closeButtonWidth, 30, this);
	switch (d->messageMode) {
		case UVMessageBarType::Success: {
			d->closeButton->setLightHoverColor(QColor(0xE6, 0xFC, 0xE3));
			d->closeButton->setDarkHoverColor(QColor(30, 36, 51));
			d->closeButton->setDarkIconColor(Qt::white);
			break;
		}
		case UVMessageBarType::Warning: {
			d->closeButton->setLightHoverColor(QColor(0x5E, 0x4C, 0x22));
			d->closeButton->setDarkHoverColor(QColor(0x5E, 0x4C, 0x22));
			d->closeButton->setLightIconColor(Qt::white);
			d->closeButton->setDarkIconColor(Qt::white);
			break;
		}
		case UVMessageBarType::Info: {
			d->closeButton->setLightHoverColor(QColor(0xEB, 0xEB, 0xEB));
			d->closeButton->setDarkHoverColor(QColor(30, 36, 51));
			d->closeButton->setDarkIconColor(Qt::white);
			break;
		}
		case UVMessageBarType::Error: {
			d->closeButton->setLightHoverColor(QColor(0xF7, 0xE1, 0xE4));
			d->closeButton->setDarkHoverColor(QColor(0xF7, 0xE1, 0xE4));
			d->closeButton->setDarkIconColor(Qt::black);
			break;
		}
	}
	d->closeButton->setBorderRadius(5);
	connect(d->closeButton, &CUVAwesomeButton::clicked, d, &CUVMessageBarPrivate::slotCloseButtonClicked);
	const auto mainLayout = new QHBoxLayout(this);
	mainLayout->setContentsMargins(0, 0, 10, 0);
	mainLayout->addStretch();
	mainLayout->addWidget(d->closeButton);
	setObjectName("CUVMessageBar");
	setStyleSheet("#CUVMessageBar{ background-color: transparent; }");

	d->messageBarStartAnimation(displayMsec);

	const auto messageBarList = CUVMessageBarPrivate::messageBarActiveMap.value(d->policy);
	for (const auto& otherMessageBar: *messageBarList) {
		connect(otherMessageBar->d_func(), &CUVMessageBarPrivate::sigMessageBarClosed, d,
		        &CUVMessageBarPrivate::slotOtherMessageBarClosed);
	}
}

CUVMessageBar::~CUVMessageBar() = default;

void CUVMessageBar::success(const QString& title, const QString& message, const int displayMsec, const UVMessageBarType::PositionPolicy& positionPolicy, QWidget* parent) {
	CUVMessageBarPrivate::showMessageBar(positionPolicy, UVMessageBarType::Success, title, message, displayMsec, parent);
}

void CUVMessageBar::warning(const QString& title, const QString& message, const int displayMsec, const UVMessageBarType::PositionPolicy& positionPolicy, QWidget* parent) {
	CUVMessageBarPrivate::showMessageBar(positionPolicy, UVMessageBarType::Warning, title, message, displayMsec, parent);
}

void CUVMessageBar::information(const QString& title, const QString& message, const int displayMsec, const UVMessageBarType::PositionPolicy& positionPolicy, QWidget* parent) {
	CUVMessageBarPrivate::showMessageBar(positionPolicy, UVMessageBarType::Info, title, message, displayMsec, parent);
}

void CUVMessageBar::error(const QString& title, const QString& message, const int displayMsec, const UVMessageBarType::PositionPolicy& positionPolicy, QWidget* parent) {
	CUVMessageBarPrivate::showMessageBar(positionPolicy, UVMessageBarType::Error, title, message, displayMsec, parent);
}

void CUVMessageBar::paintEvent(QPaintEvent* event) {
	Q_D(CUVMessageBar);

	QPainter painter(this);
	// painter.setOpacity(d._)
	painter.setRenderHints(QPainter::SmoothPixmapTransform | QPainter::Antialiasing | QPainter::TextAntialiasing);

	UVTheme->drawEffectShadow(&painter, rect(), d->shadowBorderWidth, d->borderRadius);

	painter.save();
	// 背景和图标绘制
	painter.setPen(Qt::NoPen);
	switch (d->messageMode) {
		case UVMessageBarType::Success: {
			d->drawSuccess(&painter);
			break;
		}
		case UVMessageBarType::Warning: {
			d->drawWarning(&painter);
			break;
		}
		case UVMessageBarType::Error: {
			d->drawError(&painter);
			break;
		}
		case UVMessageBarType::Info: {
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
	int titleTextWidth = painter.fontMetrics().horizontalAdvance(d->title);
	titleTextWidth = qMin(titleTextWidth, 150);
	constexpr int textFlags = Qt::AlignLeft | Qt::AlignVCenter | Qt::TextWordWrap;
	painter.drawText(QRect(d->leftPadding + d->titleLeftSpacing, -1, titleTextWidth, height()), textFlags, d->title);
	// 正文
	font.setWeight(400);
	font.setPixelSize(14);
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

bool CUVMessageBar::eventFilter(QObject* watched, QEvent* event) {
	Q_D(CUVMessageBar);

	if (watched == parentWidget()) {
		switch (event->type()) {
			case QEvent::Resize: {
				const auto resizeEvent = dynamic_cast<QResizeEvent*>(event);
				const QSize offsetSize = parentWidget()->size() - resizeEvent->oldSize();
				if (d->isNormalDisplay) {
					const int parentWidth = parentWidget()->width();
					switch (d->policy) {
						case UVMessageBarType::Top: {
							this->move(parentWidth / 2 - minimumWidth() / 2, this->y());
							break;
						}
						case UVMessageBarType::Bottom: {
							this->move(parentWidth / 2 - minimumWidth() / 2, this->pos().y() + offsetSize.height());
							break;
						}
						case UVMessageBarType::Left:
						case UVMessageBarType::TopLeft: {
							this->move(d->messageBarHorizontalMargin, this->pos().y());
							break;
						}
						case UVMessageBarType::BottomLeft: {
							this->move(d->messageBarHorizontalMargin, this->pos().y() + offsetSize.height());
							break;
						}
						case UVMessageBarType::Right:
						case UVMessageBarType::TopRight: {
							this->move(parentWidth - minimumWidth() - d->messageBarHorizontalMargin, this->y());
							break;
						}
						case UVMessageBarType::BottomRight: {
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

WId CUVMessageBar::getWinID() const {
#if 0
	return static_cast<WId>(d_func()->currentWinID);
#else
	return this->winId();
#endif
}

#if 0
#ifdef Q_OS_WIN
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
bool CUVMessageBar::nativeEvent(const QByteArray& eventType, void* message, qintptr* result)
#else
bool CUVMessageBar::nativeEvent(const QByteArray& eventType, void* message, long* result)
#endif
{
	Q_D(CUVMessageBar);

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
