#pragma once

#include <QWidget>

#include "alwidgettoolsdef.hpp"

/**
 * @brief \namespace AL
 */
namespace AL {
class CALScrollPageAreaPrivate;

class CALWIDGETTOOLS_EXPORT CALScrollPageArea : public QWidget {
	Q_OBJECT
	Q_DECLARE_PRIVATE(CALScrollPageArea)
	Q_PROPERTY(qreal borderRadius READ getBorderRadius WRITE setBorderRadius NOTIFY sigBorderRadiusChanged)

public:
	explicit CALScrollPageArea(QWidget* parent = nullptr);
	~CALScrollPageArea() override;

	void setBorderRadius(qreal borderRadius);
	[[nodiscard]] qreal getBorderRadius() const;

Q_SIGNALS:
	Q_SIGNAL void sigBorderRadiusChanged();

protected:
	const QScopedPointer<CALScrollPageAreaPrivate> d_ptr{ nullptr };

	void paintEvent(QPaintEvent* event) override;
};
}
