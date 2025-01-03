#include "uvawesometoolbutton.hpp"

#include <QApplication>
#include <QEvent>
#include <QMouseEvent>
#include <QPainter>
#include <QPropertyAnimation>

#include "uvawesometoolbutton_p.hpp"
#include "uvawesometoolbuttonstyle.hpp"
#include "uvmenu.hpp"
#include "uvtooltip.hpp"

/**
 * @brief \class CUVAwesomeToolButtonPrivate
 * Internal class for CUVAwesomeToolButton
 * @param q pointer to the public class
 * @param parent pointer to the parent class
 */
CUVAwesomeToolButtonPrivate::CUVAwesomeToolButtonPrivate(CUVAwesomeToolButton* q, QObject* parent): QObject(parent), q_ptr(q) {
}

CUVAwesomeToolButtonPrivate::~CUVAwesomeToolButtonPrivate() = default;

/**
 * @brief \class CUVAwesomeToolButton
 * @param parent pointer to the parent class
 */
CUVAwesomeToolButton::CUVAwesomeToolButton(QWidget* parent): QToolButton(parent), d_ptr(new CUVAwesomeToolButtonPrivate(this, this)) {
	Q_D(CUVAwesomeToolButton);

	setIconSize(QSize(22, 22));
	setPopupMode(QToolButton::InstantPopup);
	d->style = new CUVAwesomeToolButtonStyle(style());
	setStyle(d->style);
}

CUVAwesomeToolButton::~CUVAwesomeToolButton() {
	SAFE_DELETE(d_func()->tooltip)
	SAFE_DELETE(d_func()->style)
}

void CUVAwesomeToolButton::setBorderRadius(const int borderRadius) {
	d_func()->style->setBorderRadius(borderRadius);
	emit sigBorderRadiusChanged();
}

int CUVAwesomeToolButton::getBorderRadius() const {
	return d_func()->style->getBorderRadius();
}

void CUVAwesomeToolButton::setIsSelected(const bool isSelected) {
	d_func()->style->setIsSelected(isSelected);
	emit sigSelectedChanged();
}

bool CUVAwesomeToolButton::getIsSelected() const {
	return d_func()->style->getIsSelected();
}

void CUVAwesomeToolButton::setIsTransparent(const bool isTransparent) {
	d_func()->style->setIsTransparent(isTransparent);
	update();
}

bool CUVAwesomeToolButton::getIsTransparent() const {
	return d_func()->style->getIsTransparent();
}

void CUVAwesomeToolButton::setMenu(CUVMenu* menu) {
	if (!menu || menu == this->menu()) {
		return;
	}
	menu->setMenuItemHeight(27);
	QToolButton::setMenu(menu);
	menu->installEventFilter(this);
}

void CUVAwesomeToolButton::setAweSomeIcon(const UVIcon::CUVAweSomeIcon& icon) {
	setProperty("CUVIconType", QChar(static_cast<unsigned short>(icon)));
	constexpr int pixelSize = 1;
	auto iconFont = QFont("CUVAwesome");
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

void CUVAwesomeToolButton::setToolTip(const QString& tooltip) {
	Q_D(CUVAwesomeToolButton);

	if (!d->tooltip) {
		d->tooltip = new CUVToolTip(this);
	}

	d->tooltip->setToolTip(tooltip);
}

bool CUVAwesomeToolButton::eventFilter(QObject* watched, QEvent* event) {
	Q_D(CUVAwesomeToolButton);

	if (watched == menu()) {
		switch (event->type()) {
			case QEvent::Show: {
				//指示器动画
				const auto rotateAnimation = new QPropertyAnimation(d->style, "pExpandIconRotate");
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
