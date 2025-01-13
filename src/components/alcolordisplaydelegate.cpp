#include "alcolordisplaydelegate.hpp"

#include <QPainter>

#include "althememanager.hpp"

namespace AL {
/**
 * @brief \class CALColorDisplayDelegate
 * @param parent pointer to the parent class
 */
CALColorDisplayDelegate::CALColorDisplayDelegate(QObject* parent): QStyledItemDelegate(parent) {
	m_themeMode = ALTheme->getThemeMode();
}

CALColorDisplayDelegate::~CALColorDisplayDelegate() = default;

void CALColorDisplayDelegate::setThemeMode(const ALThemeType::ThemeMode& mode) {
	m_themeMode = mode;
}

ALThemeType::ThemeMode CALColorDisplayDelegate::getThemeMode() const {
	return m_themeMode;
}

void CALColorDisplayDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const {
	painter->save();
	painter->setRenderHint(QPainter::Antialiasing);
	const QRectF itemRect = option.rect;
	// 颜色球绘制
	if (const auto displayColor = index.data(Qt::UserRole).value<QColor>(); displayColor.isValid()) {
		painter->setPen(ALThemeColor(m_themeMode, ALThemeType::PopupBorderHover));
		painter->setBrush(displayColor);
	} else {
		QPen pen(ALThemeColor(m_themeMode, ALThemeType::PopupBorderHover));
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

QSize CALColorDisplayDelegate::sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const {
	return { 30, 30 };
}
} // namespace AL