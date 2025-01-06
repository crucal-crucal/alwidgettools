#pragma once

#include <QObject>

class CUVCheckBox;
class CUVCheckBoxStyle;

class CUVCheckBoxPrivate final : public QObject {
	Q_OBJECT
	Q_DISABLE_COPY(CUVCheckBoxPrivate)
	Q_DECLARE_PUBLIC(CUVCheckBox)

public:
	explicit CUVCheckBoxPrivate(CUVCheckBox* q, QObject* parent = nullptr);
	~CUVCheckBoxPrivate() override;

protected:
	CUVCheckBox* const q_ptr{ nullptr };

private:
	CUVCheckBoxStyle* style{ nullptr };
};
