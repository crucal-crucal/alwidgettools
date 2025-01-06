#include "uvspinbox.hpp"

#include <QClipboard>
#include <QContextMenuEvent>
#include <QGuiApplication>
#include <QLineEdit>

#include "uvmenu.hpp"
#include "uvspinboxstyle.hpp"
#include "uvspinbox_p.hpp"
#include "uvthememanager.hpp"

/**
 * \class CUVSpinBoxPrivate
 * Internal class for CUVSpinBox
 * @param q pointer to the public class
 * @param parent pointer to the parent class
 */
CUVSpinBoxPrivate::CUVSpinBoxPrivate(CUVSpinBox* q, QObject* parent): QObject(parent), q_ptr(q) {
}

CUVSpinBoxPrivate::~CUVSpinBoxPrivate() = default;

CUVMenu* CUVSpinBoxPrivate::createStandardContextMenu() {
	Q_Q(CUVSpinBox);

	QLineEdit* lineEdit = q->lineEdit();
	const auto menu = new CUVMenu(q);
	menu->setMenuItemHeight(27);
	menu->setAttribute(Qt::WA_DeleteOnClose);
	QAction* action{ nullptr };
	if (!lineEdit->isReadOnly()) {
		action = menu->addAction(UVIcon::CUVAweSomeIcon::ArrowRotateLeft, tr("Undo"), QKeySequence::Undo);
		action->setEnabled(lineEdit->isUndoAvailable());
		connect(action, &QAction::triggered, lineEdit, &QLineEdit::undo);

		action = menu->addAction(UVIcon::CUVAweSomeIcon::ArrowRotateRight, tr("Redo"), QKeySequence::Redo);
		action->setEnabled(lineEdit->isRedoAvailable());
		connect(action, &QAction::triggered, lineEdit, &QLineEdit::redo);
		menu->addSeparator();
	}

#ifndef QT_NO_CLIPBOARD
	if (!lineEdit->isReadOnly()) {
		action = menu->addAction(UVIcon::CUVAweSomeIcon::KnifeKitchen, tr("Cut"), QKeySequence::Cut);
		action->setEnabled(!lineEdit->isReadOnly() && lineEdit->hasSelectedText() && lineEdit->echoMode() == QLineEdit::Normal);
		connect(action, &QAction::triggered, lineEdit, &QLineEdit::cut);
	}

	action = menu->addAction(UVIcon::CUVAweSomeIcon::Copy, tr("Copy"), QKeySequence::Copy);
	action->setEnabled(lineEdit->hasSelectedText() && lineEdit->echoMode() == QLineEdit::Normal);
	connect(action, &QAction::triggered, lineEdit, &QLineEdit::copy);

	if (!lineEdit->isReadOnly()) {
		action = menu->addAction(UVIcon::CUVAweSomeIcon::Paste, tr("Paste"), QKeySequence::Paste);
		action->setEnabled(!lineEdit->isReadOnly() && !QGuiApplication::clipboard()->text().isEmpty());
		connect(action, &QAction::triggered, lineEdit, &QLineEdit::paste);
	}
#endif
	if (!lineEdit->isReadOnly()) {
		action = menu->addAction(UVIcon::CUVAweSomeIcon::DeleteLeft, tr("delete"));
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
 * \class CUVSpinBox
 * @param parent pointer to the parent class
 */
CUVSpinBox::CUVSpinBox(QWidget* parent): QSpinBox(parent), d_ptr(new CUVSpinBoxPrivate(this, this)) {
	Q_D(CUVSpinBox);

	d->style = new CUVSpinBoxStyle(style());
	setStyle(d->style);
	setFixedSize(120, 30);
	lineEdit()->setAlignment(Qt::AlignCenter);
	lineEdit()->setStyleSheet("background-color: transparent;");
	connect(UVTheme, &CUVThemeManager::sigThemeModeChanged, this, [=](const UVThemeType::ThemeMode& mode) {
		QPalette palette;
		palette.setColor(QPalette::Base, Qt::transparent);
		palette.setColor(QPalette::Text, UVThemeColor(mode, UVThemeType::BasicText));
		lineEdit()->setPalette(palette);
	});
}

CUVSpinBox::~CUVSpinBox() = default;

void CUVSpinBox::contextMenuEvent(QContextMenuEvent* event) {
	Q_D(CUVSpinBox);

	CUVMenu* menu = d->createStandardContextMenu();
	menu->addSeparator();
	const uint step = stepEnabled();
	QAction* up = menu->addAction(UVIcon::CUVAweSomeIcon::Plus, tr("add"));
	up->setEnabled(step & StepUpEnabled);
	QAction* down = menu->addAction(UVIcon::CUVAweSomeIcon::Minus, tr("subtract"));
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
