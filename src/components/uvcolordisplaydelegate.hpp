#pragma once

#include <QStyledItemDelegate>

#include "uvwidgettoolsdef.hpp"

class CUVColorDisplayDelegate final : public QStyledItemDelegate {
	Q_OBJECT

public:
	explicit CUVColorDisplayDelegate(QObject* parent = nullptr);
	~CUVColorDisplayDelegate() override;

	void setThemeMode(const UVThemeType::ThemeMode& mode);
	[[nodiscard]] UVThemeType::ThemeMode getThemeMode() const;

	void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const override;
	[[nodiscard]] QSize sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const override;

private:
	UVThemeType::ThemeMode m_themeMode{};
};
