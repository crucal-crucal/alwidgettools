#include "uvradiobutton.hpp"

#include "uvradiobutton_p.hpp"
#include "uvradiobuttonstyle.hpp"
#include "uvthememanager.hpp"

/**
 * \class CUVRadioButtonPrivate
 * Internal class for CUVRadioButton
 * @param q pointer to the public class
 * @param parent pointer to the parent class
 */
CUVRadioButtonPrivate::CUVRadioButtonPrivate(CUVRadioButton* q, QObject* parent): QObject(parent), q_ptr(q) {
}

CUVRadioButtonPrivate::~CUVRadioButtonPrivate() = default;

void CUVRadioButtonPrivate::slotThemeModeChanged(const UVThemeType::ThemeMode& mode) {
	Q_Q(CUVRadioButton);

	QPalette palette = q->palette();
	palette.setColor(QPalette::WindowText, UVThemeColor(mode, UVThemeType::BasicText));
	q->setPalette(palette);
}

/**
 * \class CUVRadioButton
 * @param parent pointer to the parent class
 */
CUVRadioButton::CUVRadioButton(QWidget* parent): QRadioButton(parent), d_ptr(new CUVRadioButtonPrivate(this, this)) {
	Q_D(CUVRadioButton);
	d->themeMode = UVTheme->getThemeMode();
	connect(UVTheme, &CUVThemeManager::sigThemeModeChanged, d, &CUVRadioButtonPrivate::slotThemeModeChanged);
	d->style = new CUVRadioButtonStyle(style());
	setStyle(d->style);
	QFont font = this->font();
	font.setPixelSize(15);
	this->setFont(font);
	setFixedHeight(20);
}

CUVRadioButton::CUVRadioButton(const QString& text, QWidget* parent): CUVRadioButton(parent) {
	setText(text);
}

CUVRadioButton::~CUVRadioButton() = default;
