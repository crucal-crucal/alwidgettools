#include "aldoublespinbox.hpp"

#include <QClipboard>
#include <QContextMenuEvent>
#include <QGuiApplication>
#include <QLineEdit>

#include "uvmenu.hpp"
#include "uvspinboxstyle.hpp"
#include "uvdoublespinbox_p.hpp"
#include "althememanager.hpp"

/**
 * \class CUVDoubleSpinBoxPrivate
 * Internal class for CUVDoubleSpinBox
 * @param q pointer to the public class
 * @param parent pointer to the parent class
 */
CUVDoubleSpinBoxPrivate::CUVDoubleSpinBoxPrivate(CUVDoubleSpinBox* q, QObject* parent): QObject(parent), q_ptr(q) {
}

CUVDoubleSpinBoxPrivate::~CUVDoubleSpinBoxPrivate() = default;

CUVMenu* CUVDoubleSpinBoxPrivate::createStandardContextMenu() {
	Q_Q(CUVDoubleSpinBox);

	QLineEdit* lineEdit = q->lineEdit();
	const auto menu = new CUVMenu(q);
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
 * \class CUVDoubleSpinBox
 * @param parent pointer to the parent class
 */
CUVDoubleSpinBox::CUVDoubleSpinBox(QWidget* parent): QDoubleSpinBox(parent), d_ptr(new CUVDoubleSpinBoxPrivate(this, this)) {
	Q_D(CUVDoubleSpinBox);

	d->style = new CUVSpinBoxStyle(style());
	setStyle(d->style);
	setFixedSize(120, 30);
	lineEdit()->setAlignment(Qt::AlignCenter);
	lineEdit()->setStyleSheet("background-color: transparent;");
	connect(UVTheme, &CUVThemeManager::sigThemeModeChanged, this, [=](const ALThemeType::ThemeMode& mode) {
		QPalette palette;
		palette.setColor(QPalette::Base, Qt::transparent);
		palette.setColor(QPalette::Text, UVThemeColor(mode, ALThemeType::BasicText));
		lineEdit()->setPalette(palette);
	});
}

CUVDoubleSpinBox::~CUVDoubleSpinBox() = default;

void CUVDoubleSpinBox::contextMenuEvent(QContextMenuEvent* event) {
	Q_D(CUVDoubleSpinBox);

	CUVMenu* menu = d->createStandardContextMenu();
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
