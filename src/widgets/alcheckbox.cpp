#include "alcheckbox.hpp"

#include "alcheckboxstyle.hpp"
#include "alcheckbox_p.hpp"

/**
 * @brief \namespace AL
 */
namespace AL {
/**
 * @brief \class CALCheckBoxPrivate
 * Internal class for CALCheckBox
 * @param q pointer to the public class
 * @param parent pointer to the parent class
 */
CALCheckBoxPrivate::CALCheckBoxPrivate(CALCheckBox* q, QObject* parent): QObject(parent), q_ptr(q) {
}

CALCheckBoxPrivate::~CALCheckBoxPrivate() = default;

/**
 * @brief \class CALCheckBox
 * @param parent pointer to the parent class
 */
CALCheckBox::CALCheckBox(QWidget* parent): QCheckBox(parent), d_ptr(new CALCheckBoxPrivate(this, this)) {
	Q_D(CALCheckBox);

	d->style = new CALCheckBoxStyle(style());

	setMouseTracking(true);
	setObjectName("CALCheckBox");
	setStyle(d->style);
	QFont font = this->font();
	font.setPixelSize(15);
	setFont(font);
}

CALCheckBox::CALCheckBox(const QString& text, QWidget* parent): CALCheckBox(parent) {
	setText(text);
}

CALCheckBox::~CALCheckBox() = default;
}
