#include "uvradiobuttonstyle.hpp"

#include <QPainter>
#include <QStyleOptionButton>

#include "uvthememanager.hpp"

CUVRadioButtonStyle::CUVRadioButtonStyle(QStyle* style) {
	m_themeMode = UVTheme->getThemeMode();
	connect(UVTheme, &CUVThemeManager::sigThemeModeChanged, this, [=](const UVThemeType::ThemeMode& mode) { m_themeMode = mode; });
}

CUVRadioButtonStyle::~CUVRadioButtonStyle() = default;

void CUVRadioButtonStyle::drawPrimitive(const PrimitiveElement pe, const QStyleOption* opt, QPainter* p, const QWidget* w) const {
	switch (pe) {
		case PE_IndicatorRadioButton: {
			if (const auto bopt = qstyleoption_cast<const QStyleOptionButton*>(opt)) {
				p->save();
				p->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform | QPainter::TextAntialiasing);
				QRect buttonRect = bopt->rect;
				buttonRect.adjust(1, 1, -1, -1);
				if (bopt->state.testFlag(QStyle::State_Off)) {
					p->setPen(QPen(UVThemeColor(m_themeMode, UVThemeType::BasicBorder), 1.5));
					p->setBrush(UVThemeColor(m_themeMode, bopt->state.testFlag(QStyle::State_MouseOver) ? UVThemeType::BasicHover : UVThemeType::BasicBase));
					p->drawEllipse(QPointF(buttonRect.center().x() + 1, buttonRect.center().y() + 1), 8.5, 8.5);
				} else {
					p->setPen(Qt::NoPen);
					// 外圆
					p->setBrush(UVThemeColor(m_themeMode, UVThemeType::PrimaryNormal));
					p->drawEllipse(QPointF(buttonRect.center().x() + 1, buttonRect.center().y() + 1), buttonRect.width() / 2.0, buttonRect.width() / 2.0);
					// 内圆
					p->setBrush(UVThemeColor(m_themeMode, UVThemeType::BasicTextInvert));
					if (bopt->state.testFlag(QStyle::State_Sunken)) {
						if (bopt->state.testFlag(QStyle::State_MouseOver)) {
							p->drawEllipse(QPointF(buttonRect.center().x() + 1, buttonRect.center().y() + 1), buttonRect.width() / 4.5, buttonRect.width() / 4.5);
						}
					} else {
						const qreal scale = bopt->state.testFlag(QStyle::State_MouseOver) ? 3.5 : 4.0;
						p->drawEllipse(QPointF(buttonRect.center().x() + 1, buttonRect.center().y() + 1), buttonRect.width() / scale, buttonRect.width() / scale);
					}
				}
				p->restore();
			}
			return;
		}
		default: {
			break;
		}
	}

	QProxyStyle::drawPrimitive(pe, opt, p, w);
}

int CUVRadioButtonStyle::pixelMetric(const PixelMetric metric, const QStyleOption* option, const QWidget* widget) const {
	switch (metric) {
		case QStyle::PM_ExclusiveIndicatorWidth:
		case QStyle::PM_ExclusiveIndicatorHeight: {
			return 20;
		}
		default: {
			break;
		}
	}

	return QProxyStyle::pixelMetric(metric, option, widget);
}
