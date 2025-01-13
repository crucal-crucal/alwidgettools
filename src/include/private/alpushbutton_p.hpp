#pragma once

#include <QColor>
#include <QObject>

#include "alpushbutton.hpp"
#include "alwidgettoolsdef.hpp"

namespace AL {
class CALPushButton;
class CALToolTip;

class CALPushButtonPrivate final : public QObject {
	Q_OBJECT
	Q_DISABLE_COPY(CALPushButtonPrivate)
	Q_DECLARE_PUBLIC(CALPushButton)

public:
	explicit CALPushButtonPrivate(CALPushButton* q, QObject* parent = nullptr);
	~CALPushButtonPrivate() override;

protected:
	CALPushButton* const q_ptr{ nullptr };

private:
	ALThemeType::ThemeMode themeMode{};
	CALToolTip* toolTip{ nullptr };
	bool isPressed{ false };
	CALPushButton::ButtonStyles customButtonStyles{ CALPushButton::ButtonStyle::Normal };
	int shadowBorderWidth{ 3 };
	qreal borderRadius{};
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

} // namespace AL
