#pragma once

#include <QListView>

#include "alwidgettoolsdef.hpp"

/**
 * @brief \namespace AL
 */
namespace AL {
class CALScrollBar;

class CALBaseListView final : public QListView {
	Q_OBJECT

public:
	explicit CALBaseListView(QWidget* parent = nullptr);
	~CALBaseListView() override;

Q_SIGNALS:
	 void sigMousePress(const QModelIndex& index);
	 void sigMouseRelease(const QModelIndex& index);
	 void sigMouseDoubleClick(const QModelIndex& index);

protected:
	void mousePressEvent(QMouseEvent* event) override;
	void mouseReleaseEvent(QMouseEvent* event) override;
	void mouseDoubleClickEvent(QMouseEvent* event) override;
};
}
