#include "allcdnumberstyle.hpp"

#include <QPainter>
#include <QStyleOptionFrame>

#include "althememanager.hpp"

/**
 * @brief \namespace AL
 */
namespace AL {
CALLCDNumberStyle::CALLCDNumberStyle(QStyle* parent) {
	m_isTransparent = false;
	m_borderRadius = 6;
	m_themeMode = ALTheme->getThemeMode();
	connect(ALTheme, &CALThemeManager::sigThemeModeChanged, this, [this](const ALThemeType::ThemeMode& mode) { m_themeMode = mode; });
}

CALLCDNumberStyle::~CALLCDNumberStyle() = default;

void CALLCDNumberStyle::drawControl(const ControlElement element, const QStyleOption* opt, QPainter* p, const QWidget* w) const {
	switch (element) {
		case QStyle::CE_ShapedFrame: {
			if (qstyleoption_cast<const QStyleOptionFrame*>(opt)) {
				if (!m_isTransparent) {
					// background
					const QRect numberRect = opt->rect;
					p->save();
					p->setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);
					p->setPen(Qt::NoPen);
					// border
					p->setBrush(ALThemeColor(m_themeMode, ALThemeType::BasicBorder));
					p->drawRoundedRect(numberRect, m_borderRadius, m_borderRadius);
					p->setBrush(ALThemeColor(m_themeMode, ALThemeType::BasicBase));
					p->drawRoundedRect(QRectF(numberRect.x() + 1.5, numberRect.y() + 1.5, numberRect.width() - 3.0, numberRect.height() - 3.0), m_borderRadius, m_borderRadius);
					p->restore();
				} else {
					return QProxyStyle::drawControl(element, opt, p, w);
				}
			}
			return;
		}
		default: {
			break;
		}
	}

	QProxyStyle::drawControl(element, opt, p, w);
}

void CALLCDNumberStyle::setIsTransparent(const bool istransparent) {
	m_isTransparent = istransparent;
}

bool CALLCDNumberStyle::getIsTransparent() const {
	return m_isTransparent;
}
}
