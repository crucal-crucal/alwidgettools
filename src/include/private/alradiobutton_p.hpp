#pragma once

#include "alwidgettoolsdef.hpp"

class CALRadioButton;
class CALRadioButtonStyle;

class CALRadioButtonPrivate : public QObject {
	Q_OBJECT
	Q_DISABLE_COPY(CALRadioButtonPrivate)
	Q_DECLARE_PUBLIC(CALRadioButton)

public:
	explicit CALRadioButtonPrivate(CALRadioButton* q, QObject* parent = nullptr);
	~CALRadioButtonPrivate() override;

	Q_SLOT void slotThemeModeChanged(const ALThemeType::ThemeMode& mode);

protected:
	CALRadioButton* const q_ptr{ nullptr };

private:
	ALThemeType::ThemeMode themeMode{};
	CALRadioButtonStyle* style{ nullptr };
};
