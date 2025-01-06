#pragma once

#include "uvwidgettoolsdef.hpp"

class CUVRadioButton;
class CUVRadioButtonStyle;

class CUVRadioButtonPrivate : public QObject {
	Q_OBJECT
	Q_DISABLE_COPY(CUVRadioButtonPrivate)
	Q_DECLARE_PUBLIC(CUVRadioButton)

public:
	explicit CUVRadioButtonPrivate(CUVRadioButton* q, QObject* parent = nullptr);
	~CUVRadioButtonPrivate() override;

	Q_SLOT void slotThemeModeChanged(const UVThemeType::ThemeMode& mode);

protected:
	CUVRadioButton* const q_ptr{ nullptr };

private:
	UVThemeType::ThemeMode themeMode{};
	CUVRadioButtonStyle* style{ nullptr };
};
