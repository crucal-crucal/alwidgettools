#include "uvinteractivecard.hpp"

#include <QPainter>
#include <QPainterPath>

#include "uvinteractivecard_p.hpp"
#include "uvthememanager.hpp"

/**
 * @brief \class CUVInteractiveCardPrivate
 * Internal class for CUVInteractiveCard
 * @param q pointer to the public class
 * @param parent pointer to the parent class
 */
CUVInteractiveCardPrivate::CUVInteractiveCardPrivate(CUVInteractiveCard* q, QObject* parent): QObject(parent), q_ptr(q) {
}

CUVInteractiveCardPrivate::~CUVInteractiveCardPrivate() = default;

/**
 * @brief \class CUVInteractiveCard
 * @param parent pointer to the parent class
 */
CUVInteractiveCard::CUVInteractiveCard(QWidget* parent): QPushButton(parent), d_ptr(new CUVInteractiveCardPrivate(this, this)) {
	Q_D(CUVInteractiveCard);

	d->borderRadius = 6;
	d->titlePixelSize = 15;
	d->subTitlePixelSize = 12;
	d->cardPixmapSize = { 64, 64 };
	d->titleSpacing = 2;
	d->cardPixmapBorderRadius = 6;
	d->cardPixMode = UVCardPixType::Ellipse;
	setMinimumSize(270, 80);
	setMouseTracking(true);
	d->themeMode = UVTheme->getThemeMode();
	connect(UVTheme, &CUVThemeManager::sigThemeModeChanged, this, [=](const UVThemeType::ThemeMode& mode) { d->themeMode = mode; });
}

CUVInteractiveCard::~CUVInteractiveCard() = default;

void CUVInteractiveCard::setCardPixmapSize(int width, int height) {
	d_func()->cardPixmapSize = { width, height };
	Q_EMIT sigCardPixmapSizeChanged();
}

void CUVInteractiveCard::setCardPixMode(const UVCardPixType::PixMode& mode) {
	d_func()->cardPixMode = mode;
	Q_EMIT sigCardPixModeChanged();
}

UVCardPixType::PixMode CUVInteractiveCard::getCardPixMode() const {
	return d_func()->cardPixMode;
}

void CUVInteractiveCard::setBorderRadius(const qreal radius) {
	d_func()->borderRadius = radius;
	Q_EMIT sigBorderRadiusChanged();
}

qreal CUVInteractiveCard::getBorderRadius() const {
	return d_func()->borderRadius;
}

void CUVInteractiveCard::setCardPixmapBorderRadius(const qreal radius) {
	d_func()->cardPixmapBorderRadius = radius;
	Q_EMIT sigCardPixmapBorderRadiusChanged();
}

qreal CUVInteractiveCard::getCardPixmapBorderRadius() const {
	return d_func()->cardPixmapBorderRadius;
}

void CUVInteractiveCard::setTitlePixelSize(const int pixelSize) {
	d_func()->titlePixelSize = pixelSize;
	Q_EMIT sigTitlePixelSizeChanged();
}

int CUVInteractiveCard::getTitlePixelSize() const {
	return d_func()->titlePixelSize;
}

void CUVInteractiveCard::setSubTitlePixelSize(const int pixelSize) {
	d_func()->subTitlePixelSize = pixelSize;
	Q_EMIT sigSubTitlePixelSizeChanged();
}

int CUVInteractiveCard::getSubTitlePixelSize() const {
	return d_func()->subTitlePixelSize;
}

void CUVInteractiveCard::setTitleSpacing(const int spacing) {
	d_func()->titleSpacing = spacing;
	Q_EMIT sigTitleSpacingChanged();
}

int CUVInteractiveCard::getTitleSpacing() const {
	return d_func()->titleSpacing;
}

void CUVInteractiveCard::setTitle(const QString& title) {
	d_func()->title = title;
	Q_EMIT sigTitleChanged();
}

QString CUVInteractiveCard::getTitle() const {
	return d_func()->title;
}

void CUVInteractiveCard::setSubTitle(const QString& subTitle) {
	d_func()->subTitle = subTitle;
	Q_EMIT sigSubTitleChanged();
}

QString CUVInteractiveCard::getSubTitle() const {
	return d_func()->subTitle;
}

void CUVInteractiveCard::setCardPixmap(const QPixmap& pixmap) {
	d_func()->cardPixmap = pixmap;
	Q_EMIT sigCardPixmapChanged();
}

QPixmap CUVInteractiveCard::getCardPixmap() const {
	return d_func()->cardPixmap;
}

void CUVInteractiveCard::setCardPixmapSize(const QSize& size) {
	d_func()->cardPixmapSize = size;
	Q_EMIT sigCardPixmapSizeChanged();
}

QSize CUVInteractiveCard::getCardPixmapSize() const {
	return d_func()->cardPixmapSize;
}

void CUVInteractiveCard::paintEvent(QPaintEvent* event) {
	Q_D(CUVInteractiveCard);

	QPainter painter(this);
	painter.save();
	painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform | QPainter::TextAntialiasing);
	painter.setPen(Qt::NoPen);
	painter.setBrush(underMouse() ? UVThemeColor(d->themeMode, UVThemeType::BasicHoverAlpha) : Qt::transparent);
	painter.drawRoundedRect(rect(), d->borderRadius, d->borderRadius);

	/// 图片
	if (!d->cardPixmap.isNull()) {
		QPainterPath path;
		if (d->cardPixMode == UVCardPixType::Ellipse) {
			path.addEllipse(QPointF(d->cardPixmapSize.width() / 2.0 + 10, height() / 2.0), d->cardPixmapSize.width() / 2.0, d->cardPixmapSize.height() / 2.0);
			painter.setClipPath(path);
		} else if (d->cardPixMode == UVCardPixType::RoundedRect) {
			path.addRoundedRect(10, (height() - d->cardPixmapSize.height()) / 2.0, d->cardPixmapSize.width(), d->cardPixmapSize.height(), d->cardPixmapBorderRadius, d->cardPixmapBorderRadius);
		}
		painter.drawPixmap(10, (height() - d->cardPixmapSize.height()) / 2, d->cardPixmapSize.width(), d->cardPixmapSize.height(), d->cardPixmap);
	}

	/// 文字
	painter.setPen(UVThemeColor(d->themeMode, UVThemeType::BasicText));
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
