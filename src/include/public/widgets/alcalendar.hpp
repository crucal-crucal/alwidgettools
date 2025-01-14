#pragma once

#include <QWidget>

#include "alwidgettoolsdef.hpp"

/**
 * @brief \namespace AL
 */
namespace AL {
class CALCalendarPrivate;

class CALWIDGETTOOLS_EXPORT CALCalendar : public QWidget {
	Q_OBJECT
	Q_DECLARE_PRIVATE(CALCalendar)
	Q_PROPERTY(qreal borderRadius READ getBorderRadius WRITE setBorderRadius NOTIFY sigBorderRadiusChanged)
	Q_PROPERTY(QDate selectedDate READ getSelectedDate WRITE setSelectedDate NOTIFY sigSelectedDateChanged)
	Q_PROPERTY(QDate minimumDate READ getMinimumDate WRITE setMinimumDate NOTIFY sigMinimumDateChanged)
	Q_PROPERTY(QDate maximumDate READ getMaximumDate WRITE setMaximumDate NOTIFY sigMaximumDateChanged)

public:
	explicit CALCalendar(QWidget* parent = nullptr);
	~CALCalendar() override;

	void setBorderRadius(qreal borderRadius);
	[[nodiscard]] qreal getBorderRadius() const;

	void setSelectedDate(const QDate& selectedDate);
	[[nodiscard]] QDate getSelectedDate() const;

	void setMinimumDate(const QDate& minimumDate);
	[[nodiscard]] QDate getMinimumDate() const;

	void setMaximumDate(const QDate& maximumDate);
	[[nodiscard]] QDate getMaximumDate() const;

Q_SIGNALS:
	Q_SIGNAL void sigClicked(const QDate& date);
	Q_SIGNAL void sigBorderRadiusChanged();
	Q_SIGNAL void sigSelectedDateChanged();
	Q_SIGNAL void sigMinimumDateChanged();
	Q_SIGNAL void sigMaximumDateChanged();

protected:
	const QScopedPointer<CALCalendarPrivate> d_ptr{ nullptr };

	void paintEvent(QPaintEvent* event) override;
	void showEvent(QShowEvent* event) override;
};
}
