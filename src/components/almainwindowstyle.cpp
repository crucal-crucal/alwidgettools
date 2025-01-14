#include "almainwindowstyle.hpp"

#include <qmath.h>
#include <QPainter>
#include <QPainterPath>
#include <QStyleOption>

#include "althememanager.hpp"

/**
 * @brief \namespace AL
 */
namespace AL {
/**
 * @brief \class CALMainWindowStyle
 * @param style pointer to the parent style
 */
CALMainWindowStyle::CALMainWindowStyle(QStyle* style) {
	m_themeMode = ALTheme->getThemeMode();
	connect(ALTheme, &CALThemeManager::sigThemeModeChanged, this, [=](const ALThemeType::ThemeMode& mode) { m_themeMode = mode; });
}

CALMainWindowStyle::~CALMainWindowStyle() = default;

void CALMainWindowStyle::drawPrimitive(const PrimitiveElement pe, const QStyleOption* opt, QPainter* p, const QWidget* w) const {
	switch (pe) {
		case QStyle::PE_FrameTabBarBase: {
			return;
		}
		case QStyle::PE_PanelButtonTool: {
			if (opt->state.testFlag(QStyle::State_Enabled)) {
				p->save();
				p->setRenderHint(QPainter::Antialiasing);
				p->setPen(Qt::NoPen);
				const QColor brushColor = opt->state.testFlag(QStyle::State_Sunken) ? ALThemeColor(m_themeMode, ALThemeType::BasicHoverAlpha) : opt->state.testFlag(QStyle::State_MouseOver) ? ALThemeColor(m_themeMode, ALThemeType::BasicPressAlpha) : Qt::transparent;
				p->setBrush(brushColor);
				p->drawRect(opt->rect);
				p->restore();
			}
			return;
		}
		case QStyle::PE_IndicatorArrowLeft: {
			p->save();
			p->setRenderHint(QPainter::Antialiasing);
			p->setPen(Qt::NoPen);
			p->setBrush(ALThemeColor(m_themeMode, opt->state.testFlag(QStyle::State_Enabled) ? ALThemeType::BasicText : ALThemeType::BasicTextDisable));
			// 左三角
			constexpr int sideLength = 10;
			const QRect indicatorRect = opt->rect;
			QPainterPath path;
			path.moveTo(indicatorRect.center().x() - qCos(30 * M_PI / 180.0) * sideLength / 2, indicatorRect.center().y());
			path.lineTo(indicatorRect.center().x() + qCos(30 * M_PI / 180.0) * sideLength / 2, indicatorRect.center().y() - sideLength / 2.0);
			path.lineTo(indicatorRect.center().x() + qCos(30 * M_PI / 180.0) * sideLength / 2, indicatorRect.center().y() + sideLength / 2.0);
			path.closeSubpath();
			p->drawPath(path);
			p->restore();
			return;
		}
		case QStyle::PE_IndicatorArrowRight: {
			p->save();
			p->setRenderHint(QPainter::Antialiasing);
			p->setPen(Qt::NoPen);
			p->setBrush(ALThemeColor(m_themeMode, opt->state.testFlag(QStyle::State_Enabled) ? ALThemeType::BasicText : ALThemeType::BasicTextDisable));
			// 右三角
			constexpr int sideLength = 10;
			const QRect indicatorRect = opt->rect;
			QPainterPath path;
			path.moveTo(indicatorRect.center().x() - qCos(30 * M_PI / 180.0) * sideLength / 2, indicatorRect.center().y() - sideLength / 2.0);
			path.lineTo(indicatorRect.center().x() + qCos(30 * M_PI / 180.0) * sideLength / 2, indicatorRect.center().y());
			path.lineTo(indicatorRect.center().x() - qCos(30 * M_PI / 180.0) * sideLength / 2, indicatorRect.center().y() + sideLength / 2.0);
			path.closeSubpath();
			p->drawPath(path);
			p->restore();
			return;
		}
		case QStyle::PE_IndicatorTabTear: {
			return;
		}
		case QStyle::PE_IndicatorDockWidgetResizeHandle: {
			p->save();
			p->setRenderHint(QPainter::Antialiasing);
			p->setPen(QPen(ALThemeColor(m_themeMode, ALThemeType::BasicBaseLine), 2));
			const QRect handleRect = opt->rect;
			if (opt->state.testFlag(QStyle::State_Horizontal)) {
				p->drawLine(handleRect.x(), handleRect.center().y(), handleRect.right(), handleRect.center().y());
			} else {
				p->drawLine(handleRect.center().x(), handleRect.y(), handleRect.center().x(), handleRect.bottom());
			}
			p->restore();
			return;
		}
		default: {
			break;
		}
	}

	QProxyStyle::drawPrimitive(pe, opt, p, w);
}

void CALMainWindowStyle::drawControl(const ControlElement element, const QStyleOption* opt, QPainter* p, const QWidget* w) const {
	switch (element) {
		case QStyle::CE_RubberBand: {
			// 预览颜色
			const QRect rubberRandRect = opt->rect;
			p->save();
			p->setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);
			p->setPen(Qt::NoPen);
			p->setBrush(ALThemeColor(m_themeMode, ALThemeType::BasicHoverAlpha));
			p->drawRect(rubberRandRect);
			p->restore();
			return;
		}
		case QStyle::CE_TabBarTabShape: {
			// background
			if (const auto topt = qstyleoption_cast<const QStyleOptionTab*>(opt)) {
				const QRect tabRect = topt->rect;
				p->save();
				p->setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);
				p->setPen(Qt::NoPen);
				if (topt->state.testFlag(QStyle::State_Selected)) {
					if (topt->state.testFlag(QStyle::State_Sunken)) {
						p->setBrush(ALThemeColor(m_themeMode, ALThemeType::BasicHoverAlpha));
					} else {
						p->setBrush(ALThemeColor(m_themeMode, topt->state.testFlag(QStyle::State_MouseOver) ? ALThemeType::BasicSelectedHoverAlpha : ALThemeType::BasicSelectedAlpha));
					}
				} else {
					if (topt->state.testFlag(QStyle::State_Sunken)) {
						p->setBrush(ALThemeColor(m_themeMode, ALThemeType::BasicSelectedHoverAlpha));
					} else if (topt->state.testFlag(QStyle::State_MouseOver)) {
						p->setBrush(ALThemeColor(m_themeMode, ALThemeType::BasicHoverAlpha));
					}
				}
				p->drawRect(tabRect);
				// 间隔符绘制
				if (topt->position != QStyleOptionTab::End) {
					p->setPen(Qt::NoPen);
					p->setBrush(ALThemeColor(m_themeMode, ALThemeType::PrimaryNormal));
					p->drawRoundedRect(QRectF(tabRect.right() - 3, tabRect.y() + 7, 3, tabRect.height() - 14), 2, 2);
				}
				p->restore();
			}
			return;
		}
		case QStyle::CE_TabBarTabLabel: {
			// text
			if (const auto topt = qstyleoption_cast<const QStyleOptionTab*>(opt)) {
				p->save();
				p->setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);
				p->setPen(ALThemeColor(m_themeMode, ALThemeType::BasicText));
				p->drawText(topt->rect, Qt::AlignCenter, topt->text);
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
}
