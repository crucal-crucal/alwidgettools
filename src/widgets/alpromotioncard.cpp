#include "alpromotioncard.hpp"

#include <QMouseEvent>
#include <QPainter>
#include <QPainterPath>
#include <QPropertyAnimation>
#include <QtMath>

#include "alpromotioncard_p.hpp"
#include "althememanager.hpp"

/**
 * @brief \namespace AL
 */
namespace AL {
/**
 * @brief \class CALPromotionCardPrivate
 * Internal class for CALPromotionCard
 * @param q pointer to the public class
 * @param parent pointer to the parent class
 */
CALPromotionCardPrivate::CALPromotionCardPrivate(CALPromotionCard* q, QObject* parent): QObject(parent), q_ptr(q) {
}

CALPromotionCardPrivate::~CALPromotionCardPrivate() = default;

void CALPromotionCardPrivate::setPressRadius(const qreal pressRadius) {
	this->pressRadius = pressRadius;
	Q_EMIT sigPressRadiusChanged();
}

qreal CALPromotionCardPrivate::getPressRadius() const {
	return pressRadius;
}

void CALPromotionCardPrivate::setHoverOpacity(const qreal hoverOpacity) {
	this->hoverOpacity = hoverOpacity;
	Q_EMIT sigHoverOpacityChanged();
}

qreal CALPromotionCardPrivate::getHoverOpacity() const {
	return hoverOpacity;
}

void CALPromotionCardPrivate::setPressOpacity(const qreal pressOpacity) {
	this->pressOpacity = pressOpacity;
	Q_EMIT sigPressOpacityChanged();
}

qreal CALPromotionCardPrivate::getPressOpacity() const {
	return pressOpacity;
}

qreal CALPromotionCardPrivate::getLongestDistance(const QPoint& point) {
	Q_Q(CALPromotionCard);
	const qreal topLeftDis = distance(point, QPoint(0, 0));
	const qreal topRightDis = distance(point, QPoint(q->width(), 0));
	const qreal bottomLeftDis = distance(point, QPoint(0, q->height()));
	const qreal bottomRightDis = distance(point, QPoint(q->width(), q->height()));
	QList<qreal> distList{ topLeftDis, topRightDis, bottomLeftDis, bottomRightDis };
	std::sort(distList.begin(), distList.end());
	return distList.last();
}

qreal CALPromotionCardPrivate::distance(const QPoint& point1, const QPoint& point2) {
	return std::sqrt((point1.x() - point2.x()) * (point1.x() - point2.x()) + (point1.y() - point2.y()) * (point1.y() - point2.y()));
}

void CALPromotionCardPrivate::startHoverOpacityAnimation(const bool isVisiable) {
	Q_Q(CALPromotionCard);

	const auto opacityAnimation = new QPropertyAnimation(this, "hoverOpacity");
	connect(opacityAnimation, &QPropertyAnimation::valueChanged, this, [=]() { q->update(); });
	opacityAnimation->setDuration(250);
	opacityAnimation->setStartValue(hoverOpacity);
	opacityAnimation->setEndValue(isVisiable ? 1 : 0);
	opacityAnimation->start(QAbstractAnimation::DeleteWhenStopped);
}

/**
 * @brief \class CALPromotionCard
 * @param parent pointer to the parent class
 */
CALPromotionCard::CALPromotionCard(QWidget* parent): QWidget(parent), d_ptr(new CALPromotionCardPrivate(this, this)) {
	Q_D(CALPromotionCard);

	d->pressOpacity = 0;
	d->hoverOpacity = 0;
	d->pressRadius = 0;
	d->borderRadius = 5;
	d->shadowBorderWidth = 6;
	d->horizontalCardPixmapRatio = 1;
	d->verticalCardPixmapRatio = 1;
	d->cardTitle = "";
	d->promotionTitle = "";
	d->title = "";
	d->subTitle = "";
	d->cardTitleColor = Qt::white;
	d->promotionTitleColor = Qt::white;
	d->titleColor = Qt::white;
	d->subTitleColor = Qt::white;
	d->promotionTitleBaseColor = QColor(0, 0, 0, 120);
	d->cardTitlePixelSize = 22;
	d->promotionTitlePixelSize = 12;
	d->titlePixelSize = 25;
	d->subTitlePixelSize = 16;
	d->isPressAnimationFinished = true;
	d->hoverGradient = new QRadialGradient();
	d->hoverGradient->setRadius(170);
	d->hoverGradient->setColorAt(0, QColor(0xFF, 0xFF, 0xFF, 40));
	d->hoverGradient->setColorAt(1, QColor(0xFF, 0xFF, 0xFF, 0));
	d->pressGradient = new QRadialGradient();
	d->pressGradient->setRadius(170);
	d->pressGradient->setColorAt(0, QColor(0xFF, 0xFF, 0xFF, 40));
	d->pressGradient->setColorAt(1, QColor(0xFF, 0xFF, 0xFF, 0));

	setMouseTracking(true);
	setObjectName("CALPromotionCard");
	setStyleSheet("#CALPromotionCard { background-color: transparent; }");
}

CALPromotionCard::~CALPromotionCard() {
	SAFE_DELETE(d_func()->pressGradient)
	SAFE_DELETE(d_func()->hoverGradient)
}

void CALPromotionCard::setCardTitlePixelSize(const int cardTitlePixelSize) {
	d_func()->cardTitlePixelSize = cardTitlePixelSize;
	Q_EMIT sigCardTitlePixelSizeChanged();
}

int CALPromotionCard::getCardTitlePixelSize() const {
	return d_func()->cardTitlePixelSize;
}

void CALPromotionCard::setPromotionTitlePixelSize(const int promotionTitlePixelSize) {
	d_func()->promotionTitlePixelSize = promotionTitlePixelSize;
	Q_EMIT sigPromotionTitlePixelSizeChanged();
}

int CALPromotionCard::getPromotionTitlePixelSize() const {
	return d_func()->promotionTitlePixelSize;
}

void CALPromotionCard::setTitlePixelSize(const int titlePixelSize) {
	d_func()->titlePixelSize = titlePixelSize;
	Q_EMIT sigTitlePixelSizeChanged();
}

int CALPromotionCard::getTitlePixelSize() const {
	return d_func()->titlePixelSize;
}

void CALPromotionCard::setSubTitlePixelSize(const int subTitlePixelSize) {
	d_func()->subTitlePixelSize = subTitlePixelSize;
	Q_EMIT sigSubTitlePixelSizeChanged();
}

int CALPromotionCard::getSubTitlePixelSize() const {
	return d_func()->subTitlePixelSize;
}

void CALPromotionCard::setBorderRadius(const qreal borderRadius) {
	d_func()->borderRadius = borderRadius;
	Q_EMIT sigBorderRadiusChanged();
}

qreal CALPromotionCard::getBorderRadius() const {
	return d_func()->borderRadius;
}

void CALPromotionCard::setHorizontalCardPixmapRatio(const qreal horizontalCardPixmapRatio) {
	if (horizontalCardPixmapRatio > 0 && horizontalCardPixmapRatio < 1) {
		d_func()->horizontalCardPixmapRatio = horizontalCardPixmapRatio;
		Q_EMIT sigHorizontalCardPixmapRatioChanged();
	}
}

qreal CALPromotionCard::getHorizontalCardPixmapRatio() const {
	return d_func()->horizontalCardPixmapRatio;
}

void CALPromotionCard::setVerticalCardPixmapRatio(const qreal verticalCardPixmapRatio) {
	if (verticalCardPixmapRatio > 0 && verticalCardPixmapRatio <= 1) {
		d_func()->verticalCardPixmapRatio = verticalCardPixmapRatio;
		Q_EMIT sigVerticalCardPixmapRatioChanged();
	}
}

qreal CALPromotionCard::getVerticalCardPixmapRatio() const {
	return d_func()->verticalCardPixmapRatio;
}

void CALPromotionCard::setCardPixmap(const QPixmap& cardPixmap) {
	d_func()->cardPixmap = cardPixmap;
	Q_EMIT sigCardPixmapChanged();
}

QPixmap CALPromotionCard::getCardPixmap() const {
	return d_func()->cardPixmap;
}

void CALPromotionCard::setCardTitle(const QString& cardTitle) {
	d_func()->cardTitle = cardTitle;
	Q_EMIT sigCardTitleChanged();
}

QString CALPromotionCard::getCardTitle() const {
	return d_func()->cardTitle;
}

void CALPromotionCard::setPromotionTitle(const QString& promotionTitle) {
	d_func()->promotionTitle = promotionTitle;
	Q_EMIT sigPromotionTitleChanged();
}

QString CALPromotionCard::getPromotionTitle() const {
	return d_func()->promotionTitle;
}

void CALPromotionCard::setTitle(const QString& title) {
	d_func()->title = title;
	Q_EMIT sigTitleChanged();
}

QString CALPromotionCard::getTitle() const {
	return d_func()->title;
}

void CALPromotionCard::setSubTitle(const QString& subTitle) {
	d_func()->subTitle = subTitle;
	Q_EMIT sigSubTitleChanged();
}

QString CALPromotionCard::getSubTitle() const {
	return d_func()->subTitle;
}

void CALPromotionCard::setCardTitleColor(const QColor& cardTitleColor) {
	d_func()->cardTitleColor = cardTitleColor;
	Q_EMIT sigCardTitleChanged();
}

QColor CALPromotionCard::getCardTitleColor() const {
	return d_func()->cardTitleColor;
}

void CALPromotionCard::setPromotionTitleColor(const QColor& promotionTitleColor) {
	d_func()->promotionTitleColor = promotionTitleColor;
	Q_EMIT sigPromotionTitleChanged();
}

QColor CALPromotionCard::getPromotionTitleColor() const {
	return d_func()->promotionTitleColor;
}

void CALPromotionCard::setPromotionTitleBaseColor(const QColor& promotionTitleBaseColor) {
	d_func()->promotionTitleBaseColor = promotionTitleBaseColor;
	Q_EMIT sigPromotionTitleChanged();
}

QColor CALPromotionCard::getPromotionTitleBaseColor() const {
	return d_func()->promotionTitleBaseColor;
}

void CALPromotionCard::setTitleColor(const QColor& titleColor) {
	d_func()->titleColor = titleColor;
	Q_EMIT sigTitleChanged();
}

QColor CALPromotionCard::getTitleColor() const {
	return d_func()->titleColor;
}

void CALPromotionCard::setSubTitleColor(const QColor& subTitleColor) {
	d_func()->subTitleColor = subTitleColor;
	Q_EMIT sigSubTitleChanged();
}

QColor CALPromotionCard::getSubTitleColor() const {
	return d_func()->subTitleColor;
}

bool CALPromotionCard::event(QEvent* event) {
	Q_D(CALPromotionCard);

	switch (event->type()) {
		case QEvent::MouseButtonPress: {
			const auto mouseEvent = dynamic_cast<QMouseEvent*>(event);
			const auto opacityAnimation = new QPropertyAnimation(d, "pressOpacity");
			connect(opacityAnimation, &QPropertyAnimation::valueChanged, this, [=]() { update(); });
			connect(opacityAnimation, &QPropertyAnimation::finished, this, [=]() { d->isPressAnimationFinished = true; });
			opacityAnimation->setEasingCurve(QEasingCurve::InQuad);
			opacityAnimation->setDuration(300);
			opacityAnimation->setStartValue(1);
			opacityAnimation->setEndValue(0);
			opacityAnimation->start(QAbstractAnimation::DeleteWhenStopped);

			const auto pressAnimation = new QPropertyAnimation(d, "pressRadius");
			connect(pressAnimation, &QPropertyAnimation::valueChanged, this, [=](const QVariant& value) { d->pressGradient->setRadius(value.toReal()); });
			pressAnimation->setEasingCurve(QEasingCurve::InQuad);
			pressAnimation->setDuration(300);
			pressAnimation->setStartValue(30);
			pressAnimation->setEndValue(d->getLongestDistance(mouseEvent->pos()) * 1.8);
			pressAnimation->start(QAbstractAnimation::DeleteWhenStopped);
			d->isPressAnimationFinished = false;
			d->pressGradient->setFocalPoint(mouseEvent->pos());
			d->pressGradient->setCenter(mouseEvent->pos());
			d->startHoverOpacityAnimation(false);
			break;
		}
		case QEvent::MouseButtonRelease: {
			Q_EMIT sigPromotionCardClicked();
			break;
		}
		case QEvent::MouseMove: {
			const auto mouseEvent = dynamic_cast<QMouseEvent*>(event);
			if (d->hoverOpacity < 1 && d->isPressAnimationFinished) {
				d->startHoverOpacityAnimation(true);
			}
			if (d->isPressAnimationFinished) {
				d->hoverGradient->setCenter(mouseEvent->pos());
				d->hoverGradient->setFocalPoint(mouseEvent->pos());
			}
			update();
			break;
		}
		case QEvent::Enter: {
			d->startHoverOpacityAnimation(true);
			break;
		}
		case QEvent::Leave: {
			d->startHoverOpacityAnimation(false);
			break;
		}
		default: {
			break;
		}
	}

	return QWidget::event(event);
}

void CALPromotionCard::paintEvent(QPaintEvent* event) {
	Q_D(CALPromotionCard);

	QPainter painter(this);
	painter.save();
	painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing | QPainter::SmoothPixmapTransform);
	painter.setPen(Qt::NoPen);
	/// shadow
	ALTheme->drawEffectShadow(&painter, rect(), d->shadowBorderWidth, d->borderRadius);
	const QRect foregroundRect(d->shadowBorderWidth, d->shadowBorderWidth, width() - 2 * d->shadowBorderWidth, height() - 2 * d->shadowBorderWidth);
	QPainterPath path;
	path.addRoundedRect(foregroundRect, d->borderRadius, d->borderRadius);
	painter.setClipPath(path);
	/// background image
	if (!d->cardPixmap.isNull()) {
		// 源区域计算
		QRect pixSourceRect = d->cardPixmap.rect();
		const qreal horizontalOffset = d->cardPixmap.width() * (1 - d->horizontalCardPixmapRatio) / 2;
		const qreal verticalOffset = d->cardPixmap.height() * (1 - d->verticalCardPixmapRatio) / 2;
		pixSourceRect.adjust(horizontalOffset, verticalOffset, -horizontalOffset, -verticalOffset); // NOLINT
		painter.drawPixmap(foregroundRect, d->cardPixmap, pixSourceRect);
	}
	/// text
	painter.save();
	QFont font = painter.font();
	// card title
	font.setWeight(QFont::Bold);
	font.setPixelSize(d->cardTitlePixelSize);
	painter.setFont(font);
	painter.setPen(d->cardTitleColor);
	painter.drawText(QRect(25, 25, foregroundRect.width() - 25, foregroundRect.height()), Qt::AlignLeft | Qt::AlignTop | Qt::TextSingleLine, d->cardTitle);
	// title
	font.setWeight(QFont::Bold);
	font.setPixelSize(d->titlePixelSize);
	painter.setFont(font);
	painter.setPen(d->titleColor);
	const int titleTextHeight = painter.fontMetrics().height();
	const QRect titleRect(25, (height() - titleTextHeight) / 2, foregroundRect.width() - 25, titleTextHeight);
	painter.drawText(titleRect, Qt::AlignLeft | Qt::AlignTop | Qt::TextSingleLine, d->title);
	// promotion title
	if (!d->promotionTitle.isEmpty()) {
		font.setWeight(QFont::Normal);
		font.setPixelSize(d->promotionTitlePixelSize);
		painter.setFont(font);
		const int promotionTitleTextWidth = painter.fontMetrics().horizontalAdvance(d->promotionTitle);
		const int promotionTitleTextHeight = painter.fontMetrics().height();
		const QRect promotinoTitleTextRect(32, titleRect.top() - promotionTitleTextHeight - 5, foregroundRect.width() / 2 - 25, promotionTitleTextHeight);
		// background
		painter.setPen(Qt::NoPen);
		painter.setBrush(d->promotionTitleBaseColor);
		painter.drawRoundedRect(QRect(25, promotinoTitleTextRect.top() - 2, promotionTitleTextWidth + 14, promotionTitleTextHeight + 4), 8, 8);
		// text
		painter.setPen(d->promotionTitleColor);
		painter.drawText(promotinoTitleTextRect, Qt::AlignLeft | Qt::AlignTop | Qt::TextSingleLine, d->promotionTitle);
	}
	// sub title
	font.setWeight(QFont::Medium);
	font.setPixelSize(d->subTitlePixelSize);
	painter.setFont(font);
	painter.setPen(d->subTitleColor);
	painter.drawText(QRect(25, titleRect.bottom(), foregroundRect.width() / 2 - 25, height() / 2), Qt::AlignLeft | Qt::AlignTop | Qt::TextSingleLine, d->subTitle);
	painter.restore();
	/// shadow
	if (d->isPressAnimationFinished) { // hover shadow
		painter.setOpacity(d->hoverOpacity);
		painter.setBrush(*d->hoverGradient);
		painter.drawEllipse(d->hoverGradient->center(), d->hoverGradient->radius(), d->hoverGradient->radius());
	} else { // press shadow
		painter.setOpacity(d->pressOpacity);
		painter.setBrush(*d->pressGradient);
		painter.drawEllipse(d->hoverGradient->center(), d->pressRadius, d->pressRadius / 1.1);
	}
	painter.restore();
}
}
