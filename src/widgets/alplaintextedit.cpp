#include "alplaintextedit.hpp"

#include <QClipboard>
#include <QGuiApplication>
#include <QPropertyAnimation>

#include "alapplication.hpp"
#include "alplaintextedit_p.hpp"
#include "alplaintexteditstyle.hpp"
#include "aleventbus.hpp"
#include "almenu.hpp"
#include "alscrollbar.hpp"
#include "althememanager.hpp"

/**
 * @brief \namespace AL
 */
namespace AL {
/**
 * @brief \class CALPlainTextEditPrivate
 * Internal class for CALPlainTextEdit
 * @param q pointer to the public class
 * @param parent pointer to the parent class
 */
CALPlainTextEditPrivate::CALPlainTextEditPrivate(CALPlainTextEdit* q, QObject* parent): QObject(parent), q_ptr(q) {
}

CALPlainTextEditPrivate::~CALPlainTextEditPrivate() = default;

void CALPlainTextEditPrivate::invokableWMWindowClickedEvent(const QVariantMap& data) {
	Q_Q(CALPlainTextEdit);

	if (const auto actionType = data.value("WMClickType").value<ALAppBarType::WMMouseActionType>(); actionType == ALAppBarType::WMLBUTTONDOWN) {
		if (!q->toPlainText().isEmpty() && q->hasFocus()) {
			q->clearFocus();
		}
	} else if (actionType == ALAppBarType::WMLBUTTONUP || actionType == ALAppBarType::WMNCLBUTTONDOWN) {
		if (CALApplication::containsCursorToItem(q) || (actionType == ALAppBarType::WMLBUTTONUP && !q->toPlainText().isEmpty())) {
			return;
		}
		if (q->hasFocus()) {
			q->clearFocus();
		}
	}
}

void CALPlainTextEditPrivate::slotThemeChanged(const ALThemeType::ThemeMode& mode) {
	Q_Q(CALPlainTextEdit);

	QPalette palette;
	if (mode == ALThemeType::Light) {
		palette.setColor(QPalette::Text, Qt::black);
		palette.setColor(QPalette::PlaceholderText, QColor(0x00, 0x00, 0x00, 128));
	} else {
		palette.setColor(QPalette::Text, Qt::white);
		palette.setColor(QPalette::PlaceholderText, QColor(0xBA, 0xBA, 0xBA));
	}
	q->setPalette(palette);
	themeMode = mode;
}

/**
 * @brief \class CALPlainTextEdit
 * @param parent pointer to the parent class
 */
CALPlainTextEdit::CALPlainTextEdit(QWidget* parent): QPlainTextEdit(parent), d_ptr(new CALPlainTextEditPrivate(this, this)) {
	Q_D(CALPlainTextEdit);

	setObjectName("CALPlainTextEdit");
	setStyleSheet("#CALPlainTextEdit { background-color: transparent }");
	setHorizontalScrollBar(new CALScrollBar(this));
	setVerticalScrollBar(new CALScrollBar(this));
	setMouseTracking(true);

	d->focusEvent = new CALEvent("WMWindowClicked", "invokableWMWindowClickedEvent", d);
	d->focusEvent->registerAndInit();
	d->style = new CALPlainTextEditStyle(style());
	setStyle(d->style);
	d->themeMode = ALTheme->getThemeMode();
	connect(ALTheme, &CALThemeManager::sigThemeModeChanged, d, &CALPlainTextEditPrivate::slotThemeChanged);
}

CALPlainTextEdit::CALPlainTextEdit(const QString& text, QWidget* parent): CALPlainTextEdit(parent) {
	setPlainText(text);
}

CALPlainTextEdit::~CALPlainTextEdit() = default;

void CALPlainTextEdit::focusInEvent(QFocusEvent* event) {
	Q_D(CALPlainTextEdit);

	if (event->reason() == Qt::MouseFocusReason) {
		const auto markAnimation = new QPropertyAnimation(d->style, "expandMarkWidth");
		connect(markAnimation, &QPropertyAnimation::valueChanged, this, [=]() { this->update(); });
		markAnimation->setDuration(300);
		markAnimation->setEasingCurve(QEasingCurve::InOutSine);
		markAnimation->setStartValue(d->style->getExpandMarkWidth());
		markAnimation->setEndValue(width() / 2 - 3);
		markAnimation->start(QAbstractAnimation::DeleteWhenStopped);
	}

	QPlainTextEdit::focusInEvent(event);
}

void CALPlainTextEdit::focusOutEvent(QFocusEvent* event) {
	Q_D(CALPlainTextEdit);

	if (event->reason() != Qt::PopupFocusReason) {
		const auto markAnimation = new QPropertyAnimation(d->style, "expandMarkWidth");
		connect(markAnimation, &QPropertyAnimation::valueChanged, this, [=]() { this->update(); });
		markAnimation->setDuration(300);
		markAnimation->setEasingCurve(QEasingCurve::InOutSine);
		markAnimation->setStartValue(d->style->getExpandMarkWidth());
		markAnimation->setEndValue(0);
		markAnimation->start(QAbstractAnimation::DeleteWhenStopped);
	}

	QPlainTextEdit::focusOutEvent(event);
}

void CALPlainTextEdit::contextMenuEvent(QContextMenuEvent* event) {
	const auto menu = new CALMenu(this);
	menu->setMenuItemHeight(27);
	menu->setAttribute(Qt::WA_DeleteOnClose);
	QAction* action;
	if (!isReadOnly()) {
		action = menu->addAction(ALIcon::AweSomeIcon::ArrowRotateLeft, tr("Undo"), QKeySequence::Undo);
		action->setEnabled(isUndoRedoEnabled() ? document()->isUndoRedoEnabled() : false);
		connect(action, &QAction::triggered, this, &CALPlainTextEdit::undo);

		action = menu->addAction(ALIcon::AweSomeIcon::ArrowRotateRight, tr("Redo"), QKeySequence::Redo);
		action->setEnabled(isUndoRedoEnabled() ? document()->isUndoRedoEnabled() : false);
		connect(action, &QAction::triggered, this, &CALPlainTextEdit::redo);
		menu->addSeparator();
	}

#ifndef QT_NO_CLIPBOARD
	if (!isReadOnly()) {
		action = menu->addAction(ALIcon::AweSomeIcon::KnifeKitchen, tr("Cut"), QKeySequence::Cut);
		action->setEnabled(!isReadOnly() && !textCursor().selectedText().isEmpty());
		connect(action, &QAction::triggered, this, &CALPlainTextEdit::cut);
	}

	action = menu->addAction(ALIcon::AweSomeIcon::Copy, tr("Copy"), QKeySequence::Copy);
	action->setEnabled(!textCursor().selectedText().isEmpty());
	connect(action, &QAction::triggered, this, &CALPlainTextEdit::copy);

	if (!isReadOnly()) {
		action = menu->addAction(ALIcon::AweSomeIcon::Paste, tr("Paste"), QKeySequence::Paste);
		action->setEnabled(!isReadOnly() && !QGuiApplication::clipboard()->text().isEmpty());
		connect(action, &QAction::triggered, this, &CALPlainTextEdit::paste);
	}
#endif
	if (!isReadOnly()) {
		action = menu->addAction(ALIcon::AweSomeIcon::DeleteLeft, tr("delete"));
		action->setEnabled(!isReadOnly() && !toPlainText().isEmpty() && !textCursor().selectedText().isEmpty());
		connect(action, &QAction::triggered, this, [=]() {
			if (!textCursor().selectedText().isEmpty()) {
				textCursor().deleteChar();
			}
		});
	}
	if (!menu->isEmpty()) {
		menu->addSeparator();
	}
	action = menu->addAction(tr("select all"));
	action->setShortcut(QKeySequence::SelectAll);
	action->setEnabled(!toPlainText().isEmpty() && textCursor().selectedText() != toPlainText());
	connect(action, &QAction::triggered, this, &CALPlainTextEdit::selectAll);
	menu->popup(event->globalPos());
	this->setFocus();
}
}
