#include "alcalendarmodel.hpp"

/**
 * @brief \class CALCalendarModel
 * @param parent pointer to the parent class
 */
CALCalendarModel::CALCalendarModel(QObject* parent): QAbstractListModel(parent) {
	m_displayMode = ALCalendarMode::DayMode;
	m_minimumDate.setDate(1997, 1, 1);
	m_maximumDate.setDate(2137, 12, 31);
	initRowCount();
}

CALCalendarModel::~CALCalendarModel() = default;

void CALCalendarModel::setDisplayMode(const ALCalendarMode& mode) {
	beginResetModel();
	m_displayMode = mode;
	endResetModel();
	Q_EMIT sigDisplayModeChanged();
}

ALCalendarMode CALCalendarModel::getDisplayMode() const {
	return m_displayMode;
}

void CALCalendarModel::setMinimumDate(const QDate& date) {
	m_minimumDate = date;
	initRowCount();
}

QDate CALCalendarModel::getMinimumDate() const {
	return m_minimumDate;
}

void CALCalendarModel::setMaximumDate(const QDate& date) {
	m_maximumDate = date;
	initRowCount();
}

QDate CALCalendarModel::getMaximumDate() const {
	return m_maximumDate;
}

QModelIndex CALCalendarModel::getIndexFromDate(const QDate& date) const {
	switch (m_displayMode) {
		case ALCalendarMode::DayMode: {
			return index(static_cast<int>(m_minimumDate.daysTo(date)) + m_offset);
		}
		case ALCalendarMode::MonthMode: {
			return index((date.year() - m_minimumDate.year()) * 12 + date.month() - 1);
		}
		case ALCalendarMode::YearMode: {
			return index(date.year() - m_minimumDate.year());
		}
		default: {
			break;
		}
	}

	return {};
}

QDate CALCalendarModel::getDateFromIndex(const QModelIndex& index) const {
	return !index.isValid() || index.row() < m_offset ? QDate() : m_minimumDate.addDays(index.row() - m_offset);
}

QVariant CALCalendarModel::data(const QModelIndex& index, const int role) const {
	if (role == Qt::UserRole) {
		switch (m_displayMode) {
			case ALCalendarMode::DayMode: {
				if (index.row() >= m_offset) {
					QDate date = getDateFromIndex(index);
					return QVariant::fromValue<CALCalendarData>(CALCalendarData(date.year(), date.month(), date.day(), date.day ? QString::number(date.month()) + QObject::tr("month") : QString()));
				}
			}
			case ALCalendarMode::MonthMode: {
				const int year = m_minimumDate.year() + index.row() / 12;
				const int month = index.row() % 12 + 1;
				return QVariant::fromValue<CALCalendarData>(CALCalendarData(year, month, 1, month == 1 ? QString::number(year) : QString()));
			}
			case ALCalendarMode::YearMode: {
				return QVariant::fromValue<CALCalendarData>(CALCalendarData(m_minimumDate.year() + index.row(), 1, 1));
			}
		}
	}

	return {};
}

int CALCalendarModel::rowCount(const QModelIndex& parent) const {
	switch (m_displayMode) {
		case ALCalendarMode::DayMode: {
			return m_dayRowCount;
		}
		case ALCalendarMode::MonthMode: {
			return (m_maximumDate.year() - m_minimumDate.year() + 1) * 12;
		}
		case ALCalendarMode::YearMode: {
			return m_maximumDate.year() - m_minimumDate.year() + 1;
		}
		default: {
			break;
		}
	}

	return 0;
}

void CALCalendarModel::initRowCount() {
	m_dayRowCount = static_cast<int>(m_minimumDate.daysTo(m_maximumDate));
	m_offset = m_minimumDate.dayOfWeek();
	m_dayRowCount += m_offset + 1;
}

int CALCalendarModel::getCurrentDay(const int row) const {
	return m_minimumDate.addDays(row - m_offset).day();
}
