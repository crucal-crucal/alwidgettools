#include "allistviewstyle.hpp"

#include <QPainter>
#include <QPainterPath>

#include "allistview.hpp"
#include "althememanager.hpp"

/**
 * @brief \namespace AL
 */
namespace AL {
/**
 * \class CALListViewStyle
 * @param style pointer to the parent style
 */
CALListViewStyle::CALListViewStyle(QStyle* style) {
	m_itemHeight = 35;
	m_isTransparent = false;
	m_leftPadding = 11;
	m_themeMode = ALTheme->getThemeMode();
	connect(ALTheme, &CALThemeManager::sigThemeModeChanged, this, [=](const ALThemeType::ThemeMode& mode) { m_themeMode = mode; });
}

CALListViewStyle::~CALListViewStyle() = default;

void CALListViewStyle::drawPrimitive(const PrimitiveElement pe, const QStyleOption* opt, QPainter* p, const QWidget* w) const {
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
				} else {
					if (vopt->state.testFlag(QStyle::State_MouseOver)) {
						p->fillPath(path, ALThemeColor(m_themeMode, ALThemeType::BasicHoverAlpha));
					}
				}
				p->restore();
			}
			return;
		}
		case QStyle::PE_PanelItemViewRow: {
			// itrm background interlacing
			if (const auto vopt = qstyleoption_cast<const QStyleOptionViewItem*>(opt)) {
				p->save();
				p->setRenderHint(QPainter::Antialiasing);
				p->setPen(Qt::NoPen);
				p->setBrush(ALThemeColor(m_themeMode, ALThemeType::BasicAlternating));
				p->drawRect(vopt->rect);
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

void CALListViewStyle::drawControl(const ControlElement element, const QStyleOption* opt, QPainter* p, const QWidget* w) const {
	switch (element) {
		case QStyle::CE_ShapedFrame: {
			// viewport 视口外其他区域的背景
			if (!m_isTransparent) {
				QRect frameRect = opt->rect;
				frameRect.adjust(1, 1, -1, -1);
				p->save();
				p->setRenderHints(QPainter::Antialiasing);
				p->setPen(ALThemeColor(m_themeMode, ALThemeType::PopupBorder));
				p->setBrush(ALThemeColor(m_themeMode, ALThemeType::BasicBase));
				p->drawRoundedRect(frameRect, 3, 3);
				p->restore();
			}
			return;
		}
		case QStyle::CE_ItemViewItem: {
			if (const auto vopt = qstyleoption_cast<const QStyleOptionViewItem*>(opt)) {
				// background
				this->drawPrimitive(QStyle::PE_PanelItemViewItem, opt, p, w);
				// content
				const QRect itemRect = opt->rect;
				p->save();
				p->setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing | QPainter::SmoothPixmapTransform);
				const auto listView = dynamic_cast<const CALListView*>(w);
				const QListView::ViewMode viewMode = listView->viewMode();
				QRect iconRect = proxy()->subElementRect(SE_ItemViewItemDecoration, vopt, w);
				QRect textRect = proxy()->subElementRect(SE_ItemViewItemText, vopt, w);
				iconRect.adjust(m_leftPadding, 0, 0, 0);
				textRect.adjust(m_leftPadding, 0, 0, 0);
				// icon
				if (!vopt->icon.isNull()) {
					QIcon::Mode mode = QIcon::Normal;
					if (!vopt->state.testFlag(QStyle::State_Enabled)) {
						mode = QIcon::Disabled;
					} else if (vopt->state.testFlag(QStyle::State_Selected)) {
						mode = QIcon::Selected;
					}
					const QIcon::State state = vopt->state.testFlag(QStyle::State_On) ? QIcon::On : QIcon::Off;
					vopt->icon.paint(p, iconRect, vopt->decorationAlignment, mode, state);
				}
				// text
				if (!vopt->text.isEmpty()) {
					p->setPen(ALThemeColor(m_themeMode, ALThemeType::BasicText));
					p->drawText(textRect, vopt->decorationAlignment, vopt->text);
				}
				// selected
				if (vopt->state.testFlag(QStyle::State_Selected) && viewMode == QListView::ListMode) {
					const int heightOffset = itemRect.height() / 4;
					p->setPen(Qt::NoPen);
					p->setBrush(ALThemeColor(m_themeMode, ALThemeType::PrimaryNormal));
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

QSize CALListViewStyle::sizeFromContents(const ContentsType ct, const QStyleOption* opt, const QSize& contentsSize, const QWidget* w) const {
	switch (ct) {
		case QStyle::CT_ItemViewItem: {
			QSize itemSize = QProxyStyle::sizeFromContents(ct, opt, contentsSize, w);
			const auto listView = dynamic_cast<const CALListView*>(w);
			if (const QListView::ViewMode viewMode = listView->viewMode(); viewMode == QListView::ListMode) {
				itemSize.setWidth(itemSize.width() + m_leftPadding);
			}
			itemSize.setHeight(m_itemHeight);
			return itemSize;
		}
		default: {
			break;
		}
	}

	return QProxyStyle::sizeFromContents(ct, opt, contentsSize, w);
}

void CALListViewStyle::setItemHeight(const int itemHeight) {
	m_itemHeight = itemHeight;
	Q_EMIT sigItemHeightChanged();
}

int CALListViewStyle::getItemHeight() const {
	return m_itemHeight;
}

void CALListViewStyle::setIsTransparent(const bool isTransparent) {
	m_isTransparent = isTransparent;
	Q_EMIT sigIsTransparentChanged();
}

bool CALListViewStyle::getIsTransparent() const {
	return m_isTransparent;
}
}
