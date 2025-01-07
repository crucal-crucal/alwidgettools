#include "uvspinboxstyle.hpp"

#include <QPainter>
#include <QPainterPath>
#include <QStyleOptionSpinBox>

#include "althememanager.hpp"

CUVSpinBoxStyle::CUVSpinBoxStyle(QStyle* style) {
	m_themeMode = UVTheme->getThemeMode();
	connect(UVTheme, &CUVThemeManager::sigThemeModeChanged, this, [=](const ALThemeType::ThemeMode& mode) { m_themeMode = mode; });
}

CUVSpinBoxStyle::~CUVSpinBoxStyle() = default;

void CUVSpinBoxStyle::drawComplexControl(const ComplexControl cc, const QStyleOptionComplex* opt, QPainter* p, const QWidget* widget) const {
	switch (cc) {
		case CC_SpinBox: {
			if (const auto sopt = qstyleoption_cast<const QStyleOptionSpinBox*>(opt)) {
				const bool isEnable = sopt->state.testFlag(State_Enabled);
				p->save();
				p->setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing | QPainter::SmoothPixmapTransform);
				// background
				p->setPen(UVThemeColor(m_themeMode, ALThemeType::BasicBorder));
				p->setBrush(UVThemeColor(m_themeMode, isEnable? ALThemeType::BasicBase : ALThemeType::BasicDisable));
				p->drawRoundedRect(sopt->rect, 4, 4);
				// add button
				const QRect addLineRect = subControlRect(cc, sopt, SC_ScrollBarAddLine, widget);
				if (isEnable) {
					if (sopt->activeSubControls == SC_ScrollBarAddLine) {
						if (sopt->state.testFlag(State_Sunken) && sopt->state.testFlag(State_MouseOver)) {
							p->setBrush(UVThemeColor(m_themeMode, ALThemeType::BasicPressAlpha));
						} else {
							p->setBrush(UVThemeColor(m_themeMode, sopt->state.testFlag(QStyle::State_MouseOver) ? ALThemeType::BasicHoverAlpha : ALThemeType::BasicBaseDeep));
						}
					} else {
						p->setBrush(UVThemeColor(m_themeMode, ALThemeType::BasicBaseDeep));
					}
				} else {
					p->setBrush(UVThemeColor(m_themeMode, ALThemeType::BasicDisable));
				}
				QPainterPath addLinePath;
				addLinePath.moveTo(addLineRect.topLeft());
				addLinePath.lineTo(addLineRect.bottomLeft());
				addLinePath.lineTo(addLineRect.right() - 4, addLineRect.bottom());
				addLinePath.arcTo(QRectF(addLineRect.right() - 8, addLineRect.bottom() - 8, 8, 8), -90, 90);
				addLinePath.lineTo(addLineRect.right(), addLineRect.y() + 4);
				addLinePath.arcTo(QRectF(addLineRect.right() - 8, addLineRect.y(), 8, 8), 0, 90);
				addLinePath.closeSubpath();
				p->drawPath(addLinePath);
				// sub button
				const QRect subLineRect = subControlRect(cc, sopt, SC_ScrollBarSubLine, widget);
				if (isEnable) {
					if (sopt->activeSubControls == SC_ScrollBarSubLine) {
						if (sopt->state.testFlag(State_Sunken) && sopt->state.testFlag(State_MouseOver)) {
							p->setBrush(UVThemeColor(m_themeMode, ALThemeType::BasicPressAlpha));
						} else {
							p->setBrush(UVThemeColor(m_themeMode, sopt->state.testFlag(State_MouseOver) ? ALThemeType::BasicHoverAlpha : ALThemeType::BasicBaseDeep));
						}
					} else {
						p->setBrush(UVThemeColor(m_themeMode, ALThemeType::BasicBaseDeep));
					}
				} else {
					p->setBrush(UVThemeColor(m_themeMode, ALThemeType::BasicDisable));
				}
				QPainterPath subLinePath;
				subLinePath.moveTo(subLineRect.topLeft());
				subLinePath.lineTo(subLineRect.bottomLeft());
				subLinePath.lineTo(subLineRect.right() - 4, subLineRect.bottom());
				subLinePath.arcTo(QRectF(subLineRect.right() - 8, subLineRect.bottom() - 8, 8, 8), -90, 90);
				subLinePath.lineTo(subLineRect.right(), subLineRect.y() + 4);
				subLinePath.arcTo(QRectF(subLineRect.right() - 8, subLineRect.y(), 8, 8), 0, 90);
				subLinePath.closeSubpath();
				p->drawPath(subLinePath);
				// bottom line
				if (sopt->state.testFlag(State_HasFocus)) {
					p->setPen(QPen(UVThemeColor(m_themeMode, ALThemeType::PrimaryNormal), 2));
					p->drawLine(subLineRect.right() + 1, subLineRect.y() + subLineRect.height() - 2, addLineRect.left() - 1, subLineRect.y() + subLineRect.height() - 2);
				} else {
					p->setPen(UVThemeColor(m_themeMode, ALThemeType::BasicHemline));
					p->drawLine(subLineRect.right() + 1, subLineRect.y() + subLineRect.height() - 1, addLineRect.left() - 1, subLineRect.y() + subLineRect.height() - 1);
				}
				// add icon
				QFont iconFont("CUVAwesome");
				iconFont.setPixelSize(17);
				p->setFont(iconFont);
				p->setPen(UVThemeColor(m_themeMode, isEnable ? ALThemeType::BasicText : ALThemeType::BasicTextDisable));
				p->drawText(addLineRect, Qt::AlignCenter, QChar(static_cast<unsigned short>(ALIcon::AweSomeIcon::Plus)));
				// sub icon
				p->drawText(subLineRect, Qt::AlignCenter, QChar(static_cast<unsigned short>(ALIcon::AweSomeIcon::Minus)));
				p->restore();
			}
			return;
		}
		default: {
			break;
		}
	}

	QProxyStyle::drawComplexControl(cc, opt, p, widget);
}

QRect CUVSpinBoxStyle::subControlRect(const ComplexControl cc, const QStyleOptionComplex* opt, const SubControl sc, const QWidget* widget) const {
	const QRect rect = QProxyStyle::subControlRect(cc, opt, sc, widget);
	switch (cc) {
		case CC_SpinBox: {
			switch (sc) {
				case SC_ScrollBarAddLine: {
					// add button
					const QRect spinBoxRect = QProxyStyle::subControlRect(cc, opt, SC_SpinBoxFrame, widget);
					return { spinBoxRect.width() - spinBoxRect.height(), 0, spinBoxRect.height(), spinBoxRect.height() };
				}
				case SC_ScrollBarSubLine: {
					// sub buttom
					const QRect spinBoxRect = QProxyStyle::subControlRect(cc, opt, SC_SpinBoxFrame, widget);
					return { 0, 0, spinBoxRect.height(), spinBoxRect.height() };
				}
				case SC_SpinBoxEditField: {
					const QRect spinBoxRect = QProxyStyle::subControlRect(cc, opt, SC_SpinBoxFrame, widget);
					return { spinBoxRect.height(), 0, spinBoxRect.width() - spinBoxRect.height() * 2, spinBoxRect.height() };
				}
				default: {
					break;
				}
			}
			break;
		}
		default: {
			break;
		}
	}

	return rect;
}
