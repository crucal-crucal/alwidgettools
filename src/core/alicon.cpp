#include "alicon.hpp"

#include <QPainter>
#include <QPixmap>
#include <QDebug>

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

QIcon CALIcon::getQIconFromAwesomeIcon(const ALIcon::AweSomeIcon& awesomeIcon) {
	return createIcon(awesomeIcon);
}

QIcon CALIcon::getQIconFromFluentIcon(const ALIcon::FluentIcon& fluentIcon) {
	return createIcon(fluentIcon);
}

QIcon CALIcon::getQIconFromAwesomeIcon(const ALIcon::AweSomeIcon& awesomeIcon, const QColor& iconColor) {
	return createIcon(awesomeIcon, 25, 30, 30, iconColor);
}

QIcon CALIcon::getQIconFromFluentIcon(const ALIcon::FluentIcon& fluentIcon, const QColor& iconColor) {
	return createIcon(fluentIcon, 25, 30, 30, iconColor);
}

QIcon CALIcon::getQIconFromAwesomeIcon(const ALIcon::AweSomeIcon& awesomeIcon, const int pixelSize) {
	return createIcon(awesomeIcon, pixelSize);
}

QIcon CALIcon::getQIconFromFluentIcon(const ALIcon::FluentIcon& fluentIcon, const int pixelSize) {
	return createIcon(fluentIcon, pixelSize);
}

QIcon CALIcon::getQIconFromAwesomeIcon(const ALIcon::AweSomeIcon& awesomeIcon, const int pixelSize, const QColor& iconColor) {
	return createIcon(awesomeIcon, pixelSize, 30, 30, iconColor);
}

QIcon CALIcon::getQIconFromFluentIcon(const ALIcon::FluentIcon& fluentIcon, const int pixelSize, const QColor& iconColor) {
	return createIcon(fluentIcon, pixelSize, 30, 30, iconColor);
}

QIcon CALIcon::getQIconFromAwesomeIcon(const ALIcon::AweSomeIcon& awesomeIcon, const int pixelSize, const int fixedWidth, const int fixedHeight) {
	return createIcon(awesomeIcon, pixelSize, fixedWidth, fixedHeight);
}

QIcon CALIcon::getQIconFromFluentIcon(const ALIcon::FluentIcon& fluentIcon, const int pixelSize, const int fixedWidth, const int fixedHeight) {
	return createIcon(fluentIcon, pixelSize, fixedWidth, fixedHeight);
}

QIcon CALIcon::getQIconFromAwesomeIcon(const ALIcon::AweSomeIcon& awesomeIcon, const int pixelSize, const int fixedWidth, const int fixedHeight, const QColor& iconColor) {
	return createIcon(awesomeIcon, pixelSize, fixedWidth, fixedHeight, iconColor);
}

QIcon CALIcon::getQIconFromFluentIcon(const ALIcon::FluentIcon& fluentIcon, const int pixelSize, const int fixedWidth, const int fixedHeight, const QColor& iconColor) {
	return createIcon(fluentIcon, pixelSize, fixedWidth, fixedHeight, iconColor);
}

CALIcon::CALIcon() = default;

CALIcon::~CALIcon() = default;
}
