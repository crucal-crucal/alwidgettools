#pragma once

#include "alwidgettoolsdef.hpp"

/**
 * @brief \namespace AL
 */
namespace AL {
class CALComboBox;
class CALComboBoxStyle;

class CALComboBoxPrivate final : public QObject {
	Q_OBJECT
	Q_DISABLE_COPY(CALComboBoxPrivate)
	Q_DECLARE_PUBLIC(CALComboBox)

public:
	explicit CALComboBoxPrivate(CALComboBox* q, QObject* parent = nullptr);
	~CALComboBoxPrivate() override;

protected:
	CALComboBox* const q_ptr{ nullptr };

private:
	ALThemeType::ThemeMode themeMode{};
	int borderRadius{};
	CALComboBoxStyle* comboBoxStyle{ nullptr };
	bool isAllowHidePopup{ false };
};
}
