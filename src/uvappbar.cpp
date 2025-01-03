#include "uvappbar.hpp"

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

#include "uvappbar_p.hpp"
#include "uvapplication.hpp"
#include "uvawesometoolbutton.hpp"
#include "uveventbus.hpp"
#include "uviconbutton.hpp"
#include "uvnavigationbar.hpp"
#include "uvtext.hpp"
#include "uvtheme.hpp"
#include "uvwinshadowhelper.hpp"

/**
 * @brief \class CUVAppBarPrivate
 * Internal class for CUVAppBar
 * @param q pointer to the public class
 * @param parent pointer to the parent class
 */
CUVAppBarPrivate::CUVAppBarPrivate(CUVAppBar* q, QObject* parent): QObject(parent), q_ptr(q) {
}

CUVAppBarPrivate::~CUVAppBarPrivate() = default;

void CUVAppBarPrivate::slotMinButtonClicked() {
	q_func()->window()->showMinimized();
}

void CUVAppBarPrivate::slotMaxButtonClicked() {
	Q_Q(CUVAppBar);

	q->window()->isMaximized() ? q->window()->showNormal() : q->window()->showMaximized();
}

void CUVAppBarPrivate::slotCloseButtonClicked() {
	Q_Q(CUVAppBar);

	isDefaultClosed ? q->window()->close() : Q_EMIT q->sigCloseButtonClicked();
}

void CUVAppBarPrivate::slotStayTopButtonClicked() const {
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

void CUVAppBarPrivate::slotThemeModeChanged(const UVThemeType::ThemeMode& mode) const {
	themeChangeButton->setAweSomeIcon(mode == UVThemeType::Light ? UVIcon::CUVAweSomeIcon::MoonStars : UVIcon::CUVAweSomeIcon::SunBright);
	themeChangeButton->setToolTip(mode == UVThemeType::Light ? tr("Switch to dark theme") : tr("Switch to light theme"));
}

void CUVAppBarPrivate::changeMaxButtonAwesome(const bool isMaximized) const {
	maxButton->setAweSomeIcon(isMaximized ? UVIcon::CUVAweSomeIcon::WindowRestore : UVIcon::CUVAweSomeIcon::Square);
}

void CUVAppBarPrivate::showSystemMenu(const QPoint& point) {
	Q_Q(CUVAppBar);

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

void CUVAppBarPrivate::updateCursor(const int edges) {
	Q_Q(CUVAppBar);

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

bool CUVAppBarPrivate::containsCursorToItem(QWidget* item) { // NOLINT
	Q_Q(CUVAppBar);

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

int CUVAppBarPrivate::calculateMinimumWidth() {
	Q_Q(CUVAppBar);
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
	if (q->parentWidget()->findChild<CUVNavigationBar*>()) {
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
		if (button->isVisible() && button->objectName() != "CUVNavigationButton") {
			width += button->width();
		}
	}
	return width;
}

QVBoxLayout* CUVAppBarPrivate::createVLayout(QWidget* widget) const {
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

QAbstractButton* CUVAppBarPrivate::getButtonByAppBarFlag(const UVAppBarType::ButtonFlag& flag) const {
	return buttonMap.value(flag, nullptr);
}


/**
 * @brief \class CUVAppBar
 * @param parent pointer to the parent class
 */
CUVAppBar::CUVAppBar(QWidget* parent): QWidget(parent), d_ptr(new CUVAppBarPrivate(this, this)) {
	Q_ASSERT(parent);

	Q_D(CUVAppBar);

	d->buttonFlags = UVAppBarType::RouteBackButtonHint | UVAppBarType::StayTopButtonHint | UVAppBarType::ThemeChangeButtonHint | UVAppBarType::MinimizeButtonHint | UVAppBarType::MaximizeButtonHint | UVAppBarType::CloseButtonHint;
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
	setObjectName("CUVAppBar");
	setStyleSheet("#CUVAppBar { background-color: transparent; }");
	// 路由跳转
	d->routeBackButton = new CUVAwesomeToolButton(this);
	d->routeBackButton->setAweSomeIcon(UVIcon::CUVAweSomeIcon::ArrowLeft);
	d->routeBackButton->setFixedSize(d->appBarHeight, d->appBarHeight);
	d->routeBackButton->setEnabled(false);
	d->routeBackButton->setToolTip(tr("Back"));
	connect(d->routeBackButton, &CUVAwesomeToolButton::clicked, this, &CUVAppBar::sigRouteBackButtonClicked);

	// 导航栏展开
	d->navigationButton = new CUVAwesomeToolButton(this);
	d->navigationButton->setAweSomeIcon(UVIcon::CUVAweSomeIcon::Bars);
	d->navigationButton->setFixedSize(40, 30);
	d->navigationButton->setObjectName("CUVNavigationButton");
	d->navigationButton->setVisible(false);
	d->navigationButton->setToolTip(tr("Navigation"));
	connect(d->navigationButton, &CUVAwesomeToolButton::clicked, this, &CUVAppBar::sigNavigationButtonClicked);

	// 置顶
	d->stayTopButton = new CUVAwesomeToolButton(this);
	d->stayTopButton->setAweSomeIcon(UVIcon::CUVAweSomeIcon::ArrowUpToArc);
	d->stayTopButton->setFixedSize(d->appBarHeight, d->appBarHeight);
	d->stayTopButton->setToolTip(tr("Stay Top"));
	d->buttonMap[UVAppBarType::StayTopButtonHint] = d->stayTopButton;
	connect(d->stayTopButton, &CUVAwesomeToolButton::clicked, this, [=]() { this->setIsStayTop(!this->getIsStayTop()); });
	connect(d->stayTopButton, &CUVAwesomeToolButton::sigSelectedChanged, this, [=]() { d->stayTopButton->setToolTip(d->stayTopButton->getIsSelected() ? tr("Cancel Stay Top") : tr("Stay Top")); });
	connect(this, &CUVAppBar::sigIsStayTopChanged, d, &CUVAppBarPrivate::slotStayTopButtonClicked);

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
	d->titleLabel = new CUVText(this);
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
	d->themeChangeButton = new CUVAwesomeToolButton(this);
	d->themeChangeButton->setAweSomeIcon(UVIcon::CUVAweSomeIcon::SunBright);
	d->themeChangeButton->setFixedSize(d->appBarHeight, d->appBarHeight);
	d->themeChangeButton->setToolTip(UVTheme->getThemeMode() == UVThemeType::Light ? tr("Switch to dark theme") : tr("Switch to light theme"));
	d->buttonMap[UVAppBarType::ThemeChangeButtonHint] = d->themeChangeButton;
	connect(d->themeChangeButton, &CUVAwesomeToolButton::clicked, this, &CUVAppBar::sigThemeChangeButtonClicked);
	connect(UVTheme, &CUVThemeManager::sigThemeModeChanged, this, [=](const UVThemeType::ThemeMode& mode) { d->slotThemeModeChanged(mode); });

	// 最小化
	d->minButton = new CUVAwesomeToolButton(this);
	d->minButton->setAweSomeIcon(UVIcon::CUVAweSomeIcon::Dash);
	d->minButton->setFixedSize(d->appBarHeight, d->appBarHeight);
	d->minButton->setToolTip(tr("Minimize"));
	d->buttonMap[UVAppBarType::MinimizeButtonHint] = d->minButton;
	connect(d->minButton, &CUVAwesomeToolButton::clicked, d, &CUVAppBarPrivate::slotMinButtonClicked);

	// 最大化 & 还原
	d->maxButton = new CUVAwesomeToolButton(this);
	d->maxButton->setIconSize({ 18, 18 });
	d->maxButton->setAweSomeIcon(UVIcon::CUVAweSomeIcon::Square);
	d->maxButton->setFixedSize(d->appBarHeight, d->appBarHeight);
	d->buttonMap[UVAppBarType::MaximizeButtonHint] = d->maxButton;
	connect(d->maxButton, &CUVAwesomeToolButton::clicked, d, &CUVAppBarPrivate::slotMaxButtonClicked);

	// 关闭
	d->closeButton = new CUVIconButton(UVIcon::CUVAweSomeIcon::Close, 18, d->appBarHeight, d->appBarHeight, this);
	d->closeButton->setLightHoverColor(QColor(0xE8, 0x11, 0x23));
	d->closeButton->setDarkHoverColor(QColor(0xE8, 0x11, 0x23));
	d->closeButton->setLightHoverIconColor(Qt::white);
	d->closeButton->setDarkHoverIconColor(Qt::white);
	d->closeButton->setToolTip(tr("Close"));
	d->buttonMap[UVAppBarType::CloseButtonHint] = d->closeButton;
	connect(d->closeButton, &CUVIconButton::clicked, d, &CUVAppBarPrivate::slotCloseButtonClicked);

	d->mainHLayout = new QHBoxLayout(this);
	d->mainHLayout->setContentsMargins(0, 0, 0, 0);
	d->mainHLayout->setSpacing(0);
	d->mainHLayout->addLayout(d->createVLayout(d->routeBackButton));
	d->mainHLayout->addLayout(d->createVLayout(d->navigationButton));
	d->mainHLayout->addLayout(d->iconLabelVLayout);
	d->mainHLayout->addLayout(d->titleLabelVLayout);
	d->mainHLayout->addStretch();
	d->mainHLayout->addWidget(d->stayTopButton);
	d->mainHLayout->addWidget(d->themeChangeButton);
	d->mainHLayout->addWidget(d->minButton);
	d->mainHLayout->addWidget(d->maxButton);
	d->mainHLayout->addWidget(d->closeButton);
	d->mainHLayout->setAlignment(Qt::AlignHCenter);

#ifdef Q_OS_WIN
	for (int i = 0; i < QApplication::screens().count(); i++) {
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
}

CUVAppBar::~CUVAppBar() = default;

void CUVAppBar::setIsStayTop(const bool isStayTop) {
	d_func()->isStayTop = isStayTop;
	Q_EMIT sigIsStayTopChanged();
}

bool CUVAppBar::getIsStayTop() const {
	return d_func()->isStayTop;
}

void CUVAppBar::setIsFixedSize(const bool isFixedSize) {
	Q_D(CUVAppBar);

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

bool CUVAppBar::getIsFixedSize() const {
	return d_func()->isFixedSize;
}

void CUVAppBar::setIsDefaultClosed(const bool isDefaultClosed) {
	d_func()->isDefaultClosed = isDefaultClosed;
	Q_EMIT sigIsDefaultClosedChanged();
}

bool CUVAppBar::getIsDefaultClosed() const {
	return d_func()->isDefaultClosed;
}

void CUVAppBar::setIsOnlyAllowMinAndClose(const bool isOnlyAllowMinAndClose) {
	d_func()->isOnlyAllowMinAndClose = isOnlyAllowMinAndClose;
	Q_EMIT sigIsOnlyAllowMinAndCloseChanged();
}

bool CUVAppBar::getIsOnlyAllowMinAndClose() const {
	return d_func()->isOnlyAllowMinAndClose;
}

void CUVAppBar::setAppBarHeight(const int appBarHeight) {
	d_func()->appBarHeight = appBarHeight;
	setFixedHeight(appBarHeight);
	window()->setContentsMargins(0, this->height(), 0, 0);
	Q_EMIT sigAppBarHeightChanged();
}

int CUVAppBar::getAppBarHeight() const {
	return d_func()->appBarHeight;
}

void CUVAppBar::setCustomWidgetMaximumWidth(const int customWidgetMaximumWidth) {
	Q_D(CUVAppBar);

	d->customWidgetMaximumWidth = customWidgetMaximumWidth;
	if (d->customWidget) {
		d->customWidget->setMaximumWidth(customWidgetMaximumWidth);
	}
	Q_EMIT sigCustomWidgetMaximumWidthChanged();
}

int CUVAppBar::getCustomWidgetMaximumWidth() const {
	return d_func()->customWidgetMaximumWidth;
}

void CUVAppBar::setBackgroundColor(const QColor& color) {
	d_func()->backgroundColor = color;
	update();
}

QColor CUVAppBar::getBackgroundColor() const {
	Q_D(const CUVAppBar);

	if (const QColor& backgroundColor = d_func()->backgroundColor; backgroundColor.isValid()) {
		return backgroundColor;
	}

	// 如果启用 Mica 或没有背景色，返回默认值
	if (uvApp->getIsEnableMica()) {
		return {};
	}

	return UVThemeColor(UVTheme->getThemeMode(), UVThemeType::WindowBase);
}

void CUVAppBar::setCustomWidget(const UVAppBarType::CustomArea& customArea, QWidget* widget) {
	Q_D(CUVAppBar);

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
		case UVAppBarType::LeftArea: {
			d->mainHLayout->insertWidget(4, widget);
			break;
		}
		case UVAppBarType::MiddleArea: {
			d->mainHLayout->insertWidget(5, widget);
			break;
		}
		case UVAppBarType::RightArea: {
			d->mainHLayout->insertWidget(6, widget);
			break;
		}
		default: break;
	}

	d->customWidget = widget;
	Q_EMIT sigCustomWidgetChanged();
}

QWidget* CUVAppBar::getCustomWidget() const {
	return d_func()->customWidget;
}

void CUVAppBar::setWindowButtonFlag(const UVAppBarType::ButtonFlag& buttonFlag, const bool isEnable) {
	setWindowButtonFlags(isEnable ? d_func()->buttonFlags | buttonFlag : d_func()->buttonFlags & ~buttonFlag);
}

void CUVAppBar::setWindowButtonFlags(const UVAppBarType::ButtonFlags& buttonFlags) {
	Q_D(CUVAppBar);

	d->buttonFlags = buttonFlags;
	d->routeBackButton->setVisible(d->buttonFlags.testFlag(UVAppBarType::RouteBackButtonHint));
	d->navigationButton->setVisible(d->buttonFlags.testFlag(UVAppBarType::NavigationButtonHint));
	d->stayTopButton->setVisible(d->buttonFlags.testFlag(UVAppBarType::StayTopButtonHint));
	d->themeChangeButton->setVisible(d->buttonFlags.testFlag(UVAppBarType::ThemeChangeButtonHint));
	d->minButton->setVisible(d->buttonFlags.testFlag(UVAppBarType::MinimizeButtonHint));
	d->maxButton->setVisible(d->buttonFlags.testFlag(UVAppBarType::MaximizeButtonHint));
	d->closeButton->setVisible(d->buttonFlags.testFlag(UVAppBarType::CloseButtonHint));
}

UVAppBarType::ButtonFlags CUVAppBar::getWindowButtonFlags() const {
	return d_func()->buttonFlags;
}

bool CUVAppBar::insertWidgetBeforeButton(QWidget* widget, const UVAppBarType::ButtonFlag& flag) {
	bool bRet{ false };

	Q_D(CUVAppBar);

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

bool CUVAppBar::insertWidgetBeforeWidget(QWidget* widget, QWidget* targetWidget) {
	bool bRet{ false };

	Q_D(CUVAppBar);

	const int index = d->mainHLayout->indexOf(targetWidget);
	if (index == -1) {
		qDebug() << "targetWidget not found in layout";
		return bRet;
	}

	bRet = true;
	d->mainHLayout->insertWidget(index, widget);
	return bRet;
}

bool CUVAppBar::insertWidgetBeforeLayout(QWidget* widget, QLayout* targetLayout) {
	bool bRet{ false };

	Q_D(CUVAppBar);

	const int index = d->mainHLayout->indexOf(targetLayout);
	if (index == -1) {
		qDebug() << "targetLayout not found in layout";
		return bRet;
	}

	bRet = true;
	d->mainHLayout->insertWidget(index, widget);
	return bRet;
}

bool CUVAppBar::insertLayoutBeforeButton(QLayout* layout, const UVAppBarType::ButtonFlag& flag) {
	bool bRet{ false };

	Q_D(CUVAppBar);

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

bool CUVAppBar::insertLayoutBeforeWidget(QLayout* layout, QWidget* targetWidget) {
	bool bRet{ false };

	Q_D(CUVAppBar);

	const int index = d->mainHLayout->indexOf(targetWidget);
	if (index == -1) {
		qDebug() << "targetWidget not found in layout";
		return bRet;
	}

	bRet = true;
	d->mainHLayout->insertLayout(index, layout);
	return bRet;
}

bool CUVAppBar::insertLayoutBeforeLayout(QLayout* layout, QLayout* targetLayout) {
	bool bRet{ false };

	Q_D(CUVAppBar);

	const int index = d->mainHLayout->indexOf(targetLayout);
	if (index == -1) {
		qDebug() << "targetLayout not found in layout";
		return bRet;
	}

	bRet = true;
	d->mainHLayout->insertLayout(index, layout);
	return bRet;
}

void CUVAppBar::setRouteBackButtonEnable(const bool isEnable) {
	d_func()->routeBackButton->setEnabled(isEnable);
}

void CUVAppBar::closeWindow() {
	Q_D(CUVAppBar);

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
int CUVAppBar::takeOverNativeEvent(const QByteArray& eventType, void* message, qintptr* result)
#else
int CUVAppBar::takeOverNativeEvent(const QByteArray& eventType, const void* message, long* result)
#endif
{
	Q_D(CUVAppBar);

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
				d->changeMaxButtonAwesome(false);
			} else if (wParam == SIZE_MAXIMIZED) {
				d->changeMaxButtonAwesome(true);
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
			postData.insert("WMClickType", UVAppBarType::WMLBUTTONDBLCLK);
			CUVEventBus::instance()->post("WMWindowClicked", postData);
			return 0;
		}
		case WM_LBUTTONDOWN: {
			QVariantMap postData;
			postData.insert("WMClickType", UVAppBarType::WMLBUTTONDOWN);
			CUVEventBus::instance()->post("WMWindowClicked", postData);
			return 0;
		}
		case WM_LBUTTONUP: {
			QVariantMap postData;
			postData.insert("WMClickType", UVAppBarType::WMLBUTTONUP);
			CUVEventBus::instance()->post("WMWindowClicked", postData);
			return 0;
		}
		case WM_NCLBUTTONDOWN: {
			QVariantMap postData;
			postData.insert("WMClickType", UVAppBarType::WMNCLBUTTONDOWN);
			CUVEventBus::instance()->post("WMWindowClicked", postData);
			if (d->containsCursorToItem(d->maxButton) || d->isOnlyAllowMinAndClose) {
				return 1;
			}
			break;
		}
		case WM_NCLBUTTONUP: {
			QVariantMap postData;
			postData.insert("WMClickType", UVAppBarType::WMNCLBUTTONDOWN);
			CUVEventBus::instance()->post("WMWindowClicked", postData);
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

void CUVAppBar::paintEvent(QPaintEvent* event) {
	Q_D(CUVAppBar);

	if (d->backgroundColor.isValid()) {
		QPainter painter(this);
		painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);
		painter.setBrush(d->backgroundColor);
		painter.drawRect(rect());
	}

	QWidget::paintEvent(event);
}

bool CUVAppBar::eventFilter(QObject* watched, QEvent* event) {
	Q_D(CUVAppBar);

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
