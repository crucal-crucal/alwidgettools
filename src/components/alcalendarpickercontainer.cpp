#include "alcalendarpickercontainer.hpp"

#include <QPainter>

#include "althememanager.hpp"

/**
 * @brief \class CALCalendarPickerContainer
 * @param parent pointer to the parent class
 */
CALCalendarPickerContainer::CALCalendarPickerContainer(QWidget* parent): QWidget(parent) {
	setWindowFlags(Qt::FramelessWindowHint | Qt::Popup | Qt::NoDropShadowWindowHint);
	setAttribute(Qt::WA_TranslucentBackground);
	setContentsMargins(0, 0, 0, 0);
	setObjectName("CALCalendarPickerContainer");
	setStyleSheet("#CALCalendarPickerContainer { background-color: transparent; }");
	m_themeMode = ALTheme->getThemeMode();
	connect(ALTheme, &CALThemeManager::sigThemeModeChanged, this, [=](const ALThemeType::ThemeMode& mode) { m_themeMode = mode; });
}

CALCalendarPickerContainer::~CALCalendarPickerContainer() = default;

void CALCalendarPickerContainer::paintEvent(QPaintEvent* event) {
	QPainter painter(this);
	painter.save();
	painter.setRenderHints(QPainter::Antialiasing);
	ALTheme->drawEffectShadow(&painter, rect(), 6, 5);
	painter.restore();
}
