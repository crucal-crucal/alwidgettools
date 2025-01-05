#include "uvmainwindow.hpp"

#include <QDockWidget>
#include <QEvent>
#include <QHBoxLayout>
#include <QPropertyAnimation>
#include <QScreen>
#include <QTimer>
#include <QToolBar>
#include <QVariant>

#include "uvapplication.hpp"
#include "uvcenterstackedwidget.hpp"
#include "uveventbus.hpp"
#include "uvmainwindowstyle.hpp"
#include "uvmainwindow_p.hpp"
#include "uvmenu.hpp"
#include "uvnavigationbar.hpp"
#include "uvnavigationrouter.hpp"
#include "uvthememanager.hpp"
#include "uvthemeanimationwidget.hpp"

/**
 * @brief \class CUVMainWindowPrivate
 * Internal class for CUVMainWindow
 * @param q pointer to the public class
 * @param parent pointer to the parent class
 */
CUVMainWindowPrivate::CUVMainWindowPrivate(CUVMainWindow* q, QObject* parent): QObject(parent), q_ptr(q) {
	isWindowClosing = false;
	isInitFinished = false;
	isNavigationDisplayModeChanged = false;
	isNavigationEnable = true;
	isNavigationBarExpanded = false;
	isWMClickedAnimatinoFinished = true;
	contentsMargins = 5;
	navigationTargetIndex = 0;
	currentNavigationBarDisplayMode = UVNavigationType::Maximal;
}

CUVMainWindowPrivate::~CUVMainWindowPrivate() = default;

void CUVMainWindowPrivate::slotNavigationButtonClicked() {
	if (isWMClickedAnimatinoFinished) {
		isNavigationDisplayModeChanged = false;
		resetWindowLayout(true);
		navigationBar->setIsTransparent(false);
		navigationBar->setDisplayMode(UVNavigationType::Maximal, false);
		navigationBar->move(-navigationBar->width(), navigationBar->pos().y());
		navigationBar->resize(navigationBar->width(), centerStackedWidget->height() + 1);
		const auto navigationMoveAnimation = new QPropertyAnimation(navigationBar, "pos");
		connect(navigationMoveAnimation, &QPropertyAnimation::finished, this, [=]() { isNavigationBarExpanded = true; });
		navigationMoveAnimation->setEasingCurve(QEasingCurve::InOutSine);
		navigationMoveAnimation->setDuration(300);
		navigationMoveAnimation->setStartValue(navigationBar->pos());
		navigationMoveAnimation->setEndValue(QPoint(0, 0));
		navigationMoveAnimation->start(QAbstractAnimation::DeleteWhenStopped);
		isWMClickedAnimatinoFinished = false;
	}
}

void CUVMainWindowPrivate::slotThemeReadyChange() {
	Q_Q(CUVMainWindow);

	// 主题变更绘制窗口
	appBar->setIsOnlyAllowMinAndClose(true);
	if (!themeAnimationWidget) {
		const QPoint centerPos = q->mapFromGlobal(QCursor::pos());
		themeAnimationWidget = new CUVThemeAnimationWidget(q);
		connect(themeAnimationWidget, &CUVThemeAnimationWidget::sigAnimationFinished, this, [=]() {
			appBar->setIsOnlyAllowMinAndClose(false);
			themeAnimationWidget = nullptr;
		});
		themeAnimationWidget->move(0, 0);
		themeAnimationWidget->setOldWindowImage(q->grab(q->rect()).toImage());
		UVTheme->setThemeMode(UVTheme->getThemeMode() == UVThemeType::Light ? UVThemeType::Dark : UVThemeType::Light);
		themeAnimationWidget->setNewWindowImage(q->grab(q->rect()).toImage());
		themeAnimationWidget->setCenter(centerPos);
		const qreal topLeftDis = distance(centerPos, QPoint(0, 0));
		const qreal topRightDis = distance(centerPos, QPoint(q->width(), 0));
		const qreal bottomLeftDis = distance(centerPos, QPoint(0, q->height()));
		const qreal bottomRightDis = distance(centerPos, QPoint(q->width(), q->height()));
		QList<qreal> disList{ topLeftDis, topRightDis, bottomLeftDis, bottomRightDis };
		std::sort(disList.begin(), disList.end());
		themeAnimationWidget->setEndRadius(disList[3]);
		themeAnimationWidget->resize(q->width(), q->height());
		themeAnimationWidget->startAnimation(themeChangeTime);
		themeAnimationWidget->show();
	}
}

void CUVMainWindowPrivate::slotDisplayModeChanged() {
	currentNavigationBarDisplayMode = navigationDisplayMode;
	switch (currentNavigationBarDisplayMode) {
		case UVNavigationType::Auto: {
			appBar->setWindowButtonFlag(UVAppBarType::NavigationButtonHint, false);
			doNavigationDisplayModeChange();
			break;
		}
		case UVNavigationType::Minimal: {
			navigationBar->setDisplayMode(UVNavigationType::Minimal, true);
			appBar->setWindowButtonFlag(UVAppBarType::NavigationButtonHint);
			break;
		}
		case UVNavigationType::Compact: {
			navigationBar->setDisplayMode(UVNavigationType::Compact, true);
			appBar->setWindowButtonFlag(UVAppBarType::NavigationButtonHint, false);
			break;
		}
		case UVNavigationType::Maximal: {
			navigationBar->setDisplayMode(UVNavigationType::Maximal, true);
			appBar->setWindowButtonFlag(UVAppBarType::NavigationButtonHint, false);
			break;
		}
		default: {
			break;
		}
	}
}

void CUVMainWindowPrivate::slotThemeModeChanged(const UVThemeType::ThemeMode& mode) {
	Q_Q(CUVMainWindow);

	themeMode = mode;
	if (!uvApp->getIsEnableMica()) {
		QPalette palette = q->palette();
		palette.setBrush(QPalette::Window, UVThemeColor(themeMode, UVThemeType::WindowBase));
		q->setPalette(palette);
	}
}

void CUVMainWindowPrivate::slotNavigationNodeClicked(const UVNavigationType::NavigationNodeType& nodeType, const QString& nodeKey) {
	const int nodeIndex = routeMap.value(nodeKey);
	if (nodeIndex == -1 || (navigationTargetIndex == nodeIndex || centerStackedWidget->count() <= nodeIndex)) {
		return;
	}

	navigationTargetIndex = nodeIndex;
	QTimer::singleShot(180, this, [=]() {
		QWidget* currentWidget = centerStackedWidget->widget(nodeIndex);
		centerStackedWidget->setCurrentIndex(nodeIndex);
		const auto currentWidgetAnimation = new QPropertyAnimation(currentWidget, "pos");
		currentWidgetAnimation->setEasingCurve(QEasingCurve::OutCubic);
		currentWidgetAnimation->setDuration(300);
		QPoint currentWidgetPos = currentWidget->pos();
		currentWidgetAnimation->setEndValue(currentWidgetPos);
		currentWidgetPos.setY(currentWidgetPos.y() + 80);
		currentWidgetAnimation->setStartValue(currentWidgetPos);
		currentWidgetAnimation->start(QAbstractAnimation::DeleteWhenStopped);
	});
}

void CUVMainWindowPrivate::slotNavigationNodeAdded(const UVNavigationType::NavigationNodeType& nodeType, const QString& nodeKey, QWidget* page) {
	if (nodeType == UVNavigationType::PageNode) {
		routeMap.insert(nodeKey, centerStackedWidget->count());
		centerStackedWidget->addWidget(page);
	} else {
		if (page) {
			routeMap.insert(nodeKey, centerStackedWidget->count());
			centerStackedWidget->addWidget(page);
		} else {
			routeMap.insert(nodeKey, -1);
		}
	}
}

void CUVMainWindowPrivate::invokableWMWindowClickedEvent(const QVariantMap& data) {
	if (const auto actionType = data.value("WMClickType").value<UVAppBarType::WMMouseActionType>();
		actionType == UVAppBarType::WMLBUTTONDBLCLK || actionType == UVAppBarType::WMLBUTTONUP || actionType == UVAppBarType::WMNCLBUTTONDOWN) {
		if (CUVApplication::containsCursorToItem(navigationBar)) {
			return;
		}
		if (isNavigationBarExpanded) {
			const auto navigationMoveAnimation = new QPropertyAnimation(navigationBar, "pos");
			connect(navigationMoveAnimation, &QPropertyAnimation::valueChanged, this, [=]() {
				if (isNavigationDisplayModeChanged) {
					navigationBar->setIsTransparent(true);
					isWMClickedAnimatinoFinished = true;
					resetWindowLayout(false);
					navigationMoveAnimation->deleteLater();
				}
			});
			connect(navigationMoveAnimation, &QPropertyAnimation::finished, this, [=]() {
				navigationBar->setIsTransparent(true);
				if (!isNavigationDisplayModeChanged) {
					navigationBar->setDisplayMode(UVNavigationType::Minimal, false);
					resetWindowLayout(false);
				}
				isWMClickedAnimatinoFinished = true;
			});
			navigationMoveAnimation->setEasingCurve(QEasingCurve::InOutSine);
			navigationMoveAnimation->setDuration(300);
			navigationMoveAnimation->setStartValue(navigationBar->pos());
			navigationMoveAnimation->setEndValue(QPoint(-navigationBar->width(), 0));
			navigationMoveAnimation->start(QAbstractAnimation::DeleteWhenStopped);
			isNavigationBarExpanded = false;
		}
	}
}

qreal CUVMainWindowPrivate::distance(const QPoint& point1, const QPoint& point2) {
	return std::sqrt((point1.x() - point2.x()) * (point1.x() - point2.x()) + (point1.y() - point2.y()) * (point1.y() - point2.y()));
}

void CUVMainWindowPrivate::resetWindowLayout(const bool isAnimation) const {
	if (isAnimation) {
		while (centerHLayout->count() > 0) {
			centerHLayout->takeAt(0);
		}
	} else {
		if (centerHLayout->count() == 0) {
			navigationBar->setDisplayMode(UVNavigationType::Minimal, false);
			centerHLayout->addWidget(navigationBar);
			centerHLayout->addWidget(centerStackedWidget);
		}
	}
}

void CUVMainWindowPrivate::doNavigationDisplayModeChange() {
	Q_Q(CUVMainWindow);

	if (isWindowClosing || !isNavigationEnable || !isInitFinished) {
		return;
	}

	if (navigationDisplayMode == UVNavigationType::Minimal) {
		resetWindowLayout(false);
	}
	if (navigationDisplayMode == UVNavigationType::Auto) {
		isNavigationDisplayModeChanged = true;
		resetWindowLayout(false);
		if (const int width = q->centralWidget()->width(); width >= 850 && currentNavigationBarDisplayMode != UVNavigationType::Maximal) {
			navigationBar->setDisplayMode(UVNavigationType::Maximal);
			currentNavigationBarDisplayMode = UVNavigationType::Maximal;
			appBar->setWindowButtonFlag(UVAppBarType::NavigationButtonHint, false);
		} else if (width >= 550 && width < 850 && currentNavigationBarDisplayMode != UVNavigationType::Compact) {
			navigationBar->setDisplayMode(UVNavigationType::Compact);
			currentNavigationBarDisplayMode = UVNavigationType::Compact;
			appBar->setWindowButtonFlag(UVAppBarType::NavigationButtonHint, false);
		} else if (width < 550 && currentNavigationBarDisplayMode != UVNavigationType::Minimal) {
			navigationBar->setDisplayMode(UVNavigationType::Minimal);
			currentNavigationBarDisplayMode = UVNavigationType::Minimal;
			appBar->setWindowButtonFlag(UVAppBarType::NavigationButtonHint);
		}
		isNavigationBarExpanded = false;
	}
}


/**
 * @brief \class CUVMainWindow
 */
Q_TAKEOVER_NATIVEEVENT_CPP(CUVMainWindow, d_func()->appBar)

CUVMainWindow::CUVMainWindow(QWidget* parent): QMainWindow(parent), d_ptr(new CUVMainWindowPrivate(this, this)) {
	Q_D(CUVMainWindow);

	setProperty("CUVBaseClassName", "CUVWindow");
	resize(1020, 680);
	d->themeChangeTime = 700;
	d->navigationDisplayMode = UVNavigationType::Auto;
	connect(this, &CUVMainWindow::sigNavigationDisplayModeChanged, d, &CUVMainWindowPrivate::slotDisplayModeChanged);

	/// appBar
	d->appBar = new CUVAppBar(this);
	connect(d->appBar, &CUVAppBar::sigRouteBackButtonClicked, this, []() { CUVNavigationRouter::instance()->navigationRouteBack(); });
	connect(d->appBar, &CUVAppBar::sigCloseButtonClicked, this, &CUVMainWindow::sigCloseButtonClicked);
	connect(d->appBar, &CUVAppBar::sigNavigationButtonClicked, d, &CUVMainWindowPrivate::slotNavigationButtonClicked);
	connect(d->appBar, &CUVAppBar::sigThemeChangeButtonClicked, d, &CUVMainWindowPrivate::slotThemeReadyChange);

	/// navigationBar
	d->navigationBar = new CUVNavigationBar(this);
	// 返回按钮状态变更
	connect(CUVNavigationRouter::instance(), &CUVNavigationRouter::sigNavigationRouterStateChanged, this, [d](const bool isEnable) { d->appBar->setRouteBackButtonEnable(isEnable); });
	// 转发用户卡片点击信号
	connect(d->navigationBar, &CUVNavigationBar::sigUserInfoCardClicked, this, &CUVMainWindow::sigUserInfoCardClicked);
	// 转发点击信号
	connect(d->navigationBar, &CUVNavigationBar::sigNavigationNodeClicked, this, &CUVMainWindow::sigNavigationNodeClicked);
	// 跳转处理
	connect(d->navigationBar, &CUVNavigationBar::sigNavigationNodeClicked, d, &CUVMainWindowPrivate::slotNavigationNodeClicked);
	// 新增窗口
	connect(d->navigationBar, &CUVNavigationBar::sigNavigationNodeAdded, d, &CUVMainWindowPrivate::slotNavigationNodeAdded);

	/// center stacked widget
	d->centerStackedWidget = new CUVCenterStackedWidget(this);
	d->centerStackedWidget->setContentsMargins(0, 0, 0, 0);
	const auto centralWidget = new QWidget(this);
	d->centerHLayout = new QHBoxLayout(centralWidget);
	d->centerHLayout->setSpacing(0);
	d->centerHLayout->addWidget(d->navigationBar);
	d->centerHLayout->addWidget(d->centerStackedWidget);
	d->centerHLayout->setContentsMargins(d->contentsMargins, 0, 0, 0);

	/// event filter
	d->focusEvent = new CUVEvent("WMWindowClicked", "invokableWMWindowClickedEvent", d);
	d->focusEvent->registerAndInit();

	/// theme
	d->themeMode = UVTheme->getThemeMode();
	connect(UVTheme, &CUVThemeManager::sigThemeModeChanged, d, &CUVMainWindowPrivate::slotThemeModeChanged);
	d->isInitFinished = true;
	setCentralWidget(centralWidget);
	centralWidget->installEventFilter(this);

	setObjectName("CUVWindow");
	setStyleSheet("#CUVWindow { background-color: transparent; }");
	setStyle(new CUVMainWindowStyle(style()));

	/// Delay rendering
	QTimer::singleShot(1, this, [=]() {
		QPalette palette = this->palette();
		palette.setBrush(QPalette::Window, UVThemeColor(d->themeMode, UVThemeType::WindowBase));
		this->setPalette(palette);
	});
	uvApp->syncMica(this);
	connect(uvApp, &CUVApplication::sigIsEnableMicaChanged, this, [=]() { d->slotThemeModeChanged(d->themeMode); });
}

CUVMainWindow::~CUVMainWindow() = default;

void CUVMainWindow::moveToCenter() {
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

void CUVMainWindow::setCustomWidget(const UVAppBarType::CustomArea& customArea, QWidget* customWidget) {
	d_func()->appBar->setCustomWidget(customArea, customWidget);
	Q_EMIT sigCustomWidgetChanged();
}

QWidget* CUVMainWindow::getCustomWidget() const {
	return d_func()->appBar->getCustomWidget();
}

void CUVMainWindow::setIsNavigationBarEnable(const bool isEnable) {
	Q_D(CUVMainWindow);

	d->isNavigationEnable = isEnable;
	d->navigationBar->setVisible(isEnable);
	d->centerHLayout->setContentsMargins(isEnable ? d->contentsMargins : 0, 0, 0, 0);
	d->centerStackedWidget->setIsHasRadius(isEnable);
}

bool CUVMainWindow::getIsNavigationBarEnable() const {
	return d_func()->isNavigationEnable;
}

void CUVMainWindow::setUserInfoCardVisible(const bool isVisible) {
	d_func()->navigationBar->setUserInfoCardVisible(isVisible);
}

void CUVMainWindow::setUserInfoCardPixmap(const QPixmap& pix) {
	d_func()->navigationBar->setUserInfoCardPixmap(pix);
}

void CUVMainWindow::setUserInfoCardTitle(const QString& title) {
	d_func()->navigationBar->setUserInfoCardTitle(title);
}

void CUVMainWindow::setUserInfoCardSubTitle(const QString& subTitle) {
	d_func()->navigationBar->setUserInfoCardSubTitle(subTitle);
}

UVNavigationType::NodeOperateReturnType CUVMainWindow::addExpanderNode(const QString& expanderTitle, QString& expanderKey, const UVIcon::CUVAweSomeIcon& awesomeIcon) const {
	return d_func()->navigationBar->addExpanderNode(expanderTitle, expanderKey, awesomeIcon);
}

UVNavigationType::NodeOperateReturnType CUVMainWindow::addExpanderNode(const QString& expanderTitle, QString& expanderKey, const QString& targetExpanderKey, const UVIcon::CUVAweSomeIcon& awesomeIcon) const {
	return d_func()->navigationBar->addExpanderNode(expanderTitle, expanderKey, targetExpanderKey, awesomeIcon);
}

UVNavigationType::NodeOperateReturnType CUVMainWindow::addPageNode(const QString& pageTitle, QWidget* page, const UVIcon::CUVAweSomeIcon& awesomeIcon) const {
	return d_func()->navigationBar->addPageNode(pageTitle, page, awesomeIcon);
}

UVNavigationType::NodeOperateReturnType CUVMainWindow::addPageNode(const QString& pageTitle, QWidget* page, const QString& targetExpanderKey, const UVIcon::CUVAweSomeIcon& awesomeIcon) const {
	return d_func()->navigationBar->addPageNode(pageTitle, page, targetExpanderKey, awesomeIcon);
}

UVNavigationType::NodeOperateReturnType CUVMainWindow::addPageNode(const QString& pageTitle, QWidget* page, const int keyPoints, const UVIcon::CUVAweSomeIcon& awesomeIcon) const {
	return d_func()->navigationBar->addPageNode(pageTitle, page, keyPoints, awesomeIcon);
}

UVNavigationType::NodeOperateReturnType CUVMainWindow::addPageNode(const QString& pageTitle, QWidget* page, const QString& targetExpanderKey, const int keyPoints, const UVIcon::CUVAweSomeIcon& awesomeIcon) const {
	return d_func()->navigationBar->addPageNode(pageTitle, page, targetExpanderKey, keyPoints, awesomeIcon);
}

UVNavigationType::NodeOperateReturnType CUVMainWindow::addFooterNode(const QString& footerTitle, QString& footerKey, const int keyPoints, const UVIcon::CUVAweSomeIcon& awesomeIcon) const {
	return d_func()->navigationBar->addFooterNode(footerTitle, footerKey, keyPoints, awesomeIcon);
}

UVNavigationType::NodeOperateReturnType CUVMainWindow::addFooterNode(const QString& footerTitle, QWidget* page, QString& footerKey, const int keyPoints, const UVIcon::CUVAweSomeIcon& awesomeIcon) const {
	return d_func()->navigationBar->addFooterNode(footerTitle, page, footerKey, keyPoints, awesomeIcon);
}

void CUVMainWindow::setNodeKeyPoints(const QString& nodeKey, const int keyPoints) {
	d_func()->navigationBar->setNodeKeyPoints(nodeKey, keyPoints);
}

int CUVMainWindow::getNodeKeyPoints(const QString& nodeKey) const {
	return d_func()->navigationBar->getNodeKeyPoints(nodeKey);
}

void CUVMainWindow::navigation(const QString& pageKey) {
	d_func()->navigationBar->navigation(pageKey);
}

void CUVMainWindow::setWindowButtonFlag(const UVAppBarType::ButtonFlag& buttonFlag, const bool isEnable) {
	d_func()->appBar->setWindowButtonFlag(buttonFlag, isEnable);
}

void CUVMainWindow::setWindowButtonFlags(const UVAppBarType::ButtonFlags& buttonFlags) {
	d_func()->appBar->setWindowButtonFlags(buttonFlags);
}

UVAppBarType::ButtonFlags CUVMainWindow::getWindowButtonFlags() const {
	return d_func()->appBar->getWindowButtonFlags();
}

bool CUVMainWindow::insertWidgetBeforeButtonInAppBar(QWidget* widget, const UVAppBarType::ButtonFlag& flag) {
	return d_func()->appBar->insertWidgetBeforeButton(widget, flag);
}

bool CUVMainWindow::insertWidgetBeforeWidgetInAppBar(QWidget* widget, QWidget* targetWidget) {
	return d_func()->appBar->insertWidgetBeforeWidget(widget, targetWidget);
}

bool CUVMainWindow::insertWidgetBeforeLayoutInAppBar(QWidget* widget, QLayout* targetLayout) {
	return d_func()->appBar->insertWidgetBeforeLayout(widget, targetLayout);
}

bool CUVMainWindow::insertLayoutBeforeButtonInAppBar(QLayout* layout, const UVAppBarType::ButtonFlag& flag) {
	return d_func()->appBar->insertLayoutBeforeButton(layout, flag);
}

bool CUVMainWindow::insertLayoutBeforeWidgetInAppBar(QLayout* layout, QWidget* targetWidget) {
	return d_func()->appBar->insertLayoutBeforeWidget(layout, targetWidget);
}

bool CUVMainWindow::insertLayoutBeforeLayoutInAppBar(QLayout* layout, QLayout* targetLayout) {
	return d_func()->appBar->insertLayoutBeforeLayout(layout, targetLayout);
}

void CUVMainWindow::closeWindow() {
	Q_D(CUVMainWindow);

	d->isWindowClosing = true;
	d->appBar->closeWindow();
}

void CUVMainWindow::setIsStayTop(const bool isStayTop) {
	d_func()->appBar->setIsStayTop(isStayTop);
	Q_EMIT sigIsStayTopChanged();
}

bool CUVMainWindow::getIsStayTop() const {
	return d_func()->appBar->getIsStayTop();
}

void CUVMainWindow::setIsFixedSize(const bool isFixedSize) {
	d_func()->appBar->setIsFixedSize(isFixedSize);
	Q_EMIT sigIsFixedSizeChanged();
}

bool CUVMainWindow::getIsFixedSize() const {
	return d_func()->appBar->getIsFixedSize();
}

void CUVMainWindow::setIsDefaultClosed(const bool isDefaultClosed) {
	d_func()->appBar->setIsDefaultClosed(isDefaultClosed);
	Q_EMIT sigIsDefaultClosedChanged();
}

bool CUVMainWindow::getIsDefaultClosed() const {
	return d_func()->appBar->getIsDefaultClosed();
}

void CUVMainWindow::setIsCenterStackedWidgetTransparent(const bool isTransparent) {
	d_func()->centerStackedWidget->setIsTransparent(isTransparent);
	Q_EMIT sigIsCenterStackedWidgetTransparentChanged();
}

bool CUVMainWindow::getIsCenterStackedWidgetTransparent() const {
	return d_func()->centerStackedWidget->getIsTransparent();
}

void CUVMainWindow::setAppBarHeight(const int appBarHeight) {
	d_func()->appBar->setAppBarHeight(appBarHeight);
	Q_EMIT sigAppBarHeightChanged();
}

int CUVMainWindow::getAppBarHeight() const {
	return d_func()->appBar->getAppBarHeight();
}

void CUVMainWindow::setAppBarBackgroundColor(const QColor& appBarBackgroundColor) {
	d_func()->appBar->setBackgroundColor(appBarBackgroundColor);
}

QColor CUVMainWindow::getAppBarBackgroundColor() const {
	return d_func()->appBar->getBackgroundColor();
}

void CUVMainWindow::setCustomWidgetMaximumWidth(const int width) {
	d_func()->appBar->setCustomWidgetMaximumWidth(width);
	Q_EMIT sigCustomWidgetMaximumWidthChanged();
}

int CUVMainWindow::getCustomWidgetMaximumWidth() const {
	return d_func()->appBar->getCustomWidgetMaximumWidth();
}

void CUVMainWindow::setThemeChangeTime(const int time) {
	d_func()->themeChangeTime = time;
	Q_EMIT sigThemeChangeTimeChanged();
}

int CUVMainWindow::getThemeChangeTime() const {
	return d_func()->themeChangeTime;
}

void CUVMainWindow::setNavigationDisplayMode(const UVNavigationType::NavigationDisplayMode& mode) {
	d_func()->navigationDisplayMode = mode;
	Q_EMIT sigNavigationDisplayModeChanged();
}

UVNavigationType::NavigationDisplayMode CUVMainWindow::getNavigationDisplayMode() const {
	return d_func()->navigationDisplayMode;
}

bool CUVMainWindow::eventFilter(QObject* watched, QEvent* event) {
	Q_D(CUVMainWindow);

	switch (event->type()) {
		case QEvent::Resize: {
			d->doNavigationDisplayModeChange();
			break;
		}
		default: {
			break;
		}
	}

	return QMainWindow::eventFilter(watched, event);
}

QMenu* CUVMainWindow::createPopupMenu() {
	CUVMenu* menu{ nullptr };
	if (const QList<QDockWidget*> dockWidgets = this->findChildren<QDockWidget*>(); !dockWidgets.isEmpty()) {
		menu = new CUVMenu(this);
		for (int i = 0; i < dockWidgets.count(); i++) {
			if (const QDockWidget* dockWidget = dockWidgets.at(i); dockWidget->parentWidget() == this) {
				menu->addAction(dockWidget->toggleViewAction());
			}
		}
		menu->addSeparator();
	}

	if (const QList<QToolBar*> toolBars = this->findChildren<QToolBar*>(); !toolBars.isEmpty()) {
		if (!menu) {
			menu = new CUVMenu(this);
		}

		for (int i = 0; i < toolBars.count(); i++) {
			if (const QToolBar* toolBar = toolBars.at(i); toolBar->parentWidget() == this) {
				menu->addAction(toolBar->toggleViewAction());
			}
		}
	}

	if (menu) {
		menu->setMenuItemHeight(28);
	}

	return menu;
}
