#include "alscrollbar.hpp"

#include <QApplication>
#include <QDebug>
#include <QPainter>
#include <QPointer>
#include <QPropertyAnimation>
#include <QStyleOptionSlider>
#include <QTimer>
#include <QWheelEvent>

#include "almenu.hpp"
#include "alscrollbarstyle.hpp"
#include "alscrollbar_p.hpp"

using namespace ALIcon;

/**
 * @brief \class CALScrollBarPrivate
 * Internal class for CALScrollBar
 * @param q pointer to the public class
 * @param parent pointer to the parent class
 */
CALScrollBarPrivate::CALScrollBarPrivate(CALScrollBar* q, QObject* parent): QObject(parent), q_ptr(q) {
}

CALScrollBarPrivate::~CALScrollBarPrivate() = default;

void CALScrollBarPrivate::slotRangeChanged(const int min, const int max) {
	Q_Q(CALScrollBar);

	if (q->isVisible() && isAnimation && max != 0) {
		const auto rangeSmoothAnimation = new QPropertyAnimation(this, "targetMaximum");
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

void CALScrollBarPrivate::setTargetMaximum(const int targetMaximum) {
	this->targetMaximum = targetMaximum;
	emit sigTargetMaximumChanged();
}

int CALScrollBarPrivate::getTargetMaximum() const {
	return targetMaximum;
}

void CALScrollBarPrivate::scroll(const Qt::KeyboardModifiers& modifiers, const int value) {
	Q_Q(CALScrollBar);

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

int CALScrollBarPrivate::pixelPosToRangeValue(const int pos) const {
	Q_Q(const CALScrollBar);

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

void CALScrollBarPrivate::initAllConfig() {
	Q_Q(CALScrollBar);

	handleScrollBarRangeChanged(originScrollBar->minimum(), originScrollBar->maximum());
	q->setSingleStep(originScrollBar->singleStep());
	q->setPageStep(originScrollBar->pageStep());
}

void CALScrollBarPrivate::handleScrollBarValueChanged(QScrollBar* scrollBar, const int value) {
	scrollBar->setValue(value);
}

void CALScrollBarPrivate::handleScrollBarRangeChanged(const int min, const int max) {
	Q_Q(CALScrollBar);

	q->setRange(min, max);
	q->setVisible(max > 0);
}

void CALScrollBarPrivate::handleScrollBarGeometry() {
	Q_Q(CALScrollBar);

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
 * @brief \class CALScrollBar
 * @param parent pointer to the parent class
 */
CALScrollBar::CALScrollBar(QWidget* parent): QScrollBar(parent), d_ptr(new CALScrollBarPrivate(this, this)) {
	Q_D(CALScrollBar);

	setSingleStep(1);
	setObjectName("CALScrollBar");
	setAttribute(Qt::WA_OpaquePaintEvent, false);
	d->speedLimit = 20;
	d->targetMaximum = 0;
	d->isAnimation = false;
	connect(this, &CALScrollBar::rangeChanged, d, &CALScrollBarPrivate::slotRangeChanged);
	const auto scrollBarStyle = new CALScrollBarStyle(style());
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

CALScrollBar::CALScrollBar(const Qt::Orientation orientation, QWidget* parent): CALScrollBar(parent) {
	setOrientation(orientation);
}

CALScrollBar::CALScrollBar(QScrollBar* originScrollBar, QAbstractScrollArea* parent): CALScrollBar(parent) {
	Q_D(CALScrollBar);

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
		CALScrollBarPrivate::handleScrollBarValueChanged(this, value);
	});
	connect(this, &QScrollBar::valueChanged, this, [=](const int value) {
		CALScrollBarPrivate::handleScrollBarValueChanged(d->originScrollBar, value);
	});
	connect(d->originScrollBar, &QScrollBar::rangeChanged, this, [=](const int min, const int max) {
		d->handleScrollBarRangeChanged(min, max);
	});
}

CALScrollBar::~CALScrollBar() = default;

void CALScrollBar::setIsAnimation(const bool isAnimation) {
	Q_D(CALScrollBar);

	d->isAnimation = isAnimation;
	emit sigIsAnimationChanged();
}

bool CALScrollBar::getIsAnimation() const {
	return d_func()->isAnimation;
}

void CALScrollBar::setSpeedLimit(const qreal speedLimit) {
	Q_D(CALScrollBar);

	d->speedLimit = speedLimit;
	emit sigSpeedLimitChanged();
}

qreal CALScrollBar::getSpeedLimit() const {
	return d_func()->speedLimit;
}

void CALScrollBar::setContextMenuFlags(const ContextMenuFlags& flags) {
	d_func()->contextMenuFlags = flags;
}

CALScrollBar::ContextMenuFlags CALScrollBar::getContextMenuFlags() const {
	return d_func()->contextMenuFlags;
}

bool CALScrollBar::hasFlag(const ContextMenuFlag& flag) const {
	return d_func()->contextMenuFlags.testFlag(flag);
}

void CALScrollBar::mousePressEvent(QMouseEvent* event) {
	Q_D(CALScrollBar);

	d->slideSmoothAnimation->stop();
	QScrollBar::mousePressEvent(event);
	d->scrollValue = value();
}

void CALScrollBar::mouseReleaseEvent(QMouseEvent* event) {
	Q_D(CALScrollBar);

	d->slideSmoothAnimation->stop();
	QScrollBar::mouseReleaseEvent(event);
	d->scrollValue = value();
}

void CALScrollBar::mouseMoveEvent(QMouseEvent* event) {
	Q_D(CALScrollBar);

	d->slideSmoothAnimation->stop();
	QScrollBar::mouseMoveEvent(event);
	d->scrollValue = value();
}

void CALScrollBar::wheelEvent(QWheelEvent* event) {
	Q_D(CALScrollBar);

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

void CALScrollBar::contextMenuEvent(QContextMenuEvent* event) {
	Q_D(CALScrollBar);

	const bool horiz = this->orientation() == Qt::Horizontal;
	QPointer<CALMenu> menu = new CALMenu(this);
	menu->setMenuItemHeight(27);
	const QAction* actScrollHere = menu->addAction(hasFlag(CALScrollBar::HasIcon) ? AweSomeIcon::UpDownLeftRight : AweSomeIcon::None, tr("Scroll here"));
	menu->addSeparator();
	const QAction* actScrollTop = menu->addAction(hasFlag(CALScrollBar::HasIcon) ? horiz ? AweSomeIcon::ArrowLeftToLine : AweSomeIcon::ArrowUpToLine : AweSomeIcon::None, horiz ? tr("Left edge") : tr("Top"));
	const QAction* actScrollBottom = menu->addAction(hasFlag(CALScrollBar::HasIcon) ? horiz ? AweSomeIcon::ArrowRightToLine : AweSomeIcon::ArrowDownToLine : AweSomeIcon::None, horiz ? tr("Right edge") : tr("Bottom"));
	menu->addSeparator();
	const QAction* actPageUp = menu->addAction(hasFlag(CALScrollBar::HasIcon) ? horiz ? AweSomeIcon::AnglesLeft : AweSomeIcon::AnglesUp : AweSomeIcon::None, horiz ? tr("Page left") : tr("Page up"));
	const QAction* actPageDn = menu->addAction(hasFlag(CALScrollBar::HasIcon) ? horiz ? AweSomeIcon::AnglesRight : AweSomeIcon::AnglesDown : AweSomeIcon::None, horiz ? tr("Page right") : tr("Page down"));
	menu->addSeparator();
	const QAction* actScrollUp = menu->addAction(hasFlag(CALScrollBar::HasIcon) ? horiz ? AweSomeIcon::AngleLeft : AweSomeIcon::AngleUp : AweSomeIcon::None, horiz ? tr("Scroll left") : tr("Scroll up"));
	const QAction* actScrollDn = menu->addAction(hasFlag(CALScrollBar::HasIcon) ? horiz ? AweSomeIcon::AngleRight : AweSomeIcon::AngleDown : AweSomeIcon::None, horiz ? tr("Scroll right") : tr("Scroll down"));
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

bool CALScrollBar::event(QEvent* event) {
	Q_D(CALScrollBar);

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

bool CALScrollBar::eventFilter(QObject* watched, QEvent* event) {
	Q_D(CALScrollBar);

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
