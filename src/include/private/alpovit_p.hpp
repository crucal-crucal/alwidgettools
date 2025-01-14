#pragma once

#include <QObject>

/**
 * @brief \namespace AL
 */
namespace AL {
class CALPovit;
class CALPovitModel;
class CALPovitStyle;
class CALPovitView;

class CALPovitPrivate final : public QObject {
	Q_OBJECT
	Q_DISABLE_COPY(CALPovitPrivate)
	Q_DECLARE_PUBLIC(CALPovit)

public:
	explicit CALPovitPrivate(CALPovit* q, QObject* parent = nullptr);
	~CALPovitPrivate() override;

protected:
	CALPovit* const q_ptr{ nullptr };

private:
	int textPixelSize{};
	CALPovitModel* model{ nullptr };
	CALPovitStyle* style{ nullptr };
	CALPovitView* view{ nullptr };

	void checkCurrentIndex();
};
}
