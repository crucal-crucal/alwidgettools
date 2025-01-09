#pragma once

#include <QStyledItemDelegate>

#include "alwidgettoolsdef.hpp"

class E_IconDelegate final : public QStyledItemDelegate {
	Q_OBJECT

public:
	explicit E_IconDelegate(QObject* parent = nullptr);
	~E_IconDelegate() override;

protected:
	void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const override;
	[[nodiscard]] QSize sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const override;

private:
	ALThemeType::ThemeMode m_themeMode{};
};
