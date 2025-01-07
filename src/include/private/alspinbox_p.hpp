#pragma once

#include <QObject>

class CALMenu;
class CALSpinBox;
class CALSpinBoxStyle;

class CALSpinBoxPrivate final : public QObject {
	Q_OBJECT
	Q_DISABLE_COPY(CALSpinBoxPrivate)
	Q_DECLARE_PUBLIC(CALSpinBox)

public:
	explicit CALSpinBoxPrivate(CALSpinBox* q, QObject* parent = nullptr);
	~CALSpinBoxPrivate() override;

protected:
	CALSpinBox* const q_ptr{ nullptr };

private:
	CALSpinBoxStyle* style{ nullptr };

	CALMenu* createStandardContextMenu();
};
