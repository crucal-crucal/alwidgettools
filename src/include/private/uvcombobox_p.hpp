#pragma once

#include <QObject>

#include "uvwidgettoolsdef.hpp"

class CUVComboBox;
class CUVComboBoxStyle;

class CUVComboBoxPrivate final : public QObject {
	Q_OBJECT
	Q_DISABLE_COPY(CUVComboBoxPrivate)
	Q_DECLARE_PUBLIC(CUVComboBox)

public:
	explicit CUVComboBoxPrivate(CUVComboBox* q, QObject* parent = nullptr);
	~CUVComboBoxPrivate() override;

protected:
	CUVComboBox* const q_ptr{ nullptr };

private:
	UVThemeType::ThemeMode themeMode{};
	int borderRadius{};
	CUVComboBoxStyle* comboBoxStyle{ nullptr };
	bool isAllowHidePopup{ false };
};
