#pragma once

#include <QSpinBox>

#include "alwidgettoolsdef.hpp"

/**
 * @brief \namespace AL
 */
namespace AL {
class CALSpinBoxPrivate;

class CALWIDGETTOOLS_EXPORT CALSpinBox : public QSpinBox {
	Q_OBJECT
	Q_DECLARE_PRIVATE(CALSpinBox)

public:
	explicit CALSpinBox(QWidget* parent = nullptr);
	~CALSpinBox() override;

protected:
	const QScopedPointer<CALSpinBoxPrivate> d_ptr{ nullptr };

	void contextMenuEvent(QContextMenuEvent* event) override;
};
}
