#include "uvlineeditstyle.hpp"

#include <QPainter>
#include <QPainterPath>
#include <QStyleOption>
#include <QtMath>

#include "uvtheme.hpp"

CUVLineEditStyle::CUVLineEditStyle(QStyle* style) {
	themeMode = UVTheme->getThemeMode();
	connect(UVTheme, &CUVThemeManager::sigThemeModeChanged, this, [=](const UVThemeType::ThemeMode& mode) { themeMode = mode; });
}

CUVLineEditStyle::~CUVLineEditStyle() = default;

void CUVLineEditStyle::drawPrimitive(const PrimitiveElement pe, const QStyleOption* opt, QPainter* p, const QWidget* w) const {
	switch (pe) {
		case PE_PanelLineEdit: {
			if (const auto fopt = qstyleoption_cast<const QStyleOptionFrame*>(opt)) {
				const QRect lineEditRect = fopt->rect;
				p->save();
				p->setRenderHint(QPainter::Antialiasing);
				p->setPen(Qt::NoPen);
				// 边框
				p->setBrush(UVThemeColor(themeMode, UVThemeType::BasicBorder));
				p->drawRoundedRect(lineEditRect, 6, 6);
				// 背景
				p->setBrush(fopt->state.testFlag(QStyle::State_HasFocus) ? UVThemeColor(themeMode, UVThemeType::DialogBase) : fopt->state.testFlag(QStyle::State_MouseOver) ? UVThemeColor(themeMode, UVThemeType::BasicHover) : UVThemeColor(themeMode, UVThemeType::BasicBase));
				p->drawRoundedRect(QRectF(lineEditRect.x() + 1.5, lineEditRect.y() + 1.5, lineEditRect.width() - 2, lineEditRect.height() - 1), 6, 6);
				// 底边线
				p->setBrush(UVThemeColor(themeMode, UVThemeType::BasicHemline));
				QPainterPath path;
				path.moveTo(6, lineEditRect.height());
				path.lineTo(lineEditRect.width() - 6, lineEditRect.height());
				path.arcTo(QRectF(lineEditRect.width() - 12, lineEditRect.height() - 12, 12, 12), -90, 45);
				path.lineTo(6 - 3 * std::sqrt(2), lineEditRect.height() - (6 - 3 * std::sqrt(2)));
				path.arcTo(QRectF(0, lineEditRect.height() - 12, 12, 12), 270, 45);
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
