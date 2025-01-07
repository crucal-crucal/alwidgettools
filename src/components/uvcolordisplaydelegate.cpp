#include "uvcolordisplaydelegate.hpp"

#include <QPainter>

#include "althememanager.hpp"

/**
 * @brief \class CUVColorDisplayDelegate
 * @param parent pointer to the parent class
 */
CUVColorDisplayDelegate::CUVColorDisplayDelegate(QObject* parent): QStyledItemDelegate(parent) {
	m_themeMode = UVTheme->getThemeMode();
}

CUVColorDisplayDelegate::~CUVColorDisplayDelegate() = default;

void CUVColorDisplayDelegate::setThemeMode(const ALThemeType::ThemeMode& mode) {
	m_themeMode = mode;
}

ALThemeType::ThemeMode CUVColorDisplayDelegate::getThemeMode() const {
	return m_themeMode;
}

void CUVColorDisplayDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const {
	painter->save();
	painter->setRenderHint(QPainter::Antialiasing);
	const QRectF itemRect = option.rect;
	// 颜色球绘制
	if (const auto displayColor = index.data(Qt::UserRole).value<QColor>(); displayColor.isValid()) {
		painter->setPen(UVThemeColor(m_themeMode, ALThemeType::PopupBorderHover));
		painter->setBrush(displayColor);
	} else {
		QPen pen(UVThemeColor(m_themeMode, ALThemeType::PopupBorderHover));
		pen.setStyle(Qt::DashLine);
		painter->setPen(pen);
	}
	painter->drawEllipse(itemRect.center(), 10, 10);
	// 覆盖效果绘制
	if (option.state.testFlag(QStyle::State_MouseOver) || option.state.testFlag(QStyle::State_Selected)) {
		painter->setBrush(Qt::NoBrush);
		painter->drawEllipse(itemRect.center(), 13, 13);
	}
	painter->restore();

	QStyledItemDelegate::paint(painter, option, index);
}

QSize CUVColorDisplayDelegate::sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const {
	return { 30, 30 };
}
