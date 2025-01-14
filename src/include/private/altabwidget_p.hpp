#pragma once

#include <QDrag>
#include <QObject>

/**
 * @brief \namespace AL
 */
namespace AL {
class CALTabBar;
class CALTabWidget;

class CALTabWidgetPrivate final : public QObject {
	Q_OBJECT
	Q_DISABLE_COPY(CALTabWidgetPrivate)
	Q_DECLARE_PUBLIC(CALTabWidget)

public:
	explicit CALTabWidgetPrivate(CALTabWidget* q, QObject* parent = nullptr);
	~CALTabWidgetPrivate() override;

	Q_SLOT void slotTabBarPress(int index);
	Q_SLOT void slotTabDragCreate(QDrag* drag);
	Q_SLOT void slotTabDragDrop(const QMimeData* mimeData);
	Q_SLOT void slotTabCloseRequested(int index);

protected:
	CALTabWidget* const q_ptr{ nullptr };

private:
	CALTabBar* customTabBar{ nullptr };
	bool isTransparent{};
};
}
