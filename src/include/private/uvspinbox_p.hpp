#pragma once

#include <QObject>

class CUVMenu;
class CUVSpinBox;
class CUVSpinBoxStyle;

class CUVSpinBoxPrivate final : public QObject {
	Q_OBJECT
	Q_DISABLE_COPY(CUVSpinBoxPrivate)
	Q_DECLARE_PUBLIC(CUVSpinBox)

public:
	explicit CUVSpinBoxPrivate(CUVSpinBox* q, QObject* parent = nullptr);
	~CUVSpinBoxPrivate() override;

protected:
	CUVSpinBox* const q_ptr{ nullptr };

private:
	CUVSpinBoxStyle* style{ nullptr };

	CUVMenu* createStandardContextMenu();
};
