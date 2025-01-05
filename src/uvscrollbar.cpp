#include "uvscrollbar.hpp"

#include <QApplication>
#include <QDebug>
#include <QPainter>
#include <QPointer>
#include <QPropertyAnimation>
#include <QStyleOptionSlider>
#include <QTimer>
#include <QWheelEvent>

#include "uvmenu.hpp"
#include "uvscrollbarstyle.hpp"
#include "uvscrollbar_p.hpp"

using namespace UVIcon;

/**
 * @brief \class CUVScrollBarPrivate
 * Internal class for CUVScrollBar
 * @param q pointer to the public class
 * @param parent pointer to the parent class
 */
CUVScrollBarPrivate::CUVScrollBarPrivate(CUVScrollBar* q, QObject* parent): QObject(parent), q_ptr(q) {
}

CUVScrollBarPrivate::~CUVScrollBarPrivate() = default;

void CUVScrollBarPrivate::slotRangeChanged(const int min, const int max) {
	Q_Q(CUVScrollBar);

	if (q->isVisible() && isAnimation && max != 0) {
		const auto rangeSmoothAnimation = new QPropertyAnimation(this, "pTargetMaximum");
		connect(rangeSmoothAnimation, &QPropertyAnimation::valueChanged, this, [=](const QVariant& value) {
			q->blockSignals(true);
			q->setMinimum(value.toInt());
			q->blockSignals(false);
			q->update();
		});
		connect(rangeSmoothAnimation, &QPropertyAnimation::finished, this, [=]() {
			Q_EMIT q->sigRangeAnimationFinished();
		});
		rangeSmoothAnimation->setEasingCurve(QEasingCurve::OutSine);
		rangeSmoothAnimation->setDuration(200);
		rangeSmoothAnimation->setStartValue(targetMaximum);
		rangeSmoothAnimation->setEndValue(max);
		rangeSmoothAnimation->start(QAbstractAnimation::DeleteWhenStopped);
	} else {
		if (0 == max) {
			scrollValue = -1;
		}
		targetMaximum = max;
	}
}

void CUVScrollBarPrivate::setTargetMaximum(const int targetMaximum) {
	this->targetMaximum = targetMaximum;
	emit sigTargetMaximumChanged();
}

int CUVScrollBarPrivate::getTargetMaximum() const {
	return targetMaximum;
}

void CUVScrollBarPrivate::scroll(const Qt::KeyboardModifiers& modifiers, const int value) {
	Q_Q(CUVScrollBar);

	int stepsToScroll;
	const qreal offset = static_cast<qreal>(value) / 120;
	const int singleStep = q->singleStep();
	if ((modifiers & Qt::ControlModifier) || (modifiers & Qt::ShiftModifier)) {
		constexpr int pageStep = 10;
		stepsToScroll = qBound(-pageStep, static_cast<int>(offset * pageStep), pageStep);
	} else {
		stepsToScroll = QApplication::wheelScrollLines() * static_cast<int>(offset) * singleStep;
	}
	if (std::abs(scrollValue - q->value()) > std::abs(stepsToScroll * speedLimit)) {
		scrollValue = q->value();
	}
	scrollValue -= stepsToScroll;
	slideSmoothAnimation->stop();
	slideSmoothAnimation->setStartValue(q->value());
	slideSmoothAnimation->setEndValue(scrollValue);
	slideSmoothAnimation->start();
}

int CUVScrollBarPrivate::pixelPosToRangeValue(const int pos) const {
	Q_Q(const CUVScrollBar);

	QStyleOptionSlider opt;
	q->initStyleOption(&opt);
	const QRect gr = q->style()->subControlRect(QStyle::CC_ScrollBar, &opt, QStyle::SC_ScrollBarGroove, q);
	const QRect sr = q->style()->subControlRect(QStyle::CC_ScrollBar, &opt, QStyle::SC_ScrollBarSlider, q);
	int sliderMin, sliderMax, sliderLength;
	if (q->orientation() == Qt::Horizontal) {
		sliderLength = sr.width();
		sliderMin = gr.x();
		sliderMax = gr.right() - sliderLength + 1;
		if (q->layoutDirection() == Qt::RightToLeft) {
			opt.upsideDown = !opt.upsideDown;
		}
	} else {
		sliderLength = sr.height();
		sliderMin = gr.y();
		sliderMax = gr.bottom() - sliderLength + 1;
	}

	return QStyle::sliderValueFromPosition(q->minimum(), q->maximum(), pos - sliderMin, sliderMax - sliderMin, opt.upsideDown);
}

void CUVScrollBarPrivate::initAllConfig() {
	Q_Q(CUVScrollBar);

	handleScrollBarRangeChanged(originScrollBar->minimum(), originScrollBar->maximum());
	q->setSingleStep(originScrollBar->singleStep());
	q->setPageStep(originScrollBar->pageStep());
}

void CUVScrollBarPrivate::handleScrollBarValueChanged(QScrollBar* scrollBar, const int value) {
	scrollBar->setValue(value);
}

void CUVScrollBarPrivate::handleScrollBarRangeChanged(const int min, const int max) {
	Q_Q(CUVScrollBar);

	q->setRange(min, max);
	q->setVisible(max > 0);
}

void CUVScrollBarPrivate::handleScrollBarGeometry() {
	Q_Q(CUVScrollBar);

	q->raise();
	q->setSingleStep(originScrollBar->singleStep());
	q->setPageStep(originScrollBar->pageStep());
	if (q->orientation() == Qt::Horizontal) {
		q->setGeometry(0, originScrollArea->height() - 10, originScrollArea->width(), 10);
	} else {
		q->setGeometry(originScrollArea->width() - 10, 0, 10, originScrollArea->height());
	}
}

/**
 * @brief \class CUVScrollBar
 * @param parent pointer to the parent class
 */
CUVScrollBar::CUVScrollBar(QWidget* parent): QScrollBar(parent), d_ptr(new CUVScrollBarPrivate(this, this)) {
	Q_D(CUVScrollBar);

	setSingleStep(1);
	setObjectName("CUVScrollBar");
	setAttribute(Qt::WA_OpaquePaintEvent, false);
	d->speedLimit = 20;
	d->targetMaximum = 0;
	d->isAnimation = false;
	connect(this, &CUVScrollBar::rangeChanged, d, &CUVScrollBarPrivate::slotRangeChanged);
	const auto scrollBarStyle = new CUVScrollBarStyle(style());
	scrollBarStyle->setScrollBar(this);
	setStyle(scrollBarStyle);
	d->slideSmoothAnimation = new QPropertyAnimation(this, "value");
	d->slideSmoothAnimation->setEasingCurve(QEasingCurve::OutSine);
	d->slideSmoothAnimation->setDuration(300);
	connect(d->slideSmoothAnimation, &QPropertyAnimation::finished, this, [=]() { d->scrollValue = value(); });

	d->expandTimer = new QTimer(this);
	connect(d->expandTimer, &QTimer::timeout, this, [=]() {
		d->expandTimer->stop();
		d->isExpand = underMouse();
		scrollBarStyle->startExpandAnimation(d->isExpand);
	});
}

CUVScrollBar::CUVScrollBar(const Qt::Orientation orientation, QWidget* parent): CUVScrollBar(parent) {
	setOrientation(orientation);
}

CUVScrollBar::CUVScrollBar(QScrollBar* originScrollBar, QAbstractScrollArea* parent): CUVScrollBar(parent) {
	Q_D(CUVScrollBar);

	if (!originScrollBar || !parent) {
		qCritical() << "Invalid origin or parent";
		return;
	}

	d->originScrollArea = parent;
	const Qt::Orientation orientation = originScrollBar->orientation();
	setOrientation(orientation);
	orientation == Qt::Horizontal ? parent->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff) : parent->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	parent->installEventFilter(this);

	d->originScrollBar = originScrollBar;
	d->initAllConfig();

	connect(d->originScrollBar, &QScrollBar::valueChanged, this, [=](const int value) {
		CUVScrollBarPrivate::handleScrollBarValueChanged(this, value);
	});
	connect(this, &QScrollBar::valueChanged, this, [=](const int value) {
		CUVScrollBarPrivate::handleScrollBarValueChanged(d->originScrollBar, value);
	});
	connect(d->originScrollBar, &QScrollBar::rangeChanged, this, [=](const int min, const int max) {
		d->handleScrollBarRangeChanged(min, max);
	});
}

CUVScrollBar::~CUVScrollBar() = default;

void CUVScrollBar::setIsAnimation(const bool isAnimation) {
	Q_D(CUVScrollBar);

	d->isAnimation = isAnimation;
	emit sigIsAnimationChanged();
}

bool CUVScrollBar::getIsAnimation() const {
	return d_func()->isAnimation;
}

void CUVScrollBar::setSpeedLimit(const qreal speedLimit) {
	Q_D(CUVScrollBar);

	d->speedLimit = speedLimit;
	emit sigSpeedLimitChanged();
}

qreal CUVScrollBar::getSpeedLimit() const {
	return d_func()->speedLimit;
}

void CUVScrollBar::setContextMenuFlags(const ContextMenuFlags& flags) {
	d_func()->contextMenuFlags = flags;
}

CUVScrollBar::ContextMenuFlags CUVScrollBar::getContextMenuFlags() const {
	return d_func()->contextMenuFlags;
}

bool CUVScrollBar::hasFlag(const ContextMenuFlag& flag) const {
	return d_func()->contextMenuFlags.testFlag(flag);
}

void CUVScrollBar::mousePressEvent(QMouseEvent* event) {
	Q_D(CUVScrollBar);

	d->slideSmoothAnimation->stop();
	QScrollBar::mousePressEvent(event);
	d->scrollValue = value();
}

void CUVScrollBar::mouseReleaseEvent(QMouseEvent* event) {
	Q_D(CUVScrollBar);

	d->slideSmoothAnimation->stop();
	QScrollBar::mouseReleaseEvent(event);
	d->scrollValue = value();
}

void CUVScrollBar::mouseMoveEvent(QMouseEvent* event) {
	Q_D(CUVScrollBar);

	d->slideSmoothAnimation->stop();
	QScrollBar::mouseMoveEvent(event);
	d->scrollValue = value();
}

void CUVScrollBar::wheelEvent(QWheelEvent* event) {
	Q_D(CUVScrollBar);

	if (d->slideSmoothAnimation->state() == QAbstractAnimation::Stopped) {
		d->scrollValue = value();
	}

	if (const int delta = event->angleDelta().y() != 0 ? event->angleDelta().y() : event->angleDelta().x();
		(value() == minimum() && delta > 0) || (value() == maximum() && delta < 0)) {
		QScrollBar::wheelEvent(event); // 边界情况调用基类处理
	} else {
		d->scroll(event->modifiers(), delta);
	}

	event->accept();
}

void CUVScrollBar::contextMenuEvent(QContextMenuEvent* event) {
	Q_D(CUVScrollBar);

	const bool horiz = this->orientation() == Qt::Horizontal;
	QPointer<CUVMenu> menu = new CUVMenu(this);
	menu->setMenuItemHeight(27);
	const QAction* actScrollHere = menu->addAction(hasFlag(CUVScrollBar::HasIcon) ? CUVAweSomeIcon::UpDownLeftRight : CUVAweSomeIcon::None, tr("Scroll here"));
	menu->addSeparator();
	const QAction* actScrollTop = menu->addAction(hasFlag(CUVScrollBar::HasIcon) ? horiz ? CUVAweSomeIcon::ArrowLeftToLine : CUVAweSomeIcon::ArrowUpToLine : CUVAweSomeIcon::None, horiz ? tr("Left edge") : tr("Top"));
	const QAction* actScrollBottom = menu->addAction(hasFlag(CUVScrollBar::HasIcon) ? horiz ? CUVAweSomeIcon::ArrowRightToLine : CUVAweSomeIcon::ArrowDownToLine : CUVAweSomeIcon::None, horiz ? tr("Right edge") : tr("Bottom"));
	menu->addSeparator();
	const QAction* actPageUp = menu->addAction(hasFlag(CUVScrollBar::HasIcon) ? horiz ? CUVAweSomeIcon::AnglesLeft : CUVAweSomeIcon::AnglesUp : CUVAweSomeIcon::None, horiz ? tr("Page left") : tr("Page up"));
	const QAction* actPageDn = menu->addAction(hasFlag(CUVScrollBar::HasIcon) ? horiz ? CUVAweSomeIcon::AnglesRight : CUVAweSomeIcon::AnglesDown : CUVAweSomeIcon::None, horiz ? tr("Page right") : tr("Page down"));
	menu->addSeparator();
	const QAction* actScrollUp = menu->addAction(hasFlag(CUVScrollBar::HasIcon) ? horiz ? CUVAweSomeIcon::AngleLeft : CUVAweSomeIcon::AngleUp : CUVAweSomeIcon::None, horiz ? tr("Scroll left") : tr("Scroll up"));
	const QAction* actScrollDn = menu->addAction(hasFlag(CUVScrollBar::HasIcon) ? horiz ? CUVAweSomeIcon::AngleRight : CUVAweSomeIcon::AngleDown : CUVAweSomeIcon::None, horiz ? tr("Scroll right") : tr("Scroll down"));
	const QAction* actionSelected = menu->exec(event->globalPos());
	delete menu;
	if (!actionSelected) {
		return;
	}
	if (actionSelected == actScrollHere) {
		setValue(d->pixelPosToRangeValue(horiz ? event->pos().x() : event->pos().y()));
	} else if (actionSelected == actScrollTop) {
		triggerAction(QAbstractSlider::SliderToMinimum);
	} else if (actionSelected == actScrollBottom) {
		triggerAction(QAbstractSlider::SliderToMaximum);
	} else if (actionSelected == actPageUp) {
		triggerAction(QAbstractSlider::SliderPageStepSub);
	} else if (actionSelected == actPageDn) {
		triggerAction(QAbstractSlider::SliderPageStepAdd);
	} else if (actionSelected == actScrollUp) {
		triggerAction(QAbstractSlider::SliderSingleStepSub);
	} else if (actionSelected == actScrollDn) {
		triggerAction(QAbstractSlider::SliderSingleStepAdd);
	}
}

bool CUVScrollBar::event(QEvent* event) {
	Q_D(CUVScrollBar);

	switch (event->type()) {
		case QEvent::Enter: {
			d->expandTimer->stop();
			if (!d->isExpand) {
				d->expandTimer->start(350);
			}
			break;
		}
		case QEvent::Leave: {
			d->expandTimer->stop();
			if (d->isExpand) {
				d->expandTimer->start(350);
			}
			break;
		}
		default: break;
	}

	return QScrollBar::event(event);
}

bool CUVScrollBar::eventFilter(QObject* watched, QEvent* event) {
	Q_D(CUVScrollBar);

	switch (event->type()) {
		case QEvent::Show:
		case QEvent::Resize:
		case QEvent::LayoutRequest: {
			d->handleScrollBarGeometry();
			break;
		}
		default: break;
	}
	return QScrollBar::eventFilter(watched, event);
}
