#include "uvapplication.hpp"

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

#include "uvapplication_p.hpp"
#include "uvmicabaseinitobject.hpp"
#include "uvthememanager.hpp"

/**
 * @brief \class CUVApplicationPrivate
 * Internal class for CUVApplication
 * @param q pointer to the public class
 * @param parent pointer to the parent class
 */
CUVApplicationPrivate::CUVApplicationPrivate(CUVApplication* q, QObject* parent): QObject(parent), q_ptr(q) {
}

CUVApplicationPrivate::~CUVApplicationPrivate() = default;

void CUVApplicationPrivate::slotThemeModeChanged(const UVThemeType::ThemeMode& mode) {
	themeMode = mode;
	updateAllMicaWidget();
}

bool CUVApplicationPrivate::eventFilter(QObject* watched, QEvent* event) {
	switch (event->type()) {
		case QEvent::Show:
		case QEvent::Move:
		case QEvent::Resize: {
			if (isEnableMica) {
				if (const auto widget = qobject_cast<QWidget*>(watched)) {
					updateMica(widget);
				}
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

void CUVApplicationPrivate::initMicaBaseImage(const QImage& img) {
	Q_Q(CUVApplication);

	if (img.isNull()) {
		return;
	}

	const auto initThread = new QThread;
	const auto initObject = new CUVMicaBaseInitObject(this);
	connect(initThread, &QThread::finished, initObject, &QObject::deleteLater);
	connect(initObject, &CUVMicaBaseInitObject::sigInitFinished, initThread, [=]() {
		Q_EMIT q->sigIsEnableMicaChanged();
		updateAllMicaWidget();
		initThread->quit();
		initThread->wait();
		initThread->deleteLater();
	});
	initObject->moveToThread(initThread);
	initThread->start();
	connect(this, &CUVApplicationPrivate::sigInitMicaBase, initObject, &CUVMicaBaseInitObject::slotInitMicaBase);
	Q_EMIT sigInitMicaBase(img);
}

QRect CUVApplicationPrivate::calculateWindowVirtualGeometry(const QWidget* widget) const {
	const QRect geometry = widget->geometry();
	qreal xImageRatio, yImageRatio;
	QRect relativaGeometry;
	if (QApplication::screens().count() > 1) {
		if (const QScreen* currentScreen = QApplication::screenAt(geometry.topLeft())) {
			const QRect screenGeometry = currentScreen->geometry();
			xImageRatio = static_cast<qreal>(lightBaseImage.width()) / screenGeometry.width();
			yImageRatio = static_cast<qreal>(lightBaseImage.height()) / screenGeometry.height();
			relativaGeometry = QRect((geometry.x() - screenGeometry.x()) * xImageRatio, (geometry.y() - screenGeometry.y()) * yImageRatio, geometry.width() * xImageRatio, geometry.height() * yImageRatio); // NOLINT
			return relativaGeometry;
		}
	}
	const QRect primaryScreenGeometry = QApplication::primaryScreen()->availableGeometry();
	xImageRatio = static_cast<qreal>(lightBaseImage.width()) / primaryScreenGeometry.width();
	yImageRatio = static_cast<qreal>(lightBaseImage.height()) / primaryScreenGeometry.height();
	relativaGeometry = QRect((geometry.x() - primaryScreenGeometry.x()) * xImageRatio, (geometry.y() - primaryScreenGeometry.y()) * yImageRatio, geometry.width() * xImageRatio, geometry.height() * yImageRatio); // NOLINT
	return relativaGeometry;
}

void CUVApplicationPrivate::updateMica(QWidget* widget, const bool isProcessEvent) const {
	if (widget->isVisible()) {
		QPalette palette = widget->palette();
		const QImage& baseImage = themeMode == UVThemeType::Light ? lightBaseImage : darkBaseImage;
		palette.setBrush(QPalette::Window, baseImage.copy(calculateWindowVirtualGeometry(widget)).scaled(widget->size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
		widget->setPalette(palette);
		if (isProcessEvent) {
			QApplication::processEvents();
		}
	}
}

void CUVApplicationPrivate::updateAllMicaWidget() {
	if (isEnableMica) {
		for (const auto& widget : micaWidgetList) {
			updateMica(widget, false);
		}
	}
}

/**
 * @brief \class CUVApplication
 * @return CUVApplication instance
 */
CUVApplication* CUVApplication::instance() {
	return CUVSingleton<CUVApplication>::instance();
}

void CUVApplication::init() {
	Q_D(CUVApplication);

	const QString applicationDirPath = QApplication::applicationDirPath();
	QResource::registerResource(applicationDirPath + QDir::separator() + "uvresource.rcc");
	QApplication::setAttribute(Qt::AA_DontCreateNativeWidgetSiblings);
	QFontDatabase::addApplicationFont(":/font/CUVAwesome.ttf");
	d->translator->load(":/translation/zh_CN.qm");
	QApplication::installTranslator(d->translator);

	QFont font = QApplication::font();
	font.setPixelSize(13);
	font.setFamily("Microsoft YaHei");
	font.setHintingPreference(QFont::PreferNoHinting);
	QApplication::setFont(font);
}

void CUVApplication::syncMica(QWidget* widget, const bool isSync) {
	Q_D(CUVApplication);

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

bool CUVApplication::containsCursorToItem(const QWidget* item) {
	if (!item || !item->isVisible()) {
		return false;
	}

	const auto point = item->window()->mapFromGlobal(QCursor::pos());
	const auto rect = QRectF(item->mapTo(item->window(), QPoint(0, 0)), item->size());
	return rect.contains(point);
}

void CUVApplication::setIsEnableMica(const bool enable) {
	Q_D(CUVApplication);

	d->isEnableMica = enable;
	if (enable) {
		d->initMicaBaseImage(QImage(d->micaImagePath));
	} else {
		d->slotThemeModeChanged(d->themeMode);
		Q_EMIT sigIsEnableMicaChanged();
	}
}

bool CUVApplication::getIsEnableMica() const {
	return d_func()->isEnableMica;
}

void CUVApplication::setMicaImagePath(const QString& path) {
	Q_D(CUVApplication);

	d->micaImagePath = path;
	d->initMicaBaseImage(QImage(path));
	Q_EMIT sigMicaImagePathChanged();
}

QString CUVApplication::getMicaImagePath() const {
	return d_func()->micaImagePath;
}

CUVApplication::CUVApplication(QObject* parent): QObject(parent), d_ptr(new CUVApplicationPrivate(this, this)) {
	Q_D(CUVApplication);

	d->isEnableMica = false;
	d->micaImagePath = ":include/Image/MicaBase.png";
	d->themeMode = UVTheme->getThemeMode();
	connect(UVTheme, &CUVThemeManager::sigThemeModeChanged, d, &CUVApplicationPrivate::slotThemeModeChanged);

	d->translator = new QTranslator(this);
}

CUVApplication::~CUVApplication() = default;
