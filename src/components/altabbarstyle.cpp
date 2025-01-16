#include "altabbarstyle.hpp"

#include <QPainter>
#include <QPainterPath>
#include <QStyleOptionTab>

#include "althememanager.hpp"

/**
 * @brief \namespace AL
 */
namespace AL {
CALTabBarStyle::CALTabBarStyle(QStyle* style) {
	m_themeMode = ALTheme->getThemeMode();
	connect(ALTheme, &CALThemeManager::sigThemeModeChanged, this, [=](const ALThemeType::ThemeMode& mode) { m_themeMode = mode; });
}

CALTabBarStyle::~CALTabBarStyle() = default;

void CALTabBarStyle::drawPrimitive(const PrimitiveElement pe, const QStyleOption* opt, QPainter* p, const QWidget* w) const {
	switch (pe) {
		case QStyle::PE_FrameTabBarBase: // 底边线
		case QStyle::PE_IndicatorArrowLeft:
		case QStyle::PE_IndicatorArrowRight:
		case QStyle::PE_PanelButtonTool: // 左侧隐藏图标
		case QStyle::PE_IndicatorTabTear: {
			return;
		}
		case QStyle::PE_IndicatorTabClose: {
			p->save();
			p->setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);
			if (opt->state.testFlag(QStyle::State_MouseOver)) {
				p->setPen(Qt::NoPen);
				p->setBrush(ALThemeColor(m_themeMode, ALThemeType::BasicHoverAlpha));
				p->drawRoundedRect(opt->rect, 2, 2);
			}
			QFont iconFont(ALIcon::getEnumTypeFontName(ALIcon::Awesome));
			iconFont.setPixelSize(16);
			p->setFont(iconFont);
			p->setPen(ALThemeColor(m_themeMode, ALThemeType::BasicText));
			p->drawText(opt->rect, Qt::AlignCenter, QChar(static_cast<unsigned short>(ALIcon::AweSomeIcon::Close)));
			p->restore();
			return;
		}
		default: {
			break;
		}
	}

	QProxyStyle::drawPrimitive(pe, opt, p, w);
}

void CALTabBarStyle::drawControl(const ControlElement element, const QStyleOption* opt, QPainter* p, const QWidget* w) const {
	switch (element) {
		case QStyle::CE_TabBarTabShape: {
			/// background
			if (const auto topt = qstyleoption_cast<const QStyleOptionTab*>(opt)) {
				QRect tabRect = topt->rect;
				p->save();
				p->setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);
				p->setPen(Qt::NoPen);
				if (topt->state.testFlag(QStyle::State_Selected)) {
					constexpr int margin = 9;
					constexpr int topRadius = 7;
					// select background
					tabRect.setLeft(tabRect.left() - margin);
					if (topt->position != QStyleOptionTab::End) {
						tabRect.setRight(tabRect.right() + margin + 1);
					}
					p->setBrush(ALThemeColor(m_themeMode, ALThemeType::BasicSelectedAlpha));
					QPainterPath path;
					path.moveTo(tabRect.x(), tabRect.bottom() + 1);
					path.arcTo(QRectF(tabRect.x() - margin, tabRect.bottom() - margin * 2 + 1, margin * 2, margin * 2), -90, 90);
					path.lineTo(tabRect.x() + margin, tabRect.y() + topRadius);
					path.arcTo(QRectF(tabRect.x() + margin, tabRect.y(), topRadius * 2, topRadius * 2), 180, -90);
					path.lineTo(tabRect.right() - margin - topRadius, tabRect.y());
					path.arcTo(QRectF(tabRect.right() - margin - 2 * topRadius, tabRect.y(), topRadius * 2, topRadius * 2), 90, -90);
					path.lineTo(tabRect.right() - margin, tabRect.bottom() - margin);
					path.arcTo(QRectF(tabRect.right() - margin, tabRect.bottom() - 2 * margin + 1, margin * 2, margin * 2), -180, 90);
					path.lineTo(tabRect.right(), tabRect.bottom() + 10);
					path.lineTo(tabRect.x(), tabRect.bottom() + 10);
					path.closeSubpath();
					p->drawPath(path);
				} else {
					p->setBrush(topt->state.testFlag(QStyle::State_MouseOver) ? ALThemeColor(m_themeMode, ALThemeType::BasicHoverAlpha) : Qt::transparent);
					tabRect.setHeight(tabRect.height() + 10);
					p->drawRoundedRect(tabRect, 0, 0);
					tabRect.setHeight(tabRect.height() - 10);
				}
				/// Space character
				if (!topt->state.testFlag(QStyle::State_Selected) && topt->position != QStyleOptionTab::End && topt->selectedPosition != QStyleOptionTab::NextIsSelected) {
					p->setPen(Qt::NoPen);
					p->setBrush(ALThemeColor(m_themeMode, ALThemeType::PrimaryNormal));
					p->drawRoundedRect(QRectF(tabRect.right() - 3, tabRect.y() + 7, 3, tabRect.height() - 14), 2, 2);
				}
				p->restore();
			}
			return;
		}
		case QStyle::CE_TabBarTabLabel: {
			/// text and icon
			if (const auto topt = qstyleoption_cast<const QStyleOptionTab*>(opt)) {
				QRect textRect = subElementRect(QStyle::SE_TabBarTabText, topt, w);
				textRect.setLeft(textRect.left() + 10);
				p->save();
				p->setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing | QPainter::SmoothPixmapTransform);
				// icon
				if (const QIcon icon = topt->icon; !icon.isNull()) {
					const QRectF iconRect(topt->rect.x() + 15, textRect.center().y() - topt->iconSize.height() / 2.0 + 1, topt->iconSize.width(), topt->iconSize.height());
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
					QPixmap iconPix = icon.pixmap(topt->iconSize, painter->device()->devicePixelRatio(),
												  (topt->state & State_Enabled) ? QIcon::Normal
																				: QIcon::Disabled,
												  (topt->state & State_Selected) ? QIcon::On
																				 : QIcon::Off);
#else
					QPixmap iconPix = icon.pixmap(topt->iconSize,
					                              (topt->state & State_Enabled) ? QIcon::Normal : QIcon::Disabled,
					                              (topt->state & State_Selected) ? QIcon::On : QIcon::Off);
#endif
					p->drawPixmap(static_cast<int>(iconRect.x()), static_cast<int>(iconRect.y()), iconPix);
				}
				// text
				p->setPen(ALThemeColor(m_themeMode, ALThemeType::BasicText));
				const QString text = p->fontMetrics().elidedText(topt->text, Qt::ElideRight, textRect.width());
				p->drawText(textRect, Qt::AlignVCenter | Qt::AlignLeft | Qt::TextDontClip, text);
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

QSize CALTabBarStyle::sizeFromContents(const ContentsType ct, const QStyleOption* opt, const QSize& contentsSize, const QWidget* w) const {
	switch (ct) {
		case QStyle::CT_TabBarTab: {
			return { 220, 35 };
		}
		default: {
			break;
		}
	}

	return QProxyStyle::sizeFromContents(ct, opt, contentsSize, w);
}

QRect CALTabBarStyle::subElementRect(const SubElement subElement, const QStyleOption* option, const QWidget* widget) const {
	switch (subElement) {
		case QStyle::SE_TabBarScrollLeftButton:
		case QStyle::SE_TabBarScrollRightButton: {
			return {};
		}
		default: {
			break;
		}
	}

	return QProxyStyle::subElementRect(subElement, option, widget);
}
}
