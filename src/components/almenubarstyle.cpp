#include "almenubarstyle.hpp"

#include <QPainter>
#include <QStyleOptionToolButton>

#include "almenubar.hpp"
#include "althememanager.hpp"

/**
 * @brief \namespace AL
 */
namespace AL {
/**
 * @brief \class CALMenuBarStyle
 * @param style pointer to the parent style
 */
CALMenuBarStyle::CALMenuBarStyle(QStyle* style) {
	m_menubarItemMargin = 0;
	m_themeMode = ALTheme->getThemeMode();
	connect(ALTheme, &CALThemeManager::sigThemeModeChanged, this, [this](const ALThemeType::ThemeMode& mode) { m_themeMode = mode; });
}

CALMenuBarStyle::~CALMenuBarStyle() = default;

void CALMenuBarStyle::drawPrimitive(const PrimitiveElement pe, const QStyleOption* opt, QPainter* p, const QWidget* w) const {
	switch (pe) {
		case QStyle::PE_PanelButtonTool: {
			return;
		}
		default: {
			break;
		}
	}

	QProxyStyle::drawPrimitive(pe, opt, p, w);
}

void CALMenuBarStyle::drawControl(const ControlElement element, const QStyleOption* opt, QPainter* p, const QWidget* w) const {
	switch (element) {
		case QStyle::CE_ToolButtonLabel: {
			if (const auto topt = qstyleoption_cast<const QStyleOptionToolButton*>(opt)) {
				p->save();
				p->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform | QPainter::TextAntialiasing);
				p->setPen(Qt::NoPen);
				if (topt->state.testFlag(QStyle::State_Enabled) && topt->state.testFlag(QStyle::State_MouseOver)) {
					p->setBrush(ALThemeColor(m_themeMode, ALThemeType::BasicHoverAlpha));
					p->drawRect(topt->rect);
				}
				// 展开图标
				p->setPen(topt->state.testFlag(QStyle::State_Enabled) ? ALThemeColor(m_themeMode, ALThemeType::BasicText) : Qt::gray);
				QFont iconFont(ALIcon::getEnumTypeFontName(ALIcon::Awesome));
				iconFont.setPixelSize(18);
				p->setFont(iconFont);
				p->drawText(topt->rect, Qt::AlignCenter, QChar(static_cast<unsigned short>(ALIcon::AweSomeIcon::AngleRight)));
				p->restore();
			}
			return;
		}
		case QStyle::CE_MenuBarEmptyArea: {
			return;
		}
		case QStyle::CE_MenuBarItem: {
			if (const auto mopt = qstyleoption_cast<const QStyleOptionMenuItem*>(opt)) {
				if (mopt->menuItemType == QStyleOptionMenuItem::Separator) {
					return;
				}
				const int menuBarHeight = w->height();
				const QRect menuItemRect = mopt->rect;
				p->save();
				p->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform | QPainter::TextAntialiasing);
				// background
				p->setPen(Qt::NoPen);
				if (mopt->state.testFlag(QStyle::State_Enabled)) {
					if (mopt->state.testFlag(QStyle::State_Sunken)) {
						p->setBrush(ALThemeColor(m_themeMode, ALThemeType::BasicPressAlpha));
					} else if (mopt->state.testFlag(QStyle::State_Selected)) {
						p->setBrush(ALThemeColor(m_themeMode, ALThemeType::BasicSelectedAlpha));
					}
					p->drawRoundedRect(menuItemRect, 3, 3);
				}
				// icon and text
				const QIcon icon = mopt->icon;
				QString menuText = mopt->text;
				QString iconText;
				QString iconFontFamily;
				menuText = menuText.replace("&", "");
				if (const auto menuBar = dynamic_cast<const CALMenuBar*>(w)) {
					if (const QAction* action = menuBar->actionAt(menuItemRect.center())) {
						iconText = action->property(ALIcon::iconProperty).toString();
						iconFontFamily = ALIcon::getEnumTypeFontName(static_cast<ALIcon::IconType>(action->property(ALIcon::iconTypeProperty).toInt()));
					}
				}
				if (menuText.isEmpty()) {
					// icon
					if (!iconText.isEmpty() && !iconFontFamily.isEmpty() && iconFontFamily != ALIcon::errFontFamily) {
						p->save();
						p->setPen(ALThemeColor(m_themeMode, mopt->state.testFlag(QStyle::State_Enabled) ? ALThemeType::BasicText : ALThemeType::BasicDetailsText));
						QFont iconFont(iconFontFamily);
						iconFont.setPixelSize(menuBarHeight * 0.7); // NOLINT
						p->setFont(iconFont);
						p->drawText(menuItemRect, Qt::AlignCenter, iconText);
						p->restore();
					} else if (!icon.isNull()) {
						QRect menuIconRect(menuItemRect.center().x() - menuBarHeight * 0.4, menuItemRect.center().y() - menuBarHeight * 0.4, menuBarHeight * 0.8, menuBarHeight * 0.8); // NOLINT
						p->drawPixmap(menuIconRect, icon.pixmap(menuBarHeight * 0.8, menuBarHeight * 0.8));                                                                             // NOLINT
					}
				} else {
					// icon + text
					p->setPen(ALThemeColor(m_themeMode, mopt->state.testFlag(QStyle::State_Enabled) ? ALThemeType::BasicText : ALThemeType::BasicDetailsText));
					if (icon.isNull() && iconText.isEmpty()) {
						p->drawText(menuItemRect, Qt::AlignCenter, menuText);
					} else {
						if (!iconText.isEmpty() && !iconFontFamily.isEmpty() && iconFontFamily != ALIcon::errFontFamily) {
							p->save();
							const QRectF menuIconRect(menuItemRect.x() + menuBarHeight * 0.1 + m_menubarItemMargin, menuBarHeight * 0.1, menuBarHeight * 0.8, menuBarHeight * 0.8);
							QFont iconFont(iconFontFamily);
							iconFont.setPixelSize(menuBarHeight * 0.7); // NOLINT
							p->setFont(iconFont);
							p->drawText(menuIconRect, Qt::AlignCenter, iconText);
							p->restore();
							p->drawText(QRect(menuIconRect.right(), menuItemRect.y(), menuItemRect.width() - menuIconRect.width(), menuBarHeight), Qt::AlignCenter, menuText); // NOLINT
						} else {
							QRect menuIconRect(menuItemRect.x() + menuBarHeight * 0.1 + m_menubarItemMargin, menuBarHeight * 0.1, menuBarHeight * 0.8, menuBarHeight * 0.8);   // NOLINT
							p->drawPixmap(menuIconRect, icon.pixmap(menuBarHeight * 0.8, menuBarHeight * 0.8));                                                                // NOLINT
							p->drawText(QRect(menuIconRect.right(), menuItemRect.y(), menuItemRect.width() - menuIconRect.width(), menuBarHeight), Qt::AlignCenter, menuText); // NOLINT
						}
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

	QProxyStyle::drawControl(element, opt, p, w);
}

QSize CALMenuBarStyle::sizeFromContents(const ContentsType ct, const QStyleOption* opt, const QSize& contentsSize, const QWidget* w) const {
	switch (ct) {
		case QStyle::CT_MenuBar: {
			QSize menuBarHeight = QProxyStyle::sizeFromContents(ct, opt, contentsSize, w);
			menuBarHeight.setHeight(w->height());
			return menuBarHeight;
		}
		case QStyle::CT_MenuBarItem: {
			if (const auto mopt = qstyleoption_cast<const QStyleOptionMenuItem*>(opt)) {
				QSize menuBarItemSize = QProxyStyle::sizeFromContents(ct, opt, contentsSize, w);
				menuBarItemSize.setHeight(w->height());
				if (!mopt->icon.isNull() && !mopt->text.isEmpty()) {
					menuBarItemSize.setWidth(menuBarItemSize.width() + mopt->fontMetrics.horizontalAdvance(mopt->text) + 2 * m_menubarItemMargin);
				}
				if (!mopt->text.contains("&")) {
					menuBarItemSize.setWidth(menuBarItemSize.width() + 5);
				}
				return menuBarItemSize;
			}
		}
		default: {
			break;
		}
	}

	return QProxyStyle::sizeFromContents(ct, opt, contentsSize, w);
}

int CALMenuBarStyle::pixelMetric(const PixelMetric metric, const QStyleOption* option, const QWidget* widget) const {
	switch (metric) {
		case QStyle::PM_MenuBarHMargin: {
			return 0;
		}
		case QStyle::PM_MenuBarItemSpacing: {
			return 6;
		}
		case QStyle::PM_ToolBarExtensionExtent: {
			return 26;
		}
		default: {
			break;
		}
	}

	return QProxyStyle::pixelMetric(metric, option, widget);
}
}
