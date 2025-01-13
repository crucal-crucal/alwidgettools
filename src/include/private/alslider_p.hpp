#pragma once

#include "alwidgettoolsdef.hpp"

namespace AL {
class CALSliderStyle;
class CALSlider;

class CALSliderPrivate final : public QObject {
	Q_OBJECT
	Q_DISABLE_COPY(CALSliderPrivate)
	Q_DECLARE_PUBLIC(CALSlider)

public:
	explicit CALSliderPrivate(CALSlider* q, QObject* parent = nullptr);
	~CALSliderPrivate() override;

protected:
	CALSlider* const q_ptr{ nullptr };

private:
	CALSliderStyle* style{ nullptr };
};
} // namespace AL