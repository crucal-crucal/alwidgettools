#include "uvpushbutton.hpp"

#include <QPainter>
#include <QPainterPath>

#include "uvpushbutton_p.hpp"
#include "althememanager.hpp"
#include "uvtooltip.hpp"

/**
 * @brief \class CUVPushButtonPrivate
 * Internal class for CUVPushButton
 * @param q pointer to the public class
 * @param parent pointer to the parent class
 */
CUVPushButtonPrivate::CUVPushButtonPrivate(CALPushButton* q, QObject* parent): QObject(parent), q_ptr(q) {
}

CUVPushButtonPrivate::~CUVPushButtonPrivate() = default;

/**
 * @brief \class CALPushButton
 * @param parent pointer to the parent class
 */
CALPushButton::CALPushButton(QWidget* parent): QPushButton(parent), d_ptr(new CUVPushButtonPrivate(this, this)) {
	Q_D(CUVPushButton);

	d->borderRadius = 5;
	d->borderWidth = 0.5;
	d->themeMode = UVTheme->getThemeMode();
	d->lightDefaultColor = UVThemeColor(ALThemeType::Light, ALThemeType::BasicBase);
	d->darkDefaultColor = UVThemeColor(ALThemeType::Dark, ALThemeType::BasicBase);
	d->lightHoverColor = UVThemeColor(ALThemeType::Light, ALThemeType::BasicHover);
	d->darkHoverColor = UVThemeColor(ALThemeType::Dark, ALThemeType::BasicHover);
	d->lightPressedColor = UVThemeColor(ALThemeType::Light, ALThemeType::BasicPress);
	d->darkPressedColor = UVThemeColor(ALThemeType::Dark, ALThemeType::BasicPress);
	d->lightTextColor = UVThemeColor(ALThemeType::Light, ALThemeType::BasicText);
	d->darkTextColor = UVThemeColor(ALThemeType::Dark, ALThemeType::BasicText);
	d->lightBorderColor = UVThemeColor(ALThemeType::Light, ALThemeType::BasicBorder);
	d->darkBorderColor = UVThemeColor(ALThemeType::Dark, ALThemeType::BasicBorder);
	connect(UVTheme, &CUVThemeManager::sigThemeModeChanged, this, [=](const ALThemeType::ThemeMode& mode) { d->themeMode = mode; });

	setMouseTracking(true);
	setFixedHeight(30);
	QFont font = this->font();
	font.setPixelSize(14);
	this->setFont(font);
	setObjectName("CUVPushButton");
	setStyleSheet("#CUVPushButton { background-color: transparent; }");
}

CALPushButton::CALPushButton(const QString& text, QWidget* parent): CALPushButton(parent) {
	setText(text);
}

CALPushButton::~CALPushButton() = default;

void CALPushButton::setButtonStyle(const ButtonStyle& buttonStyle, const bool isEnable) {
	setButtonStyles(isEnable ? d_func()->customButtonStyles | buttonStyle : d_func()->customButtonStyles & ~buttonStyle);
}

void CALPushButton::setButtonStyles(const ButtonStyles& buttonStyles) {
	d_func()->customButtonStyles = buttonStyles;
}

CALPushButton::ButtonStyles CALPushButton::getButtonStyles() const {
	return d_func()->customButtonStyles;
}

void CALPushButton::setBorderRadius(const int borderRadius) {
	Q_D(CUVPushButton);

	d->borderRadius = borderRadius;
	emit sigBorderRadiusChanged();
}

int CALPushButton::getBorderRadius() const {
	return d_func()->borderRadius;
}

void CALPushButton::setLightDefaultColor(const QColor& defaultColor) {
	d_func()->lightDefaultColor = defaultColor;
	emit sigLightDefaultColorChanged();
}

QColor CALPushButton::getLightDefaultColor() const {
	return d_func()->lightDefaultColor;
}

void CALPushButton::setDarkDefaultColor(const QColor& defaultColor) {
	Q_D(CUVPushButton);

	d->darkDefaultColor = defaultColor;
	emit sigDarkDefaultColorChanged();
}

QColor CALPushButton::getDarkDefaultColor() const {
	return d_func()->darkDefaultColor;
}

void CALPushButton::setLightHoverColor(const QColor& hoverColor) {
	d_func()->lightHoverColor = hoverColor;
	Q_EMIT sigLightHoverColorChanged();
}

QColor CALPushButton::getLightHoverColor() const {
	return d_func()->lightHoverColor;
}

void CALPushButton::setDarkHoverColor(const QColor& hoverColor) {
	Q_D(CUVPushButton);

	d->darkHoverColor = hoverColor;
	emit sigDarkHoverColorChanged();
}

QColor CALPushButton::getDarkHoverColor() const {
	return d_func()->darkHoverColor;
}

void CALPushButton::setLightPressedColor(const QColor& pressedColor) {
	d_func()->lightPressedColor = pressedColor;
	Q_EMIT sigLightPressedColorChanged();
}

QColor CALPushButton::getLightPressedColor() const {
	return d_func()->lightPressedColor;
}

void CALPushButton::setDarkPressedColor(const QColor& pressedColor) {
	Q_D(CUVPushButton);

	d->darkPressedColor = pressedColor;
	emit sigDarkPressedColorChanged();
}

QColor CALPushButton::getDarkPressedColor() const {
	return d_func()->darkPressedColor;
}

void CALPushButton::setLightTextColor(const QColor& textColor) {
	d_func()->lightTextColor = textColor;
}

QColor CALPushButton::getLightTextColor() const {
	return d_func()->lightTextColor;
}

void CALPushButton::setDarkTextColor(const QColor& textColor) {
	Q_D(CUVPushButton);

	d->darkTextColor = textColor;
}

QColor CALPushButton::getDarkTextColor() const {
	return d_func()->darkTextColor;
}

void CALPushButton::setShadowBorderWidth(const int shadowBorderWidth) {
	Q_D(CUVPushButton);

	d->shadowBorderWidth = shadowBorderWidth;
}

int CALPushButton::getShadowBorderWidth() const {
	return d_func()->shadowBorderWidth;
}

void CALPushButton::setBorderWidth(const qreal borderwidth) {
	Q_D(CUVPushButton);

	d->borderWidth = borderwidth;
	update();
}

qreal CALPushButton::getBorderWidth() const {
	return d_func()->borderWidth;
}

void CALPushButton::setBorderColor(const QColor& borderColor) {
	Q_D(CUVPushButton);

	d->darkBorderColor = borderColor;
	update();
}

QColor CALPushButton::getBorderColor() const {
	return d_func()->darkBorderColor;
}

void CALPushButton::setTextPixelSize(const int pixelSize) {
	QFont font = this->font();
	font.setPixelSize(pixelSize);
	this->setFont(font);
}

int CALPushButton::getTextPixelSize() const {
	return this->font().pixelSize();
}

void CALPushButton::setTextPointSize(const int size) {
	QFont font = this->font();
	font.setPointSize(size);
	this->setFont(font);
}

int CALPushButton::getTextPointSize() const {
	return this->font().pointSize();
}

void CALPushButton::setToolTip(const QString& tooltip) {
	Q_D(CUVPushButton);

	if (!d->toolTip) {
		d->toolTip = new CUVToolTip(this);
	}

	d->toolTip->setToolTip(tooltip);
}

void CALPushButton::mousePressEvent(QMouseEvent* event) {
	d_func()->isPressed = true;
	QPushButton::mousePressEvent(event);
}

void CALPushButton::mouseReleaseEvent(QMouseEvent* event) {
	d_func()->isPressed = false;
	QPushButton::mouseReleaseEvent(event);
}

void CALPushButton::paintEvent(QPaintEvent* event) {
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
	if (d->themeMode == ALThemeType::Light) {
		painter.setPen(UVThemeColor(d->themeMode, ALThemeType::BasicBorder));
		painter.setBrush(isEnabled() ? d->isPressed ? d->lightPressedColor : (underMouse() ? d->lightHoverColor : d->lightDefaultColor) : UVThemeColor(d->themeMode, ALThemeType::BasicDisable));
	} else {
		painter.setPen(Qt::NoPen);
		painter.setBrush(isEnabled() ? d->isPressed ? d->darkPressedColor : (underMouse() ? d->darkHoverColor : d->darkDefaultColor) : UVThemeColor(d->themeMode, ALThemeType::BasicDisable));
	}
	painter.drawRoundedRect(foregroundRect, d->borderRadius, d->borderRadius);
	// 底边线绘制
	if (!d->isPressed && d->customButtonStyles.testFlag(ButtonStyle::BottomLine)) {
		painter.setPen(UVThemeColor(d->themeMode, ALThemeType::BasicBaseLine));
		painter.drawLine(foregroundRect.x() + d->borderRadius, height() - d->shadowBorderWidth, foregroundRect.width(), height() - d->shadowBorderWidth);
	}
	// 绘制边框
	if (d->customButtonStyles.testFlag(ButtonStyle::Border)) {
		painter.setPen(QPen(UVThemeColor(d->themeMode, ALThemeType::BasicBorder), d->borderWidth));
		painter.drawRoundedRect(foregroundRect, d->borderRadius, d->borderRadius);
	} else {
		painter.setPen(Qt::NoPen);
		painter.drawRoundedRect(foregroundRect, d->borderRadius, d->borderRadius);
	}

	// 文字绘制
	painter.setPen(isEnabled() ? UVThemeColor(d->themeMode, ALThemeType::BasicText) : UVThemeColor(d->themeMode, ALThemeType::BasicTextDisable));
	painter.drawText(foregroundRect, Qt::AlignCenter, text());
	painter.restore();
}
