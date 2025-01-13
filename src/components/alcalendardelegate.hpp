#pragma once

#include <QStyledItemDelegate>

#include "alcalendarmodel.hpp"

namespace AL {
class CALCalendarDelegate final : public QStyledItemDelegate {
	Q_OBJECT

public:
	explicit CALCalendarDelegate(CALCalendarModel* calendarModel, QObject* parent = nullptr);
	~CALCalendarDelegate() override;

	Q_SLOT void slotCalendarModelDisplayModeChanged();

	void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const override;
	[[nodiscard]] QSize sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const override;

	void setItemWidth(int width);
	[[nodiscard]] int getItemWidth() const;

	void setItemHeight(int height);
	[[nodiscard]] int getItemHeight() const;

	void setIsTransparent(bool istransparent);
	[[nodiscard]] bool isTransparent() const;

private:
	CALCalendarModel* m_calendarModel{ nullptr };
	ALThemeType::ThemeMode m_themeMode{};
	QDate m_nowDate{};
	int m_itemWidth{};
	int m_itemHeight{};
	bool m_isTransparent{};

	void drawYearOrMonth(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const;
	void drawDays(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const;
};

} // namespace AL
