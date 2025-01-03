#include "uvicon.hpp"

#include <QPainter>
#include <QPixmap>

/**
 * @brief \class CUVIcon
 * @return CUVIcon instance
 */
QIcon createIcon(const UVIcon::CUVAweSomeIcon& awesomeIcon, const int pixelSize = 25, const int fixedWidth = 30, const int fixedHeight = 30, const QColor& iconColor = Qt::black) {
	QFont iconFont("CUVAwesome");
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

QIcon CUVIcon::getQIconFromAwesomeIcon(const UVIcon::CUVAweSomeIcon& awesomeIcon) { // NOLINT
	return createIcon(awesomeIcon);
}

QIcon CUVIcon::getQIconFromAwesomeIcon(const UVIcon::CUVAweSomeIcon& awesomeIcon, const QColor& iconColor) { // NOLINT
	return createIcon(awesomeIcon, 25, 30, 30, iconColor);
}

QIcon CUVIcon::getQIconFromAwesomeIcon(const UVIcon::CUVAweSomeIcon& awesomeIcon, const int pixelSize) { // NOLINT
	return createIcon(awesomeIcon, pixelSize);
}

QIcon CUVIcon::getQIconFromAwesomeIcon(const UVIcon::CUVAweSomeIcon& awesomeIcon, const int pixelSize, const QColor& iconColor) { // NOLINT
	return createIcon(awesomeIcon, pixelSize, 30, 30, iconColor);
}

QIcon CUVIcon::getQIconFromAwesomeIcon(const UVIcon::CUVAweSomeIcon& awesomeIcon, const int pixelSize, const int fixedWidth, const int fixedHeight) { // NOLINT
	return createIcon(awesomeIcon, pixelSize, fixedWidth, fixedHeight);
}

QIcon CUVIcon::getQIconFromAwesomeIcon(const UVIcon::CUVAweSomeIcon& awesomeIcon, const int pixelSize, const int fixedWidth, const int fixedHeight, const QColor& iconColor) { // NOLINT
	return createIcon(awesomeIcon, pixelSize, fixedWidth, fixedHeight, iconColor);
}

CUVIcon::CUVIcon() = default;

CUVIcon::~CUVIcon() = default;
