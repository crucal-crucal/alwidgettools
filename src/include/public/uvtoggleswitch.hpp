#pragma once

#include <QWidget>

#include "alwidgettoolsdef.hpp"

class CUVToggleSwitchPrivate;

class CALWIDGETTOOLS_EXPORT CUVToggleSwitch : public QWidget {
	Q_OBJECT
	Q_DISABLE_COPY(CUVToggleSwitch)
	Q_DECLARE_PRIVATE(CUVToggleSwitch)

public:
	explicit CUVToggleSwitch(QWidget* parent = nullptr);
	~CUVToggleSwitch() override;

	void setIsToggled(bool isToggled);
	[[nodiscard]] bool isToggled() const;

signals:
	void sigToggleChanged(bool toggled);

protected:
	const QScopedPointer<CUVToggleSwitchPrivate> d_ptr{ nullptr };

	bool event(QEvent* event) override;
	void mousePressEvent(QMouseEvent* event) override;
	void mouseReleaseEvent(QMouseEvent* event) override;
	void mouseMoveEvent(QMouseEvent* event) override;
	void paintEvent(QPaintEvent* event) override;
};
