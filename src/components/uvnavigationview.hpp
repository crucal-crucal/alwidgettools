#pragma once

#include <QTreeView>

class CUVScrollBar;
class CUVNavigationStyle;

class CUVNavigationView final : public QTreeView {
	Q_OBJECT

public:
	explicit CUVNavigationView(QWidget* parent = nullptr);
	~CUVNavigationView() override;

	void navigationNodeStateChanged(const QVariantMap& data) const;

	Q_SLOT void slotCustomContextMenuRequested(const QPoint& pos);

Q_SIGNALS:
	Q_SIGNAL void sigNavigationClicked(const QModelIndex& index);
	Q_SIGNAL void sigNavigationOpenNewWindow(const QString& nodeKey);

protected:
	void mouseReleaseEvent(QMouseEvent* event) override;
	void mouseDoubleClickEvent(QMouseEvent* event) override;

private:
	CUVNavigationStyle* m_navigationStyle{ nullptr };
};
