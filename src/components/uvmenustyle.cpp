#include "uvmenustyle.hpp"

#include <QPainter>
#include <QPainterPath>
#include <QStyleOption>

#include "uvmenu.hpp"
#include "uvthememanager.hpp"

using namespace UVIcon;

/**
 * @brief \class CUVMenuStyle
 * @param style pointer to the parent style
 */
CUVMenuStyle::CUVMenuStyle(QStyle* style): QProxyStyle(style) {
	m_iconWidth = 22;
	m_shadowBorderWidth = 6;
	m_menuItemHeight = 32;
	m_themeMode = UVTheme->getThemeMode();
	connect(UVTheme, &CUVThemeManager::sigThemeModeChanged, this, [=](const UVThemeType::ThemeMode& mode) { m_themeMode = mode; });
}

CUVMenuStyle::~CUVMenuStyle() = default;

void CUVMenuStyle::drawPrimitive(const PrimitiveElement element, const QStyleOption* option, QPainter* painter, const QWidget* widget) const {
	switch (element) {
		case QStyle::PE_PanelMenu: {
			// 高性能阴影
			painter->save();
			painter->setRenderHint(QPainter::Antialiasing);
			UVTheme->drawEffectShadow(painter, option->rect, m_shadowBorderWidth, 6);
			// 背景绘制
			const QRect foregroundRect(m_shadowBorderWidth, m_shadowBorderWidth, option->rect.width() - 2 * m_shadowBorderWidth, option->rect.height() - 2 * m_shadowBorderWidth);
			painter->setPen(UVThemeColor(m_themeMode, UVThemeType::PopupBorder));
			painter->setBrush(UVThemeColor(m_themeMode, UVThemeType::PopupBase));
			painter->drawRoundedRect(foregroundRect, 6, 6);
			painter->restore();
			return;
		}
		case QStyle::PE_FrameMenu: {
			return;
		}
		default: {
			break;
		}
	}
	QProxyStyle::drawPrimitive(element, option, painter, widget);
}

void CUVMenuStyle::drawControl(const ControlElement element, const QStyleOption* option, QPainter* painter, const QWidget* widget) const {
	switch (element) {
		case QStyle::CE_MenuItem: {
			// 内容绘制 区分类型
			if (const auto mopt = qstyleoption_cast<const QStyleOptionMenuItem*>(option)) {
				if (mopt->menuItemType == QStyleOptionMenuItem::Separator) {
					const QRect separatorRect = mopt->rect;
					painter->save();
					painter->setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);
					painter->setPen(Qt::NoPen);
					painter->setBrush(UVThemeColor(m_themeMode, UVThemeType::BasicBaseLine));
					painter->drawRoundedRect(QRectF(separatorRect.x() + separatorRect.width() * 0.055, separatorRect.center().y(), separatorRect.width() - separatorRect.width() * 0.11, 1.5), 1, 1);
					painter->restore();
					return;
				} else {
					const QRect menuRect = mopt->rect;
					const qreal contentPadding = menuRect.width() * 0.055;
					const qreal textLeftSpacing = menuRect.width() * 0.082;
					painter->save();
					painter->setRenderHints(QPainter::SmoothPixmapTransform | QPainter::Antialiasing | QPainter::TextAntialiasing);
					// 覆盖效果
					if (mopt->state.testFlag(QStyle::State_Enabled) && (mopt->state.testFlag(QStyle::State_MouseOver) || mopt->state.testFlag(QStyle::State_Selected))) {
						QRect hoverRect = menuRect;
						hoverRect.adjust(0, 2, 0, -2);
						painter->setPen(Qt::NoPen);
						painter->setBrush(UVThemeColor(m_themeMode, UVThemeType::PopupHover));
						painter->drawRoundedRect(hoverRect, 5, 5);
					}
					// Icon绘制
					const QIcon menuIcon = mopt->icon;
					// check绘制
					if (mopt->menuHasCheckableItems) {
						painter->save();
						painter->setPen(!mopt->state.testFlag(QStyle::State_Enabled) ? Qt::gray : Qt::white);
						QFont iconFont("CUVAwesome");
						iconFont.setPixelSize(m_menuItemHeight * 0.57); // NOLINT
						painter->setFont(iconFont);
						painter->drawText(QRectF(menuRect.x() + contentPadding, menuRect.y(), m_iconWidth, menuRect.height()),
						                  Qt::AlignCenter, mopt->checked ? QChar(static_cast<unsigned short>(CUVAweSomeIcon::Check)) : QChar(static_cast<unsigned short>(CUVAweSomeIcon::None)));
						painter->restore();
					} else {
						QString iconText{};
						if (const auto menu = qobject_cast<const CUVMenu*>(widget)) {
							if (const auto action = menu->actionAt(menuRect.center())) {
								iconText = action->property("CUVIconType").toString();
							}
						}
						if (!iconText.isEmpty()) {
							painter->save();
							painter->setPen(!mopt->state.testFlag(QStyle::State_Enabled) ? Qt::gray : m_themeMode == UVThemeType::Light ? Qt::black : Qt::white);
							auto iconFont = QFont("CUVAwesome");
							iconFont.setPixelSize(m_menuItemHeight * 0.57); // NOLINT
							painter->setFont(iconFont);
							painter->drawText(QRectF(menuRect.x() + contentPadding, menuRect.y(), m_iconWidth, menuRect.height()), Qt::AlignCenter, iconText);
							painter->restore();
						} else {
							if (!menuIcon.isNull()) {
								painter->drawPixmap(QRect(menuRect.x() + contentPadding, menuRect.center().y() - m_iconWidth / 2, m_iconWidth, m_iconWidth), menuIcon.pixmap(m_iconWidth, m_iconWidth)); // NOLINT
							}
						}
					}
					// 文字和快捷键绘制
					if (!mopt->text.isEmpty()) {
						QFont font("Source Han Sans SC Normal");
						font.setPixelSize(14);
						painter->setFont(font);
						QStringList textList = mopt->text.split("\t");
						painter->setPen(!mopt->state.testFlag(QStyle::State_Enabled) ? Qt::gray : m_themeMode == UVThemeType::Light ? Qt::black : Qt::white);

						painter->drawText(QRectF(menuRect.x() + (m_isAnyoneItemHasIcon ? contentPadding + textLeftSpacing : 0) + m_iconWidth, menuRect.y(), menuRect.width(), menuRect.height()), Qt::AlignLeft | Qt::AlignVCenter | Qt::TextSingleLine, textList[0]);
						if (textList.count() > 1) {
							painter->drawText(QRectF(menuRect.x() + contentPadding + m_iconWidth + textLeftSpacing, menuRect.y(), menuRect.width() - (contentPadding * 2 + m_iconWidth + textLeftSpacing), menuRect.height()), Qt::AlignRight | Qt::AlignVCenter | Qt::TextSingleLine, textList[1]);
						}
					}
					// 展开图标
					if (mopt->menuItemType == QStyleOptionMenuItem::SubMenu) {
						painter->save();
						painter->setPen(!mopt->state.testFlag(QStyle::State_Enabled) ? Qt::gray : m_themeMode == UVThemeType::Light ? Qt::black : Qt::white);
						auto iconFont = QFont("CUVAwesome");
						iconFont.setPixelSize(18);
						painter->setFont(iconFont);
						painter->drawText(QRect(menuRect.right() - 25, menuRect.y(), 25, menuRect.height()), Qt::AlignVCenter, QChar(static_cast<unsigned short>(CUVAweSomeIcon::AngleRight)));
						painter->restore();
					}
					painter->restore();
				}
			}

			return;
		}
		case QStyle::CE_MenuEmptyArea: {
			return;
		}
		default: {
			break;
		}
	}
	QProxyStyle::drawControl(element, option, painter, widget);
}

int CUVMenuStyle::pixelMetric(const PixelMetric metric, const QStyleOption* option, const QWidget* widget) const {
	switch (metric) {
		case QStyle::PM_SmallIconSize: {
			//图标宽度
			return m_iconWidth;
		}
		case QStyle::PM_MenuPanelWidth: {
			//外围容器宽度
			return m_shadowBorderWidth * 1.5; // NOLINT
		}
		default: {
			break;
		}
	}
	return QProxyStyle::pixelMetric(metric, option, widget);
}

QSize CUVMenuStyle::sizeFromContents(const ContentsType type, const QStyleOption* option, const QSize& size, const QWidget* widget) const {
	switch (type) {
		case QStyle::CT_MenuItem: {
			if (const auto mopt = qstyleoption_cast<const QStyleOptionMenuItem*>(option)) {
				if (mopt->menuItemType == QStyleOptionMenuItem::Separator) {
					break;
				}
				const QSize menuItemSize = QProxyStyle::sizeFromContents(type, option, size, widget);
				const auto menu = dynamic_cast<const CUVMenu*>(widget);
				if (menu->isHasIcon() || mopt->menuHasCheckableItems) {
					m_isAnyoneItemHasIcon = true;
				}
				if (menu->isHasChildMenu()) {
					return { menuItemSize.width() + 20, m_menuItemHeight };
				} else {
					return { menuItemSize.width(), m_menuItemHeight };
				}
			}
		}
		default: {
			break;
		}
	}
	return QProxyStyle::sizeFromContents(type, option, size, widget);
}

void CUVMenuStyle::setMenuItemHeight(const int height) {
	m_menuItemHeight = height;
	emit sigMenuItemHeightChanged();
}

int CUVMenuStyle::getMenuItemHeight() const {
	return m_menuItemHeight;
}
