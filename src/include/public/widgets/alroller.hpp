#pragma once

#include <QWidget>

#include "alwidgettoolsdef.hpp"

/**
 * @brief \namespace AL
 */
namespace AL {
class CALRollerPrivate;

class CALWIDGETTOOLS_EXPORT CALRoller : public QWidget {
	Q_OBJECT
	Q_DECLARE_PRIVATE(CALRoller)
	Q_PROPERTY(int borderRadius READ getBorderRadius WRITE setBorderRadius NOTIFY sigBorderRadiusChanged)
	Q_PROPERTY(int itemHeight READ getItemHeight WRITE setItemHeight NOTIFY sigItemHeightChanged)
	Q_PROPERTY(int maxVisibleItems READ getMaxVisibleItems WRITE setMaxVisibleItems NOTIFY sigMaxVisibleItemsChanged)
	Q_PROPERTY(int currentIndex READ getCurrentIndex WRITE setCurrentIndex NOTIFY sigCurrentIndexChanged)
	Q_PROPERTY(QStringList itemList READ getItemList WRITE setItemList NOTIFY sigItemListChanged)

public:
	explicit CALRoller(QWidget* parent = nullptr);
	~CALRoller() override;

	[[nodiscard]] QString getCurrentData() const;

	void setBorderRadius(int radius);
	[[nodiscard]] int getBorderRadius() const;

	void setItemHeight(int height);
	[[nodiscard]] int getItemHeight() const;

	void setMaxVisibleItems(int count);
	[[nodiscard]] int getMaxVisibleItems() const;

	void setCurrentIndex(int index);
	[[nodiscard]] int getCurrentIndex() const;

	void setItemList(const QStringList& list);
	[[nodiscard]] QStringList getItemList() const;

Q_SIGNALS:
	void sigBorderRadiusChanged();
	void sigItemHeightChanged();
	void sigMaxVisibleItemsChanged();
	void sigCurrentIndexChanged();
	void sigItemListChanged();

protected:
	const QScopedPointer<CALRollerPrivate> d_ptr{ nullptr };

	void wheelEvent(QWheelEvent* event) override;
	void mouseReleaseEvent(QMouseEvent* event) override;
	void mouseMoveEvent(QMouseEvent* event) override;
	void leaveEvent(QEvent* event) override;
	void paintEvent(QPaintEvent* event) override;
};
}
