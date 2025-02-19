#pragma once

#include <QWidget>

#include "alwidgettoolsdef.hpp"

/**
 * @brief \namespace AL
 */
namespace AL {
class CALDrawerAreaPrivate;

class CALWIDGETTOOLS_EXPORT CALDrawerArea : public QWidget {
	Q_OBJECT
	Q_DECLARE_PRIVATE(CALDrawerArea)
	Q_PROPERTY(int borderRadius READ getBorderRadius WRITE setBorderRadius NOTIFY sigBorderRadiusChanged)
	Q_PROPERTY(int headerHeight READ getHeaderHeight WRITE setHeaderHeight NOTIFY sigHeaderHeightChanged)

public:
	explicit CALDrawerArea(QWidget* parent = nullptr);
	~CALDrawerArea() override;

	void setDrawerHeader(QWidget* widget);

	bool addDrawer(QWidget* widget);
	bool removeDrawer(QWidget* widget);

	void expand();
	void collpase();

	[[nodiscard]] bool getIsExpand() const;

	void setBorderRadius(int radius);
	[[nodiscard]] int getBorderRadius() const;

	void setHeaderHeight(int height);
	[[nodiscard]] int getHeaderHeight() const;

Q_SIGNALS:
	void sigBorderRadiusChanged();
	void sigHeaderHeightChanged();
	void sigExpandStateChanged(bool isExpand);

protected:
	const QScopedPointer<CALDrawerAreaPrivate> d_ptr{ nullptr };
};
}
