#include "alroller.hpp"

#include <QPainter>
#include <QPropertyAnimation>
#include <QWheelEvent>

#include "alroller_p.hpp"
#include "althememanager.hpp"

/**
 * @brief \namespace AL
 */
namespace AL {
/**
 * @brief \class CALRollerPrivate
 * Internal class for CALRoller
 * @param q pointer to the public class
 * @param parent pointer to the parent class
 */
CALRollerPrivate::CALRollerPrivate(CALRoller* q, QObject* parent): QObject(parent), q_ptr(q) {
}

CALRollerPrivate::~CALRollerPrivate() = default;

void CALRollerPrivate::scroll(const int delta) {
	const int steps = delta / 120;
	targetScrollOffset -= steps * itemHeight;
	targetScrollOffset = qRound(targetScrollOffset / itemHeight) * itemHeight;
	scrollAnimation->stop();
	scrollAnimation->setStartValue(scrollOffset);
	scrollAnimation->setEndValue(targetScrollOffset);
	scrollAnimation->start();
}

void CALRollerPrivate::drawItem(QPainter& painter, const int& index, const int& startIndexOffset, const int& yStart, const int& yEnd, const int& centerYStart, const int& centerYEnd) {
	Q_Q(CALRoller);

	qreal y = (index + startIndexOffset) * itemHeight - scrollOffset;
	while (y <= yStart) {
		y += itemList.size() * itemHeight;
	}
	while (y >= yEnd) {
		y -= itemList.size() * itemHeight;
	}
	if (y < yStart || y > yEnd) {
		return;
	}

	/// 可见区域绘制
	painter.save();
	painter.translate(0, y);
	// text
	if (y >= centerYStart && y <= centerYEnd) {
		painter.setPen(ALThemeColor(themeMode, ALThemeType::BasicTextInvert));
	} else {
		if (const auto itemRect = QRectF(0, y, q->width(), itemHeight); !mousePoint.isNull() && scrollAnimation->state() == QPropertyAnimation::State::Stopped && itemRect.contains(mousePoint)) {
			painter.setBrush(ALThemeColor(themeMode, ALThemeType::BasicHover));
			painter.drawRoundedRect(QRect(indicatorMargin, indicatorMargin, q->width() - 2 * indicatorMargin, itemHeight - 2 * indicatorMargin), borderRadius, borderRadius);
		}
		painter.setPen(ALThemeColor(themeMode, ALThemeType::BasicText));
	}
	painter.drawText(QRect(0, 0, q->width(), itemHeight), Qt::AlignCenter | Qt::TextSingleLine, itemList.at(index));
	painter.restore();
}

/**
 * @brief \class CALRoller
 * @param parent pointer to the parent class
 */
CALRoller::CALRoller(QWidget* parent): QWidget(parent), d_ptr(new CALRollerPrivate(this, this)) {
	Q_D(CALRoller);

	d->penBorderWidth = 1;
	d->indicatorMargin = 3;
	d->targetScrollOffset = 0;
	d->scrollOffset = 0;
	d->itemHeight = 35;
	d->borderRadius = 3;
	d->maxVisibleItems = 5;
	d->currentIndex = 0;
	d->themeMode = ALTheme->getThemeMode();
	connect(ALTheme, &CALThemeManager::sigThemeModeChanged, this, [d](const ALThemeType::ThemeMode& mode) { d->themeMode = mode; });
	d->scrollAnimation = new QPropertyAnimation(d, "scrollOffset");
	connect(d->scrollAnimation, &QPropertyAnimation::valueChanged, this, [this]() { update(); });
	connect(d->scrollAnimation, &QPropertyAnimation::finished, this, [d, this]() {
		while (d->scrollOffset < -d->itemList.size() * d->itemHeight) {
			d->scrollOffset += d->itemList.size() * d->itemHeight;
		}
		while (d->scrollOffset > d->itemList.size() * d->itemHeight) {
			d->scrollOffset -= d->itemList.size() * d->itemHeight;
		}
		d->targetScrollOffset = d->scrollOffset;
		int currentIndex = 0;
		if (d->scrollOffset >= 0) {
			currentIndex = static_cast<int>(d->scrollOffset / d->itemHeight);
		} else if (d->scrollOffset <= -d->itemHeight) {
			currentIndex = d->itemList.count() + static_cast<int>(d->scrollOffset / d->itemHeight);
		}
		if (currentIndex != d->currentIndex) {
			d->currentIndex = currentIndex;
			Q_EMIT sigCurrentIndexChanged();
		}
		update();
	});

	setFixedSize(90, 175);
	setMouseTracking(true);
	setObjectName("CALRoller");
	setStyleSheet("#CALRoller { background-color: transparent; }");
	QFont font = this->font();
	font.setPixelSize(16);
	setFont(font);
}

CALRoller::~CALRoller() = default;

QString CALRoller::getCurrentData() const {
	return d_func()->itemList.at(d_func()->currentIndex);
}

void CALRoller::setBorderRadius(const int radius) {
	d_func()->borderRadius = radius;
	Q_EMIT sigBorderRadiusChanged();
}

int CALRoller::getBorderRadius() const {
	return d_func()->borderRadius;
}

void CALRoller::setItemHeight(const int height) {
	Q_D(CALRoller);

	d->itemHeight = height;
	setFixedHeight(d->maxVisibleItems * height);
	update();
	Q_EMIT sigItemHeightChanged();
}

int CALRoller::getItemHeight() const {
	return d_func()->itemHeight;
}

void CALRoller::setMaxVisibleItems(const int count) {
	Q_D(CALRoller);

	d->maxVisibleItems = count;
	setFixedHeight(d->maxVisibleItems * d->itemHeight);
	update();
	Q_EMIT sigMaxVisibleItemsChanged();
}

int CALRoller::getMaxVisibleItems() const {
	return d_func()->maxVisibleItems;
}

void CALRoller::setCurrentIndex(const int index) {
	Q_D(CALRoller);

	if (index >= d->itemList.count()) {
		return;
	}

	d->currentIndex = index;
	d->scrollOffset = d->itemHeight * index;
	d->targetScrollOffset = d->scrollOffset;
	update();
	Q_EMIT sigCurrentIndexChanged();
}

int CALRoller::getCurrentIndex() const {
	return d_func()->currentIndex;
}

void CALRoller::setItemList(const QStringList& list) {
	d_func()->itemList = list;
	update();
	Q_EMIT sigItemListChanged();
}

QStringList CALRoller::getItemList() const {
	return d_func()->itemList;
}

void CALRoller::wheelEvent(QWheelEvent* event) {
	d_func()->scroll(event->angleDelta().y());
	event->accept();
}

void CALRoller::mouseReleaseEvent(QMouseEvent* event) {
	Q_D(CALRoller);

	const int centerIndex = d->maxVisibleItems >> 1;
	const int clickIndex = event->pos().y() / d->itemHeight;
	const int jumpCount = qAbs(centerIndex - clickIndex);
	if (clickIndex > centerIndex) {
		for (int i = 0; i < jumpCount; ++i) {
			d->scroll(-120);
		}
	} else if (clickIndex < centerIndex) {
		for (int i = 0; i < jumpCount; ++i) {
			d->scroll(120);
		}
	}

	QWidget::mouseReleaseEvent(event);
}

void CALRoller::mouseMoveEvent(QMouseEvent* event) {
	d_func()->mousePoint = event->pos();
	update();

	QWidget::mouseMoveEvent(event);
}

void CALRoller::leaveEvent(QEvent* event) {
	d_func()->mousePoint = {};
	update();
	QWidget::leaveEvent(event);
}

void CALRoller::paintEvent(QPaintEvent* event) {
	Q_D(CALRoller);

	QPainter painter(this);
	painter.save();
	painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing | QPainter::SmoothPixmapTransform);
	/// 背景绘制
	const QRect foregroundRect(d->penBorderWidth, d->penBorderWidth, width() - 2 * d->penBorderWidth, height() - 2 * d->penBorderWidth);
	painter.setPen(QPen(ALThemeColor(d->themeMode, ALThemeType::BasicBorder), d->penBorderWidth));
	painter.setBrush(ALThemeColor(d->themeMode, isEnabled() ? ALThemeType::BasicBase : ALThemeType::BasicDisable));
	painter.drawRoundedRect(foregroundRect, d->borderRadius, d->borderRadius);
	/// 中心指示区域绘制
	painter.setPen(Qt::NoPen);
	painter.setBrush(ALThemeColor(d->themeMode, ALThemeType::PrimaryNormal));
	const bool isEven = d->maxVisibleItems % 2 == 0;
	const int atop = isEven ? d->itemHeight * d->maxVisibleItems / 2 : (height() - d->itemHeight) / 2 + d->indicatorMargin;
	painter.drawRoundedRect(QRect(d->indicatorMargin, atop, width() - 2 * d->indicatorMargin, d->itemHeight - 2 * d->indicatorMargin), d->borderRadius, d->borderRadius);

	const int yStart = -d->itemHeight;
	const int yEnd = height();
	const int centerYStart = isEven ? d->itemHeight * d->maxVisibleItems / 2 : height() / 2 - d->itemHeight;
	const int centerYEnd = isEven ? d->itemHeight * d->maxVisibleItems / 2 + d->itemHeight / 2 : height() / 2;
	const int startIndexOffset = d->maxVisibleItems / 2;
	for (int i = 0; i < d->itemList.size(); ++i) {
		d->drawItem(painter, i, startIndexOffset, yStart, yEnd, centerYStart, centerYEnd);
	}
	painter.restore();
}
}
