#include "alpovitstyle.hpp"

#include <QPainter>
#include <QStyleOptionViewItem>

#include "althememanager.hpp"

/**
 * @brief \namespace AL
 */
namespace AL {
/**
 * @brief \class CALPovitStyle
 * @param style pointer to the parent style
 */
CALPovitStyle::CALPovitStyle(QStyle* style) {
	m_currentIndex = -1;
	m_povitSpacing = 5;
	m_themeMode = ALTheme->getThemeMode();
	connect(ALTheme, &CALThemeManager::sigThemeModeChanged, this, [=](const ALThemeType::ThemeMode& mode) { m_themeMode = mode; });
}

CALPovitStyle::~CALPovitStyle() = default;

void CALPovitStyle::drawPrimitive(const PrimitiveElement pe, const QStyleOption* opt, QPainter* p, const QWidget* w) const {
	switch (pe) {
		case QStyle::PE_PanelItemViewRow:
		case QStyle::PE_Widget: {
			return;
		}
		default: {
			break;
		}
	}

	QProxyStyle::drawPrimitive(pe, opt, p, w);
}

void CALPovitStyle::drawControl(const ControlElement element, const QStyleOption* opt, QPainter* p, const QWidget* w) const {
	switch (element) {
		case QStyle::CE_ShapedFrame: {
			// viewport 视口外的其他区域背景
			return;
		}
		case QStyle::CE_ItemViewItem: {
			if (const auto vopt = qstyleoption_cast<const QStyleOptionViewItem*>(opt)) {
				/// content
				p->save();
				p->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform | QPainter::TextAntialiasing);
				QRect textRect = proxy()->subElementRect(SE_ItemViewItemText, vopt, w);
				textRect.adjust(0, 0, 0, -5);
				// text
				if (!vopt->text.isEmpty()) {
					p->setPen(ALThemeColor(m_themeMode, m_pressIndex == vopt->index ? ALThemeType::BasicTextPress : m_currentIndex == vopt->index.row() || vopt->state.testFlag(QStyle::State_MouseOver) ? ALThemeType::BasicText : ALThemeType::BasicTextNoFocus));
					p->drawText(textRect, Qt::AlignCenter, vopt->text);
				}
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

int CALPovitStyle::pixelMetric(const PixelMetric metric, const QStyleOption* option, const QWidget* widget) const {
	switch (metric) {
		case QStyle::PM_FocusFrameHMargin: {
			return m_povitSpacing;
		}
		default: {
			break;
		}
	}

	return QProxyStyle::pixelMetric(metric, option, widget);
}

const QColor& CALPovitStyle::getMarkColor() const {
	return ALThemeColor(m_themeMode, ALThemeType::PrimaryNormal);
}

void CALPovitStyle::setPressIndex(const QModelIndex& index) {
	m_pressIndex = index;
}

QModelIndex CALPovitStyle::getPressIndex() const {
	return m_pressIndex;
}

void CALPovitStyle::setCurrentIndex(const int index) {
	m_currentIndex = index;
}

int CALPovitStyle::getCurrentIndex() const {
	return m_currentIndex;
}

void CALPovitStyle::setPovitSpacing(const int spacing) {
	m_povitSpacing = spacing;
}

int CALPovitStyle::getPovitSpacing() const {
	return m_povitSpacing;
}
}
