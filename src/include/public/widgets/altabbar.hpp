#pragma once

#include <QDrag>
#include <QTabBar>

#include "alwidgettoolsdef.hpp"

/**
 * @brief \namespace AL
 */
namespace AL {
class CALTabBarPrivate;

class CALWIDGETTOOLS_EXPORT CALTabBar : public QTabBar {
	Q_OBJECT
	Q_DECLARE_PRIVATE(CALTabBar)

public:
	explicit CALTabBar(QWidget* parent = nullptr);
	~CALTabBar() override;

Q_SIGNALS:
	Q_SIGNAL void sigTabBarPress(int index);
	Q_SIGNAL void sigTabDragCreate(QDrag* drag);
	Q_SIGNAL void sigTabDragDrop(const QMimeData* mimeData);

protected:
	const QScopedPointer<CALTabBarPrivate> d_ptr{ nullptr };

	void mousePressEvent(QMouseEvent* event) override;
	void mouseMoveEvent(QMouseEvent* event) override;
	void dragEnterEvent(QDragEnterEvent* event) override;
	void dropEvent(QDropEvent* event) override;
};
}
