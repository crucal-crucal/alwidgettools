#include "altreeviewstyle.hpp"

#include <QPainter>
#include <QPainterPath>
#include <QStyleOptionViewItem>

#include "althememanager.hpp"

/**
 * @brief \namespace AL
 */
namespace AL {
/**
 * @brief \class CALTreeViewStyle
 * @param style pointer to the parent style
 */
CALTreeViewStyle::CALTreeViewStyle(QStyle* style) {
	m_leftPadding = 11;
	m_itemHeight = 35;
	m_headerMargin = 5;
	m_themeMode = ALTheme->getThemeMode();
	connect(ALTheme, &CALThemeManager::sigThemeModeChanged, this, [=](const ALThemeType::ThemeMode& mode) { m_themeMode = mode; });
}

CALTreeViewStyle::~CALTreeViewStyle() = default;

void CALTreeViewStyle::drawPrimitive(const PrimitiveElement pe, const QStyleOption* opt, QPainter* p, const QWidget* w) const {
	switch (pe) {
		case QStyle::PE_PanelItemViewItem: {
			// item background
			if (const auto vopt = qstyleoption_cast<const QStyleOptionViewItem*>(opt)) {
				p->save();
				p->setRenderHint(QPainter::Antialiasing);
				QRect itemRect = vopt->rect;
				itemRect.adjust(0, 2, 0, -2);
				QPainterPath path;
				path.addRoundedRect(itemRect, 4, 4);
				if (vopt->state.testFlag(QStyle::State_Selected)) {
					p->fillPath(path, ALThemeColor(m_themeMode, vopt->state.testFlag(QStyle::State_MouseOver) ? ALThemeType::BasicSelectedHoverAlpha : ALThemeType::BasicSelectedAlpha));
				} else if (vopt->state.testFlag(QStyle::State_MouseOver)) {
					p->fillPath(path, ALThemeColor(m_themeMode, ALThemeType::BasicHoverAlpha));
				}
				p->restore();
			}
			return;
		}
		case QStyle::PE_IndicatorBranch: {
			// expand icon
			if (const auto vopt = qstyleoption_cast<const QStyleOptionViewItem*>(opt); vopt->state.testFlag(QStyle::State_Children)) {
				p->save();
				QRect indicatorRect = opt->rect;
				indicatorRect.adjust(0, 0, -2, 0);
				QFont iconFont(ALIcon::getEnumTypeFontName(ALIcon::Fluent));
				iconFont.setPixelSize(17);
				p->setFont(iconFont);
				p->setPen(ALThemeColor(m_themeMode, ALThemeType::BasicText));
				p->drawText(indicatorRect, Qt::AlignVCenter | Qt::AlignRight, vopt->state.testFlag(QStyle::State_Open) ? QChar(static_cast<unsigned short>(ALIcon::FluentIcon::ChevronDown)) : QChar(static_cast<unsigned short>(ALIcon::AweSomeIcon::AngleRight)));
				p->restore();
			}
			return;
		}
		case QStyle::PE_PanelItemViewRow:
		case QStyle::PE_Widget:
		case QStyle::PE_Frame: {
			return;
		}
		default: {
			break;
		}
	}

	QProxyStyle::drawPrimitive(pe, opt, p, w);
}

void CALTreeViewStyle::drawControl(const ControlElement element, const QStyleOption* opt, QPainter* p, const QWidget* w) const {
	switch (element) {
		case QStyle::CE_ShapedFrame: {
			// viewport 视口外的其他区域背景
			QRect frameRect = opt->rect;
			frameRect.adjust(1, 1, -1, -1);
			p->save();
			p->setRenderHint(QPainter::Antialiasing);
			p->setPen(ALThemeColor(m_themeMode, ALThemeType::PopupBorder));
			p->setBrush(ALThemeColor(m_themeMode, ALThemeType::BasicBase));
			p->drawRoundedRect(frameRect, 3, 3);
			p->restore();
			return;
		}
		case QStyle::CE_HeaderLabel: {
			// 表头文字
			if (const auto hopt = qstyleoption_cast<const QStyleOptionHeader*>(opt); !hopt->text.isEmpty()) {
				const QRect textRect = hopt->rect;
				p->save();
				p->setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing | QPainter::SmoothPixmapTransform);
				p->setPen(ALThemeColor(m_themeMode, ALThemeType::BasicText));
				p->drawText(textRect, hopt->textAlignment, hopt->text);
				p->restore();
			}
			return;
		}
		case QStyle::CE_HeaderSection: {
			// 表头背景
			p->save();
			p->setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing | QPainter::SmoothPixmapTransform);
			p->setPen(Qt::NoPen);
			p->setBrush(ALThemeColor(m_themeMode, ALThemeType::BasicBaseDeepAlpha));
			p->drawRect(opt->rect);
			p->restore();
			return;
		}
		case QStyle::CE_ItemViewItem: {
			if (const auto vopt = qstyleoption_cast<const QStyleOptionViewItem*>(opt)) {
				// background
				drawPrimitive(QStyle::PE_PanelItemViewItem, opt, p, w);
				/// content
				const QRect itemRect = opt->rect;
				p->save();
				p->setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing | QPainter::SmoothPixmapTransform);
				const QRect checkRect = proxy()->subElementRect(SE_ItemViewItemCheckIndicator, vopt, w);
				const QRect iconRect = proxy()->subElementRect(SE_ItemViewItemDecoration, vopt, w);
				const QRect textRect = proxy()->subElementRect(SE_ItemViewItemText, vopt, w);
				// checkbox
				if (checkRect.isValid()) {
					p->save();
					if (vopt->checkState == Qt::Checked) {
						p->setPen(Qt::NoPen);
						p->setBrush(ALThemeColor(m_themeMode, ALThemeType::PrimaryNormal));
						p->drawRoundedRect(checkRect, 2, 2);
						QFont iconFont(ALIcon::getEnumTypeFontName(ALIcon::Awesome));
						iconFont.setPixelSize(static_cast<int>(checkRect.width() * 0.85));
						p->setFont(iconFont);
						p->setPen(ALThemeColor(ALThemeType::Dark, ALThemeType::BasicText));
						p->drawText(checkRect, Qt::AlignCenter, QChar(static_cast<unsigned short>(ALIcon::AweSomeIcon::Check)));
					} else if (vopt->checkState == Qt::PartiallyChecked) {
						p->setPen(Qt::NoPen);
						p->setBrush(ALThemeColor(m_themeMode, ALThemeType::PrimaryNormal));
						p->drawRoundedRect(checkRect, 2, 2);
						p->setPen(ALThemeColor(ALThemeType::Dark, ALThemeType::BasicText));
						const QLine checkLine(checkRect.x() + 3, checkRect.center().y(), checkRect.right() - 3, checkRect.center().y());
						p->drawLine(checkLine);
					} else {
						p->setPen(QPen(ALThemeColor(m_themeMode, ALThemeType::BasicBorderDeep), 1.2));
						p->setBrush(Qt::transparent);
						p->drawRoundedRect(checkRect, 2, 2);
					}
					p->restore();
				}
				// icon
				if (!vopt->icon.isNull()) {
					constexpr QIcon::Mode mode = QIcon::Normal;
					const QIcon::State state = vopt->state.testFlag(QStyle::State_Open) ? QIcon::On : QIcon::Off;
					vopt->icon.paint(p, iconRect, vopt->decorationAlignment, mode, state);
				}
				// text
				if (!vopt->text.isEmpty()) {
					p->setPen(ALThemeColor(m_themeMode, ALThemeType::BasicText));
					p->drawText(textRect, vopt->displayAlignment, vopt->text);
				}
				// select effect
				const int heightOffset = itemRect.height() / 4;
				p->setPen(Qt::NoPen);
				p->setBrush(ALThemeColor(m_themeMode, ALThemeType::PrimaryNormal));
				if (vopt->state.testFlag(QStyle::State_Selected) && (vopt->viewItemPosition == QStyleOptionViewItem::Beginning || vopt->viewItemPosition == QStyleOptionViewItem::OnlyOne)) {
					p->drawRoundedRect(QRectF(itemRect.x() + 3, itemRect.y() + heightOffset, 3, itemRect.height() - 2 * heightOffset), 3, 3);
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

QSize CALTreeViewStyle::sizeFromContents(const ContentsType ct, const QStyleOption* opt, const QSize& contentsSize, const QWidget* w) const {
	switch (ct) {
		case QStyle::CT_ItemViewItem: {
			QSize itemSize = QProxyStyle::sizeFromContents(ct, opt, contentsSize, w);
			itemSize.setHeight(m_itemHeight);
			return itemSize;
		}
		default: {
			break;
		}
	}

	return QProxyStyle::sizeFromContents(ct, opt, contentsSize, w);
}

int CALTreeViewStyle::pixelMetric(const PixelMetric metric, const QStyleOption* option, const QWidget* widget) const {
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

QRect CALTreeViewStyle::subElementRect(const SubElement subElement, const QStyleOption* option, const QWidget* widget) const {
	switch (subElement) {
		case QStyle::SE_ItemViewItemCheckIndicator: {
			if (const auto vopt = qstyleoption_cast<const QStyleOptionViewItem*>(option)) {
				if (vopt->viewItemPosition != QStyleOptionViewItem::Middle && vopt->viewItemPosition != QStyleOptionViewItem::End) {
					QRect indicatorRect = QProxyStyle::subElementRect(subElement, option, widget);
					indicatorRect.adjust(m_leftPadding, 0, m_leftPadding, 0);
					return indicatorRect;
				}
			}
			break;
		}
		case QStyle::SE_ItemViewItemDecoration: {
			if (const auto vopt = qstyleoption_cast<const QStyleOptionViewItem*>(option)) {
				if (vopt->viewItemPosition != QStyleOptionViewItem::Middle && vopt->viewItemPosition != QStyleOptionViewItem::End) {
					QRect iconRect = QProxyStyle::subElementRect(subElement, option, widget);
					iconRect.adjust(m_leftPadding + 5, 0, m_leftPadding + 5, 0);
					return iconRect;
				}
			}
			break;
		}
		case QStyle::SE_ItemViewItemText: {
			if (const auto vopt = qstyleoption_cast<const QStyleOptionViewItem*>(option)) {
				if (vopt->viewItemPosition != QStyleOptionViewItem::Middle && vopt->viewItemPosition != QStyleOptionViewItem::End) {
					QRect textRect = QProxyStyle::subElementRect(subElement, option, widget);
					textRect.adjust(m_leftPadding + 10, 0, 0, 0);
					return textRect;
				}
			}
			break;
		}
		default: {
			break;
		}
	}

	return QProxyStyle::subElementRect(subElement, option, widget);
}

void CALTreeViewStyle::setItemHeight(const int itemHeight) {
	m_itemHeight = itemHeight;
	Q_EMIT sigItemHeightChanged();
}

int CALTreeViewStyle::getItemHeight() const {
	return m_itemHeight;
}

void CALTreeViewStyle::setHeaderMargin(const int headerMargin) {
	m_headerMargin = headerMargin;
	Q_EMIT sigHeaderMarginChanged();
}

int CALTreeViewStyle::getHeaderMargin() const {
	return m_headerMargin;
}
}
