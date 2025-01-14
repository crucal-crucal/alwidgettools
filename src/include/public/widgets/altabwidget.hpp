#pragma once

#include <QTabWidget>

#include "../al/alwidgettoolsdef.hpp"

/**
 * @brief \namespace AL
 */
namespace AL {
class CALTabWidgetPrivate;

class CALWIDGETTOOLS_EXPORT CALTabWidget : public QTabWidget {
	Q_OBJECT
	Q_DECLARE_PRIVATE(CALTabWidget)
	Q_PROPERTY(bool isTransparent READ getIsTransparent WRITE setIsTransparent NOTIFY sigIsTransparentChanged)

public:
	explicit CALTabWidget(QWidget* parent = nullptr);
	~CALTabWidget() override;

	void setTabPosition(const TabPosition& position);

	void setIsTransparent(bool isTransparent);
	bool getIsTransparent() const;

Q_SIGNALS:
	Q_SIGNAL void sigIsTransparentChanged();

protected:
	const QScopedPointer<CALTabWidgetPrivate> d_ptr{ nullptr };

	void paintEvent(QPaintEvent* event) override;
	void dragEnterEvent(QDragEnterEvent* event) override;
	void dropEvent(QDropEvent* event) override;
};
}
