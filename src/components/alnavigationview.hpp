#pragma once

#include <QTreeView>

/**
 * @brief \namespace AL
 */
namespace AL {
class CALScrollBar;
class CALNavigationStyle;

class CALNavigationView final : public QTreeView {
	Q_OBJECT

public:
	explicit CALNavigationView(QWidget* parent = nullptr);
	~CALNavigationView() override;

	void navigationNodeStateChanged(const QVariantMap& data) const;

	Q_SLOT void slotCustomContextMenuRequested(const QPoint& pos);

Q_SIGNALS:
	 void sigNavigationClicked(const QModelIndex& index);
	 void sigNavigationOpenNewWindow(const QString& nodeKey);

protected:
	void mouseReleaseEvent(QMouseEvent* event) override;
	void mouseDoubleClickEvent(QMouseEvent* event) override;

private:
	CALNavigationStyle* m_navigationStyle{ nullptr };
};
}
