#include "alprogressbar.hpp"

#include <QPainter>
#include <QPainterPath>
#include <QPropertyAnimation>

#include "alprogressbar_p.hpp"

namespace AL {
/**
 * @brief \class CALProgressBarPrivate
 * Internal class for CALProgressBar
 * @param q pointer to the public class
 * @param parent pointer to the parent class
 */
CALProgressBarPrivate::CALProgressBarPrivate(CALProgressBar* q, QObject* parent): QObject(parent), q_ptr(q) {
}

CALProgressBarPrivate::~CALProgressBarPrivate() = default;

void CALProgressBarPrivate::init() {
	Q_Q(CALProgressBar);

	delegate = new CALProgressBarDelegate(q);
	progressType = ALProgressType::IndeterminateProgress;
	position = ALProgressType::Right;
	progressColor = Qt::green;
	backgroundColor = Qt::gray;
	borderRadius = 5.0;
	showProgerssText = true;

	const auto animation = new QPropertyAnimation(delegate);
	animation->setPropertyName("offset");
	animation->setTargetObject(delegate);
	animation->setStartValue(0);
	animation->setEndValue(1);
	animation->setDuration(1000);
	animation->setLoopCount(-1);
	animation->start();
}

void CALProgressBarPrivate::calculateRects(QRect& progressRect, QRect& textRect, const ALProgressType::ProgressTextPosition& position) const {
	Q_Q(const CALProgressBar);

	const int progressWidth = q->width();
	const int progressHeight = q->height();

	if (!showProgerssText) {
		progressRect = QRect(0, 0, progressWidth, progressHeight);
		textRect = QRect();
		return;
	}

	constexpr int textReservedSize = 40; // 预留文本宽度

	switch (position) {
		case ALProgressType::Center: {
			progressRect = QRect(0, 0, progressWidth, progressHeight);
			textRect = QRect(0, 0, progressWidth, progressHeight);
			break;
		}
		case ALProgressType::Left: {
			progressRect = QRect(textReservedSize, 0, progressWidth - textReservedSize, progressHeight);
			textRect = QRect(0, 0, textReservedSize, progressHeight);
			break;
		}
		case ALProgressType::Right: {
			progressRect = QRect(0, 0, progressWidth - textReservedSize, progressHeight);
			textRect = QRect(progressWidth - textReservedSize, 0, textReservedSize, progressHeight);
			break;
		}
		default: {
			progressRect = QRect(0, 0, progressWidth, progressHeight);
			textRect = QRect(); // 默认不显示文本
			break;
		}
	}
}

/**
 * @brief \class CALProgressBarDelegate
 * Internal class for CALProgressBar
 * @param parent pointer to the CALProgressBar
 */
CALProgressBarDelegate::CALProgressBarDelegate(CALProgressBar* parent): QObject(parent), m_progress(parent), m_offset(0) {
	Q_ASSERT(parent);
}

CALProgressBarDelegate::~CALProgressBarDelegate() = default;

void CALProgressBarDelegate::setOffset(const qreal offset) {
	m_offset = offset;
	m_progress->update();
}

qreal CALProgressBarDelegate::getOffset() const {
	return m_offset;
}

/**
 * @brief \class CALProgressBar
 * @param parent pointer to the parent class
 */
CALProgressBar::CALProgressBar(QWidget* parent): QProgressBar(parent), d_ptr(new CALProgressBarPrivate(this, this)) {
	d_func()->init();
}

CALProgressBar::~CALProgressBar() = default;

void CALProgressBar::setProgressType(const ALProgressType::ProgressType& type) {
	d_func()->progressType = type;
	update();
}

ALProgressType::ProgressType CALProgressBar::getProgressType() const {
	return d_func()->progressType;
}

void CALProgressBar::setProgressTextPosition(const ALProgressType::ProgressTextPosition& position) {
	d_func()->position = position;
	update();
}

ALProgressType::ProgressTextPosition CALProgressBar::getProgressTextPosition() const {
	return d_func()->position;
}

void CALProgressBar::setProgressColor(const QColor& color) {
	d_func()->progressColor = color;
	update();
}

QColor CALProgressBar::getProgressColor() const {
	return d_func()->progressColor;
}

void CALProgressBar::setBackgroundColor(const QColor& color) {
	d_func()->backgroundColor = color;
	update();
}

QColor CALProgressBar::getBackgroundColor() const {
	return d_func()->backgroundColor;
}

void CALProgressBar::setBorderRadius(const qreal& borderRadius) {
	d_func()->borderRadius = borderRadius;
	update();
}

qreal CALProgressBar::getBorderRadius() const {
	return d_func()->borderRadius;
}

void CALProgressBar::setShowProgressText(const bool bShow) {
	d_func()->showProgerssText = bShow;
}

bool CALProgressBar::getShowProgressText() const {
	return d_func()->showProgerssText;
}

void CALProgressBar::paintEvent(QPaintEvent* event) {
	Q_D(CALProgressBar);

	QPainter painter(this);
	painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing | QPainter::SmoothPixmapTransform);
	painter.save();

	// 背景绘制
	QBrush brush;
	brush.setStyle(Qt::SolidPattern);
	brush.setColor(isEnabled() ? d->backgroundColor : Qt::gray);
	painter.setBrush(brush);
	painter.setPen(Qt::NoPen);

	// 获取进度条和文本区域的位置和大小
	QRect progressRect, textRect;
	d->calculateRects(progressRect, textRect, d->position);

	// 绘制进度条背景
	QPainterPath path;
	path.addRoundedRect(progressRect, d->borderRadius, d->borderRadius);
	painter.setClipPath(path);
	painter.drawRect(progressRect);

	// 进度绘制
	if (isEnabled()) {
		brush.setColor(d->progressColor);
		painter.setBrush(brush);

		if (ALProgressType::IndeterminateProgress == d->progressType) {
			painter.drawRect(static_cast<int>(d->delegate->getOffset() * progressRect.width() * 2 - progressRect.width()), progressRect.top(), progressRect.width(), progressRect.height());
		} else {
			const qreal p = static_cast<qreal>(progressRect.width()) * (value() - minimum()) / (maximum() - minimum());
			painter.drawRect(progressRect.adjusted(0, 0, static_cast<int>(p - progressRect.width()), 0));
		}
	}

	if (d->showProgerssText) {
		// 取消剪切范围，绘制文本
		painter.setClipping(false);

		// 设置字体
		QFont font = painter.font();
		font.setPointSize(10);
		painter.setFont(font);
		// 绘制文本
		QPen pen(Qt::white);
		pen.setWidth(1);
		painter.setPen(pen);
		const QString curValue = QString("%1%").arg(qMax(0, value() * 100 / (maximum() - minimum())));
		painter.drawText(textRect, Qt::AlignCenter, curValue);
	}

	painter.restore();
}
} // namespace AL