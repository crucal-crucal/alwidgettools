#include "alspinbox.hpp"

#include <QClipboard>
#include <QContextMenuEvent>
#include <QGuiApplication>
#include <QLineEdit>
#include <QTimer>

#include "alicon.hpp"
#include "almenu.hpp"
#include "alspinboxstyle.hpp"
#include "alspinbox_p.hpp"
#include "althememanager.hpp"

/**
 * @brief \namespace AL
 */
namespace AL {
/**
 * \class CALSpinBoxPrivate
 * Internal class for CALSpinBox
 * @param q pointer to the public class
 * @param parent pointer to the parent class
 */
CALSpinBoxPrivate::CALSpinBoxPrivate(CALSpinBox* q, QObject* parent): QObject(parent), q_ptr(q) {
}

CALSpinBoxPrivate::~CALSpinBoxPrivate() = default;

CALMenu* CALSpinBoxPrivate::createStandardContextMenu() {
	Q_Q(CALSpinBox);

	QLineEdit* lineEdit = q->lineEdit();
	const auto menu = new CALMenu(q);
	menu->setMenuItemHeight(27);
	menu->setAttribute(Qt::WA_DeleteOnClose);
	QAction* action;
	if (!lineEdit->isReadOnly()) {
		action = menu->addAction(CALIconFactory::createIconType(ALIcon::AweSomeIcon::ArrowRotateLeft), tr("Undo"), QKeySequence::Undo);
		action->setEnabled(lineEdit->isUndoAvailable());
		connect(action, &QAction::triggered, lineEdit, &QLineEdit::undo);

		action = menu->addAction(CALIconFactory::createIconType(ALIcon::AweSomeIcon::ArrowRotateRight), tr("Redo"), QKeySequence::Redo);
		action->setEnabled(lineEdit->isRedoAvailable());
		connect(action, &QAction::triggered, lineEdit, &QLineEdit::redo);
		menu->addSeparator();
	}

#ifndef QT_NO_CLIPBOARD
	if (!lineEdit->isReadOnly()) {
		action = menu->addAction(CALIconFactory::createIconType(ALIcon::AweSomeIcon::KnifeKitchen), tr("Cut"), QKeySequence::Cut);
		action->setEnabled(!lineEdit->isReadOnly() && lineEdit->hasSelectedText() && lineEdit->echoMode() == QLineEdit::Normal);
		connect(action, &QAction::triggered, lineEdit, &QLineEdit::cut);
	}

	action = menu->addAction(CALIconFactory::createIconType(ALIcon::AweSomeIcon::Copy), tr("Copy"), QKeySequence::Copy);
	action->setEnabled(lineEdit->hasSelectedText() && lineEdit->echoMode() == QLineEdit::Normal);
	connect(action, &QAction::triggered, lineEdit, &QLineEdit::copy);

	if (!lineEdit->isReadOnly()) {
		action = menu->addAction(CALIconFactory::createIconType(ALIcon::AweSomeIcon::Paste), tr("Paste"), QKeySequence::Paste);
		action->setEnabled(!lineEdit->isReadOnly() && !QGuiApplication::clipboard()->text().isEmpty());
		connect(action, &QAction::triggered, lineEdit, &QLineEdit::paste);
	}
#endif
	if (!lineEdit->isReadOnly()) {
		action = menu->addAction(CALIconFactory::createIconType(ALIcon::AweSomeIcon::DeleteLeft), tr("delete"));
		action->setEnabled(!lineEdit->isReadOnly() && !lineEdit->text().isEmpty() && lineEdit->hasSelectedText());
		connect(action, &QAction::triggered, this, [lineEdit]() {
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

void CALSpinBoxPrivate::changeTheme() {
	Q_Q(CALSpinBox);

	QPalette palette;
	palette.setColor(QPalette::Base, Qt::transparent);
	palette.setColor(QPalette::Text, ALThemeColor(themeMode, ALThemeType::BasicText));
	q->lineEdit()->setPalette(palette);
}

/**
 * \class CALSpinBox
 * @param parent pointer to the parent class
 */
CALSpinBox::CALSpinBox(QWidget* parent): QSpinBox(parent), d_ptr(new CALSpinBoxPrivate(this, this)) {
	Q_D(CALSpinBox);

	lineEdit()->setAlignment(Qt::AlignCenter);
	lineEdit()->setStyleSheet("background-color: transparent;");

	d->style = new CALSpinBoxStyle(style());
	setStyle(d->style);
	setFixedSize(120, 30);
	connect(ALTheme, &CALThemeManager::sigThemeModeChanged, this, [d, this](const ALThemeType::ThemeMode& mode) {
		d->themeMode = mode;
		if (isVisible()) {
			d->changeTheme();
		} else {
			QTimer::singleShot(1, this, [d] {
				d->changeTheme();
			});
		}
	});
}

CALSpinBox::~CALSpinBox() = default;

void CALSpinBox::contextMenuEvent(QContextMenuEvent* event) {
	Q_D(CALSpinBox);

	CALMenu* menu = d->createStandardContextMenu();
	menu->addSeparator();
	const uint step = stepEnabled();
	QAction* up = menu->addAction(CALIconFactory::createIconType(ALIcon::AweSomeIcon::Plus), tr("add"));
	up->setEnabled(step & StepUpEnabled);
	QAction* down = menu->addAction(CALIconFactory::createIconType(ALIcon::AweSomeIcon::Minus), tr("subtract"));
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
}
