#include "e_icondelegate.hpp"

#include <QPainter>

#include "althememanager.hpp"

E_IconDelegate::E_IconDelegate(QObject* parent): QStyledItemDelegate(parent) {
	m_themeMode = ALTheme->getThemeMode();
	connect(ALTheme, &CALThemeManager::sigThemeModeChanged, this, [=](const ALThemeType::ThemeMode& mode) { m_themeMode = mode; });
}

E_IconDelegate::~E_IconDelegate() = default;

void E_IconDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const {
	QStyleOptionViewItem viewOption(option);
	initStyleOption(&viewOption, index);

	if (option.state.testFlag(QStyle::State_HasFocus)) {
		viewOption.state &= ~QStyle::State_HasFocus;
	}
	QStyledItemDelegate::paint(painter, viewOption, index);
	const QStringList iconList = index.data(Qt::UserRole).toStringList();
	if (iconList.count() != 2) {
		return;
	}

	const QString& iconName = iconList.at(0);
	const QString& iconValue = iconList.at(1);
	painter->save();
	painter->setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing | QPainter::SmoothPixmapTransform);
	/// icon
	painter->save();
	QFont iconFont("CALAwesome");
	iconFont.setPixelSize(22);
	painter->setFont(iconFont);
	painter->setPen(ALThemeColor(m_themeMode, ALThemeType::BasicText));
	painter->drawText(option.rect.x() + option.rect.width() / 2 - 11, option.rect.y() + option.rect.height() / 2 - 11, iconValue);
	painter->restore();
	/// text
	painter->setPen(ALThemeColor(m_themeMode, ALThemeType::BasicText));
	QFont textFont = painter->font();
	textFont.setPixelSize(13);
	painter->setFont(textFont);
	const qreal rowTextWidth = option.rect.width() * 0.8;
	if (const int subTotleRow = static_cast<int>(painter->fontMetrics().horizontalAdvance(iconName) / rowTextWidth); subTotleRow > 0) {
		QString subTitleText = iconName;
		for (int i = 0; i < subTotleRow + 1; ++i) {
			QString text = painter->fontMetrics().elidedText(subTitleText, Qt::ElideRight, static_cast<int>(rowTextWidth));
			if (text.right(3).contains("…")) {
				text = text.replace("…", subTitleText.mid(text.length() - 1, 1));
			}
			subTitleText.remove(0, text.length());
			painter->drawText(option.rect.x() + option.rect.width() / 2 - painter->fontMetrics().horizontalAdvance(text) / 2, option.rect.y() + option.rect.height() - 10 * (subTotleRow + 1 - i), text);
		}
	} else {
		painter->drawText(option.rect.x() + option.rect.width() / 2 - painter->fontMetrics().horizontalAdvance(iconName) / 2, option.rect.y() + option.rect.height() - 20, iconName);
	}
	painter->restore();
}

QSize E_IconDelegate::sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const {
	return { 100, 100 };
}
