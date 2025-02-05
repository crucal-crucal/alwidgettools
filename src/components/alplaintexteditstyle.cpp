#include "alplaintexteditstyle.hpp"

#include <QPainter>
#include <QPainterPath>
#include <QStyleOptionFrame>
#include <QtMath>

#include "althememanager.hpp"

/**
 * @brief \namespace AL
 */
namespace AL {
/**
 * @brief \class CALPlainTextEditStyle
 * @param style pointer to the parent style
 */
CALPlainTextEditStyle::CALPlainTextEditStyle(QStyle* style) {
	m_expandMarkWidth = 0.0;
	m_borderRadius = 6.0;
	m_themeMode = ALTheme->getThemeMode();
	connect(ALTheme, &CALThemeManager::sigThemeModeChanged, this, [=](const ALThemeType::ThemeMode& mode) { m_themeMode = mode; });
}

CALPlainTextEditStyle::~CALPlainTextEditStyle() = default;

void CALPlainTextEditStyle::drawControl(const ControlElement element, const QStyleOption* opt, QPainter* p, const QWidget* w) const {
	switch (element) {
		case QStyle::CE_ShapedFrame: {
			if (const auto fopt = qstyleoption_cast<const QStyleOptionFrame*>(opt)) {
				/// background
				const QRect editRect = fopt->rect;
				p->save();
				p->setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);
				p->setPen(Qt::NoPen);
				/// border
				p->setBrush(ALThemeColor(m_themeMode, ALThemeType::BasicBorder));
				p->drawRoundedRect(editRect, m_borderRadius, m_borderRadius);
				p->setBrush(ALThemeColor(m_themeMode, ALThemeType::BasicBase));
				p->drawRoundedRect(QRectF(editRect.x() + 1.5, editRect.y() + 1.5, editRect.width() - 3, editRect.height() - 3), m_borderRadius, m_borderRadius);
				/// bottom line
				p->setBrush(ALThemeColor(m_themeMode, ALThemeType::BasicHemline));
				QPainterPath path;
				path.moveTo(m_borderRadius, editRect.height());
				path.lineTo(editRect.width() - m_borderRadius, editRect.height());
				path.arcTo(QRectF(editRect.width() - m_borderRadius * 2, editRect.height() - m_borderRadius * 2, m_borderRadius * 2, m_borderRadius * 2), -90, 45);
				path.lineTo(m_borderRadius - 3 * std::sqrt(2), editRect.height() - (m_borderRadius - 3 * std::sqrt(2)));
				path.arcTo(QRectF(0, editRect.height() - m_borderRadius * 2, m_borderRadius * 2, m_borderRadius * 2), 270, 45);
				path.closeSubpath();
				p->drawPath(path);
				/// focus indicator
				p->setPen(Qt::NoPen);
				p->setBrush(ALThemeColor(m_themeMode, ALThemeType::PrimaryNormal));
				p->drawRoundedRect(QRectF(editRect.width() / 2.0 - m_expandMarkWidth, editRect.height() - 2.5, m_expandMarkWidth * 2, 2.5), 2, 2);
				p->restore();
			}
			return;
		}
		default: {
			break;
		}
	}

	QProxyStyle::drawControl(element, opt, p, w);
}

void CALPlainTextEditStyle::setExpandMarkWidth(const qreal expandMarkWidth) {
	m_expandMarkWidth = expandMarkWidth;
	Q_EMIT sigExpandMarkWidthChanged();
}

qreal CALPlainTextEditStyle::getExpandMarkWidth() const {
	return m_expandMarkWidth;
}
}
