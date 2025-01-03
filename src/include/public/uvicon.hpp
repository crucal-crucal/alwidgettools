#pragma once

#include <QIcon>

#include "uvwidgettoolsdef.hpp"

class CUVWIDGETTOOLS_EXPORT CUVIcon {
public:
	static QIcon getQIconFromAwesomeIcon(const UVIcon::CUVAweSomeIcon& awesomeIcon);
	static QIcon getQIconFromAwesomeIcon(const UVIcon::CUVAweSomeIcon& awesomeIcon, const QColor& iconColor);
	static QIcon getQIconFromAwesomeIcon(const UVIcon::CUVAweSomeIcon& awesomeIcon, int pixelSize);
	static QIcon getQIconFromAwesomeIcon(const UVIcon::CUVAweSomeIcon& awesomeIcon, int pixelSize, const QColor& iconColor);
	static QIcon getQIconFromAwesomeIcon(const UVIcon::CUVAweSomeIcon& awesomeIcon, int pixelSize, int fixedWidth, int fixedHeight);
	static QIcon getQIconFromAwesomeIcon(const UVIcon::CUVAweSomeIcon& awesomeIcon, int pixelSize, int fixedWidth, int fixedHeight, const QColor& iconColor);

private:
	explicit CUVIcon();
	~CUVIcon();
};
