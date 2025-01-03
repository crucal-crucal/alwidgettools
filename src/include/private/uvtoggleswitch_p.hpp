#pragma once

#include <QObject>

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
	bool _isToggled{ false };
	int _margin{ 1 };
	qreal _circleCenterX{ 0 };
	qreal _circleRadius{ 0 };
	bool _isLeftButtonPress{ false };
	bool _isMousePressMove{ false };
	int _lastMouseX{ 0 };

	void _startPosAnimation(qreal startX, qreal endX, bool isToggle);
	void _startRadiusAnimation(qreal startRadius, qreal endRadius);
	void _adjustCircleCenterX();
};
