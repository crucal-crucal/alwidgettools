#pragma once

#include <QObject>

class CUVSliderStyle;
class CUVSlider;

class CUVSliderPrivate final : public QObject {
	Q_OBJECT
	Q_DISABLE_COPY(CUVSliderPrivate)
	Q_DECLARE_PUBLIC(CUVSlider)

public:
	explicit CUVSliderPrivate(CUVSlider* q, QObject* parent = nullptr);
	~CUVSliderPrivate() override;

protected:
	CUVSlider* const q_ptr{ nullptr };

private:
	CUVSliderStyle* style{ nullptr };
};
