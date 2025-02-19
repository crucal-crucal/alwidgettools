#include "alcombobox.hpp"

#include <QApplication>
#include <QLayout>
#include <QListView>
#include <QPropertyAnimation>

#include "alcomboboxstyle.hpp"
#include "alcombobox_p.hpp"
#include "alscrollbar.hpp"
#include "althememanager.hpp"

/**
 * @brief \namespace AL
 */
namespace AL {
/**
 * @brief \class CALComboBoxPrivate
 * Internal class for CALComboBox
 * @param q pointer to the public class
 * @param parent pointer to the parent class
 */
CALComboBoxPrivate::CALComboBoxPrivate(CALComboBox* q, QObject* parent): QObject(parent), q_ptr(q) {
}

CALComboBoxPrivate::~CALComboBoxPrivate() = default;

/**
 * @brief \class CALComboBox
 * @param parent pointer to the parent class
 */
CALComboBox::CALComboBox(QWidget* parent): QComboBox(parent), d_ptr(new CALComboBoxPrivate(this, this)) {
	Q_D(CALComboBox);

	d->borderRadius = 3;
	d->isAllowHidePopup = false;
	d->themeMode = ALTheme->getThemeMode();
	connect(ALTheme, &CALThemeManager::sigThemeModeChanged, this, [d](const ALThemeType::ThemeMode& mode) { d->themeMode = mode; });
	setObjectName("CALComboBox");
	setFixedHeight(35);
	d->comboBoxStyle = new CALComboBoxStyle(style());
	setStyle(d->comboBoxStyle);

	// 调用view 让container初始化
	setView(new QListView(this));
	const auto comboBoxView = this->view();
	comboBoxView->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
	const auto scrollBar = new CALScrollBar(this);
	comboBoxView->setVerticalScrollBar(scrollBar);
	comboBoxView->setAutoScroll(false);
	comboBoxView->setSelectionMode(QAbstractItemView::NoSelection);
	comboBoxView->setObjectName("CALComboBoxView");
	comboBoxView->setStyleSheet("#CALComboBoxView{ background-color: transparent; }");
	comboBoxView->setStyle(d->comboBoxStyle);
	comboBoxView->verticalScrollBar()->setVisible(true);
	comboBoxView->horizontalScrollBar()->setVisible(false);
	if (QWidget* container = this->findChild<QFrame*>()) {
		container->setWindowFlags(Qt::Popup | Qt::FramelessWindowHint | Qt::NoDropShadowWindowHint);
		container->setAttribute(Qt::WA_TranslucentBackground);
		container->setObjectName("CALComboBoxContainer");
		container->setStyle(d->comboBoxStyle);
		QLayout* layout = container->layout();
		while (layout->count()) {
			layout->takeAt(0);
		}
		layout->addWidget(view());
		layout->setContentsMargins(6, 0, 6, 6);
	}
	QComboBox::setMaxVisibleItems(5);
}

CALComboBox::~CALComboBox() = default;

void CALComboBox::setBorderRadius(const int borderRadius) {
	Q_D(CALComboBox);

	d->borderRadius = borderRadius;
	Q_EMIT sigBorderRadiusChanged();
}

int CALComboBox::getBorderRadius() const {
	Q_D(const CALComboBox);

	return d->borderRadius;
}

void CALComboBox::showPopup() {
	Q_D(CALComboBox);

	bool oldAnimationEffects = qApp->isEffectEnabled(Qt::UI_AnimateCombo); // NOLINT
	qApp->setEffectEnabled(Qt::UI_AnimateCombo, false);                    // NOLINT
	QComboBox::showPopup();
	qApp->setEffectEnabled(Qt::UI_AnimateCombo, oldAnimationEffects); // NOLINT

	if (count() > 0) {
		if (QWidget* container = this->findChild<QFrame*>()) {
			int containerHeight;
			if (count() >= maxVisibleItems()) {
				containerHeight = maxVisibleItems() * 35 + 10;
			} else {
				containerHeight = count() * 35 + 10;
			}
			view()->resize(view()->width(), containerHeight - 8);
			container->move(container->x(), container->y() + 3);
			QLayout* layout = container->layout();
			while (layout->count()) {
				layout->takeAt(0);
			}
			const auto fixedSizeAnimation = new QPropertyAnimation(container, "maximumHeight");
			connect(fixedSizeAnimation, &QPropertyAnimation::valueChanged, this, [container](const QVariant& value) { container->setFixedHeight(value.toInt()); });
			fixedSizeAnimation->setStartValue(1);
			fixedSizeAnimation->setEndValue(containerHeight);
			fixedSizeAnimation->setEasingCurve(QEasingCurve::OutCubic);
			fixedSizeAnimation->setDuration(400);
			fixedSizeAnimation->start(QAbstractAnimation::DeleteWhenStopped);

			const auto viewPosAnimation = new QPropertyAnimation(view(), "pos");
			connect(viewPosAnimation, &QPropertyAnimation::finished, this, [this, d, layout]() {
				d->isAllowHidePopup = true;
				layout->addWidget(view());
			});
			const QPoint viewPos = view()->pos();
			viewPosAnimation->setStartValue(QPoint(viewPos.x(), viewPos.y() - view()->height()));
			viewPosAnimation->setEndValue(viewPos);
			viewPosAnimation->setEasingCurve(QEasingCurve::OutCubic);
			viewPosAnimation->setDuration(400);
			viewPosAnimation->start(QAbstractAnimation::DeleteWhenStopped);
		}
		// 指示器动画
		const auto rotateAnimation = new QPropertyAnimation(d->comboBoxStyle, "expandIconRotate");
		connect(rotateAnimation, &QPropertyAnimation::valueChanged, this, [=](const QVariant& value) { update(); });
		rotateAnimation->setDuration(300);
		rotateAnimation->setEasingCurve(QEasingCurve::InOutSine);
		rotateAnimation->setStartValue(d->comboBoxStyle->getExpandIconRotate());
		rotateAnimation->setEndValue(-180);
		rotateAnimation->start(QAbstractAnimation::DeleteWhenStopped);
		const auto markAnimation = new QPropertyAnimation(d->comboBoxStyle, "expandMarkWidth");
		markAnimation->setDuration(300);
		markAnimation->setEasingCurve(QEasingCurve::InOutSine);
		markAnimation->setStartValue(d->comboBoxStyle->getExpandMarkWidth());
		markAnimation->setEndValue(width() / 2 - d->borderRadius - 6);
		markAnimation->start(QAbstractAnimation::DeleteWhenStopped);
	}
}

void CALComboBox::hidePopup() {
	Q_D(CALComboBox);

	if (d->isAllowHidePopup) {
		QWidget* container = this->findChild<QFrame*>();
		const int containerHeight = container->height();
		if (container) { // NOLINT
			QLayout* layout = container->layout();
			while (layout->count()) {
				layout->takeAt(0);
			}
			const auto viewPosAnimation = new QPropertyAnimation(view(), "pos");
			connect(viewPosAnimation, &QPropertyAnimation::finished, this, [=]() { layout->addWidget(view()); });
			constexpr auto viewPos = QPoint(7, 1);
			connect(viewPosAnimation, &QPropertyAnimation::finished, this, [=]() { view()->move(viewPos); });
			viewPosAnimation->setStartValue(viewPos);
			viewPosAnimation->setEndValue(QPoint(viewPos.x(), viewPos.y() - view()->height()));
			viewPosAnimation->setEasingCurve(QEasingCurve::InCubic);
			viewPosAnimation->start(QAbstractAnimation::DeleteWhenStopped);

			const auto fixedSizeAnimation = new QPropertyAnimation(container, "maximumHeight");
			connect(fixedSizeAnimation, &QPropertyAnimation::finished, this, [=]() {
				QComboBox::hidePopup();
				container->setFixedHeight(containerHeight);
			});
			connect(fixedSizeAnimation, &QPropertyAnimation::valueChanged, this, [=](const QVariant& value) { container->setFixedHeight(value.toInt()); });
			fixedSizeAnimation->setStartValue(container->height());
			fixedSizeAnimation->setEndValue(1);
			fixedSizeAnimation->setEasingCurve(QEasingCurve::InCubic);
			fixedSizeAnimation->start(QAbstractAnimation::DeleteWhenStopped);
			d->isAllowHidePopup = false;
		}
		// 指示器动画
		const auto rotateAnimation = new QPropertyAnimation(d->comboBoxStyle, "expandIconRotate");
		connect(rotateAnimation, &QPropertyAnimation::valueChanged, this, [=](const QVariant& value) { update(); });
		rotateAnimation->setDuration(300);
		rotateAnimation->setEasingCurve(QEasingCurve::InOutSine);
		rotateAnimation->setStartValue(d->comboBoxStyle->getExpandIconRotate());
		rotateAnimation->setEndValue(0);
		rotateAnimation->start(QAbstractAnimation::DeleteWhenStopped);
		const auto markAnimation = new QPropertyAnimation(d->comboBoxStyle, "expandMarkWidth");
		markAnimation->setDuration(300);
		markAnimation->setEasingCurve(QEasingCurve::InOutSine);
		markAnimation->setStartValue(d->comboBoxStyle->getExpandMarkWidth());
		markAnimation->setEndValue(0);
		markAnimation->start(QAbstractAnimation::DeleteWhenStopped);
	}
}
}
