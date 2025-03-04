#include "alawesomebutton.hpp"

#include <QDebug>
#include <QPainter>

#include "alawesomebutton_p.hpp"
#include "altooltip.hpp"

/**
 * @brief \namespace AL
 */
namespace AL {
/**
 * @brief \class CALAwesomeButtonPrivate
 * Internal class for CALAwesomeButton
 * @param q pointer to the public class
 * @param parent pointer to the parent class
 */
CALAwesomeButtonPrivate::CALAwesomeButtonPrivate(CALAwesomeButton* q, QObject* parent): QObject(parent), q_ptr(q) {
}

CALAwesomeButtonPrivate::~CALAwesomeButtonPrivate() = default;

void CALAwesomeButtonPrivate::init() {
	Q_Q(CALAwesomeButton);

	iconFont = QFont(ALIcon::getEnumTypeFontName(ALIcon::Awesome));
	lightHoverColor = QColor(0xEA, 0xE9, 0xF0);
	darkHoverColor = QColor(0x2F, 0x2E, 0x2C);
	lightIconColor = Qt::black;
	darkIconColor = Qt::white;
	ligntSelectedColor = QColor(0x00, 0x7A, 0xFF);
	darkSelectedColor = QColor(0x00, 0x7A, 0xFF);
	isSelected = false;
	borderRaius = 0;
	opacity = 1;
}

/**
 * @brief \class CALAwesomeButton
 * @param iconType Awesome icon type
 * @param parent pointer to the parent class
 */
CALAwesomeButton::CALAwesomeButton(const ALIcon::AweSomeIcon& iconType, QWidget* parent)
: QPushButton(parent), d_ptr(new CALAwesomeButtonPrivate(this, this)) {
	Q_D(CALAwesomeButton);

	d->init();
	d->iconFont.setPixelSize(15);
	this->setFont(d->iconFont);
	d->iconType = iconType;
	this->setText(QChar(static_cast<ushort>(iconType)));
	connect(this, &CALAwesomeButton::sigIsSelectedChanged, this, [this]() { this->update(); });
}

/**
 * @param iconType Awewome icon type
 * @param pixelSize font pixel size
 * @param parent pointer to the parent class
 */
CALAwesomeButton::CALAwesomeButton(const ALIcon::AweSomeIcon& iconType, const int pixelSize, QWidget* parent)
: QPushButton(parent), d_ptr(new CALAwesomeButtonPrivate(this, this)) {
	Q_D(CALAwesomeButton);

	d->init();
	d->iconFont.setPixelSize(pixelSize);
	this->setFont(d->iconFont);
	d->iconType = iconType;
	this->setText(QChar(static_cast<unsigned short>(iconType)));
	connect(this, &CALAwesomeButton::sigIsSelectedChanged, this, [this]() { this->update(); });
}

/**
 * @param iconType Awewome icon type
 * @param pixelSize font pixel size
 * @param fixedWidth fiexed button width
 * @param fixedHeight fixed button height
 * @param parent pointer to the parent class
 */
CALAwesomeButton::CALAwesomeButton(const ALIcon::AweSomeIcon& iconType, const int pixelSize, const int fixedWidth, const int fixedHeight, QWidget* parent)
: CALAwesomeButton(iconType, pixelSize, parent) {
	this->setFixedSize(fixedWidth, fixedHeight);
}

CALAwesomeButton::~CALAwesomeButton() = default;

void CALAwesomeButton::setAwesomeIcon(const ALIcon::AweSomeIcon& iconType) {
	Q_D(CALAwesomeButton);

	if (d->iconType != iconType) {
		d->iconType = iconType;
		this->setText(QChar(static_cast<ushort>(iconType)));
	}
}

ALIcon::AweSomeIcon CALAwesomeButton::getAwesomeIcon() const {
	return d_func()->iconType;
}

void CALAwesomeButton::setBorderRadius(const int radius) {
	Q_D(CALAwesomeButton);

	if (d->borderRaius != radius) {
		d->borderRaius = radius;
		Q_EMIT sigBorderRadiusChanged(radius);
		update();
	}
}

int CALAwesomeButton::getBorderRadius() const {
	return d_func()->borderRaius;
}

void CALAwesomeButton::setLightHoverColor(const QColor& color) {
	Q_D(CALAwesomeButton);

	if (d->lightHoverColor != color) {
		d->lightHoverColor = color;
		Q_EMIT sigLightHoverColorChanged(color);
		update();
	}
}

QColor CALAwesomeButton::getLightHoverColor() const {
	return d_func()->lightHoverColor;
}

void CALAwesomeButton::setDarkHoverColor(const QColor& color) {
	Q_D(CALAwesomeButton);

	if (d->darkHoverColor != color) {
		d->darkHoverColor = color;
		Q_EMIT sigDarkHoverColorChanged(color);
		update();
	}
}

QColor CALAwesomeButton::getDarkHoverColor() const {
	return d_func()->darkHoverColor;
}

void CALAwesomeButton::setLightIconColor(const QColor& color) {
	Q_D(CALAwesomeButton);

	if (d->lightIconColor != color) {
		d->lightIconColor = color;
		Q_EMIT sigLightIconColorChanged(color);
		update();
	}
}

QColor CALAwesomeButton::getLightIconColor() const {
	return d_func()->lightIconColor;
}

void CALAwesomeButton::setDarkIconColor(const QColor& color) {
	Q_D(CALAwesomeButton);

	if (d->darkIconColor != color) {
		d->darkIconColor = color;
		Q_EMIT sigDarkIconColorChanged(color);
		update();
	}
}

QColor CALAwesomeButton::getDarkIconColor() const {
	return d_func()->darkIconColor;
}

void CALAwesomeButton::setLigntSelectedColor(const QColor& color) {
	Q_D(CALAwesomeButton);

	if (d->ligntSelectedColor != color) {
		d->ligntSelectedColor = color;
		Q_EMIT sigLigntSelectedColorChanged(color);
		update();
	}
}

QColor CALAwesomeButton::getLigntSelectedColor() const {
	return d_func()->ligntSelectedColor;
}

void CALAwesomeButton::setDarkSelectedColor(const QColor& color) {
	Q_D(CALAwesomeButton);

	if (d->darkSelectedColor != color) {
		d->darkSelectedColor = color;
		Q_EMIT sigDarkSelectedColorChanged(color);
		update();
	}
}

QColor CALAwesomeButton::getDarkSelectedColor() const {
	return d_func()->darkSelectedColor;
}

void CALAwesomeButton::setIsSelected(const bool isSelected) {
	Q_D(CALAwesomeButton);

	if (d->isSelected != isSelected) {
		d->isSelected = isSelected;
		Q_EMIT sigIsSelectedChanged(isSelected);
	}
}

bool CALAwesomeButton::getIsSelected() const {
	Q_D(const CALAwesomeButton);

	return d->isSelected;
}

void CALAwesomeButton::setOpacity(const qreal opacity) {
	d_func()->opacity = opacity;
	Q_EMIT sigOpacityChanged(opacity);
}

qreal CALAwesomeButton::getOpacity() const {
	return d_func()->opacity;
}

void CALAwesomeButton::installToolTipFilter(const int showDelay, const ALToolTipType::Position& position) {
	this->installEventFilter(new CALToolTipFilter(this, showDelay, position));
}

void CALAwesomeButton::paintEvent(QPaintEvent* event) {
	Q_D(CALAwesomeButton);

	QPainter painter(this);
	painter.save();
	painter.setOpacity(d->opacity);
	painter.setRenderHints(QPainter::SmoothPixmapTransform | QPainter::Antialiasing | QPainter::TextAntialiasing);
	painter.setPen(Qt::NoPen);
	painter.setBrush(d->isSelected ? d->darkSelectedColor : isEnabled() ? underMouse() ? d->darkHoverColor : Qt::transparent : Qt::transparent);
	painter.drawRoundedRect(rect(), d->borderRaius, d->borderRaius);

	// 图标绘制
	painter.setPen(isEnabled() ? d->darkIconColor : QColor(0xA1, 0xA1, 0xA4));
	painter.setFont(d->iconFont);
	painter.drawText(0, 0, width(), height(), Qt::AlignCenter, QChar(static_cast<unsigned short>(d->iconType)));
	painter.restore();
}
}
