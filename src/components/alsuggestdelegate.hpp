#pragma once

#include <QStyledItemDelegate>

#include "alwidgettoolsdef.hpp"

class CALSuggestDelegate final : public QStyledItemDelegate {
	Q_OBJECT

public:
	explicit CALSuggestDelegate(QObject *parent = nullptr);
	~CALSuggestDelegate() override;

protected:
	void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const override;
	[[nodiscard]] QSize sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const override;

private:
	ALThemeType::ThemeMode themeMode{};
};
