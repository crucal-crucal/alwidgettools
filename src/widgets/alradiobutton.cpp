#include "alradiobutton.hpp"

#include "alradiobutton_p.hpp"
#include "alradiobuttonstyle.hpp"
#include "althememanager.hpp"

/**
 * @brief \namespace AL
 */
namespace AL {
/**
 * \class CALRadioButtonPrivate
 * Internal class for CALRadioButton
 * @param q pointer to the public class
 * @param parent pointer to the parent class
 */
CALRadioButtonPrivate::CALRadioButtonPrivate(CALRadioButton* q, QObject* parent): QObject(parent), q_ptr(q) {
}

CALRadioButtonPrivate::~CALRadioButtonPrivate() = default;

void CALRadioButtonPrivate::slotThemeModeChanged(const ALThemeType::ThemeMode& mode) {
	Q_Q(CALRadioButton);

	QPalette palette = q->palette();
	palette.setColor(QPalette::WindowText, ALThemeColor(mode, ALThemeType::BasicText));
	q->setPalette(palette);
}

/**
 * \class CALRadioButton
 * @param parent pointer to the parent class
 */
CALRadioButton::CALRadioButton(QWidget* parent): QRadioButton(parent), d_ptr(new CALRadioButtonPrivate(this, this)) {
	Q_D(CALRadioButton);
	d->themeMode = ALTheme->getThemeMode();
	connect(ALTheme, &CALThemeManager::sigThemeModeChanged, d, &CALRadioButtonPrivate::slotThemeModeChanged);
	d->style = new CALRadioButtonStyle(style());
	setStyle(d->style);
	QFont font = this->font();
	font.setPixelSize(15);
	this->setFont(font);
	setFixedHeight(20);
}

CALRadioButton::CALRadioButton(const QString& text, QWidget* parent): CALRadioButton(parent) {
	setText(text);
}

CALRadioButton::~CALRadioButton() = default;
}
