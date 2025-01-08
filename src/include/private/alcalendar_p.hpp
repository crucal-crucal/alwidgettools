#pragma once

#include <QDate>
#include <QPixmap>

#include "alwidgettoolsdef.hpp"

class CALCalendar;
class CALBaseListView;
class CALCalendarModel;
class CALCalendarTitleModel;
class CALCalendarDelegate;
class CALCalendarTitleDelegate;
class CALAwesomeToolButton;

class CALCalendarPrivate final : public QObject {
	Q_OBJECT
	Q_DISABLE_COPY(CALCalendarPrivate)
	Q_DECLARE_PUBLIC(CALCalendar)
	Q_PROPERTY(qreal zoomRatio READ getZoomRatio WRITE setZoomRatio NOTIFY sigZoomRatioChanged)
	Q_PROPERTY(qreal pixOpacity READ getPixOpacity WRITE setPixOpacity NOTIFY sigPixOpacityChanged)

public:
	explicit CALCalendarPrivate(CALCalendar* q, QObject* parent = nullptr);
	~CALCalendarPrivate() override;

	void setZoomRatio(qreal zoomRatio);
	[[nodiscard]] qreal getZoomRatio() const;

	void setPixOpacity(qreal pixOpacity);
	[[nodiscard]] qreal getPixOpacity() const;

	Q_SLOT void slotSwitchButtonClicked();
	Q_SLOT void slotCalendarViewClicked(const QModelIndex& index);
	Q_SLOT void slotUpButtonClicked() const;
	Q_SLOT void slotDownButtonClicked() const;
	Q_SLOT void slotUpdateSwitchButtonText() const;

Q_SIGNALS:
	Q_SIGNAL void sigZoomRatioChanged();
	Q_SIGNAL void sigPixOpacityChanged();

protected:
	CALCalendar* const q_ptr{ nullptr };

private:
	QPixmap oldCalendarViewPix{};
	QPixmap newCalendarViewPix{};
	int lastSelectedYear{};
	int lastSelectedMonth{};
	bool isSwitchAnimationFinished{};
	bool isDrawNewPix{};
	QDate selectedDate{};
	qreal borderRadius{};
	qreal borderWidth{};
	qreal pixOpacity{};
	qreal zoomRatio{};
	ALThemeType::ThemeMode themeMode{};
	CALBaseListView* calendarView{ nullptr };
	CALCalendarModel* calendarModel{ nullptr };
	CALCalendarDelegate* calendarDelegate{ nullptr };
	CALBaseListView* calendarTitleView{ nullptr };
	CALCalendarTitleModel* calendarTitleModel{ nullptr };
	CALCalendarTitleDelegate* calendarTitleDelegate{ nullptr };
	CALAwesomeToolButton* modeSwitchButton{ nullptr };
	CALAwesomeToolButton* upButton{ nullptr };
	CALAwesomeToolButton* downButton{ nullptr };

	void scrollToDate(const QDate& date) const;
	void doSwitchAnimation(bool isZoomIn);
};
