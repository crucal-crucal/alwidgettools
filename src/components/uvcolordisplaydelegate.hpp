#pragma once

#include <QStyledItemDelegate>

#include "alwidgettoolsdef.hpp"

class CUVColorDisplayDelegate final : public QStyledItemDelegate {
	Q_OBJECT

public:
	explicit CUVColorDisplayDelegate(QObject* parent = nullptr);
	~CUVColorDisplayDelegate() override;

	void setThemeMode(const ALThemeType::ThemeMode& mode);
	[[nodiscard]] ALThemeType::ThemeMode getThemeMode() const;

	void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const override;
	[[nodiscard]] QSize sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const override;

private:
	ALThemeType::ThemeMode m_themeMode{};
};
