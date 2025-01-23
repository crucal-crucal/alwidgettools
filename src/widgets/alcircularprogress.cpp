#include "alcircularprogress.hpp"

#include <QPainter>
#include <QPainterPath>
#include <QParallelAnimationGroup>
#include <QPropertyAnimation>
#include <QTimer>

#include "alcircularprogress_p.hpp"

/**
 * @brief \namespace AL
 */
namespace AL {
/**
 * @brief \class CALCircularProgressPrivate
 * Internal class for CALCircularProgress
 * @param q pointer to the public class
 * @param parent pointer to the parent class
 */
CALCircularProgressPrivate::CALCircularProgressPrivate(CALCircularProgress* q, QObject* parent): QObject(parent), q_ptr(q) {
}

CALCircularProgressPrivate::~CALCircularProgressPrivate() = default;

void CALCircularProgressPrivate::init() {
	Q_Q(CALCircularProgress);

	delegate = new CALCircularProgressDelegate(q);
	progressType = ALProgressType::IndeterminateProgress;
	showProgressText = true;
	color = Qt::green;
	disabledColor = Qt::gray;
	textColor = Qt::black;
	penWidth = 6.25;
	size = 64;
	startIndex1 = 0xe052;
	endIndex1 = 0xe0cb;
	currentIndex1 = startIndex1;
	startIndex2 = 0xe100;
	endIndex2 = 0xe176;
	currentIndex2 = startIndex2;
	timer = new QTimer(q);
	connect(timer, &QTimer::timeout, this, &CALCircularProgressPrivate::_updateAnimation);

	q->setSizePolicy(QSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding));

	const auto group = new QParallelAnimationGroup(q);
	group->setLoopCount(-1);

	auto animation = new QPropertyAnimation(delegate, "dashLength");
	animation->setEasingCurve(QEasingCurve::InOutQuad);
	animation->setStartValue(0.1);
	animation->setKeyValueAt(0.15, 0.2);
	animation->setKeyValueAt(0.6, 20);
	animation->setKeyValueAt(0.7, 20);
	animation->setEndValue(20);
	animation->setDuration(2050);
	group->addAnimation(animation);

	animation = new QPropertyAnimation(delegate, "dashOffset");
	animation->setEasingCurve(QEasingCurve::InOutQuad);
	animation->setStartValue(0);
	animation->setKeyValueAt(0.15, 0);
	animation->setKeyValueAt(0.6, -7);
	animation->setKeyValueAt(0.7, -7);
	animation->setEndValue(-25);
	animation->setDuration(2050);
	group->addAnimation(animation);

	animation = new QPropertyAnimation(delegate, "angle");
	animation->setStartValue(0);
	animation->setEndValue(719);
	animation->setDuration(2050);
	group->addAnimation(animation);

	group->start();
}

void CALCircularProgressPrivate::_updateAnimation() {
	Q_Q(CALCircularProgress);

	switch (progressType) {
		case ALProgressType::DiscontinuousLoading: {
			if (++currentIndex1 >= endIndex1) {
				currentIndex1 = startIndex1;
			}
			break;
		}
		case ALProgressType::ContinuousLoading: {
			if (++currentIndex2 >= endIndex2) {
				currentIndex2 = startIndex2;
			}
			break;
		}
		default: break;
	}
	q->update();
}

void CALCircularProgressPrivate::drawIndeterminate(QPainter& painter, QPen& pen) {
	Q_Q(CALCircularProgress);

	painter.save();

	QVector<qreal> pattern{};
	pattern << delegate->getDashLength() * size / 50 << 30.0 * size / 50;
	pen.setDashOffset(delegate->getDashOffset() * size / 50);
	pen.setDashPattern(pattern);
	painter.setPen(pen);
	painter.drawEllipse(QPoint(0, 0), size / 2, size / 2);

	painter.restore();
}

void CALCircularProgressPrivate::drawDeterminate(QPainter& painter, const QPen& pen) {
	Q_Q(CALCircularProgress);

	painter.save();

	painter.setPen(pen);
	const qreal x = static_cast<qreal>(q->width() - size) / 2;
	const qreal y = static_cast<qreal>(q->height() - size) / 2;
	const qreal a = static_cast<qreal>(360) * (q->value() - q->minimum()) / (q->maximum() - q->minimum());
	QPainterPath path;
	path.arcMoveTo(x, y, size, size, 0);
	path.arcTo(x, y, size, size, 0, a);
	painter.drawPath(path);

	painter.restore();
}

void CALCircularProgressPrivate::drawDiscontinuous(QPainter& painter, const QPen& pen) {
	Q_Q(CALCircularProgress);

	painter.save();

	painter.setPen(pen);
	QFont font("Segoe Boot Semilight");
	font.setPixelSize(40);
	painter.setFont(font);
	const QFontMetrics fm(font);
	const int textX = (q->width() - fm.horizontalAdvance(QChar(currentIndex1))) / 2;
	const int textY = (q->height() + fm.height()) / 2 - fm.descent() / 4;
	painter.drawText(textX, textY, QChar(currentIndex1));

	painter.restore();
}

void CALCircularProgressPrivate::drawContinuous(QPainter& painter, const QPen& pen) {
	Q_Q(CALCircularProgress);

	painter.save();

	painter.setPen(pen);
	QFont font("Segoe Boot Semilight");
	font.setPixelSize(40);
	painter.setFont(font);
	const QFontMetrics fm(font);
	const int textX = (q->width() - fm.horizontalAdvance(QChar(currentIndex2))) / 2;
	const int textY = (q->height() + fm.height()) / 2 - fm.descent() / 4;
	painter.drawText(textX, textY, QChar(currentIndex2));

	painter.restore();
}

void CALCircularProgressPrivate::drawValue(QPainter& painter, QPen& pen) {
	Q_Q(CALCircularProgress);

	painter.save();

	painter.resetTransform();
	pen.setCapStyle(Qt::FlatCap);
	pen.setColor(textColor);
	pen.setWidthF(1.0);
	painter.setPen(pen);
	const QString strCurValue = QString("%1").arg(qMax(0, q->value() * 100 / (q->maximum() - q->minimum())));
	QFont font = painter.font();
	font.setPointSize(10);
	painter.setFont(font);
#if 0
	const QFontMetrics fm(painter.font());
	const int textX = (q->width() - fm.horizontalAdvance(strCurValue)) / 2;
	const int textY = (q->height() + fm.height()) / 2 - fm.descent();
	painter.drawText(textX, textY, strCurValue);
#else
	painter.drawText(q->rect(), Qt::AlignCenter, strCurValue);
#endif
	painter.restore();
}

/**
 * @brief \class CALCircularProgressDelegate
 * Internal class for CALCircularProgress
 * @param parent pointer to the CALCircularProgress
 */
CALCircularProgressDelegate::CALCircularProgressDelegate(CALCircularProgress* parent)
: QObject(parent), progress(parent), dashOffset(0.0), dashLength(89), angle(0) {
	Q_ASSERT(parent);
}

CALCircularProgressDelegate::~CALCircularProgressDelegate() = default;

void CALCircularProgressDelegate::setDashOffset(const qreal offset) {
	dashOffset = offset;
	progress->update();
}

qreal CALCircularProgressDelegate::getDashOffset() const {
	return dashOffset;
}

void CALCircularProgressDelegate::setDashLength(const qreal length) {
	dashLength = length;
	progress->update();
}

qreal CALCircularProgressDelegate::getDashLength() const {
	return dashLength;
}

void CALCircularProgressDelegate::setAngle(const int nAngle) {
	angle = nAngle;
	progress->update();
}

int CALCircularProgressDelegate::getAngle() const {
	return angle;
}

/**
 * @brief \class CALCircularProgress
 * @param parent pointer to the parent class
 */
CALCircularProgress::CALCircularProgress(QWidget* parent): QProgressBar(parent), d_ptr(new CALCircularProgressPrivate(this, this)) {
	d_func()->init();
}

CALCircularProgress::~CALCircularProgress() = default;

void CALCircularProgress::setProgressType(const ALProgressType::ProgressType& type) {
	Q_D(CALCircularProgress);

	if (this->isVisible()) {
		switch (type) {
			case ALProgressType::DiscontinuousLoading:
			case ALProgressType::ContinuousLoading: {
				if (!d->timer->isActive()) {
					d->timer->start(30);
				}
				break;
			}
			default: {
				d->timer->stop();
				break;
			}
		}
	}

	d->progressType = type;
	update();
}

ALProgressType::ProgressType CALCircularProgress::getProgressType() const {
	return d_func()->progressType;
}

void CALCircularProgress::setLineWidth(const qreal width) {
	d_func()->penWidth = width;
	update();
	updateGeometry();
}

qreal CALCircularProgress::getLineWidth() const {
	return d_func()->penWidth;
}

void CALCircularProgress::setSize(const int size) {
	d_func()->size = size;
	update();
	updateGeometry();
}

int CALCircularProgress::getSize() const {
	return d_func()->size;
}

void CALCircularProgress::setColor(const QColor& color) {
	d_func()->color = color;
	update();
}

QColor CALCircularProgress::getColor() const {
	return d_func()->color;
}

void CALCircularProgress::setDisabledColor(const QColor& color) {
	d_func()->disabledColor = color;
	update();
}

QColor CALCircularProgress::getDisabledColor() const {
	return d_func()->disabledColor;
}

void CALCircularProgress::setShowProgressText(const bool bShow) {
	d_func()->showProgressText = bShow;
	update();
}

bool CALCircularProgress::getShowProgressText() const {
	return d_func()->showProgressText;
}

void CALCircularProgress::setTextColor(const QColor& color) {
	d_func()->textColor = color;
	update();
}

QColor CALCircularProgress::getTextColor() const {
	return d_func()->textColor;
}

QSize CALCircularProgress::sizeHint() const {
	Q_D(const CALCircularProgress);

	const qreal s = d->size + d->penWidth + 8;
	return { static_cast<int>(s), static_cast<int>(s) };
}

void CALCircularProgress::showEvent(QShowEvent* event) {
	Q_D(CALCircularProgress);

	switch (d->progressType) {
		case ALProgressType::DiscontinuousLoading:
		case ALProgressType::ContinuousLoading: {
			if (!d->timer->isActive()) {
				d->timer->start(30);
			}
			break;
		}
		default: {
			d->timer->stop();
			break;
		}
	}

	QProgressBar::showEvent(event);
}

void CALCircularProgress::hideEvent(QHideEvent* event) {
	Q_D(CALCircularProgress);

	d->timer->stop();

	QProgressBar::hideEvent(event);
}

void CALCircularProgress::paintEvent(QPaintEvent* event) {
	Q_D(CALCircularProgress);

	QPainter painter(this);
	painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);
	painter.save();

	if (!isEnabled()) {
		QPen pen;
		pen.setCapStyle(Qt::RoundCap);
		pen.setWidthF(d->penWidth);
		pen.setColor(d->disabledColor);
		painter.setPen(pen);
		painter.drawLine(rect().center() - QPointF(20, 20), rect().center() + QPointF(20, 20));
		painter.drawLine(rect().center() + QPointF(20, -20), rect().center() - QPointF(20, -20));
		painter.restore();
		return;
	}

	if (ALProgressType::IndeterminateProgress == d->progressType) {
		painter.translate(static_cast<qreal>(width()) / 2, static_cast<qreal>(height()) / 2);
		painter.rotate(d->delegate->getAngle());
	}

	QPen pen;
	pen.setCapStyle(Qt::RoundCap);
	pen.setWidthF(d->penWidth);
	pen.setColor(d->color);

	switch (d->progressType) {
		case ALProgressType::IndeterminateProgress: {
			d->drawIndeterminate(painter, pen);
			break;
		}
		case ALProgressType::DeterminateProgress: {
			d->drawDeterminate(painter, pen);
			break;
		}
		case ALProgressType::DiscontinuousLoading: {
			d->drawDiscontinuous(painter, pen);
			break;
		}
		case ALProgressType::ContinuousLoading: {
			d->drawContinuous(painter, pen);
			break;
		}
		default: break;
	}

	if (d->showProgressText) {
		d->drawValue(painter, pen);
	}

	painter.restore();
}
}
