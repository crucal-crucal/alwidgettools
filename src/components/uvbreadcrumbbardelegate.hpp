#pragma once

#include <QStyledItemDelegate>

#include "uvwidgettoolsdef.hpp"

class CUVBreadcrumbBarDelegate final : public QStyledItemDelegate {
	Q_OBJECT

public:
	explicit CUVBreadcrumbBarDelegate(QObject* parent = nullptr);
	~CUVBreadcrumbBarDelegate() override;

	void setPressIndex(const QModelIndex& index);
	[[nodiscard]] QModelIndex getPressIndex() const;

protected:
	void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const override;

private:
	UVThemeType::ThemeMode m_themeMode{};

	QModelIndex m_pressIndex{};
};
