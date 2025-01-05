#include "uvpushbutton.hpp"

#include <QPainter>
#include <QPainterPath>

#include "uvpushbutton_p.hpp"
#include "uvthememanager.hpp"
#include "uvtooltip.hpp"

/**
 * @brief \class CUVPushButtonPrivate
 * Internal class for CUVPushButton
 * @param q pointer to the public class
 * @param parent pointer to the parent class
 */
CUVPushButtonPrivate::CUVPushButtonPrivate(CUVPushButton* q, QObject* parent): QObject(parent), q_ptr(q) {
}

CUVPushButtonPrivate::~CUVPushButtonPrivate() = default;

/**
 * @brief \class CUVPushButton
 * @param parent pointer to the parent class
 */
CUVPushButton::CUVPushButton(QWidget* parent): QPushButton(parent), d_ptr(new CUVPushButtonPrivate(this, this)) {
	Q_D(CUVPushButton);

	d->borderRadius = 5;
	d->borderWidth = 0.5;
	d->themeMode = UVTheme->getThemeMode();
	d->lightDefaultColor = UVThemeColor(UVThemeType::Light, UVThemeType::BasicBase);
	d->darkDefaultColor = UVThemeColor(UVThemeType::Dark, UVThemeType::BasicBase);
	d->lightHoverColor = UVThemeColor(UVThemeType::Light, UVThemeType::BasicHover);
	d->darkHoverColor = UVThemeColor(UVThemeType::Dark, UVThemeType::BasicHover);
	d->lightPressedColor = UVThemeColor(UVThemeType::Light, UVThemeType::BasicPress);
	d->darkPressedColor = UVThemeColor(UVThemeType::Dark, UVThemeType::BasicPress);
	d->lightTextColor = UVThemeColor(UVThemeType::Light, UVThemeType::BasicText);
	d->darkTextColor = UVThemeColor(UVThemeType::Dark, UVThemeType::BasicText);
	d->lightBorderColor = UVThemeColor(UVThemeType::Light, UVThemeType::BasicBorder);
	d->darkBorderColor = UVThemeColor(UVThemeType::Dark, UVThemeType::BasicBorder);
	connect(UVTheme, &CUVThemeManager::sigThemeModeChanged, this, [=](const UVThemeType::ThemeMode& mode) { d->themeMode = mode; });
	d->toolTip = new CUVToolTip(this);

	setMouseTracking(true);
	setFixedHeight(30);
	QFont font = this->font();
	font.setPixelSize(14);
	this->setFont(font);
	setObjectName("CUVPushButton");
	setStyleSheet("#CUVPushButton { background-color: transparent; }");
}

CUVPushButton::CUVPushButton(const QString& text, QWidget* parent): CUVPushButton(parent) {
	setText(text);
}

CUVPushButton::~CUVPushButton() = default;

void CUVPushButton::setButtonStyle(const ButtonStyle& buttonStyle, const bool isEnable) {
	setButtonStyles(isEnable ? d_func()->customButtonStyles | buttonStyle : d_func()->customButtonStyles & ~buttonStyle);
}

void CUVPushButton::setButtonStyles(const ButtonStyles& buttonStyles) {
	d_func()->customButtonStyles = buttonStyles;
}

CUVPushButton::ButtonStyles CUVPushButton::getButtonStyles() const {
	return d_func()->customButtonStyles;
}

void CUVPushButton::setBorderRadius(const int borderRadius) {
	Q_D(CUVPushButton);

	d->borderRadius = borderRadius;
	emit sigBorderRadiusChanged();
}

int CUVPushButton::getBorderRadius() const {
	return d_func()->borderRadius;
}

void CUVPushButton::setLightDefaultColor(const QColor& defaultColor) {
	d_func()->lightDefaultColor = defaultColor;
	emit sigLightDefaultColorChanged();
}

QColor CUVPushButton::getLightDefaultColor() const {
	return d_func()->lightDefaultColor;
}

void CUVPushButton::setDarkDefaultColor(const QColor& defaultColor) {
	Q_D(CUVPushButton);

	d->darkDefaultColor = defaultColor;
	emit sigDarkDefaultColorChanged();
}

QColor CUVPushButton::getDarkDefaultColor() const {
	return d_func()->darkDefaultColor;
}

void CUVPushButton::setLightHoverColor(const QColor& hoverColor) {
	d_func()->lightHoverColor = hoverColor;
	Q_EMIT sigLightHoverColorChanged();
}

QColor CUVPushButton::getLightHoverColor() const {
	return d_func()->lightHoverColor;
}

void CUVPushButton::setDarkHoverColor(const QColor& hoverColor) {
	Q_D(CUVPushButton);

	d->darkHoverColor = hoverColor;
	emit sigDarkHoverColorChanged();
}

QColor CUVPushButton::getDarkHoverColor() const {
	return d_func()->darkHoverColor;
}

void CUVPushButton::setLightPressedColor(const QColor& pressedColor) {
	d_func()->lightPressedColor = pressedColor;
	Q_EMIT sigLightPressedColorChanged();
}

QColor CUVPushButton::getLightPressedColor() const {
	return d_func()->lightPressedColor;
}

void CUVPushButton::setDarkPressedColor(const QColor& pressedColor) {
	Q_D(CUVPushButton);

	d->darkPressedColor = pressedColor;
	emit sigDarkPressedColorChanged();
}

QColor CUVPushButton::getDarkPressedColor() const {
	return d_func()->darkPressedColor;
}

void CUVPushButton::setLightTextColor(const QColor& textColor) {
	d_func()->lightTextColor = textColor;
}

QColor CUVPushButton::getLightTextColor() const {
	return d_func()->lightTextColor;
}

void CUVPushButton::setDarkTextColor(const QColor& textColor) {
	Q_D(CUVPushButton);

	d->darkTextColor = textColor;
}

QColor CUVPushButton::getDarkTextColor() const {
	return d_func()->darkTextColor;
}

void CUVPushButton::setShadowBorderWidth(const int shadowBorderWidth) {
	Q_D(CUVPushButton);

	d->shadowBorderWidth = shadowBorderWidth;
}

int CUVPushButton::getShadowBorderWidth() const {
	return d_func()->shadowBorderWidth;
}

void CUVPushButton::setBorderWidth(const qreal borderwidth) {
	Q_D(CUVPushButton);

	d->borderWidth = borderwidth;
	update();
}

qreal CUVPushButton::getBorderWidth() const {
	return d_func()->borderWidth;
}

void CUVPushButton::setBorderColor(const QColor& borderColor) {
	Q_D(CUVPushButton);

	d->darkBorderColor = borderColor;
	update();
}

QColor CUVPushButton::getBorderColor() const {
	return d_func()->darkBorderColor;
}

void CUVPushButton::setTextPixelSize(const int pixelSize) {
	QFont font = this->font();
	font.setPixelSize(pixelSize);
	this->setFont(font);
}

int CUVPushButton::getTextPixelSize() const {
	return this->font().pixelSize();
}

void CUVPushButton::setTextPointSize(const int size) {
	QFont font = this->font();
	font.setPointSize(size);
	this->setFont(font);
}

int CUVPushButton::getTextPointSize() const {
	return this->font().pointSize();
}

void CUVPushButton::setToolTip(const QString& tooltip) {
	d_func()->toolTip->setToolTip(tooltip);
}

void CUVPushButton::mousePressEvent(QMouseEvent* event) {
	d_func()->isPressed = true;
	QPushButton::mousePressEvent(event);
}

void CUVPushButton::mouseReleaseEvent(QMouseEvent* event) {
	d_func()->isPressed = false;
	QPushButton::mouseReleaseEvent(event);
}

void CUVPushButton::paintEvent(QPaintEvent* event) {
	Q_D(CUVPushButton);
	QPainter painter(this);
	painter.save();
	painter.setRenderHints(QPainter::SmoothPixmapTransform | QPainter::Antialiasing | QPainter::TextAntialiasing);
	// 阴影绘制
	if (d->customButtonStyles.testFlag(ButtonStyle::Shadow)) {
		UVTheme->drawEffectShadow(&painter, rect(), d->shadowBorderWidth, d->borderRadius);
	}
	// 背景绘制
	const QRect foregroundRect(d->shadowBorderWidth, d->shadowBorderWidth, width() - 2 * (d->shadowBorderWidth), height() - 2 * d->shadowBorderWidth);
	if (d->themeMode == UVThemeType::Light) {
		painter.setPen(UVThemeColor(d->themeMode, UVThemeType::BasicBorder));
		painter.setBrush(isEnabled() ? d->isPressed ? d->lightPressedColor : (underMouse() ? d->lightHoverColor : d->lightDefaultColor) : UVThemeColor(d->themeMode, UVThemeType::BasicDisable));
	} else {
		painter.setPen(Qt::NoPen);
		painter.setBrush(isEnabled() ? d->isPressed ? d->darkPressedColor : (underMouse() ? d->darkHoverColor : d->darkDefaultColor) : UVThemeColor(d->themeMode, UVThemeType::BasicDisable));
	}
	painter.drawRoundedRect(foregroundRect, d->borderRadius, d->borderRadius);
	// 底边线绘制
	if (!d->isPressed && d->customButtonStyles.testFlag(ButtonStyle::BottomLine)) {
		painter.setPen(UVThemeColor(d->themeMode, UVThemeType::BasicBaseLine));
		painter.drawLine(foregroundRect.x() + d->borderRadius, height() - d->shadowBorderWidth, foregroundRect.width(), height() - d->shadowBorderWidth);
	}
	// 绘制边框
	if (d->customButtonStyles.testFlag(ButtonStyle::Border)) {
		painter.setPen(QPen(UVThemeColor(d->themeMode, UVThemeType::BasicBorder), d->borderWidth));
		painter.drawRoundedRect(foregroundRect, d->borderRadius, d->borderRadius);
	} else {
		painter.setPen(Qt::NoPen);
		painter.drawRoundedRect(foregroundRect, d->borderRadius, d->borderRadius);
	}

	// 文字绘制
	painter.setPen(isEnabled() ? UVThemeColor(d->themeMode, UVThemeType::BasicText) : UVThemeColor(d->themeMode, UVThemeType::BasicTextDisable));
	painter.drawText(foregroundRect, Qt::AlignCenter, text());
	painter.restore();
}
