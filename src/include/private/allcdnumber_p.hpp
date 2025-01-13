#pragma once

#include "alwidgettoolsdef.hpp"

class QTimer;

namespace AL {
class CALLCDNumber;
class CALLCDNumberStyle;

class CALLCDNumberPrivate final : public QObject {
	Q_OBJECT
	Q_DISABLE_COPY(CALLCDNumberPrivate)
	Q_DECLARE_PUBLIC(CALLCDNumber)

public:
	explicit CALLCDNumberPrivate(CALLCDNumber* q, QObject* parent = nullptr);
	~CALLCDNumberPrivate() override;

	Q_SLOT void slotThemeModeChanged(const ALThemeType::ThemeMode& mode);

protected:
	CALLCDNumber* const q_ptr{ nullptr };

private:
	ALThemeType::ThemeMode themeMode{};
	bool isUseAutoClock{};
	QString autoClockFormat{};
	QTimer* clockTimer{ nullptr };
	CALLCDNumberStyle* lcdNumberStyle{ nullptr };
};
} // namespace AL