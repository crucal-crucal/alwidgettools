#pragma once

#include <QStyledItemDelegate>

#include "uvwidgettoolsdef.hpp"

class CUVSuggestDelegate final : public QStyledItemDelegate {
	Q_OBJECT

public:
	explicit CUVSuggestDelegate(QObject *parent = nullptr);
	~CUVSuggestDelegate() override;

protected:
	void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const override;
	[[nodiscard]] QSize sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const override;

private:
	UVThemeType::ThemeMode themeMode{};
};
