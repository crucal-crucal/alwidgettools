#include "alapplication.hpp"

#include <QApplication>
#include <QDebug>
#include <QDir>
#include <QEvent>
#include <QFontDatabase>
#include <QResource>
#include <QScreen>
#include <QThread>
#include <QTranslator>
#include <QWidget>

#include "alapplication_p.hpp"
#include "almicabaseinitobject.hpp"
#include "althememanager.hpp"

/**
 * @brief \namespace AL
 */
namespace AL {
/**
 * @brief \class CALApplicationPrivate
 * Internal class for CALApplication
 * @param q pointer to the public class
 * @param parent pointer to the parent class
 */
CALApplicationPrivate::CALApplicationPrivate(AL::CALApplication* q, QObject* parent): QObject(parent), q_ptr(q) {
}

CALApplicationPrivate::~CALApplicationPrivate() = default;

void CALApplicationPrivate::slotThemeModeChanged(const ALThemeType::ThemeMode& mode) {
	themeMode = mode;
	updateAllMicaWidget();
}

bool CALApplicationPrivate::eventFilter(QObject* watched, QEvent* event) {
	switch (event->type()) {
		case QEvent::Show:
		case QEvent::Move:
		case QEvent::Resize: {
			if (const auto widget = qobject_cast<QWidget*>(watched); isEnableMica && widget) {
				updateMica(widget);
			}
			break;
		}
		case QEvent::Destroy: {
			if (const auto widget = qobject_cast<QWidget*>(watched)) {
				micaWidgetList.removeOne(widget);
			}
			break;
		}
		default: break;
	}

	return QObject::eventFilter(watched, event);
}

void CALApplicationPrivate::initMicaBaseImage(const QImage& img) {
	Q_Q(CALApplication);

	if (img.isNull()) {
		return;
	}

	const auto initThread = new QThread;
	const auto initObject = new CALMicaBaseInitObject(this);
	connect(initThread, &QThread::finished, initObject, &QObject::deleteLater);
	connect(initObject, &CALMicaBaseInitObject::sigInitFinished, initThread, [this, q, initThread]() {
		Q_EMIT q->sigIsEnableMicaChanged();
		updateAllMicaWidget();
		initThread->quit();
		initThread->wait();
		initThread->deleteLater();
	});
	initObject->moveToThread(initThread);
	initThread->start();
	connect(this, &CALApplicationPrivate::sigInitMicaBase, initObject, &CALMicaBaseInitObject::slotInitMicaBase);
	Q_EMIT sigInitMicaBase(img);
}

QRect CALApplicationPrivate::calculateWindowVirtualGeometry(const QWidget* widget) const {
	const QRect geometry = widget->geometry();
	qreal xImageRatio, yImageRatio;
	QRect relativaGeometry;
	if (const QScreen* currentScreen = QApplication::screenAt(geometry.topLeft()); QApplication::screens().count() > 1 && currentScreen) {
		const QRect screenGeometry = currentScreen->geometry();
		xImageRatio = static_cast<qreal>(lightBaseImage.width()) / screenGeometry.width();
		yImageRatio = static_cast<qreal>(lightBaseImage.height()) / screenGeometry.height();
		relativaGeometry = QRect((geometry.x() - screenGeometry.x()) * xImageRatio, (geometry.y() - screenGeometry.y()) * yImageRatio, geometry.width() * xImageRatio, geometry.height() * yImageRatio); // NOLINT
		return relativaGeometry;
	}
	const QRect primaryScreenGeometry = QApplication::primaryScreen()->availableGeometry();
	xImageRatio = static_cast<qreal>(lightBaseImage.width()) / primaryScreenGeometry.width();
	yImageRatio = static_cast<qreal>(lightBaseImage.height()) / primaryScreenGeometry.height();
	relativaGeometry = QRect((geometry.x() - primaryScreenGeometry.x()) * xImageRatio, (geometry.y() - primaryScreenGeometry.y()) * yImageRatio, geometry.width() * xImageRatio, geometry.height() * yImageRatio); // NOLINT
	return relativaGeometry;
}

void CALApplicationPrivate::updateMica(QWidget* widget, const bool isProcessEvent) const {
	if (!widget->isVisible()) {
		return;
	}

	QPalette palette = widget->palette();
	const QImage& baseImage = themeMode == ALThemeType::Light ? lightBaseImage : darkBaseImage;
	palette.setBrush(QPalette::Window, baseImage.copy(calculateWindowVirtualGeometry(widget)).scaled(widget->size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
	widget->setPalette(palette);
	if (isProcessEvent) {
		QApplication::processEvents();
	}
}

void CALApplicationPrivate::updateAllMicaWidget() {
	if (!isEnableMica) {
		return;
	}

	for (const auto& widget : micaWidgetList) {
		updateMica(widget, false);
	}
}

/**
 * @brief \class CALApplication
 * @return CALApplication instance
 */
CALApplication* CALApplication::instance() {
	return CALSingleton<CALApplication>::instance();
}

void CALApplication::initializeApplication() {
	Q_D(CALApplication);

	/// register resource
	const QString rccFileName =
#ifdef Q_OS_WIN
			"libalwidgettoolsresource.dll"
#else
		"libalwidgettoolsresource.so"
#endif
		;
	if (!QResource::registerResource(QApplication::applicationDirPath() + QDir::separator() + rccFileName)) {
		qWarning() << "Failed to register libalwidgettoolsresource";
	}
	/// add font
	const QStringList fonts = {
		":alwidgettools/CALAwesomeFont",
		":alwidgettools/CALFluentFont",
		":alwidgettools/CALSegoeFont",
		":alwidgettools/CALSegoenFont"
	};
	for (const auto& font : fonts) {
		if (-1 == QFontDatabase::addApplicationFont(font)) {
			qWarning() << "Failed to load font: " << font <<
				"\nplease check the resource file: " + rccFileName;
		}
	}
	/// load & install translation
	if (!d->translator->load(":alwidgettools/CALTranslate")) {
		qWarning() << "Failed to load CALTranslate, please check the resource file: " + rccFileName;
	}
	if (!QApplication::installTranslator(d->translator)) {
		qWarning() << "Failed to install translator";
	}

	QFont font = QApplication::font();
	font.setPixelSize(13);
	font.setFamily("Microsoft YaHei");
	font.setHintingPreference(QFont::PreferNoHinting);
	QApplication::setFont(font);
}

void CALApplication::syncMica(QWidget* widget, const bool isSync) {
	Q_D(CALApplication);

	if (!widget) {
		return;
	}

	if (isSync) {
		widget->installEventFilter(d);
		d->micaWidgetList.append(widget);
		if (d->isEnableMica) {
			d->updateMica(widget, false);
		}
	} else {
		widget->removeEventFilter(d);
		d->micaWidgetList.removeOne(widget);
	}
}

bool CALApplication::containsCursorToItem(const QWidget* item) {
	if (!item || !item->isVisible()) {
		return false;
	}

	const auto rect = QRectF(item->mapTo(item->window(), QPoint(0, 0)), item->size());
	return rect.contains(item->window()->mapFromGlobal(QCursor::pos()));
}

void CALApplication::setIsEnableMica(const bool enable) {
	Q_D(CALApplication);

	d->isEnableMica = enable;
	if (enable) {
		d->initMicaBaseImage(QImage(d->micaImagePath));
	} else {
		d->slotThemeModeChanged(d->themeMode);
		Q_EMIT sigIsEnableMicaChanged();
	}
}

bool CALApplication::getIsEnableMica() const {
	return d_func()->isEnableMica;
}

void CALApplication::setMicaImagePath(const QString& path) {
	Q_D(CALApplication);

	d->micaImagePath = path;
	d->initMicaBaseImage(QImage(path));
	Q_EMIT sigMicaImagePathChanged();
}

QString CALApplication::getMicaImagePath() const {
	return d_func()->micaImagePath;
}

CALApplication::CALApplication(QObject* parent): QObject(parent), d_ptr(new CALApplicationPrivate(this, this)) {
	Q_D(CALApplication);

	d->isEnableMica = false;
	d->micaImagePath = ":alwidgettools/CALImage-crucal";
	d->themeMode = ALTheme->getThemeMode();
	connect(ALTheme, &CALThemeManager::sigThemeModeChanged, d, &CALApplicationPrivate::slotThemeModeChanged);

	d->translator = new QTranslator(this);
}

CALApplication::~CALApplication() = default;
}
