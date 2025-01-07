#include "alcircularprogress.hpp"

#include <QPainter>
#include <QPainterPath>
#include <QParallelAnimationGroup>
#include <QPropertyAnimation>
#include <QTimer>

#include "uvcircularprogress_p.hpp"

/**
 * @brief \class CUVCircularProgressPrivate
 * Internal class for CUVCircularProgress
 * @param q pointer to the public class
 * @param parent pointer to the parent class
 */
CUVCircularProgressPrivate::CUVCircularProgressPrivate(CUVCircularProgress* q, QObject* parent): QObject(parent), q_ptr(q) {
}

CUVCircularProgressPrivate::~CUVCircularProgressPrivate() = default;

void CUVCircularProgressPrivate::init() {
	Q_Q(CUVCircularProgress);

	delegate = new CUVCircularProgressDelegate(q);
	progressType = ALProgressType::IndeterminateProgress;
	showProgressText = true;
	color = Qt::green;
	disabledColor = Qt::gray;
	textColor = Qt::black;
	penWidth = 6.25;
	size = 64;
	timer = new QTimer(q);
	connect(timer, &QTimer::timeout, this, &CUVCircularProgressPrivate::_updateAnimation);

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

void CUVCircularProgressPrivate::_updateAnimation() {
	Q_Q(CUVCircularProgress);

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

void CUVCircularProgressPrivate::drawIndeterminate(QPainter& painter, QPen& pen) {
	Q_Q(CUVCircularProgress);

	painter.save();

	QVector<qreal> pattern{};
	pattern << delegate->getDashLength() * size / 50 << 30.0 * size / 50;
	pen.setDashOffset(delegate->getDashOffset() * size / 50);
	pen.setDashPattern(pattern);
	painter.setPen(pen);
	painter.drawEllipse(QPoint(0, 0), size / 2, size / 2);

	painter.restore();
}

void CUVCircularProgressPrivate::drawDeterminate(QPainter& painter, const QPen& pen) {
	Q_Q(CUVCircularProgress);

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

void CUVCircularProgressPrivate::drawDiscontinuous(QPainter& painter, const QPen& pen) {
	Q_Q(CUVCircularProgress);

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

void CUVCircularProgressPrivate::drawContinuous(QPainter& painter, const QPen& pen) {
	Q_Q(CUVCircularProgress);

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

void CUVCircularProgressPrivate::drawValue(QPainter& painter, QPen& pen) {
	Q_Q(CUVCircularProgress);

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
 * @brief \class CUVCircularProgressDelegate
 * Internal class for CUVCircularProgress
 * @param parent pointer to the CUVCircularProgress
 */
CUVCircularProgressDelegate::CUVCircularProgressDelegate(CUVCircularProgress* parent)
: QObject(parent), progress(parent), dashOffset(0.0), dashLength(89), angle(0) {
	Q_ASSERT(parent);
}

CUVCircularProgressDelegate::~CUVCircularProgressDelegate() = default;

void CUVCircularProgressDelegate::setDashOffset(const qreal offset) {
	dashOffset = offset;
	progress->update();
}

qreal CUVCircularProgressDelegate::getDashOffset() const {
	return dashOffset;
}

void CUVCircularProgressDelegate::setDashLength(const qreal length) {
	dashLength = length;
	progress->update();
}

qreal CUVCircularProgressDelegate::getDashLength() const {
	return dashLength;
}

void CUVCircularProgressDelegate::setAngle(const int nAngle) {
	angle = nAngle;
	progress->update();
}

int CUVCircularProgressDelegate::getAngle() const {
	return angle;
}

/**
 * @brief \class CUVCircularProgress
 * @param parent pointer to the parent class
 */
CUVCircularProgress::CUVCircularProgress(QWidget* parent): QProgressBar(parent), d_ptr(new CUVCircularProgressPrivate(this, this)) {
	d_func()->init();
}

CUVCircularProgress::~CUVCircularProgress() = default;

void CUVCircularProgress::setProgressType(const ALProgressType::ProgressType& type) {
	Q_D(CUVCircularProgress);

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

ALProgressType::ProgressType CUVCircularProgress::getProgressType() const {
	return d_func()->progressType;
}

void CUVCircularProgress::setLineWidth(const qreal width) {
	d_func()->penWidth = width;
	update();
	updateGeometry();
}

qreal CUVCircularProgress::getLineWidth() const {
	return d_func()->penWidth;
}

void CUVCircularProgress::setSize(const int size) {
	d_func()->size = size;
	update();
	updateGeometry();
}

int CUVCircularProgress::getSize() const {
	return d_func()->size;
}

void CUVCircularProgress::setColor(const QColor& color) {
	d_func()->color = color;
	update();
}

QColor CUVCircularProgress::getColor() const {
	return d_func()->color;
}

void CUVCircularProgress::setDisabledColor(const QColor& color) {
	d_func()->disabledColor = color;
	update();
}

QColor CUVCircularProgress::getDisabledColor() const {
	return d_func()->disabledColor;
}

void CUVCircularProgress::setShowProgressText(const bool bShow) {
	d_func()->showProgressText = bShow;
	update();
}

bool CUVCircularProgress::getShowProgressText() const {
	return d_func()->showProgressText;
}

void CUVCircularProgress::setTextColor(const QColor& color) {
	d_func()->textColor = color;
	update();
}

QColor CUVCircularProgress::getTextColor() const {
	return d_func()->textColor;
}

QSize CUVCircularProgress::sizeHint() const {
	Q_D(const CUVCircularProgress);

	const qreal s = d->size + d->penWidth + 8;
	return { static_cast<int>(s), static_cast<int>(s) };
}

void CUVCircularProgress::showEvent(QShowEvent* event) {
	Q_D(CUVCircularProgress);

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

void CUVCircularProgress::hideEvent(QHideEvent* event) {
	Q_D(CUVCircularProgress);

	d->timer->stop();

	QProgressBar::hideEvent(event);
}

void CUVCircularProgress::paintEvent(QPaintEvent* event) {
	Q_D(CUVCircularProgress);

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
