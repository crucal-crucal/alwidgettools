#pragma once

#include <QDockWidget>

#include "alwidgettoolsdef.hpp"

/**
 * @brief \namespace AL
 */
namespace AL {
class CALDockWidgetPrivate;

class CALWIDGETTOOLS_EXPORT CALDockWidget : public QDockWidget {
	Q_OBJECT
	Q_DECLARE_PRIVATE(CALDockWidget)

public:
	explicit CALDockWidget(QWidget* parent = nullptr, Qt::WindowFlags flags = Qt::WindowFlags());
	explicit CALDockWidget(const QString& title, QWidget* parent = nullptr, Qt::WindowFlags flags = Qt::WindowFlags());
	~CALDockWidget() override;

protected:
	const QScopedPointer<CALDockWidgetPrivate> d_ptr{ nullptr };

	void paintEvent(QPaintEvent* event) override;
#ifdef Q_OS_WIN
	bool event(QEvent* event) override;
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
	bool nativeEvent(const QByteArray& eventType, void* message, qintptr* result) override;
#else
	bool nativeEvent(const QByteArray& eventType, void* message, long* result) override;
#endif
#endif
};
}
