#include "alsuggestboxsearchviewcontainer.hpp"

#include <QPainter>

#include "althememanager.hpp"

namespace AL {
/**
 * @brief \class CALSuggestBoxSearchViewContainer
 * @param parent pointer to the parent class
 */
CALSuggestBoxSearchViewContainer::CALSuggestBoxSearchViewContainer(QWidget* parent): QWidget(parent) {
	setContentsMargins(8, 8, 8, 8);
	setObjectName("CALSuggestBoxSearchViewContainer");
	setStyleSheet("#CALSuggestBoxSearchViewContainer { background-color: tranparent; } ");
	themeMode = ALTheme->getThemeMode();
	connect(ALTheme, &CALThemeManager::sigThemeModeChanged, this, [=](const ALThemeType::ThemeMode& mode) { themeMode = mode; });
}

CALSuggestBoxSearchViewContainer::~CALSuggestBoxSearchViewContainer() = default;

void CALSuggestBoxSearchViewContainer::paintEvent(QPaintEvent* event) {
	QPainter painter(this);

	painter.save();
	painter.setRenderHint(QPainter::Antialiasing);
	ALTheme->drawEffectShadow(&painter, rect(), 6, 8);
	painter.setPen(ALThemeColor(themeMode, ALThemeType::PopupBorder));
	painter.setBrush(ALThemeColor(themeMode, ALThemeType::PopupBase));
	painter.drawRoundedRect(QRect(6, 0, rect().width() - 2 * 6, rect().height() - 6), 8, 8);
	painter.restore();
}
} // namespace AL