#include "alstatusbar.hpp"

#include "alstatusbar_p.hpp"
#include "alstatusbarstyle.hpp"

/**
 * @brief \namespace AL
 */
namespace AL {
/**
 * @brief \class CALStatusBarPrivate
 * Internal class for CALStatusBar
 * @param q pointer to the public class
 * @param parent pointer to the parent class
 */
CALStatusBarPrivate::CALStatusBarPrivate(CALStatusBar* q, QObject* parent): QObject(parent), q_ptr(q) {
}

CALStatusBarPrivate::~CALStatusBarPrivate() = default;

/**
 * @brief \class CALStatusBar
 * @param parent pointer to the parent class
 */
CALStatusBar::CALStatusBar(QWidget* parent): QStatusBar(parent), d_ptr(new CALStatusBarPrivate(this, this)) {
	Q_D(CALStatusBar);

	d->style = new CALStatusBarStyle(style());
	setStyle(d->style);
	setFixedHeight(28);
	setContentsMargins(20, 0, 0, 0);
}

CALStatusBar::~CALStatusBar() = default;
}
