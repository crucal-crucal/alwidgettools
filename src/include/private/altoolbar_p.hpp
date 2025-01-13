#pragma once

#include "alwidgettoolsdef.hpp"

namespace AL {
class CALToolBar;
class CALToolBarStyle;

class CALToolBarPrivate final : public QObject {
	Q_OBJECT
	Q_DISABLE_COPY(CALToolBarPrivate)
	Q_DECLARE_PUBLIC(CALToolBar)

public:
	explicit CALToolBarPrivate(CALToolBar* q, QObject* parent = nullptr);
	~CALToolBarPrivate() override;

protected:
	CALToolBar* const q_ptr{ nullptr };

private:
	ALThemeType::ThemeMode themeMode{};
	int shadowBorderWidth{};
	CALToolBarStyle* style{ nullptr };
};
} // namespace AL