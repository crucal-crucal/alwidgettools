#pragma once

#include <QLCDNumber>

#include "alwidgettoolsdef.hpp"

/**
 * @brief \namespace AL
 */
namespace AL {
class CALLCDNumberPrivate;

class CALWIDGETTOOLS_EXPORT CALLCDNumber : public QLCDNumber {
	Q_OBJECT
	Q_DECLARE_PRIVATE(CALLCDNumber)
	Q_PROPERTY(bool isUseAutoClock READ getIsUseAutoClock WRITE setIsUseAutoClock NOTIFY sigIsUseAutoClockChanged)
	Q_PROPERTY(bool isTransparent READ getIsTransparent WRITE setIsTransparent NOTIFY sigIsTransparentChanged)
	Q_PROPERTY(QString autoClockFormat READ getAutoClockFormat WRITE setAutoClockFormat NOTIFY sigAutoClockFormatChanged)

public:
	explicit CALLCDNumber(QWidget* parent = nullptr);
	explicit CALLCDNumber(int numDigits, QWidget* parent = nullptr);
	~CALLCDNumber() override;

	void setIsUseAutoClock(bool isUseAutoClock);
	[[nodiscard]] bool getIsUseAutoClock() const;

	void setIsTransparent(bool isTransparent);
	[[nodiscard]] bool getIsTransparent() const;

	void setAutoClockFormat(const QString& autoClockFormat);
	[[nodiscard]] QString getAutoClockFormat() const;

Q_SIGNALS:
	void sigIsUseAutoClockChanged();
	void sigIsTransparentChanged();
	void sigAutoClockFormatChanged();

protected:
	const QScopedPointer<CALLCDNumberPrivate> d_ptr{ nullptr };
};
}
