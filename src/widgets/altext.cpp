#include "altext.hpp"

#include <QPainter>
#include <QPalette>
#include <QTimer>
#include <QVariant>

#include "altext_p.hpp"
#include "althememanager.hpp"

/**
 * @brief \namespace AL
 */
namespace AL {
/**
 * @brief \class CALTextPrivate
 * Internal class for CALText
 * @param q pointer to the public class
 * @param parent pointer to the parent class
 */
CALTextPrivate::CALTextPrivate(CALText* q, QObject* parent): QObject(parent), q_ptr(q) {
}

CALTextPrivate::~CALTextPrivate() = default;

void CALTextPrivate::slotThemeChanged(const ALThemeType::ThemeMode& mode) {
	Q_Q(CALText);

	themeMode = mode;
	if (q->isVisible()) {
		changeTheme();
	} else {
		QTimer::singleShot(1, this, [this] {
			changeTheme();
		});
	}
}

void CALTextPrivate::changeTheme() {
	Q_Q(CALText);

	QPalette palette = q->palette();
	palette.setColor(QPalette::WindowText, themeMode == ALThemeType::Light ? Qt::black : Qt::white);
	q->setPalette(palette);
}

/**
 * @brief \class CALText
 * @param parent pointer to the parent class
 */
CALText::CALText(QWidget* parent): QLabel(parent), d_ptr(new CALTextPrivate(this, this)) {
	Q_D(CALText);

	setObjectName("CALText");
	setStyleSheet("#CALText { background-color: transparent; }");
	QFont textFont = font();
	textFont.setLetterSpacing(QFont::AbsoluteSpacing, d->textSpacing);
	textFont.setPixelSize(28);
	setFont(textFont);
	setWordWrap(true);
	d->textSpacing = 0.5;
	d->isWrapAnyWhere = false;
	d->textStyle = ALTextType::NoStyle;
	d->iconType = ALIcon::None;
	d->themeMode = ALTheme->getThemeMode();
	d->slotThemeChanged(ALTheme->getThemeMode());
	connect(ALTheme, &CALThemeManager::sigThemeModeChanged, d, &CALTextPrivate::slotThemeChanged);
}

CALText::CALText(const QString& text, QWidget* parent): CALText(parent) {
	setText(text);
}

CALText::CALText(const QString& text, const int& pixelSize, QWidget* parent): CALText(text, parent) {
	QFont textFont = this->font();
	textFont.setPixelSize(pixelSize);
	setFont(textFont);
}

CALText::~CALText() = default;

void CALText::setIsWrapAnyWhere(const bool& isWrapAnyWhere) {
	setWordWrap(isWrapAnyWhere);
	d_func()->isWrapAnyWhere = isWrapAnyWhere;
}

bool CALText::getIsWrapAnyWhere() const {
	return d_func()->isWrapAnyWhere;
}

void CALText::setTextPixelSize(const int& pixelSize) {
	QFont font = this->font();
	font.setPixelSize(pixelSize);
	setFont(font);
}

int CALText::getTextPixelSize() const {
	return this->font().pixelSize();
}

void CALText::setTextPointSize(const int& pointSize) {
	QFont font = this->font();
	font.setPointSize(pointSize);
	setFont(font);
}

int CALText::getTextPointSize() const {
	return this->font().pointSize();
}

void CALText::setTextStyle(const ALTextType::TextStyle& textStyle) {
	Q_D(CALText);

	QFont textFont = font();
	d->textStyle = textStyle;
	switch (textStyle) {
		case ALTextType::NoStyle: {
			break;
		}
		case ALTextType::Caption: {
			textFont.setPixelSize(12);
			break;
		}
		case ALTextType::Body: {
			textFont.setPixelSize(13);
			break;
		}
		case ALTextType::BodyStrong: {
			textFont.setPixelSize(13);
			textFont.setWeight(QFont::DemiBold);
			break;
		}
		case ALTextType::Subtitle: {
			textFont.setPixelSize(20);
			textFont.setWeight(QFont::DemiBold);
			break;
		}
		case ALTextType::Title: {
			textFont.setPixelSize(28);
			textFont.setWeight(QFont::DemiBold);
			break;
		}
		case ALTextType::TitleLarge: {
			textFont.setPixelSize(40);
			textFont.setWeight(QFont::DemiBold);
			break;
		}
		case ALTextType::Display: {
			textFont.setPixelSize(48);
			textFont.setWeight(QFont::DemiBold);
			break;
		}
	}
	setFont(textFont);
}

ALTextType::TextStyle CALText::getTextStyle() const {
	return d_func()->textStyle;
}

void CALText::setAweSomeIcon(const ALIcon::AweSomeIcon& icon) {
	this->setProperty(ALIcon::iconProperty, QChar(static_cast<unsigned short>(icon)));
	d_func()->iconType = ALIcon::Awesome;
	update();
	Q_EMIT sigAweSomeIconChanged();
}

ALIcon::AweSomeIcon CALText::getAweSomeIcon() const {
	return static_cast<ALIcon::AweSomeIcon>(this->property(ALIcon::iconProperty).toInt());
}

void CALText::setFluentIcon(const ALIcon::FluentIcon& icon) {
	this->setProperty(ALIcon::iconProperty, QChar(static_cast<unsigned short>(icon)));
	d_func()->iconType = ALIcon::Fluent;
	update();
	Q_EMIT sigFluentIconChanged();
}

ALIcon::FluentIcon CALText::getFluentIcon() const {
	return static_cast<ALIcon::FluentIcon>(this->property(ALIcon::iconProperty).toInt());
}

void CALText::paintEvent(QPaintEvent* event) {
	Q_D(CALText);

	if (d->iconType != ALIcon::None) {
		QPainter painter(this);
		painter.save();
		painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing | QPainter::SmoothPixmapTransform);
		QFont iconFont(ALIcon::getEnumTypeFontName(d->iconType));
		iconFont.setPixelSize(this->font().pixelSize());
		painter.setFont(iconFont);
		painter.setPen(ALThemeColor(d->themeMode, ALThemeType::BasicText));
		painter.drawText(rect(), this->alignment(), QChar(static_cast<unsigned short>(this->property(ALIcon::iconProperty).toInt())));
		painter.restore();
	} else {
		if (wordWrap() && d->isWrapAnyWhere) {
			QPainter painter(this);
			painter.save();
			painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);
			painter.setPen(ALThemeColor(d->themeMode, ALThemeType::BasicText));
			painter.drawText(rect(), this->alignment(), text());
			painter.restore();
		} else {
			QLabel::paintEvent(event);
		}
	}
}
}
