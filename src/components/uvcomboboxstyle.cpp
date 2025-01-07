#include "uvcomboboxstyle.hpp"

#include <QPainter>
#include <QPainterPath>
#include <QStyleOption>
#include <QWidget>

#include "althememanager.hpp"

/**
 * @brief \class CUVComboBoxStyle
 * @param style pointer to the parent style
 */
CUVComboBoxStyle::CUVComboBoxStyle(QStyle* style): QProxyStyle(style) {
	m_expandIconRotate = 0;
	m_expandMarkWidth = 0;
	m_shadowBorderWidth = 6;
	m_themeMode = UVTheme->getThemeMode();
	connect(UVTheme, &CUVThemeManager::sigThemeModeChanged, this, [=](const ALThemeType::ThemeMode& mode) { m_themeMode = mode; });
}

CUVComboBoxStyle::~CUVComboBoxStyle() = default;

void CUVComboBoxStyle::drawPrimitive(const PrimitiveElement element, const QStyleOption* option, QPainter* painter, const QWidget* widget) const {
	switch (element) {
		case QStyle::PE_Widget: {
			return;
		}
#ifndef Q_OS_WIN
		case PE_PanelMenu:
		{
			return;
		}
		case PE_IndicatorArrowDown:
		{
			return;
		}
#endif
		default: {
			break;
		}
	}
	QProxyStyle::drawPrimitive(element, option, painter, widget);
}

void CUVComboBoxStyle::drawControl(const ControlElement element, const QStyleOption* option, QPainter* painter, const QWidget* widget) const {
	switch (element) {
		case QStyle::CE_ComboBoxLabel: {
			return;
		}
		case QStyle::CE_ShapedFrame: {
			// container区域
			if (widget->objectName() == "CUVComboBoxContainer") {
				const QRect viewRect = option->rect;
				painter->save();
				painter->setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);
				UVTheme->drawEffectShadow(painter, viewRect, m_shadowBorderWidth, 6);
				const QRect foregroundRect(viewRect.x() + m_shadowBorderWidth, viewRect.y(), viewRect.width() - 2 * m_shadowBorderWidth, viewRect.height() - m_shadowBorderWidth);
				painter->setPen(UVThemeColor(m_themeMode, ALThemeType::PopupBorder));
				painter->setBrush(UVThemeColor(m_themeMode, ALThemeType::PopupBase));
				painter->drawRoundedRect(foregroundRect, 6, 6);
				painter->restore();
			}
			return;
		}
		case QStyle::CE_ItemViewItem: {
			// 覆盖高亮
			if (const auto vopt = qstyleoption_cast<const QStyleOptionViewItem*>(option)) {
				constexpr int margin = 2;
				painter->save();
				painter->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
				painter->setPen(Qt::NoPen);
				QPainterPath path;
				QRect optionRect = option->rect;
				optionRect.adjust(margin, margin, -margin, -margin);
				path.addRoundedRect(optionRect, 5, 5);
				if (option->state.testFlag(QStyle::State_Selected)) {
					if (option->state.testFlag(QStyle::State_MouseOver)) {
						// 选中且鼠标悬停时
						painter->setBrush(UVThemeColor(m_themeMode, ALThemeType::BasicSelectedHoverAlpha));
						painter->drawPath(path);
					} else {
						// 选中但鼠标未悬停
						painter->setBrush(UVThemeColor(m_themeMode, ALThemeType::BasicSelectedAlpha));
						painter->drawPath(path);
					}
					painter->setPen(Qt::NoPen);
					painter->setBrush(UVThemeColor(m_themeMode, ALThemeType::PrimaryNormal));
					painter->drawRoundedRect(QRectF(optionRect.x() + 3, optionRect.y() + optionRect.height() * 0.2, 3, optionRect.height() - optionRect.height() * 0.4), 2, 2);
				} else {
					if (option->state.testFlag(QStyle::State_MouseOver)) {
						// 未选中但鼠标悬停时
						painter->setBrush(UVThemeColor(m_themeMode, ALThemeType::BasicHoverAlpha));
						painter->drawPath(path);
					}
				}
#if 0
				painter->setPen(UVThemeColor(m_themeMode, UVThemeType::BasicText));
				// 计算勾选图标的绘制区域
				const int checkSize = static_cast<int>(qMin(optionRect.width(), optionRect.height()) * 0.5); // 图标大小为项目高度的一半
				const auto checkRect = QRect(optionRect.x() + 5,
				                             optionRect.y() + (optionRect.height() - checkSize) / 2,
				                             checkSize,
				                             checkSize);
				// 绘制勾选图标
				QPainterPath checkPath;
				checkPath.moveTo(checkRect.left() + checkRect.width() * 0.2, checkRect.top() + checkRect.height() * 0.5);
				checkPath.lineTo(checkRect.left() + checkRect.width() * 0.4, checkRect.bottom() - checkRect.height() * 0.25);
				checkPath.lineTo(checkRect.right() - checkRect.width() * 0.2, checkRect.top() + checkRect.height() * 0.3);
				painter->drawPath(checkPath);
#endif
				// 文字绘制
				painter->setPen(UVThemeColor(m_themeMode, ALThemeType::BasicText));
				painter->drawText(QRect(option->rect.x() + 25, option->rect.y(), option->rect.width() - 25, option->rect.height()), Qt::AlignVCenter, vopt->text);
				painter->restore();
			}
			return;
		}
		default: {
			break;
		}
	}
	QProxyStyle::drawControl(element, option, painter, widget);
}

void CUVComboBoxStyle::drawComplexControl(const ComplexControl control, const QStyleOptionComplex* option, QPainter* painter, const QWidget* widget) const {
	switch (control) {
		case QStyle::CC_ComboBox: {
			// 主体显示绘制
			if (const auto copt = qstyleoption_cast<const QStyleOptionComboBox*>(option)) {
				painter->save();
				painter->setRenderHints(QPainter::SmoothPixmapTransform | QPainter::Antialiasing | QPainter::TextAntialiasing);
				// 背景绘制
				const bool isEnabled = copt->state.testFlag(QStyle::State_Enabled);
				painter->setPen(UVThemeColor(m_themeMode, ALThemeType::BasicBorder));
				painter->setBrush(UVThemeColor(m_themeMode, isEnabled ? copt->state.testFlag(QStyle::State_MouseOver) ? ALThemeType::BasicHover : ALThemeType::BasicBase : ALThemeType::BasicDisable));
				QRect comboBoxRect = copt->rect;
				comboBoxRect.adjust(m_shadowBorderWidth, 1, -m_shadowBorderWidth, -1);
				painter->drawRoundedRect(comboBoxRect, 3, 3);
				// 底边线绘制
				painter->setPen(UVThemeColor(m_themeMode, ALThemeType::BasicBaseLine));
				painter->drawLine(comboBoxRect.x() + 3, comboBoxRect.y() + comboBoxRect.height(), comboBoxRect.x() + comboBoxRect.width() - 3, comboBoxRect.y() + comboBoxRect.height());
				// 文字绘制
				const QRect textRect = subControlRect(QStyle::CC_ComboBox, copt, QStyle::SC_ScrollBarSubLine, widget);
				painter->setPen(UVThemeColor(m_themeMode, isEnabled ? ALThemeType::BasicText : ALThemeType::BasicTextDisable));
				painter->drawText(textRect, Qt::AlignVCenter | Qt::AlignLeft, copt->currentText);
				// 展开指示器绘制
				painter->setPen(Qt::NoPen);
				painter->setBrush(UVThemeColor(m_themeMode, ALThemeType::PrimaryNormal));
				painter->drawRoundedRect(QRectF(comboBoxRect.center().x() - m_expandMarkWidth, comboBoxRect.height() - 3, m_expandMarkWidth * 2, 3), 2, 2);
				// 展开图标绘制
				if (const QRect expandIconRect = subControlRect(QStyle::CC_ComboBox, copt, QStyle::SC_ScrollBarAddPage, widget); expandIconRect.isValid()) {
					QFont iconFont("CUVAwesome");
					iconFont.setPixelSize(17);
					painter->setFont(iconFont);
					painter->setPen(UVThemeColor(m_themeMode, isEnabled ? ALThemeType::BasicText : ALThemeType::BasicTextDisable));
					painter->translate(expandIconRect.x() + expandIconRect.width() / 2.0, expandIconRect.y() + expandIconRect.height() / 2.0);
					painter->rotate(m_expandIconRotate);
					painter->translate(-expandIconRect.x() - expandIconRect.width() / 2.0, -expandIconRect.y() - expandIconRect.height() / 2.0);
					painter->drawText(expandIconRect, Qt::AlignCenter, QChar(static_cast<unsigned short>(ALIcon::AweSomeIcon::AngleDown)));
				}
				painter->restore();
			}
			return;
		}
		default: {
			break;
		}
	}

	QProxyStyle::drawComplexControl(control, option, painter, widget);
}

QRect CUVComboBoxStyle::subControlRect(const ComplexControl cc, const QStyleOptionComplex* opt, const SubControl sc, const QWidget* widget) const {
	switch (cc) {
		case QStyle::CC_ComboBox: {
			switch (sc) {
				case QStyle::SC_ScrollBarSubLine: {
					// 文字区域
					QRect textRect = QProxyStyle::subControlRect(cc, opt, sc, widget);
					textRect.setLeft(16);
					textRect.setRight(textRect.right() - 15);
					return textRect;
				}
				case QStyle::SC_ScrollBarAddPage: {
					// 展开图标区域
					QRect expandIconRect = QProxyStyle::subControlRect(cc, opt, sc, widget);
					expandIconRect.setLeft(expandIconRect.left() - 30);
					return expandIconRect;
				}
				default: {
					break;
				}
			}
			break;
		}
		default: {
			break;
		}
	}

	return QProxyStyle::subControlRect(cc, opt, sc, widget);
}

QSize CUVComboBoxStyle::sizeFromContents(const ContentsType type, const QStyleOption* option, const QSize& size, const QWidget* widget) const {
	switch (type) {
		case QStyle::CT_ItemViewItem: {
			QSize itemSize = QProxyStyle::sizeFromContents(type, option, size, widget);
			itemSize.setHeight(35);
			return itemSize;
		}
		case QStyle::CT_ComboBox: {
			QSize comboBoxSize = QProxyStyle::sizeFromContents(type, option, size, widget);
			comboBoxSize.setWidth(comboBoxSize.width() + 26);
			return comboBoxSize;
		}
		default: {
			break;
		}
	}
	return QProxyStyle::sizeFromContents(type, option, size, widget);
}

void CUVComboBoxStyle::setExpandIconRotate(const qreal ExpandIconRotate) {
	m_expandIconRotate = ExpandIconRotate;
	Q_EMIT sigExpandIconRotateChanged();
}

qreal CUVComboBoxStyle::getExpandIconRotate() const {
	return m_expandIconRotate;
}

void CUVComboBoxStyle::setExpandMarkWidth(const qreal ExpandMarkWidth) {
	m_expandMarkWidth = ExpandMarkWidth;
	Q_EMIT sigExpandMarkWidthChanged();
}

qreal CUVComboBoxStyle::getExpandMarkWidth() const {
	return m_expandMarkWidth;
}
