#pragma once

#include <QObject>

/**
 * @brief \namespace AL
 */
namespace AL {
class CALToolTip;
class CALToolButton;
class CALToolButtonStyle;

class CALToolButtonPrivate final : public QObject {
	Q_OBJECT
	Q_DISABLE_COPY(CALToolButtonPrivate)
	Q_DECLARE_PUBLIC(CALToolButton)

public:
	explicit CALToolButtonPrivate(CALToolButton* q, QObject* parent = nullptr);
	~CALToolButtonPrivate() override;

protected:
	CALToolButton* const q_ptr{ nullptr };

private:
	CALToolButtonStyle* style{ nullptr };
	CALToolTip* tooltip{ nullptr };
};
}
