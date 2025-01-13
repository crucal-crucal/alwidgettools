#pragma once

#include <QAbstractListModel>
#include <QDate>
#include <QMetaType>
#include <utility>

#include "alwidgettoolsdef.hpp"

namespace AL {
enum ALCalendarMode {
	YearMode  = 0x0001,
	MonthMode = 0x0002,
	DayMode   = 0x0003
};

struct CALCalendarData : public QObjectData {
public:
	explicit CALCalendarData() {
		reset();
	}

	~CALCalendarData() override = default;

	CALCalendarData(int year, int month, int day, QString desText = QString()): year(year), month(month), day(day), desText(std::move(desText)) {
	};

	CALCalendarData(const CALCalendarData& other) {
		year = other.year;
		month = other.month;
		day = other.day;
		desText = other.desText;
	}

	void reset() {
		year = 1924;
		month = 1;
		day = 1;
		desText.clear();
	}

	int year{};
	int month{};
	int day{};
	QString desText{};
};

Q_DECLARE_METATYPE(CALCalendarData)

class CALCalendarModel final : public QAbstractListModel {
	Q_OBJECT

public:
	explicit CALCalendarModel(QObject* parent = nullptr);
	~CALCalendarModel() override;

	void setDisplayMode(const ALCalendarMode& mode);
	[[nodiscard]] ALCalendarMode getDisplayMode() const;

	void setMinimumDate(const QDate& date);
	[[nodiscard]] QDate getMinimumDate() const;

	void setMaximumDate(const QDate& date);
	[[nodiscard]] QDate getMaximumDate() const;

	[[nodiscard]] QModelIndex getIndexFromDate(const QDate& date) const;
	[[nodiscard]] QDate getDateFromIndex(const QModelIndex& index) const;
	[[nodiscard]] QVariant data(const QModelIndex& index, int role) const override;

Q_SIGNALS:
	Q_SIGNAL void sigCurrentYearMonthChanged(const QString& date);
	Q_SIGNAL void sigDisplayModeChanged();

protected:
	[[nodiscard]] int rowCount(const QModelIndex& parent) const override;

private:
	int m_offset{};
	int m_dayRowCount{};
	QDate m_minimumDate{};
	QDate m_maximumDate{};
	ALCalendarMode m_displayMode{};

	void initRowCount();
	[[nodiscard]] int getCurrentDay(int row) const;
};

} // namespace AL
