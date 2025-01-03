#include "uviconbutton.hpp"

#include <QEvent>
#include <QFont>
#include <QPainter>
#include <QPainterPath>
#include <QPropertyAnimation>

#include "uviconbutton_p.hpp"
#include "uvtheme.hpp"
#include "uvtooltip.hpp"

/**
 * @brief \class CUVIconButtonPrivate
 * Internal class for CUVIconButton
 * @param q pointer to the public class
 * @param parent pointer to the parent class
 */
CUVIconButtonPrivate::CUVIconButtonPrivate(CUVIconButton* q, QObject* parent): QObject(parent), q_ptr(q) {
}

CUVIconButtonPrivate::~CUVIconButtonPrivate() = default;

void CUVIconButtonPrivate::setHoverAlpha(const int alpha) {
	hoverAlpha = alpha;
	Q_EMIT sigHoverAlphaChanged();
}

int CUVIconButtonPrivate::getHoverAlpha() const {
	return hoverAlpha;
}

/**
 * @brief \class CUVIconButton
 * @param pix pixmap
 * @param parent pointer to the parent class
 */
CUVIconButton::CUVIconButton(const QPixmap& pix, QWidget* parent): QPushButton(parent), d_ptr(new CUVIconButtonPrivate(this, this)) {
	Q_D(CUVIconButton);

	d->iconPix = pix.copy();
	d->hoverAlpha = 0;
	d->opacity = 1;
	d->lightHoverColor = UVThemeColor(UVThemeType::Light, UVThemeType::BasicHoverAlpha);
	d->darkHoverColor = UVThemeColor(UVThemeType::Dark, UVThemeType::BasicHoverAlpha);
	d->lightIconColor = UVThemeColor(UVThemeType::Light, UVThemeType::BasicText);
	d->darkIconColor = UVThemeColor(UVThemeType::Dark, UVThemeType::BasicText);
	d->lightHoverIconColor = UVThemeColor(UVThemeType::Light, UVThemeType::BasicText);
	d->darkHoverIconColor = UVThemeColor(UVThemeType::Dark, UVThemeType::BasicText);
	d->isSelected = false;
	d->borderRadius = 0;
	d->themeMode = UVTheme->getThemeMode();
	connect(this, &CUVIconButton::sigIsSelectedChanged, this, [=]() { update(); });
	connect(UVTheme, &CUVThemeManager::sigThemeModeChanged, this, [=](const UVThemeType::ThemeMode& mode) {
		d->themeMode = mode;
		update();
	});
}

CUVIconButton::CUVIconButton(const UVIcon::CUVAweSomeIcon& icon, QWidget* parent): CUVIconButton(icon, 15, parent) {
}

CUVIconButton::CUVIconButton(const UVIcon::CUVAweSomeIcon& icon, const int& pixelSize, QWidget* parent): QPushButton(parent), d_ptr(new CUVIconButtonPrivate(this, this)) {
	Q_D(CUVIconButton);

	d->hoverAlpha = 0;
	d->opacity = 1;
	d->lightHoverColor = UVThemeColor(UVThemeType::Light, UVThemeType::BasicHoverAlpha);
	d->darkHoverColor = UVThemeColor(UVThemeType::Dark, UVThemeType::BasicHoverAlpha);
	d->lightIconColor = UVThemeColor(UVThemeType::Light, UVThemeType::BasicText);
	d->darkIconColor = UVThemeColor(UVThemeType::Dark, UVThemeType::BasicText);
	d->lightHoverIconColor = UVThemeColor(UVThemeType::Light, UVThemeType::BasicText);
	d->darkHoverIconColor = UVThemeColor(UVThemeType::Dark, UVThemeType::BasicText);
	d->isSelected = false;
	d->borderRadius = 0;
	d->themeMode = UVTheme->getThemeMode();
	d->tooltip = new CUVToolTip(this);
	auto iconFont = QFont("CUVAwesome");
	iconFont.setPixelSize(pixelSize);
	this->setFont(iconFont);
	d->awesomeIcon = icon;
	this->setText(QChar(static_cast<unsigned short>(icon)));
	connect(this, &CUVIconButton::sigIsSelectedChanged, this, [=]() { update(); });
	connect(UVTheme, &CUVThemeManager::sigThemeModeChanged, this, [=](const UVThemeType::ThemeMode& mode) {
		d->themeMode = mode;
		update();
	});
}

CUVIconButton::CUVIconButton(const UVIcon::CUVAweSomeIcon& icon, const int& pixelSize, const int& fixedWidth, const int& fixedHeight, QWidget* parent): CUVIconButton(icon, pixelSize, parent) {
	this->setFixedSize(fixedWidth, fixedHeight);
}

CUVIconButton::~CUVIconButton() = default;

void CUVIconButton::setCUVIcon(const UVIcon::CUVAweSomeIcon& icon) {
	d_func()->awesomeIcon = icon;
	this->setText(QChar(static_cast<unsigned short>(icon)));
}

UVIcon::CUVAweSomeIcon CUVIconButton::getCUVIcon() const {
	return d_func()->awesomeIcon;
}

void CUVIconButton::setPixmap(const QPixmap& pix) {
	d_func()->iconPix = pix.copy();
}

void CUVIconButton::setBorderRadius(const qreal borderRadius) {
	d_func()->borderRadius = borderRadius;
	Q_EMIT sigBorderRadiusChanged();
}

qreal CUVIconButton::getBorderRadius() const {
	return d_func()->borderRadius;
}

void CUVIconButton::setOpacity(const qreal opacity) {
	d_func()->opacity = opacity;
	Q_EMIT sigOpacityChanged();
}

qreal CUVIconButton::getOpacity() const {
	return d_func()->opacity;
}

void CUVIconButton::setLightHoverColor(const QColor& color) {
	d_func()->lightHoverColor = color;
	Q_EMIT sigLightHoverColorChanged();
}

QColor CUVIconButton::getLightHoverColor() const {
	return d_func()->lightHoverColor;
}

void CUVIconButton::setDarkHoverColor(const QColor& color) {
	d_func()->darkHoverColor = color;
	Q_EMIT sigDarkHoverColorChanged();
}

QColor CUVIconButton::getDarkHoverColor() const {
	return d_func()->darkHoverColor;
}

void CUVIconButton::setLightIconColor(const QColor& color) {
	d_func()->lightIconColor = color;
	Q_EMIT sigLightIconColorChanged();
}

QColor CUVIconButton::getLightIconColor() const {
	return d_func()->lightIconColor;
}

void CUVIconButton::setDarkIconColor(const QColor& color) {
	d_func()->darkIconColor = color;
	Q_EMIT sigDarkIconColorChanged();
}

QColor CUVIconButton::getDarkIconColor() const {
	return d_func()->darkIconColor;
}

void CUVIconButton::setLightHoverIconColor(const QColor& color) {
	d_func()->lightHoverIconColor = color;
	Q_EMIT sigLightHoverIconColorChanged();
}

QColor CUVIconButton::getLightHoverIconColor() const {
	return d_func()->lightHoverIconColor;
}

void CUVIconButton::setDarkHoverIconColor(const QColor& color) {
	d_func()->darkHoverIconColor = color;
	Q_EMIT sigDarkHoverIconColorChanged();
}

QColor CUVIconButton::getDarkHoverIconColor() const {
	return d_func()->darkHoverIconColor;
}

void CUVIconButton::setIsSelected(const bool isSelected) {
	d_func()->isSelected = isSelected;
	Q_EMIT sigIsSelectedChanged();
}

bool CUVIconButton::getIsSelected() const {
	return d_func()->isSelected;
}

void CUVIconButton::setToolTip(const QString& tooltip) {
	d_func()->tooltip->setToolTip(tooltip);
}

bool CUVIconButton::event(QEvent* event) {
	Q_D(CUVIconButton);

	switch (event->type()) {
		case QEvent::Enter: {
			if (isEnabled() && !d->isSelected) {
				d->isAlphaAnimationFinished = false;
				const auto alphaAnimation = new QPropertyAnimation(d, "hoverAlpha");
				connect(alphaAnimation, &QPropertyAnimation::valueChanged, this, [=]() { update(); });
				connect(alphaAnimation, &QPropertyAnimation::finished, this, [=]() { d->isAlphaAnimationFinished = true; });
				alphaAnimation->setDuration(175);
				alphaAnimation->setStartValue(d->hoverAlpha);
				alphaAnimation->setEndValue(d->themeMode == UVThemeType::Light ? d->lightHoverColor.alpha() : d->darkHoverColor.alpha());
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

void CUVIconButton::paintEvent(QPaintEvent* event) {
	Q_D(CUVIconButton);

	QPainter painter(this);
	painter.save();
	painter.setOpacity(d->opacity);
	painter.setRenderHints(QPainter::SmoothPixmapTransform | QPainter::TextAntialiasing | QPainter::Antialiasing);
	painter.setPen(Qt::NoPen);

	if (d->isAlphaAnimationFinished || d->isSelected) {
		painter.setBrush(d->isSelected ? d->themeMode == UVThemeType::Light ? d->lightHoverColor : d->darkHoverColor : isEnabled() ? underMouse() ? d->themeMode == UVThemeType::Light ? d->lightHoverColor : d->darkHoverColor : Qt::transparent : Qt::transparent);
	} else {
		QColor hoverColor = d->themeMode == UVThemeType::Light ? d->lightHoverColor : d->darkHoverColor;
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
		painter.setPen(isEnabled() ? d->themeMode == UVThemeType::Light ? underMouse() ? d->lightHoverIconColor : d->lightIconColor : underMouse() ? d->darkHoverIconColor : d->darkIconColor : UVThemeColor(d->themeMode, UVThemeType::BasicTextDisable));
		painter.drawText(rect(), Qt::AlignCenter, QChar(static_cast<unsigned short>(d->awesomeIcon)));
	}

	painter.restore();
}
