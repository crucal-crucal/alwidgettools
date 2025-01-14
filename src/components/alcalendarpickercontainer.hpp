#pragma once

#include <QWidget>

#include "alwidgettoolsdef.hpp"

/**
 * @brief \namespace AL
 */
namespace AL {
class CALCalendarPickerContainer final : public QWidget {
	Q_OBJECT

public:
	explicit CALCalendarPickerContainer(QWidget* parent = nullptr);
	~CALCalendarPickerContainer() override;

protected:
	void paintEvent(QPaintEvent* event) override;

private:
	ALThemeType::ThemeMode m_themeMode{};
};
}
