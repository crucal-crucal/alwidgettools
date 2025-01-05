#include "uvmultiselectcombobox.hpp"

#include <QAction>
#include <QApplication>
#include <QLayout>
#include <QMouseEvent>
#include <QPainter>
#include <QPropertyAnimation>
#include <QSvgRenderer>
#include <algorithm>

#include "uvcomboboxstyle.hpp"
#include "uvcomboboxview.hpp"
#include "uvmultiselectcombobox_p.hpp"
#include "uvscrollbar.hpp"

/**
 * @brief \class CUVMultiSelectComboBoxPrivate
 * internal class for CUVMultiSelectComboBox
 * @param q pointer to the public class
 * @param parent pointer to the parent class
 */
CUVMultiSelectComboBoxPrivate::CUVMultiSelectComboBoxPrivate(CUVMultiSelectComboBox* q, QObject* parent): QObject(parent), q_ptr(q) {
}

CUVMultiSelectComboBoxPrivate::~CUVMultiSelectComboBoxPrivate() = default;

void CUVMultiSelectComboBoxPrivate::onItemPressed(const QModelIndex& index) {
	Q_Q(CUVMultiSelectComboBox);

	if (!index.isValid() || index.row() >= _itemSelection.size()) {
		return;
	}

	if (index.row() == 0) {
		const bool allSelected = !_itemSelection[0];
		for (bool& i: _itemSelection) {
			i = allSelected;
		}
		// 更新选择模型
		for (int i = 1; i < q->count(); ++i) {
			QModelIndex itemIndex = q->model()->index(i, 0);
			_comboBoxView->selectionModel()->select(itemIndex, allSelected ? QItemSelectionModel::Select : QItemSelectionModel::Deselect);
		}
	} else {
		_itemSelection[index.row()] = !_comboBoxView->selectionModel()->isSelected(index);
		_comboBoxView->selectionModel()->select(index, _itemSelection[index.row()] ? QItemSelectionModel::Select : QItemSelectionModel::Deselect);

		// 检查是否所有项都被选中
		_itemSelection[0] = std::all_of(_itemSelection.begin() + 1, _itemSelection.end(), [](const bool selected) { return selected; });
	}
	_refreshCurrentIndexs();
	const auto markAnimation = new QPropertyAnimation(this, "pExpandMarkWidth");
	connect(markAnimation, &QPropertyAnimation::valueChanged, this, [=](const QVariant& value) {
		q->update();
	});
	markAnimation->setDuration(300);
	markAnimation->setEasingCurve(QEasingCurve::InOutSine);
	markAnimation->setStartValue(_pExpandMarkWidth);
	const qreal step = static_cast<qreal>(q->width() / 2.0 - _pBorderRadius) / q->count();
	markAnimation->setEndValue(step * _selectedTextList.count());
	markAnimation->start(QAbstractAnimation::DeleteWhenStopped);
}

void CUVMultiSelectComboBoxPrivate::_refreshCurrentIndexs() {
	Q_Q(CUVMultiSelectComboBox);

	QString str{};
	_adjustSelectedVector();
	QVector<bool> selectedIndexVector{};
	bool allSelected = true;
	for (int i = 0; i < q->count(); i++) {
		QModelIndex index = q->model()->index(i, 0);
		if (_itemSelection[i]) {
			_comboBoxView->selectionModel()->select(index, QItemSelectionModel::Select | QItemSelectionModel::Rows);
			if (i != 0) {
				if (!str.isEmpty()) {
					str.append(",");
				}
				str.append(q->itemText(i));
			}
			selectedIndexVector.append(true);
		} else {
			_comboBoxView->selectionModel()->select(index, QItemSelectionModel::Deselect);
			selectedIndexVector.append(false);
			if (i != 0) { // 跳过"全选"项
				allSelected = false;
			}
		}
	}

	_itemSelection[0] = allSelected;
	_comboBoxView->selectionModel()->select(q->model()->index(0, 0), allSelected ? QItemSelectionModel::Select : QItemSelectionModel::Deselect);

	if (str != _currentText) {
		q->update();
		_currentText = str;
#if QT_VERSION >= QT_VERSION_CHECK(5, 14, 0)
		_selectedTextList = _currentText.split(",", Qt::SkipEmptyParts);
#else
		if (_selectedTextList.count() == 1 && _selectedTextList.at(0).isEmpty()) {
			_selectedTextList.clear()
		}
#endif
		//q->setCurrentIndex(-1);
		Q_EMIT q->sigItemSelectionChanged(selectedIndexVector);
		Q_EMIT q->sigCurrentTextListChanged(_selectedTextList);
	}
}

void CUVMultiSelectComboBoxPrivate::_adjustSelectedVector() {
	Q_Q(CUVMultiSelectComboBox);

	while (_itemSelection.count() < q->count()) {
		_itemSelection.append(false);
	}
}

/**
 * @brief \class CUVMultiSelectComboBox
 * @param parent pointer to the parent class
 */
CUVMultiSelectComboBox::CUVMultiSelectComboBox(QWidget* parent): QComboBox(parent), d_ptr(new CUVMultiSelectComboBoxPrivate(this, this)) {
	Q_D(CUVMultiSelectComboBox);

	d->_pBorderRadius = 2;
	d->_pExpandIconRotate = 0;
	d->_pExpandMarkWidth = 0;
	d->_pIconLeftMargin = 20;
	setFixedHeight(35);

	d->_comboBoxStyle = new CUVComboBoxStyle(style());
	setStyle(d->_comboBoxStyle);

	// 初始化container
	d->_comboBoxView = new CUVComboBoxView(this);
	setView(d->_comboBoxView);
	QAbstractItemView* comboBoxView = this->view();
	comboBoxView->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
	const auto scrollBar = new CUVScrollBar(this);
	comboBoxView->setVerticalScrollBar(scrollBar);
	// comboBoxView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff); // 竖直滚动条不可见
	const auto floatVScrollBar = new CUVScrollBar(scrollBar, comboBoxView);
	// floatVScrollBar->setVisible(false); // 竖直滚动条不可见
	floatVScrollBar->setIsAnimation(true);
	comboBoxView->setAutoScroll(false);
	comboBoxView->setSelectionMode(QAbstractItemView::NoSelection);
	comboBoxView->setSelectionBehavior(QAbstractItemView::SelectRows);
	comboBoxView->setObjectName("CUVComboBoxView");
	comboBoxView->setStyleSheet("#CUVComboBoxView { background-color: transparent; }");
	comboBoxView->setStyle(d->_comboBoxStyle);
	if (QWidget* container = this->findChild<QFrame*>()) {
		container->setWindowFlags(Qt::Popup | Qt::FramelessWindowHint | Qt::NoDropShadowWindowHint);
		container->setAttribute(Qt::WA_TranslucentBackground);
		container->setObjectName("CUVComboBoxContainer");
		container->setStyle(d->_comboBoxStyle);
		QLayout* layout = container->layout();
		while (layout->count()) {
			layout->takeAt(0);
		}
		layout->addWidget(view());
		layout->setContentsMargins(0, 0, 0, 6);
#ifndef Q_OS_WIN
		container->setStyleSheet("background-color:transparent;");
#endif
	}
	QComboBox::setMaxVisibleItems(5);
	connect(d->_comboBoxView, &CUVComboBoxView::sigItemPressed, d, &CUVMultiSelectComboBoxPrivate::onItemPressed);
	connect(this, QOverload<int>::of(&CUVMultiSelectComboBox::currentIndexChanged), d, &CUVMultiSelectComboBoxPrivate::_refreshCurrentIndexs);
	d->_itemSelection.resize(32);
	d->_itemSelection.fill(false);
	d->_itemSelection[0] = true;
	QComboBox::setMaxVisibleItems(5);
}

CUVMultiSelectComboBox::~CUVMultiSelectComboBox() = default;

void CUVMultiSelectComboBox::setCurrentSelection(const QString& selection) {
	Q_D(CUVMultiSelectComboBox);

	d->_itemSelection.fill(false);
	d->_comboBoxView->selectionModel()->clearSelection();
	for (int i = 0; i < this->count(); i++) {
		if (selection == itemText(i)) {
			QModelIndex index = model()->index(i, 0);
			d->_comboBoxView->selectionModel()->select(index, QItemSelectionModel::Select | QItemSelectionModel::Rows);
			d->_itemSelection[index.row()] = true;
		}
	}
	d->_refreshCurrentIndexs();
}

void CUVMultiSelectComboBox::setCurrentSelection(const QStringList& selection) {
	Q_D(CUVMultiSelectComboBox);

	d->_itemSelection.fill(false);
	d->_comboBoxView->selectionModel()->clearSelection();
	for (int i = 0; i < this->count(); i++) {
		if (selection.contains(itemText(i))) {
			QModelIndex index = model()->index(i, 0);
			d->_comboBoxView->selectionModel()->select(index, QItemSelectionModel::Select | QItemSelectionModel::Rows);
			d->_itemSelection[index.row()] = true;
		}
	}
	d->_refreshCurrentIndexs();
}

void CUVMultiSelectComboBox::setCurrentSelection(const int index) {
	Q_D(CUVMultiSelectComboBox);

	if (index >= this->count() || index < 0) {
		return;
	}

	d->_itemSelection.fill(false);
	d->_comboBoxView->selectionModel()->clearSelection();
	const QModelIndex currentIndex = model()->index(index, 0);
	d->_comboBoxView->selectionModel()->select(currentIndex, QItemSelectionModel::Select | QItemSelectionModel::Rows);
	d->_itemSelection[index] = true;
	d->_refreshCurrentIndexs();
}

void CUVMultiSelectComboBox::setCurrentSelection(const QList<int>& selectionIndex) {
	Q_D(CUVMultiSelectComboBox);

	d->_itemSelection.fill(false);
	d->_comboBoxView->selectionModel()->clearSelection();
	for (const auto& index: selectionIndex) {
		if (index >= this->count() || index < 0) {
			continue;
		}
		QModelIndex currentIndex = model()->index(index, 0);
		d->_comboBoxView->selectionModel()->select(currentIndex, QItemSelectionModel::Select | QItemSelectionModel::Rows);
		d->_itemSelection[index] = true;
	}
	d->_refreshCurrentIndexs();
}

QStringList CUVMultiSelectComboBox::getCurrentSelection() const {
	Q_D(const CUVMultiSelectComboBox);

	return d->_selectedTextList;
}

QList<int> CUVMultiSelectComboBox::getCurrentSelectionIndex() const {
	Q_D(const CUVMultiSelectComboBox);

	QList<int> indexList{};
	for (int i = 0; i < d->_itemSelection.size(); i++) {
		if (d->_itemSelection[i]) {
			indexList.append(i);
		}
	}

	return indexList;
}

void CUVMultiSelectComboBox::setBorderRadius(const int radius) {
	Q_D(CUVMultiSelectComboBox);

	d->_pBorderRadius = radius;
	Q_EMIT sigBorderRadiusChanged();
}

int CUVMultiSelectComboBox::getBorderRadius() const {
	Q_D(const CUVMultiSelectComboBox);

	return d->_pBorderRadius;
}

void CUVMultiSelectComboBox::selectAllItems() {
	Q_D(CUVMultiSelectComboBox);

	QVector<int> indices(d->_itemSelection.size());
	std::iota(indices.begin(), indices.end(), 0); // 直接填充 [0, 1, ..., N-1]
	setCurrentSelection(QList<int>::fromVector(indices));
}

void CUVMultiSelectComboBox::deselectAllItems() {
	setCurrentSelection(QList<int>());
}

bool CUVMultiSelectComboBox::isAllSelected() const {
	Q_D(const CUVMultiSelectComboBox);

	// 检查是否所有项目都被选中
	for (int i = 1; i < d->_itemSelection.size(); ++i) {
		if (!d->_itemSelection[i]) {
			return false;
		}
	}
	return true;
}

void CUVMultiSelectComboBox::setIcon(const QPixmap& pixmap) {
	Q_D(CUVMultiSelectComboBox);

	d->_pIcon = pixmap;
}

void CUVMultiSelectComboBox::setSvgIcon(const QString& iconpath) {
	Q_D(CUVMultiSelectComboBox);

	if (iconpath.isEmpty()) {
		return;
	}

	if (QSvgRenderer svg_renderer(iconpath); svg_renderer.isValid()) {
		QPixmap pixmap(height(), height());
		pixmap.fill(Qt::transparent);

		QPainter painter(&pixmap);
		svg_renderer.render(&painter);
		painter.end();
		d->_pIcon = pixmap;
	}
}

void CUVMultiSelectComboBox::setIcon(const QIcon& icon, const int width, const int height) {
	Q_D(CUVMultiSelectComboBox);

	d->_pIcon = icon.pixmap(width, height);
}

void CUVMultiSelectComboBox::paintEvent(QPaintEvent* event) {
	Q_D(CUVMultiSelectComboBox);

	QPainter painter(this);
	painter.save();
	painter.setRenderHints(QPainter::SmoothPixmapTransform | QPainter::Antialiasing | QPainter::TextAntialiasing);
	painter.setPen(QColor(0x4B, 0x4B, 0x4D));
	painter.setBrush(isEnabled() ? underMouse() ? QColor(0x44, 0x44, 0x44) : QColor(0x3B, 0x3B, 0x3B) : Qt::transparent);
	QRect foregroundRect = rect();
	foregroundRect.adjust(1, 1, -1, -1); // 上下左右内部边距
	painter.drawRoundedRect(foregroundRect, d->_pBorderRadius, d->_pBorderRadius);
	// 图标绘制
	int textX;
	if (!d->_pIcon.isNull()) {
		// const int iconY = (height() - d->_pIcon.height()) / 2;
		painter.drawPixmap(d->_pIconLeftMargin, 5, height() - 10, height() - 10, d->_pIcon);
		textX = d->_pIconLeftMargin + height() - 5;
	} else {
		textX = d->_pIconLeftMargin + 10;
	}
	// 文字绘制
	painter.setPen(isEnabled() ? Qt::white : Qt::gray);
	// const QString currentText = painter.fontMetrics().elidedText(tr("Automatically track target Settings"), Qt::ElideRight, foregroundRect.width() - 27 - static_cast<int>(width() * 0.05));
	const QString currentText = painter.fontMetrics().elidedText(d->_currentText, Qt::ElideRight, foregroundRect.width() - 27 - static_cast<int>(width() * 0.05));
	QFont font = painter.font();
	font.setPixelSize(12);
	painter.setFont(font);
	const int textY = (height() + painter.fontMetrics().ascent() - painter.fontMetrics().descent()) / 2;
	painter.drawText(textX, textY, currentText);
	// 展开指示器绘制
	painter.setPen(Qt::NoPen);
	painter.setBrush(QColor(0x4C, 0xA0, 0xE0));
	painter.drawRoundedRect(QRectF(width() / 2.0 - d->_pExpandMarkWidth, height() - 3, d->_pExpandMarkWidth * 2, 3), 2, 2);
	// 展开图标绘制
	if (count() > 0) {
		const QRectF expandIconRect(width() - 25, 0, 20, height());
		const QPointF centerPoint = expandIconRect.center();
		const int size = static_cast<int>(qMin(expandIconRect.width(), expandIconRect.height()));
		const QPixmap expandIcon = QPixmap(":icon/control/combobox_down_arrow.svg").scaled(size, size, Qt::KeepAspectRatio, Qt::SmoothTransformation);
		painter.setPen(isEnabled() ? Qt::white : Qt::gray);
		painter.translate(centerPoint);
		painter.rotate(d->_pExpandIconRotate);
		const QPointF drawPoint(-expandIcon.width() / 2.0, -expandIcon.height() / 2.0);
		if (isEnabled()) {
			painter.drawPixmap(drawPoint, expandIcon);
		} else {
			painter.setOpacity(0.5);
			painter.drawPixmap(drawPoint, expandIcon);
		}
		painter.restore();
	}
}

void CUVMultiSelectComboBox::showPopup() {
	Q_D(CUVMultiSelectComboBox);

	bool oldAnimationEffects = qApp->isEffectEnabled(Qt::UI_AnimateCombo); // NOLINT
	qApp->setEffectEnabled(Qt::UI_AnimateCombo, false);                    // NOLINT
	QComboBox::showPopup();
	qApp->setEffectEnabled(Qt::UI_AnimateCombo, oldAnimationEffects); // NOLINT

	if (count() > 0) {
		if (QWidget* container = this->findChild<QFrame*>()) {
			const int containerHeight = container->height();
			container->move(container->x(), container->y() + 3);
			QLayout* layout = container->layout();
			while (layout->count()) {
				layout->takeAt(0);
			}
			const auto fixedSizeAnimation = new QPropertyAnimation(container, "maximumHeight");
			connect(fixedSizeAnimation, &QPropertyAnimation::valueChanged, this, [=](const QVariant& value) {
				container->setFixedHeight(value.toInt());
			});
			fixedSizeAnimation->setStartValue(1);
			fixedSizeAnimation->setEndValue(containerHeight);
			fixedSizeAnimation->setEasingCurve(QEasingCurve::OutCubic);
			fixedSizeAnimation->setDuration(200);
			fixedSizeAnimation->start(QAbstractAnimation::DeleteWhenStopped);

			const auto viewPosAnimation = new QPropertyAnimation(view(), "pos");
			connect(viewPosAnimation, &QPropertyAnimation::finished, this, [=]() {
				d->_isAllowHidePopup = true;
				layout->addWidget(view());
			});
			const QPoint viewPos = view()->pos();
			viewPosAnimation->setStartValue(QPoint(viewPos.x(), viewPos.y() - view()->height()));
			viewPosAnimation->setEndValue(viewPos);
			viewPosAnimation->setEasingCurve(QEasingCurve::OutCubic);
			viewPosAnimation->setDuration(200);
			viewPosAnimation->start(QAbstractAnimation::DeleteWhenStopped);
		}
		//指示器动画
		const auto rotateAnimation = new QPropertyAnimation(d, "pExpandIconRotate");
		connect(rotateAnimation, &QPropertyAnimation::valueChanged, this, [=](const QVariant& value) {
			update();
		});
		rotateAnimation->setDuration(200);
		rotateAnimation->setEasingCurve(QEasingCurve::InOutSine);
		rotateAnimation->setStartValue(d->_pExpandIconRotate);
		rotateAnimation->setEndValue(-180);
		rotateAnimation->start(QAbstractAnimation::DeleteWhenStopped);
		const auto markAnimation = new QPropertyAnimation(d, "pExpandMarkWidth");
		markAnimation->setDuration(200);
		markAnimation->setEasingCurve(QEasingCurve::InOutSine);
		markAnimation->setStartValue(d->_pExpandMarkWidth);
		const qreal step = (width() / 2.0 - d->_pBorderRadius) / count();
		markAnimation->setEndValue(step * d->_selectedTextList.count());
		markAnimation->start(QAbstractAnimation::DeleteWhenStopped);
	}
	d->_refreshCurrentIndexs();
}

void CUVMultiSelectComboBox::hidePopup() {
	Q_D(CUVMultiSelectComboBox);

	if (d->_isFirstPopup && !this->view()->underMouse()) {
		d->_isFirstPopup = false;
		return;
	}
	const auto point = d->_comboBoxView->window()->mapFromGlobal(QCursor::pos());
	if (const auto rect = QRectF(d->_comboBoxView->mapTo(d->_comboBoxView->window(), QPoint(0, 0)), d->_comboBoxView->size()); rect.contains(point)) {
		return;
	} else {
		if (d->_isAllowHidePopup) {
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
				d->_isAllowHidePopup = false;
			}
			//指示器动画
			const auto rotateAnimation = new QPropertyAnimation(d, "pExpandIconRotate");
			connect(rotateAnimation, &QPropertyAnimation::valueChanged, this, [=](const QVariant& value) {
				update();
			});
			rotateAnimation->setDuration(200);
			rotateAnimation->setEasingCurve(QEasingCurve::InOutSine);
			rotateAnimation->setStartValue(d->_pExpandIconRotate);
			rotateAnimation->setEndValue(0);
			rotateAnimation->start(QAbstractAnimation::DeleteWhenStopped);
			const auto markAnimation = new QPropertyAnimation(d, "pExpandMarkWidth");
			markAnimation->setDuration(200);
			markAnimation->setEasingCurve(QEasingCurve::InOutSine);
			markAnimation->setStartValue(d->_pExpandMarkWidth);
			markAnimation->setEndValue(0);
			markAnimation->start(QAbstractAnimation::DeleteWhenStopped);
		}
	}
}
