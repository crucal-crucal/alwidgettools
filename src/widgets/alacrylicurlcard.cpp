#include "alacrylicurlcard.hpp"

#include <QDesktopServices>
#include <QPainter>
#include <QPainterPath>
#include <QUrl>

#include "alacrylicurlcard_p.hpp"
#include "althememanager.hpp"

/**
 * @brief \namespace AL
 */
namespace AL {
/**
 * @brief \class CALAcrylicUrlCardPrivate
 * Internal class for CALAcrylicUrlCard
 * @param q pointer to the public class
 * @param parent pointer to the parent class
 */
CALAcrylicUrlCardPrivate::CALAcrylicUrlCardPrivate(CALAcrylicUrlCard* q, QObject* parent): QObject(parent), q_ptr(q) {
}

CALAcrylicUrlCardPrivate::~CALAcrylicUrlCardPrivate() = default;

void CALAcrylicUrlCardPrivate::slotArcylicUrlCardClicked() {
	Q_Q(CALAcrylicUrlCard);

	const QUrl openUrl(url);
	if (!openUrl.isValid()) {
		Q_EMIT q->sigOpenError(tr("URL is invalid"));
		return;
	}

	if (!QDesktopServices::openUrl(openUrl)) {
		Q_EMIT q->sigOpenError(tr("Failed to open URL") + QString("url: %1").arg(openUrl.toString()));
	}
}

/**
 * @brief \class CALAcrylicUrlCard
 * @param parent pointer to the parent class
 */
CALAcrylicUrlCard::CALAcrylicUrlCard(QWidget* parent): QPushButton(parent), d_ptr(new CALAcrylicUrlCardPrivate(this, this)) {
	Q_D(CALAcrylicUrlCard);

	d->borderWidth = 1.5;
	d->borderRadius = 5.0;
	d->mainOpacity = 0.95;
	d->noiseOpacity = 0.06;
	d->brushAlpha = 245;
	d->titlePixelSize = 15;
	d->subTitlePixelSize = 12;
	d->titleSpacing = 10;
	d->subTitleSpacing = 10;
	d->cardPixmapBorderRadius = 6.0;
	d->cardPixmapSize = { 54, 54 };
	d->cardPixMode = ALCardPixType::Ellipse;
	d->noisePix = QPixmap(":alwidgettools/CALImage-noise");
	d->themeMode = ALTheme->getThemeMode();
	connect(ALTheme, &CALThemeManager::sigThemeModeChanged, this, [d](const ALThemeType::ThemeMode& mode) { d->themeMode = mode; });

	setFixedSize(180, 200);
	connect(this, &CALAcrylicUrlCard::clicked, d, &CALAcrylicUrlCardPrivate::slotArcylicUrlCardClicked);
}

CALAcrylicUrlCard::~CALAcrylicUrlCard() = default;

void CALAcrylicUrlCard::setBrushAlpha(const int alpha) {
	d_func()->brushAlpha = alpha;
	Q_EMIT sigBrushAlphaChanged();
}

int CALAcrylicUrlCard::getBrushAlpha() const {
	return d_func()->brushAlpha;
}

void CALAcrylicUrlCard::setTitlePixelSize(const int size) {
	d_func()->titlePixelSize = size;
	Q_EMIT sigTitlePixelSizeChanged();
}

int CALAcrylicUrlCard::getTitlePixelSize() const {
	return d_func()->titlePixelSize;
}

void CALAcrylicUrlCard::setSubTitlePixelSize(const int size) {
	d_func()->subTitlePixelSize = size;
	Q_EMIT sigSubTitlePixelSizeChanged();
}

int CALAcrylicUrlCard::getSubTitlePixelSize() const {
	return d_func()->subTitlePixelSize;
}

void CALAcrylicUrlCard::setTitleSpacing(const int spacing) {
	d_func()->titleSpacing = spacing;
	Q_EMIT sigTitleSpacingChanged();
}

int CALAcrylicUrlCard::getTitleSpacing() const {
	return d_func()->titleSpacing;
}

void CALAcrylicUrlCard::setSubTitleSpacing(const int spacing) {
	d_func()->subTitleSpacing = spacing;
	Q_EMIT sigSubTitleSpacingChanged();
}

int CALAcrylicUrlCard::getSubTitleSpacing() const {
	return d_func()->subTitleSpacing;
}

void CALAcrylicUrlCard::setBorderRadius(const qreal radius) {
	d_func()->borderRadius = radius;
	Q_EMIT sigBorderRadiusChanged();
}

qreal CALAcrylicUrlCard::getBorderRadius() const {
	return d_func()->borderRadius;
}

void CALAcrylicUrlCard::setMainOpacity(const qreal opacity) {
	d_func()->mainOpacity = opacity;
	Q_EMIT sigMainOpacityChanged();
}

qreal CALAcrylicUrlCard::getMainOpacity() const {
	return d_func()->mainOpacity;
}

void CALAcrylicUrlCard::setNoiseOpacity(const qreal opacity) {
	d_func()->noiseOpacity = opacity;
	Q_EMIT sigNoiseOpacityChanged();
}

qreal CALAcrylicUrlCard::getNoiseOpacity() const {
	return d_func()->noiseOpacity;
}

void CALAcrylicUrlCard::setBorderWidth(const qreal width) {
	d_func()->borderWidth = width;
	Q_EMIT sigBorderWidthChanged();
}

qreal CALAcrylicUrlCard::getBorderWidth() const {
	return d_func()->borderWidth;
}

void CALAcrylicUrlCard::setCardPixmapBorderRadius(const qreal radius) {
	d_func()->cardPixmapBorderRadius = radius;
	Q_EMIT sigCardPixmapBorderRadiusChanged();
}

qreal CALAcrylicUrlCard::getCardPixmapBorderRadius() const {
	return d_func()->cardPixmapBorderRadius;
}

void CALAcrylicUrlCard::setTitle(const QString& title) {
	d_func()->title = title;
	Q_EMIT sigTitleChanged();
}

QString CALAcrylicUrlCard::getTitle() const {
	return d_func()->title;
}

void CALAcrylicUrlCard::setSubTitle(const QString& subTitle) {
	d_func()->subTitle = subTitle;
	Q_EMIT sigSubTitleChanged();
}

QString CALAcrylicUrlCard::getSubTitle() const {
	return d_func()->subTitle;
}

void CALAcrylicUrlCard::setUrl(const QString& url) {
	d_func()->url = url;
	Q_EMIT sigUrlChanged();
}

QString CALAcrylicUrlCard::getUrl() const {
	return d_func()->url;
}

void CALAcrylicUrlCard::setCardPixmap(const QPixmap& pixmap) {
	d_func()->cardPixmap = pixmap;
	Q_EMIT sigCardPixmapChanged();
}

QPixmap CALAcrylicUrlCard::getCardPixmap() const {
	return d_func()->cardPixmap;
}

void CALAcrylicUrlCard::setCardPixmapSize(const QSize& size) {
	d_func()->cardPixmapSize = size;
	Q_EMIT sigCardPixmapSizeChanged();
}

void CALAcrylicUrlCard::setCardPixmapSize(const int width, const int height) {
	d_func()->cardPixmapSize = { width, height };
	Q_EMIT sigCardPixmapSizeChanged();
}

QSize CALAcrylicUrlCard::getCardPixmapSize() const {
	return d_func()->cardPixmapSize;
}

void CALAcrylicUrlCard::paintEvent(QPaintEvent* event) {
	Q_D(CALAcrylicUrlCard);

	QPainter painter(this);
	painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing | QPainter::SmoothPixmapTransform);
	// 亚克力绘制
	painter.save();
	painter.setPen(QPen(ALThemeColor(d->themeMode, underMouse() ? ALThemeType::BasicBorderHover : ALThemeType::BasicBorder), d->borderWidth));
	QColor brushColor = ALThemeColor(d->themeMode, underMouse() ? ALThemeType::BasicHover : ALThemeType::BasicBase);
	brushColor.setAlpha(d->brushAlpha);
	painter.setBrush(brushColor);
	QRect foregroundRect = rect();
	foregroundRect.adjust(d->borderWidth, d->borderWidth, -d->borderWidth, -d->borderWidth); // NOLINT
	const int width = foregroundRect.width();
	painter.setOpacity(d->noiseOpacity);
	painter.drawPixmap(foregroundRect, d->noisePix);
	painter.setOpacity(d->mainOpacity);
	painter.drawRoundedRect(foregroundRect, d->borderRadius, d->borderRadius);
	painter.restore();
	// 图片
	const QRectF pixRect(width / 8.5, height() / 4.0 - d->cardPixmapSize.height() / 2.0, d->cardPixmapSize.width(), d->cardPixmapSize.height());
	if (!d->cardPixmap.isNull()) {
		painter.save();
		QPainterPath path;
		switch (d->cardPixMode) {
			case ALCardPixType::Ellipse: {
				path.addEllipse(pixRect.center(), d->cardPixmapSize.width() / 2.0, d->cardPixmapSize.height() / 2.0);
				painter.setClipPath(path);
				break;
			}
			case ALCardPixType::RoundedRect: {
				path.addRoundedRect(pixRect, d->cardPixmapBorderRadius, d->cardPixmapBorderRadius);
				painter.setClipPath(path);
				break;
			}
			default: {
				break;
			}
		}
		painter.drawPixmap(pixRect, d->cardPixmap, d->cardPixmap.rect());
		painter.restore();
	}
	// text
	painter.save();
	QFont font = this->font();
	font.setWeight(QFont::Bold);
	font.setPixelSize(d->titlePixelSize);
	painter.setFont(font);
	painter.setPen(ALThemeColor(d->themeMode, ALThemeType::BasicText));
	painter.drawText(QRectF(pixRect.x(), pixRect.bottom() + d->titleSpacing, width - width / 7.0, height() / 3.0), Qt::AlignLeading | Qt::AlignTop | Qt::TextSingleLine, d->title);
	font.setWeight(QFont::Normal);
	font.setPixelSize(d->subTitlePixelSize);
	painter.setFont(font);
	painter.setPen(ALThemeColor(d->themeMode, ALThemeType::BasicDetailsText));
	const qreal titleHeight = painter.fontMetrics().boundingRect(d->title).height() * 1.1;
	painter.drawText(QRectF(pixRect.x(), pixRect.bottom() + d->titleSpacing + d->subTitleSpacing + titleHeight, width - width / 7.0, height() / 3.0), Qt::AlignLeading | Qt::AlignTop | Qt::TextSingleLine, d->subTitle);
	painter.restore();
	// icon
	painter.save();
	QFont iconFont(ALIcon::getEnumTypeFontName(ALIcon::Awesome));
	iconFont.setPixelSize(13);
	painter.setFont(iconFont);
	painter.setPen(ALThemeColor(d->themeMode, ALThemeType::BasicText));
	painter.drawText(static_cast<int>(width - 1.5 * iconFont.pixelSize()), height() - iconFont.pixelSize(), QChar(static_cast<unsigned short>(ALIcon::AweSomeIcon::UpRightFromSquare)));
	painter.restore();
}
}
