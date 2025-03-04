#include "altooltip.hpp"

#include <QApplication>
#include <QDebug>
#include <QEvent>
#include <QPainter>
#include <QPainterPath>
#include <QPropertyAnimation>
#include <QScreen>
#include <QTimer>
#include <QVBoxLayout>

#include "altext.hpp"
#include "althememanager.hpp"
#include "altooltip_p.hpp"

/**
 * @brief \namespace AL
 */
namespace AL {
QScreen* getCurrentScreen() {
	for (const auto& screen : QApplication::screens()) {
		if (screen->geometry().contains(QCursor::pos())) {
			return screen;
		}
	}

	return nullptr;
}

QRect getCurrentScreenGeometry(const bool avalible = true) {
	const QScreen* screen = getCurrentScreen();
	if (!screen) {
		screen = QApplication::primaryScreen();
	}

	if (!screen) {
		return { 0, 0, 1920, 1080 };
	}

	return avalible ? screen->availableGeometry() : screen->geometry();
}

/**
 * @brief \class CALToolTipPrivate
 * Internal class for CALToolTip
 * @param q pointer to the public class
 * @param parent pointer to the parent class
 */
CALToolTipPrivate::CALToolTipPrivate(CALToolTip* q, QObject* parent): QObject(parent), q_ptr(q) {
	opacity = -1;
}

CALToolTipPrivate::~CALToolTipPrivate() = default;

void CALToolTipPrivate::setOpacity(const qreal opacity) {
	if (this->opacity != opacity) {
		this->opacity = opacity;
		Q_EMIT sigOpacityChanged();
	}
}

qreal CALToolTipPrivate::getOpacity() const {
	return opacity;
}

bool CALToolTipPrivate::eventFilter(QObject* watched, QEvent* event) {
	Q_Q(CALToolTip);

	switch (event->type()) {
		// case QEvent::Enter:
		case QEvent::ToolTip: {
			if (!q->isVisible()) {
				QTimer::singleShot(duration, this, [this]() { doShowAnimation(); });
				if (duration > -1) {
					QTimer::singleShot(duration, this, [q]() { q->hide(); });
				}
			}
			break;
		}
		case QEvent::Leave:
		case QEvent::Hide: {
			q->hide();
			break;
		}
		case QEvent::HoverMove:
		case QEvent::MouseMove: {
			updatePos();
			break;
		}
		default: break;
	}
	return QObject::eventFilter(watched, event);
}

void CALToolTipPrivate::doShowAnimation() {
	Q_Q(CALToolTip);

	if (toolTipText->text().isEmpty() && !customWidget) {
		return;
	}

	const QPoint cursorPoint = QCursor::pos();
	const QScreen* screen = QGuiApplication::screenAt(cursorPoint);
	if (!screen) {
		qWarning() << "Failed to get the screen!";
		return;
	}

	// 获取当前屏幕的可视区域
	const QRect availableGeometry = screen->availableGeometry();

	// 计算新位置，确保 Tooltip 不会超出可视区域
	int newX = qMin(cursorPoint.x() + 10, availableGeometry.right() - q->width());
	int newY = qMin(cursorPoint.y(), availableGeometry.bottom() - q->height());

	// 防止 Tooltip 显示在屏幕外的情况
	newX = qMax(newX, availableGeometry.left());
	newY = qMax(newY, availableGeometry.top());

	q->move(newX, newY);
	q->show();
	const auto showAnimation = new QPropertyAnimation(q, "windowOpacity");
	showAnimation->setEasingCurve(QEasingCurve::InOutSine);
	showAnimation->setDuration(250);
	showAnimation->setStartValue(0);
	showAnimation->setEndValue(1);
	showAnimation->start(QAbstractAnimation::DeleteWhenStopped);
}

void CALToolTipPrivate::updatePos() {
	Q_Q(CALToolTip);

	if (q->isVisible()) {
		if (!toolTipText->text().isEmpty()) {
			q->resize(q->fontMetrics().horizontalAdvance(toolTipText->text()), q->height());
		} else if (customWidget) {
			q->resize(customWidget->size());
		}

		const QPoint cursorPoint = QCursor::pos();
		const QScreen* screen = QGuiApplication::screenAt(cursorPoint);
		if (!screen) {
			qWarning() << "Failed to get the screen!";
			return;
		}

		const QRect availableGeometry = screen->availableGeometry();

		// 计算新位置，确保 Tooltip 不会超出可视区域
		int newX = qMin(cursorPoint.x() + 10, availableGeometry.right() - q->width());
		int newY = qMin(cursorPoint.y(), availableGeometry.bottom() - q->height());

		// 防止 Tooltip 显示在屏幕外的情况
		newX = qMax(newX, availableGeometry.left());
		newY = qMax(newY, availableGeometry.top());

		q->move(newX, newY);
	}
}

/**
 * @brief \class CALToolTip
 * @param installFilterToShow 是否启用自身的 eventFilter 来显示
 * @param parent pointer to the parent class
 */
CALToolTip::CALToolTip(QWidget* parent, const bool installFilterToShow): QWidget(parent), d_ptr(new CALToolTipPrivate(this, this)) {
	Q_D(CALToolTip);

	d->borderRadius = 5;
	d->shadowBorderWidth = 6;
	d->duration = 1000;
	d->installFilterToShow = installFilterToShow;
	d->timer = new QTimer(this);
	d->opacityAnimation = new QPropertyAnimation(this, "windowOpacity", this);
	d->opacityAnimation->setDuration(150);
	d->customWidget = nullptr;
	setObjectName("CALToolTip");
	if (parent && installFilterToShow) {
		parent->installEventFilter(d);
	}

	setAttribute(Qt::WA_TransparentForMouseEvents);
	setAttribute(Qt::WA_TranslucentBackground);
	setWindowFlags(Qt::Tool | Qt::FramelessWindowHint);

	d->toolTipText = new CALText(this);
	d->toolTipText->setWordWrap(false);
	d->toolTipText->setTextPixelSize(17);
	d->mainVLayout = new QVBoxLayout(this);
	d->mainVLayout->setContentsMargins(d->shadowBorderWidth * 2, d->shadowBorderWidth * 2, d->shadowBorderWidth * 2, d->shadowBorderWidth * 2);
	d->mainVLayout->addWidget(d->toolTipText);

	d->themeMode = ALTheme->getThemeMode();
	connect(ALTheme, &CALThemeManager::sigThemeModeChanged, this, [d, this](const ALThemeType::ThemeMode& mode) {
		d->themeMode = mode;
		update();
	});
}

CALToolTip::~CALToolTip() = default;

void CALToolTip::setBorderRadius(const qreal borderRadius) {
	Q_D(CALToolTip);

	if (d->borderRadius != borderRadius) {
		d->borderRadius = borderRadius;
		Q_EMIT sigBorderRadiusChanged();
	}
}

qreal CALToolTip::getBorderRadius() const {
	Q_D(const CALToolTip);

	return d->borderRadius;
}

void CALToolTip::setText(const QString& text) {
	Q_D(CALToolTip);

	resize(fontMetrics().horizontalAdvance(text), height());
	d->toolTipText->setText(text);
	d->text = text;
	Q_EMIT sigTextChanged();
}

QString CALToolTip::getText() const {
	Q_D(const CALToolTip);

	return d->text;
}

void CALToolTip::setDuration(const int duration) {
	d_func()->duration = duration;
}

int CALToolTip::getDuration() const {
	return d_func()->duration;
}

void CALToolTip::setCustomWidget(QWidget* customWidget) {
	Q_D(CALToolTip);

	if (!customWidget || customWidget == this) {
		return;
	}

	if (d->customWidget) {
		d->mainVLayout->removeWidget(d->customWidget);
		d->customWidget->deleteLater();
	}

	d->toolTipText->hide();
	d->mainVLayout->addWidget(customWidget);
	d->customWidget = customWidget;
	Q_EMIT sigCustomWidgetChanged();
}

QWidget* CALToolTip::getCustomWidget() const {
	Q_D(const CALToolTip);

	return d->customWidget;
}

void CALToolTip::updatePos() {
	d_func()->updatePos();
}

void CALToolTip::adjustPos(QWidget* widget, const ALToolTipType::Position& position) {
	move(CALToolTipPositionManager::create(position)->position(this, widget));
}

void CALToolTip::paintEvent(QPaintEvent* event) {
	Q_D(CALToolTip);

	QPainter painter(this);
	painter.save();
	painter.setRenderHint(QPainter::Antialiasing);
	// 绘制阴影
	ALTheme->drawEffectShadow(&painter, rect(), d->shadowBorderWidth, d->borderRadius);
	QRect forefroundRect = rect();
	forefroundRect.adjust(d->shadowBorderWidth, d->shadowBorderWidth, -d->shadowBorderWidth, -d->shadowBorderWidth);
	painter.setPen(ALThemeColor(d->themeMode, ALThemeType::PopupBorder));
	painter.setBrush(ALThemeColor(d->themeMode, ALThemeType::PopupBase));
	painter.drawRoundedRect(forefroundRect, d->borderRadius, d->borderRadius);
	painter.restore();
}

void CALToolTip::showEvent(QShowEvent* event) {
	Q_D(CALToolTip);

	if (!d->installFilterToShow) {
		d->opacityAnimation->setStartValue(0);
		d->opacityAnimation->setEndValue(1);
		d->opacityAnimation->start();
		d->timer->stop();
		if (d->duration > 0) {
			d->timer->start(d->duration + d->opacityAnimation->duration());
		}
	}

	QWidget::showEvent(event);
}

void CALToolTip::hideEvent(QHideEvent* event) {
	Q_D(CALToolTip);

	if (!d->installFilterToShow) {
		d_func()->timer->stop();
	}
	QWidget::hideEvent(event);
}

/**
 * \class CALToolTipPositionManager
 * @param tooltip pointer to CALToolTip
 * @param parent pointer to the need calculate widget
 * @return new pos
 */
QPoint CALToolTipPositionManager::position(CALToolTip* tooltip, QWidget* parent) {
	const QPoint pos = calculatePosition(tooltip, parent);
	const QRect rect = getCurrentScreenGeometry();
	const int x = qMax(rect.left(), qMin(pos.x(), rect.right() - tooltip->width() - 4));
	const int y = qMax(rect.top(), qMin(pos.y(), rect.bottom() - tooltip->height() - 4));
	return { x, y };
}

std::unique_ptr<CALToolTipPositionManager> CALToolTipPositionManager::create(const ALToolTipType::Position& position) {
	static const std::unordered_map<ALToolTipType::Position, std::function<std::unique_ptr<CALToolTipPositionManager>()>> managers = {
		{ ALToolTipType::Position::Top, []() { return std::make_unique<CALTopToolTipManager>(); } },
		{ ALToolTipType::Position::Bottom, []() { return std::make_unique<CALBottomToolTipManager>(); } },
		{ ALToolTipType::Position::Left, []() { return std::make_unique<CALLeftToolTipManager>(); } },
		{ ALToolTipType::Position::Right, []() { return std::make_unique<CALRightToolTipManager>(); } },
		{ ALToolTipType::Position::TopRight, []() { return std::make_unique<CALTopRightToolTipManager>(); } },
		{ ALToolTipType::Position::BottomRight, []() { return std::make_unique<CALBottomRightToolTipManager>(); } },
		{ ALToolTipType::Position::TopLeft, []() { return std::make_unique<CALTopLeftToolTipManager>(); } },
		{ ALToolTipType::Position::BottomLeft, []() { return std::make_unique<CALBottomLeftToolTipManager>(); } }
	};

	const auto it = managers.find(position);
	if (it == managers.end()) {
		throw std::invalid_argument("Invalid tooltip position.");
	}

	return it->second();
}

/**
 * \class CALTopToolTipManager
 * @brief Manages the positioning of tooltips above the parent widget
 *
 * 该类计算 tooltip 的位置，使其显示在父窗口附件的正上方，并且水平居中
 *
 * @param tooltip pointer to CALToolTip
 * @param parent pointer to the need calculate widget
 * @return new pos
 */
QPoint CALTopToolTipManager::calculatePosition(CALToolTip* tooltip, QWidget* parent) {
	const QPoint pos = parent->mapToGlobal(QPoint());
	const int x = pos.x() + parent->width() / 2 - tooltip->width() / 2;
	const int y = pos.y() - tooltip->height();

	return { x, y };
}

/**
 * \class CALBottomToolTipManager
 * @brief Manages the positioning of tooltips below the parent widget
 *
 * 该类计算 tooltip 的位置，使其显示在父窗口的正下方，并水平居中
 *
 * @param tooltip pointer to CALToolTip
 * @param parent pointer to the need calculate widget
 * @return new pos
 */
QPoint CALBottomToolTipManager::calculatePosition(CALToolTip* tooltip, QWidget* parent) {
	const QPoint pos = parent->mapToGlobal(QPoint());
	const int x = pos.x() + parent->width() / 2 - tooltip->width() / 2;
	const int y = pos.y() + parent->height();

	return { x, y };
}

/**
 * \class CALLeftToolTipManager
 * @brief Manages the positioning of tooltips to the left of the parent widget.
 *
 * 该类计算 tooltip 的位置，使其显示在父窗口的左侧，并垂直居中
 *
 * @param tooltip pointer to CALToolTip
 * @param parent pointer to the need calculate widget
 * @return
 */
QPoint CALLeftToolTipManager::calculatePosition(CALToolTip* tooltip, QWidget* parent) {
	const QPoint pos = parent->mapToGlobal(QPoint());
	const int x = pos.x() - tooltip->width();
	const int y = pos.y() + (parent->height() - tooltip->height()) / 2;

	return { x, y };
}

/**
 * \class CALRightToolTipManager
 * @brief Manages the positioning of tooltips to the right of the parent widget.
 *
 * 该类计算 tooltip 的位置，使其显示在父窗口的右侧，并垂直居中
 *
 * @param tooltip pointer to CALToolTip
 * @param parent pointer to the need calculate widget
 * @return new pos
 */
QPoint CALRightToolTipManager::calculatePosition(CALToolTip* tooltip, QWidget* parent) {
	const QPoint pos = parent->mapToGlobal(QPoint());
	const int x = pos.x() + parent->width();
	const int y = pos.y() + (parent->height() - tooltip->height()) / 2;

	return { x, y };
}

/**
 * \class CALTopRightToolTipManager
 * @brief Manages the positioning of tooltips above and to the right of the parent widget.
 *
 * 该类计算 tooltip 的位置，使其显示在父窗口的右上角，并对齐到右侧，同时考虑布局边距
 *
 * @param tooltip pointer to CALToolTip
 * @param parent pointer to the need calculate widget
 * @return new pos
 */
QPoint CALTopRightToolTipManager::calculatePosition(CALToolTip* tooltip, QWidget* parent) {
	const QPoint pos = parent->mapToGlobal(QPoint());
	const int x = pos.x() + parent->width() - tooltip->width() + tooltip->layout()->contentsMargins().right();
	const int y = pos.y() - tooltip->height();

	return { x, y };
}

/**
 * \class CALTopLeftToolTipManager
 * @brief Manages the positioning of tooltips above and to the left of the parent widget.
 *
 * 该类计算 tooltip 的位置，使其显示在父窗口的左上角，并对齐到左侧，同时考虑布局边距
 *
 * @param tooltip pointer to CALToolTip
 * @param parent pointer to the need calculate widget
 * @return new pos
 */
QPoint CALTopLeftToolTipManager::calculatePosition(CALToolTip* tooltip, QWidget* parent) {
	const QPoint pos = parent->mapToGlobal(QPoint());
	const int x = pos.x() - tooltip->layout()->contentsMargins().left();
	const int y = pos.y() - tooltip->height();

	return { x, y };
}

/**
 * \class CALBottomRightToolTipManager
 * @brief Manages the positioning of tooltips below and to the right of the parent widget.
 *
 * 该类计算 tooltip 的位置，使其显示在父窗口的右下角，并对齐到右侧，同时考虑布局边距
 *
 * @param tooltip pointer to CALToolTip
 * @param parent pointer to the need calculate widget
 * @return new pos
 */
QPoint CALBottomRightToolTipManager::calculatePosition(CALToolTip* tooltip, QWidget* parent) {
	const QPoint pos = parent->mapToGlobal(QPoint());
	const int x = pos.x() + parent->width() - tooltip->width() + tooltip->layout()->contentsMargins().right();
	const int y = pos.y() + parent->height();

	return { x, y };
}

/**
 * \class CALBottomLeftToolTipManager
 * @brief Manages the positioning of tooltips below and to the left of the parent widget.
 *
 * 该类计算 tooltip 的位置，使其显示在父窗口的左下角，并对齐到左侧，同时考虑布局边距
 *
 * @param tooltip pointer to CALToolTip
 * @param parent pointer to the need calculate widget
 * @return new pos
 */
QPoint CALBottomLeftToolTipManager::calculatePosition(CALToolTip* tooltip, QWidget* parent) {
	const QPoint pos = parent->mapToGlobal(QPoint());
	const int x = pos.x() - tooltip->layout()->contentsMargins().left();
	const int y = pos.y() + parent->height();

	return { x, y };
}

/**
 * \class CALToolTipFilterPrivate
 * Internal class for CALToolTipFilter
 * @param q pointer to the public class
 * @param parent pointer to the parent class
 */
CALToolTipFilterPrivate::CALToolTipFilterPrivate(CALToolTipFilter* q, QObject* parent): QObject(parent), q_ptr(q) {
	isEnter = false;
	timer = new QTimer(q);
	timer->setSingleShot(true);
	connect(timer, &QTimer::timeout, q, &CALToolTipFilter::slotShowToolTip);
}

CALToolTipFilterPrivate::~CALToolTipFilterPrivate() = default;

/**
 * \class CALToolTipFilter
 * @param parent pointer to the parent widget, not null
 * @param showDelay tooltip show delay time
 * @param position tooltip position \see \enum ALToolTipType::Position
 */
CALToolTipFilter::CALToolTipFilter(QWidget* parent, const int showDelay, const ALToolTipType::Position& position): QObject(parent), d_ptr(new CALToolTipFilterPrivate(this, this)) {
	Q_D(CALToolTipFilter);

	d->toolTipDelay = showDelay;
	d->position = position;
}

CALToolTipFilter::~CALToolTipFilter() = default;

void CALToolTipFilter::hideToolTip() {
	Q_D(CALToolTipFilter);

	d->isEnter = false;
	d->timer->stop();

	if (d->toolTip) {
		d->toolTip->hide();
	}
}

void CALToolTipFilter::setToolTipDelay(const int delay) {
	d_func()->toolTipDelay = delay;
}

bool CALToolTipFilter::eventFilter(QObject* watched, QEvent* event) {
	Q_D(CALToolTipFilter);

	switch (event->type()) {
		case QEvent::ToolTip: {
			return true;
		}
		case QEvent::Hide:
		case QEvent::Leave: {
			hideToolTip();
			break;
		}
		case QEvent::Enter: {
			d->isEnter = true;
			const auto parent = qobject_cast<QWidget*>(this->parent());
			if (canShowToolTip()) {
				if (!d->toolTip) {
					d->toolTip = createToolTip();
				}
				d->toolTip->setDuration(parent->toolTipDuration() > 0 ? parent->toolTipDuration() : -1);
				d->timer->start(d->toolTipDelay);
			}
			break;
		}
		case QEvent::MouseButtonPress: {
			hideToolTip();
			break;
		}
		default: {
			break;
		}
	}

	return QObject::eventFilter(watched, event);
}

CALToolTip* CALToolTipFilter::createToolTip() {
	const auto parent = qobject_cast<QWidget*>(this->parent());
	return new CALToolTip(parent->window(), false);
}

bool CALToolTipFilter::canShowToolTip() const {
	const auto parent = qobject_cast<QWidget*>(this->parent());
	return parent->isWidgetType() && !parent->toolTip().isEmpty() && parent->isEnabled();
}

void CALToolTipFilter::slotShowToolTip() {
	Q_D(CALToolTipFilter);

	if (!d->isEnter) {
		return;
	}

	const auto parent = qobject_cast<QWidget*>(this->parent());
	d->toolTip->setText(parent->toolTip());
	d->toolTip->adjustPos(parent, d->position);
	d->toolTip->show();
}
}
