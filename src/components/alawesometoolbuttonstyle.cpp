#include "alawesometoolbuttonstyle.hpp"

#include <QPainter>
#include <QPainterPath>
#include <QStyleOption>
#include <QtMath>

#include "althememanager.hpp"

namespace AL {
/**
 * @brief \class CALAwesomeToolButtonStyle
 * @param style pointer to the parent style
 */
CALAwesomeToolButtonStyle::CALAwesomeToolButtonStyle(QStyle* style) {
	m_isSelected = false;
	m_isTransparent = true;
	m_expandIconRotate = 0;
	m_borderRadius = 4;
	m_themeMode = ALTheme->getThemeMode();
	connect(ALTheme, &CALThemeManager::sigThemeModeChanged, this, [=](const ALThemeType::ThemeMode& mode) { m_themeMode = mode; });
}

CALAwesomeToolButtonStyle::~CALAwesomeToolButtonStyle() = default;

void CALAwesomeToolButtonStyle::drawComplexControl(const ComplexControl cc, const QStyleOptionComplex* opt, QPainter* p, const QWidget* widget) const {
	switch (cc) {
		case QStyle::CC_ToolButton: {
			// 内容绘制
			if (const auto bopt = qstyleoption_cast<const QStyleOptionToolButton*>(opt)) {
				if (bopt->arrowType != Qt::NoArrow) {
					break;
				}
				QRect toolButtonRect = bopt->rect;
				if (!m_isTransparent) {
					// 边框距离
					toolButtonRect.adjust(1, 1, -1, -1);
				}
				p->save();
				p->setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing | QPainter::SmoothPixmapTransform);
				p->setPen(m_isTransparent ? Qt::transparent : ALThemeColor(m_themeMode, ALThemeType::BasicBorder));
				/// 背景绘制
				if (bopt->state.testFlag(QStyle::State_Enabled)) {                                                                        // 启用状态
					if (bopt->state.testFlag(QStyle::State_Sunken)) {                                                                     // 按下状态
						p->setBrush(ALThemeColor(m_themeMode, m_isTransparent ? ALThemeType::BasicPressAlpha : ALThemeType::BasicPress)); // 按下状态颜色
						p->drawRoundedRect(toolButtonRect, m_borderRadius, m_borderRadius);
					} else {
						if (m_isSelected) {
							p->setBrush(ALThemeColor(m_themeMode, m_isTransparent ? ALThemeType::BasicSelectedAlpha : ALThemeType::BasicHover));
							p->drawRoundedRect(toolButtonRect, m_borderRadius, m_borderRadius);
						} else {
							if (bopt->state.testFlag(QStyle::State_MouseOver) || bopt->state.testFlag(QStyle::State_On)) {
								p->setBrush(ALThemeColor(m_themeMode, m_isTransparent ? ALThemeType::BasicHoverAlpha : ALThemeType::BasicHover)); // 悬停状态颜色
								p->drawRoundedRect(toolButtonRect, m_borderRadius, m_borderRadius);
							} else if (!m_isTransparent) {
								p->setBrush(ALThemeColor(m_themeMode, ALThemeType::BasicBase)); // 正常状态颜色
								p->drawRoundedRect(toolButtonRect, m_borderRadius, m_borderRadius);
								// 底边线绘制
								p->setPen(ALThemeColor(m_themeMode, ALThemeType::BasicBaseLine));
								p->drawLine(toolButtonRect.x() + m_borderRadius, toolButtonRect.y() + toolButtonRect.height(), toolButtonRect.x() + toolButtonRect.width() - m_borderRadius, toolButtonRect.y() + toolButtonRect.height());
							}
						}
					}
				}
				// 指示器绘制
				drawIndicator(p, bopt, widget);
				// 图标绘制
				QRect contentRect = subControlRect(cc, bopt, QStyle::SC_ScrollBarAddLine, widget);
				const int heightOffset = static_cast<int>(contentRect.height() * 0.05);
				contentRect.adjust(0, heightOffset, 0, -heightOffset);
				drawIcon(p, contentRect, bopt, widget);
				// 文字绘制
				drawText(p, contentRect, bopt);
				p->restore();
			}
			return;
		}
		default: {
			break;
		}
	}

	QProxyStyle::drawComplexControl(cc, opt, p, widget);
}

QSize CALAwesomeToolButtonStyle::sizeFromContents(const ContentsType ct, const QStyleOption* opt, const QSize& contentsSize, const QWidget* w) const {
	if (ct == QStyle::CT_ToolButton) {
		if (const auto bopt = qstyleoption_cast<const QStyleOptionToolButton*>(opt)) {
			QSize toolButtonSize = QProxyStyle::sizeFromContents(ct, opt, contentsSize, w);
			if (bopt->features.testFlag(QStyleOptionToolButton::HasMenu) && !bopt->features.testFlag(QStyleOptionToolButton::MenuButtonPopup)) {
				toolButtonSize.setWidth(toolButtonSize.width() + m_contentMargin + static_cast<int>(0.65 * std::min(bopt->iconSize.width(), bopt->iconSize.height())));
			}
			return toolButtonSize;
		}
	}

	return QProxyStyle::sizeFromContents(ct, opt, contentsSize, w);
}

void CALAwesomeToolButtonStyle::drawIndicator(QPainter* painter, const QStyleOptionToolButton* bopt, const QWidget* widget) const {
	if (bopt->features.testFlag(QStyleOptionToolButton::MenuButtonPopup)) {
		const QRect indicatorRect = subControlRect(QStyle::CC_ToolButton, bopt, QStyle::SC_ScrollBarSubLine, widget);
		// 指示器区域
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
		// 指示器
		painter->setBrush(ALThemeColor(m_themeMode, bopt->state.testFlag(QStyle::State_Enabled) ? ALThemeType::BasicText : ALThemeType::BasicTextDisable));
		QPainterPath indicatorPath{};
		const qreal indicatorHeight = qCos(30 * M_PI / 180.0) * indicatorRect.width() * 0.85;
		indicatorPath.moveTo(indicatorRect.x() + indicatorRect.width() * 0.15, indicatorRect.center().y());
		indicatorPath.lineTo(indicatorRect.right() - indicatorRect.width() * 0.15, indicatorRect.center().y());
		indicatorPath.lineTo(indicatorRect.center().x(), indicatorRect.center().y() + indicatorHeight / 2);
		indicatorPath.closeSubpath();
		painter->drawPath(indicatorPath);
	} else if (bopt->features.testFlag(QStyleOptionToolButton::HasMenu)) {
		// 展开指示器
		const QSize iconSize = bopt->iconSize;
		painter->save();
		const QRect toolButtonRect = bopt->rect;
		QFont iconFont("CALAwesome");
		iconFont.setPixelSize(0.75 * std::min(iconSize.width(), iconSize.height())); // NOLINT
		painter->setFont(iconFont);
		const int indicatorWidth = painter->fontMetrics().horizontalAdvance(QChar(static_cast<unsigned short>(ALIcon::AweSomeIcon::AngleDown)));
		const QRect expandIconRect(toolButtonRect.right() - m_contentMargin - indicatorWidth, toolButtonRect.y() + 1, indicatorWidth, toolButtonRect.height());
		painter->setPen(ALThemeColor(m_themeMode, ALThemeType::BasicText));
		painter->translate(expandIconRect.center().x(), expandIconRect.y() + static_cast<qreal>(expandIconRect.height()) / 2);
		painter->rotate(m_expandIconRotate);
		painter->translate(-expandIconRect.center().x() - 1, -expandIconRect.y() - static_cast<qreal>(expandIconRect.height()) / 2);
		painter->drawText(expandIconRect, Qt::AlignCenter, QChar(static_cast<unsigned short>(ALIcon::AweSomeIcon::AngleDown)));
		painter->restore();
	}
}

void CALAwesomeToolButtonStyle::drawIcon(QPainter* painter, QRect iconRect, const QStyleOptionToolButton* bopt, const QWidget* widget) const {
	if (bopt->toolButtonStyle != Qt::ToolButtonTextOnly) {
		const QSize iconSize = bopt->iconSize;
		if (widget->property("CALIconType").toString().isEmpty()) {
			// 绘制QIcon
			if (const QIcon icon = bopt->icon; !icon.isNull()) {
				const QPixmap iconPix = icon.pixmap(iconSize, (bopt->state & State_Enabled) ? QIcon::Normal : QIcon::Disabled, (bopt->state & State_Selected) ? QIcon::On : QIcon::Off);
				switch (bopt->toolButtonStyle) {
					case Qt::ToolButtonIconOnly: {
						painter->drawPixmap(QRect(QPoint(iconRect.x(), iconRect.center().y() - iconSize.height() / 2), iconSize), iconPix);
						break;
					}
					case Qt::ToolButtonFollowStyle:
					case Qt::ToolButtonTextBesideIcon: {
						painter->drawPixmap(QRect(QPoint(iconRect.x() + m_contentMargin, iconRect.center().y() - iconSize.height() / 2), iconSize), iconPix);
						break;
					}
					case Qt::ToolButtonTextUnderIcon: {
						if (bopt->features.testFlag(QStyleOptionToolButton::HasMenu) && !bopt->features.testFlag(QStyleOptionToolButton::MenuButtonPopup)) {
							iconRect.setRight(iconRect.right() - static_cast<int>(calculateExpandIndicatorWidth(bopt, painter)));
						}
						painter->drawPixmap(QRect(QPoint(iconRect.center().x() - iconSize.width() / 2, iconRect.y()), iconSize), iconPix);
						break;
					}
					default: {
						break;
					}
				}
			}
		} else {
			// 绘制AwesomeIcon
			painter->save();
			painter->setPen(ALThemeColor(m_themeMode, bopt->state.testFlag(QStyle::State_Enabled) ? ALThemeType::BasicText : ALThemeType::BasicTextDisable));
			QFont iconFont("CALAwesome");
			switch (bopt->toolButtonStyle) {
				case Qt::ToolButtonIconOnly: {
					iconFont.setPixelSize(0.75 * std::min(iconSize.width(), iconSize.height())); // NOLINT
					painter->setFont(iconFont);
					painter->drawText(iconRect, Qt::AlignCenter, widget->property("CALIconType").toString());
					break;
				}
				case Qt::ToolButtonFollowStyle:
				case Qt::ToolButtonTextBesideIcon: {
					const QRect adjustIconRect(iconRect.x() + m_contentMargin, iconRect.y(), iconSize.width(), iconRect.height());
					iconFont.setPixelSize(0.75 * std::min(iconSize.width(), iconSize.height())); // NOLINT
					painter->setFont(iconFont);
					painter->drawText(adjustIconRect, Qt::AlignCenter, widget->property("CALIconType").toString());
					break;
				}
				case Qt::ToolButtonTextUnderIcon: {
					if (bopt->features.testFlag(QStyleOptionToolButton::HasMenu) && !bopt->features.testFlag(QStyleOptionToolButton::MenuButtonPopup)) {
						iconRect.setRight(iconRect.right() - calculateExpandIndicatorWidth(bopt, painter)); // NOLINT
					}
					const QRect adjustIconRect(iconRect.center().x() - iconSize.width() / 2, iconRect.y() + 0.2 * std::min(iconSize.width(), iconSize.height()), iconSize.width(), iconSize.height()); // NOLINT
					iconFont.setPixelSize(0.8 * std::min(iconSize.width(), iconSize.height()));                                                                                                        // NOLINT
					painter->setFont(iconFont);
					painter->drawText(adjustIconRect, Qt::AlignHCenter, widget->property("CALIconType").toString());
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

void CALAwesomeToolButtonStyle::drawText(QPainter* painter, QRect contentRect, const QStyleOptionToolButton* bopt) const {
	if (!bopt->text.isEmpty()) {
		painter->setPen(ALThemeColor(m_themeMode, bopt->state.testFlag(QStyle::State_Enabled) ? ALThemeType::BasicText : ALThemeType::BasicTextDisable));
		switch (bopt->toolButtonStyle) {
			case Qt::ToolButtonTextOnly: {
				contentRect.setLeft(contentRect.left() + m_contentMargin);
				painter->drawText(contentRect, Qt::AlignLeft | Qt::AlignVCenter, bopt->text);
				break;
			}
			case Qt::ToolButtonTextBesideIcon: {
				painter->drawText(QRect(contentRect.x() + m_contentMargin * 2 + bopt->iconSize.width(), contentRect.y(), contentRect.width() - bopt->iconSize.width(), contentRect.height()), Qt::AlignLeft | Qt::AlignVCenter, bopt->text);
				break;
			}
			case Qt::ToolButtonTextUnderIcon: {
				if (bopt->features.testFlag(QStyleOptionToolButton::HasMenu) && !bopt->features.testFlag(QStyleOptionToolButton::MenuButtonPopup)) {
					contentRect.setLeft(contentRect.left() + m_contentMargin);
					painter->drawText(contentRect, Qt::AlignBottom | Qt::AlignLeft, bopt->text);
				} else {
					painter->drawText(contentRect, Qt::AlignBottom | Qt::AlignHCenter, bopt->text);
				}
				break;
			}
			case Qt::ToolButtonFollowStyle: { // NOLINT
				break;
			}
			default: {
				break;
			}
		}
	}
}

qreal CALAwesomeToolButtonStyle::calculateExpandIndicatorWidth(const QStyleOptionToolButton* bopt, QPainter* painter) {
	// 展开指示器
	const QSize iconSize = bopt->iconSize;
	painter->save();
	QFont iconFont("CALAwesome");
	iconFont.setPixelSize(0.75 * std::min(iconSize.width(), iconSize.height())); // NOLINT
	painter->setFont(iconFont);
	const int indicatorWidth = painter->fontMetrics().horizontalAdvance(QChar(static_cast<unsigned short>(ALIcon::AweSomeIcon::AngleDown)));
	painter->restore();
	return indicatorWidth;
}
} // namespace AL