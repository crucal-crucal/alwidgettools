#pragma once

#include <QObject>

class CUVMenu;
class CUVDoubleSpinBox;
class CUVSpinBoxStyle;

class CUVDoubleSpinBoxPrivate final : public QObject {
	Q_OBJECT
	Q_DISABLE_COPY(CUVDoubleSpinBoxPrivate)
	Q_DECLARE_PUBLIC(CUVDoubleSpinBox)

public:
	explicit CUVDoubleSpinBoxPrivate(CUVDoubleSpinBox* q, QObject* parent = nullptr);
	~CUVDoubleSpinBoxPrivate() override;

protected:
	CUVDoubleSpinBox* const q_ptr{ nullptr };

private:
	CUVSpinBoxStyle* style{ nullptr };

	CUVMenu* createStandardContextMenu();
};
