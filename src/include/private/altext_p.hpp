#pragma once

#include "alwidgettoolsdef.hpp"

/**
 * @brief \namespace AL
 */
namespace AL {
class CALText;

class CALTextPrivate final : public QObject {
	Q_OBJECT
	Q_DISABLE_COPY(CALTextPrivate)
	Q_DECLARE_PUBLIC(CALText)

public:
	explicit CALTextPrivate(CALText* q, QObject* parent = nullptr);
	~CALTextPrivate() override;

	Q_SLOT void slotThemeChanged(const ALThemeType::ThemeMode& mode);

protected:
	CALText* const q_ptr{ nullptr };

private:
	ALThemeType::ThemeMode themeMode{};
	ALTextType::TextStyle textStyle{};
	ALIcon::IconType iconType{};
	bool isWrapAnyWhere{};
	qreal textSpacing{};

	void changeTheme();
};
}
