#include "uvtooltip.hpp"

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

#include "uvtext.hpp"
#include "uvthememanager.hpp"
#include "uvtooltip_p.hpp"

/**
 * @brief \class CUVToolTipPrivate
 * Internal class for CUVTooltip
 * @param q pointer to the public class
 * @param parent pointer to the parent class
 */
CUVToolTipPrivate::CUVToolTipPrivate(CUVToolTip* q, QObject* parent): QObject(parent), q_ptr(q) {
	opacity = -1;
}

CUVToolTipPrivate::~CUVToolTipPrivate() = default;

void CUVToolTipPrivate::setOpacity(const qreal opacity) {
	if (this->opacity != opacity) {
		this->opacity = opacity;
		emit sigOpacityChanged();
	}
}

qreal CUVToolTipPrivate::getOpacity() const {
	return opacity;
}

bool CUVToolTipPrivate::eventFilter(QObject* watched, QEvent* event) {
	Q_Q(CUVToolTip);

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

void CUVToolTipPrivate::doShowAnimation() {
	Q_Q(CUVToolTip);

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
 * @brief \class CUVToolTip
 * @param parent pointer to the parent class
 */
CUVToolTip::CUVToolTip(QWidget* parent): QWidget(parent), d_ptr(new CUVToolTipPrivate(this, this)) {
	Q_D(CUVToolTip);

	d->borderRadius = 5;
	d->displayMsec = -1;
	d->showDelayMsec = 0;
	d->hideDelayMsec = 0;
	d->customWidget = nullptr;
	setObjectName("CUVToolTip");
	if (parent) {
		parent->installEventFilter(d);
	}

	setAttribute(Qt::WA_TransparentForMouseEvents);
	setAttribute(Qt::WA_TranslucentBackground);
	setWindowFlags(Qt::Tool | Qt::FramelessWindowHint);

	d->toolTipText = new CUVText(this);
	d->toolTipText->setWordWrap(false);
	d->toolTipText->setTextPixelSize(17);
	d->mainVLayout = new QVBoxLayout(this);
	d->mainVLayout->setContentsMargins(d->shadowBorderWidth * 2, d->shadowBorderWidth * 2, d->shadowBorderWidth * 2, d->shadowBorderWidth * 2);
	d->mainVLayout->addWidget(d->toolTipText);

	d->themeMode = UVTheme->getThemeMode();
	connect(UVTheme, &CUVThemeManager::sigThemeModeChanged, this, [=](const UVThemeType::ThemeMode& mode) {
		d->themeMode = mode;
		update();
	});

	hide();
}

CUVToolTip::~CUVToolTip() = default;

void CUVToolTip::setBorderRadius(const int borderRadius) {
	Q_D(CUVToolTip);

	if (d->borderRadius != borderRadius) {
		d->borderRadius = borderRadius;
		emit sigBorderRadiusChanged();
	}
}

int CUVToolTip::getBorderRadius() const {
	Q_D(const CUVToolTip);

	return d->borderRadius;
}

void CUVToolTip::setDisplayMsec(const int displayMsec) {
	Q_D(CUVToolTip);

	if (d->displayMsec != displayMsec) {
		d->displayMsec = displayMsec;
		emit sigDisplayMsecChanged();
	}
}

int CUVToolTip::getDisplayMsec() const {
	Q_D(const CUVToolTip);

	return d->displayMsec;
}

void CUVToolTip::setShowDelayMsec(const int delayMsec) {
	Q_D(CUVToolTip);

	if (d->showDelayMsec != delayMsec) {
		d->showDelayMsec = delayMsec;
		emit sigShowDelayMsecChanged();
	}
}

int CUVToolTip::getShowDelayMsec() const {
	Q_D(const CUVToolTip);

	return d->showDelayMsec;
}

void CUVToolTip::setHideDelayMsec(const int delayMsec) {
	Q_D(CUVToolTip);

	if (d->hideDelayMsec != delayMsec) {
		d->hideDelayMsec = delayMsec;
		emit sigHideDelayMsecChanged();
	}
}

int CUVToolTip::getHideDelayMsec() const {
	Q_D(const CUVToolTip);

	return d->hideDelayMsec;
}

void CUVToolTip::setToolTip(const QString& tooltip) {
	Q_D(CUVToolTip);

	d->toolTipText->setText(tooltip);
	emit sigToolTipChanged();
}

QString CUVToolTip::getToolTip() const {
	Q_D(const CUVToolTip);

	return d->toolTipText->text();
}

void CUVToolTip::setCustomWidget(QWidget* customWidget) {
	Q_D(CUVToolTip);

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

QWidget* CUVToolTip::getCustomWidget() const {
	Q_D(const CUVToolTip);

	return d->customWidget;
}

void CUVToolTip::paintEvent(QPaintEvent* event) {
	Q_D(CUVToolTip);

	QPainter painter(this);
	painter.save();
	painter.setRenderHint(QPainter::Antialiasing);
	// 绘制阴影
	UVTheme->drawEffectShadow(&painter, rect(), d->shadowBorderWidth, d->borderRadius);
	QRect forefroundRect = rect();
	forefroundRect.adjust(d->shadowBorderWidth, d->shadowBorderWidth, -d->shadowBorderWidth, -d->shadowBorderWidth);
	painter.setPen(UVThemeColor(d->themeMode, UVThemeType::PopupBorder));
	painter.setBrush(UVThemeColor(d->themeMode, UVThemeType::PopupBase));
	painter.drawRoundedRect(forefroundRect, d->borderRadius, d->borderRadius);
	painter.restore();
}

/**
 * @brief \class CUVToolTipHelper
 * @return CUVToolTip instance
 */
CUVToolTip* CUVToolTipHelper::instance() {
	return CUVSingleton<CUVToolTip>::instance();
}

void CUVToolTipHelper::showText(const QPoint& pos, const QString& text, QWidget* w) {
	CUVToolTip* tooltip = instance();
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

void CUVToolTipHelper::showText(const QPoint& pos, const QString& text, QWidget* w, const QRect& rect) {
	showText(pos, text, w);
}

void CUVToolTipHelper::showText(const QPoint& pos, const QString& text, QWidget* w, const QRect& rect, const int msecShowTime) {
	CUVToolTip* tooltip = instance();
	tooltip->setDisplayMsec(msecShowTime);
	showText(pos, text, w, rect);
}

bool CUVToolTipHelper::isVisible() {
	return instance()->isVisible();
}

QString CUVToolTipHelper::text() {
	return instance()->getToolTip();
}
