#pragma once

#include <QListView>

class CUVScrollBar;

class CUVBaseListView final : public QListView {
	Q_OBJECT

public:
	explicit CUVBaseListView(QWidget* parent = nullptr);
	~CUVBaseListView() override;

Q_SIGNALS:
	Q_SIGNAL void sigMousePress(const QModelIndex& index);
	Q_SIGNAL void sigMouseRelease(const QModelIndex& index);
	Q_SIGNAL void sigMouseDoubleClick(const QModelIndex& index);

protected:
	void mousePressEvent(QMouseEvent* event) override;
	void mouseReleaseEvent(QMouseEvent* event) override;
	void mouseDoubleClickEvent(QMouseEvent* event) override;
};
