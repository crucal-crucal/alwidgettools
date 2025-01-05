#pragma once

#include <QWidget>

#include "uvwidgettoolsdef.hpp"

class CUVScrollPageAreaPrivate;

class CUVWIDGETTOOLS_EXPORT CUVScrollPageArea : public QWidget {
	Q_OBJECT
	Q_DECLARE_PRIVATE(CUVScrollPageArea)
	Q_PROPERTY(int borderRadius READ getBorderRadius WRITE setBorderRadius NOTIFY sigBorderRadiusChanged)

public:
	explicit CUVScrollPageArea(QWidget* parent = nullptr);
	~CUVScrollPageArea() override;

	void setBorderRadius(int borderRadius);
	[[nodiscard]] int getBorderRadius() const;

Q_SIGNALS:
	Q_SIGNAL void sigBorderRadiusChanged();

protected:
	const QScopedPointer<CUVScrollPageAreaPrivate> d_ptr{ nullptr };

	void paintEvent(QPaintEvent* event) override;
};
