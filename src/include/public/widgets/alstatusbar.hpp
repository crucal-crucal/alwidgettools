#pragma once

#include <QStatusBar>

#include "alwidgettoolsdef.hpp"

/**
 * @brief \namespace AL
 */
namespace AL {
class CALStatusBarPrivate;

class CALWIDGETTOOLS_EXPORT CALStatusBar : public QStatusBar {
	Q_OBJECT
	Q_DECLARE_PRIVATE(CALStatusBar)

public:
	explicit CALStatusBar(QWidget* parent = nullptr);
	~CALStatusBar() override;

protected:
	const QScopedPointer<CALStatusBarPrivate> d_ptr{ nullptr };
};
}
