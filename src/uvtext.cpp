#include "uvtext.hpp"

#include <QPainter>
#include <QPalette>

#include "uvtext_p.hpp"
#include "uvthememanager.hpp"

/**
 * @brief \class CUVTextPrivate
 * Internal class for CUVText
 * @param q pointer to the public class
 * @param parent pointer to the parent class
 */
CUVTextPrivate::CUVTextPrivate(CUVText* q, QObject* parent): QObject(parent), q_ptr(q) {
}

CUVTextPrivate::~CUVTextPrivate() = default;

void CUVTextPrivate::slotThemeChanged(const UVThemeType::ThemeMode& mode) {
	Q_Q(CUVText);

	themeMode = mode;
	QPalette palette = q->palette();
	palette.setColor(QPalette::WindowText, themeMode == UVThemeType::Light ? Qt::black : Qt::white);
	q->setPalette(palette);
}

/**
 * @brief \class CUVText
 * @param parent pointer to the parent class
 */
CUVText::CUVText(QWidget* parent): QLabel(parent), d_ptr(new CUVTextPrivate(this, this)) {
	Q_D(CUVText);

	setObjectName("CUVText");
	setStyleSheet("#CUVText { background-color: transparent; }");
	QFont textFont = font();
	textFont.setLetterSpacing(QFont::AbsoluteSpacing, d->textSpacing);
	textFont.setPixelSize(28);
	setFont(textFont);
	setWordWrap(true);
	d->themeMode = UVTheme->getThemeMode();
	d->slotThemeChanged(UVTheme->getThemeMode());
	connect(UVTheme, &CUVThemeManager::sigThemeModeChanged, d, &CUVTextPrivate::slotThemeChanged);
}

CUVText::CUVText(const QString& text, QWidget* parent): CUVText(parent) {
	setText(text);
}

CUVText::CUVText(const QString& text, const int& pixelSize, QWidget* parent): CUVText(text, parent) {
	QFont textFont = this->font();
	textFont.setPixelSize(pixelSize);
	setFont(textFont);
}

CUVText::~CUVText() = default;

void CUVText::setIsWrapAnyWhere(const bool& isWrapAnyWhere) {
	setWordWrap(isWrapAnyWhere);
	d_func()->isWrapAnyWhere = isWrapAnyWhere;
}

bool CUVText::getIsWrapAnyWhere() const {
	return d_func()->isWrapAnyWhere;
}

void CUVText::setTextPixelSize(const int& pixelSize) {
	QFont font = this->font();
	font.setPixelSize(pixelSize);
	setFont(font);
}

int CUVText::getTextPixelSize() const {
	return this->font().pixelSize();
}

void CUVText::setTextPointSize(const int& pointSize) {
	QFont font = this->font();
	font.setPointSize(pointSize);
	setFont(font);
}

int CUVText::getTextPointSize() const {
	return this->font().pointSize();
}

void CUVText::setTextStyle(const UVTextType::TextStyle& textStyle) {
	Q_D(CUVText);

	QFont textFont = font();
	d->textStyle = textStyle;
	switch (textStyle) {
		case UVTextType::NoStyle: {
			break;
		}
		case UVTextType::Caption: {
			textFont.setPixelSize(12);
			break;
		}
		case UVTextType::Body: {
			textFont.setPixelSize(13);
			break;
		}
		case UVTextType::BodyStrong: {
			textFont.setPixelSize(13);
			textFont.setWeight(QFont::DemiBold);
			break;
		}
		case UVTextType::Subtitle: {
			textFont.setPixelSize(20);
			textFont.setWeight(QFont::DemiBold);
			break;
		}
		case UVTextType::Title: {
			textFont.setPixelSize(28);
			textFont.setWeight(QFont::DemiBold);
			break;
		}
		case UVTextType::TitleLarge: {
			textFont.setPixelSize(40);
			textFont.setWeight(QFont::DemiBold);
			break;
		}
		case UVTextType::Display: {
			textFont.setPixelSize(48);
			textFont.setWeight(QFont::DemiBold);
			break;
		}
	}
	setFont(textFont);
}

UVTextType::TextStyle CUVText::getTextStyle() const {
	return d_func()->textStyle;
}

void CUVText::paintEvent(QPaintEvent* event) {
	Q_D(CUVText);

	if (wordWrap() && d->isWrapAnyWhere) {
		QPainter painter(this);
		painter.save();
		painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);
		painter.setPen(UVThemeColor(d->themeMode, UVThemeType::BasicText));
		painter.drawText(rect(), this->alignment(), text());
		painter.restore();
	} else {
		QLabel::paintEvent(event);
	}
}
