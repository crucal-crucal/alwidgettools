#include "alcheckbox.hpp"

#include "uvcheckboxstyle.hpp"
#include "uvcheckbox_p.hpp"

/**
 * @brief \class CUVCheckBoxPrivate
 * Internal class for CUVCheckBox
 * @param q pointer to the public class
 * @param parent pointer to the parent class
 */
CUVCheckBoxPrivate::CUVCheckBoxPrivate(CUVCheckBox* q, QObject* parent): QObject(parent), q_ptr(q) {

}

CUVCheckBoxPrivate::~CUVCheckBoxPrivate() = default;

/**
 * @brief \class CUVCheckBox
 * @param parent pointer to the parent class
 */
CUVCheckBox::CUVCheckBox(QWidget* parent): QCheckBox(parent), d_ptr(new CUVCheckBoxPrivate(this, this)) {
	Q_D(CUVCheckBox);

	d->style = new CUVCheckBoxStyle(style());

	setMouseTracking(true);
	setObjectName("CUVCheckBox");
	setStyle(d->style);
	QFont font = this->font();
	font.setPixelSize(15);
	setFont(font);
}

CUVCheckBox::CUVCheckBox(const QString& text, QWidget* parent): CUVCheckBox(parent) {
	setText(text);
}

CUVCheckBox::~CUVCheckBox() = default;
