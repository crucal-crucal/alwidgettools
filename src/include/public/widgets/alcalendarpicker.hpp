#pragma once

#include <QPushButton>

#include "alwidgettoolsdef.hpp"

namespace AL {
class CALCalendarPickerPrivate;

class CALWIDGETTOOLS_EXPORT CALCalendarPicker : public QPushButton {
	Q_OBJECT
	Q_DECLARE_PRIVATE(CALCalendarPicker)
	Q_PROPERTY(qreal borderRadius READ getBorderRadius WRITE setBorderRadius NOTIFY sigBorderRadiusChanged)

public:
	explicit CALCalendarPicker(QWidget* parent = nullptr);
	~CALCalendarPicker() override;

	void setSelectedDate(const QDate& date);
	[[nodiscard]] QDate getSelectedDate() const;

	void setBorderRadius(qreal borderRadius);
	[[nodiscard]] qreal getBorderRadius() const;

Q_SIGNALS:
	Q_SIGNAL void sigBorderRadiusChanged();
	Q_SIGNAL void sigSelectedDateChanged(const QDate& date);

protected:
	const QScopedPointer<CALCalendarPickerPrivate> d_ptr{ nullptr };

	void paintEvent(QPaintEvent* event) override;
	void showEvent(QShowEvent* event) override;
};
} // namespace AL