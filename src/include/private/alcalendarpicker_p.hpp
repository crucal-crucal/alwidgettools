#pragma once

#include "alwidgettoolsdef.hpp"

namespace AL {
class CALCalendar;
class CALCalendarPicker;
class CALCalendarPickerContainer;

class CALCalendarPickerPrivate final : public QObject {
	Q_OBJECT
	Q_DISABLE_COPY(CALCalendarPickerPrivate)
	Q_DECLARE_PUBLIC(CALCalendarPicker)

public:
	explicit CALCalendarPickerPrivate(CALCalendarPicker* q, QObject* parent = nullptr);
	~CALCalendarPickerPrivate() override;

	Q_SLOT void slotCalendarPickerClicked();
	Q_SLOT void slotCanendarSelectedDateChanged();

protected:
	CALCalendarPicker* const q_ptr{ nullptr };

private:
	ALThemeType::ThemeMode themeMode{};
	qreal borderRadius{};
	CALCalendar* calendar{ nullptr };
	CALCalendarPickerContainer* calendarPickerContainer{ nullptr };
};
} // namespace AL