#pragma once

#include <QStyledItemDelegate>

#include "alwidgettoolsdef.hpp"

/**
 * @brief \namespace AL
 */
namespace AL {
class CALColorDisplayDelegate final : public QStyledItemDelegate {
	Q_OBJECT

public:
	explicit CALColorDisplayDelegate(QObject* parent = nullptr);
	~CALColorDisplayDelegate() override;

	void setThemeMode(const ALThemeType::ThemeMode& mode);
	[[nodiscard]] ALThemeType::ThemeMode getThemeMode() const;

	void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const override;
	[[nodiscard]] QSize sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const override;

private:
	ALThemeType::ThemeMode m_themeMode{};
};
}
