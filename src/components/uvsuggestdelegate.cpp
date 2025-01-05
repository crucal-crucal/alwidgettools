#include "uvsuggestdelegate.hpp"

#include <QPainter>
#include <QPainterPath>

#include "uvsuggestbox_p.hpp"
#include "uvsuggestmodel.hpp"
#include "uvthememanager.hpp"

/**
 * @brief \class CUVSuggestDelegate
 * @param parent pointer to the parent class
 */
CUVSuggestDelegate::CUVSuggestDelegate(QObject* parent): QStyledItemDelegate(parent) {
	themeMode = UVTheme->getThemeMode();
	connect(UVTheme, &CUVThemeManager::sigThemeModeChanged, this, [=](const UVThemeType::ThemeMode& mode) { themeMode = mode; });
}

CUVSuggestDelegate::~CUVSuggestDelegate() = default;

void CUVSuggestDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const {
	constexpr int margin = 2;
	QStyleOptionViewItem viewOption(option);
	initStyleOption(&viewOption, index);

	const auto model = dynamic_cast<CUVSuggestModel*>(const_cast<QAbstractItemModel*>(index.model()));
	const CUVSuggestion* suggest = model->getSearchSuggestion(index.row());
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
		painter->fillPath(path, option.state.testFlag(QStyle::State_MouseOver) ? UVThemeColor(themeMode, UVThemeType::BasicSelectedHoverAlpha) : UVThemeColor(themeMode, UVThemeType::BasicSelectedAlpha));
	} else {
		if (option.state.testFlag(QStyle::State_MouseOver)) { // 覆盖时颜色
			painter->fillPath(path, UVThemeColor(themeMode, UVThemeType::BasicHoverAlpha));
		}
	}
	/// 文字
	painter->setPen(UVThemeColor(themeMode, UVThemeType::BasicText));
	painter->drawText(option.rect.x() + 57, option.rect.y() + 25, suggest->getSuggestText());
	/// 图标
	if (suggest->getAwesomeIcon() != UVIcon::CUVAweSomeIcon::None) {
		auto iconFont = QFont("CUVAwesome");
		iconFont.setPixelSize(17);
		painter->setFont(iconFont);
		painter->drawText(option.rect.x() + 11, option.rect.y() + 26, QChar(static_cast<unsigned short>(suggest->getAwesomeIcon())));
	}
	painter->restore();
}

QSize CUVSuggestDelegate::sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const {
	QSize size = QStyledItemDelegate::sizeHint(option, index);
	size.setHeight(40);
	return size;
}
