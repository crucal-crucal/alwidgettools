#include "e_plaintextedit_example.hpp"

#include <QDebug>
#include <QHBoxLayout>

#include "alplaintextedit.hpp"

using namespace AL;

E_PlainTextEdit_Example::E_PlainTextEdit_Example(QWidget* parent): E_BasePage(parent) {
	setWindowTitle("CALPlainTextEdit");
	createCustomWidget("some thing...");

	const auto centralWidget = new QWidget(this);
	centralWidget->setWindowTitle(this->windowTitle());
	mainVLayout = new QVBoxLayout(centralWidget);
	mainVLayout->setContentsMargins(0, 0, 0, 0);
	mainVLayout->setSpacing(5);

	const auto plainTextEdit = new CALPlainTextEdit(this);
	plainTextEdit->setPlainText("this is test!");

	mainVLayout->addWidget(plainTextEdit);

	mainVLayout->addStretch();
	addCentralWidget(centralWidget, true, true, 0);
}

E_PlainTextEdit_Example::~E_PlainTextEdit_Example() = default;
