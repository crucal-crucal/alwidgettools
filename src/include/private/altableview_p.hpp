#pragma once

#include <QObject>

/**
 * @brief \namespace AL
 */
namespace AL {
class CALScrollBar;
class CALTableView;
class CALTableViewStyle;

class CALTableViewPrivate final : public QObject {
	Q_OBJECT
	Q_DISABLE_COPY(CALTableViewPrivate)
	Q_DECLARE_PUBLIC(CALTableView)

public:
	explicit CALTableViewPrivate(CALTableView* q, QObject* parent = nullptr);
	~CALTableViewPrivate() override;

protected:
	CALTableView* const q_ptr{ nullptr };

private:
	CALTableViewStyle* style{ nullptr };
	CALScrollBar* horizontalScrollBar{ nullptr };
	CALScrollBar* verticalScrollBar{ nullptr };
};
}
