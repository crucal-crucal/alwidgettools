#pragma once

#include <QObject>

/**
 * @brief \namespace AL
 */
namespace AL {
class CALScrollBar;
class CALScrollArea;

class CALScrollAreaPrivate final : public QObject {
	Q_OBJECT
	Q_DECLARE_PUBLIC(CALScrollArea)

public:
	explicit CALScrollAreaPrivate(CALScrollArea* q, QObject* parent = nullptr);
	~CALScrollAreaPrivate() override;

	[[nodiscard]] CALScrollBar* getScrollBar(const Qt::Orientation& orientation) const;

protected:
	CALScrollArea* const q_ptr{ nullptr };

private:
	CALScrollBar* horizontalScrollBar{ nullptr };
	CALScrollBar* verticalScrollBar{ nullptr };
};
}
