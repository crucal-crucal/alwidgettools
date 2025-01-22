#include "alwidget.hpp"

#include <QPainter>
#include <QScreen>

#include "alapplication.hpp"
#include "althememanager.hpp"
#include "alwidget_p.hpp"

/**
 * @brief \namespace AL
 */
namespace AL {
/**
 * @brief \class CALWidgetPrivate
 * Internal class for CALWidget
 * @param q pointer to the public class
 * @param parent pointer to the parent class
 */
CALWidgetPrivate::CALWidgetPrivate(CALWidget* q, QObject* parent): QObject(parent), q_ptr(q) {
}

CALWidgetPrivate::~CALWidgetPrivate() = default;

/**
 * @brief \class CALWidget
 * @param parent pointer to the parent class
 */
CALWidget::CALWidget(QWidget* parent): QWidget(parent), d_ptr(new CALWidgetPrivate(this, this)) {
	Q_D(CALWidget);

	/// appBar
	d->appBar = new CALAppBar(this);
	d->appBar->setWindowButtonFlags(ALAppBarType::StayTopButtonHint | ALAppBarType::MinimizeButtonHint | ALAppBarType::MaximizeButtonHint | ALAppBarType::CloseButtonHint);
	connect(d->appBar, &CALAppBar::sigRouteBackButtonClicked, this, &CALWidget::sigRouteBackButtonClicked);
	connect(d->appBar, &CALAppBar::sigNavigationButtonClicked, this, &CALWidget::sigNavigationButtonClicked);
	connect(d->appBar, &CALAppBar::sigThemeChangeButtonClicked, this, &CALWidget::sigThemeChangeButtonClicked);
	connect(d->appBar, &CALAppBar::sigCloseButtonClicked, this, &CALWidget::sigCloseButtonClicked);
	/// theme
	d->themeMode = ALTheme->getThemeMode();
	connect(ALTheme, &CALThemeManager::sigThemeModeChanged, this, [=](const ALThemeType::ThemeMode& mode) { d->themeMode = mode; });
	/// mica
	d->isEanbleMica = alApp->getIsEnableMica();
	connect(alApp, &CALApplication::sigIsEnableMicaChanged, this, [=]() {
		d->isEanbleMica = alApp->getIsEnableMica();
		update();
	});
	alApp->syncMica(this);

	setObjectName("CALWidget");
	setWindowTitle("CALWidget");
	resize(500, 500);
}

AL_TAKEOVER_NATIVEEVENT_CPP(CALWidget, d_func()->appBar)

CALWidget::~CALWidget() = default;

void CALWidget::moveToCenter() {
	if (isMaximized() || isFullScreen()) {
		return;
	}
#if QT_VERSION >= QT_VERSION_CHECK(5, 14, 0)
	auto geometry = screen()->availableGeometry();
#else
	auto geometry = qApp->screenAt(this->geometry().center())->geometry();
#endif
	setGeometry((geometry.left() + geometry.right() - width()) / 2, (geometry.top() + geometry.bottom() - height()) / 2, width(), height());
}

void CALWidget::setWindowButtonFlag(const ALAppBarType::ButtonFlag& buttonFlag, const bool isEnable) {
	d_func()->appBar->setWindowButtonFlag(buttonFlag, isEnable);
}

void CALWidget::setWindowButtonFlags(const ALAppBarType::ButtonFlags& buttonFlags) {
	d_func()->appBar->setWindowButtonFlags(buttonFlags);
}

ALAppBarType::ButtonFlags CALWidget::getWindowButtonFlags() const {
	return d_func()->appBar->getWindowButtonFlags();
}

void CALWidget::setIsStayTop(const bool isStayTop) {
	d_func()->appBar->setIsStayTop(isStayTop);
	Q_EMIT sigIsStayTopChanged();
}

bool CALWidget::getIsStayTop() const {
	return d_func()->appBar->getIsStayTop();
}

void CALWidget::setIsFixedSize(const bool isFixedSize) {
	d_func()->appBar->setIsFixedSize(isFixedSize);
	Q_EMIT sigIsFixedSizeChanged();
}

bool CALWidget::getIsFixedSize() const {
	return d_func()->appBar->getIsFixedSize();
}

void CALWidget::setIsDefaultClosed(const bool isDefaultClosed) {
	d_func()->appBar->setIsDefaultClosed(isDefaultClosed);
	Q_EMIT sigIsDefaultClosedChanged();
}

bool CALWidget::getIsDefaultClosed() const {
	return d_func()->appBar->getIsDefaultClosed();
}

void CALWidget::setAppBarHeight(const int appBarHeight) {
	d_func()->appBar->setAppBarHeight(appBarHeight);
	Q_EMIT sigAppBarHeightChanged();
}

int CALWidget::getAppBarHeight() const {
	return d_func()->appBar->getAppBarHeight();
}

void CALWidget::setAppBarBackground(const QColor& color) {
	d_func()->appBar->setBackgroundColor(color);
	Q_EMIT sigAppBarBackgroundChanged();
}

QColor CALWidget::getAppBarBackground() const {
	return d_func()->appBar->getBackgroundColor();
}

void CALWidget::paintEvent(QPaintEvent* event) {
	Q_D(CALWidget);

	if (!d->isEanbleMica) {
		QPainter painter(this);
		painter.save();
		painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);
		painter.setPen(Qt::NoPen);
		painter.setBrush(ALThemeColor(d->themeMode, ALThemeType::WindowBase));
		painter.drawRect(rect());
		painter.restore();
	}

	QWidget::paintEvent(event);
}
}
