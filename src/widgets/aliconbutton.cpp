#include "aliconbutton.hpp"

#include <QEvent>
#include <QFont>
#include <QPainter>
#include <QPainterPath>
#include <QPropertyAnimation>

#include "aliconbutton_p.hpp"
#include "althememanager.hpp"
#include "altooltip.hpp"

/**
 * @brief \namespace AL
 */
namespace AL {
/**
 * @brief \class CALIconButtonPrivate
 * Internal class for CALIconButton
 * @param q pointer to the public class
 * @param parent pointer to the parent class
 */
CALIconButtonPrivate::CALIconButtonPrivate(CALIconButton* q, QObject* parent): QObject(parent), q_ptr(q) {
}

CALIconButtonPrivate::~CALIconButtonPrivate() = default;

void CALIconButtonPrivate::setHoverAlpha(const int alpha) {
	hoverAlpha = alpha;
	Q_EMIT sigHoverAlphaChanged();
}

int CALIconButtonPrivate::getHoverAlpha() const {
	return hoverAlpha;
}

/**
 * @brief \class CALIconButton
 * @param pix pixmap
 * @param parent pointer to the parent class
 */
CALIconButton::CALIconButton(const QPixmap& pix, QWidget* parent): QPushButton(parent), d_ptr(new CALIconButtonPrivate(this, this)) {
	Q_D(CALIconButton);

	d->iconPix = pix.copy();
	d->hoverAlpha = 0;
	d->opacity = 1;
	d->lightHoverColor = ALThemeColor(ALThemeType::Light, ALThemeType::BasicHoverAlpha);
	d->darkHoverColor = ALThemeColor(ALThemeType::Dark, ALThemeType::BasicHoverAlpha);
	d->lightIconColor = ALThemeColor(ALThemeType::Light, ALThemeType::BasicText);
	d->darkIconColor = ALThemeColor(ALThemeType::Dark, ALThemeType::BasicText);
	d->lightHoverIconColor = ALThemeColor(ALThemeType::Light, ALThemeType::BasicText);
	d->darkHoverIconColor = ALThemeColor(ALThemeType::Dark, ALThemeType::BasicText);
	d->isSelected = false;
	d->borderRadius = 0;
	d->themeMode = ALTheme->getThemeMode();
	connect(this, &CALIconButton::sigIsSelectedChanged, this, [=]() { update(); });
	connect(ALTheme, &CALThemeManager::sigThemeModeChanged, this, [=](const ALThemeType::ThemeMode& mode) {
		d->themeMode = mode;
		update();
	});
}

CALIconButton::CALIconButton(const ALIcon::AweSomeIcon& icon, QWidget* parent): CALIconButton(icon, 15, parent) {
}

CALIconButton::CALIconButton(const ALIcon::AweSomeIcon& icon, const int& pixelSize, QWidget* parent): QPushButton(parent), d_ptr(new CALIconButtonPrivate(this, this)) {
	Q_D(CALIconButton);

	d->hoverAlpha = 0;
	d->opacity = 1;
	d->lightHoverColor = ALThemeColor(ALThemeType::Light, ALThemeType::BasicHoverAlpha);
	d->darkHoverColor = ALThemeColor(ALThemeType::Dark, ALThemeType::BasicHoverAlpha);
	d->lightIconColor = ALThemeColor(ALThemeType::Light, ALThemeType::BasicText);
	d->darkIconColor = ALThemeColor(ALThemeType::Dark, ALThemeType::BasicText);
	d->lightHoverIconColor = ALThemeColor(ALThemeType::Light, ALThemeType::BasicText);
	d->darkHoverIconColor = ALThemeColor(ALThemeType::Dark, ALThemeType::BasicText);
	d->isSelected = false;
	d->borderRadius = 0;
	d->themeMode = ALTheme->getThemeMode();
	d->iconType = ALIcon::Awesome;
	QFont iconFont(ALIcon::getEnumTypeFontName(d->iconType));
	iconFont.setPixelSize(pixelSize);
	this->setFont(iconFont);
	setProperty("CALIcon", QChar(static_cast<unsigned short>(icon)));
	this->setText(QChar(static_cast<unsigned short>(icon)));
	connect(this, &CALIconButton::sigIsSelectedChanged, this, [=]() { update(); });
	connect(ALTheme, &CALThemeManager::sigThemeModeChanged, this, [=](const ALThemeType::ThemeMode& mode) {
		d->themeMode = mode;
		update();
	});
}

CALIconButton::CALIconButton(const ALIcon::AweSomeIcon& icon, const int& pixelSize, const int& fixedWidth, const int& fixedHeight, QWidget* parent): CALIconButton(icon, pixelSize, parent) {
	this->setFixedSize(fixedWidth, fixedHeight);
}

CALIconButton::CALIconButton(const ALIcon::FluentIcon& icon, QWidget* parent): CALIconButton(icon, 15, parent) {
}

CALIconButton::CALIconButton(const ALIcon::FluentIcon& icon, const int& pixelSize, QWidget* parent): QPushButton(parent), d_ptr(new CALIconButtonPrivate(this, this)) {
	Q_D(CALIconButton);

	d->hoverAlpha = 0;
	d->opacity = 1;
	d->lightHoverColor = ALThemeColor(ALThemeType::Light, ALThemeType::BasicHoverAlpha);
	d->darkHoverColor = ALThemeColor(ALThemeType::Dark, ALThemeType::BasicHoverAlpha);
	d->lightIconColor = ALThemeColor(ALThemeType::Light, ALThemeType::BasicText);
	d->darkIconColor = ALThemeColor(ALThemeType::Dark, ALThemeType::BasicText);
	d->lightHoverIconColor = ALThemeColor(ALThemeType::Light, ALThemeType::BasicText);
	d->darkHoverIconColor = ALThemeColor(ALThemeType::Dark, ALThemeType::BasicText);
	d->isSelected = false;
	d->borderRadius = 0;
	d->themeMode = ALTheme->getThemeMode();
	d->iconType = ALIcon::Fluent;
	QFont iconFont(ALIcon::getEnumTypeFontName(d->iconType));
	iconFont.setPixelSize(pixelSize);
	this->setFont(iconFont);
	setProperty("CALIcon", QChar(static_cast<unsigned short>(icon)));
	this->setText(QChar(static_cast<unsigned short>(icon)));
	connect(this, &CALIconButton::sigIsSelectedChanged, this, [=]() { update(); });
	connect(ALTheme, &CALThemeManager::sigThemeModeChanged, this, [=](const ALThemeType::ThemeMode& mode) {
		d->themeMode = mode;
		update();
	});
}

CALIconButton::CALIconButton(const ALIcon::FluentIcon& icon, const int& pixelSize, const int& fixedWidth, const int& fixedHeight, QWidget* parent): CALIconButton(icon, pixelSize, parent) {
	this->setFixedSize(fixedWidth, fixedHeight);
}

CALIconButton::~CALIconButton() = default;

void CALIconButton::setAweSomeIcon(const ALIcon::AweSomeIcon& icon) {
	this->setProperty("CALIcon", QChar(static_cast<unsigned short>(icon)));
	this->setText(QChar(static_cast<unsigned short>(icon)));
}

ALIcon::AweSomeIcon CALIconButton::getAweSomeIcon() const {
	return static_cast<ALIcon::AweSomeIcon>(this->property("CALIcon").toInt());
}

void CALIconButton::setFluentIcon(const ALIcon::FluentIcon& icon) {
	this->setProperty("CALIcon", QChar(static_cast<unsigned short>(icon)));
	this->setText(QChar(static_cast<unsigned short>(icon)));
}

ALIcon::FluentIcon CALIconButton::getFluentIcon() const {
	return static_cast<ALIcon::FluentIcon>(this->property("CALIcon").toInt());
}

void CALIconButton::setPixmap(const QPixmap& pix) {
	d_func()->iconPix = pix.copy();
}

void CALIconButton::setBorderRadius(const qreal borderRadius) {
	d_func()->borderRadius = borderRadius;
	Q_EMIT sigBorderRadiusChanged();
}

qreal CALIconButton::getBorderRadius() const {
	return d_func()->borderRadius;
}

void CALIconButton::setOpacity(const qreal opacity) {
	d_func()->opacity = opacity;
	Q_EMIT sigOpacityChanged();
}

qreal CALIconButton::getOpacity() const {
	return d_func()->opacity;
}

void CALIconButton::setLightHoverColor(const QColor& color) {
	d_func()->lightHoverColor = color;
	Q_EMIT sigLightHoverColorChanged();
}

QColor CALIconButton::getLightHoverColor() const {
	return d_func()->lightHoverColor;
}

void CALIconButton::setDarkHoverColor(const QColor& color) {
	d_func()->darkHoverColor = color;
	Q_EMIT sigDarkHoverColorChanged();
}

QColor CALIconButton::getDarkHoverColor() const {
	return d_func()->darkHoverColor;
}

void CALIconButton::setLightIconColor(const QColor& color) {
	d_func()->lightIconColor = color;
	Q_EMIT sigLightIconColorChanged();
}

QColor CALIconButton::getLightIconColor() const {
	return d_func()->lightIconColor;
}

void CALIconButton::setDarkIconColor(const QColor& color) {
	d_func()->darkIconColor = color;
	Q_EMIT sigDarkIconColorChanged();
}

QColor CALIconButton::getDarkIconColor() const {
	return d_func()->darkIconColor;
}

void CALIconButton::setLightHoverIconColor(const QColor& color) {
	d_func()->lightHoverIconColor = color;
	Q_EMIT sigLightHoverIconColorChanged();
}

QColor CALIconButton::getLightHoverIconColor() const {
	return d_func()->lightHoverIconColor;
}

void CALIconButton::setDarkHoverIconColor(const QColor& color) {
	d_func()->darkHoverIconColor = color;
	Q_EMIT sigDarkHoverIconColorChanged();
}

QColor CALIconButton::getDarkHoverIconColor() const {
	return d_func()->darkHoverIconColor;
}

void CALIconButton::setIsSelected(const bool isSelected) {
	d_func()->isSelected = isSelected;
	Q_EMIT sigIsSelectedChanged();
}

bool CALIconButton::getIsSelected() const {
	return d_func()->isSelected;
}

void CALIconButton::setToolTip(const QString& tooltip) {
	Q_D(CALIconButton);

	if (!d->tooltip) {
		d->tooltip = new CALToolTip(this);
	}
	d->tooltip->setToolTip(tooltip);
}

bool CALIconButton::event(QEvent* event) {
	Q_D(CALIconButton);

	switch (event->type()) {
		case QEvent::Enter: {
			if (isEnabled() && !d->isSelected) {
				d->isAlphaAnimationFinished = false;
				const auto alphaAnimation = new QPropertyAnimation(d, "hoverAlpha");
				connect(alphaAnimation, &QPropertyAnimation::valueChanged, this, [=]() { update(); });
				connect(alphaAnimation, &QPropertyAnimation::finished, this, [=]() { d->isAlphaAnimationFinished = true; });
				alphaAnimation->setDuration(175);
				alphaAnimation->setStartValue(d->hoverAlpha);
				alphaAnimation->setEndValue(d->themeMode == ALThemeType::Light ? d->lightHoverColor.alpha() : d->darkHoverColor.alpha());
				alphaAnimation->start(QAbstractAnimation::DeleteWhenStopped);
			}
			break;
		}
		case QEvent::Leave: {
			if (isEnabled() && !d->isSelected) {
				d->isAlphaAnimationFinished = false;
				const auto alphaAnimation = new QPropertyAnimation(d, "hoverAlpha");
				connect(alphaAnimation, &QPropertyAnimation::valueChanged, this, [=]() { update(); });
				connect(alphaAnimation, &QPropertyAnimation::finished, this, [=]() { d->isAlphaAnimationFinished = true; });
				alphaAnimation->setDuration(175);
				alphaAnimation->setStartValue(d->hoverAlpha);
				alphaAnimation->setEndValue(0);
				alphaAnimation->start(QAbstractAnimation::DeleteWhenStopped);
			}
			break;
		}
		default: break;
	}

	return QPushButton::event(event);
}

void CALIconButton::paintEvent(QPaintEvent* event) {
	Q_D(CALIconButton);

	QPainter painter(this);
	painter.save();
	painter.setOpacity(d->opacity);
	painter.setRenderHints(QPainter::SmoothPixmapTransform | QPainter::TextAntialiasing | QPainter::Antialiasing);
	painter.setPen(Qt::NoPen);

	if (d->isAlphaAnimationFinished || d->isSelected) {
		painter.setBrush(d->isSelected ? d->themeMode == ALThemeType::Light ? d->lightHoverColor : d->darkHoverColor : isEnabled() ? underMouse() ? d->themeMode == ALThemeType::Light ? d->lightHoverColor : d->darkHoverColor : Qt::transparent : Qt::transparent);
	} else {
		QColor hoverColor = d->themeMode == ALThemeType::Light ? d->lightHoverColor : d->darkHoverColor;
		hoverColor.setAlpha(d->hoverAlpha);
		painter.setBrush(hoverColor);
	}
	painter.drawRoundedRect(rect(), d->borderRadius, d->borderRadius);

	// 图标绘制
	if (!d->iconPix.isNull()) {
		QPainterPath path;
		path.addEllipse(rect());
		painter.setClipPath(path);
		painter.drawPixmap(rect(), d->iconPix);
	} else {
		painter.setPen(isEnabled() ? d->themeMode == ALThemeType::Light ? underMouse() ? d->lightHoverIconColor : d->lightIconColor : underMouse() ? d->darkHoverIconColor : d->darkIconColor : ALThemeColor(d->themeMode, ALThemeType::BasicTextDisable));
		painter.drawText(rect(), Qt::AlignCenter, this->property("CALIcon").toString());
	}

	painter.restore();
}
}
