#pragma once

#include <QObject>

/**
 * @brief \namespace AL
 */
namespace AL {
class CALCheckBox;
class CALCheckBoxStyle;

class CALCheckBoxPrivate final : public QObject {
	Q_OBJECT
	Q_DISABLE_COPY(CALCheckBoxPrivate)
	Q_DECLARE_PUBLIC(CALCheckBox)

public:
	explicit CALCheckBoxPrivate(CALCheckBox* q, QObject* parent = nullptr);
	~CALCheckBoxPrivate() override;

protected:
	CALCheckBox* const q_ptr{ nullptr };

private:
	CALCheckBoxStyle* style{ nullptr };
};
}
