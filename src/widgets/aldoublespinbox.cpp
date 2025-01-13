#include "aldoublespinbox.hpp"

#include <QClipboard>
#include <QContextMenuEvent>
#include <QGuiApplication>
#include <QLineEdit>

#include "almenu.hpp"
#include "alspinboxstyle.hpp"
#include "aldoublespinbox_p.hpp"
#include "althememanager.hpp"

namespace AL {
/**
 * \class CALDoubleSpinBoxPrivate
 * Internal class for CALDoubleSpinBox
 * @param q pointer to the public class
 * @param parent pointer to the parent class
 */
CALDoubleSpinBoxPrivate::CALDoubleSpinBoxPrivate(CALDoubleSpinBox* q, QObject* parent): QObject(parent), q_ptr(q) {
}

CALDoubleSpinBoxPrivate::~CALDoubleSpinBoxPrivate() = default;

CALMenu* CALDoubleSpinBoxPrivate::createStandardContextMenu() {
	Q_Q(CALDoubleSpinBox);

	QLineEdit* lineEdit = q->lineEdit();
	const auto menu = new CALMenu(q);
	menu->setMenuItemHeight(27);
	menu->setAttribute(Qt::WA_DeleteOnClose);
	QAction* action{ nullptr };
	if (!lineEdit->isReadOnly()) {
		action = menu->addAction(ALIcon::AweSomeIcon::ArrowRotateLeft, tr("Undo"), QKeySequence::Undo);
		action->setEnabled(lineEdit->isUndoAvailable());
		connect(action, &QAction::triggered, lineEdit, &QLineEdit::undo);

		action = menu->addAction(ALIcon::AweSomeIcon::ArrowRotateRight, tr("Redo"), QKeySequence::Redo);
		action->setEnabled(lineEdit->isRedoAvailable());
		connect(action, &QAction::triggered, lineEdit, &QLineEdit::redo);
		menu->addSeparator();
	}

#ifndef QT_NO_CLIPBOARD
	if (!lineEdit->isReadOnly()) {
		action = menu->addAction(ALIcon::AweSomeIcon::KnifeKitchen, tr("Cut"), QKeySequence::Cut);
		action->setEnabled(!lineEdit->isReadOnly() && lineEdit->hasSelectedText() && lineEdit->echoMode() == QLineEdit::Normal);
		connect(action, &QAction::triggered, lineEdit, &QLineEdit::cut);
	}

	action = menu->addAction(ALIcon::AweSomeIcon::Copy, tr("Copy"), QKeySequence::Copy);
	action->setEnabled(lineEdit->hasSelectedText() && lineEdit->echoMode() == QLineEdit::Normal);
	connect(action, &QAction::triggered, lineEdit, &QLineEdit::copy);

	if (!lineEdit->isReadOnly()) {
		action = menu->addAction(ALIcon::AweSomeIcon::Paste, tr("Paste"), QKeySequence::Paste);
		action->setEnabled(!lineEdit->isReadOnly() && !QGuiApplication::clipboard()->text().isEmpty());
		connect(action, &QAction::triggered, lineEdit, &QLineEdit::paste);
	}
#endif
	if (!lineEdit->isReadOnly()) {
		action = menu->addAction(ALIcon::AweSomeIcon::DeleteLeft, tr("delete"));
		action->setEnabled(!lineEdit->isReadOnly() && !lineEdit->text().isEmpty() && lineEdit->hasSelectedText());
		connect(action, &QAction::triggered, this, [=]() {
			if (lineEdit->hasSelectedText()) {
				lineEdit->setText(lineEdit->text().remove(lineEdit->selectionStart(), lineEdit->selectionEnd() - lineEdit->selectionStart()));
			}
		});
	}
	if (!menu->isEmpty()) {
		menu->addSeparator();
	}
	action = menu->addAction(tr("select all"));
	action->setShortcut(QKeySequence::SelectAll);
	action->setEnabled(!lineEdit->text().isEmpty() && lineEdit->selectedText() != lineEdit->text());
	connect(action, &QAction::triggered, lineEdit, &QLineEdit::selectAll);
	return menu;
}

/**
 * \class CALDoubleSpinBox
 * @param parent pointer to the parent class
 */
CALDoubleSpinBox::CALDoubleSpinBox(QWidget* parent): QDoubleSpinBox(parent), d_ptr(new CALDoubleSpinBoxPrivate(this, this)) {
	Q_D(CALDoubleSpinBox);

	d->style = new CALSpinBoxStyle(style());
	setStyle(d->style);
	setFixedSize(120, 30);
	lineEdit()->setAlignment(Qt::AlignCenter);
	lineEdit()->setStyleSheet("background-color: transparent;");
	connect(ALTheme, &CALThemeManager::sigThemeModeChanged, this, [=](const ALThemeType::ThemeMode& mode) {
		QPalette palette;
		palette.setColor(QPalette::Base, Qt::transparent);
		palette.setColor(QPalette::Text, ALThemeColor(mode, ALThemeType::BasicText));
		lineEdit()->setPalette(palette);
	});
}

CALDoubleSpinBox::~CALDoubleSpinBox() = default;

void CALDoubleSpinBox::contextMenuEvent(QContextMenuEvent* event) {
	Q_D(CALDoubleSpinBox);

	CALMenu* menu = d->createStandardContextMenu();
	menu->addSeparator();
	const uint step = stepEnabled();
	QAction* up = menu->addAction(ALIcon::AweSomeIcon::Plus, tr("add"));
	up->setEnabled(step & StepUpEnabled);
	QAction* down = menu->addAction(ALIcon::AweSomeIcon::Minus, tr("subtract"));
	down->setEnabled(step & StepDownEnabled);
	menu->addSeparator();

	const QPoint pos = event->reason() == QContextMenuEvent::Mouse ? event->globalPos() : mapToGlobal(QPoint(event->pos().x(), 0)) + QPoint(width() / 2, height() / 2);
	const QAction* action = menu->exec(pos);
	SAFE_DELETE(menu)
	if (action) {
		if (action == up) {
			stepBy(1);
		} else if (action == down) {
			stepBy(-1);
		}
	}
	event->accept();
}

} // namespace AL
