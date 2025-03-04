#include "almainwindow.hpp"

#include <QDockWidget>
#include <QEvent>
#include <QHBoxLayout>
#include <QPropertyAnimation>
#include <QScreen>
#include <QTimer>
#include <QtMath>
#include <QToolBar>
#include <QVariant>

#include "alapplication.hpp"
#include "alcenterstackedwidget.hpp"
#include "aleventbus.hpp"
#include "almainwindowstyle.hpp"
#include "almainwindow_p.hpp"
#include "almenu.hpp"
#include "alnavigationbar.hpp"
#include "alnavigationrouter.hpp"
#include "althemeanimationwidget.hpp"
#include "althememanager.hpp"

/**
 * @brief \namespace AL
 */
namespace AL {
/**
 * @brief \class CALMainWindowPrivate
 * Internal class for CALMainWindow
 * @param q pointer to the public class
 * @param parent pointer to the parent class
 */
CALMainWindowPrivate::CALMainWindowPrivate(CALMainWindow* q, QObject* parent): QObject(parent), q_ptr(q) {
	isWindowClosing = false;
	isInitFinished = false;
	isNavigationDisplayModeChanged = false;
	isNavigationEnable = true;
	isNavigationBarExpanded = false;
	isWMClickedAnimatinoFinished = true;
	contentsMargins = 5;
	navigationTargetIndex = 0;
	currentNavigationBarDisplayMode = ALNavigationType::Maximal;
}

CALMainWindowPrivate::~CALMainWindowPrivate() = default;

void CALMainWindowPrivate::slotNavigationButtonClicked() {
	if (isWMClickedAnimatinoFinished) {
		isNavigationDisplayModeChanged = false;
		resetWindowLayout(true);
		navigationBar->setIsTransparent(false);
		navigationBar->setDisplayMode(ALNavigationType::Maximal, false);
		navigationBar->move(-navigationBar->width(), navigationBar->pos().y());
		navigationBar->resize(navigationBar->width(), centerStackedWidget->height() + 1);
		const auto navigationMoveAnimation = new QPropertyAnimation(navigationBar, "pos");
		connect(navigationMoveAnimation, &QPropertyAnimation::finished, this, [this]() { isNavigationBarExpanded = true; });
		navigationMoveAnimation->setEasingCurve(QEasingCurve::InOutSine);
		navigationMoveAnimation->setDuration(300);
		navigationMoveAnimation->setStartValue(navigationBar->pos());
		navigationMoveAnimation->setEndValue(QPoint(0, 0));
		navigationMoveAnimation->start(QAbstractAnimation::DeleteWhenStopped);
		isWMClickedAnimatinoFinished = false;
	}
}

void CALMainWindowPrivate::slotThemeReadyChange() {
	Q_Q(CALMainWindow);

	// 主题变更绘制窗口
	appBar->setIsOnlyAllowMinAndClose(true);
	if (!themeAnimationWidget) {
		const QPoint centerPos = q->rect().center();
		themeAnimationWidget = new CALThemeAnimationWidget(q);
		connect(themeAnimationWidget, &CALThemeAnimationWidget::sigAnimationFinished, this, [this]() {
			appBar->setIsOnlyAllowMinAndClose(false);
			themeAnimationWidget = nullptr;
		});
		themeAnimationWidget->move(0, 0);
		themeAnimationWidget->setOldWindowImage(q->grab(q->rect()).toImage());
		ALTheme->setThemeMode(ALTheme->getThemeMode() == ALThemeType::Light ? ALThemeType::Dark : ALThemeType::Light);
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

void CALMainWindowPrivate::slotDisplayModeChanged() {
	currentNavigationBarDisplayMode = navigationDisplayMode;
	if (!isNavigationEnable) {
		return;
	}

	switch (currentNavigationBarDisplayMode) {
		case ALNavigationType::Auto: {
			appBar->setWindowButtonFlag(ALAppBarType::NavigationButtonHint, false);
			doNavigationDisplayModeChange();
			break;
		}
		case ALNavigationType::Minimal: {
			navigationBar->setDisplayMode(ALNavigationType::Minimal, true);
			appBar->setWindowButtonFlag(ALAppBarType::NavigationButtonHint);
			break;
		}
		case ALNavigationType::Compact: {
			navigationBar->setDisplayMode(ALNavigationType::Compact, true);
			appBar->setWindowButtonFlag(ALAppBarType::NavigationButtonHint, false);
			break;
		}
		case ALNavigationType::Maximal: {
			navigationBar->setDisplayMode(ALNavigationType::Maximal, true);
			appBar->setWindowButtonFlag(ALAppBarType::NavigationButtonHint, false);
			break;
		}
		default: {
			break;
		}
	}
}

void CALMainWindowPrivate::slotThemeModeChanged(const ALThemeType::ThemeMode& mode) {
	Q_Q(CALMainWindow);

	themeMode = mode;
	if (!alApp->getIsEnableMica()) {
		QPalette palette = q->palette();
		palette.setBrush(QPalette::Window, ALThemeColor(themeMode, ALThemeType::WindowBase));
		q->setPalette(palette);
	}
}

void CALMainWindowPrivate::slotNavigationNodeClicked(const ALNavigationType::NavigationNodeType& nodeType, const QString& nodeKey) {
	QWidget* page = routeMap.value(nodeKey);
	if (!page) {
		return;
	}
	const int nodeIndex = centerStackedWidget->indexOf(page);
	if (navigationTargetIndex == nodeIndex || centerStackedWidget->count() <= nodeIndex) {
		return;
	}

	navigationTargetIndex = nodeIndex;
	QTimer::singleShot(180, this, [this, nodeIndex]() {
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

void CALMainWindowPrivate::slotNavigationNodeAdded(const ALNavigationType::NavigationNodeType& nodeType, const QString& nodeKey, QWidget* page) {
	if (nodeType == ALNavigationType::PageNode) {
		routeMap.insert(nodeKey, page);
		centerStackedWidget->addWidget(page);
	} else {
		routeMap.insert(nodeKey, page);
		if (page) {
			centerStackedWidget->addWidget(page);
		}
	}
}

void CALMainWindowPrivate::slotNavigationNodeRemoved(const ALNavigationType::NavigationNodeType& nodeType, const QString& nodeKey) {
	Q_Q(CALMainWindow);

	if (!routeMap.contains(nodeKey)) {
		return;
	}

	QWidget* page = routeMap.value(nodeKey);
	routeMap.remove(nodeKey);
	centerStackedWidget->removeWidget(page);
	if (const QWidget* currentWidget = centerStackedWidget->currentWidget()) {
		q->navigation(currentWidget->property("CALPageKey").toString());
	}
}

void CALMainWindowPrivate::invokableWMWindowClickedEvent(const QVariantMap& data) {
	if (const auto actionType = data.value("WMClickType").value<ALAppBarType::WMMouseActionType>();
		actionType & ALAppBarType::WMLBUTTONDBLCLK || actionType & ALAppBarType::WMLBUTTONUP || actionType & ALAppBarType::WMNCLBUTTONDOWN) {
		if (CALApplication::containsCursorToItem(navigationBar)) {
			return;
		}

		if (isNavigationBarExpanded) {
			const auto navigationMoveAnimation = new QPropertyAnimation(navigationBar, "pos");
			connect(navigationMoveAnimation, &QPropertyAnimation::valueChanged, this, [this, navigationMoveAnimation]() {
				if (isNavigationDisplayModeChanged) {
					navigationBar->setIsTransparent(true);
					isWMClickedAnimatinoFinished = true;
					resetWindowLayout(false);
					navigationMoveAnimation->deleteLater();
				}
			});
			connect(navigationMoveAnimation, &QPropertyAnimation::finished, this, [this]() {
				navigationBar->setIsTransparent(true);
				if (!isNavigationDisplayModeChanged) {
					navigationBar->setDisplayMode(ALNavigationType::Minimal, false);
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

qreal CALMainWindowPrivate::distance(const QPoint& point1, const QPoint& point2) {
	return std::sqrt((point1.x() - point2.x()) * (point1.x() - point2.x()) + (point1.y() - point2.y()) * (point1.y() - point2.y()));
}

void CALMainWindowPrivate::resetWindowLayout(const bool isAnimation) const {
	if (isAnimation) {
		while (centerHLayout->count() > 0) {
			centerHLayout->takeAt(0);
		}
	} else {
		if (centerHLayout->count() == 0) {
			navigationBar->setDisplayMode(ALNavigationType::Minimal, false);
			centerHLayout->addWidget(navigationBar);
			centerHLayout->addWidget(centerStackedWidget);
		}
	}
}

void CALMainWindowPrivate::doNavigationDisplayModeChange() {
	Q_Q(CALMainWindow);

	if (isWindowClosing || !isNavigationEnable || !isInitFinished) {
		return;
	}

	if (navigationDisplayMode == ALNavigationType::Minimal) {
		resetWindowLayout(false);
	}
	if (navigationDisplayMode == ALNavigationType::Auto) {
		isNavigationDisplayModeChanged = true;
		resetWindowLayout(false);
		if (const int width = q->centralWidget()->width(); width >= 850 && currentNavigationBarDisplayMode != ALNavigationType::Maximal) {
			navigationBar->setDisplayMode(ALNavigationType::Maximal);
			currentNavigationBarDisplayMode = ALNavigationType::Maximal;
			appBar->setWindowButtonFlag(ALAppBarType::NavigationButtonHint, false);
		} else if (width >= 550 && width < 850 && currentNavigationBarDisplayMode != ALNavigationType::Compact) {
			navigationBar->setDisplayMode(ALNavigationType::Compact);
			currentNavigationBarDisplayMode = ALNavigationType::Compact;
			appBar->setWindowButtonFlag(ALAppBarType::NavigationButtonHint, false);
		} else if (width < 550 && currentNavigationBarDisplayMode != ALNavigationType::Minimal) {
			navigationBar->setDisplayMode(ALNavigationType::Minimal);
			currentNavigationBarDisplayMode = ALNavigationType::Minimal;
			appBar->setWindowButtonFlag(ALAppBarType::NavigationButtonHint);
		}
		isNavigationBarExpanded = false;
	}
}

/**
 * \class CALMainWindow
 * @param parent pointer to the parent class
 */
CALMainWindow::CALMainWindow(QWidget* parent): QMainWindow(parent), d_ptr(new CALMainWindowPrivate(this, this)) {
	Q_D(CALMainWindow);

	setProperty("CALBaseClassName", "CALMainWindow");
	resize(1020, 680);
	d->themeChangeTime = 700;
	d->navigationDisplayMode = ALNavigationType::Auto;
	connect(this, &CALMainWindow::sigNavigationDisplayModeChanged, d, &CALMainWindowPrivate::slotDisplayModeChanged);

	/// appBar
	d->appBar = new CALAppBar(this);
	connect(d->appBar, &CALAppBar::sigRouteBackButtonClicked, this, []() { CALNavigationRouter::instance()->navigationRouteBack(); });
	connect(d->appBar, &CALAppBar::sigCloseButtonClicked, this, &CALMainWindow::sigCloseButtonClicked);
	connect(d->appBar, &CALAppBar::sigNavigationButtonClicked, d, &CALMainWindowPrivate::slotNavigationButtonClicked);
	connect(d->appBar, &CALAppBar::sigThemeChangeButtonClicked, d, &CALMainWindowPrivate::slotThemeReadyChange);

	/// navigationBar
	d->navigationBar = new CALNavigationBar(this);
	// 返回按钮状态变更
	connect(CALNavigationRouter::instance(), &CALNavigationRouter::sigNavigationRouterStateChanged, this, [d](const bool isEnable) { d->appBar->setRouteBackButtonEnable(isEnable); });
	// 转发用户卡片点击信号
	connect(d->navigationBar, &CALNavigationBar::sigUserInfoCardClicked, this, &CALMainWindow::sigUserInfoCardClicked);
	// 转发点击信号
	connect(d->navigationBar, &CALNavigationBar::sigNavigationNodeClicked, this, &CALMainWindow::sigNavigationNodeClicked);
	// 跳转处理
	connect(d->navigationBar, &CALNavigationBar::sigNavigationNodeClicked, d, &CALMainWindowPrivate::slotNavigationNodeClicked);
	// 新增窗口
	connect(d->navigationBar, &CALNavigationBar::sigNavigationNodeAdded, d, &CALMainWindowPrivate::slotNavigationNodeAdded);
	// 删除窗口
	connect(d->navigationBar, &CALNavigationBar::sigNavigationNodeRemoved, d, &CALMainWindowPrivate::slotNavigationNodeRemoved);

	/// center stacked widget
	d->centerStackedWidget = new CALCenterStackedWidget(this);
	d->centerStackedWidget->setContentsMargins(0, 0, 0, 0);
	const auto centralWidget = new QWidget(this);
	d->centerHLayout = new QHBoxLayout(centralWidget);
	d->centerHLayout->setSpacing(0);
	d->centerHLayout->addWidget(d->navigationBar);
	d->centerHLayout->addWidget(d->centerStackedWidget);
	d->centerHLayout->setContentsMargins(d->contentsMargins, 0, 0, 0);

	/// event filter
	d->focusEvent = new CALEvent("WMWindowClicked", "invokableWMWindowClickedEvent", d);
	d->focusEvent->registerAndInit();

	/// theme
	d->themeMode = ALTheme->getThemeMode();
	connect(ALTheme, &CALThemeManager::sigThemeModeChanged, d, &CALMainWindowPrivate::slotThemeModeChanged);
	d->isInitFinished = true;
	setCentralWidget(centralWidget);
	centralWidget->installEventFilter(this);

	setObjectName("CALMainWindow");
	setStyleSheet("#CALMainWindow { background-color: transparent; }");
	setStyle(new CALMainWindowStyle(style()));

	/// Delay rendering
	QTimer::singleShot(1, this, [this, d]() {
		QPalette palette = this->palette();
		palette.setBrush(QPalette::Window, ALThemeColor(d->themeMode, ALThemeType::WindowBase));
		this->setPalette(palette);
	});
	alApp->syncMica(this);
	connect(alApp, &CALApplication::sigIsEnableMicaChanged, this, [d]() { d->slotThemeModeChanged(d->themeMode); });
}

AL_TAKEOVER_NATIVEEVENT_CPP(CALMainWindow, d_func()->appBar)

CALMainWindow::~CALMainWindow() = default;

void CALMainWindow::moveToCenter() {
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

void CALMainWindow::setCustomWidget(const ALAppBarType::CustomArea& customArea, QWidget* customWidget) {
	d_func()->appBar->setCustomWidget(customArea, customWidget);
	Q_EMIT sigCustomWidgetChanged();
}

QWidget* CALMainWindow::getCustomWidget() const {
	return d_func()->appBar->getCustomWidget();
}

void CALMainWindow::setIsNavigationBarEnable(const bool isEnable) {
	Q_D(CALMainWindow);

	d->isNavigationEnable = isEnable;
	d->navigationBar->setVisible(isEnable);
	d->centerHLayout->setContentsMargins(isEnable ? d->contentsMargins : 0, 0, 0, 0);
	d->centerStackedWidget->setIsHasRadius(isEnable);
}

bool CALMainWindow::getIsNavigationBarEnable() const {
	return d_func()->isNavigationEnable;
}

void CALMainWindow::setUserInfoCardVisible(const bool isVisible) {
	d_func()->navigationBar->setUserInfoCardVisible(isVisible);
}

void CALMainWindow::setUserInfoCardPixmap(const QPixmap& pix) {
	d_func()->navigationBar->setUserInfoCardPixmap(pix);
}

void CALMainWindow::setUserInfoCardTitle(const QString& title) {
	d_func()->navigationBar->setUserInfoCardTitle(title);
}

void CALMainWindow::setUserInfoCardSubTitle(const QString& subTitle) {
	d_func()->navigationBar->setUserInfoCardSubTitle(subTitle);
}

ALNavigationType::NodeOperateReturnType CALMainWindow::addExpanderNode(const QString& expanderTitle, QString& expanderKey, const ALIcon::AweSomeIcon& awesomeIcon) const {
	return d_func()->navigationBar->addExpanderNode(expanderTitle, expanderKey, awesomeIcon);
}

ALNavigationType::NodeOperateReturnType CALMainWindow::addExpanderNode(const QString& expanderTitle, QString& expanderKey, const ALIcon::FluentIcon& fluentIcon) const {
	return d_func()->navigationBar->addExpanderNode(expanderTitle, expanderKey, fluentIcon);
}

ALNavigationType::NodeOperateReturnType CALMainWindow::addExpanderNode(const QString& expanderTitle, QString& expanderKey, const QString& targetExpanderKey, const ALIcon::AweSomeIcon& awesomeIcon) const {
	return d_func()->navigationBar->addExpanderNode(expanderTitle, expanderKey, targetExpanderKey, awesomeIcon);
}

ALNavigationType::NodeOperateReturnType CALMainWindow::addExpanderNode(const QString& expanderTitle, QString& expanderKey, const QString& targetExpanderKey, const ALIcon::FluentIcon& fluentIcon) const {
	return d_func()->navigationBar->addExpanderNode(expanderTitle, expanderKey, targetExpanderKey, fluentIcon);
}

ALNavigationType::NodeOperateReturnType CALMainWindow::addPageNode(const QString& pageTitle, QWidget* page, const ALIcon::AweSomeIcon& awesomeIcon) const {
	return d_func()->navigationBar->addPageNode(pageTitle, page, awesomeIcon);
}

ALNavigationType::NodeOperateReturnType CALMainWindow::addPageNode(const QString& pageTitle, QWidget* page, const ALIcon::FluentIcon& fluentIcon) const {
	return d_func()->navigationBar->addPageNode(pageTitle, page, fluentIcon);
}

ALNavigationType::NodeOperateReturnType CALMainWindow::addPageNode(const QString& pageTitle, QWidget* page, const QString& targetExpanderKey, const ALIcon::AweSomeIcon& awesomeIcon) const {
	return d_func()->navigationBar->addPageNode(pageTitle, page, targetExpanderKey, awesomeIcon);
}

ALNavigationType::NodeOperateReturnType CALMainWindow::addPageNode(const QString& pageTitle, QWidget* page, const QString& targetExpanderKey, const ALIcon::FluentIcon& fluentIcon) const {
	return d_func()->navigationBar->addPageNode(pageTitle, page, targetExpanderKey, fluentIcon);
}

ALNavigationType::NodeOperateReturnType CALMainWindow::addPageNode(const QString& pageTitle, QWidget* page, const int keyPoints, const ALIcon::AweSomeIcon& awesomeIcon) const {
	return d_func()->navigationBar->addPageNode(pageTitle, page, keyPoints, awesomeIcon);
}

ALNavigationType::NodeOperateReturnType CALMainWindow::addPageNode(const QString& pageTitle, QWidget* page, const int keyPoints, const ALIcon::FluentIcon& fluentIcon) const {
	return d_func()->navigationBar->addPageNode(pageTitle, page, keyPoints, fluentIcon);
}

ALNavigationType::NodeOperateReturnType CALMainWindow::addPageNode(const QString& pageTitle, QWidget* page, const QString& targetExpanderKey, const int keyPoints, const ALIcon::AweSomeIcon& awesomeIcon) const {
	return d_func()->navigationBar->addPageNode(pageTitle, page, targetExpanderKey, keyPoints, awesomeIcon);
}

ALNavigationType::NodeOperateReturnType CALMainWindow::addPageNode(const QString& pageTitle, QWidget* page, const QString& targetExpanderKey, const int keyPoints, const ALIcon::FluentIcon& fluentIcon) const {
	return d_func()->navigationBar->addPageNode(pageTitle, page, targetExpanderKey, keyPoints, fluentIcon);
}

ALNavigationType::NodeOperateReturnType CALMainWindow::addFooterNode(const QString& footerTitle, QString& footerKey, const int keyPoints, const ALIcon::AweSomeIcon& awesomeIcon) const {
	return d_func()->navigationBar->addFooterNode(footerTitle, footerKey, keyPoints, awesomeIcon);
}

ALNavigationType::NodeOperateReturnType CALMainWindow::addFooterNode(const QString& footerTitle, QString& footerKey, const int keyPoints, const ALIcon::FluentIcon& fluentIcon) const {
	return d_func()->navigationBar->addFooterNode(footerTitle, footerKey, keyPoints, fluentIcon);
}

ALNavigationType::NodeOperateReturnType CALMainWindow::addFooterNode(const QString& footerTitle, QWidget* page, QString& footerKey, const int keyPoints, const ALIcon::AweSomeIcon& awesomeIcon) const {
	return d_func()->navigationBar->addFooterNode(footerTitle, page, footerKey, keyPoints, awesomeIcon);
}

ALNavigationType::NodeOperateReturnType CALMainWindow::addFooterNode(const QString& footerTitle, QWidget* page, QString& footerKey, const int keyPoints, const ALIcon::FluentIcon& fluentIcon) const {
	return d_func()->navigationBar->addFooterNode(footerTitle, page, footerKey, keyPoints, fluentIcon);
}

bool CALMainWindow::removeNavigationNode(const QString& nodeKey) {
	return d_func()->navigationBar->removeNavigationNode(nodeKey);
}

void CALMainWindow::setNodeKeyPoints(const QString& nodeKey, const int keyPoints) {
	d_func()->navigationBar->setNodeKeyPoints(nodeKey, keyPoints);
}

int CALMainWindow::getNodeKeyPoints(const QString& nodeKey) const {
	return d_func()->navigationBar->getNodeKeyPoints(nodeKey);
}

void CALMainWindow::navigation(const QString& pageKey) {
	d_func()->navigationBar->navigation(pageKey);
}

void CALMainWindow::setWindowButtonFlag(const ALAppBarType::ButtonFlag& buttonFlag, const bool isEnable) {
	d_func()->appBar->setWindowButtonFlag(buttonFlag, isEnable);
}

void CALMainWindow::setWindowButtonFlags(const ALAppBarType::ButtonFlags& buttonFlags) {
	d_func()->appBar->setWindowButtonFlags(buttonFlags);
}

ALAppBarType::ButtonFlags CALMainWindow::getWindowButtonFlags() const {
	return d_func()->appBar->getWindowButtonFlags();
}

bool CALMainWindow::insertWidgetBeforeButtonInAppBar(QWidget* widget, const ALAppBarType::ButtonFlag& flag) {
	return d_func()->appBar->insertWidgetBeforeButton(widget, flag);
}

bool CALMainWindow::insertWidgetBeforeWidgetInAppBar(QWidget* widget, QWidget* targetWidget) {
	return d_func()->appBar->insertWidgetBeforeWidget(widget, targetWidget);
}

bool CALMainWindow::insertWidgetBeforeLayoutInAppBar(QWidget* widget, QLayout* targetLayout) {
	return d_func()->appBar->insertWidgetBeforeLayout(widget, targetLayout);
}

bool CALMainWindow::insertLayoutBeforeButtonInAppBar(QLayout* layout, const ALAppBarType::ButtonFlag& flag) {
	return d_func()->appBar->insertLayoutBeforeButton(layout, flag);
}

bool CALMainWindow::insertLayoutBeforeWidgetInAppBar(QLayout* layout, QWidget* targetWidget) {
	return d_func()->appBar->insertLayoutBeforeWidget(layout, targetWidget);
}

bool CALMainWindow::insertLayoutBeforeLayoutInAppBar(QLayout* layout, QLayout* targetLayout) {
	return d_func()->appBar->insertLayoutBeforeLayout(layout, targetLayout);
}

void CALMainWindow::closeWindow() {
	Q_D(CALMainWindow);

	d->isWindowClosing = true;
	d->appBar->closeWindow();
}

void CALMainWindow::setIsStayTop(const bool isStayTop) {
	d_func()->appBar->setIsStayTop(isStayTop);
	Q_EMIT sigIsStayTopChanged();
}

bool CALMainWindow::getIsStayTop() const {
	return d_func()->appBar->getIsStayTop();
}

void CALMainWindow::setIsFixedSize(const bool isFixedSize) {
	d_func()->appBar->setIsFixedSize(isFixedSize);
	Q_EMIT sigIsFixedSizeChanged();
}

bool CALMainWindow::getIsFixedSize() const {
	return d_func()->appBar->getIsFixedSize();
}

void CALMainWindow::setIsDefaultClosed(const bool isDefaultClosed) {
	d_func()->appBar->setIsDefaultClosed(isDefaultClosed);
	Q_EMIT sigIsDefaultClosedChanged();
}

bool CALMainWindow::getIsDefaultClosed() const {
	return d_func()->appBar->getIsDefaultClosed();
}

void CALMainWindow::setIsCenterStackedWidgetTransparent(const bool isTransparent) {
	d_func()->centerStackedWidget->setIsTransparent(isTransparent);
	Q_EMIT sigIsCenterStackedWidgetTransparentChanged();
}

bool CALMainWindow::getIsCenterStackedWidgetTransparent() const {
	return d_func()->centerStackedWidget->getIsTransparent();
}

void CALMainWindow::setAppBarHeight(const int appBarHeight) {
	d_func()->appBar->setAppBarHeight(appBarHeight);
	Q_EMIT sigAppBarHeightChanged();
}

int CALMainWindow::getAppBarHeight() const {
	return d_func()->appBar->getAppBarHeight();
}

void CALMainWindow::setAppBarBackgroundColor(const QColor& appBarBackgroundColor) {
	d_func()->appBar->setBackgroundColor(appBarBackgroundColor);
}

QColor CALMainWindow::getAppBarBackgroundColor() const {
	return d_func()->appBar->getBackgroundColor();
}

void CALMainWindow::setCustomWidgetMaximumWidth(const int width) {
	d_func()->appBar->setCustomWidgetMaximumWidth(width);
	Q_EMIT sigCustomWidgetMaximumWidthChanged();
}

int CALMainWindow::getCustomWidgetMaximumWidth() const {
	return d_func()->appBar->getCustomWidgetMaximumWidth();
}

void CALMainWindow::setThemeChangeTime(const int time) {
	d_func()->themeChangeTime = time;
	Q_EMIT sigThemeChangeTimeChanged();
}

int CALMainWindow::getThemeChangeTime() const {
	return d_func()->themeChangeTime;
}

void CALMainWindow::setNavigationDisplayMode(const ALNavigationType::NavigationDisplayMode& mode) {
	d_func()->navigationDisplayMode = mode;
	Q_EMIT sigNavigationDisplayModeChanged(mode);
}

ALNavigationType::NavigationDisplayMode CALMainWindow::getNavigationDisplayMode() const {
	return d_func()->navigationDisplayMode;
}

void CALMainWindow::setThemeMode(const ALThemeType::ThemeMode& mode) {
	ALTheme->setThemeMode(mode);
}

ALThemeType::ThemeMode CALMainWindow::getThemeMode() {
	return ALTheme->getThemeMode();
}

bool CALMainWindow::eventFilter(QObject* watched, QEvent* event) {
	Q_D(CALMainWindow);

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

QMenu* CALMainWindow::createPopupMenu() {
	CALMenu* menu{ nullptr };
	if (const QList<QDockWidget*> dockWidgets = this->findChildren<QDockWidget*>(); !dockWidgets.isEmpty()) {
		menu = new CALMenu(this);
		for (int i = 0; i < dockWidgets.count(); ++i) {
			if (const QDockWidget* dockWidget = dockWidgets.at(i); dockWidget->parentWidget() == this) {
				menu->addAction(dockWidget->toggleViewAction());
			}
		}
		menu->addSeparator();
	}

	if (const QList<QToolBar*> toolBars = this->findChildren<QToolBar*>(); !toolBars.isEmpty()) {
		if (!menu) {
			menu = new CALMenu(this);
		}

		for (int i = 0; i < toolBars.count(); ++i) {
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
}
