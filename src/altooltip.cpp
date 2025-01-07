#include "altooltip.hpp"

#include <QApplication>
#include <QDebug>
#include <QDesktopWidget>
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
		emit sigOpacityChanged();
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
			QTimer::singleShot(showDelayMsec, this, [=]() {
				doShowAnimation();
			});
			if (displayMsec > -1) {
				QTimer::singleShot(displayMsec, this, [=]() {
					q->hide();
				});
			}
			break;
		}
		case QEvent::Leave: {
			QTimer::singleShot(hideDelayMsec, this, [=]() {
				q->hide();
			});
			break;
		}
		case QEvent::Hide: {
			q->hide();
			break;
		}
		case QEvent::HoverMove:
		case QEvent::MouseMove: {
			if (q->isVisible()) {
				const QPoint cursorPoint = QCursor::pos();
				const QScreen* screen = QGuiApplication::screenAt(cursorPoint);
				if (!screen) {
					qWarning() << "Failed to get the screen!";
					break;
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
		default: break;
	}
	return QObject::eventFilter(watched, event);
}

void CALToolTipPrivate::doShowAnimation() {
	Q_Q(CALToolTip);

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

/**
 * @brief \class CALToolTip
 * @param parent pointer to the parent class
 */
CALToolTip::CALToolTip(QWidget* parent): QWidget(parent), d_ptr(new CALToolTipPrivate(this, this)) {
	Q_D(CALToolTip);

	d->borderRadius = 5;
	d->displayMsec = -1;
	d->showDelayMsec = 0;
	d->hideDelayMsec = 0;
	d->customWidget = nullptr;
	setObjectName("CALToolTip");
	if (parent) {
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
	connect(ALTheme, &CALThemeManager::sigThemeModeChanged, this, [=](const ALThemeType::ThemeMode& mode) {
		d->themeMode = mode;
		update();
	});

	hide();
}

CALToolTip::~CALToolTip() = default;

void CALToolTip::setBorderRadius(const int borderRadius) {
	Q_D(CALToolTip);

	if (d->borderRadius != borderRadius) {
		d->borderRadius = borderRadius;
		emit sigBorderRadiusChanged();
	}
}

int CALToolTip::getBorderRadius() const {
	Q_D(const CALToolTip);

	return d->borderRadius;
}

void CALToolTip::setDisplayMsec(const int displayMsec) {
	Q_D(CALToolTip);

	if (d->displayMsec != displayMsec) {
		d->displayMsec = displayMsec;
		emit sigDisplayMsecChanged();
	}
}

int CALToolTip::getDisplayMsec() const {
	Q_D(const CALToolTip);

	return d->displayMsec;
}

void CALToolTip::setShowDelayMsec(const int delayMsec) {
	Q_D(CALToolTip);

	if (d->showDelayMsec != delayMsec) {
		d->showDelayMsec = delayMsec;
		emit sigShowDelayMsecChanged();
	}
}

int CALToolTip::getShowDelayMsec() const {
	Q_D(const CALToolTip);

	return d->showDelayMsec;
}

void CALToolTip::setHideDelayMsec(const int delayMsec) {
	Q_D(CALToolTip);

	if (d->hideDelayMsec != delayMsec) {
		d->hideDelayMsec = delayMsec;
		emit sigHideDelayMsecChanged();
	}
}

int CALToolTip::getHideDelayMsec() const {
	Q_D(const CALToolTip);

	return d->hideDelayMsec;
}

void CALToolTip::setToolTip(const QString& tooltip) {
	Q_D(CALToolTip);

	d->toolTipText->setText(tooltip);
	emit sigToolTipChanged();
}

QString CALToolTip::getToolTip() const {
	Q_D(const CALToolTip);

	return d->toolTipText->text();
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
	emit sigCustomWidgetChanged();
}

QWidget* CALToolTip::getCustomWidget() const {
	Q_D(const CALToolTip);

	return d->customWidget;
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

/**
 * @brief \class CALToolTipHelper
 * @return CALToolTip instance
 */
CALToolTip* CALToolTipHelper::instance() {
	return CALSingleton<CALToolTip>::instance();
}

void CALToolTipHelper::showText(const QPoint& pos, const QString& text, QWidget* w) {
	CALToolTip* tooltip = instance();
	if (text.isEmpty() || pos.isNull()) {
		tooltip->hide();
		return;
	}

	tooltip->setToolTip(text);
	tooltip->move(pos);
	if (w) {
		w->installEventFilter(tooltip->d_func());
	} else {
		tooltip->show();
	}
}

void CALToolTipHelper::showText(const QPoint& pos, const QString& text, QWidget* w, const QRect& rect) {
	showText(pos, text, w);
}

void CALToolTipHelper::showText(const QPoint& pos, const QString& text, QWidget* w, const QRect& rect, const int msecShowTime) {
	CALToolTip* tooltip = instance();
	tooltip->setDisplayMsec(msecShowTime);
	showText(pos, text, w, rect);
}

bool CALToolTipHelper::isVisible() {
	return instance()->isVisible();
}

QString CALToolTipHelper::text() {
	return instance()->getToolTip();
}
