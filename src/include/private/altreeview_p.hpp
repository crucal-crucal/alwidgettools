#pragma once

#include <QObject>

/**
 * @brief \namespace AL
 */
namespace AL {
class CALTreeView;
class CALTreeViewStyle;
class CALScrollBar;

class CALTreeViewPrivate final : public QObject {
	Q_OBJECT
	Q_DISABLE_COPY(CALTreeViewPrivate)
	Q_DECLARE_PUBLIC(CALTreeView)

public:
	explicit CALTreeViewPrivate(CALTreeView* q, QObject* parent = nullptr);
	~CALTreeViewPrivate() override;

protected:
	CALTreeView* const q_ptr{ nullptr };

private:
	CALTreeViewStyle* style{ nullptr };
	CALScrollBar* verticalScrollBar{ nullptr };
	CALScrollBar* horizontalScrollBar{ nullptr };
};
}
