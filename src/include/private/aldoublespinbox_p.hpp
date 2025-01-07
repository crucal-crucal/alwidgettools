#pragma once

#include <QObject>

class CALMenu;
class CALDoubleSpinBox;
class CALSpinBoxStyle;

class CALDoubleSpinBoxPrivate final : public QObject {
	Q_OBJECT
	Q_DISABLE_COPY(CALDoubleSpinBoxPrivate)
	Q_DECLARE_PUBLIC(CALDoubleSpinBox)

public:
	explicit CALDoubleSpinBoxPrivate(CALDoubleSpinBox* q, QObject* parent = nullptr);
	~CALDoubleSpinBoxPrivate() override;

protected:
	CALDoubleSpinBox* const q_ptr{ nullptr };

private:
	CALSpinBoxStyle* style{ nullptr };

	CALMenu* createStandardContextMenu();
};
