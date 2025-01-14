#pragma once

#include <QWidget>

#include "alwidgettoolsdef.hpp"

/**
 * @brief \namespace AL
 */
namespace AL {
class CALScrollPagePrivate;

class CALWIDGETTOOLS_EXPORT CALScrollPage : public QWidget {
	Q_OBJECT
	Q_DECLARE_PRIVATE(CALScrollPage)
	Q_PROPERTY(QWidget* customWidget READ getCustomWidget WRITE setCustomWidget NOTIFY sigCustomWidgetChanged)

public:
	explicit CALScrollPage(QWidget* parent = nullptr);
	~CALScrollPage() override;

	void setCustomWidget(QWidget* widget);
	[[nodiscard]] QWidget* getCustomWidget() const;

	void addCentralWidget(QWidget* widget, bool isWidgetResizeable = true, bool isVerticalGrabGesture = true, qreal mousePressEventDelay = 0.5);

	void navigation(int widgetIndex, bool isLogRoute = true);

	void setPageTitleSpacing(int spacing);
	[[nodiscard]] int getPageTitleSpacing() const;

	void setTitleVisible(bool isVisible);

Q_SIGNALS:
	Q_SIGNAL void sigCustomWidgetChanged();

protected:
	const QScopedPointer<CALScrollPagePrivate> d_ptr{ nullptr };
};
}
