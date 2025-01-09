#pragma once

#include "alwidgettoolsdef.hpp"

class CALAppBar;
class CALWidget;

class CALWidgetPrivate final : public QObject {
	Q_OBJECT
	Q_DISABLE_COPY(CALWidgetPrivate)
	Q_DECLARE_PUBLIC(CALWidget)

public:
	explicit CALWidgetPrivate(CALWidget* q, QObject* parent = nullptr);
	~CALWidgetPrivate() override;

protected:
	CALWidget* const q_ptr{ nullptr };

private:
	ALThemeType::ThemeMode themeMode{};
	bool isEanbleMica{};
	CALAppBar* appBar{ nullptr };
};
