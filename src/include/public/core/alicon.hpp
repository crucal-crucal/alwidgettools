#pragma once

#include <QIcon>

#include "alwidgettoolsdef.hpp"

class CALWIDGETTOOLS_EXPORT CALIcon {
public:
	static QIcon getQIconFromAwesomeIcon(const ALIcon::AweSomeIcon& awesomeIcon);
	static QIcon getQIconFromAwesomeIcon(const ALIcon::AweSomeIcon& awesomeIcon, const QColor& iconColor);
	static QIcon getQIconFromAwesomeIcon(const ALIcon::AweSomeIcon& awesomeIcon, int pixelSize);
	static QIcon getQIconFromAwesomeIcon(const ALIcon::AweSomeIcon& awesomeIcon, int pixelSize, const QColor& iconColor);
	static QIcon getQIconFromAwesomeIcon(const ALIcon::AweSomeIcon& awesomeIcon, int pixelSize, int fixedWidth, int fixedHeight);
	static QIcon getQIconFromAwesomeIcon(const ALIcon::AweSomeIcon& awesomeIcon, int pixelSize, int fixedWidth, int fixedHeight, const QColor& iconColor);

private:
	explicit CALIcon();
	~CALIcon();
};
