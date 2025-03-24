#include <QApplication>
#include <QDebug>
#include <QDir>
#include <QResource>
#include <QTranslator>

#include "mainwindow.hpp"
#include "alapplication.hpp"
#include "allog.hpp"

int main(int argc, char* argv[]) {
	/// enable high dpi
#if (QT_VERSION < QT_VERSION_CHECK(6, 0, 0))
	QGuiApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);
#if (QT_VERSION >= QT_VERSION_CHECK(5, 14, 0))
	QGuiApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
	QGuiApplication::setHighDpiScaleFactorRoundingPolicy(Qt::HighDpiScaleFactorRoundingPolicy::PassThrough);
#else
	qputenv("QT_SCALE_FACTOR", "1.5");
#endif
#endif

	QApplication app(argc, argv);
	QApplication::setAttribute(Qt::AA_DontCreateNativeWidgetSiblings);
	alApp->initializeApplication();
	/// register rcc
	const QString applicationDirPath = QApplication::applicationDirPath();
	QResource::registerResource(applicationDirPath + QDir::separator() + "example.rcc");
	/// load translation
	const auto exampleTranslator = new QTranslator;
	exampleTranslator->load(":example/translation/zh_CN.qm");
	QApplication::installTranslator(exampleTranslator);
	/// mica Image
	alApp->setMicaImagePath(":example/control/mica.jpg");

	MainWindow widget;
	QObject::connect(&widget, &MainWindow::destroyed, [exampleTranslator]() {
		if (exampleTranslator) {
			exampleTranslator->deleteLater();
		}
	});

	widget.show();

	return QApplication::exec();
}
