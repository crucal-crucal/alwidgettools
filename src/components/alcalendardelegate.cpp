#include "alcalendardelegate.hpp"

#include <QPainter>
#include <QPainterPath>
#include <QStyleOption>

#include "althememanager.hpp"

namespace AL {
/**
 * @brief \class CALCalendarDelegate
 * @param calendarModel pointer to the calendar model
 * @param parent pointer to the parent class
 */
CALCalendarDelegate::CALCalendarDelegate(CALCalendarModel* calendarModel, QObject* parent): QStyledItemDelegate(parent) {
	m_calendarModel = calendarModel;
	m_isTransparent = false;
	m_itemHeight = m_itemWidth = 42;
	m_nowDate = QDate::currentDate();
	m_themeMode = ALTheme->getThemeMode();
	connect(ALTheme, &CALThemeManager::sigThemeModeChanged, this, [=](const ALThemeType::ThemeMode& mode) { m_themeMode = mode; });
	connect(m_calendarModel, &CALCalendarModel::sigDisplayModeChanged, this, &CALCalendarDelegate::slotCalendarModelDisplayModeChanged);
}

CALCalendarDelegate::~CALCalendarDelegate() = default;

void CALCalendarDelegate::slotCalendarModelDisplayModeChanged() {
	switch (m_calendarModel->getDisplayMode()) {
		case ALCalendarMode::DayMode: {
			m_itemHeight = m_itemWidth = 42;
			break;
		}
		case ALCalendarMode::MonthMode:
		case ALCalendarMode::YearMode: {
			m_itemWidth = 53;
			m_itemHeight = 60;
			break;
		}
		default: {
			break;
		}
	}
}

void CALCalendarDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const {
	if (m_isTransparent) {
		return;
	}

	painter->save();
	painter->setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);
	switch (m_calendarModel->getDisplayMode()) {
		case ALCalendarMode::DayMode: {
			drawDays(painter, option, index);
			break;
		}
		case ALCalendarMode::MonthMode:
		case ALCalendarMode::YearMode: {
			drawYearOrMonth(painter, option, index);
			break;
		}
		default: {
			break;
		}
	}
	painter->restore();

	QStyledItemDelegate::paint(painter, option, index);
}

QSize CALCalendarDelegate::sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const {
	return { m_itemWidth, m_itemHeight };
}

void CALCalendarDelegate::setItemWidth(const int width) {
	m_itemWidth = width;
}

int CALCalendarDelegate::getItemWidth() const {
	return m_itemWidth;
}

void CALCalendarDelegate::setItemHeight(const int height) {
	m_itemHeight = height;
}

int CALCalendarDelegate::getItemHeight() const {
	return m_itemHeight;
}

void CALCalendarDelegate::setIsTransparent(const bool istransparent) {
	m_isTransparent = istransparent;
}

bool CALCalendarDelegate::isTransparent() const {
	return m_isTransparent;
}

void CALCalendarDelegate::drawYearOrMonth(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const {
	QRectF itemRect = option.rect;
	const auto data = index.data(Qt::UserRole).value<CALCalendarData>();
	const qreal baseRadius = m_itemWidth * 0.5;
	/// current date
	const ALCalendarMode displayMode = m_calendarModel->getDisplayMode();
	const bool bIsNow = (displayMode == ALCalendarMode::YearMode && data.year == m_nowDate.year()) || (displayMode == ALCalendarMode::MonthMode && data.year == m_nowDate.year() && data.month == m_nowDate.month());
	if (bIsNow) {
		painter->setPen(Qt::NoPen);
		painter->setBrush(ALThemeColor(m_themeMode, option.state.testFlag(QStyle::State_MouseOver) ? ALThemeType::PrimaryHover : ALThemeType::PrimaryNormal));
		painter->drawEllipse(itemRect.center(), baseRadius, baseRadius);
	} else {
		if (option.state.testFlag(QStyle::State_MouseOver)) {
			painter->setPen(Qt::NoPen);
			painter->setBrush(ALThemeColor(m_themeMode, ALThemeType::BasicHover));
			painter->drawEllipse(itemRect.center(), baseRadius, baseRadius);
		}
	}
	/// text
	painter->setPen(ALThemeColor(m_themeMode, bIsNow ? ALThemeType::BasicTextInvert : ALThemeType::BasicText));
	painter->drawText(itemRect, Qt::AlignCenter, displayMode == ALCalendarMode::YearMode ? QString::number(data.year) : QString::number(data.month));
	if (const QString desText = data.desText; !desText.isEmpty()) {
		itemRect.adjust(0, 10, 0, 0);
		QFont font = painter->font();
		font.setPixelSize(9);
		painter->setFont(font);
		painter->drawText(itemRect, Qt::AlignTop | Qt::AlignHCenter, desText);
	}
}

void CALCalendarDelegate::drawDays(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const {
	constexpr qreal penWidth = 1.5;
	const qreal baseRadius = m_itemWidth * 0.5 - penWidth;
	if (QVariant variant = index.data(Qt::UserRole); variant.isValid()) {
		QRectF itemRect = option.rect;
		const auto data = variant.value<CALCalendarData>();
		/// current date
		const bool bIsNow = (data.year == m_nowDate.year() && data.month == m_nowDate.month() && data.day == m_nowDate.day());
		if (bIsNow) {
			const QColor drawColor = ALThemeColor(m_themeMode, option.state.testFlag(QStyle::State_MouseOver) ? ALThemeType::PrimaryHover : ALThemeType::PrimaryNormal);
			if (option.state.testFlag(QStyle::State_Selected)) {
				painter->setPen(QPen(drawColor, penWidth));
				painter->drawEllipse(itemRect.center(), baseRadius, baseRadius);
				painter->setBrush(drawColor);
				painter->setPen(Qt::NoPen);
				painter->drawEllipse(itemRect.center(), baseRadius - 2 * penWidth, baseRadius - 2 * penWidth);
			} else {
				painter->setPen(Qt::NoPen);
				painter->setBrush(drawColor);
				painter->drawEllipse(itemRect.center(), baseRadius, baseRadius);
			}
		} else {
			if (option.state.testFlag(QStyle::State_Selected)) {
				painter->setPen(ALThemeColor(m_themeMode, option.state.testFlag(QStyle::State_MouseOver) ? ALThemeType::PrimaryHover : ALThemeType::PrimaryNormal));
				painter->setBrush(Qt::NoBrush);
				painter->drawEllipse(itemRect.center(), baseRadius, baseRadius);
			}
			if (option.state.testFlag(QStyle::State_MouseOver)) {
				painter->setPen(Qt::NoPen);
				painter->setBrush(ALThemeColor(m_themeMode, ALThemeType::BasicHover));
				const qreal radius = option.state.testFlag(QStyle::State_Selected) ? baseRadius - penWidth : baseRadius;
				painter->drawEllipse(itemRect.center(), radius, radius);
			}
		}
		/// text
		painter->setPen(ALThemeColor(m_themeMode, bIsNow ? ALThemeType::BasicTextInvert : ALThemeType::BasicText));
		painter->drawText(itemRect, Qt::AlignCenter, QString::number(data.day));
		if (const QString desText = data.desText; !desText.isEmpty()) {
			itemRect.adjust(0, 2 * penWidth, 0, 0);
			QFont font = painter->font();
			font.setPixelSize(9);
			painter->setFont(font);
			painter->drawText(itemRect, Qt::AlignTop | Qt::AlignHCenter, desText);
		}
	}
}

} // namespace AL
