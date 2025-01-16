#include "alappbar.hpp"

#ifdef Q_OS_WIN
#include <Windows.h>
#else
#include <QDateTime>
#include <QWindow>
#endif

#include <QApplication>
#include <QDebug>
#include <QMouseEvent>
#include <QPainter>
#include <QPropertyAnimation>
#include <QScreen>
#include <QVBoxLayout>

#include "alappbar_p.hpp"
#include "alapplication.hpp"
#include "altoolbutton.hpp"
#include "aleventbus.hpp"
#include "aliconbutton.hpp"
#include "alnavigationbar.hpp"
#include "altext.hpp"
#include "althememanager.hpp"
#include "alwinshadowhelper.hpp"

/**
 * @brief \namespace AL
 */
namespace AL {
/**
 * @brief \class CALAppBarPrivate
 * Internal class for CALAppBar
 * @param q pointer to the public class
 * @param parent pointer to the parent class
 */
CALAppBarPrivate::CALAppBarPrivate(CALAppBar* q, QObject* parent): QObject(parent), q_ptr(q) {
}

CALAppBarPrivate::~CALAppBarPrivate() = default;

void CALAppBarPrivate::slotMinButtonClicked() {
	q_func()->window()->showMinimized();
}

void CALAppBarPrivate::slotMaxButtonClicked() {
	Q_Q(CALAppBar);

	q->window()->isMaximized() ? q->window()->showNormal() : q->window()->showMaximized();
}

void CALAppBarPrivate::slotCloseButtonClicked() {
	Q_Q(CALAppBar);

	isDefaultClosed ? q->window()->close() : Q_EMIT q->sigCloseButtonClicked();
}

void CALAppBarPrivate::slotStayTopButtonClicked() const {
#ifdef Q_OS_WIN
	HWND hwnd = reinterpret_cast<HWND>(currentWinID); // NOLINT
	::SetWindowPos(hwnd, isStayTop ? HWND_TOPMOST : HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
#else
	Q_Q(const ElaAppBar);
	bool isVisible = q->window()->isVisible();
	q->window()->setWindowFlag(Qt::WindowStaysOnTopHint, _pIsStayTop);
	if (isVisible) {
		q->window()->show();
	}
#endif
	stayTopButton->setIsSelected(isStayTop);
	stayTopButton->update();
}

void CALAppBarPrivate::slotThemeModeChanged(const ALThemeType::ThemeMode& mode) const {
	themeChangeButton->setFluentIcon(mode == ALThemeType::Light ? ALIcon::FluentIcon::QuietHours : ALIcon::FluentIcon::Brightness);
	themeChangeButton->setToolTip(mode == ALThemeType::Light ? tr("Switch to dark theme") : tr("Switch to light theme"));
}

void CALAppBarPrivate::changeMaxButtonIcon(const bool isMaximized) const {
	maxButton->setFluentIcon(isMaximized ? ALIcon::FluentIcon::ChromeRestore : ALIcon::FluentIcon::ChromeMaximize);
}

void CALAppBarPrivate::showSystemMenu(const QPoint& point) {
	Q_Q(CALAppBar);

#ifdef Q_OS_WIN
	const QScreen* screen = QApplication::screenAt(QCursor::pos());
	if (!screen) {
		screen = QGuiApplication::primaryScreen();
	}

	if (!screen) {
		return;
	}

	const QPoint origin = screen->geometry().topLeft();
	const auto nativePos = QPointF(QPointF(point - origin) * screen->devicePixelRatio()).toPoint() + origin;
	auto hwnd = reinterpret_cast<HWND>(q->window()->winId()); // NOLINT
	HMENU hMenu = ::GetSystemMenu(hwnd, FALSE);               // NOLINT
	if (q->window()->isMaximized() || q->window()->isFullScreen()) {
		::EnableMenuItem(hMenu, SC_MOVE, MFS_DISABLED);
		::EnableMenuItem(hMenu, SC_RESTORE, MFS_ENABLED);
	} else {
		::EnableMenuItem(hMenu, SC_MOVE, MFS_ENABLED);
		::EnableMenuItem(hMenu, SC_RESTORE, MFS_DISABLED);
	}

	if (!isFixedSize && !q->window()->isMaximized() && !q->window()->isFullScreen()) {
		::EnableMenuItem(hMenu, SC_SIZE, MFS_ENABLED);
		::EnableMenuItem(hMenu, SC_MAXIMIZE, MFS_ENABLED);
	} else {
		::EnableMenuItem(hMenu, SC_SIZE, MFS_DISABLED);
		::EnableMenuItem(hMenu, SC_MAXIMIZE, MFS_DISABLED);
	}
	if (const int result = ::TrackPopupMenu(hMenu, (TPM_RETURNCMD | (QGuiApplication::isRightToLeft() ? TPM_RIGHTALIGN : TPM_LEFTALIGN)), nativePos.x(), nativePos.y(), 0, hwnd, nullptr); result != FALSE) {
		::PostMessageW(hwnd, WM_SYSCOMMAND, result, 0);
	}
#endif
}

void CALAppBarPrivate::updateCursor(const int edges) {
	Q_Q(CALAppBar);

	switch (edges) {
		case 0: {
			q->window()->setCursor(Qt::ArrowCursor);
			break;
		}
		case Qt::LeftEdge:
		case Qt::RightEdge: {
			q->window()->setCursor(Qt::SizeHorCursor);
			break;
		}
		case Qt::TopEdge:
		case Qt::BottomEdge: {
			q->window()->setCursor(Qt::SizeVerCursor);
			break;
		}
		case Qt::LeftEdge | Qt::TopEdge:
		case Qt::RightEdge | Qt::BottomEdge: {
			q->window()->setCursor(Qt::SizeFDiagCursor);
			break;
		}
		case Qt::RightEdge | Qt::TopEdge:
		case Qt::LeftEdge | Qt::BottomEdge: {
			q->window()->setCursor(Qt::SizeBDiagCursor);
			break;
		}
		default: break;
	}
}

bool CALAppBarPrivate::containsCursorToItem(QWidget* item) { // NOLINT
	Q_Q(CALAppBar);

	if (!item || !item->isVisible()) {
		return false;
	}

	const QPoint point = item->window()->mapFromGlobal(QCursor::pos());
	const QRectF rect = { item->mapTo(item->window(), QPoint(0, 0)), item->size() };
	if (item == q) {
		if (containsCursorToItem(routeBackButton) || containsCursorToItem(navigationButton) || containsCursorToItem(customWidget) || containsCursorToItem(stayTopButton)
			|| containsCursorToItem(themeChangeButton) || containsCursorToItem(minButton) || containsCursorToItem(maxButton) || containsCursorToItem(closeButton)) {
			return false;
		}
	}
	if (rect.contains(point)) {
		return true;
	}

	return false;
}

int CALAppBarPrivate::calculateMinimumWidth() {
	Q_Q(CALAppBar);
	int width = 0;
	if (titleLabel->isVisible()) {
		width += titleLabel->width();
		width += 10;
	}
	if (iconLabel->isVisible()) {
		width += iconLabel->width();
		width += 10;
	}
	bool isHasNavigationBar = false;
	if (q->parentWidget()->findChild<CALNavigationBar*>()) {
		isHasNavigationBar = true;
		width += 305;
	} else {
		width += 5;
	}
	if (customWidget) {
		const int customWidgetWidth = customWidget->width();
		if (isHasNavigationBar) {
			if (customWidgetWidth > 300) {
				width += customWidgetWidth - 300;
			}
		} else {
			width += customWidgetWidth;
		}
	}
	QList<QAbstractButton*> buttonList = q->findChildren<QAbstractButton*>();
	for (const auto button : buttonList) {
		if (button->isVisible() && button->objectName() != "CALNavigationButton") {
			width += button->width();
		}
	}
	return width;
}

QVBoxLayout* CALAppBarPrivate::createVLayout(QWidget* widget) const {
	if (!widget) {
		return nullptr;
	}
	const auto vLayout = new QVBoxLayout;
	vLayout->setContentsMargins(0, 0, 0, 0);
	vLayout->setSpacing(0);
	if (widget == iconLabel || widget == titleLabel) {
		vLayout->addSpacing(6);
	}
	vLayout->addWidget(widget);
	vLayout->setAlignment(Qt::AlignCenter);
	return vLayout;
}

QAbstractButton* CALAppBarPrivate::getButtonByAppBarFlag(const ALAppBarType::ButtonFlag& flag) const {
	return buttonMap.value(flag, nullptr);
}


/**
 * @brief \class CALAppBar
 * @param parent pointer to the parent class
 */
CALAppBar::CALAppBar(QWidget* parent): QWidget(parent), d_ptr(new CALAppBarPrivate(this, this)) {
	Q_ASSERT(parent);

	Q_D(CALAppBar);

	window()->setAttribute(Qt::WA_Mapped);
	d->appBarHeight = 45;
	setFixedHeight(d->appBarHeight);
	window()->setContentsMargins(0, this->height(), 0, 0);
	d->isStayTop = false;
	d->isFixedSize = false;
	d->isDefaultClosed = true;
	d->isOnlyAllowMinAndClose = false;
	d->customWidgetMaximumWidth = 550;

	window()->installEventFilter(this);
#ifdef Q_OS_WIN
#if (QT_VERSION >= QT_VERSION_CHECK(6, 5, 3) && QT_VERSION <= QT_VERSION_CHECK(6, 6, 1))
	window()->setWindowFlags((window()->windowFlags()) | Qt::WindowMinimizeButtonHint | Qt::FramelessWindowHint);
#endif
#else
	window()->setWindowFlags((window()->windowFlags()) | Qt::FramelessWindowHint | Qt::WindowMinMaxButtonsHint | Qt::WindowCloseButtonHint);
#endif
	setMouseTracking(true);
	setObjectName("CALAppBar");
	setStyleSheet("#CALAppBar { background-color: transparent; }");
	// 路由跳转
	d->routeBackButton = new CALToolButton(this);
	d->routeBackButton->setAweSomeIcon(ALIcon::AweSomeIcon::ArrowLeft);
	d->routeBackButton->setFixedSize(d->appBarHeight, d->appBarHeight);
	d->routeBackButton->setEnabled(false);
	d->routeBackButton->setToolTip(tr("Back"));
	connect(d->routeBackButton, &CALToolButton::clicked, this, &CALAppBar::sigRouteBackButtonClicked);

	// 导航栏展开
	d->navigationButton = new CALToolButton(this);
	d->navigationButton->setAweSomeIcon(ALIcon::AweSomeIcon::Bars);
	d->navigationButton->setFixedSize(40, 30);
	d->navigationButton->setObjectName("CALNavigationButton");
	d->navigationButton->setToolTip(tr("Navigation"));
	connect(d->navigationButton, &CALToolButton::clicked, this, &CALAppBar::sigNavigationButtonClicked);

	// 置顶
	d->stayTopButton = new CALToolButton(this);
	d->stayTopButton->setAweSomeIcon(ALIcon::AweSomeIcon::ArrowUpToArc);
	d->stayTopButton->setFixedSize(d->appBarHeight, d->appBarHeight);
	d->stayTopButton->setToolTip(tr("Stay Top"));
	d->buttonMap[ALAppBarType::StayTopButtonHint] = d->stayTopButton;
	connect(d->stayTopButton, &CALToolButton::clicked, this, [=]() { this->setIsStayTop(!this->getIsStayTop()); });
	connect(d->stayTopButton, &CALToolButton::sigSelectedChanged, this, [=]() { d->stayTopButton->setToolTip(d->stayTopButton->getIsSelected() ? tr("Cancel Stay Top") : tr("Stay Top")); });
	connect(this, &CALAppBar::sigIsStayTopChanged, d, &CALAppBarPrivate::slotStayTopButtonClicked);

	// 图标
	d->iconLabel = new QLabel(this);
	d->iconLabelVLayout = d->createVLayout(d->iconLabel);
	d->iconLabel->setPixmap(parent->windowIcon().pixmap(18, 18));
	d->iconLabel->setVisible(parent->windowIcon().isNull() ? false : true);
	d->iconLabelVLayout->setContentsMargins(parent->windowIcon().isNull() ? 0 : 10, 0, 0, 0);
	connect(parent, &QWidget::windowIconChanged, this, [=](const QIcon& icon) {
		d->iconLabel->setPixmap(icon.pixmap(18, 18));
		d->iconLabel->setVisible(icon.isNull() ? false : true);
		d->iconLabelVLayout->setContentsMargins(icon.isNull() ? 0 : 10, 0, 0, 0);
	});

	// 标题
	d->titleLabel = new CALText(this);
	d->titleLabel->setIsWrapAnyWhere(false);
	d->titleLabel->setTextPixelSize(13);
	d->titleLabelVLayout = d->createVLayout(d->titleLabel);
	d->titleLabel->setText(parent->windowTitle());
	d->titleLabel->setVisible(parent->windowTitle().isEmpty() ? false : true);
	d->titleLabelVLayout->setContentsMargins(parent->windowTitle().isEmpty() ? 0 : 10, 0, 0, 0);
	connect(parent, &QWidget::windowTitleChanged, this, [=](const QString& title) {
		d->titleLabel->setText(title);
		d->titleLabel->setVisible(title.isEmpty() ? false : true);
		d->titleLabelVLayout->setContentsMargins(title.isEmpty() ? 0 : 10, 0, 0, 0);
	});

	// 主题
	d->themeChangeButton = new CALToolButton(this);
	d->themeChangeButton->setFluentIcon(ALIcon::FluentIcon::QuietHours);
	d->themeChangeButton->setFixedSize(d->appBarHeight, d->appBarHeight);
	d->themeChangeButton->setToolTip(ALTheme->getThemeMode() == ALThemeType::Light ? tr("Switch to dark theme") : tr("Switch to light theme"));
	d->buttonMap[ALAppBarType::ThemeChangeButtonHint] = d->themeChangeButton;
	connect(d->themeChangeButton, &CALToolButton::clicked, this, &CALAppBar::sigThemeChangeButtonClicked);
	connect(ALTheme, &CALThemeManager::sigThemeModeChanged, this, [=](const ALThemeType::ThemeMode& mode) { d->slotThemeModeChanged(mode); });

	// 最小化
	d->minButton = new CALToolButton(this);
	d->minButton->setFluentIcon(ALIcon::FluentIcon::ChromeMinimize);
	d->minButton->setFixedSize(d->appBarHeight, d->appBarHeight);
	d->minButton->setToolTip(tr("Minimize"));
	d->buttonMap[ALAppBarType::MinimizeButtonHint] = d->minButton;
	connect(d->minButton, &CALToolButton::clicked, d, &CALAppBarPrivate::slotMinButtonClicked);

	// 最大化 & 还原
	d->maxButton = new CALToolButton(this);
	d->maxButton->setIconSize({ 18, 18 });
	d->maxButton->setFluentIcon(ALIcon::FluentIcon::ChromeMaximize);
	d->maxButton->setFixedSize(d->appBarHeight, d->appBarHeight);
	d->buttonMap[ALAppBarType::MaximizeButtonHint] = d->maxButton;
	connect(d->maxButton, &CALToolButton::clicked, d, &CALAppBarPrivate::slotMaxButtonClicked);

	// 关闭
	d->closeButton = new CALIconButton(ALIcon::FluentIcon::ChromeClose, 14, d->appBarHeight, d->appBarHeight, this);
	d->closeButton->setLightHoverColor(QColor(0xE8, 0x11, 0x23));
	d->closeButton->setDarkHoverColor(QColor(0xE8, 0x11, 0x23));
	d->closeButton->setLightHoverIconColor(Qt::white);
	d->closeButton->setDarkHoverIconColor(Qt::white);
	d->closeButton->setToolTip(tr("Close"));
	d->buttonMap[ALAppBarType::CloseButtonHint] = d->closeButton;
	connect(d->closeButton, &CALIconButton::clicked, d, &CALAppBarPrivate::slotCloseButtonClicked);

	d->mainHLayout = new QHBoxLayout(this);
	d->mainHLayout->setContentsMargins(0, 0, 0, 0);
	d->mainHLayout->setSpacing(0);
	d->mainHLayout->addLayout(d->createVLayout(d->routeBackButton));
	d->mainHLayout->addLayout(d->createVLayout(d->navigationButton));
	d->mainHLayout->addLayout(d->iconLabelVLayout);
	d->mainHLayout->addLayout(d->titleLabelVLayout);
	d->mainHLayout->addStretch();
	d->mainHLayout->addStretch();
	d->mainHLayout->addWidget(d->stayTopButton);
	d->mainHLayout->addWidget(d->themeChangeButton);
	d->mainHLayout->addWidget(d->minButton);
	d->mainHLayout->addWidget(d->maxButton);
	d->mainHLayout->addWidget(d->closeButton);
	d->mainHLayout->setAlignment(Qt::AlignHCenter);

#ifdef Q_OS_WIN
	for (int i = 0; i < QApplication::screens().count(); ++i) {
		connect(QApplication::screens().at(i), &QScreen::logicalDotsPerInchChanged, this, [=] {
			if (d->isFixedSize) {
				auto hwnd = reinterpret_cast<HWND>(d->currentWinID); // NOLINT
				SetWindowPos(hwnd, nullptr, 0, 0, 0, 0, SWP_NOZORDER | SWP_NOOWNERZORDER | SWP_NOMOVE | SWP_FRAMECHANGED);
			}
		});
	}
	//主屏幕变更处理
	connect(dynamic_cast<QApplication*>(QCoreApplication::instance()), &QApplication::primaryScreenChanged, this, [=]() {
		auto hwnd = reinterpret_cast<HWND>(d->currentWinID); // NOLINT
		::SetWindowPos(hwnd, nullptr, 0, 0, 0, 0, SWP_NOZORDER | SWP_NOOWNERZORDER | SWP_NOMOVE | SWP_NOSIZE | SWP_FRAMECHANGED);
		::RedrawWindow(hwnd, nullptr, nullptr, RDW_INVALIDATE | RDW_UPDATENOW);
	});
	d->lastScreen = QApplication::screenAt(window()->geometry().center());
#endif
	setWindowButtonFlags(ALAppBarType::RouteBackButtonHint | ALAppBarType::StayTopButtonHint | ALAppBarType::ThemeChangeButtonHint | ALAppBarType::MinimizeButtonHint | ALAppBarType::MaximizeButtonHint | ALAppBarType::CloseButtonHint);
}

CALAppBar::~CALAppBar() = default;

void CALAppBar::setIsStayTop(const bool isStayTop) {
	d_func()->isStayTop = isStayTop;
	Q_EMIT sigIsStayTopChanged();
}

bool CALAppBar::getIsStayTop() const {
	return d_func()->isStayTop;
}

void CALAppBar::setIsFixedSize(const bool isFixedSize) {
	Q_D(CALAppBar);

	d->isFixedSize = isFixedSize;
#ifdef Q_OS_WIN
	auto hwnd = reinterpret_cast<HWND>(d->currentWinID); // NOLINT
	DWORD style = ::GetWindowLongPtr(hwnd, GWL_STYLE);
	if (d->isFixedSize) {
		// 切换DPI处理
		style &= ~WS_THICKFRAME;
		::SetWindowLongPtr(hwnd, GWL_STYLE, style);
	} else {
		::SetWindowLongPtr(hwnd, GWL_STYLE, style | WS_MAXIMIZEBOX | WS_THICKFRAME);
	}
#else
	bool isVisible = window()->isVisible();
	window()->setWindowFlags((window()->windowFlags()) | Qt::FramelessWindowHint | Qt::WindowMinimizeButtonHint | Qt::WindowCloseButtonHint);
	if (!isFixedSize) {
		window()->setWindowFlag(Qt::WindowMaximizeButtonHint);
	}
	if (isVisible) {
		window()->show();
	}
#endif
	Q_EMIT sigIsFixedSizeChanged();
}

bool CALAppBar::getIsFixedSize() const {
	return d_func()->isFixedSize;
}

void CALAppBar::setIsDefaultClosed(const bool isDefaultClosed) {
	d_func()->isDefaultClosed = isDefaultClosed;
	Q_EMIT sigIsDefaultClosedChanged();
}

bool CALAppBar::getIsDefaultClosed() const {
	return d_func()->isDefaultClosed;
}

void CALAppBar::setIsOnlyAllowMinAndClose(const bool isOnlyAllowMinAndClose) {
	d_func()->isOnlyAllowMinAndClose = isOnlyAllowMinAndClose;
	Q_EMIT sigIsOnlyAllowMinAndCloseChanged();
}

bool CALAppBar::getIsOnlyAllowMinAndClose() const {
	return d_func()->isOnlyAllowMinAndClose;
}

void CALAppBar::setAppBarHeight(const int appBarHeight) {
	Q_D(CALAppBar);

	d->appBarHeight = appBarHeight;
	d->routeBackButton->setFixedSize(appBarHeight, appBarHeight);
	d->themeChangeButton->setFixedSize(appBarHeight, appBarHeight);
	d->stayTopButton->setFixedSize(appBarHeight, appBarHeight);
	d->minButton->setFixedSize(appBarHeight, appBarHeight);
	d->maxButton->setFixedSize(appBarHeight, appBarHeight);
	d->closeButton->setFixedSize(appBarHeight, appBarHeight);
	setFixedHeight(appBarHeight);
	window()->setContentsMargins(0, this->height(), 0, 0);

	Q_EMIT sigAppBarHeightChanged();
}

int CALAppBar::getAppBarHeight() const {
	return d_func()->appBarHeight;
}

void CALAppBar::setCustomWidgetMaximumWidth(const int customWidgetMaximumWidth) {
	Q_D(CALAppBar);

	d->customWidgetMaximumWidth = customWidgetMaximumWidth;
	if (d->customWidget) {
		d->customWidget->setMaximumWidth(customWidgetMaximumWidth);
	}
	Q_EMIT sigCustomWidgetMaximumWidthChanged();
}

int CALAppBar::getCustomWidgetMaximumWidth() const {
	return d_func()->customWidgetMaximumWidth;
}

void CALAppBar::setBackgroundColor(const QColor& color) {
	d_func()->backgroundColor = color;
	update();
}

QColor CALAppBar::getBackgroundColor() const {
	Q_D(const CALAppBar);

	if (const QColor& backgroundColor = d_func()->backgroundColor; backgroundColor.isValid()) {
		return backgroundColor;
	}

	// 如果启用 Mica 或没有背景色，返回默认值
	if (alApp->getIsEnableMica()) {
		return {};
	}

	return ALThemeColor(ALTheme->getThemeMode(), ALThemeType::WindowBase);
}

void CALAppBar::setCustomWidget(const ALAppBarType::CustomArea& customArea, QWidget* widget) {
	Q_D(CALAppBar);

	if (!widget || widget == this) {
		return;
	}

	widget->setMinimumHeight(0);
	widget->setMaximumHeight(height());
	widget->setMaximumWidth(d->customWidgetMaximumWidth);
	widget->setParent(this);

	if (d->customWidget) {
		d->mainHLayout->removeWidget(d->customWidget);
	}

	switch (customArea) {
		case ALAppBarType::LeftArea: {
			d->mainHLayout->insertWidget(4, widget);
			break;
		}
		case ALAppBarType::MiddleArea: {
			d->mainHLayout->insertWidget(5, widget);
			break;
		}
		case ALAppBarType::RightArea: {
			d->mainHLayout->insertWidget(6, widget);
			break;
		}
		default: break;
	}

	d->customWidget = widget;
	Q_EMIT sigCustomWidgetChanged();
}

QWidget* CALAppBar::getCustomWidget() const {
	return d_func()->customWidget;
}

void CALAppBar::setWindowButtonFlag(const ALAppBarType::ButtonFlag& buttonFlag, const bool isEnable) {
	setWindowButtonFlags(isEnable ? d_func()->buttonFlags | buttonFlag : d_func()->buttonFlags & ~buttonFlag);
}

void CALAppBar::setWindowButtonFlags(const ALAppBarType::ButtonFlags& buttonFlags) {
	Q_D(CALAppBar);

	d->buttonFlags = buttonFlags;
	d->routeBackButton->setVisible(d->buttonFlags.testFlag(ALAppBarType::RouteBackButtonHint));
	d->navigationButton->setVisible(d->buttonFlags.testFlag(ALAppBarType::NavigationButtonHint));
	d->stayTopButton->setVisible(d->buttonFlags.testFlag(ALAppBarType::StayTopButtonHint));
	d->themeChangeButton->setVisible(d->buttonFlags.testFlag(ALAppBarType::ThemeChangeButtonHint));
	d->minButton->setVisible(d->buttonFlags.testFlag(ALAppBarType::MinimizeButtonHint));
	d->maxButton->setVisible(d->buttonFlags.testFlag(ALAppBarType::MaximizeButtonHint));
	d->closeButton->setVisible(d->buttonFlags.testFlag(ALAppBarType::CloseButtonHint));
}

ALAppBarType::ButtonFlags CALAppBar::getWindowButtonFlags() const {
	return d_func()->buttonFlags;
}

bool CALAppBar::insertWidgetBeforeButton(QWidget* widget, const ALAppBarType::ButtonFlag& flag) {
	bool bRet{ false };

	Q_D(CALAppBar);

	const auto button = d->getButtonByAppBarFlag(flag);
	if (!button) {
		qDebug() << "Button not found for flag:" << flag;
		return bRet;
	}

	const int index = d->mainHLayout->indexOf(button);
	if (index == -1) {
		qDebug() << "Button not found in layout for flag:" << flag;
		return bRet;
	}

	bRet = true;
	d->mainHLayout->insertWidget(index, widget);
	return bRet;
}

bool CALAppBar::insertWidgetBeforeWidget(QWidget* widget, QWidget* targetWidget) {
	bool bRet{ false };

	Q_D(CALAppBar);

	const int index = d->mainHLayout->indexOf(targetWidget);
	if (index == -1) {
		qDebug() << "targetWidget not found in layout";
		return bRet;
	}

	bRet = true;
	d->mainHLayout->insertWidget(index, widget);
	return bRet;
}

bool CALAppBar::insertWidgetBeforeLayout(QWidget* widget, QLayout* targetLayout) {
	bool bRet{ false };

	Q_D(CALAppBar);

	const int index = d->mainHLayout->indexOf(targetLayout);
	if (index == -1) {
		qDebug() << "targetLayout not found in layout";
		return bRet;
	}

	bRet = true;
	d->mainHLayout->insertWidget(index, widget);
	return bRet;
}

bool CALAppBar::insertLayoutBeforeButton(QLayout* layout, const ALAppBarType::ButtonFlag& flag) {
	bool bRet{ false };

	Q_D(CALAppBar);

	const auto button = d->getButtonByAppBarFlag(flag);
	if (!button) {
		qDebug() << "Button not found for flag:" << flag;
		return bRet;
	}

	const int index = d->mainHLayout->indexOf(button);
	if (index == -1) {
		qDebug() << "Button not found in layout for flag:" << flag;
		return bRet;
	}

	bRet = true;
	d->mainHLayout->insertLayout(index, layout);
	return bRet;
}

bool CALAppBar::insertLayoutBeforeWidget(QLayout* layout, QWidget* targetWidget) {
	bool bRet{ false };

	Q_D(CALAppBar);

	const int index = d->mainHLayout->indexOf(targetWidget);
	if (index == -1) {
		qDebug() << "targetWidget not found in layout";
		return bRet;
	}

	bRet = true;
	d->mainHLayout->insertLayout(index, layout);
	return bRet;
}

bool CALAppBar::insertLayoutBeforeLayout(QLayout* layout, QLayout* targetLayout) {
	bool bRet{ false };

	Q_D(CALAppBar);

	const int index = d->mainHLayout->indexOf(targetLayout);
	if (index == -1) {
		qDebug() << "targetLayout not found in layout";
		return bRet;
	}

	bRet = true;
	d->mainHLayout->insertLayout(index, layout);
	return bRet;
}

void CALAppBar::setRouteBackButtonEnable(const bool isEnable) {
	d_func()->routeBackButton->setEnabled(isEnable);
}

void CALAppBar::closeWindow() {
	Q_D(CALAppBar);

	const auto closeOpacityAnimation = new QPropertyAnimation(window(), "windowOpacity");
	connect(closeOpacityAnimation, &QPropertyAnimation::finished, this, [=]() { window()->close(); });
	closeOpacityAnimation->setStartValue(1);
	closeOpacityAnimation->setEndValue(0);
	closeOpacityAnimation->setEasingCurve(QEasingCurve::InOutSine);
	closeOpacityAnimation->start(QAbstractAnimation::DeleteWhenStopped);
	if (window()->isMaximized() || window()->isFullScreen() || d->isFixedSize) {
		return;
	}
	const auto geometryAnimation = new QPropertyAnimation(window(), "geometry");
	const QRect geometry = window()->geometry();
	geometryAnimation->setStartValue(geometry);
	const qreal targetWidth = (geometry.width() - d->lastMinTrackWidth) * 0.7 + d->lastMinTrackWidth;
	const qreal targetHeight = (geometry.height() - window()->minimumHeight()) * 0.7 + window()->minimumHeight();
	geometryAnimation->setEndValue(QRectF(geometry.center().x() - targetWidth / 2.0, geometry.center().y() - targetHeight / 2.0, targetWidth, targetHeight));
	geometryAnimation->setEasingCurve(QEasingCurve::InOutSine);
	geometryAnimation->start(QAbstractAnimation::DeleteWhenStopped);
}

#ifdef Q_OS_WIN
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
int CALAppBar::takeOverNativeEvent(const QByteArray& eventType, void* message, qintptr* result)
#else
int CALAppBar::takeOverNativeEvent(const QByteArray& eventType, const void* message, long* result)
#endif
{
	Q_D(CALAppBar);

	if ((eventType != "windows_generic_MSG") || (message == nullptr)) {
		return 0;
	}

	const auto msg = static_cast<const MSG*>(message);
	HWND hwnd = msg->hwnd; // NOLINT
	if (!hwnd) {
		return 0;
	}
	d->currentWinID = reinterpret_cast<qint64>(hwnd);
	const UINT uMsg = msg->message;
	const WPARAM wParam = msg->wParam;
	const LPARAM lParam = msg->lParam;
	switch (uMsg) {
		case WM_WINDOWPOSCHANGING: {
			if (const auto wp = reinterpret_cast<WINDOWPOS*>(lParam); wp != nullptr && (wp->flags & SWP_NOSIZE) == 0) {
				wp->flags |= SWP_NOCOPYBITS;
				*result = static_cast<long>(::DefWindowProcW(hwnd, uMsg, wParam, lParam));
				return 1;
			}
			return 0;
		}
		case WM_NCACTIVATE: {
			*result = TRUE;
			return 1;
		}
		case WM_SIZE: {
			if (wParam == SIZE_RESTORED) {
				d->changeMaxButtonIcon(false);
			} else if (wParam == SIZE_MAXIMIZED) {
				d->changeMaxButtonIcon(true);
			}
			return 0;
		}
		case WM_NCCALCSIZE: {
#if (QT_VERSION >= QT_VERSION_CHECK(6, 5, 3) && QT_VERSION <= QT_VERSION_CHECK(6, 6, 1))
			if (wParam == FALSE)
			{
				return 0;
			}
			if (::IsZoomed(hwnd))
			{
				this->move(7, 7);
				window()->setContentsMargins(8, 8 + height(), 8, 8);
			}
			else
			{
				this->move(0, 0);
				window()->setContentsMargins(0, height(), 0, 0);
			}
			*result = 0;
			return 1;
#else
			if (wParam == FALSE) {
				return 0;
			}
			RECT* clientRect = &reinterpret_cast<NCCALCSIZE_PARAMS*>(lParam)->rgrc[0];
			if (!::IsZoomed(hwnd)) {
				clientRect->top -= 1;
				clientRect->bottom -= 1;
			} else {
				if (const LRESULT hitTestResult = ::DefWindowProcW(hwnd, WM_NCCALCSIZE, wParam, lParam); (hitTestResult != HTERROR) && (hitTestResult != HTNOWHERE)) {
					*result = static_cast<long>(hitTestResult);
					return 1;
				}
#if QT_VERSION >= QT_VERSION_CHECK(5, 14, 0)
				auto geometry = window()->screen()->geometry();
#else
				QScreen* screen = qApp->screenAt(window()->geometry().center());
				QRect geometry;
				if (!screen)
				{
					screen = qApp->screenAt(QCursor::pos());
				}
				geometry = screen->geometry();
#endif
				clientRect->top = geometry.top();
			}
			*result = WVR_REDRAW;
			return 1;
#endif
		}
		case WM_MOVE: {
			if (QScreen* currentScreen = QApplication::screenAt(window()->geometry().center()); currentScreen && currentScreen != d->lastScreen) {
				if (d->lastScreen) {
					::SetWindowPos(hwnd, nullptr, 0, 0, 0, 0, SWP_NOZORDER | SWP_NOOWNERZORDER | SWP_NOMOVE | SWP_NOSIZE | SWP_FRAMECHANGED);
					::RedrawWindow(hwnd, nullptr, nullptr, RDW_INVALIDATE | RDW_UPDATENOW);
				}
				d->lastScreen = currentScreen;
			}
			break;
		}
		case WM_NCHITTEST: {
			if (d->containsCursorToItem(d->maxButton)) {
				if (*result == HTNOWHERE) {
					if (!d->isHoverMaxButton) {
						d->isHoverMaxButton = true;
						d->maxButton->setIsSelected(true);
						d->maxButton->update();
					}
					*result = HTZOOM;
				}
				return 1;
			} else {
				if (d->isHoverMaxButton) {
					d->isHoverMaxButton = false;
					d->maxButton->setIsSelected(false);
					d->maxButton->update();
				}
			}
			POINT nativeLocalPos{ GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };
			::ScreenToClient(hwnd, &nativeLocalPos);
			RECT clientRect{ 0, 0, 0, 0 };
			::GetClientRect(hwnd, &clientRect);
			const auto clientWidth = clientRect.right - clientRect.left;
			const auto clientHeight = clientRect.bottom - clientRect.top;
			const bool left = nativeLocalPos.x < d->margins;
			const bool right = nativeLocalPos.x > clientWidth - d->margins;
			const bool top = nativeLocalPos.y < d->margins;
			const bool bottom = nativeLocalPos.y > clientHeight - d->margins;
			*result = 0;
			if (!d->isOnlyAllowMinAndClose && !d->isFixedSize && !window()->isFullScreen() && !window()->isMaximized()) {
				if (left && top) {
					*result = HTTOPLEFT;
				} else if (left && bottom) {
					*result = HTBOTTOMLEFT;
				} else if (right && top) {
					*result = HTTOPRIGHT;
				} else if (right && bottom) {
					*result = HTBOTTOMRIGHT;
				} else if (left) {
					*result = HTLEFT;
				} else if (right) {
					*result = HTRIGHT;
				} else if (top) {
					*result = HTTOP;
				} else if (bottom) {
					*result = HTBOTTOM;
				}
			}
			if (0 != *result) {
				return 1;
			}
			if (d->containsCursorToItem(this)) {
				*result = HTCAPTION;
				return 1;
			}
			*result = HTCLIENT;
			return 1;
		}
		case WM_GETMINMAXINFO: {
			const auto minmaxInfo = reinterpret_cast<MINMAXINFO*>(lParam);
			RECT rect;
			SystemParametersInfo(SPI_GETWORKAREA, 0, &rect, 0);
			d->lastMinTrackWidth = d->calculateMinimumWidth();
			minmaxInfo->ptMinTrackSize.x = d->lastMinTrackWidth * qApp->devicePixelRatio();            // NOLINT
			minmaxInfo->ptMinTrackSize.y = parentWidget()->minimumHeight() * qApp->devicePixelRatio(); // NOLINT
			minmaxInfo->ptMaxPosition.x = rect.left;
			minmaxInfo->ptMaxPosition.y = rect.top;
			return 1;
		}
		case WM_LBUTTONDBLCLK: {
			QVariantMap postData;
			postData.insert("WMClickType", ALAppBarType::WMLBUTTONDBLCLK);
			CALEventBus::instance()->post("WMWindowClicked", postData);
			return 0;
		}
		case WM_LBUTTONDOWN: {
			QVariantMap postData;
			postData.insert("WMClickType", ALAppBarType::WMLBUTTONDOWN);
			CALEventBus::instance()->post("WMWindowClicked", postData);
			return 0;
		}
		case WM_LBUTTONUP: {
			QVariantMap postData;
			postData.insert("WMClickType", ALAppBarType::WMLBUTTONUP);
			CALEventBus::instance()->post("WMWindowClicked", postData);
			return 0;
		}
		case WM_NCLBUTTONDOWN: {
			QVariantMap postData;
			postData.insert("WMClickType", ALAppBarType::WMNCLBUTTONDOWN);
			CALEventBus::instance()->post("WMWindowClicked", postData);
			if (d->containsCursorToItem(d->maxButton) || d->isOnlyAllowMinAndClose) {
				return 1;
			}
			break;
		}
		case WM_NCLBUTTONUP: {
			QVariantMap postData;
			postData.insert("WMClickType", ALAppBarType::WMNCLBUTTONDOWN);
			CALEventBus::instance()->post("WMWindowClicked", postData);
			if (d->containsCursorToItem(d->maxButton) && !d->isOnlyAllowMinAndClose) {
				d->slotMaxButtonClicked();
				return 1;
			}
			break;
		}
		case WM_NCLBUTTONDBLCLK: {
			if (!d->isOnlyAllowMinAndClose && !d->isFixedSize) {
				return 0;
			}
			return 1;
		}
		case WM_NCRBUTTONDOWN: {
			if (wParam == HTCAPTION && !d->isOnlyAllowMinAndClose) {
				d->showSystemMenu(QCursor::pos());
			}
			break;
		}
		case WM_KEYDOWN:
		case WM_SYSKEYDOWN: {
			if ((GetAsyncKeyState(VK_MENU) & 0x8000) && (GetAsyncKeyState(VK_SPACE) & 0x8000) && !d->isOnlyAllowMinAndClose) {
				auto pos = window()->geometry().topLeft();
				d->showSystemMenu(QPoint(pos.x(), pos.y() + this->height()));
			}
			break;
		}
		default: break;
	}

	return -1;
}
#endif

void CALAppBar::paintEvent(QPaintEvent* event) {
	Q_D(CALAppBar);

	if (d->backgroundColor.isValid()) {
		QPainter painter(this);
		painter.save();
		painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);
		painter.setBrush(d->backgroundColor);
		painter.drawRect(rect());
		painter.restore();
	}

	QWidget::paintEvent(event);
}

bool CALAppBar::eventFilter(QObject* watched, QEvent* event) {
	Q_D(CALAppBar);

	switch (event->type()) {
		case QEvent::Resize: {
			QSize size = parentWidget()->size();
#if (QT_VERSION >= QT_VERSION_CHECK(6, 5, 3) && QT_VERSION <= QT_VERSION_CHECK(6, 6, 1))
			if (::IsZoomed((HWND)d->_currentWinID))
			{
				this->resize(size.width() - 14, this->height());
			}
			else
			{
				this->resize(size.width(), this->height());
			}
#else
			this->resize(size.width(), this->height());
#endif
			break;
		}
#ifdef Q_OS_WIN
		case QEvent::Show: {
			if (!d->isFixedSize && !d->isOnlyAllowMinAndClose) {
				auto hwnd = reinterpret_cast<HWND>(d->currentWinID);
				DWORD style = ::GetWindowLongPtr(hwnd, GWL_STYLE);
				::SetWindowLongPtr(hwnd, GWL_STYLE, style | WS_MAXIMIZEBOX | WS_THICKFRAME);
			}
#if (QT_VERSION >= QT_VERSION_CHECK(6, 5, 3) && QT_VERSION <= QT_VERSION_CHECK(6, 6, 1))
		auto hwnd = reinterpret_cast<HWND>(d->currentWinID);
        setShadow(hwnd);
        DWORD style = ::GetWindowLongPtr(hwnd, GWL_STYLE);
        bool hasCaption = (style & WS_CAPTION) == WS_CAPTION;
        if (!hasCaption) {
            ::SetWindowLongPtr(hwnd, GWL_STYLE, style | WS_CAPTION);
        }
#endif
			break;
		}
#endif
		case QEvent::Close: {
			if (const auto closeEvent = dynamic_cast<QCloseEvent*>(event); !d->isDefaultClosed && closeEvent->spontaneous()) {
				event->ignore();
				if (window()->isMinimized()) {
					window()->showNormal();
				}
				d->slotCloseButtonClicked();
			}
			return true;
		}
#ifndef Q_OS_WIN
		case QEvent::MouseButtonPress: {
			if (d->edges != 0) {
				if (auto mouseEvent = dynamic_cast<QMouseEvent*>(event); mouseEvent->button() == Qt::LeftButton) {
					d->updateCursor(d->edges);
					window()->windowHandle()->startSystemResize(Qt::Edges(d->edges));
				}
			} else {
				if (d->containsCursorToItem(this)) {
					qint64 clickTimer = QDateTime::currentMSecsSinceEpoch();
					qint64 offset = clickTimer - static_cast<qint64>(d->clickTimer);
					d->clickTimer = clickTimer;
					if (offset > 300) {
						window()->windowHandle()->startSystemMove();
					}
				}
			}
			break;
		}
		case QEvent::MouseButtonDblClick: {
			if (d->containsCursorToItem(this)) {
				if (window()->isMaximized()) {
					window()->showNormal();
				} else {
					window()->showMaximized();
				}
			}
			break;
		}
		case QEvent::MouseButtonRelease: {
			d->edges = 0;
			break;
		}
		case QEvent::HoverMove: {
			if (window()->isMaximized() || window()->isFullScreen()) {
				break;
			}
			if (d->isFixedSize) {
				break;
			}
			auto mouseEvent = dynamic_cast<QHoverEvent*>(event);
			const QPoint p =
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
				mouseEvent->pos();
#else
            mouseEvent->position().toPoint();
#endif
			if (p.x() >= d->margins && p.x() <= (window()->width() - d->margins) && p.y() >= d->margins && p.y() <= (window()->height() - d->margins)) {
				if (d->edges != 0) {
					d->edges = 0;
					d->updateCursor(d->edges);
				}
				break;
			}
			d->edges = 0;
			if (p.x() < d->margins) {
				d->edges |= Qt::LeftEdge;
			}
			if (p.x() > (window()->width() - d->margins)) {
				d->edges |= Qt::RightEdge;
			}
			if (p.y() < d->margins) {
				d->edges |= Qt::TopEdge;
			}
			if (p.y() > (window()->height() - d->margins)) {
				d->edges |= Qt::BottomEdge;
			}
			d->updateCursor(d->edges);
			break;
		}
#endif
		default: {
			break;
		}
	}
	return QWidget::eventFilter(watched, event);
}
}
