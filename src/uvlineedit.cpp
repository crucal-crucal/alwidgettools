#include "uvlineedit.hpp"

#include <QClipboard>
#include <QContextMenuEvent>
#include <QGuiApplication>
#include <QPainter>
#include <QPropertyAnimation>

#include "uvapplication.hpp"
#include "uveventbus.hpp"
#include "uvlineeditstyle.hpp"
#include "uvlineedit_p.hpp"
#include "uvmenu.hpp"
#include "uvthememanager.hpp"

/**
 * @brief \class CUVLineEditPrivate
 * Internal class for CUVLineEdit
 * @param q pointer to the public class
 * @param parent pointer to the parent class
 */
CUVLineEditPrivate::CUVLineEditPrivate(CUVLineEdit* q, QObject* parent): QObject(parent), q_ptr(q) {
}

CUVLineEditPrivate::~CUVLineEditPrivate() = default;

void CUVLineEditPrivate::setExpandMarkWidth(const qreal expandMarkWidth) {
	this->expandMarkWidth = expandMarkWidth;
	Q_EMIT sigExpandMarkWidthChanged();
}

qreal CUVLineEditPrivate::getExpandMarkWidth() const {
	return this->expandMarkWidth;
}

void CUVLineEditPrivate::invokableWMWindowClickedEvent(const QVariantMap& data) {
	Q_Q(CUVLineEdit);

	if (const auto actionType = data.value("WMClickType").value<UVAppBarType::WMMouseActionType>(); actionType == UVAppBarType::WMLBUTTONDOWN) {
		if (q->hasSelectedText() && q->hasFocus()) {
			q->clearFocus();
		}
	} else if (actionType == UVAppBarType::WMLBUTTONUP || actionType == UVAppBarType::WMNCLBUTTONDOWN) {
		if (CUVApplication::containsCursorToItem(q) || (actionType == UVAppBarType::WMLBUTTONUP && q->hasSelectedText())) {
			return;
		}
		if (q->hasFocus()) {
			q->clearFocus();
		}
	}
}

void CUVLineEditPrivate::slotThemeChanged(const UVThemeType::ThemeMode& mode) {
	Q_Q(CUVLineEdit);

	themeMode = mode;
	QPalette palette;
	palette.setColor(QPalette::Text, mode == UVThemeType::Light ? Qt::black : Qt::white);
	palette.setColor(QPalette::PlaceholderText, mode == UVThemeType::Light ? QColor(0x00, 0x00, 0x00, 128) : QColor(0xBA, 0xBA, 0xBA));
	q->setPalette(palette);
}

CUVLineEdit::CUVLineEdit(QWidget* parent): QLineEdit(parent), d_ptr(new CUVLineEditPrivate(this, this)) {
	Q_D(CUVLineEdit);

	setObjectName("CUVLineEdit");
	d->themeMode = UVTheme->getThemeMode();
	d->borderRadius = 6;
	d->expandMarkWidth = 0;
	d->isClearButtonEnable = true;
	setFocusPolicy(Qt::StrongFocus);
	// 事件总线
	d->focusEvent = new CUVEvent("WMWindowClicked", "invokableWMWindowClickedEvent", d);
	d->focusEvent->registerAndInit();
	setMouseTracking(true);
	QFont textFont = font();
	textFont.setLetterSpacing(QFont::AbsoluteSpacing, d->textSpacing);
	setFont(textFont);
	setStyle(new CUVLineEditStyle(style()));
	setStyleSheet("#CUVLineEdit { padding-left: 10px; } ");
	d->slotThemeChanged(UVTheme->getThemeMode());
	connect(UVTheme, &CUVThemeManager::sigThemeModeChanged, d, &CUVLineEditPrivate::slotThemeChanged);
}

CUVLineEdit::~CUVLineEdit() = default;

void CUVLineEdit::setBorderRadius(const int borderRadius) {
	d_func()->borderRadius = borderRadius;
	Q_EMIT sigBorderRadiusChanged();
}

int CUVLineEdit::getBorderRadius() const {
	return d_func()->borderRadius;
}

void CUVLineEdit::setIsClearButtonEnable(const bool isEnable) {
	d_func()->isClearButtonEnable = isEnable;
	setClearButtonEnabled(isEnable);
	Q_EMIT sigIsClearButtonEnableChanged();
}

bool CUVLineEdit::isClearButtonEnable() const {
	return d_func()->isClearButtonEnable;
}

void CUVLineEdit::focusInEvent(QFocusEvent* event) {
	Q_D(CUVLineEdit);

	Q_EMIT sigFocusIn(this->text());
	if (event->reason() == Qt::MouseFocusReason) {
		setIsClearButtonEnable(d->isClearButtonEnable);
		const auto markAnimation = new QPropertyAnimation(d, "expandMarkWidth");
		connect(markAnimation, &QPropertyAnimation::valueChanged, this, [=]() { update(); });
		markAnimation->setDuration(300);
		markAnimation->setEasingCurve(QEasingCurve::InOutSine);
		markAnimation->setStartValue(d->expandMarkWidth);
		markAnimation->setEndValue((width() - d->borderRadius) / 2);
		markAnimation->start(QAbstractAnimation::DeleteWhenStopped);
	}

	QLineEdit::focusInEvent(event);
}

void CUVLineEdit::focusOutEvent(QFocusEvent* event) {
	Q_D(CUVLineEdit);

	Q_EMIT sigFocusOut(this->text());
	if (event->reason() == Qt::PopupFocusReason) {
		setIsClearButtonEnable(d->isClearButtonEnable);
		const auto markAnimation = new QPropertyAnimation(d, "expandMarkWidth");
		connect(markAnimation, &QPropertyAnimation::valueChanged, this, [=]() { update(); });
		markAnimation->setDuration(300);
		markAnimation->setEasingCurve(QEasingCurve::InOutSine);
		markAnimation->setStartValue(d->expandMarkWidth);
		markAnimation->setEndValue(0);
		markAnimation->start(QAbstractAnimation::DeleteWhenStopped);
		Q_EMIT sigWmFocusOut(this->text());
	}

	QLineEdit::focusOutEvent(event);
}

void CUVLineEdit::paintEvent(QPaintEvent* event) {
	Q_D(CUVLineEdit);

	QLineEdit::paintEvent(event);
	QPainter painter(this);
	painter.save();
	painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);
	painter.setPen(Qt::NoPen);
	painter.setBrush(UVThemeColor(d->themeMode, UVThemeType::PrimaryNormal));
	painter.drawRoundedRect(QRectF(width() / 2.0 - d->expandMarkWidth, height() - 2.5, d->expandMarkWidth * 2.0, 2.5), 2, 2);
	painter.restore();
}

void CUVLineEdit::contextMenuEvent(QContextMenuEvent* event) {
	const auto menu = new CUVMenu(this);
	menu->setMenuItemHeight(27);
	menu->setAttribute(Qt::WA_DeleteOnClose);
	QAction* action;
	if (!isReadOnly()) {
		action = menu->addAction(UVIcon::CUVAweSomeIcon::ArrowRotateLeft, tr("Undo"), QKeySequence::Undo);
		action->setEnabled(isUndoAvailable());
		connect(action, &QAction::triggered, this, &CUVLineEdit::undo);

		action = menu->addAction(UVIcon::CUVAweSomeIcon::ArrowRotateRight, tr("Redo"), QKeySequence::Redo);
		action->setEnabled(isRedoAvailable());
		connect(action, &QAction::triggered, this, &CUVLineEdit::redo);
		menu->addSeparator();
	}

#ifndef QT_NO_CLIPBOARD
	if (!isReadOnly()) {
		action = menu->addAction(UVIcon::CUVAweSomeIcon::KnifeKitchen, tr("Cut"), QKeySequence::Cut);
		action->setEnabled(!isReadOnly() && hasSelectedText() && echoMode() == QLineEdit::Normal);
		connect(action, &QAction::triggered, this, &CUVLineEdit::cut);
	}

	action = menu->addAction(UVIcon::CUVAweSomeIcon::Copy, tr("Copy"), QKeySequence::Copy);
	action->setEnabled(hasSelectedText() && echoMode() == QLineEdit::Normal);
	connect(action, &QAction::triggered, this, &CUVLineEdit::copy);

	if (!isReadOnly()) {
		action = menu->addAction(UVIcon::CUVAweSomeIcon::Paste, tr("Paste"), QKeySequence::Paste);
		action->setEnabled(!isReadOnly() && !QGuiApplication::clipboard()->text().isEmpty());
		connect(action, &QAction::triggered, this, &CUVLineEdit::paste);
	}
#endif
	if (!isReadOnly()) {
		action = menu->addAction(UVIcon::CUVAweSomeIcon::DeleteLeft, tr("delete"));
		action->setEnabled(!isReadOnly() && !text().isEmpty() && hasSelectedText());
		connect(action, &QAction::triggered, this, [=]() {
			if (hasSelectedText()) {
				setText(text().remove(selectionStart(), selectionEnd() - selectionStart()));
			}
		});
	}
	if (!menu->isEmpty()) {
		menu->addSeparator();
	}
	action = menu->addAction(tr("select all"));
	action->setShortcut(QKeySequence::SelectAll);
	action->setEnabled(!text().isEmpty() && selectedText() != text());
	connect(action, &QAction::triggered, this, &CUVLineEdit::selectAll);
	menu->popup(event->globalPos());
}
