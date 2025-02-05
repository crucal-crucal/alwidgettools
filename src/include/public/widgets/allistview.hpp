#pragma once

#include <QListView>

#include "alwidgettoolsdef.hpp"

/**
 * @brief \namespace AL
 */
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
	void sigItemHeightChanged();
	void sigIsTransparentChanged();

protected:
	const QScopedPointer<CALListViewPrivate> d_ptr{ nullptr };
};
}
