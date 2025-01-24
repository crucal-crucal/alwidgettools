#include "altoolbarstyle.hpp"

#include <QPainter>
#include <QPainterPath>
#include <QStyleOptionToolButton>
#include <QtMath>
#include <QToolButton>

#include "althememanager.hpp"
#include "altoolbar.hpp"

/**
 * @brief \namespace AL
 */
namespace AL {
/**
 * @brief \class CALToolBarStyle
 * @param style pointer to the parent style
 */
CALToolBarStyle::CALToolBarStyle(QStyle* style) {
	m_themeMode = ALTheme->getThemeMode();
	connect(ALTheme, &CALThemeManager::sigThemeModeChanged, this, [=](const ALThemeType::ThemeMode& mode) { m_themeMode = mode; });
}

CALToolBarStyle::~CALToolBarStyle() = default;

void CALToolBarStyle::drawPrimitive(const PrimitiveElement pe, const QStyleOption* opt, QPainter* p, const QWidget* w) const {
	switch (pe) {
		case QStyle::PE_PanelButtonTool: {
			return;
		}
		case QStyle::PE_FrameMenu: {
			p->save();
			p->setRenderHint(QPainter::Antialiasing);
			p->setPen(ALThemeColor(m_themeMode, ALThemeType::PopupBorder));
			p->setBrush(ALThemeColor(m_themeMode, ALThemeType::DialogBase));
			p->drawRect(opt->rect);
			p->restore();
			return;
		}
		case QStyle::PE_IndicatorToolBarHandle: {
			// 拖动虚线
			const QRect handleRect = opt->rect;
			p->save();
			p->setRenderHint(QPainter::Antialiasing);
			QPen handlePen(Qt::DotLine);
			handlePen.setColor(ALThemeColor(m_themeMode, ALThemeType::PrimaryNormal));
			handlePen.setWidthF(1.8);
			p->setPen(handlePen);
			if (const auto toolBar = dynamic_cast<const CALToolBar*>(w); toolBar->orientation() == Qt::Vertical) {
				p->drawLine(handleRect.x(), handleRect.center().y(), handleRect.right(), handleRect.center().y());
			} else {
				p->drawLine(handleRect.center().x(), handleRect.y(), handleRect.center().x(), handleRect.bottom());
			}
			p->restore();
			return;
		}
		case QStyle::PE_IndicatorToolBarSeparator: {
			// 间隔符
			const QRect separatorRect = opt->rect;
			p->save();
			p->setRenderHint(QPainter::Antialiasing);
			p->setPen(Qt::NoPen);
			p->setBrush(ALThemeColor(m_themeMode, ALThemeType::PrimaryNormal));
			if (const auto toolBar = dynamic_cast<const CALToolBar*>(w); toolBar->orientation() == Qt::Vertical) {
				p->drawRect(QRectF(separatorRect.x() + separatorRect.width() * 0.1, separatorRect.center().y() - 0.9, separatorRect.width() * 0.8, 1.8));
			} else {
				p->drawRect(QRectF(separatorRect.center().x() - 0.9, separatorRect.y() + separatorRect.height() * 0.1, 1.8, separatorRect.height() * 0.8));
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

void CALToolBarStyle::drawControl(const ControlElement element, const QStyleOption* opt, QPainter* p, const QWidget* w) const {
	switch (element) {
		case QStyle::CE_ToolButtonLabel: {
			// 展开按钮
			if (const auto bopt = qstyleoption_cast<const QStyleOptionToolButton*>(opt)) {
				p->save();
				p->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform | QPainter::TextAntialiasing);
				p->setPen(Qt::NoPen);
				if (w->objectName() == "qt_toolbar_ext_button") {
					if (bopt->state.testFlag(QStyle::State_Enabled) && bopt->state.testFlag(QStyle::State_MouseOver)) {
						p->setBrush(ALThemeColor(m_themeMode, ALThemeType::BasicHoverAlpha));
						p->drawRect(bopt->rect);
					}
					// 展开图标
					p->setPen(ALThemeColor(m_themeMode, bopt->state.testFlag(QStyle::State_Enabled) ? ALThemeType::BasicText : ALThemeType::BasicTextDisable));
					QFont iconFont(ALIcon::getEnumTypeFontName(ALIcon::Awesome));
					iconFont.setPixelSize(18);
					p->setFont(iconFont);
					p->drawText(bopt->rect, Qt::AlignCenter, QChar(static_cast<unsigned short>(ALIcon::AweSomeIcon::AngleRight)));
				} else {
					if (bopt->arrowType != Qt::NoArrow) {
						break;
					}
					// background
					if (bopt->state.testFlag(QStyle::State_Enabled)) {
						if (bopt->state.testFlag(QStyle::State_Sunken)) {
							p->setBrush(ALThemeColor(m_themeMode, ALThemeType::BasicPressAlpha));
							p->drawRoundedRect(bopt->rect, 4, 4);
						} else {
							if (bopt->state.testFlag(QStyle::State_MouseOver) || bopt->state.testFlag(QStyle::State_On)) {
								p->setBrush(ALThemeColor(m_themeMode, ALThemeType::BasicHoverAlpha));
								p->drawRoundedRect(bopt->rect, 4, 4);
							}
						}
					}
					// indicator
					drawIndicator(p, bopt, w);
					// icon
					QRect contentRect = subControlRect(QStyle::CC_ToolButton, bopt, QStyle::SC_ScrollBarAddLine, w);
					QRect iconRect = contentRect;
					const int heightOffset = iconRect.height() * 0.1; // NOLINT
					iconRect.adjust(3, heightOffset, -3, -heightOffset);
					drawIcon(p, iconRect, bopt, w);
					// text
					contentRect.adjust(0, heightOffset, 0, -heightOffset);
					drawText(p, contentRect, bopt);
				}
				p->restore();
			}
			return;
		}
		case QStyle::CE_ToolBar: {
			return;
		}
		default: {
			break;
		}
	}

	QProxyStyle::drawControl(element, opt, p, w);
}

int CALToolBarStyle::pixelMetric(const PixelMetric metric, const QStyleOption* option, const QWidget* widget) const {
	switch (metric) {
		case QStyle::PM_ToolBarExtensionExtent: {
			return 16;
		}
		default: {
			break;
		}
	}

	return QProxyStyle::pixelMetric(metric, option, widget);
}

void CALToolBarStyle::drawIndicator(QPainter* painter, const QStyleOptionToolButton* bopt, const QWidget* widget) const {
	if (bopt->features.testFlag(QStyleOptionToolButton::MenuButtonPopup)) {
		const QRect indicatorRect = subControlRect(QStyle::CC_ToolButton, bopt, QStyle::SC_ScrollBarSubLine, widget);
		// indicator area
		if (bopt->state.testFlag(QStyle::State_Enabled) && bopt->activeSubControls.testFlag(QStyle::SC_ScrollBarSubLine)) {
			painter->setBrush(ALThemeColor(m_themeMode, ALThemeType::BasicIndicator));
			QPainterPath path;
			path.moveTo(indicatorRect.topLeft());
			path.lineTo(indicatorRect.right() - 4, indicatorRect.y());
			path.arcTo(QRect(indicatorRect.right() - 8, indicatorRect.y(), 8, 8), 90, -90);
			path.lineTo(indicatorRect.right(), indicatorRect.bottom() - 4);
			path.arcTo(QRect(indicatorRect.right() - 8, indicatorRect.bottom() - 8, 8, 8), 0, -90);
			path.lineTo(indicatorRect.bottomLeft());
			path.closeSubpath();
			painter->drawPath(path);
		}
		// indicator
		painter->setBrush(ALThemeColor(m_themeMode, bopt->state.testFlag(QStyle::State_Enabled) ? ALThemeType::BasicText : ALThemeType::BasicDetailsText));
		QPainterPath indicatorPath;
		const qreal indicatorHeight = qCos(30 * M_PI / 180.0) * indicatorRect.width() * 0.85;
		indicatorPath.moveTo(indicatorRect.x() + indicatorRect.width() * 0.15, indicatorRect.center().y());
		indicatorPath.lineTo(indicatorRect.right() - indicatorRect.width() * 0.15, indicatorRect.center().y());
		indicatorPath.lineTo(indicatorRect.center().x(), indicatorRect.center().y() + indicatorHeight / 2);
		indicatorPath.closeSubpath();
		painter->drawPath(indicatorPath);
	}
}

void CALToolBarStyle::drawIcon(QPainter* painter, const QRect& iconRect, const QStyleOptionToolButton* bopt, const QWidget* widget) const {
	if (bopt->toolButtonStyle != Qt::ToolButtonTextOnly) {
		const QSize iconSize = bopt->iconSize;
		const auto toolButton = dynamic_cast<const QToolButton*>(widget);
		if (!toolButton) {
			return;
		}
		const QAction* action = toolButton->defaultAction();
		if (!action) {
			return;
		}
		if (action->property(ALIcon::iconTypeProperty).toInt() == ALIcon::None) {
			// QIcon
			if (const QIcon icon = bopt->icon; !icon.isNull()) {
				const QPixmap iconPix = icon.pixmap(iconSize, bopt->state.testFlag(QStyle::State_Enabled) ? QIcon::Normal : QIcon::Disabled, bopt->state.testFlag(QStyle::State_Selected) ? QIcon::On : QIcon::Off);
				switch (bopt->toolButtonStyle) {
					case Qt::ToolButtonTextBesideIcon: {
						painter->drawPixmap(QRect(QPoint(iconRect.x(), iconRect.center().y() - iconSize.height() / 2), iconSize), iconPix);
						break;
					}
					case Qt::ToolButtonTextUnderIcon: {
						painter->drawPixmap(QRect(QPoint(iconRect.center().x() - iconSize.height() / 2, iconRect.y()), iconSize), iconPix);
						break;
					}
					case Qt::ToolButtonFollowStyle: {
						painter->drawPixmap(iconRect.x(), iconRect.y() - iconSize.height() / 2, iconPix);
						break;
					}
					default: {
						break;
					}
				}
			}
		} else {
			// CALIcon
			painter->save();
			painter->setPen(ALThemeColor(m_themeMode, ALThemeType::BasicText));
			QFont iconFont(ALIcon::getEnumTypeFontName(static_cast<ALIcon::IconType>(action->property(ALIcon::iconTypeProperty).toInt())));
			const QString iconText = action->property(ALIcon::iconProperty).toString();
			switch (bopt->toolButtonStyle) {
				case Qt::ToolButtonIconOnly:
				case Qt::ToolButtonTextBesideIcon:
				case Qt::ToolButtonFollowStyle: {
					const QRect adjustIconRect(iconRect.x(), iconRect.y(), iconSize.width(), iconRect.height());
					iconFont.setPixelSize(static_cast<int>(0.8 * std::min(adjustIconRect.width(), adjustIconRect.height())));
					painter->setFont(iconFont);
					painter->drawText(adjustIconRect, Qt::AlignCenter, iconText);
					break;
				}
				case Qt::ToolButtonTextUnderIcon: {
					const QRect adjustIconRect(iconRect.center().x() - iconSize.width() / 2, iconRect.y(), iconSize.width(), iconSize.height());
					iconFont.setPixelSize(static_cast<int>(0.8 * std::min(iconSize.width(), iconSize.height())));
					painter->setFont(iconFont);
					painter->drawText(adjustIconRect, Qt::AlignHCenter, iconText);
					break;
				}

				default: {
					break;
				}
			}
			painter->restore();
		}
	}
}

void CALToolBarStyle::drawText(QPainter* painter, const QRect& contentRect, const QStyleOptionToolButton* bopt) const {
	if (!bopt->text.isEmpty()) {
		painter->setPen(ALThemeColor(m_themeMode, ALThemeType::BasicText));
		switch (bopt->toolButtonStyle) {
			case Qt::ToolButtonTextOnly: {
				painter->drawText(contentRect, Qt::AlignCenter, bopt->text);
				break;
			}
			case Qt::ToolButtonTextBesideIcon: {
				painter->drawText(QRect(contentRect.x() + bopt->iconSize.width() + 12, contentRect.y(), contentRect.width() - bopt->iconSize.width(), contentRect.height()), Qt::AlignLeft | Qt::AlignVCenter, bopt->text);
				break;
			}
			case Qt::ToolButtonTextUnderIcon: {
				painter->drawText(contentRect, Qt::AlignBottom | Qt::AlignHCenter, bopt->text);
				break;
			}
			default: {
				break;
			}
		}
	}
}
}
