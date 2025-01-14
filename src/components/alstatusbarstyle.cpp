#include "alstatusbarstyle.hpp"

#include <QPainter>
#include <QStyleOption>

#include "althememanager.hpp"

/**
 * @brief \namespace AL
 */
namespace AL {
/**
 * @brief \class CALStatusBarStyle
 * @param style pointer to the parent style
 */
CALStatusBarStyle::CALStatusBarStyle(QStyle* style) {
	m_themeMode = ALTheme->getThemeMode();
	connect(ALTheme, &CALThemeManager::sigThemeModeChanged, this, [=](const ALThemeType::ThemeMode& mode) { m_themeMode = mode; });
}

CALStatusBarStyle::~CALStatusBarStyle() = default;

void CALStatusBarStyle::drawPrimitive(const PrimitiveElement pe, const QStyleOption* opt, QPainter* p, const QWidget* w) const {
	switch (pe) {
		case QStyle::PE_PanelStatusBar: {
			/// background
			const QRect statusBarRect = opt->rect;
			p->save();
			p->setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);
			p->setPen(ALThemeColor(m_themeMode, ALThemeType::BasicBorder));
			p->setBrush(ALThemeColor(m_themeMode, ALThemeType::BasicBaseAlpha));
			p->drawRect(statusBarRect);
			p->restore();
			return;
		}
		case QStyle::PE_FrameStatusBarItem: {
			/// Space character
			const QRect statusBarItemRect = opt->rect;
			p->save();
			p->setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);
			p->setPen(Qt::NoPen);
			p->setBrush(ALThemeColor(m_themeMode, ALThemeType::PrimaryNormal));
			p->drawRoundedRect(QRectF(statusBarItemRect.right(), statusBarItemRect.y() + statusBarItemRect.height() * 0.1, 3, statusBarItemRect.height() * 0.8), 2, 2);
			p->restore();
			return;
		}
		default: {
			break;
		}
	}

	QProxyStyle::drawPrimitive(pe, opt, p, w);
}

void CALStatusBarStyle::drawControl(const ControlElement element, const QStyleOption* opt, QPainter* p, const QWidget* w) const {
	switch (element) {
		case QStyle::CE_SizeGrip: {
			return;
		}
		default: {
			break;
		}
	}

	QProxyStyle::drawControl(element, opt, p, w);
}

QSize CALStatusBarStyle::sizeFromContents(const ContentsType ct, const QStyleOption* opt, const QSize& contentsSize, const QWidget* w) const {
	return QProxyStyle::sizeFromContents(ct, opt, contentsSize, w);
}

int CALStatusBarStyle::pixelMetric(const PixelMetric metric, const QStyleOption* option, const QWidget* widget) const {
	return QProxyStyle::pixelMetric(metric, option, widget);
}
}
