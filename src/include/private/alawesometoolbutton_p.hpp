#pragma once

#include "alwidgettoolsdef.hpp"

namespace AL {
class CALToolTip;
class CALAwesomeToolButton;
class CALAwesomeToolButtonStyle;

class CALAwesomeToolButtonPrivate final : public QObject {
	Q_OBJECT
	Q_DISABLE_COPY(CALAwesomeToolButtonPrivate)
	Q_DECLARE_PUBLIC(CALAwesomeToolButton)

public:
	explicit CALAwesomeToolButtonPrivate(CALAwesomeToolButton* q, QObject* parent = nullptr);
	~CALAwesomeToolButtonPrivate() override;

protected:
	CALAwesomeToolButton* const q_ptr{ nullptr };

private:
	CALAwesomeToolButtonStyle* style{ nullptr };
	CALToolTip* tooltip{ nullptr };
};

} // namespace AL
