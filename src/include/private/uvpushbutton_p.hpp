#pragma once

#include <QColor>
#include <QObject>

#include "uvpushbutton.hpp"
#include "alwidgettoolsdef.hpp"

class CALPushButton;
class CUVToolTip;

class CUVPushButtonPrivate final : public QObject {
	Q_OBJECT
	Q_DISABLE_COPY(CUVPushButtonPrivate)
	Q_DECLARE_PUBLIC(CALPushButton)

public:
	explicit CUVPushButtonPrivate(CALPushButton* q, QObject* parent = nullptr);
	~CUVPushButtonPrivate() override;

protected:
	CALPushButton* const q_ptr{ nullptr };

private:
	ALThemeType::ThemeMode themeMode{};
	CUVToolTip* toolTip{ nullptr };
	bool isPressed{ false };
	CALPushButton::ButtonStyles customButtonStyles{ CALPushButton::ButtonStyle::Normal };
	int shadowBorderWidth{ 3 };
	int borderRadius{};
	qreal borderWidth{};
	QColor lightTextColor{};
	QColor darkTextColor{};
	QColor lightDefaultColor{};
	QColor darkDefaultColor{};
	QColor lightHoverColor{};
	QColor darkHoverColor{};
	QColor lightPressedColor{};
	QColor darkPressedColor{};
	QColor lightBorderColor{};
	QColor darkBorderColor{};
};
