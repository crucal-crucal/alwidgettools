#pragma once

#include <QColor>
#include <QObject>

#include "uvpushbutton.hpp"
#include "uvwidgettoolsdef.hpp"

class CUVPushButton;
class CUVToolTip;

class CUVPushButtonPrivate final : public QObject {
	Q_OBJECT
	Q_DISABLE_COPY(CUVPushButtonPrivate)
	Q_DECLARE_PUBLIC(CUVPushButton)

public:
	explicit CUVPushButtonPrivate(CUVPushButton* q, QObject* parent = nullptr);
	~CUVPushButtonPrivate() override;

protected:
	CUVPushButton* const q_ptr{ nullptr };

private:
	UVThemeType::ThemeMode themeMode{};
	CUVToolTip* toolTip{ nullptr };
	bool isPressed{ false };
	CUVPushButton::CustomButtonStyles customButtonStyles{ CUVPushButton::ButtonStyle::Normal };
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
