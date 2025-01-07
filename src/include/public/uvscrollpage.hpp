#pragma once

#include <QWidget>

#include "alwidgettoolsdef.hpp"

class CUVScrollPagePrivate;

class CALWIDGETTOOLS_EXPORT CUVScrollPage : public QWidget {
	Q_OBJECT
	Q_DECLARE_PRIVATE(CUVScrollPage)
	Q_PROPERTY(QWidget* customWidget READ getCustomWidget WRITE setCustomWidget NOTIFY sigCustomWidgetChanged)

public:
	explicit CUVScrollPage(QWidget* parent = nullptr);
	~CUVScrollPage() override;

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
	const QScopedPointer<CUVScrollPagePrivate> d_ptr{ nullptr };
};
