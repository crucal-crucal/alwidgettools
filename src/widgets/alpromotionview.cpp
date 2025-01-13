#include "alpromotionview.hpp"

#include <QPainter>
#include <QPropertyAnimation>
#include <QTimer>
#include <QWheelEvent>

#include "alpromotioncard.hpp"
#include "alpromotionview_p.hpp"
#include "althememanager.hpp"

namespace AL {
/**
 * @brief \class CALPromotionViewPrivate
 * Internal class for CALPromotionView
 * @param q pointer to the public class
 * @param parent pointer to the parent class
 */
CALPromotionViewPrivate::CALPromotionViewPrivate(CALPromotionView* q, QObject* parent): QObject(parent), q_ptr(q) {
}

CALPromotionViewPrivate::~CALPromotionViewPrivate() = default;

void CALPromotionViewPrivate::onPromationCardClicked(CALPromotionCard* clickedCard) {
	Q_Q(CALPromotionView);

	const int newCurrentIndex = listPromotionCard.indexOf(clickedCard);
	const int oldCUrrentIndex = currentIndex;
	if (newCurrentIndex == oldCUrrentIndex) {
		return;
	}

	/// card move animation
	const bool isRightToLeft = listPromotionCard.at(oldCUrrentIndex)->x() < listPromotionCard.at(newCurrentIndex)->x();
	int originIndex = getAdjacentIndex(Qt::RightToLeft, newCurrentIndex);
	for (int i = 0; i < listPromotionCard.count(); ++i) {
		CALPromotionCard* card = listPromotionCard.at(originIndex);
		const auto geometryAnimation = new QPropertyAnimation(card, "geometry");
		QRect geometry = card->geometry();
		geometryAnimation->setEasingCurve(QEasingCurve::OutCubic);
		geometryAnimation->setDuration(650);
		geometryAnimation->setStartValue(geometry);
		if (i == 0) {
			// 最左侧卡片
			QRect targetGeometry(-cardCollapseWidth + leftPadding, 0, cardCollapseWidth, q->height() - bottomMargin);
			if (listPromotionCard.count() > 2) {
				if (!isRightToLeft) {
					geometryAnimation->setKeyValueAt(0.7, QRect(geometry.x() + cardCollapseWidth * 0.7 + cardSpacing, 0, cardCollapseWidth, q->height() - bottomMargin)); // NOLINT
					geometryAnimation->setKeyValueAt(0.71, QRect(-cardCollapseWidth, 0, cardCollapseWidth, q->height() - bottomMargin));
				}
				geometryAnimation->setEndValue(targetGeometry);
			} else {
				geometryAnimation->setEndValue(isRightToLeft ? targetGeometry : QRect(leftPadding + cardExpandWidth + cardSpacing * 2, 0, cardCollapseWidth, q->height() - bottomMargin));
			}
			startHorizontalCardPixmapRatioAnimation(card, card->getHorizontalCardPixmapRatio(), 0.5);
		} else if (i == 1) {
			// 展开的卡片
			geometryAnimation->setEndValue(QRect(leftPadding + cardSpacing, 0, cardExpandWidth, q->height() - bottomMargin));
			startHorizontalCardPixmapRatioAnimation(card, card->getHorizontalCardPixmapRatio(), 1);
		} else {
			if (QRect targetGeometry(leftPadding + cardExpandWidth + cardSpacing * i + cardCollapseWidth * (i - 2), 0, cardCollapseWidth, q->height() - bottomMargin); isRightToLeft && targetGeometry.x() > card->x()) {
				connect(geometryAnimation, &QPropertyAnimation::valueChanged, this, [=]() {
					if (card->geometry().right() <= 0) {
						geometryAnimation->pause();
						const qreal timeRatio = geometryAnimation->currentTime() / 650.0;
						geometryAnimation->setKeyValueAt(timeRatio, QRect(listPromotionCard.at(getAdjacentIndex(Qt::RightToLeft, originIndex))->geometry().right() + cardCollapseWidth * (1 - timeRatio), 0, cardCollapseWidth, q->height() - bottomMargin)); // NOLINT
						geometryAnimation->setEndValue(targetGeometry);
						geometryAnimation->resume();
					}
				});
				geometryAnimation->setEndValue(QRect(-cardCollapseWidth * 2 + leftPadding, 0, cardCollapseWidth, q->height() - bottomMargin));
			} else {
				geometryAnimation->setEndValue(targetGeometry);
			}
			card->setHorizontalCardPixmapRatio(0.5);
		}
		originIndex = getAdjacentIndex(Qt::LeftToRight, originIndex);
		geometryAnimation->start(QAbstractAnimation::DeleteWhenStopped);
	}

	currentIndex = newCurrentIndex;
	q->update();
	Q_EMIT q->sigCurrentIndexChanged();
}

void CALPromotionViewPrivate::startCardGeometryAnimation(CALPromotionCard* card, const QRect& start, const QRect& end) {
	const auto geometryAnimation = new QPropertyAnimation(card, "geometry");
	geometryAnimation->setEasingCurve(QEasingCurve::OutCubic);
	geometryAnimation->setDuration(650);
	geometryAnimation->setStartValue(start);
	geometryAnimation->setEndValue(end);
	geometryAnimation->start(QAbstractAnimation::DeleteWhenStopped);
}

void CALPromotionViewPrivate::startHorizontalCardPixmapRatioAnimation(CALPromotionCard* card, const qreal& start, const qreal& end) {
	const auto geometryAnimation = new QPropertyAnimation(card, "horizontalCardPixmapRatio");
	geometryAnimation->setEasingCurve(QEasingCurve::OutCubic);
	geometryAnimation->setDuration(650);
	geometryAnimation->setStartValue(start);
	geometryAnimation->setEndValue(end);
	geometryAnimation->start(QAbstractAnimation::DeleteWhenStopped);
}

void CALPromotionViewPrivate::updatePromotionCardGrometry() {
	Q_Q(CALPromotionView);

	if (const int count = listPromotionCard.count(); count < 3) {
		if (count == 1) {
			listPromotionCard.at(0)->setGeometry(leftPadding + cardSpacing, 0, cardExpandWidth, q->height() - bottomMargin);
		} else {
			listPromotionCard.at(0)->setGeometry(leftPadding + cardSpacing, 0, cardExpandWidth, q->height() - bottomMargin);
			const QRect lastGeometry = listPromotionCard.at(0)->geometry();
			listPromotionCard.at(1)->setGeometry(lastGeometry.right() + cardSpacing, lastGeometry.y(), cardCollapseWidth, lastGeometry.height());
			listPromotionCard.at(1)->setHorizontalCardPixmapRatio(0.5);
		}
	} else {
		for (int i = 0; i < count; ++i) {
			if (i == 0) {
				// 展开的卡片
				listPromotionCard.at(i)->setGeometry(leftPadding + cardSpacing, 0, cardExpandWidth, q->height() - bottomMargin);
				listPromotionCard.at(i)->setHorizontalCardPixmapRatio(1);
			} else if (i == listPromotionCard.count() - 1) {
				// 最左侧卡片
				listPromotionCard.at(i)->setGeometry(-cardCollapseWidth + leftPadding, 0, cardCollapseWidth, q->height() - bottomMargin);
				listPromotionCard.at(i)->setHorizontalCardPixmapRatio(0.5);
			} else {
				const QRect lastGeometry = listPromotionCard.at(i - 1)->geometry();
				listPromotionCard.at(i)->setGeometry(lastGeometry.right() + cardSpacing, lastGeometry.y(), cardCollapseWidth, q->height() - bottomMargin);
				listPromotionCard.at(i)->setHorizontalCardPixmapRatio(0.5);
			}
		}
	}
}

int CALPromotionViewPrivate::getAdjacentIndex(const Qt::LayoutDirection& direction, const int& index) const {
	if (direction == Qt::LeftToRight) {
		if (index == listPromotionCard.count() - 1) {
			return 0;
		}
		return index + 1;
	} else {
		if (index > 0) {
			return index - 1;
		}
		return listPromotionCard.count() - 1;
	}
}

int CALPromotionViewPrivate::getRightLimitX() const {
	const int count = listPromotionCard.count();
	return leftPadding + cardExpandWidth + cardSpacing * (count - 1) + cardCollapseWidth * (count - 3);
}

/**
 * @brief \class CALPromotionView
 * @param parent pointer to the parent class
 */
CALPromotionView::CALPromotionView(QWidget* parent): QWidget(parent), d_ptr(new CALPromotionViewPrivate(this, this)) {
	Q_D(CALPromotionView);

	setFixedHeight(300);
	setObjectName("CALPromotionView");
	setStyleSheet("#CALPromotionView { background-color: transparent; }");

	d->currentIndex = 0;
	d->leftPadding = 40;
	d->cardSpacing = 5;
	d->bottomMargin = 30;
	d->indicatorSpacing = 12;
	d->cardExpandWidth = 600;
	d->cardCollapseWidth = 300;
	d->autoScrollInterval = 5000;
	d->isAutoScroll = false;
	d->isAllowSwitch = true;
	d->themeMode = ALTheme->getThemeMode();
	connect(ALTheme, &CALThemeManager::sigThemeModeChanged, this, [=](const ALThemeType::ThemeMode& mode) { d->themeMode = mode; });
	d->autoScrollTimer = new QTimer(this);
	connect(d->autoScrollTimer, &QTimer::timeout, this, [=]() {
		if (isVisible() && d->listPromotionCard.count() > 2) {
			d->onPromationCardClicked(d->listPromotionCard.at(d->getAdjacentIndex(Qt::LeftToRight, d->currentIndex)));
		}
	});
}

CALPromotionView::~CALPromotionView() = default;

void CALPromotionView::setCardExpandWidth(const int width) {
	Q_D(CALPromotionView);

	if (width <= 0) {
		return;
	}

	d->cardExpandWidth = width;
	d->updatePromotionCardGrometry();
}

int CALPromotionView::getCardExpandWidth() const {
	return d_func()->cardExpandWidth;
}

void CALPromotionView::setCardCollapseWidth(const int width) {
	Q_D(CALPromotionView);

	if (width <= 0) {
		return;
	}

	d->cardCollapseWidth = width;
	d->updatePromotionCardGrometry();
}

int CALPromotionView::getCardCollapseWidth() const {
	return d_func()->cardCollapseWidth;
}

void CALPromotionView::setCurrentIndex(const int index) {
	Q_D(CALPromotionView);

	if (index < 0 || index >= d->listPromotionCard.count()) {
		return;
	}

	d->onPromationCardClicked(d->listPromotionCard.at(index));
}

int CALPromotionView::getCurrentIndex() const {
	return d_func()->currentIndex;
}

void CALPromotionView::setAutoScrollInterval(const int interval) {
	Q_D(CALPromotionView);

	if (interval < 400) {
		return;
	}

	d->autoScrollInterval = interval;
	Q_EMIT sigAutoScrollIntervalChanged();
}

int CALPromotionView::getAutoScrollInterval() const {
	return d_func()->autoScrollInterval;
}

void CALPromotionView::setIsAutoScroll(const bool isAutoScroll) {
	Q_D(CALPromotionView);

	if (isAutoScroll) {
		d->autoScrollTimer->start(d->autoScrollInterval);
	} else {
		d->autoScrollTimer->stop();
	}
	d->isAutoScroll = isAutoScroll;
	Q_EMIT sigIsAutoScrollChanged();
}

bool CALPromotionView::getIsAutoScroll() const {
	return d_func()->isAutoScroll;
}

void CALPromotionView::appendPromotionCard(CALPromotionCard* card) {
	Q_D(CALPromotionView);

	if (!card || d->listPromotionCard.contains(card)) {
		return;
	}

	card->setMinimumSize(0, 0);
	card->setMaximumSize(10000, 10000);
	card->setParent(this);
	d->listPromotionCard.append(card);
	connect(card, &CALPromotionCard::sigPromotionCardClicked, this, [=]() { d->onPromationCardClicked(card); });
	d->updatePromotionCardGrometry();
}

void CALPromotionView::wheelEvent(QWheelEvent* event) {
	Q_D(CALPromotionView);

	if (d->isAllowSwitch) {
		d->autoScrollTimer->stop();
		d->isAllowSwitch = false;
		QTimer::singleShot(400, this, [=] {
			d->isAllowSwitch = true;
			d->autoScrollTimer->start(d->autoScrollInterval);
		});
		d->onPromationCardClicked(d->listPromotionCard.at(d->getAdjacentIndex(event->angleDelta().y() > 0 ? Qt::RightToLeft : Qt::LeftToRight, d->currentIndex)));
	}
	event->accept();
}

void CALPromotionView::mousePressEvent(QMouseEvent* event) {
	Q_D(CALPromotionView);

	for (int i = 0; i < d->listPromotionCard.count(); ++i) {
		if (d->allpoints.at(i).contains(event->pos())) {
			d->autoScrollTimer->stop();
			d->onPromationCardClicked(d->listPromotionCard.at(i));
			d->autoScrollTimer->start(d->autoScrollInterval);
			break;
		}
	}

	QWidget::mousePressEvent(event);
}

void CALPromotionView::paintEvent(QPaintEvent* event) {
	Q_D(CALPromotionView);

	QPainter painter(this);
	painter.save();
	painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing | QPainter::SmoothPixmapTransform);
	painter.setPen(Qt::NoPen);
	// 页标指示器绘制
	const int promotionCardCount = d->listPromotionCard.count();
	const bool isCountOdd = promotionCardCount % 2;
	const QPoint startPoint = isCountOdd ? QPoint(width() / 2 - promotionCardCount / 2 * d->indicatorSpacing, height() - d->bottomMargin / 2) : QPoint(width() / 2 - promotionCardCount / 2 * d->indicatorSpacing - d->indicatorSpacing / 2, height() - d->bottomMargin / 2);
	d->allpoints.clear();
	for (int i = 0; i < promotionCardCount; ++i) {
		const bool isCurrentIndex = i == d->currentIndex;
		painter.setBrush(ALThemeColor(d->themeMode, isCurrentIndex ? ALThemeType::PrimaryHover : ALThemeType::BasicIndicator));
		const auto pointRectF = QRectF(startPoint.x() + i * d->indicatorSpacing, startPoint.y(), isCurrentIndex ? 5.5 : 3.5, isCurrentIndex ? 5.5 : 3.5);
		d->allpoints.append(pointRectF);
		painter.drawEllipse(pointRectF);
	}
	painter.restore();
}
} // namespace AL