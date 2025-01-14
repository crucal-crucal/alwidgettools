#pragma once

#include <QObject>
#include <QPixmap>

/**
 * @brief \namespace AL
 */
namespace AL {
class CALTabBar;
class CALTabBarStyle;

class CALTabBarPrivate final : public QObject {
	Q_OBJECT
	Q_DISABLE_COPY(CALTabBarPrivate)
	Q_DECLARE_PUBLIC(CALTabBar)

public:
	explicit CALTabBarPrivate(CALTabBar* q, QObject* parent = nullptr);
	~CALTabBarPrivate() override;

protected:
	CALTabBar* const q_ptr{ nullptr };

private:
	QPixmap lastDragPix{};
	CALTabBarStyle* style{ nullptr };
};
}
