#pragma once

#include <QDoubleSpinBox>

#include "alwidgettoolsdef.hpp"

/**
 * @brief \namespace AL
 */
namespace AL {
class CALDoubleSpinBoxPrivate;

class CALWIDGETTOOLS_EXPORT CALDoubleSpinBox : public QDoubleSpinBox {
	Q_OBJECT
	Q_DECLARE_PRIVATE(CALDoubleSpinBox)

public:
	explicit CALDoubleSpinBox(QWidget* parent = nullptr);
	~CALDoubleSpinBox() override;

protected:
	const QScopedPointer<CALDoubleSpinBoxPrivate> d_ptr{ nullptr };

	void contextMenuEvent(QContextMenuEvent* event) override;
};
}
