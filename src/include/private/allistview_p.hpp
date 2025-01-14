#pragma once

#include <QObject>

/**
 * @brief \namespace AL
 */
namespace AL {
class CALListView;
class CALListViewStyle;
class CALScrollBar;

class CALListViewPrivate final : public QObject {
	Q_OBJECT
	Q_DISABLE_COPY(CALListViewPrivate)
	Q_DECLARE_PUBLIC(CALListView)

public:
	explicit CALListViewPrivate(CALListView* q, QObject* parent = nullptr);
	~CALListViewPrivate() override;

protected:
	CALListView* const q_ptr{ nullptr };

private:
	CALListViewStyle* style{ nullptr };
	CALScrollBar* verticalScrollBar{ nullptr };
	CALScrollBar* horizontalScrollBar{ nullptr };
};
}
