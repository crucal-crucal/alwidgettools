#include "uvprogressbar.hpp"

#include <QPainter>
#include <QPainterPath>
#include <QPropertyAnimation>

#include "uvprogressbar_p.hpp"

/**
 * @brief \class CUVProgressBarPrivate
 * Internal class for CUVProgressBar
 * @param q pointer to the public class
 * @param parent pointer to the parent class
 */
CUVProgressBarPrivate::CUVProgressBarPrivate(CUVProgressBar* q, QObject* parent): QObject(parent), q_ptr(q) {
}

CUVProgressBarPrivate::~CUVProgressBarPrivate() = default;

void CUVProgressBarPrivate::init() {
	Q_Q(CUVProgressBar);

	delegate = new CUVProgressBarDelegate(q);
	progressType = UVProgressType::IndeterminateProgress;
	position = UVProgressType::Right;
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

void CUVProgressBarPrivate::calculateRects(QRect& progressRect, QRect& textRect, const UVProgressType::ProgressTextPosition& position) const {
	Q_Q(const CUVProgressBar);

	const int progressWidth = q->width();
	const int progressHeight = q->height();

	if (!showProgerssText) {
		progressRect = QRect(0, 0, progressWidth, progressHeight);
		textRect = QRect();
		return;
	}

	constexpr int textReservedSize = 40; // 预留文本宽度

	switch (position) {
		case UVProgressType::Center: {
			progressRect = QRect(0, 0, progressWidth, progressHeight);
			textRect = QRect(0, 0, progressWidth, progressHeight);
			break;
		}
		case UVProgressType::Left: {
			progressRect = QRect(textReservedSize, 0, progressWidth - textReservedSize, progressHeight);
			textRect = QRect(0, 0, textReservedSize, progressHeight);
			break;
		}
		case UVProgressType::Right: {
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
 * @brief \class CUVProgressBarDelegate
 * Internal class for CUVProgressBar
 * @param parent pointer to the CUVProgressBar
 */
CUVProgressBarDelegate::CUVProgressBarDelegate(CUVProgressBar* parent): QObject(parent), m_progress(parent), m_offset(0) {
	Q_ASSERT(parent);
}

CUVProgressBarDelegate::~CUVProgressBarDelegate() = default;

void CUVProgressBarDelegate::setOffset(const qreal offset) {
	m_offset = offset;
	m_progress->update();
}

qreal CUVProgressBarDelegate::getOffset() const {
	return m_offset;
}

/**
 * @brief \class CUVProgressBar
 * @param parent pointer to the parent class
 */
CUVProgressBar::CUVProgressBar(QWidget* parent): QProgressBar(parent), d_ptr(new CUVProgressBarPrivate(this, this)) {
	d_func()->init();
}

CUVProgressBar::~CUVProgressBar() = default;

void CUVProgressBar::setProgressType(const UVProgressType::ProgressType& type) {
	d_func()->progressType = type;
	update();
}

UVProgressType::ProgressType CUVProgressBar::getProgressType() const {
	return d_func()->progressType;
}

void CUVProgressBar::setProgressTextPosition(const UVProgressType::ProgressTextPosition& position) {
	d_func()->position = position;
	update();
}

UVProgressType::ProgressTextPosition CUVProgressBar::getProgressTextPosition() const {
	return d_func()->position;
}

void CUVProgressBar::setProgressColor(const QColor& color) {
	d_func()->progressColor = color;
	update();
}

QColor CUVProgressBar::getProgressColor() const {
	return d_func()->progressColor;
}

void CUVProgressBar::setBackgroundColor(const QColor& color) {
	d_func()->backgroundColor = color;
	update();
}

QColor CUVProgressBar::getBackgroundColor() const {
	return d_func()->backgroundColor;
}

void CUVProgressBar::setBorderRadius(const qreal& borderRadius) {
	d_func()->borderRadius = borderRadius;
	update();
}

qreal CUVProgressBar::getBorderRadius() const {
	return d_func()->borderRadius;
}

void CUVProgressBar::setShowProgressText(const bool bShow) {
	d_func()->showProgerssText = bShow;
}

bool CUVProgressBar::getShowProgressText() const {
	return d_func()->showProgerssText;
}

void CUVProgressBar::paintEvent(QPaintEvent* event) {
	Q_D(CUVProgressBar);

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

		if (UVProgressType::IndeterminateProgress == d->progressType) {
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
