#pragma once

#include "alwidgettoolsdef.hpp"

class CUVToggleSwitch;

class CUVToggleSwitchPrivate final : public QObject {
	Q_OBJECT
	Q_DISABLE_COPY(CUVToggleSwitchPrivate)
	Q_DECLARE_PUBLIC(CUVToggleSwitch)

public:
	explicit CUVToggleSwitchPrivate(CUVToggleSwitch* q, QObject* parent = nullptr);
	~CUVToggleSwitchPrivate() override;

protected:
	CUVToggleSwitch* const q_ptr{ nullptr };

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
