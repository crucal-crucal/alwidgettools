#pragma once

#include <QWidget>

#include "alwidgettoolsdef.hpp"

class CALToggleSwitchPrivate;

class CALWIDGETTOOLS_EXPORT CALToggleSwitch : public QWidget {
	Q_OBJECT
	Q_DISABLE_COPY(CALToggleSwitch)
	Q_DECLARE_PRIVATE(CALToggleSwitch)

public:
	explicit CALToggleSwitch(QWidget* parent = nullptr);
	~CALToggleSwitch() override;

	void setIsToggled(bool isToggled);
	[[nodiscard]] bool isToggled() const;

signals:
	void sigToggleChanged(bool toggled);

protected:
	const QScopedPointer<CALToggleSwitchPrivate> d_ptr{ nullptr };

	bool event(QEvent* event) override;
	void mousePressEvent(QMouseEvent* event) override;
	void mouseReleaseEvent(QMouseEvent* event) override;
	void mouseMoveEvent(QMouseEvent* event) override;
	void paintEvent(QPaintEvent* event) override;
};
