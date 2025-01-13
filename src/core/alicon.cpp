#include "alicon.hpp"

#include <QPainter>
#include <QPixmap>

namespace AL {
/**
 * @brief \class CALIcon
 * @return CALIcon instance
 */
QIcon createIcon(const ALIcon::AweSomeIcon& awesomeIcon, const int pixelSize = 25, const int fixedWidth = 30, const int fixedHeight = 30, const QColor& iconColor = Qt::black) {
	QFont iconFont("CALAwesome");
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

QIcon CALIcon::getQIconFromAwesomeIcon(const ALIcon::AweSomeIcon& awesomeIcon) { // NOLINT
	return createIcon(awesomeIcon);
}

QIcon CALIcon::getQIconFromAwesomeIcon(const ALIcon::AweSomeIcon& awesomeIcon, const QColor& iconColor) { // NOLINT
	return createIcon(awesomeIcon, 25, 30, 30, iconColor);
}

QIcon CALIcon::getQIconFromAwesomeIcon(const ALIcon::AweSomeIcon& awesomeIcon, const int pixelSize) { // NOLINT
	return createIcon(awesomeIcon, pixelSize);
}

QIcon CALIcon::getQIconFromAwesomeIcon(const ALIcon::AweSomeIcon& awesomeIcon, const int pixelSize, const QColor& iconColor) { // NOLINT
	return createIcon(awesomeIcon, pixelSize, 30, 30, iconColor);
}

QIcon CALIcon::getQIconFromAwesomeIcon(const ALIcon::AweSomeIcon& awesomeIcon, const int pixelSize, const int fixedWidth, const int fixedHeight) { // NOLINT
	return createIcon(awesomeIcon, pixelSize, fixedWidth, fixedHeight);
}

QIcon CALIcon::getQIconFromAwesomeIcon(const ALIcon::AweSomeIcon& awesomeIcon, const int pixelSize, const int fixedWidth, const int fixedHeight, const QColor& iconColor) { // NOLINT
	return createIcon(awesomeIcon, pixelSize, fixedWidth, fixedHeight, iconColor);
}

CALIcon::CALIcon() = default;

CALIcon::~CALIcon() = default;} // namespace AL