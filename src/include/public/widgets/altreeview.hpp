#pragma once

#include <QTreeView>

#include "alwidgettoolsdef.hpp"

/**
 * @brief \namespace AL
 */
namespace AL {
class CALTreeViewPrivate;

class CALWIDGETTOOLS_EXPORT CALTreeView : public QTreeView {
	Q_OBJECT
	Q_DECLARE_PRIVATE(CALTreeView)
	Q_PROPERTY(int itemHeight READ getItemHeight WRITE setItemHeight NOTIFY sigItemHeightChanged)
	Q_PROPERTY(int headerMargin READ getHeaderMargin WRITE setHeaderMargin NOTIFY sigHeaderMarginChanged)

public:
	explicit CALTreeView(QWidget* parent = nullptr);
	~CALTreeView() override;

	void setItemHeight(int height);
	[[nodiscard]] int getItemHeight() const;

	void setHeaderMargin(int margin);
	[[nodiscard]] int getHeaderMargin() const;

Q_SIGNALS:
	void sigItemHeightChanged();
	void sigHeaderMarginChanged();

protected:
	const QScopedPointer<CALTreeViewPrivate> d_ptr{ nullptr };
};
}
