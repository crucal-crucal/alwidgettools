#include "altoolbutton.hpp"

#include <QApplication>
#include <QEvent>
#include <QMouseEvent>
#include <QPainter>
#include <QPropertyAnimation>

#include "alawesometoolbutton_p.hpp"
#include "alicon.hpp"
#include "altoolbuttonstyle.hpp"
#include "almenu.hpp"
#include "altooltip.hpp"

/**
 * @brief \namespace AL
 */
namespace AL {
/**
 * @brief \class CALToolButtonPrivate
 * Internal class for CALToolButton
 * @param q pointer to the public class
 * @param parent pointer to the parent class
 */
CALToolButtonPrivate::CALToolButtonPrivate(CALToolButton* q, QObject* parent): QObject(parent), q_ptr(q) {
}

CALToolButtonPrivate::~CALToolButtonPrivate() = default;

/**
 * @brief \class CALToolButton
 * @param parent pointer to the parent class
 */
CALToolButton::CALToolButton(QWidget* parent): QToolButton(parent), d_ptr(new CALToolButtonPrivate(this, this)) {
	Q_D(CALToolButton);

	setIconSize(QSize(22, 22));
	setPopupMode(QToolButton::InstantPopup);
	d->style = new CALToolButtonStyle(style());
	setStyle(d->style);
}

CALToolButton::~CALToolButton() {
	SAFE_DELETE(d_func()->style)
}

void CALToolButton::setBorderRadius(const int borderRadius) {
	d_func()->style->setBorderRadius(borderRadius);
	Q_EMIT sigBorderRadiusChanged();
}

int CALToolButton::getBorderRadius() const {
	return d_func()->style->getBorderRadius();
}

void CALToolButton::setIsSelected(const bool isSelected) {
	d_func()->style->setIsSelected(isSelected);
	Q_EMIT sigSelectedChanged();
}

bool CALToolButton::getIsSelected() const {
	return d_func()->style->getIsSelected();
}

void CALToolButton::setIsTransparent(const bool isTransparent) {
	d_func()->style->setIsTransparent(isTransparent);
	update();
}

bool CALToolButton::getIsTransparent() const {
	return d_func()->style->getIsTransparent();
}

void CALToolButton::setMenu(CALMenu* menu) {
	if (!menu || menu == this->menu()) {
		return;
	}
	menu->setMenuItemHeight(27);
	QToolButton::setMenu(menu);
	menu->installEventFilter(this);
}

void CALToolButton::setALIcon(const std::shared_ptr<CALIconType>& icon_type) {
	if (!icon_type) {
		qWarning() << __func__ << " received a nullptr icon_type!";
		return;
	}

	d_func()->style->setALIconType(icon_type->iconType());
	setProperty(ALIcon::iconProperty, QChar(icon_type->value()));
	constexpr int pixelSize = 1;
	QFont iconFont(icon_type->familyName());
	QPixmap pix(pixelSize, pixelSize);
	pix.fill(Qt::transparent);
	QPainter painter;
	painter.begin(&pix);
	painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing | QPainter::SmoothPixmapTransform);
	iconFont.setPixelSize(pixelSize);
	painter.setFont(iconFont);
	painter.drawText(pix.rect(), Qt::AlignCenter, QChar(icon_type->value()));
	painter.end();
	setIcon(QIcon(pix));
}

void CALToolButton::installToolTipFilter(const int showDelay, const ALToolTipType::Position& position) {
	this->installEventFilter(new CALToolTipFilter(this, showDelay, position));
}

bool CALToolButton::eventFilter(QObject* watched, QEvent* event) {
	Q_D(CALToolButton);

	if (watched == menu()) {
		switch (event->type()) {
			case QEvent::Show: {
				// 指示器动画
				const auto rotateAnimation = new QPropertyAnimation(d->style, "expandIconRotate");
				connect(rotateAnimation, &QPropertyAnimation::valueChanged, this, [this](const QVariant&) { update(); });
				rotateAnimation->setDuration(300);
				rotateAnimation->setEasingCurve(QEasingCurve::InOutSine);
				rotateAnimation->setStartValue(d->style->getExpandIconRotate());
				rotateAnimation->setEndValue(-180);
				rotateAnimation->start(QAbstractAnimation::DeleteWhenStopped);
				break;
			}
			case QEvent::Hide: {
				// 指示器动画
				const auto rotateAnimation = new QPropertyAnimation(d->style, "expandIconRotate");
				connect(rotateAnimation, &QPropertyAnimation::valueChanged, this, [this](const QVariant&) { update(); });
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
}
