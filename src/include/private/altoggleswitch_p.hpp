#pragma once

#include "alwidgettoolsdef.hpp"

/**
 * @brief \namespace AL
 */
namespace AL {
class CALToggleSwitch;

class CALToggleSwitchPrivate final : public QObject {
	Q_OBJECT
	Q_DISABLE_COPY(CALToggleSwitchPrivate)
	Q_DECLARE_PUBLIC(CALToggleSwitch)

public:
	explicit CALToggleSwitchPrivate(CALToggleSwitch* q, QObject* parent = nullptr);
	~CALToggleSwitchPrivate() override;

protected:
	CALToggleSwitch* const q_ptr{ nullptr };

private:
	ALThemeType::ThemeMode themeMode{};
	bool isToggled{};
	int margin{};
	qreal circleCenterX{};
	qreal circleRadius{};
	bool isLeftButtonPress{};
	bool isMousePressMove{};
	int lastMouseX{};

	void startPosAnimation(qreal startX, qreal endX, bool isToggle);
	void startRadiusAnimation(qreal startRadius, qreal endRadius);
	void adjustCircleCenterX();
};
}
