#include "uvcenterstackedwidget.hpp"

#include <QPainter>
#include <QPainterPath>

#include "uvtheme.hpp"

CUVCenterStackedWidget::CUVCenterStackedWidget(QWidget* parent): QStackedWidget(parent) {
	setObjectName("CUVCenterStackedWidget");
	m_themeMode = UVTheme->getThemeMode();
	connect(UVTheme, &CUVThemeManager::sigThemeModeChanged, this, &CUVCenterStackedWidget::slotThemeModeChanged);
}

CUVCenterStackedWidget::~CUVCenterStackedWidget() = default;

void CUVCenterStackedWidget::slotThemeModeChanged(const UVThemeType::ThemeMode& mode) {
	m_themeMode = mode;
	update();
}

void CUVCenterStackedWidget::setIsTransparent(const bool isTransparent) {
	m_isTransparent = isTransparent;
	update();
}

bool CUVCenterStackedWidget::getIsTransparent() const {
	return m_isTransparent;
}

void CUVCenterStackedWidget::setIsHasRadius(const bool isHasRadius) {
	m_isHasRadius = isHasRadius;
	update();
}

void CUVCenterStackedWidget::paintEvent(QPaintEvent* event) {
	if (!m_isTransparent) {
		QPainter painter(this);
		QRect targetRect = this->rect();
		targetRect.adjust(1, 1, 10, 10);
		painter.save();
		painter.setRenderHint(QPainter::Antialiasing);
		painter.setPen(QPen(UVThemeColor(m_themeMode, UVThemeType::BasicBaseLine), 1.5));
		painter.setBrush(UVThemeColor(m_themeMode, UVThemeType::WindowCentralStackBase));
		if (m_isHasRadius) {
			painter.drawRoundedRect(targetRect, 10, 10);
		} else {
			painter.drawRect(targetRect);
		}
		painter.restore();
	}

	QStackedWidget::paintEvent(event);
}
