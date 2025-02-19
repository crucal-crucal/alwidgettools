#include "alinteractivecard.hpp"

#include <QPainter>
#include <QPainterPath>

#include "alinteractivecard_p.hpp"
#include "althememanager.hpp"

/**
 * @brief \namespace AL
 */
namespace AL {
/**
 * @brief \class CALInteractiveCardPrivate
 * Internal class for CALInteractiveCard
 * @param q pointer to the public class
 * @param parent pointer to the parent class
 */
CALInteractiveCardPrivate::CALInteractiveCardPrivate(CALInteractiveCard* q, QObject* parent): QObject(parent), q_ptr(q) {
}

CALInteractiveCardPrivate::~CALInteractiveCardPrivate() = default;

/**
 * @brief \class CALInteractiveCard
 * @param parent pointer to the parent class
 */
CALInteractiveCard::CALInteractiveCard(QWidget* parent): QPushButton(parent), d_ptr(new CALInteractiveCardPrivate(this, this)) {
	Q_D(CALInteractiveCard);

	d->borderRadius = 6;
	d->titlePixelSize = 15;
	d->subTitlePixelSize = 12;
	d->cardPixmapSize = { 64, 64 };
	d->titleSpacing = 2;
	d->cardPixmapBorderRadius = 6;
	d->cardPixMode = ALCardPixType::Ellipse;
	setMinimumSize(270, 80);
	setMouseTracking(true);
	d->themeMode = ALTheme->getThemeMode();
	connect(ALTheme, &CALThemeManager::sigThemeModeChanged, this, [d](const ALThemeType::ThemeMode& mode) { d->themeMode = mode; });
}

CALInteractiveCard::~CALInteractiveCard() = default;

void CALInteractiveCard::setCardPixmapSize(int width, int height) {
	d_func()->cardPixmapSize = { width, height };
	Q_EMIT sigCardPixmapSizeChanged();
}

void CALInteractiveCard::setCardPixMode(const ALCardPixType::PixMode& mode) {
	d_func()->cardPixMode = mode;
	Q_EMIT sigCardPixModeChanged();
}

ALCardPixType::PixMode CALInteractiveCard::getCardPixMode() const {
	return d_func()->cardPixMode;
}

void CALInteractiveCard::setBorderRadius(const qreal radius) {
	d_func()->borderRadius = radius;
	Q_EMIT sigBorderRadiusChanged();
}

qreal CALInteractiveCard::getBorderRadius() const {
	return d_func()->borderRadius;
}

void CALInteractiveCard::setCardPixmapBorderRadius(const qreal radius) {
	d_func()->cardPixmapBorderRadius = radius;
	Q_EMIT sigCardPixmapBorderRadiusChanged();
}

qreal CALInteractiveCard::getCardPixmapBorderRadius() const {
	return d_func()->cardPixmapBorderRadius;
}

void CALInteractiveCard::setTitlePixelSize(const int pixelSize) {
	d_func()->titlePixelSize = pixelSize;
	Q_EMIT sigTitlePixelSizeChanged();
}

int CALInteractiveCard::getTitlePixelSize() const {
	return d_func()->titlePixelSize;
}

void CALInteractiveCard::setSubTitlePixelSize(const int pixelSize) {
	d_func()->subTitlePixelSize = pixelSize;
	Q_EMIT sigSubTitlePixelSizeChanged();
}

int CALInteractiveCard::getSubTitlePixelSize() const {
	return d_func()->subTitlePixelSize;
}

void CALInteractiveCard::setTitleSpacing(const int spacing) {
	d_func()->titleSpacing = spacing;
	Q_EMIT sigTitleSpacingChanged();
}

int CALInteractiveCard::getTitleSpacing() const {
	return d_func()->titleSpacing;
}

void CALInteractiveCard::setTitle(const QString& title) {
	d_func()->title = title;
	Q_EMIT sigTitleChanged();
}

QString CALInteractiveCard::getTitle() const {
	return d_func()->title;
}

void CALInteractiveCard::setSubTitle(const QString& subTitle) {
	d_func()->subTitle = subTitle;
	Q_EMIT sigSubTitleChanged();
}

QString CALInteractiveCard::getSubTitle() const {
	return d_func()->subTitle;
}

void CALInteractiveCard::setCardPixmap(const QPixmap& pixmap) {
	d_func()->cardPixmap = pixmap;
	Q_EMIT sigCardPixmapChanged();
}

QPixmap CALInteractiveCard::getCardPixmap() const {
	return d_func()->cardPixmap;
}

void CALInteractiveCard::setCardPixmapSize(const QSize& size) {
	d_func()->cardPixmapSize = size;
	Q_EMIT sigCardPixmapSizeChanged();
}

QSize CALInteractiveCard::getCardPixmapSize() const {
	return d_func()->cardPixmapSize;
}

void CALInteractiveCard::paintEvent(QPaintEvent* event) {
	Q_D(CALInteractiveCard);

	QPainter painter(this);
	painter.save();
	painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform | QPainter::TextAntialiasing);
	painter.setPen(Qt::NoPen);
	painter.setBrush(underMouse() ? ALThemeColor(d->themeMode, ALThemeType::BasicHoverAlpha) : Qt::transparent);
	painter.drawRoundedRect(rect(), d->borderRadius, d->borderRadius);

	/// 图片
	if (!d->cardPixmap.isNull()) {
		painter.save();
		QPainterPath path;
		if (d->cardPixMode == ALCardPixType::Ellipse) {
			path.addEllipse(QPointF(d->cardPixmapSize.width() / 2.0 + 10, height() / 2.0), d->cardPixmapSize.width() / 2.0, d->cardPixmapSize.height() / 2.0);
			painter.setClipPath(path);
		} else if (d->cardPixMode == ALCardPixType::RoundedRect) {
			path.addRoundedRect(QRectF(10, (height() - d->cardPixmapSize.height()) / 2.0, d->cardPixmapSize.width(), d->cardPixmapSize.height()), d->cardPixmapBorderRadius, d->cardPixmapBorderRadius);
			painter.setClipPath(path);
		}
		painter.drawPixmap(10, (height() - d->cardPixmapSize.height()) / 2, d->cardPixmapSize.width(), d->cardPixmapSize.height(), d->cardPixmap);
		painter.restore();
	}

	/// 文字
	painter.setPen(ALThemeColor(d->themeMode, ALThemeType::BasicText));
	QFont font = this->font();
	font.setWeight(QFont::Bold);
	font.setPixelSize(d->titlePixelSize);
	painter.setFont(font);
	const int textStartX = d->cardPixmapSize.width() + 26;
	const int textWidth = width() - textStartX;
	painter.drawText(textStartX, rect().y(), textWidth, height() / 2 - d->titleSpacing, Qt::AlignBottom | Qt::AlignLeft, d->title);
	font.setWeight(QFont::Normal);
	font.setPixelSize(d->subTitlePixelSize);
	painter.setFont(font);
	painter.drawText(textStartX, height() / 2 + d->titleSpacing, textWidth, height() / 2 - d->titleSpacing, Qt::AlignTop | Qt::AlignLeft, d->subTitle);
	painter.restore();
}
}
