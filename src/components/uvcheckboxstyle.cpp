#include "uvcheckboxstyle.hpp"

#include <QPainter>
#include <QStyleOptionButton>

#include "uvthememanager.hpp"

/**
 * @brief \class CUVCheckBoxStyle
 * @param style pointer to the parent style
 */
CUVCheckBoxStyle::CUVCheckBoxStyle(QStyle* style) {
	m_borderRadius = 6;
	m_checkIndicatorWidth = 22;
	m_themeMode = UVTheme->getThemeMode();
	connect(UVTheme, &CUVThemeManager::sigThemeModeChanged, this, [=](const UVThemeType::ThemeMode& mode) { m_themeMode = mode; });
}

CUVCheckBoxStyle::~CUVCheckBoxStyle() = default;

void CUVCheckBoxStyle::drawControl(const ControlElement element, const QStyleOption* opt, QPainter* p, const QWidget* w) const {
	switch (element) {
		case QStyle::CE_CheckBox: {
			if (const auto bopt = qstyleoption_cast<const QStyleOptionButton*>(opt)) {
				const bool isEnable = bopt->state.testFlag(QStyle::State_Enabled);
				p->save();
				p->setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);
				const QRect checkBoxRect = bopt->rect;
				QRect checkRect(checkBoxRect.x(), checkBoxRect.y(), m_checkIndicatorWidth, m_checkIndicatorWidth);
				checkRect.adjust(1, 1, -1, -1);
				// 复选框绘制
				p->setPen(Qt::NoPen);
				if (bopt->state.testFlag(QStyle::State_On) || bopt->state.testFlag(QStyle::State_NoChange)) {
					p->setPen(Qt::NoPen);
					if (bopt->state.testFlag(QStyle::State_Sunken)) {
						p->setBrush(UVThemeColor(m_themeMode, UVThemeType::PrimaryNormal));
					} else {
						p->setBrush(UVThemeColor(m_themeMode, bopt->state.testFlag(QStyle::State_MouseOver) ? UVThemeType::PrimaryHover : UVThemeType::PrimaryNormal));
					}
				} else {
					if (bopt->state.testFlag(QStyle::State_Sunken)) {
						p->setBrush(UVThemeColor(m_themeMode, UVThemeType::BasicBorderDeep));
					} else {
						p->setPen(UVThemeColor(m_themeMode, UVThemeType::BasicBorderDeep));
						p->setBrush(UVThemeColor(m_themeMode, bopt->state.testFlag(QStyle::State_MouseOver) ? UVThemeType::BasicHover : UVThemeType::BasicBase));
					}
				}
				p->drawRoundedRect(checkRect, m_borderRadius, m_borderRadius);
				// 图标绘制
				p->setPen(UVThemeColor(m_themeMode, UVThemeType::BasicText));
				if (bopt->state.testFlag(QStyle::State_On)) {
					QFont iconFont("CUVAwesome");
					iconFont.setPixelSize(m_checkIndicatorWidth * 0.75); // NOLINT
					p->setFont(iconFont);
					p->drawText(checkRect, Qt::AlignCenter, QChar(static_cast<unsigned short>(UVIcon::CUVAweSomeIcon::Check)));
				} else if (bopt->state.testFlag(QStyle::State_NoChange)) {
					p->drawLine(checkRect.x() + 3, checkRect.center().y(), checkRect.right() - 3, checkRect.center().y());
				}
				// 文字绘制
				p->setPen(UVThemeColor(m_themeMode, isEnable ? UVThemeType::BasicText : UVThemeType::BasicTextDisable));
				const QRect textRect(checkRect.right() + 10, checkBoxRect.y(), checkBoxRect.width(), checkBoxRect.height());
				p->drawText(textRect, Qt::AlignLeft | Qt::AlignVCenter, bopt->text);
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

int CUVCheckBoxStyle::pixelMetric(const PixelMetric metric, const QStyleOption* option, const QWidget* widget) const {
	switch (metric) {
		case QStyle::PM_IndicatorWidth:
		case QStyle::PM_IndicatorHeight: {
			return m_checkIndicatorWidth;
		}
		case QStyle::PM_CheckBoxLabelSpacing: {
			return 10;
		}
		default: {
			break;
		}
	}

	return QProxyStyle::pixelMetric(metric, option, widget);
}

void CUVCheckBoxStyle::setCheckIndicatorWidth(const int width) {
	m_checkIndicatorWidth = width;
}

int CUVCheckBoxStyle::getCheckIndicatorWidth() const {
	return m_checkIndicatorWidth;
}
