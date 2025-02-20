#include "altableviewstyle.hpp"

#include <QPainter>
#include <QPainterPath>

#include "altableview.hpp"
#include "althememanager.hpp"

/**
 * @brief \namespace AL
 */
namespace AL {
CALTableViewStyle::CALTableViewStyle(QStyle* style) {
	m_horizontalPadding = 11;
	m_headerMargin = 6;
	m_currentHoverRow = -1;
	m_themeMode = ALTheme->getThemeMode();
	connect(ALTheme, &CALThemeManager::sigThemeModeChanged, this, [this](const ALThemeType::ThemeMode& mode) { m_themeMode = mode; });
}

CALTableViewStyle::~CALTableViewStyle() = default;

void CALTableViewStyle::drawPrimitive(const PrimitiveElement pe, const QStyleOption* opt, QPainter* p, const QWidget* w) const {
	switch (pe) {
		case QStyle::PE_PanelItemViewItem: {
			if (const auto vopt = qstyleoption_cast<const QStyleOptionViewItem*>(opt)) {
				if (const auto tableView = qobject_cast<const CALTableView*>(w)) {
					p->save();
					p->setRenderHint(QPainter::Antialiasing);
					if (const QAbstractItemView::SelectionBehavior selection_behavior = tableView->selectionBehavior(); selection_behavior == QAbstractItemView::SelectRows) {
						if (vopt->index.row() == m_currentHoverRow) {
							p->setPen(Qt::NoPen);
							p->setBrush(ALThemeColor(m_themeMode, ALThemeType::BasicHoverAlpha));
							p->drawRect(vopt->rect);
						}
					} else {
						if (vopt->state.testFlag(QStyle::State_Selected) || vopt->state.testFlag(QStyle::State_MouseOver)) {
							p->setPen(Qt::NoPen);
							p->setBrush(ALThemeColor(m_themeMode, ALThemeType::BasicHoverAlpha));
							p->drawRect(vopt->rect);
						}
					}
					p->restore();
				}
			}
			return;
		}
		case QStyle::PE_PanelItemViewRow: {
			if (const auto vopt = qstyleoption_cast<const QStyleOptionViewItem*>(opt)) {
				p->save();
				p->setRenderHint(QPainter::Antialiasing);
				const QRect itemRect = vopt->rect;
				p->setPen(Qt::NoPen);
				if (vopt->state.testFlag(QStyle::State_Selected)) {
					p->setBrush(ALThemeColor(m_themeMode, ALThemeType::BasicSelectedAlpha));
					p->drawRect(itemRect);
				} else if (vopt->features.testFlag(QStyleOptionViewItem::Alternate)) {
					p->setPen(Qt::NoPen);
					p->setBrush(ALThemeColor(m_themeMode, ALThemeType::BasicAlternating));
					p->drawRect(itemRect);
				}
				p->restore();
			}
			return;
		}
		case QStyle::PE_Widget: {
			return;
		}
		default: {
			break;
		}
	}

	QProxyStyle::drawPrimitive(pe, opt, p, w);
}

void CALTableViewStyle::drawControl(const ControlElement element, const QStyleOption* opt, QPainter* p, const QWidget* w) const {
	switch (element) {
		case QStyle::CE_ShapedFrame: {
			// viewport 视口外的其他区域背景
			QRect frameRect = opt->rect;
			frameRect.adjust(1, 1, -1, -1);
			p->save();
			p->setRenderHint(QPainter::Antialiasing);
			p->setPen(ALThemeColor(m_themeMode, ALThemeType::PopupBorder));
			p->setBrush(ALThemeColor(m_themeMode, ALThemeType::BasicBaseAlpha));
			p->drawRoundedRect(frameRect, 4, 4);
			p->restore();
			return;
		}
		case QStyle::CE_HeaderLabel: {
			// 表头文字
			if (const auto hopt = qstyleoption_cast<const QStyleOptionHeader*>(opt)) {
				const QRect headerRect = opt->rect;
				p->save();
				p->setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing | QPainter::SmoothPixmapTransform);
				if (!hopt->text.isEmpty()) {
					p->setPen(ALThemeColor(m_themeMode, ALThemeType::BasicText));
					p->drawText(headerRect, hopt->textAlignment, hopt->text);
				}
				p->restore();
			}
			return;
		}
		case QStyle::CE_HeaderSection: {
			// 表头背景
			p->save();
			p->setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing | QPainter::SmoothPixmapTransform);
			p->setPen(Qt::NoPen);
			p->setBrush(ALThemeColor(m_themeMode, ALThemeType::BasicBaseDeep));
			p->drawRect(opt->rect);
			if (opt->state.testFlag(QStyle::State_Sunken)) {
				p->setBrush(ALThemeColor(m_themeMode, ALThemeType::BasicPressAlpha));
			} else if (opt->state.testFlag(QStyle::State_MouseOver)) {
				p->setBrush(ALThemeColor(m_themeMode, ALThemeType::BasicHoverAlpha));
			}
			p->drawRect(opt->rect);
			p->restore();
			return;
		}
		case QStyle::CE_HeaderEmptyArea: {
			// 表头未使用区域
			const QRect frameRect = opt->rect;
			p->save();
			p->setRenderHint(QPainter::Antialiasing);
			p->setPen(Qt::NoPen);
			p->setBrush(ALThemeColor(m_themeMode, ALThemeType::BasicBaseDeep));
			p->drawRect(frameRect);
			p->restore();
			return;
		}
		case QStyle::CE_ItemViewItem: {
			if (const auto vopt = qstyleoption_cast<const QStyleOptionViewItem*>(opt)) {
				if (const auto tableView = qobject_cast<const CALTableView*>(w)) {
					// background
					drawPrimitive(QStyle::PE_PanelItemViewItem, opt, p, w);
					const QAbstractItemView::SelectionBehavior selection_behavior = tableView->selectionBehavior();
					// content
					const QRect itemRect = opt->rect;
					p->save();
					p->setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing | QPainter::SmoothPixmapTransform);
					QRect iconRect = proxy()->subElementRect(SE_ItemViewItemDecoration, vopt, w);
					QRect textRect = proxy()->subElementRect(SE_ItemViewItemText, vopt, w);
					if (vopt->index.column() == 0) {
						iconRect.adjust(m_horizontalPadding, 0, m_horizontalPadding, 0);
						textRect.adjust(m_horizontalPadding, 0, 0, 0);
					}
					// icon
					if (!vopt->icon.isNull()) {
						QIcon::Mode mode = QIcon::Normal;
						if (!vopt->state.testFlag(QStyle::State_Enabled)) {
							mode = QIcon::Disabled;
						} else if (vopt->state.testFlag(QStyle::State_Selected)) {
							mode = QIcon::Selected;
						}
						const QIcon::State state = vopt->state.testFlag(QStyle::State_Open) ? QIcon::On : QIcon::Off;
						vopt->icon.paint(p, iconRect, vopt->decorationAlignment, mode, state);
					}
					// text
					if (!vopt->text.isEmpty()) {
						p->setPen(ALThemeColor(m_themeMode, ALThemeType::BasicText));
						p->drawText(textRect, vopt->displayAlignment, vopt->text);
					}
					// select
					const int hegithOffset = itemRect.height() / 4;
					p->setPen(Qt::NoPen);
					p->setBrush(ALThemeColor(m_themeMode, ALThemeType::PrimaryNormal));
					if (vopt->state.testFlag(QStyle::State_Selected)) {
						if (selection_behavior == QAbstractItemView::SelectRows && vopt->index.column() == 0) {
							p->drawRoundedRect(QRectF(itemRect.x() + 3, itemRect.y() + hegithOffset, 3, itemRect.height() - 2 * hegithOffset), 3, 3);
						}
					}
					p->restore();
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

int CALTableViewStyle::pixelMetric(const PixelMetric metric, const QStyleOption* option, const QWidget* widget) const {
	switch (metric) {
		case QStyle::PM_HeaderMargin: {
			return m_headerMargin;
		}
		default: {
			break;
		}
	}

	return QProxyStyle::pixelMetric(metric, option, widget);
}

void CALTableViewStyle::setHeaderMargin(const int margin) {
	m_headerMargin = margin;
	Q_EMIT sigHeaderMarginChanged();
}

int CALTableViewStyle::getHeaderMargin() const {
	return m_headerMargin;
}

void CALTableViewStyle::setCurrentHoverRow(const int row) {
	m_currentHoverRow = row;
	Q_EMIT sigCurrentHoverRowChanged();
}

int CALTableViewStyle::getCurrentHoverRow() const {
	return m_currentHoverRow;
}
}
