#pragma once

#include <QWidget>

#include "alwidgettoolsdef.hpp"

class CALScrollPageAreaPrivate;

class CALWIDGETTOOLS_EXPORT CALScrollPageArea : public QWidget {
	Q_OBJECT
	Q_DECLARE_PRIVATE(CALScrollPageArea)
	Q_PROPERTY(int borderRadius READ getBorderRadius WRITE setBorderRadius NOTIFY sigBorderRadiusChanged)

public:
	explicit CALScrollPageArea(QWidget* parent = nullptr);
	~CALScrollPageArea() override;

	void setBorderRadius(int borderRadius);
	[[nodiscard]] int getBorderRadius() const;

Q_SIGNALS:
	Q_SIGNAL void sigBorderRadiusChanged();

protected:
	const QScopedPointer<CALScrollPageAreaPrivate> d_ptr{ nullptr };

	void paintEvent(QPaintEvent* event) override;
};
