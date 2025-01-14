#pragma once

#include <QStyledItemDelegate>

#include "alwidgettoolsdef.hpp"

/**
 * @brief \namespace AL
 */
namespace AL {
class CALCalendarTitleDelegate final : public QStyledItemDelegate {
	Q_OBJECT

public:
	explicit CALCalendarTitleDelegate(QObject* parent = nullptr);
	~CALCalendarTitleDelegate() override;

	void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const override;
	[[nodiscard]] QSize sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const override;

private:
	ALThemeType::ThemeMode m_themeMode{};
};
}
