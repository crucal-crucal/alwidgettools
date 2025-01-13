#include "allineedit.hpp"

#include <QClipboard>
#include <QContextMenuEvent>
#include <QGuiApplication>
#include <QPainter>
#include <QPropertyAnimation>

#include "alapplication.hpp"
#include "aleventbus.hpp"
#include "allineeditstyle.hpp"
#include "allineedit_p.hpp"
#include "almenu.hpp"
#include "althememanager.hpp"

namespace AL {
/**
 * @brief \class CALLineEditPrivate
 * Internal class for CALLineEdit
 * @param q pointer to the public class
 * @param parent pointer to the parent class
 */
CALLineEditPrivate::CALLineEditPrivate(CALLineEdit* q, QObject* parent): QObject(parent), q_ptr(q) {
}

CALLineEditPrivate::~CALLineEditPrivate() = default;

void CALLineEditPrivate::setExpandMarkWidth(const qreal expandMarkWidth) {
	this->expandMarkWidth = expandMarkWidth;
	Q_EMIT sigExpandMarkWidthChanged();
}

qreal CALLineEditPrivate::getExpandMarkWidth() const {
	return this->expandMarkWidth;
}

void CALLineEditPrivate::invokableWMWindowClickedEvent(const QVariantMap& data) {
	Q_Q(CALLineEdit);

	if (const auto actionType = data.value("WMClickType").value<ALAppBarType::WMMouseActionType>(); actionType == ALAppBarType::WMLBUTTONDOWN) {
		if (q->hasSelectedText() && q->hasFocus()) {
			q->clearFocus();
		}
	} else if (actionType == ALAppBarType::WMLBUTTONUP || actionType == ALAppBarType::WMNCLBUTTONDOWN) {
		if (CALApplication::containsCursorToItem(q) || (actionType == ALAppBarType::WMLBUTTONUP && q->hasSelectedText())) {
			return;
		}
		if (q->hasFocus()) {
			q->clearFocus();
		}
	}
}

void CALLineEditPrivate::slotThemeChanged(const ALThemeType::ThemeMode& mode) {
	Q_Q(CALLineEdit);

	themeMode = mode;
	QPalette palette;
	palette.setColor(QPalette::Text, mode == ALThemeType::Light ? Qt::black : Qt::white);
	palette.setColor(QPalette::PlaceholderText, mode == ALThemeType::Light ? QColor(0x00, 0x00, 0x00, 128) : QColor(0xBA, 0xBA, 0xBA));
	q->setPalette(palette);
}

CALLineEdit::CALLineEdit(QWidget* parent): QLineEdit(parent), d_ptr(new CALLineEditPrivate(this, this)) {
	Q_D(CALLineEdit);

	setObjectName("CALLineEdit");
	setStyleSheet("#CALLineEdit { padding-left: 10px; } ");

	d->themeMode = ALTheme->getThemeMode();
	d->borderRadius = 6;
	d->expandMarkWidth = 0;
	d->isClearButtonEnable = true;
	setFocusPolicy(Qt::StrongFocus);
	// 事件总线
	d->focusEvent = new CALEvent("WMWindowClicked", "invokableWMWindowClickedEvent", d);
	d->focusEvent->registerAndInit();
	setMouseTracking(true);
	QFont textFont = font();
	textFont.setLetterSpacing(QFont::AbsoluteSpacing, d->textSpacing);
	setFont(textFont);
	setStyle(new CALLineEditStyle(style()));
	d->slotThemeChanged(ALTheme->getThemeMode());
	connect(ALTheme, &CALThemeManager::sigThemeModeChanged, d, &CALLineEditPrivate::slotThemeChanged);
}

CALLineEdit::~CALLineEdit() = default;

void CALLineEdit::setBorderRadius(const int borderRadius) {
	d_func()->borderRadius = borderRadius;
	Q_EMIT sigBorderRadiusChanged();
}

int CALLineEdit::getBorderRadius() const {
	return d_func()->borderRadius;
}

void CALLineEdit::setIsClearButtonEnable(const bool isEnable) {
	d_func()->isClearButtonEnable = isEnable;
	setClearButtonEnabled(isEnable);
	Q_EMIT sigIsClearButtonEnableChanged();
}

bool CALLineEdit::isClearButtonEnable() const {
	return d_func()->isClearButtonEnable;
}

void CALLineEdit::focusInEvent(QFocusEvent* event) {
	Q_D(CALLineEdit);

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

void CALLineEdit::focusOutEvent(QFocusEvent* event) {
	Q_D(CALLineEdit);

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

void CALLineEdit::paintEvent(QPaintEvent* event) {
	Q_D(CALLineEdit);

	QLineEdit::paintEvent(event);
	QPainter painter(this);
	painter.save();
	painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);
	painter.setPen(Qt::NoPen);
	painter.setBrush(ALThemeColor(d->themeMode, ALThemeType::PrimaryNormal));
	painter.drawRoundedRect(QRectF(width() / 2.0 - d->expandMarkWidth, height() - 2.5, d->expandMarkWidth * 2.0, 2.5), 2, 2);
	painter.restore();
}

void CALLineEdit::contextMenuEvent(QContextMenuEvent* event) {
	const auto menu = new CALMenu(this);
	menu->setMenuItemHeight(27);
	menu->setAttribute(Qt::WA_DeleteOnClose);
	QAction* action;
	if (!isReadOnly()) {
		action = menu->addAction(ALIcon::AweSomeIcon::ArrowRotateLeft, tr("Undo"), QKeySequence::Undo);
		action->setEnabled(isUndoAvailable());
		connect(action, &QAction::triggered, this, &CALLineEdit::undo);

		action = menu->addAction(ALIcon::AweSomeIcon::ArrowRotateRight, tr("Redo"), QKeySequence::Redo);
		action->setEnabled(isRedoAvailable());
		connect(action, &QAction::triggered, this, &CALLineEdit::redo);
		menu->addSeparator();
	}

#ifndef QT_NO_CLIPBOARD
	if (!isReadOnly()) {
		action = menu->addAction(ALIcon::AweSomeIcon::KnifeKitchen, tr("Cut"), QKeySequence::Cut);
		action->setEnabled(!isReadOnly() && hasSelectedText() && echoMode() == QLineEdit::Normal);
		connect(action, &QAction::triggered, this, &CALLineEdit::cut);
	}

	action = menu->addAction(ALIcon::AweSomeIcon::Copy, tr("Copy"), QKeySequence::Copy);
	action->setEnabled(hasSelectedText() && echoMode() == QLineEdit::Normal);
	connect(action, &QAction::triggered, this, &CALLineEdit::copy);

	if (!isReadOnly()) {
		action = menu->addAction(ALIcon::AweSomeIcon::Paste, tr("Paste"), QKeySequence::Paste);
		action->setEnabled(!isReadOnly() && !QGuiApplication::clipboard()->text().isEmpty());
		connect(action, &QAction::triggered, this, &CALLineEdit::paste);
	}
#endif
	if (!isReadOnly()) {
		action = menu->addAction(ALIcon::AweSomeIcon::DeleteLeft, tr("delete"));
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
	connect(action, &QAction::triggered, this, &CALLineEdit::selectAll);
	menu->popup(event->globalPos());
}
} // namespace AL