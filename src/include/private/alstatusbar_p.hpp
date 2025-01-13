#pragma once

#include "alwidgettoolsdef.hpp"

namespace AL {
class CALStatusBar;
class CALStatusBarStyle;

class CALStatusBarPrivate final : public QObject {
	Q_OBJECT
	Q_DISABLE_COPY(CALStatusBarPrivate)
	Q_DECLARE_PUBLIC(CALStatusBar)

public:
	explicit CALStatusBarPrivate(CALStatusBar* q, QObject* parent = nullptr);
	~CALStatusBarPrivate() override;

protected:
	CALStatusBar* const q_ptr{ nullptr };

private:
	CALStatusBarStyle* style{ nullptr };
};
} // namespace AL