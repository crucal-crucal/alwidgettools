#include "alimagecard.hpp"

#include <QPainter>
#include <QPainterPath>

#include "alimagecard_p.hpp"
#include "althememanager.hpp"

/**
 * @brief \namespace AL
 */
namespace AL {
/**
 * \class CALImageCardPrivate
 * Internal class for CALImageCard
 * @param q pointer to the public class
 * @param parent pointer to the parent class
 */
CALImageCardPrivate::CALImageCardPrivate(CALImageCard* q, QObject* parent): QObject(parent), q_ptr(q) {
}

CALImageCardPrivate::~CALImageCardPrivate() = default;

/**
 * \class CALImageCard
 * @param parent pointer to the parent class
 */
CALImageCard::CALImageCard(QWidget* parent): QWidget(parent), d_ptr(new CALImageCardPrivate(this, this)) {
	Q_D(CALImageCard);

	d->borderRadius = 6;
	d->maximumAspectRatio = 2.2;
	d->isPreserveAspectCrop = true;
	d->themeMode = ALTheme->getThemeMode();
	connect(ALTheme, &CALThemeManager::sigThemeModeChanged, this, [d](const ALThemeType::ThemeMode& mode) { d->themeMode = mode; });
	setMinimumSize(350, 260);
}

CALImageCard::~CALImageCard() = default;

void CALImageCard::setIsPreserveAspectCrop(const bool isPreserveAspectCrop) {
	d_func()->isPreserveAspectCrop = isPreserveAspectCrop;
	Q_EMIT sigIsPreserveAspectCropChanged();
}

bool CALImageCard::getIsPreserveAspectCrop() const {
	return d_func()->isPreserveAspectCrop;
}

void CALImageCard::setBorderRadius(const qreal borderRadius) {
	d_func()->borderRadius = borderRadius;
	Q_EMIT sigBorderRadiusChanged();
}

qreal CALImageCard::getBorderRadius() const {
	return d_func()->borderRadius;
}

void CALImageCard::setMaximumAspectRatio(const qreal maximumAspectRatio) {
	d_func()->maximumAspectRatio = maximumAspectRatio;
	Q_EMIT sigMaximumAspectRatioChanged();
}

qreal CALImageCard::getMaximumAspectRatio() const {
	return d_func()->maximumAspectRatio;
}

void CALImageCard::setCardImage(const QImage& cardImage) {
	d_func()->cardImage = cardImage;
	Q_EMIT sigCardImageChanged();
}

QImage CALImageCard::getCardImage() const {
	return d_func()->cardImage;
}

void CALImageCard::paintEvent(QPaintEvent* event) {
	Q_D(CALImageCard);

	QPainter painter(this);
	painter.save();
	painter.setRenderHints(QPainter::SmoothPixmapTransform | QPainter::Antialiasing);
	QPainterPath path;
	path.addRoundedRect(rect(), d->borderRadius, d->borderRadius);
	painter.setClipPath(path);
	// image
	if (d->isPreserveAspectCrop) {
		if (qreal itemAspectRatio = rect().width() * 1.0 / rect().height(); itemAspectRatio < d->maximumAspectRatio) {
			itemAspectRatio = d->maximumAspectRatio;
			const qreal cropHeight = d->cardImage.width() / itemAspectRatio;
			painter.drawImage(QRectF(0.0, 0.0, this->height() * d->maximumAspectRatio, rect().height()), d->cardImage, QRectF(0, 0, d->cardImage.width(), cropHeight));
		} else {
			const qreal cripHeight = d->cardImage.width() / itemAspectRatio;
			painter.drawImage(rect(), d->cardImage, QRectF(0, 0, d->cardImage.width(), cripHeight));
		}
	} else {
		painter.drawImage(rect(), d->cardImage);
	}
	painter.restore();
}
}
