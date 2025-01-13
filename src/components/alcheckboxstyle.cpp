#include "alcheckboxstyle.hpp"

#include <QPainter>
#include <QStyleOptionButton>

#include "althememanager.hpp"

namespace AL {
/**
 * @brief \class CALCheckBoxStyle
 * @param style pointer to the parent style
 */
CALCheckBoxStyle::CALCheckBoxStyle(QStyle* style) {
	m_borderRadius = 6;
	m_checkIndicatorWidth = 21;
	m_themeMode = ALTheme->getThemeMode();
	connect(ALTheme, &CALThemeManager::sigThemeModeChanged, this, [=](const ALThemeType::ThemeMode& mode) { m_themeMode = mode; });
}

CALCheckBoxStyle::~CALCheckBoxStyle() = default;

void CALCheckBoxStyle::drawControl(const ControlElement element, const QStyleOption* opt, QPainter* p, const QWidget* w) const {
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
						p->setBrush(ALThemeColor(m_themeMode, ALThemeType::PrimaryNormal));
					} else {
						p->setBrush(ALThemeColor(m_themeMode, bopt->state.testFlag(QStyle::State_MouseOver) ? ALThemeType::PrimaryHover : ALThemeType::PrimaryNormal));
					}
				} else {
					if (bopt->state.testFlag(QStyle::State_Sunken)) {
						p->setBrush(ALThemeColor(m_themeMode, ALThemeType::BasicBorderDeep));
					} else {
						p->setPen(ALThemeColor(m_themeMode, ALThemeType::BasicBorderDeep));
						p->setBrush(ALThemeColor(m_themeMode, bopt->state.testFlag(QStyle::State_MouseOver) ? ALThemeType::BasicHover : ALThemeType::BasicBase));
					}
				}
				p->drawRoundedRect(checkRect, m_borderRadius, m_borderRadius);
				// 图标绘制
				p->setPen(ALThemeColor(m_themeMode, ALThemeType::BasicText));
				if (bopt->state.testFlag(QStyle::State_On)) {
					p->save();
					QFont iconFont("CALAwesome");
					iconFont.setPixelSize(m_checkIndicatorWidth * 0.75); // NOLINT
					p->setFont(iconFont);
					p->drawText(checkRect, Qt::AlignCenter, QChar(static_cast<unsigned short>(ALIcon::AweSomeIcon::Check)));
					p->restore();
				} else if (bopt->state.testFlag(QStyle::State_NoChange)) {
					p->drawLine(checkRect.x() + 3, checkRect.center().y(), checkRect.right() - 3, checkRect.center().y());
				}
				// 文字绘制
				p->setPen(ALThemeColor(m_themeMode, isEnable ? ALThemeType::BasicText : ALThemeType::BasicTextDisable));
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

int CALCheckBoxStyle::pixelMetric(const PixelMetric metric, const QStyleOption* option, const QWidget* widget) const {
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

void CALCheckBoxStyle::setCheckIndicatorWidth(const int width) {
	m_checkIndicatorWidth = width;
}

int CALCheckBoxStyle::getCheckIndicatorWidth() const {
	return m_checkIndicatorWidth;
}
} // namespace AL