#pragma once

#include <QListView>

#include "alwidgettoolsdef.hpp"

namespace AL {
class CALListViewPrivate;

class CALWIDGETTOOLS_EXPORT CALListView : public QListView {
	Q_OBJECT
	Q_DECLARE_PRIVATE(CALListView)
	Q_PROPERTY(int itemHeight READ getItemHeight WRITE setItemHeight NOTIFY sigItemHeightChanged)
	Q_PROPERTY(bool isTransparent READ getIsTransparent WRITE setIsTransparent NOTIFY sigIsTransparentChanged)

public:
	explicit CALListView(QWidget* parent = nullptr);
	~CALListView() override;

	void setItemHeight(int itemHeight);
	[[nodiscard]] int getItemHeight() const;

	void setIsTransparent(bool isTransparent);
	[[nodiscard]] bool getIsTransparent() const;

Q_SIGNALS:
	Q_SIGNAL void sigItemHeightChanged();
	Q_SIGNAL void sigIsTransparentChanged();

protected:
	const QScopedPointer<CALListViewPrivate> d_ptr{ nullptr };
};
} // namespace AL