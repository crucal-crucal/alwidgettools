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

	if (q->isVisible() && _pIsAnimation && max != 0) {
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
		rangeSmoothAnimation->setStartValue(_pTargetMaximum);
		rangeSmoothAnimation->setEndValue(max);
		rangeSmoothAnimation->start(QAbstractAnimation::DeleteWhenStopped);
	} else {
		if (0 == max) {
			_scrollValue = -1;
		}
		_pTargetMaximum = max;
	}
}

void CUVScrollBarPrivate::setTargetMaximum(const int targetMaximum) {
	_pTargetMaximum = targetMaximum;
	emit sigTargetMaximumChanged();
}

int CUVScrollBarPrivate::getTargetMaximum() const {
	return _pTargetMaximum;
}

void CUVScrollBarPrivate::_scroll(const Qt::KeyboardModifiers& modifiers, const int value) {
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
	if (std::abs(_scrollValue - q->value()) > std::abs(stepsToScroll * _pSpeedLimit)) {
		_scrollValue = q->value();
	}
	_scrollValue -= stepsToScroll;
	_slideSmoothAnimation->stop();
	_slideSmoothAnimation->setStartValue(q->value());
	_slideSmoothAnimation->setEndValue(_scrollValue);
	_slideSmoothAnimation->start();
}

int CUVScrollBarPrivate::_pixelPosToRangeValue(const int pos) const {
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

void CUVScrollBarPrivate::_initAllConfig() {
	Q_Q(CUVScrollBar);

	_handleScrollBarRangeChanged(_originScrollBar->minimum(), _originScrollBar->maximum());
	q->setSingleStep(_originScrollBar->singleStep());
	q->setPageStep(_originScrollBar->pageStep());
}

void CUVScrollBarPrivate::_handleScrollBarValueChanged(QScrollBar* scrollBar, const int value) {
	scrollBar->setValue(value);
}

void CUVScrollBarPrivate::_handleScrollBarRangeChanged(const int min, const int max) {
	Q_Q(CUVScrollBar);

	q->setRange(min, max);
	q->setVisible(max > 0);
}

void CUVScrollBarPrivate::_handleScrollBarGeometry() {
	Q_Q(CUVScrollBar);

	q->raise();
	q->setSingleStep(_originScrollBar->singleStep());
	q->setPageStep(_originScrollBar->pageStep());
	if (q->orientation() == Qt::Horizontal) {
		q->setGeometry(0, _originScrollArea->height() - 10, _originScrollArea->width(), 10);
	} else {
		q->setGeometry(_originScrollArea->width() - 10, 0, 10, _originScrollArea->height());
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
	d->_pSpeedLimit = 20;
	d->_pTargetMaximum = 0;
	d->_pIsAnimation = false;
	connect(this, &CUVScrollBar::rangeChanged, d, &CUVScrollBarPrivate::slotRangeChanged);
	const auto scrollBarStyle = new CUVScrollBarStyle(style());
	scrollBarStyle->setScrollBar(this);
	setStyle(scrollBarStyle);
	d->_slideSmoothAnimation = new QPropertyAnimation(this, "value");
	d->_slideSmoothAnimation->setEasingCurve(QEasingCurve::OutSine);
	d->_slideSmoothAnimation->setDuration(300);
	connect(d->_slideSmoothAnimation, &QPropertyAnimation::finished, this, [=]() { d->_scrollValue = value(); });

	d->_expandTimer = new QTimer(this);
	connect(d->_expandTimer, &QTimer::timeout, this, [=]() {
		d->_expandTimer->stop();
		d->_isExpand = underMouse();
		scrollBarStyle->startExpandAnimation(d->_isExpand);
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

	d->_originScrollArea = parent;
	const Qt::Orientation orientation = originScrollBar->orientation();
	setOrientation(orientation);
	orientation == Qt::Horizontal ? parent->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff) : parent->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	parent->installEventFilter(this);

	d->_originScrollBar = originScrollBar;
	d->_initAllConfig();

	connect(d->_originScrollBar, &QScrollBar::valueChanged, this, [=](const int value) {
		CUVScrollBarPrivate::_handleScrollBarValueChanged(this, value);
	});
	connect(this, &QScrollBar::valueChanged, this, [=](const int value) {
		CUVScrollBarPrivate::_handleScrollBarValueChanged(d->_originScrollBar, value);
	});
	connect(d->_originScrollBar, &QScrollBar::rangeChanged, this, [=](const int min, const int max) {
		d->_handleScrollBarRangeChanged(min, max);
	});
}

CUVScrollBar::~CUVScrollBar() = default;

void CUVScrollBar::setIsAnimation(const bool isAnimation) {
	Q_D(CUVScrollBar);

	d->_pIsAnimation = isAnimation;
	emit sigIsAnimationChanged();
}

bool CUVScrollBar::getIsAnimation() const {
	return d_func()->_pIsAnimation;
}

void CUVScrollBar::setSpeedLimit(const qreal speedLimit) {
	Q_D(CUVScrollBar);

	d->_pSpeedLimit = speedLimit;
	emit sigSpeedLimitChanged();
}

qreal CUVScrollBar::getSpeedLimit() const {
	return d_func()->_pSpeedLimit;
}

void CUVScrollBar::setContextMenuFlags(const ContextMenuFlags& flags) {
	d_func()->_pContextMenuFlags = flags;
}

CUVScrollBar::ContextMenuFlags CUVScrollBar::getContextMenuFlags() const {
	return d_func()->_pContextMenuFlags;
}

bool CUVScrollBar::hasFlag(const ContextMenuFlag& flag) const {
	return d_func()->_pContextMenuFlags.testFlag(flag);
}

void CUVScrollBar::mousePressEvent(QMouseEvent* event) {
	Q_D(CUVScrollBar);

	d->_slideSmoothAnimation->stop();
	QScrollBar::mousePressEvent(event);
	d->_scrollValue = value();
}

void CUVScrollBar::mouseReleaseEvent(QMouseEvent* event) {
	Q_D(CUVScrollBar);

	d->_slideSmoothAnimation->stop();
	QScrollBar::mouseReleaseEvent(event);
	d->_scrollValue = value();
}

void CUVScrollBar::mouseMoveEvent(QMouseEvent* event) {
	Q_D(CUVScrollBar);

	d->_slideSmoothAnimation->stop();
	QScrollBar::mouseMoveEvent(event);
	d->_scrollValue = value();
}

void CUVScrollBar::wheelEvent(QWheelEvent* event) {
	Q_D(CUVScrollBar);

	if (d->_slideSmoothAnimation->state() == QAbstractAnimation::Stopped) {
		d->_scrollValue = value();
	}

	if (const int delta = event->angleDelta().y() != 0 ? event->angleDelta().y() : event->angleDelta().x();
		(value() == minimum() && delta > 0) || (value() == maximum() && delta < 0)) {
		QScrollBar::wheelEvent(event); // 边界情况调用基类处理
	} else {
		d->_scroll(event->modifiers(), delta);
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
		setValue(d->_pixelPosToRangeValue(horiz ? event->pos().x() : event->pos().y()));
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
			d->_expandTimer->stop();
			if (!d->_isExpand) {
				d->_expandTimer->start(350);
			}
			break;
		}
		case QEvent::Leave: {
			d->_expandTimer->stop();
			if (d->_isExpand) {
				d->_expandTimer->start(350);
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
			d->_handleScrollBarGeometry();
			break;
		}
		default: break;
	}
	return QScrollBar::eventFilter(watched, event);
}
