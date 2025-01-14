#include "alcalendartitledelegate.hpp"

#include <QPainter>

#include "althememanager.hpp"

/**
 * @brief \namespace AL
 */
namespace AL {
/**
 * @brief \class CALCalendarTitleDelegate
 * @param parent pointer to the parent class
 */
CALCalendarTitleDelegate::CALCalendarTitleDelegate(QObject* parent): QStyledItemDelegate(parent) {
	m_themeMode = ALTheme->getThemeMode();
	connect(ALTheme, &CALThemeManager::sigThemeModeChanged, this, [=](const ALThemeType::ThemeMode& mode) { m_themeMode = mode; });
}

CALCalendarTitleDelegate::~CALCalendarTitleDelegate() = default;

void CALCalendarTitleDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const {
	painter->save();
	painter->setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);
	const QRectF itemRect = option.rect;
	// text
	if (const QString title = index.data(Qt::UserRole).toString(); !title.isEmpty()) {
		painter->setPen(ALThemeColor(m_themeMode, ALThemeType::BasicText));
		QFont font = painter->font();
		font.setWeight(QFont::Bold);
		painter->setFont(font);
		painter->drawText(itemRect, Qt::AlignCenter, title);
	}
	painter->restore();

	QStyledItemDelegate::paint(painter, option, index);
}

QSize CALCalendarTitleDelegate::sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const {
	return { 42, 30 };
}
}
