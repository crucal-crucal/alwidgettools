#include "uvawesomebutton.hpp"

#include <QDebug>
#include <QPainter>

#include "uvawesomebutton_p.hpp"
#include "uvtooltip.hpp"

/**
 * @brief \class CUVAwesomeButtonPrivate
 * Internal class for CUVAwesomeButton
 * @param q pointer to the public class
 * @param parent pointer to the parent class
 */
CUVAwesomeButtonPrivate::CUVAwesomeButtonPrivate(CUVAwesomeButton* q, QObject* parent): QObject(parent), q_ptr(q) {
}

CUVAwesomeButtonPrivate::~CUVAwesomeButtonPrivate() = default;

void CUVAwesomeButtonPrivate::init() {
	Q_Q(CUVAwesomeButton);

	iconFont = QFont("CUVAwesome");
	lightHoverColor = QColor(0xEA, 0xE9, 0xF0);
	darkHoverColor = QColor(0x2F, 0x2E, 0x2C);
	lightIconColor = Qt::black;
	darkIconColor = Qt::white;
	ligntSelectedColor = QColor(0x00, 0x7A, 0xFF);
	darkSelectedColor = QColor(0x00, 0x7A, 0xFF);
	isSelected = false;
	borderRaius = 0;
}

/**
 * @brief \class CUVAwesomeButton
 * @param iconType Awesome icon type
 * @param parent pointer to the parent class
 */
CUVAwesomeButton::CUVAwesomeButton(const CUVAweSomeIcon& iconType, QWidget* parent)
: QPushButton(parent), d_ptr(new CUVAwesomeButtonPrivate(this, this)) {
	Q_D(CUVAwesomeButton);

	d->init();
	d->iconFont.setPixelSize(15);
	this->setFont(d->iconFont);
	d->iconType = iconType;
	this->setText(QChar(static_cast<ushort>(iconType)));
	connect(this, &CUVAwesomeButton::sigIsSelectedChanged, this, [=]() { this->update(); });
}

/**
 * @param iconType Awewome icon type
 * @param pixelSize font pixel size
 * @param parent pointer to the parent class
 */
CUVAwesomeButton::CUVAwesomeButton(const CUVAweSomeIcon& iconType, const int pixelSize, QWidget* parent)
: QPushButton(parent), d_ptr(new CUVAwesomeButtonPrivate(this, this)) {
	Q_D(CUVAwesomeButton);

	d->init();
	d->iconFont.setPixelSize(pixelSize);
	this->setFont(d->iconFont);
	d->iconType = iconType;
	this->setText(QChar(static_cast<unsigned short>(iconType)));
	connect(this, &CUVAwesomeButton::sigIsSelectedChanged, this, [=]() { this->update(); });
}

/**
 * @param iconType Awewome icon type
 * @param pixelSize font pixel size
 * @param fixedWidth fiexed button width
 * @param fixedHeight fixed button height
 * @param parent pointer to the parent class
 */
CUVAwesomeButton::CUVAwesomeButton(const CUVAweSomeIcon& iconType, const int pixelSize, const int fixedWidth, const int fixedHeight, QWidget* parent)
: CUVAwesomeButton(iconType, pixelSize, parent) {
	this->setFixedSize(fixedWidth, fixedHeight);
}

CUVAwesomeButton::~CUVAwesomeButton() {
	SAFE_DELETE(d_func()->tooltip)
}

void CUVAwesomeButton::setAwesomeIcon(const CUVAweSomeIcon& iconType) {
	Q_D(CUVAwesomeButton);

	if (d->iconType != iconType) {
		d->iconType = iconType;
		this->setText(QChar(static_cast<ushort>(iconType)));
	}
}

CUVAweSomeIcon CUVAwesomeButton::getAwesomeIcon() const {
	return d_func()->iconType;
}

void CUVAwesomeButton::setBorderRadius(const int radius) {
	Q_D(CUVAwesomeButton);

	if (d->borderRaius != radius) {
		d->borderRaius = radius;
		emit sigBorderRadiusChanged(radius);
		update();
	}
}

int CUVAwesomeButton::getBorderRadius() const {
	return d_func()->borderRaius;
}

void CUVAwesomeButton::setLightHoverColor(const QColor& color) {
	Q_D(CUVAwesomeButton);

	if (d->lightHoverColor != color) {
		d->lightHoverColor = color;
		emit sigLightHoverColorChanged(color);
		update();
	}
}

QColor CUVAwesomeButton::getLightHoverColor() const {
	return d_func()->lightHoverColor;
}

void CUVAwesomeButton::setDarkHoverColor(const QColor& color) {
	Q_D(CUVAwesomeButton);

	if (d->darkHoverColor != color) {
		d->darkHoverColor = color;
		emit sigDarkHoverColorChanged(color);
		update();
	}
}

QColor CUVAwesomeButton::getDarkHoverColor() const {
	return d_func()->darkHoverColor;
}

void CUVAwesomeButton::setLightIconColor(const QColor& color) {
	Q_D(CUVAwesomeButton);

	if (d->lightIconColor != color) {
		d->lightIconColor = color;
		emit sigLightIconColorChanged(color);
		update();
	}
}

QColor CUVAwesomeButton::getLightIconColor() const {
	return d_func()->lightIconColor;
}

void CUVAwesomeButton::setDarkIconColor(const QColor& color) {
	Q_D(CUVAwesomeButton);

	if (d->darkIconColor != color) {
		d->darkIconColor = color;
		emit sigDarkIconColorChanged(color);
		update();
	}
}

QColor CUVAwesomeButton::getDarkIconColor() const {
	return d_func()->darkIconColor;
}

void CUVAwesomeButton::setLigntSelectedColor(const QColor& color) {
	Q_D(CUVAwesomeButton);

	if (d->ligntSelectedColor != color) {
		d->ligntSelectedColor = color;
		emit sigLigntSelectedColorChanged(color);
		update();
	}
}

QColor CUVAwesomeButton::getLigntSelectedColor() const {
	return d_func()->ligntSelectedColor;
}

void CUVAwesomeButton::setDarkSelectedColor(const QColor& color) {
	Q_D(CUVAwesomeButton);

	if (d->darkSelectedColor != color) {
		d->darkSelectedColor = color;
		emit sigDarkSelectedColorChanged(color);
		update();
	}
}

QColor CUVAwesomeButton::getDarkSelectedColor() const {
	return d_func()->darkSelectedColor;
}

void CUVAwesomeButton::setIsSelected(const bool isSelected) {
	Q_D(CUVAwesomeButton);

	if (d->isSelected != isSelected) {
		d->isSelected = isSelected;
		emit sigIsSelectedChanged(isSelected);
	}
}

bool CUVAwesomeButton::getIsSelected() const {
	Q_D(const CUVAwesomeButton);

	return d->isSelected;
}

void CUVAwesomeButton::setToolTip(const QString& tooltip) {
	Q_D(CUVAwesomeButton);

	if (!d->tooltip) {
		d->tooltip = new CUVToolTip(this);
	}

	d->tooltip->setToolTip(tooltip);
}

void CUVAwesomeButton::paintEvent(QPaintEvent* event) {
	Q_D(CUVAwesomeButton);

	QPainter painter(this);
	painter.save();
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
