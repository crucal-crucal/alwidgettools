#include "allineeditstyle.hpp"

#include <QPainter>
#include <QPainterPath>
#include <QStyleOption>
#include <QtMath>

#include "althememanager.hpp"

/**
 * @brief \namespace AL
 */
namespace AL {
/**
 * @brief \class CALLineEditStyle
 * @param style pointer to the parent style
 */
CALLineEditStyle::CALLineEditStyle(QStyle* style) {
	m_themeMode = ALTheme->getThemeMode();
	connect(ALTheme, &CALThemeManager::sigThemeModeChanged, this, [this](const ALThemeType::ThemeMode& mode) { m_themeMode = mode; });
}

CALLineEditStyle::~CALLineEditStyle() = default;

void CALLineEditStyle::drawPrimitive(const PrimitiveElement pe, const QStyleOption* opt, QPainter* p, const QWidget* w) const {
	switch (pe) {
		case PE_PanelLineEdit: {
			if (const auto fopt = qstyleoption_cast<const QStyleOptionFrame*>(opt)) {
				const QRect lineEditRect = fopt->rect;
				p->save();
				p->setRenderHint(QPainter::Antialiasing);
				p->setPen(Qt::NoPen);
				// 边框
				p->setBrush(ALThemeColor(m_themeMode, ALThemeType::BasicBorder));
				p->drawRoundedRect(lineEditRect, 6, 6);
				// 背景
				p->setBrush(fopt->state.testFlag(QStyle::State_HasFocus) ? ALThemeColor(m_themeMode, ALThemeType::DialogBase) : fopt->state.testFlag(QStyle::State_MouseOver) ? ALThemeColor(m_themeMode, ALThemeType::BasicHover) : ALThemeColor(m_themeMode, ALThemeType::BasicBase));
				p->drawRoundedRect(QRectF(lineEditRect.x() + 1.5, lineEditRect.y() + 1.5, lineEditRect.width() - 2, lineEditRect.height() - 1), 6, 6);
				// 底边线
				p->setBrush(ALThemeColor(m_themeMode, ALThemeType::BasicHemline));
				QPainterPath path;
				path.moveTo(6, lineEditRect.height());
				path.lineTo(lineEditRect.width() - 6, lineEditRect.height());
				path.arcTo(QRectF(lineEditRect.width() - 12, lineEditRect.height() - 12, 12, 12), -90, 45);
				path.lineTo(6 - 3 * std::sqrt(2), lineEditRect.height() - (6 - 3 * std::sqrt(2)));
				path.arcTo(QRectF(0, lineEditRect.height() - 12, 12, 12), 225, 45);
				path.closeSubpath();
				p->drawPath(path);
				p->restore();
			}
			return;
		}
		default: break;
	}

	QProxyStyle::drawPrimitive(pe, opt, p, w);
}
}
