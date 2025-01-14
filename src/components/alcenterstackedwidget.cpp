#include "alcenterstackedwidget.hpp"

#include <QPainter>
#include <QPainterPath>

#include "althememanager.hpp"

/**
 * @brief \namespace AL
 */
namespace AL {
/**
 * @brief \class CALCenterStackedWidget
 * @param parent pointer to the parent class
 */
CALCenterStackedWidget::CALCenterStackedWidget(QWidget* parent): QStackedWidget(parent) {
	setObjectName("CALCenterStackedWidget");
	m_themeMode = ALTheme->getThemeMode();
	connect(ALTheme, &CALThemeManager::sigThemeModeChanged, this, &CALCenterStackedWidget::slotThemeModeChanged);
}

CALCenterStackedWidget::~CALCenterStackedWidget() = default;

void CALCenterStackedWidget::slotThemeModeChanged(const ALThemeType::ThemeMode& mode) {
	m_themeMode = mode;
	update();
}

void CALCenterStackedWidget::setIsTransparent(const bool isTransparent) {
	m_isTransparent = isTransparent;
	update();
}

bool CALCenterStackedWidget::getIsTransparent() const {
	return m_isTransparent;
}

void CALCenterStackedWidget::setIsHasRadius(const bool isHasRadius) {
	m_isHasRadius = isHasRadius;
	update();
}

void CALCenterStackedWidget::paintEvent(QPaintEvent* event) {
	if (!m_isTransparent) {
		QPainter painter(this);
		QRect targetRect = this->rect();
		targetRect.adjust(1, 1, 10, 10);
		painter.save();
		painter.setRenderHint(QPainter::Antialiasing);
		painter.setPen(QPen(ALThemeColor(m_themeMode, ALThemeType::BasicBaseLine), 1.5));
		painter.setBrush(ALThemeColor(m_themeMode, ALThemeType::WindowCentralStackBase));
		if (m_isHasRadius) {
			painter.drawRoundedRect(targetRect, 10, 10);
		} else {
			painter.drawRect(targetRect);
		}
		painter.restore();
	}

	QStackedWidget::paintEvent(event);
}
}
