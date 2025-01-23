#include "almultiselectcombobox.hpp"

#include <QAction>
#include <QApplication>
#include <QLayout>
#include <QMouseEvent>
#include <QPainter>
#include <QPropertyAnimation>
#include <algorithm>

#include "alcomboboxstyle.hpp"
#include "alcomboboxview.hpp"
#include "almultiselectcombobox_p.hpp"
#include "alscrollbar.hpp"
#include "althememanager.hpp"

/**
 * @brief \namespace AL
 */
namespace AL {
/**
 * @brief \class CALMultiSelectComboBoxPrivate
 * internal class for CALMultiSelectComboBox
 * @param q pointer to the public class
 * @param parent pointer to the parent class
 */
CALMultiSelectComboBoxPrivate::CALMultiSelectComboBoxPrivate(CALMultiSelectComboBox* q, QObject* parent): QObject(parent), q_ptr(q) {
}

CALMultiSelectComboBoxPrivate::~CALMultiSelectComboBoxPrivate() = default;

void CALMultiSelectComboBoxPrivate::onItemPressed(const QModelIndex& index) {
	Q_Q(CALMultiSelectComboBox);

	if (!index.isValid() || index.row() >= itemSelection.size()) {
		return;
	}

	if (index.row() == 0) {
		const bool allSelected = !itemSelection[0];
		for (bool& i : itemSelection) {
			i = allSelected;
		}
		// 更新选择模型
		for (int i = 1; i < q->count(); ++i) {
			QModelIndex itemIndex = q->model()->index(i, 0);
			comboBoxView->selectionModel()->select(itemIndex, allSelected ? QItemSelectionModel::Select : QItemSelectionModel::Deselect);
		}
	} else {
		itemSelection[index.row()] = !comboBoxView->selectionModel()->isSelected(index);
		comboBoxView->selectionModel()->select(index, itemSelection[index.row()] ? QItemSelectionModel::Select : QItemSelectionModel::Deselect);

		// 检查是否所有项都被选中
		itemSelection[0] = std::all_of(itemSelection.begin() + 1, itemSelection.end(), [](const bool selected) { return selected; });
	}
	refreshCurrentIndexs();
	const auto markAnimation = new QPropertyAnimation(this, "expandMarkWidth");
	connect(markAnimation, &QPropertyAnimation::valueChanged, this, [=](const QVariant& value) {
		q->update();
	});
	markAnimation->setDuration(300);
	markAnimation->setEasingCurve(QEasingCurve::InOutSine);
	markAnimation->setStartValue(expandMarkWidth);
	const qreal step = static_cast<qreal>(q->width() / 2.0 - borderRadius) / (q->count());
	markAnimation->setEndValue(step * (selectedTextList.count()));
	markAnimation->start(QAbstractAnimation::DeleteWhenStopped);
}

void CALMultiSelectComboBoxPrivate::setExpandIconRotate(const qreal expandIconRotate) {
	this->expandIconRotate = expandIconRotate;
	Q_EMIT sigExpandIconRotateChanged(expandIconRotate);
}

qreal CALMultiSelectComboBoxPrivate::getExpandIconRotate() const {
	return expandIconRotate;
}

void CALMultiSelectComboBoxPrivate::setExpandMarkWidth(const qreal expandMarkWidth) {
	this->expandMarkWidth = expandMarkWidth;
	Q_EMIT sigExpandMarkWidthChanged(expandMarkWidth);
}

qreal CALMultiSelectComboBoxPrivate::getExpandMarkWidth() const {
	return expandMarkWidth;
}

void CALMultiSelectComboBoxPrivate::refreshCurrentIndexs() {
	Q_Q(CALMultiSelectComboBox);

	QString str{};
	adjustSelectedVector();
	QVector<bool> selectedIndexVector{};
	bool allSelected = true;
	for (int i = 0; i < q->count(); ++i) {
		QModelIndex index = q->model()->index(i, 0);
		if (itemSelection[i]) {
			comboBoxView->selectionModel()->select(index, QItemSelectionModel::Select | QItemSelectionModel::Rows);
			if (i != 0) {
				if (!str.isEmpty()) {
					str.append(",");
				}
				str.append(q->itemText(i));
			}
			selectedIndexVector.append(true);
		} else {
			comboBoxView->selectionModel()->select(index, QItemSelectionModel::Deselect);
			selectedIndexVector.append(false);
			if (i != 0) { // 跳过"全选"项
				allSelected = false;
			}
		}
	}

	itemSelection[0] = allSelected;
	comboBoxView->selectionModel()->select(q->model()->index(0, 0), allSelected ? QItemSelectionModel::Select : QItemSelectionModel::Deselect);

	if (str != currentText) {
		q->update();
		currentText = str;
#if QT_VERSION >= QT_VERSION_CHECK(5, 14, 0)
		selectedTextList = currentText.split(",", Qt::SkipEmptyParts);
#else
		if (_selectedTextList.count() == 1 && _selectedTextList.at(0).isEmpty()) {
			_selectedTextList.clear()
		}
#endif
		//q->setCurrentIndex(-1);
		Q_EMIT q->sigItemSelectionChanged(selectedIndexVector);
		Q_EMIT q->sigCurrentTextListChanged(selectedTextList);
	}
}

void CALMultiSelectComboBoxPrivate::adjustSelectedVector() {
	Q_Q(CALMultiSelectComboBox);

	while (itemSelection.count() < q->count()) {
		itemSelection.append(false);
	}
}

/**
 * @brief \class CALMultiSelectComboBox
 * @param parent pointer to the parent class
 */
CALMultiSelectComboBox::CALMultiSelectComboBox(QWidget* parent): QComboBox(parent), d_ptr(new CALMultiSelectComboBoxPrivate(this, this)) {
	Q_D(CALMultiSelectComboBox);

	d->isFirstPopup = false;
	d->isAllowHidePopup = false;
	d->borderRadius = 2;
	d->expandIconRotate = 0;
	d->expandMarkWidth = 0;
	d->themeMode = ALTheme->getThemeMode();
	connect(ALTheme, &CALThemeManager::sigThemeModeChanged, this, [=](const ALThemeType::ThemeMode& mode) { d->themeMode = mode; });
	setFixedHeight(35);

	d->comboBoxStyle = new CALComboBoxStyle(style());
	setStyle(d->comboBoxStyle);

	// 初始化container
	d->comboBoxView = new CALComboBoxView(this);
	setView(d->comboBoxView);
	QAbstractItemView* comboBoxView = this->view();
	comboBoxView->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
	const auto scrollBar = new CALScrollBar(this);
	comboBoxView->setVerticalScrollBar(scrollBar);
	const auto floatVScrollBar = new CALScrollBar(scrollBar, comboBoxView);
	floatVScrollBar->setIsAnimation(true);
	comboBoxView->setAutoScroll(false);
	comboBoxView->setSelectionMode(QAbstractItemView::NoSelection);
	comboBoxView->setSelectionBehavior(QAbstractItemView::SelectRows);
	comboBoxView->setObjectName("CALComboBoxView");
	comboBoxView->setStyleSheet("#CALComboBoxView { background-color: transparent; }");
	comboBoxView->setStyle(d->comboBoxStyle);
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
#ifndef Q_OS_WIN
		container->setStyleSheet("background-color:transparent;");
#endif
	}
	QComboBox::setMaxVisibleItems(5);
	connect(d->comboBoxView, &CALComboBoxView::sigItemPressed, d, &CALMultiSelectComboBoxPrivate::onItemPressed);
	connect(this, QOverload<int>::of(&CALMultiSelectComboBox::currentIndexChanged), d, &CALMultiSelectComboBoxPrivate::refreshCurrentIndexs);
	d->itemSelection.resize(32);
	d->itemSelection.fill(false);
	d->itemSelection[0] = true;
	QComboBox::setMaxVisibleItems(5);
}

CALMultiSelectComboBox::~CALMultiSelectComboBox() = default;

void CALMultiSelectComboBox::setCurrentSelection(const QString& selection) {
	Q_D(CALMultiSelectComboBox);

	d->itemSelection.fill(false);
	d->comboBoxView->selectionModel()->clearSelection();
	for (int i = 0; i < this->count(); ++i) {
		if (selection == itemText(i)) {
			QModelIndex index = model()->index(i, 0);
			d->comboBoxView->selectionModel()->select(index, QItemSelectionModel::Select | QItemSelectionModel::Rows);
			d->itemSelection[index.row()] = true;
		}
	}
	d->refreshCurrentIndexs();
}

void CALMultiSelectComboBox::setCurrentSelection(const QStringList& selection) {
	Q_D(CALMultiSelectComboBox);

	d->itemSelection.fill(false);
	d->comboBoxView->selectionModel()->clearSelection();
	for (int i = 0; i < this->count(); ++i) {
		if (selection.contains(itemText(i))) {
			QModelIndex index = model()->index(i, 0);
			d->comboBoxView->selectionModel()->select(index, QItemSelectionModel::Select | QItemSelectionModel::Rows);
			d->itemSelection[index.row()] = true;
		}
	}
	d->refreshCurrentIndexs();
}

void CALMultiSelectComboBox::setCurrentSelection(const int index) {
	Q_D(CALMultiSelectComboBox);

	if (index >= this->count() || index < 0) {
		return;
	}

	d->itemSelection.fill(false);
	d->comboBoxView->selectionModel()->clearSelection();
	const QModelIndex currentIndex = model()->index(index, 0);
	d->comboBoxView->selectionModel()->select(currentIndex, QItemSelectionModel::Select | QItemSelectionModel::Rows);
	d->itemSelection[index] = true;
	d->refreshCurrentIndexs();
}

void CALMultiSelectComboBox::setCurrentSelection(const QList<int>& selectionIndex) {
	Q_D(CALMultiSelectComboBox);

	d->itemSelection.fill(false);
	d->comboBoxView->selectionModel()->clearSelection();
	for (const auto& index : selectionIndex) {
		if (index >= this->count() || index < 0) {
			continue;
		}
		QModelIndex currentIndex = model()->index(index, 0);
		d->comboBoxView->selectionModel()->select(currentIndex, QItemSelectionModel::Select | QItemSelectionModel::Rows);
		d->itemSelection[index] = true;
	}
	d->refreshCurrentIndexs();
}

QStringList CALMultiSelectComboBox::getCurrentSelection() const {
	Q_D(const CALMultiSelectComboBox);

	return d->selectedTextList;
}

QList<int> CALMultiSelectComboBox::getCurrentSelectionIndex() const {
	Q_D(const CALMultiSelectComboBox);

	QList<int> indexList{};
	for (int i = 0; i < d->itemSelection.size(); ++i) {
		if (d->itemSelection[i]) {
			indexList.append(i);
		}
	}

	return indexList;
}

void CALMultiSelectComboBox::setBorderRadius(const int radius) {
	Q_D(CALMultiSelectComboBox);

	d->borderRadius = radius;
	Q_EMIT sigBorderRadiusChanged();
}

int CALMultiSelectComboBox::getBorderRadius() const {
	Q_D(const CALMultiSelectComboBox);

	return d->borderRadius;
}

void CALMultiSelectComboBox::selectAllItems() {
	Q_D(CALMultiSelectComboBox);

	QVector<int> indices(d->itemSelection.size());
	std::iota(indices.begin(), indices.end(), 0); // 直接填充 [0, 1, ..., N-1]
	setCurrentSelection(QList<int>::fromVector(indices));
}

void CALMultiSelectComboBox::deselectAllItems() {
	setCurrentSelection(QList<int>());
}

bool CALMultiSelectComboBox::isAllSelected() const {
	Q_D(const CALMultiSelectComboBox);

	// 检查是否所有项目都被选中
	for (int i = 1; i < d->itemSelection.size(); ++i) {
		if (!d->itemSelection[i]) {
			return false;
		}
	}
	return true;
}

void CALMultiSelectComboBox::paintEvent(QPaintEvent* event) {
	Q_D(CALMultiSelectComboBox);

	QPainter painter(this);
	painter.save();
	painter.setRenderHints(QPainter::SmoothPixmapTransform | QPainter::Antialiasing | QPainter::TextAntialiasing);
	painter.setPen(ALThemeColor(d->themeMode, ALThemeType::BasicBorder));
	painter.setBrush(isEnabled() ? ALThemeColor(d->themeMode, underMouse() ? ALThemeType::BasicHover : ALThemeType::BasicBase) : Qt::transparent);
	QRect foregroundRect = rect();
	foregroundRect.adjust(6, 1, -6, -1); // 上下左右内部边距
	painter.drawRoundedRect(foregroundRect, d->borderRadius, d->borderRadius);
	// 底边线绘制
	painter.setPen(ALThemeColor(d->themeMode, ALThemeType::BasicBaseLine));
	painter.drawLine(foregroundRect.x() + d->borderRadius, foregroundRect.y() + foregroundRect.height(), foregroundRect.x() + foregroundRect.width() - d->borderRadius, foregroundRect.y() + foregroundRect.height());
	// 文字绘制
	painter.setPen(ALThemeColor(d->themeMode, isEnabled() ? ALThemeType::BasicText : ALThemeType::BasicTextDisable));
	const QString currentText = painter.fontMetrics().elidedText(d->currentText, Qt::ElideRight, foregroundRect.width() - 27 - static_cast<int>(width() * 0.05));
	painter.drawText(15, height() / 2 + painter.fontMetrics().ascent() / 2 - 1, currentText);
	// 展开指示器绘制
	painter.setPen(Qt::NoPen);
	painter.setBrush(d->themeMode == ALThemeType::Light ? QColor(0x0E, 0x6F, 0xC3) : QColor(0x4C, 0xA0, 0xE0));
	painter.drawRoundedRect(QRectF(width() / 2.0 - d->expandMarkWidth, height() - 3, d->expandMarkWidth * 2, 3), 2, 2);
	// 展开图标绘制
	if (count() > 0) {
		painter.save();
		QFont iconFont(ALIcon::getEnumTypeFontName(ALIcon::Awesome));
		iconFont.setPixelSize(17);
		painter.setFont(iconFont);
		painter.setPen(ALThemeColor(d->themeMode, isEnabled() ? ALThemeType::BasicText : ALThemeType::BasicTextDisable));
		const QRectF expandIconRect(width() - 25, 0, 20, height());
		painter.translate(expandIconRect.x() + expandIconRect.width() / 2.0 - 2.0, expandIconRect.y() + expandIconRect.height() / 2.0);
		painter.rotate(d->expandIconRotate);
		painter.translate(-expandIconRect.x() - expandIconRect.width() / 2.0 + 2.0, expandIconRect.y() - expandIconRect.height() / 2.0);
		painter.drawText(expandIconRect, Qt::AlignVCenter, QChar(static_cast<unsigned short>(ALIcon::AweSomeIcon::AngleDown)));
		painter.restore();
	}
	painter.restore();
}

void CALMultiSelectComboBox::showPopup() {
	Q_D(CALMultiSelectComboBox);

	bool oldAnimationEffects = qApp->isEffectEnabled(Qt::UI_AnimateCombo); // NOLINT
	qApp->setEffectEnabled(Qt::UI_AnimateCombo, false);                    // NOLINT
	QComboBox::showPopup();
	qApp->setEffectEnabled(Qt::UI_AnimateCombo, oldAnimationEffects); // NOLINT

	if (count() > 0) {
		if (QWidget* container = this->findChild<QFrame*>()) {
			const int containerHeight = (count()) >= maxVisibleItems() ? maxVisibleItems() * 35 + 8 : (count()) * 35 + 8;
			view()->resize(view()->width(), containerHeight - 8);
			container->move(container->x(), container->y() + 3);
			QLayout* layout = container->layout();
			while (layout->count()) {
				layout->takeAt(0);
			}
			const auto fixedSizeAnimation = new QPropertyAnimation(container, "maximumHeight");
			connect(fixedSizeAnimation, &QPropertyAnimation::valueChanged, this, [=](const QVariant& value) { container->setFixedHeight(value.toInt()); });
			fixedSizeAnimation->setStartValue(1);
			fixedSizeAnimation->setEndValue(containerHeight);
			fixedSizeAnimation->setEasingCurve(QEasingCurve::OutCubic);
			fixedSizeAnimation->setDuration(400);
			fixedSizeAnimation->start(QAbstractAnimation::DeleteWhenStopped);

			const auto viewPosAnimation = new QPropertyAnimation(view(), "pos");
			connect(viewPosAnimation, &QPropertyAnimation::finished, this, [=]() {
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
		//指示器动画
		const auto rotateAnimation = new QPropertyAnimation(d, "expandIconRotate");
		connect(rotateAnimation, &QPropertyAnimation::valueChanged, this, [=](const QVariant& value) { update(); });
		rotateAnimation->setDuration(300);
		rotateAnimation->setEasingCurve(QEasingCurve::InOutSine);
		rotateAnimation->setStartValue(d->expandIconRotate);
		rotateAnimation->setEndValue(-180);
		rotateAnimation->start(QAbstractAnimation::DeleteWhenStopped);
		const auto markAnimation = new QPropertyAnimation(d, "expandMarkWidth");
		markAnimation->setDuration(300);
		markAnimation->setEasingCurve(QEasingCurve::InOutSine);
		markAnimation->setStartValue(d->expandMarkWidth);
		const qreal step = (width() / 2.0 - d->borderRadius) / (count());
		markAnimation->setEndValue(step * (d->selectedTextList.count()));
		markAnimation->start(QAbstractAnimation::DeleteWhenStopped);
	}
	d->refreshCurrentIndexs();
}

void CALMultiSelectComboBox::hidePopup() {
	Q_D(CALMultiSelectComboBox);

	if (d->isFirstPopup && !this->view()->underMouse()) {
		d->isFirstPopup = false;
		return;
	}
	const auto point = d->comboBoxView->window()->mapFromGlobal(QCursor::pos());
	if (const auto rect = QRectF(d->comboBoxView->mapTo(d->comboBoxView->window(), QPoint(0, 0)), d->comboBoxView->size()); rect.contains(point)) {
		return;
	} else {
		if (d->isAllowHidePopup) {
			if (QWidget* container = this->findChild<QFrame*>()) {
				const int containerHeight = container->height();
				QLayout* layout = container->layout();
				while (layout->count()) {
					layout->takeAt(0);
				}
				const auto viewPosAnimation = new QPropertyAnimation(view(), "pos");
				connect(viewPosAnimation, &QPropertyAnimation::finished, this, [=]() {
					layout->addWidget(view());
					QMouseEvent focusEvent(QEvent::MouseButtonPress, QPoint(-1, -1), QPoint(-1, -1), Qt::NoButton, Qt::NoButton, Qt::NoModifier);
					QApplication::sendEvent(parentWidget(), &focusEvent);
				});
				const QPoint viewPos = view()->pos();
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
				connect(fixedSizeAnimation, &QPropertyAnimation::valueChanged, this, [=](const QVariant& value) {
					container->setFixedHeight(value.toInt());
				});
				fixedSizeAnimation->setStartValue(container->height());
				fixedSizeAnimation->setEndValue(1);
				fixedSizeAnimation->setEasingCurve(QEasingCurve::InCubic);
				fixedSizeAnimation->start(QAbstractAnimation::DeleteWhenStopped);
				d->isAllowHidePopup = false;
			}
			//指示器动画
			const auto rotateAnimation = new QPropertyAnimation(d, "expandIconRotate");
			connect(rotateAnimation, &QPropertyAnimation::valueChanged, this, [=](const QVariant& value) {
				update();
			});
			rotateAnimation->setDuration(300);
			rotateAnimation->setEasingCurve(QEasingCurve::InOutSine);
			rotateAnimation->setStartValue(d->expandIconRotate);
			rotateAnimation->setEndValue(0);
			rotateAnimation->start(QAbstractAnimation::DeleteWhenStopped);
			const auto markAnimation = new QPropertyAnimation(d, "expandMarkWidth");
			markAnimation->setDuration(300);
			markAnimation->setEasingCurve(QEasingCurve::InOutSine);
			markAnimation->setStartValue(d->expandMarkWidth);
			markAnimation->setEndValue(0);
			markAnimation->start(QAbstractAnimation::DeleteWhenStopped);
		}
	}
}
}
