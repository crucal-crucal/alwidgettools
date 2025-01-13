#include "alawesometoolbutton.hpp"

#include <QApplication>
#include <QEvent>
#include <QMouseEvent>
#include <QPainter>
#include <QPropertyAnimation>

#include "alawesometoolbutton_p.hpp"
#include "alawesometoolbuttonstyle.hpp"
#include "almenu.hpp"
#include "altooltip.hpp"

namespace AL {
/**
 * @brief \class CALAwesomeToolButtonPrivate
 * Internal class for CALAwesomeToolButton
 * @param q pointer to the public class
 * @param parent pointer to the parent class
 */
CALAwesomeToolButtonPrivate::CALAwesomeToolButtonPrivate(CALAwesomeToolButton* q, QObject* parent): QObject(parent), q_ptr(q) {
}

CALAwesomeToolButtonPrivate::~CALAwesomeToolButtonPrivate() = default;

/**
 * @brief \class CALAwesomeToolButton
 * @param parent pointer to the parent class
 */
CALAwesomeToolButton::CALAwesomeToolButton(QWidget* parent): QToolButton(parent), d_ptr(new CALAwesomeToolButtonPrivate(this, this)) {
	Q_D(CALAwesomeToolButton);

	setIconSize(QSize(22, 22));
	setPopupMode(QToolButton::InstantPopup);
	d->style = new CALAwesomeToolButtonStyle(style());
	setStyle(d->style);
}

CALAwesomeToolButton::~CALAwesomeToolButton() {
	SAFE_DELETE(d_func()->tooltip)
	SAFE_DELETE(d_func()->style)
}

void CALAwesomeToolButton::setBorderRadius(const int borderRadius) {
	d_func()->style->setBorderRadius(borderRadius);
	emit sigBorderRadiusChanged();
}

int CALAwesomeToolButton::getBorderRadius() const {
	return d_func()->style->getBorderRadius();
}

void CALAwesomeToolButton::setIsSelected(const bool isSelected) {
	d_func()->style->setIsSelected(isSelected);
	emit sigSelectedChanged();
}

bool CALAwesomeToolButton::getIsSelected() const {
	return d_func()->style->getIsSelected();
}

void CALAwesomeToolButton::setIsTransparent(const bool isTransparent) {
	d_func()->style->setIsTransparent(isTransparent);
	update();
}

bool CALAwesomeToolButton::getIsTransparent() const {
	return d_func()->style->getIsTransparent();
}

void CALAwesomeToolButton::setMenu(CALMenu* menu) {
	if (!menu || menu == this->menu()) {
		return;
	}
	menu->setMenuItemHeight(27);
	QToolButton::setMenu(menu);
	menu->installEventFilter(this);
}

void CALAwesomeToolButton::setAweSomeIcon(const ALIcon::AweSomeIcon& icon) {
	setProperty("CALIconType", QChar(static_cast<unsigned short>(icon)));
	constexpr int pixelSize = 1;
	QFont iconFont("CALAwesome");
	QPixmap pix(pixelSize, pixelSize);
	pix.fill(Qt::transparent);
	QPainter painter;
	painter.begin(&pix);
	painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing | QPainter::SmoothPixmapTransform);
	iconFont.setPixelSize(pixelSize);
	painter.setFont(iconFont);
	painter.drawText(pix.rect(), Qt::AlignCenter, QChar(static_cast<unsigned short>(icon)));
	painter.end();
	setIcon(QIcon(pix));
}

void CALAwesomeToolButton::setToolTip(const QString& tooltip) {
	Q_D(CALAwesomeToolButton);

	if (!d->tooltip) {
		d->tooltip = new CALToolTip(this);
	}

	d->tooltip->setToolTip(tooltip);
}

bool CALAwesomeToolButton::eventFilter(QObject* watched, QEvent* event) {
	Q_D(CALAwesomeToolButton);

	if (watched == menu()) {
		switch (event->type()) {
			case QEvent::Show: {
				//指示器动画
				const auto rotateAnimation = new QPropertyAnimation(d->style, "expandIconRotate");
				connect(rotateAnimation, &QPropertyAnimation::valueChanged, this, [=](const QVariant& value) {
					update();
				});
				rotateAnimation->setDuration(300);
				rotateAnimation->setEasingCurve(QEasingCurve::InOutSine);
				rotateAnimation->setStartValue(d->style->getExpandIconRotate());
				rotateAnimation->setEndValue(-180);
				rotateAnimation->start(QAbstractAnimation::DeleteWhenStopped);
				break;
			}
			case QEvent::Hide: {
				//指示器动画
				const auto rotateAnimation = new QPropertyAnimation(d->style, "expandIconRotate");
				connect(rotateAnimation, &QPropertyAnimation::valueChanged, this, [=](const QVariant& value) {
					update();
				});
				rotateAnimation->setDuration(300);
				rotateAnimation->setEasingCurve(QEasingCurve::InOutSine);
				rotateAnimation->setStartValue(d->style->getExpandIconRotate());
				rotateAnimation->setEndValue(0);
				rotateAnimation->start(QAbstractAnimation::DeleteWhenStopped);
				QMouseEvent focusEvent(QEvent::MouseButtonPress, QPoint(-1, -1), QPoint(-1, -1), Qt::NoButton, Qt::NoButton, Qt::NoModifier);
				QApplication::sendEvent(parentWidget(), &focusEvent);
				break;
			}
			default: {
				break;
			}
		}
	}

	return QToolButton::eventFilter(watched, event);
}
} // namespace AL