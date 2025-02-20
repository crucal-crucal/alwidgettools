#include "alsuggestdelegate.hpp"

#include <QPainter>
#include <QPainterPath>
#include <QDebug>

#include "alsuggestbox_p.hpp"
#include "alsuggestmodel.hpp"
#include "althememanager.hpp"

/**
 * @brief \namespace AL
 */
namespace AL {
/**
 * @brief \class CALSuggestDelegate
 * @param parent pointer to the parent class
 */
CALSuggestDelegate::CALSuggestDelegate(QObject* parent): QStyledItemDelegate(parent) {
	themeMode = ALTheme->getThemeMode();
	connect(ALTheme, &CALThemeManager::sigThemeModeChanged, this, [this](const ALThemeType::ThemeMode& mode) { themeMode = mode; });
}

CALSuggestDelegate::~CALSuggestDelegate() = default;

void CALSuggestDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const {
	constexpr int margin = 2;
	QStyleOptionViewItem viewOption(option);
	initStyleOption(&viewOption, index);

	const auto model = dynamic_cast<CALSuggestModel*>(const_cast<QAbstractItemModel*>(index.model()));
	const CALSuggestion* suggest = model->getSearchSuggestion(index.row());
	if (option.state.testFlag(QStyle::State_HasFocus)) {
		viewOption.state &= ~QStyle::State_HasFocus;
	}
	painter->save();
	painter->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform | QPainter::TextAntialiasing);
	QPainterPath path;
	QRect optionRect = option.rect;
	optionRect.adjust(margin * 2, margin, -margin * 2, -margin);
	path.addRoundedRect(optionRect, 8, 8);
	if (option.state.testFlag(QStyle::State_Selected)) { // 选中
		painter->fillPath(path, option.state.testFlag(QStyle::State_MouseOver) ? ALThemeColor(themeMode, ALThemeType::BasicSelectedHoverAlpha) : ALThemeColor(themeMode, ALThemeType::BasicSelectedAlpha));
	} else {
		if (option.state.testFlag(QStyle::State_MouseOver)) { // 覆盖时颜色
			painter->fillPath(path, ALThemeColor(themeMode, ALThemeType::BasicHoverAlpha));
		}
	}
	/// 文字
	painter->setPen(ALThemeColor(themeMode, ALThemeType::BasicText));
	painter->drawText(option.rect.x() + 57, option.rect.y() + 25, suggest->getSuggestText());
	/// 图标
	const QString iconFontFamily = ALIcon::getEnumTypeFontName(suggest->getIconType());
	if (suggest->getIconType() != ALIcon::None && iconFontFamily != ALIcon::errFontFamily) {
		painter->save();
		QFont iconFont(iconFontFamily);
		iconFont.setPixelSize(17);
		painter->setFont(iconFont);
		painter->drawText(option.rect.x() + 11, option.rect.y() + 26, suggest->property(ALIcon::iconProperty).toString());
		painter->restore();
	}
	painter->restore();
}

QSize CALSuggestDelegate::sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const {
	QSize size = QStyledItemDelegate::sizeHint(option, index);
	size.setHeight(40);
	return size;
}
}
