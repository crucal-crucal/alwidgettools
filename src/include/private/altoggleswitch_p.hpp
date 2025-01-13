#pragma once

#include "alwidgettoolsdef.hpp"

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
	bool isToggled{ false };
	int margin{ 1 };
	qreal circleCenterX{ 0 };
	qreal circleRadius{ 0 };
	bool isLeftButtonPress{ false };
	bool isMousePressMove{ false };
	int lastMouseX{ 0 };

	void startPosAnimation(qreal startX, qreal endX, bool isToggle);
	void startRadiusAnimation(qreal startRadius, qreal endRadius);
	void adjustCircleCenterX();
};

} // namespace AL
