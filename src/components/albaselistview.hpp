#pragma once

#include <QListView>

#include "alwidgettoolsdef.hpp"

namespace AL {
class CALScrollBar;

class CALBaseListView final : public QListView {
	Q_OBJECT

public:
	explicit CALBaseListView(QWidget* parent = nullptr);
	~CALBaseListView() override;

Q_SIGNALS:
	Q_SIGNAL void sigMousePress(const QModelIndex& index);
	Q_SIGNAL void sigMouseRelease(const QModelIndex& index);
	Q_SIGNAL void sigMouseDoubleClick(const QModelIndex& index);

protected:
	void mousePressEvent(QMouseEvent* event) override;
	void mouseReleaseEvent(QMouseEvent* event) override;
	void mouseDoubleClickEvent(QMouseEvent* event) override;
};

} // namespace AL
