#include "uvsuggestboxsearchviewcontainer.hpp"

#include <QPainter>

#include "althememanager.hpp"

/**
 * @brief \class CUVSuggestBoxSearchViewContainer
 * @param parent pointer to the parent class
 */
CUVSuggestBoxSearchViewContainer::CUVSuggestBoxSearchViewContainer(QWidget* parent): QWidget(parent) {
	setContentsMargins(8, 8, 8, 8);
	setObjectName("CUVSuggestBoxSearchViewContainer");
	setStyleSheet("#CUVSuggestBoxSearchViewContainer { background-color: tranparent; } ");
	themeMode = UVTheme->getThemeMode();
	connect(UVTheme, &CUVThemeManager::sigThemeModeChanged, this, [=](const ALThemeType::ThemeMode& mode) { themeMode = mode; });
}

CUVSuggestBoxSearchViewContainer::~CUVSuggestBoxSearchViewContainer() = default;

void CUVSuggestBoxSearchViewContainer::paintEvent(QPaintEvent* event) {
	QPainter painter(this);

	painter.save();
	painter.setRenderHint(QPainter::Antialiasing);
	UVTheme->drawEffectShadow(&painter, rect(), 6, 8);
	painter.setPen(UVThemeColor(themeMode, ALThemeType::PopupBorder));
	painter.setBrush(UVThemeColor(themeMode, ALThemeType::PopupBase));
	painter.drawRoundedRect(QRect(6, 0, rect().width() - 2 * 6, rect().height() - 6), 8, 8);
	painter.restore();
}
