#include "e_about.hpp"

#include <QIcon>

E_About::E_About(QWidget* parent): CALWidget(parent) {
	setWindowTitle("about...");
	setWindowIcon(QIcon(":/image/crucal.png"));
	setWindowModality(Qt::ApplicationModal);
	setIsStayTop(true);
	setWindowButtonFlags(ALAppBarType::CloseButtonHint);
}

E_About::~E_About() = default;
