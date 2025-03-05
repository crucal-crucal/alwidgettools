#include "alicon.hpp"

#include <QPainter>
#include <QPixmap>
#include <QDebug>
#include <QMetaEnum>

#include "alicon_p.hpp"

/**
 * @brief \namespace AL
 */
namespace AL {
/**
 * @brief \class CALIcon
 * @return CALIcon instance
 */
QIcon createIcon(const ALIcon::AweSomeIcon& awesomeIcon, const int pixelSize = 25, const int fixedWidth = 30, const int fixedHeight = 30, const QColor& iconColor = Qt::black) {
	QFont iconFont(ALIcon::getEnumTypeFontName(awesomeIcon));
	iconFont.setPixelSize(pixelSize);

	QPixmap pix(fixedWidth, fixedHeight);
	pix.fill(Qt::transparent);

	QPainter painter(&pix);
	painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing | QPainter::SmoothPixmapTransform);
	painter.setFont(iconFont);
	painter.setPen(iconColor);
	painter.drawText(pix.rect(), Qt::AlignCenter, QChar(static_cast<unsigned short>(awesomeIcon)));

	return pix;
}

QIcon createIcon(const ALIcon::FluentIcon& fluentIcon, const int pixelSize = 25, const int fixedWidth = 30, const int fixedHeight = 30, const QColor& iconColor = Qt::black) {
	QFont iconFont(ALIcon::getEnumTypeFontName(fluentIcon));
	iconFont.setPixelSize(pixelSize);

	QPixmap pix(fixedWidth, fixedHeight);
	pix.fill(Qt::transparent);

	QPainter painter(&pix);
	painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing | QPainter::SmoothPixmapTransform);
	painter.setFont(iconFont);
	painter.setPen(iconColor);
	painter.drawText(pix.rect(), Qt::AlignCenter, QChar(static_cast<unsigned short>(fluentIcon)));

	return pix;
}

/**
 * @brief \class CALAwesomeIconTypePrivate
 * Internal class for CALAwesomeIconType
 * @param awesomeicon \see \enum ALIcon::AweSomeIcon
 */
CALAwesomeIconTypePrivate::CALAwesomeIconTypePrivate(const ALIcon::AweSomeIcon& awesomeicon): icon(awesomeicon) {
}

CALAwesomeIconTypePrivate::~CALAwesomeIconTypePrivate() = default;

/**
 * @brief \class CALFluentIconTypePrivate
 * Internal class for CALFluentIconType
 * @param fluenticon \see \enum ALIcon::FluentIcon
 */
CALFluentIconTypePrivate::CALFluentIconTypePrivate(const ALIcon::FluentIcon& fluenticon): icon(fluenticon) {
}

CALFluentIconTypePrivate::~CALFluentIconTypePrivate() = default;

/**
 * \class CALAwesomeIconType
 * @param icon awesome icon
 */
CALAwesomeIconType::CALAwesomeIconType(const ALIcon::AweSomeIcon& icon): CALIconType(), d_ptr(std::make_unique<CALAwesomeIconTypePrivate>(icon)) {
}

CALAwesomeIconType::~CALAwesomeIconType() = default;

QString CALAwesomeIconType::name() const {
	return { QMetaEnum::fromType<ALIcon::AweSomeIcon>().valueToKey(static_cast<int>(d_ptr->icon)) };
}

int CALAwesomeIconType::value() const {
	return static_cast<int>(d_ptr->icon);
}

QString CALAwesomeIconType::familyName() const {
	return ALIcon::awesomeFontFamily;
}

ALIcon::IconType CALAwesomeIconType::iconType() const {
	return ALIcon::IconType::Awesome;
}

bool CALAwesomeIconType::isNull() const {
	return d_ptr->icon == ALIcon::AweSomeIcon::None;
}

/**
 * \class CALFluentIconType
 * @param icon fluent icon
 */
CALFluentIconType::CALFluentIconType(const ALIcon::FluentIcon& icon): CALIconType(), d_ptr(std::make_unique<CALFluentIconTypePrivate>(icon)) {
}

CALFluentIconType::~CALFluentIconType() = default;

QString CALFluentIconType::name() const {
	return { QMetaEnum::fromType<ALIcon::FluentIcon>().valueToKey(static_cast<int>(d_ptr->icon)) };
}

int CALFluentIconType::value() const {
	return static_cast<int>(d_ptr->icon);
}

QString CALFluentIconType::familyName() const {
	return ALIcon::fluentFontFamily;
}

ALIcon::IconType CALFluentIconType::iconType() const {
	return ALIcon::IconType::Fluent;
}

bool CALFluentIconType::isNull() const {
	return d_ptr->icon == ALIcon::FluentIcon::None;
}

QIcon CALIcon::getQIconFromALIcon(const ALIcon::AweSomeIcon& awesomeIcon) {
	return createIcon(awesomeIcon);
}

QIcon CALIcon::getQIconFromALIcon(const ALIcon::FluentIcon& fluentIcon) {
	return createIcon(fluentIcon);
}

QIcon CALIcon::getQIconFromALIcon(const ALIcon::AweSomeIcon& awesomeIcon, const QColor& iconColor) {
	return createIcon(awesomeIcon, 25, 30, 30, iconColor);
}

QIcon CALIcon::getQIconFromALIcon(const ALIcon::FluentIcon& fluentIcon, const QColor& iconColor) {
	return createIcon(fluentIcon, 25, 30, 30, iconColor);
}

QIcon CALIcon::getQIconFromALIcon(const ALIcon::AweSomeIcon& awesomeIcon, const int pixelSize) {
	return createIcon(awesomeIcon, pixelSize);
}

QIcon CALIcon::getQIconFromALIcon(const ALIcon::FluentIcon& fluentIcon, const int pixelSize) {
	return createIcon(fluentIcon, pixelSize);
}

QIcon CALIcon::getQIconFromALIcon(const ALIcon::AweSomeIcon& awesomeIcon, const int pixelSize, const QColor& iconColor) {
	return createIcon(awesomeIcon, pixelSize, 30, 30, iconColor);
}

QIcon CALIcon::getQIconFromALIcon(const ALIcon::FluentIcon& fluentIcon, const int pixelSize, const QColor& iconColor) {
	return createIcon(fluentIcon, pixelSize, 30, 30, iconColor);
}

QIcon CALIcon::getQIconFromALIcon(const ALIcon::AweSomeIcon& awesomeIcon, const int pixelSize, const int fixedWidth, const int fixedHeight) {
	return createIcon(awesomeIcon, pixelSize, fixedWidth, fixedHeight);
}

QIcon CALIcon::getQIconFromALIcon(const ALIcon::FluentIcon& fluentIcon, const int pixelSize, const int fixedWidth, const int fixedHeight) {
	return createIcon(fluentIcon, pixelSize, fixedWidth, fixedHeight);
}

QIcon CALIcon::getQIconFromALIcon(const ALIcon::AweSomeIcon& awesomeIcon, const int pixelSize, const int fixedWidth, const int fixedHeight, const QColor& iconColor) {
	return createIcon(awesomeIcon, pixelSize, fixedWidth, fixedHeight, iconColor);
}

QIcon CALIcon::getQIconFromALIcon(const ALIcon::FluentIcon& fluentIcon, const int pixelSize, const int fixedWidth, const int fixedHeight, const QColor& iconColor) {
	return createIcon(fluentIcon, pixelSize, fixedWidth, fixedHeight, iconColor);
}

CALIcon::CALIcon() = default;

CALIcon::~CALIcon() = default;
}
