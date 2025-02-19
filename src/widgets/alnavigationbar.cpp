#include "alnavigationbar.hpp"

#include <QDebug>
#include <QLayout>
#include <QPainter>
#include <QPainterPath>
#include <QPropertyAnimation>

#include "altoolbutton.hpp"
#include "albaselistview.hpp"
#include "alcustomwidget.hpp"
#include "alfooterdelegate.hpp"
#include "alfootermodel.hpp"
#include "aliconbutton.hpp"
#include "alinteractivecard.hpp"
#include "almenu.hpp"
#include "alnavigationbar_p.hpp"
#include "alnavigationmodel.hpp"
#include "alnavigationnode.hpp"
#include "alnavigationrouter.hpp"
#include "alnavigationview.hpp"
#include "alsuggestbox.hpp"
#include "althememanager.hpp"

/**
 * @brief \namespace AL
 */
namespace AL {
/**
 * @brief \class CALNavigationBarPrivate
 * Internal class for CALNavigationBar
 * @param q pointer to the public class
 * @param parent pointer to the parent class
 */
CALNavigationBarPrivate::CALNavigationBarPrivate(CALNavigationBar* q, QObject* parent): QObject(parent), q_ptr(q) {
	isShowUserCard = true;
	currentDisplayMode = ALNavigationType::Maximal;
}

CALNavigationBarPrivate::~CALNavigationBarPrivate() = default;

void CALNavigationBarPrivate::setNavigationViewWidth(const int width) {
	navigationViewWidth = width;
	Q_EMIT sigNavigationViewWidthChanged();
}

int CALNavigationBarPrivate::getNavigationViewWidth() const {
	return navigationViewWidth;
}

void CALNavigationBarPrivate::slotNavigationButtonClicked() {
	q_func()->setDisplayMode(currentDisplayMode == ALNavigationType::Compact ? ALNavigationType::Maximal : ALNavigationType::Compact);
}

void CALNavigationBarPrivate::slotNavigationOpenNewWindow(const QString& nodeKey) {
	const QMetaObject* meta = mapPageMeta.value(nodeKey);
	if (!meta) {
		return;
	}

	if (const auto widget = qobject_cast<QWidget*>(meta->newInstance())) {
		const auto floatWidget = new CALCustomWidget(q_func());
		floatWidget->setWindowIcon(widget->windowIcon());
		floatWidget->setWindowTitle(widget->windowTitle());
		floatWidget->setCenterWidget(widget);
		floatWidget->show();
	}
}

void CALNavigationBarPrivate::invokableNavigationRouteBack(const QVariantMap& routeData) {
	q_func()->navigation(routeData.value("CALPageKey").toString(), false);
}

void CALNavigationBarPrivate::slotTreeViewClicked(const QModelIndex& index, const bool isLogRoute) {
	Q_Q(CALNavigationBar);

	if (index.isValid()) {
		const auto node = static_cast<CALNavigationNode*>(index.internalPointer());
		if (!node) {
			return;
		}
		if (node->getIsExpanderNode()) {
			if (currentDisplayMode == ALNavigationType::Compact) {
				if (node->getIsHasPageChild()) {
					// 展开菜单
					if (CALMenu* menu = mapCompactMenu.value(node)) {
						const QPoint nodeTopRight = navigationView->mapToGlobal(navigationView->visualRect(node->getModelIndex()).topRight());
						menu->popup(QPoint(nodeTopRight.x() + 10, nodeTopRight.y()));
					}
				}
			} else {
				if (node->getIsHasChild()) {
					QVariantMap data;
					const bool isExpanded = navigationView->isExpanded(index);
					data.insert(isExpanded ? "Collapse" : "Expand", QVariant::fromValue(node));
					node->setIsExpanded(!isExpanded);
					navigationView->navigationNodeStateChanged(data);
					isExpanded ? navigationView->collapse(index) : navigationView->expand(index);
				}
			}
		} else {
			if (node->getKeyPoints()) {
				node->setKeyPoints(0);
				navigationView->update();
			}
			if (const CALNavigationNode* selectedNode = navigationModel->getSelectedNode(); selectedNode != node) {
				// 记录跳转
				if (isLogRoute) {
					QVariantMap routeData{};
					QString pageKey{};
					if (selectedNode) {
						pageKey.append(selectedNode->getNodeKey());
					} else if (footerModel->getSelectedNode()) {
						pageKey.append(footerModel->getSelectedNode()->getNodeKey());
					}

					routeData.insert("CALPageKey", pageKey);
					CALNavigationRouter::instance()->navigationRoute(this, "invokableNavigationRouteBack", routeData);
				}
				Q_EMIT q->sigNavigationNodeClicked(ALNavigationType::PageNode, node->getNodeKey());

				if (footerModel->getSelectedNode()) {
					footerView->clearSelection();
					QVariantMap footerPostData{};
					footerPostData.insert("SelectMarkChanged", true);
					footerPostData.insert("LastSelectedNode", QVariant::fromValue(footerModel->getSelectedNode()));
					footerPostData.insert("SelectedNode", QVariant::fromValue(nullptr));
					footerModel->setSelectedNode(nullptr);
					footerDelegate->navigationNodeStateChange(footerPostData);
				}
				QVariantMap postData{};
				postData.insert("SelectMarkChanged", true);
				postData.insert("LastSelectedNode", QVariant::fromValue(navigationModel->getSelectedExpandedNode() ? navigationModel->getSelectedExpandedNode() : navigationModel->getSelectedNode()));
				if (currentDisplayMode == ALNavigationType::Compact) {
					if (CALNavigationNode* originNode = node->getOriginalNode(); originNode == node) {
						postData.insert("SelectedNode", QVariant::fromValue(node));
					} else {
						if (originNode == navigationModel->getSelectedExpandedNode()) {
							navigationModel->setSelectedNode(node);
							resetNodeSelected();
							return;
						}
						navigationModel->setSelectedExpandedNode(originNode);
						postData.insert("SelectedNode", QVariant::fromValue(originNode));
					}
				} else {
					postData.insert("SelectedNode", QVariant::fromValue(node));
				}
				navigationModel->setSelectedNode(node);
				navigationView->navigationNodeStateChanged(postData);
				if (!node->getIsVisible() && currentDisplayMode != ALNavigationType::Compact) {
					expandSelectedNodeParent();
				}
			}
		}
		resetNodeSelected();
	}
}

void CALNavigationBarPrivate::slotFooterViewClicked(const QModelIndex& index, const bool isLogRoute) {
	Q_Q(CALNavigationBar);

	const auto node = index.data(Qt::UserRole).value<CALNavigationNode*>();
	if (node->getKeyPoints()) {
		node->setKeyPoints(0);
		navigationView->update();
	}
	if (const CALNavigationNode* selectedNode = footerModel->getSelectedNode(); selectedNode != node) {
		// 记录跳转
		if (isLogRoute && node->getIsHasFooterNode()) {
			QVariantMap routaData{};
			QString pageKey{};
			if (selectedNode) {
				pageKey.append(selectedNode->getNodeKey());
			} else if (navigationModel->getSelectedNode()) {
				pageKey.append(navigationModel->getSelectedNode()->getNodeKey());
			}
			routaData.insert("CALPageKey", pageKey);
			CALNavigationRouter::instance()->navigationRoute(this, "invokableNavigationRouteBack", routaData);
		}
		Q_EMIT q->sigNavigationNodeClicked(ALNavigationType::FooterNode, node->getNodeKey());

		if (node->getIsHasFooterNode()) {
			if (navigationModel->getSelectedNode() || navigationModel->getSelectedExpandedNode()) {
				QVariantMap mainPostData{};
				mainPostData.insert("SelectMarkChanged", true);
				mainPostData.insert("LastSelectedNode", QVariant::fromValue(navigationModel->getSelectedExpandedNode() ? navigationModel->getSelectedExpandedNode() : navigationModel->getSelectedNode()));
				mainPostData.insert("SelectedNode", QVariant::fromValue(nullptr));
				navigationView->clearSelection();
				navigationView->navigationNodeStateChanged(mainPostData);
				navigationModel->setSelectedExpandedNode(nullptr);
				navigationModel->setSelectedNode(nullptr);
			}
			footerView->clearSelection();
			footerView->selectionModel()->select(index, QItemSelectionModel::Select);
			QVariantMap postData{};
			postData.insert("SelectMarkChanged", true);
			postData.insert("LastSelectedNode", QVariant::fromValue(footerModel->getSelectedNode()));
			postData.insert("SelectedNode", QVariant::fromValue(node));
			footerDelegate->navigationNodeStateChange(postData);
			footerModel->setSelectedNode(node);
		}
	}
}

void CALNavigationBarPrivate::resetNodeSelected() const {
	navigationView->clearSelection();
	CALNavigationNode* selectedNode = navigationModel->getSelectedNode();
	if (!selectedNode || !selectedNode->getModelIndex().isValid()) {
		return;
	}

	if (selectedNode->getParentNode()->getIsRootNode() || selectedNode->getIsVisible()) {
		navigationView->selectionModel()->select(selectedNode->getModelIndex(), QItemSelectionModel::Select);
		if (navigationModel->getSelectedExpandedNode()) {
			QVariantMap postData{};
			postData.insert("SelectMarkChanged", true);
			postData.insert("LastSelectedNode", QVariant::fromValue(navigationModel->getSelectedExpandedNode()));
			postData.insert("SelectedNode", QVariant::fromValue(selectedNode));
			navigationView->navigationNodeStateChanged(postData);
		}
		navigationModel->setSelectedExpandedNode(nullptr);
	} else {
		CALNavigationNode* parentNode = selectedNode->getParentNode();
		while (parentNode && !parentNode->getParentNode()->getIsRootNode() && !parentNode->getIsVisible()) {
			parentNode = parentNode->getParentNode();
		}
		if (!parentNode) {
			return;
		}
		// 单级节点展开, 收起时 Mark 变化
		if (!navigationModel->getSelectedExpandedNode()) {
			QVariantMap postData{};
			postData.insert("SelectMarkChanged", true);
			postData.insert("LastSelectedNode", QVariant::fromValue(navigationModel->getSelectedNode()));
			postData.insert("SelectedNode", QVariant::fromValue(parentNode));
			navigationView->navigationNodeStateChanged(postData);
		} else {
			// 多级节点
			if (navigationModel->getSelectedExpandedNode() != parentNode) {
				// 同一起源点展开, 收起时 Mark 变化
				if (navigationModel->getSelectedExpandedNode()->getOriginalNode() == parentNode->getOriginalNode()) {
					QVariantMap postData{};
					postData.insert("SelectMarkChanged", true);
					postData.insert("LastSelectedNode", QVariant::fromValue(navigationModel->getSelectedExpandedNode()));
					postData.insert("SelectedNode", QVariant::fromValue(parentNode));
					navigationView->navigationNodeStateChanged(postData);
				}
			}
		}
		navigationModel->setSelectedExpandedNode(parentNode);
		navigationView->selectionModel()->select(parentNode->getModelIndex(), QItemSelectionModel::Select);
	}
}

void CALNavigationBarPrivate::expandSelectedNodeParent() const {
	CALNavigationNode* parentNode = navigationModel->getSelectedNode()->getParentNode();
	while (parentNode && !parentNode->getIsRootNode()) {
		QVariantMap postData{};
		postData.insert("Expand", QVariant::fromValue(parentNode));
		navigationView->navigationNodeStateChanged(postData);
		parentNode->setIsExpanded(true);
		navigationView->expand(parentNode->getModelIndex());
		parentNode = parentNode->getParentNode();
	}
}

void CALNavigationBarPrivate::initNodeModelIndex(const QModelIndex& parentIndex) { // NOLINT
	const int rowCount = navigationModel->rowCount(parentIndex);
	for (int row = 0; row < rowCount; ++row) {
		QModelIndex index = navigationModel->index(row, 0, parentIndex);
		const auto childNode = static_cast<CALNavigationNode*>(index.internalPointer());
		childNode->setModelIndex(index);
		if (navigationModel->hasChildren(index)) {
			initNodeModelIndex(index);
		}
	}
}

void CALNavigationBarPrivate::addStackedPage(QWidget* page, const QString& pageKey) {
	Q_Q(CALNavigationBar);

	if (page) {
		page->setProperty("CALPageKey", pageKey);
	}
	Q_EMIT q->sigNavigationNodeAdded(ALNavigationType::PageNode, pageKey, page);
	const CALNavigationNode* node = navigationModel->getNavigationNode(pageKey);
	QVariantMap suggestData{};
	suggestData.insert("CALNodeType", "Stacked");
	suggestData.insert("CALPageKey", pageKey);
	QString suggestKey{};
	if (node->getIconType() == ALIcon::Awesome) {
		suggestKey = navigationSuggestBox->addSuggestion(node->getAwesomeIcon(), node->getNodeTitle(), suggestData);
	} else {
		suggestKey = navigationSuggestBox->addSuggestion(node->getFluentIcon(), node->getNodeTitle(), suggestData);
	}
	mapSuggestKey.insert(pageKey, suggestKey);
}

void CALNavigationBarPrivate::addFooterPage(QWidget* page, const QString& footKey) {
	Q_Q(CALNavigationBar);

	if (page) {
		page->setProperty("CALPageKey", footKey);
	}
	Q_EMIT q->sigNavigationNodeAdded(ALNavigationType::FooterNode, footKey, page);
	footerView->setFixedHeight(40 * footerModel->getFooterNodeCount());
	const CALNavigationNode* node = footerModel->getNavigationNode(footKey);
	QVariantMap suggestData{};
	suggestData.insert("CALNodeType", "Footer");
	suggestData.insert("CALPageKey", footKey);
	QString suggestKey{};
	if (node->getIconType() == ALIcon::Awesome) {
		suggestKey = navigationSuggestBox->addSuggestion(node->getAwesomeIcon(), node->getNodeTitle(), suggestData);
	} else {
		suggestKey = navigationSuggestBox->addSuggestion(node->getFluentIcon(), node->getNodeTitle(), suggestData);
	}
	mapSuggestKey.insert(footKey, suggestKey);
}

void CALNavigationBarPrivate::raiseNavigationBar() {
	q_func()->raise();
}

void CALNavigationBarPrivate::doComponentAnimation(const ALNavigationType::NavigationDisplayMode& displayMode, const bool isAnimation) {
	switch (displayMode) {
		case ALNavigationType::Minimal: {
			doNavigationBarWidthAnimation(displayMode, isAnimation);
			if (currentDisplayMode == ALNavigationType::Maximal) {
				searchButton->setVisible(true);
				userInfoCard->setVisible(false);
				navigationSuggestBox->setVisible(false);
				userButton->setVisible(isShowUserCard);
				handleNavigationExpandState(true);
			}
			currentDisplayMode = displayMode;
			break;
		}
		case ALNavigationType::Compact: {
			doNavigationBarWidthAnimation(displayMode, isAnimation);
			doNavigationViewWidthAnimation(isAnimation);
			if (currentDisplayMode != ALNavigationType::Minimal) {
				handleMaximalToCompactLayout();
				doNavigationButtonAnimation(true, isAnimation);
				doSearchButtonAnimation(true, isAnimation);
				doUserButtonAnimation(true, isAnimation);
				navigationSuggestBox->setVisible(false);
				handleNavigationExpandState(true);
			}
			currentDisplayMode = displayMode;
			break;
		}
		case ALNavigationType::Maximal: {
			resetLayout();
			handleCompactToMaximalLayout();
			doNavigationBarWidthAnimation(displayMode, isAnimation);
			doUserButtonAnimation(false, isAnimation);
			doNavigationButtonAnimation(false, isAnimation);
			doSearchButtonAnimation(false, isAnimation);
			navigationSuggestBox->setVisible(true);
			currentDisplayMode = displayMode;
			handleNavigationExpandState(false);
			break;
		}
		default: {
			break;
		}
	}
}

void CALNavigationBarPrivate::handleNavigationExpandState(const bool isSave) {
	if (isSave) {
		// 保存展开状态, 收起根节点所有子树
		listLastExpandedNodes = navigationModel->getRootExpandedNodes();
		for (const auto& node : listLastExpandedNodes) {
			slotTreeViewClicked(node->getModelIndex(), false);
		}
	} else {
		for (const auto& node : listLastExpandedNodes) {
			// 修正动画覆盖
			navigationView->resize(295, navigationView->height());
			slotTreeViewClicked(node->getModelIndex(), false);
		}
	}
}

void CALNavigationBarPrivate::handleMaximalToCompactLayout() const {
	// 动画过程布局
	while (navigationButtonVLayout->count()) {
		navigationButtonVLayout->takeAt(0);
	}
	navigationButtonVLayout->addSpacing(isShowUserCard ? 76 : 40);
	navigationSuggestBoxHLayout->addStretch();

	while (userButtonVLayout->count()) {
		userButtonVLayout->takeAt(0);
	}
	userButtonVLayout->addSpacing(36);
}

void CALNavigationBarPrivate::handleCompactToMaximalLayout() const {
	// 动画过程布局
	while (navigationButtonVLayout->count()) {
		navigationButtonVLayout->takeAt(0);
	}
	navigationButtonVLayout->addSpacing(38);
	navigationSuggestBoxHLayout->insertSpacing(0, 46);

	while (userButtonVLayout->count()) {
		userButtonVLayout->takeAt(0);
	}
	if (isShowUserCard) {
		userButtonVLayout->addSpacing(74);
	}
}

void CALNavigationBarPrivate::resetLayout() const {
	// 恢复初始布局
	while (navigationButtonVLayout->count()) {
		navigationButtonVLayout->takeAt(0);
	}
	navigationButtonVLayout->addWidget(navigationButton);
	navigationButtonVLayout->addWidget(searchButton);

	while (navigationSuggestBoxHLayout->count()) {
		navigationSuggestBoxHLayout->takeAt(0);
	}
	navigationSuggestBoxHLayout->addLayout(navigationButtonVLayout);
	navigationSuggestBoxHLayout->addWidget(navigationSuggestBox);

	while (userButtonVLayout->count()) {
		userButtonVLayout->takeAt(0);
	}
	userButtonVLayout->addWidget(userButton);
}

void CALNavigationBarPrivate::doNavigationBarWidthAnimation(const ALNavigationType::NavigationDisplayMode& displayMode, const bool isAnimation) {
	Q_Q(CALNavigationBar);

	const auto navigationBarWidthAnimation = new QPropertyAnimation(q, "maximumWidth");
	navigationBarWidthAnimation->setEasingCurve(QEasingCurve::OutCubic);
	navigationBarWidthAnimation->setDuration(isAnimation ? 285 : 0);
	navigationBarWidthAnimation->setStartValue(q->width());

	switch (displayMode) {
		case ALNavigationType::Minimal: {
			connect(navigationBarWidthAnimation, &QPropertyAnimation::valueChanged, this, [=](const QVariant& value) { q->setFixedWidth(value.toInt()); });
			navigationBarWidthAnimation->setEndValue(0);
			break;
		}
		case ALNavigationType::Compact: {
			connect(navigationBarWidthAnimation, &QPropertyAnimation::valueChanged, this, [=](const QVariant& value) { q->setFixedWidth(value.toInt()); });
			navigationBarWidthAnimation->setEndValue(47);
			break;
		}
		case ALNavigationType::Maximal: {
			connect(navigationBarWidthAnimation, &QPropertyAnimation::finished, this, [=]() { resetLayout(); });
			connect(navigationBarWidthAnimation, &QPropertyAnimation::valueChanged, this, [=](const QVariant& value) { q->setFixedWidth(value.toInt()); });
			navigationBarWidthAnimation->setEndValue(300);
			break;
		}
		default: {
			break;
		}
	}

	navigationBarWidthAnimation->start(QAbstractAnimation::DeleteWhenStopped);
}

void CALNavigationBarPrivate::doNavigationViewWidthAnimation(const bool isAnimation) {
	const auto navigationViewWidthAnimation = new QPropertyAnimation(this, "navigationViewWidth");
	connect(navigationViewWidthAnimation, &QPropertyAnimation::valueChanged, this, [=](const QVariant& value) { navigationView->setColumnWidth(0, value.toInt()); });
	navigationViewWidthAnimation->setEasingCurve(QEasingCurve::OutCubic);
	navigationViewWidthAnimation->setStartValue(navigationView->columnWidth(0));
	navigationViewWidthAnimation->setEndValue(40);
	navigationViewWidthAnimation->setDuration(isAnimation ? 285 : 0);
	navigationViewWidthAnimation->start(QAbstractAnimation::DeleteWhenStopped);
}

void CALNavigationBarPrivate::doNavigationButtonAnimation(const bool isCompact, const bool isAnimation) {
	// 导航按钮
	const auto navigationButtonAnimation = new QPropertyAnimation(navigationButton, "pos");
	const QPoint navigationButtonPos = navigationButton->pos();
	navigationButtonAnimation->setStartValue(navigationButtonPos);
	if (isCompact) {
		connect(navigationButtonAnimation, &QPropertyAnimation::finished, this, [=]() { resetLayout(); });
		navigationButtonAnimation->setEndValue(isShowUserCard ? QPoint(0, 56) : navigationButtonPos);
		navigationButtonAnimation->setEasingCurve(QEasingCurve::OutCubic);
		navigationButtonAnimation->setDuration(isAnimation ? 285 : 0);
	} else {
		navigationButtonAnimation->setEndValue(isShowUserCard ? QPoint(0, 94) : navigationButtonPos);
		navigationButtonAnimation->setEasingCurve(QEasingCurve::InOutSine);
		navigationButtonAnimation->setDuration(isAnimation ? 130 : 0);
	}
	navigationButtonAnimation->start(QAbstractAnimation::DeleteWhenStopped);
}

void CALNavigationBarPrivate::doSearchButtonAnimation(const bool isCompact, const bool isAnimation) const {
	if (isCompact) {
		const QPoint navigationButtonPos = navigationButton->pos();
		// 搜索按钮
		const auto searchButtonAnimation = new QPropertyAnimation(searchButton, "pos");
		const auto startValue = QPoint(200, isShowUserCard ? navigationButtonPos.y() : navigationButtonPos.y() + 38);
		const auto endValue = QPoint(0, isShowUserCard ? navigationButtonPos.y() : navigationButtonPos.y() + 38);
		searchButtonAnimation->setStartValue(startValue);
		searchButtonAnimation->setEndValue(endValue);
		searchButtonAnimation->setEasingCurve(QEasingCurve::OutCubic);
		searchButtonAnimation->setDuration(isAnimation ? 285 : 0);
		searchButtonAnimation->start(QAbstractAnimation::DeleteWhenStopped);
		searchButton->setVisible(true);
	} else {
		searchButton->setVisible(false);
	}
}

void CALNavigationBarPrivate::doUserButtonAnimation(const bool isCompact, const bool isAnimation) {
	const auto userButtonAnimation = new QPropertyAnimation(userButton, "geometry");
	connect(userButtonAnimation, &QPropertyAnimation::valueChanged, this, [=](const QVariant& value) { userButton->setFixedSize(value.toRect().size()); });
	if (isCompact) {
		userInfoCard->setVisible(false);
		userButton->setVisible(isShowUserCard);
		userButtonAnimation->setEasingCurve(QEasingCurve::OutCubic);
		userButtonAnimation->setStartValue(QRect(13, 18, 64, 64));
		userButtonAnimation->setEndValue(QRect(3, 10, 36, 36));
		userButtonAnimation->setDuration(isAnimation ? 285 : 0);
		userButtonAnimation->start(QAbstractAnimation::DeleteWhenStopped);
	} else {
		connect(userButtonAnimation, &QPropertyAnimation::finished, this, [=]() {
			userInfoCard->setVisible(isShowUserCard);
			userButton->setFixedSize(36, 36);
			userButton->setGeometry(QRect(3, 10, 36, 36));
			userButton->setVisible(false);
		});
		userButtonAnimation->setEasingCurve(QEasingCurve::InOutSine);
		userButtonAnimation->setStartValue(QRect(3, 10, 36, 36));
		userButtonAnimation->setEndValue(QRect(13, 18, 64, 64));
		userButtonAnimation->setDuration(isAnimation ? 130 : 0);
		userButtonAnimation->start(QAbstractAnimation::DeleteWhenStopped);
	}
}

/**
 * @brief \class CALNavigationBar
 * @param parent pointer to the parent class
 */
CALNavigationBar::CALNavigationBar(QWidget* parent): QWidget(parent), d_ptr(new CALNavigationBarPrivate(this, this)) {
	Q_D(CALNavigationBar);

	setFixedWidth(300);
	d->isTransparent = true;
	/// user
	// user card
	d->userInfoCard = new CALInteractiveCard(this);
	d->userInfoCard->setCardPixmap(QPixmap(":alwidgettools/image/crucal.png"));
	d->userInfoCard->setTitle("crucal");
	d->userInfoCard->setSubTitle("alwidgettools");
	connect(d->userInfoCard, &CALInteractiveCard::clicked, this, &CALNavigationBar::sigUserInfoCardClicked);
	// user button
	d->userButton = new CALIconButton(QPixmap(":alwidgettools/image/crucal.png"), this);
	d->userButton->setFixedSize(36, 36);
	d->userButton->setVisible(false);
	d->userButton->setBorderRadius(8);
	connect(d->userButton, &CALIconButton::clicked, this, &CALNavigationBar::sigUserInfoCardClicked);
	// user button layout
	d->userButtonVLayout = new QVBoxLayout;
	d->userButtonVLayout->setAlignment(Qt::AlignLeft);
	d->userButtonVLayout->setContentsMargins(0, 0, 0, 0);
	d->userButtonVLayout->setSpacing(0);
	d->userButtonVLayout->addWidget(d->userButton);
	// user card layout
	const auto userCardLayout = new QHBoxLayout;
	userCardLayout->setContentsMargins(0, 0, 0, 0);
	userCardLayout->addSpacing(3);
	userCardLayout->addLayout(d->userButtonVLayout);
	userCardLayout->addWidget(d->userInfoCard);

	/// Search bar and button group
	// navigation button
	d->navigationButton = new CALToolButton(this);
	d->navigationButton->setFixedSize(40, 38);
	d->navigationButton->setAweSomeIcon(ALIcon::AweSomeIcon::Bars);
	d->navigationButton->setBorderRadius(8);
	connect(d->navigationButton, &CALToolButton::clicked, d, &CALNavigationBarPrivate::slotNavigationButtonClicked);
	// search button
	d->searchButton = new CALToolButton(this);
	d->searchButton->setFixedSize(40, 38);
	d->searchButton->setAweSomeIcon(ALIcon::AweSomeIcon::MagnifyingGlass);
	d->searchButton->setBorderRadius(8);
	d->searchButton->setVisible(false);
	connect(d->searchButton, &CALToolButton::clicked, d, &CALNavigationBarPrivate::slotNavigationButtonClicked);
	// navigation suggest box
	d->navigationSuggestBox = new CALSuggestBox(this);
	d->navigationSuggestBox->setMinimumWidth(0);
	// navigation button layout
	d->navigationButtonVLayout = new QVBoxLayout;
	d->navigationButtonVLayout->setContentsMargins(0, 0, 0, 0);
	d->navigationButtonVLayout->setSpacing(0);
	d->navigationButtonVLayout->addWidget(d->navigationButton);
	d->navigationButtonVLayout->addWidget(d->searchButton);
	// navigation suggest layout
	d->navigationSuggestBoxHLayout = new QHBoxLayout;
	d->navigationSuggestBoxHLayout->setContentsMargins(0, 0, 10, 0);
	d->navigationSuggestBoxHLayout->setSpacing(6);
	d->navigationSuggestBoxHLayout->addLayout(d->navigationButtonVLayout);
	d->navigationSuggestBoxHLayout->addWidget(d->navigationSuggestBox);

	/// search jump
	connect(d->navigationSuggestBox, &CALSuggestBox::sigSuggestionClicked, this, [=](const QString& suggestText, const QVariantMap& suggestData) {
		if (suggestData.value("CALNodeType").toString() == "Stacked") {
			if (const CALNavigationNode* node = d->navigationModel->getNavigationNode(suggestData.value("CALPageKey").toString())) {
				d->slotTreeViewClicked(node->getModelIndex());
			}
		} else {
			if (const CALNavigationNode* node = d->navigationModel->getNavigationNode(suggestData.value("CALPageKey").toString())) {
				d->slotFooterViewClicked(node->getModelIndex());
			}
		}
	});

	/// navigation model and view
	// model
	d->navigationModel = new CALNavigationModel(this);
	// view
	d->navigationView = new CALNavigationView(this);
	d->navigationView->setModel(d->navigationModel);
	connect(d->navigationView, &CALNavigationView::sigNavigationClicked, this, [=](const QModelIndex& index) { d->slotTreeViewClicked(index); });
	connect(d->navigationView, &CALNavigationView::sigNavigationOpenNewWindow, d, &CALNavigationBarPrivate::slotNavigationOpenNewWindow);

	/// footer model, and delegate
	// model
	d->footerModel = new CALFooterModel(this);
	// view
	d->footerView = new CALBaseListView(this);
	d->footerView->setFixedHeight(0);
	d->footerView->setModel(d->footerModel);
	// delegate
	d->footerDelegate = new CALFooterDelegate(this);
	d->footerDelegate->setCALBaseListView(d->footerView);
	d->footerView->setItemDelegate(d->footerDelegate);
	connect(d->footerView, &CALBaseListView::sigMousePress, this, [=](const QModelIndex& index) {
		d->footerDelegate->setPressIndex(index);
		d->footerView->viewport()->update();
	});
	connect(d->footerView, &CALBaseListView::sigMouseRelease, this, [=](const QModelIndex& index) {
		d->footerDelegate->setPressIndex({});
		d->footerView->viewport()->update();
	});
	connect(d->footerView, &CALBaseListView::sigMouseDoubleClick, this, [=](const QModelIndex& index) {
		d->footerDelegate->setPressIndex(index);
		d->footerView->viewport()->update();
	});
	connect(d->footerView, &CALBaseListView::clicked, this, [=](const QModelIndex& index) { d->slotFooterViewClicked(index); });

	/// main layout
	const auto mainVLayout = new QVBoxLayout(this);
	mainVLayout->setAlignment(Qt::AlignLeft);
	mainVLayout->setSpacing(0);
	mainVLayout->setContentsMargins(0, 10, 5, 0);
	mainVLayout->addLayout(userCardLayout);
	mainVLayout->addSpacing(4);
	mainVLayout->addLayout(d->navigationSuggestBoxHLayout);
	mainVLayout->addSpacing(4);
	mainVLayout->addWidget(d->navigationView);
	mainVLayout->addWidget(d->footerView);

	/// theme
	d->themeMode = ALTheme->getThemeMode();
	connect(ALTheme, &CALThemeManager::sigThemeModeChanged, this, [=](const ALThemeType::ThemeMode& mode) { d->themeMode = mode; });
}

CALNavigationBar::~CALNavigationBar() = default;

void CALNavigationBar::setUserInfoCardVisible(const bool isVisible) {
	Q_D(CALNavigationBar);

	d->isShowUserCard = isVisible;
	d->userInfoCard->setVisible(isVisible);
	d->userButton->setVisible(!isVisible);
}

void CALNavigationBar::setUserInfoCardPixmap(const QPixmap& pix) {
	Q_D(CALNavigationBar);

	d->userInfoCard->setCardPixmap(pix);
	d->userButton->setPixmap(pix);
}

void CALNavigationBar::setUserInfoCardTitle(const QString& title) {
	d_func()->userInfoCard->setTitle(title);
}

void CALNavigationBar::setUserInfoCardSubTitle(const QString& subTitle) {
	d_func()->userInfoCard->setSubTitle(subTitle);
}

ALNavigationType::NodeOperateReturnType CALNavigationBar::addExpanderNode(const QString& expanderTitle, QString& expanderKey, const ALIcon::AweSomeIcon& awewomeIcon) {
	Q_D(CALNavigationBar);

	const ALNavigationType::NodeOperateReturnType resType = d->navigationModel->addExpanderNode(expanderTitle, expanderKey, awewomeIcon);
	if (resType == ALNavigationType::Success) {
		d->initNodeModelIndex({});
		d->resetNodeSelected();
	}

	return resType;
}

ALNavigationType::NodeOperateReturnType CALNavigationBar::addExpanderNode(const QString& expanderTitle, QString& expanderKey, const ALIcon::FluentIcon& fluentIcon) {
	Q_D(CALNavigationBar);

	const ALNavigationType::NodeOperateReturnType resType = d->navigationModel->addExpanderNode(expanderTitle, expanderKey, fluentIcon);
	if (resType == ALNavigationType::Success) {
		d->initNodeModelIndex({});
		d->resetNodeSelected();
	}

	return resType;
}

ALNavigationType::NodeOperateReturnType CALNavigationBar::addExpanderNode(const QString& expanderTitle, QString& expanderKey, const QString& targetExpanderKey, const ALIcon::AweSomeIcon& awewomeIcon) {
	Q_D(CALNavigationBar);

	const ALNavigationType::NodeOperateReturnType resType = d->navigationModel->addExpanderNode(expanderTitle, expanderKey, targetExpanderKey, awewomeIcon);
	if (resType == ALNavigationType::Success) {
		d->initNodeModelIndex({});
		d->resetNodeSelected();
	}

	return resType;
}

ALNavigationType::NodeOperateReturnType CALNavigationBar::addExpanderNode(const QString& expanderTitle, QString& expanderKey, const QString& targetExpanderKey, const ALIcon::FluentIcon& fluentIcon) {
	Q_D(CALNavigationBar);

	const ALNavigationType::NodeOperateReturnType resType = d->navigationModel->addExpanderNode(expanderTitle, expanderKey, targetExpanderKey, fluentIcon);
	if (resType == ALNavigationType::Success) {
		d->initNodeModelIndex({});
		d->resetNodeSelected();
	}

	return resType;
}

ALNavigationType::NodeOperateReturnType CALNavigationBar::addPageNode(const QString& pageTitle, QWidget* page, const ALIcon::AweSomeIcon& awewomeIcon) {
	Q_D(CALNavigationBar);
	if (!page) {
		return ALNavigationType::PageInvalid;
	}

	QString pageKey{};
	const ALNavigationType::NodeOperateReturnType resType = d->navigationModel->addPageNode(pageTitle, pageKey, awewomeIcon);
	if (resType == ALNavigationType::Success) {
		d->mapPageMeta.insert(pageKey, page->metaObject());
		d->addStackedPage(page, pageKey);
		d->initNodeModelIndex({});
		d->resetNodeSelected();
	}

	return resType;
}

ALNavigationType::NodeOperateReturnType CALNavigationBar::addPageNode(const QString& pageTitle, QWidget* page, const ALIcon::FluentIcon& fluentIcon) {
	Q_D(CALNavigationBar);
	if (!page) {
		return ALNavigationType::PageInvalid;
	}

	QString pageKey{};
	const ALNavigationType::NodeOperateReturnType resType = d->navigationModel->addPageNode(pageTitle, pageKey, fluentIcon);
	if (resType == ALNavigationType::Success) {
		d->mapPageMeta.insert(pageKey, page->metaObject());
		d->addStackedPage(page, pageKey);
		d->initNodeModelIndex({});
		d->resetNodeSelected();
	}

	return resType;
}

ALNavigationType::NodeOperateReturnType CALNavigationBar::addPageNode(const QString& pageTitle, QWidget* page, const QString& targetExpanderKey, const ALIcon::AweSomeIcon& awewomeIcon) {
	Q_D(CALNavigationBar);
	if (!page) {
		return ALNavigationType::PageInvalid;
	}
	if (targetExpanderKey.isEmpty()) {
		return ALNavigationType::TargetNodeInvalid;
	}

	QString pageKey{};
	const ALNavigationType::NodeOperateReturnType resType = d->navigationModel->addPageNode(pageTitle, pageKey, targetExpanderKey, awewomeIcon);
	if (resType == ALNavigationType::Success) {
		d->mapPageMeta.insert(pageKey, page->metaObject());
		CALNavigationNode* node = d->navigationModel->getNavigationNode(pageKey);
		if (CALNavigationNode* originalNode = node->getOriginalNode(); d->mapCompactMenu.contains(originalNode)) {
			CALMenu* menu = d->mapCompactMenu.value(originalNode);
			const QAction* action = menu->addAction(node->getAwesomeIcon(), node->getNodeTitle());
			connect(action, &QAction::triggered, this, [=]() { d->slotTreeViewClicked(node->getModelIndex()); });
		} else {
			const auto menu = new CALMenu(const_cast<CALNavigationBar*>(this));
			const QAction* action = menu->addAction(node->getAwesomeIcon(), node->getNodeTitle());
			connect(action, &QAction::triggered, this, [=]() { d->slotTreeViewClicked(node->getModelIndex()); });
			d->mapCompactMenu.insert(originalNode, menu);
		}
		d->addStackedPage(page, pageKey);
		d->initNodeModelIndex({});
		d->resetNodeSelected();
	}

	return resType;
}

ALNavigationType::NodeOperateReturnType CALNavigationBar::addPageNode(const QString& pageTitle, QWidget* page, const QString& targetExpanderKey, const ALIcon::FluentIcon& fluentIcon) {
	Q_D(CALNavigationBar);
	if (!page) {
		return ALNavigationType::PageInvalid;
	}
	if (targetExpanderKey.isEmpty()) {
		return ALNavigationType::TargetNodeInvalid;
	}

	QString pageKey{};
	const ALNavigationType::NodeOperateReturnType resType = d->navigationModel->addPageNode(pageTitle, pageKey, targetExpanderKey, fluentIcon);
	if (resType == ALNavigationType::Success) {
		d->mapPageMeta.insert(pageKey, page->metaObject());
		CALNavigationNode* node = d->navigationModel->getNavigationNode(pageKey);
		if (CALNavigationNode* originalNode = node->getOriginalNode(); d->mapCompactMenu.contains(originalNode)) {
			CALMenu* menu = d->mapCompactMenu.value(originalNode);
			const QAction* action = menu->addAction(node->getFluentIcon(), node->getNodeTitle());
			connect(action, &QAction::triggered, this, [=]() { d->slotTreeViewClicked(node->getModelIndex()); });
		} else {
			const auto menu = new CALMenu(const_cast<CALNavigationBar*>(this));
			const QAction* action = menu->addAction(node->getFluentIcon(), node->getNodeTitle());
			connect(action, &QAction::triggered, this, [=]() { d->slotTreeViewClicked(node->getModelIndex()); });
			d->mapCompactMenu.insert(originalNode, menu);
		}
		d->addStackedPage(page, pageKey);
		d->initNodeModelIndex({});
		d->resetNodeSelected();
	}

	return resType;
}

ALNavigationType::NodeOperateReturnType CALNavigationBar::addPageNode(const QString& pageTitle, QWidget* page, const int keyPoints, const ALIcon::AweSomeIcon& awewomeIcon) {
	Q_D(CALNavigationBar);
	if (!page) {
		return ALNavigationType::PageInvalid;
	}

	QString pageKey{};
	const ALNavigationType::NodeOperateReturnType resType = d->navigationModel->addPageNode(pageTitle, pageKey, keyPoints, awewomeIcon);
	if (resType == ALNavigationType::Success) {
		d->mapPageMeta.insert(pageKey, page->metaObject());
		d->addStackedPage(page, pageKey);
		d->initNodeModelIndex({});
		d->resetNodeSelected();
	}

	return resType;
}

ALNavigationType::NodeOperateReturnType CALNavigationBar::addPageNode(const QString& pageTitle, QWidget* page, const int keyPoints, const ALIcon::FluentIcon& fluentIcon) {
	Q_D(CALNavigationBar);
	if (!page) {
		return ALNavigationType::PageInvalid;
	}

	QString pageKey{};
	const ALNavigationType::NodeOperateReturnType resType = d->navigationModel->addPageNode(pageTitle, pageKey, keyPoints, fluentIcon);
	if (resType == ALNavigationType::Success) {
		d->mapPageMeta.insert(pageKey, page->metaObject());
		d->addStackedPage(page, pageKey);
		d->initNodeModelIndex({});
		d->resetNodeSelected();
	}

	return resType;
}

ALNavigationType::NodeOperateReturnType CALNavigationBar::addPageNode(const QString& pageTitle, QWidget* page, const QString& targetExpanderKey, const int keyPoints, const ALIcon::AweSomeIcon& awewomeIcon) {
	Q_D(CALNavigationBar);
	if (!page) {
		return ALNavigationType::PageInvalid;
	}
	if (targetExpanderKey.isEmpty()) {
		return ALNavigationType::TargetNodeInvalid;
	}

	QString pageKey{};
	const ALNavigationType::NodeOperateReturnType resType = d->navigationModel->addPageNode(pageTitle, pageKey, targetExpanderKey, keyPoints, awewomeIcon);
	if (resType == ALNavigationType::Success) {
		d->mapPageMeta.insert(pageKey, page->metaObject());
		CALNavigationNode* node = d->navigationModel->getNavigationNode(pageKey);
		if (CALNavigationNode* originalNode = node->getOriginalNode(); d->mapCompactMenu.contains(originalNode)) {
			CALMenu* menu = d->mapCompactMenu.value(originalNode);
			const QAction* action = menu->addAction(node->getAwesomeIcon(), node->getNodeTitle());
			connect(action, &QAction::triggered, this, [=]() { d->slotTreeViewClicked(node->getModelIndex()); });
		} else {
			const auto menu = new CALMenu(const_cast<CALNavigationBar*>(this));
			const QAction* action = menu->addAction(node->getAwesomeIcon(), node->getNodeTitle());
			connect(action, &QAction::triggered, this, [=]() { d->slotTreeViewClicked(node->getModelIndex()); });
			d->mapCompactMenu.insert(originalNode, menu);
		}
		d->addStackedPage(page, pageKey);
		d->initNodeModelIndex({});
		d->resetNodeSelected();
	}

	return resType;
}

ALNavigationType::NodeOperateReturnType CALNavigationBar::addPageNode(const QString& pageTitle, QWidget* page, const QString& targetExpanderKey, const int keyPoints, const ALIcon::FluentIcon& fluentIcon) {
	Q_D(CALNavigationBar);
	if (!page) {
		return ALNavigationType::PageInvalid;
	}
	if (targetExpanderKey.isEmpty()) {
		return ALNavigationType::TargetNodeInvalid;
	}

	QString pageKey{};
	const ALNavigationType::NodeOperateReturnType resType = d->navigationModel->addPageNode(pageTitle, pageKey, targetExpanderKey, keyPoints, fluentIcon);
	if (resType == ALNavigationType::Success) {
		d->mapPageMeta.insert(pageKey, page->metaObject());
		CALNavigationNode* node = d->navigationModel->getNavigationNode(pageKey);
		if (CALNavigationNode* originalNode = node->getOriginalNode(); d->mapCompactMenu.contains(originalNode)) {
			CALMenu* menu = d->mapCompactMenu.value(originalNode);
			const QAction* action = menu->addAction(node->getFluentIcon(), node->getNodeTitle());
			connect(action, &QAction::triggered, this, [=]() { d->slotTreeViewClicked(node->getModelIndex()); });
		} else {
			const auto menu = new CALMenu(const_cast<CALNavigationBar*>(this));
			const QAction* action = menu->addAction(node->getFluentIcon(), node->getNodeTitle());
			connect(action, &QAction::triggered, this, [=]() { d->slotTreeViewClicked(node->getModelIndex()); });
			d->mapCompactMenu.insert(originalNode, menu);
		}
		d->addStackedPage(page, pageKey);
		d->initNodeModelIndex({});
		d->resetNodeSelected();
	}

	return resType;
}

ALNavigationType::NodeOperateReturnType CALNavigationBar::addFooterNode(const QString& footerTitle, QString& footerKey, const int keyPoints, const ALIcon::AweSomeIcon& awewomeIcon) {
	return addFooterNode(footerTitle, nullptr, footerKey, keyPoints, awewomeIcon);
}

ALNavigationType::NodeOperateReturnType CALNavigationBar::addFooterNode(const QString& footerTitle, QString& footerKey, const int keyPoints, const ALIcon::FluentIcon& fluentIcon) {
	return addFooterNode(footerTitle, nullptr, footerKey, keyPoints, fluentIcon);
}

ALNavigationType::NodeOperateReturnType CALNavigationBar::addFooterNode(const QString& footerTitle, QWidget* page, QString& footerKey, const int keyPoints, const ALIcon::AweSomeIcon& awewomeIcon) {
	Q_D(CALNavigationBar);

	const auto resType = d->footerModel->addFooterNode(footerTitle, footerKey, page ? true : false, keyPoints, awewomeIcon);
	if (resType == ALNavigationType::Success) {
		d->addFooterPage(page, footerKey);
	}

	return resType;
}

ALNavigationType::NodeOperateReturnType CALNavigationBar::addFooterNode(const QString& footerTitle, QWidget* page, QString& footerKey, const int keyPoints, const ALIcon::FluentIcon& fluentIcon) {
	Q_D(CALNavigationBar);

	const auto resType = d->footerModel->addFooterNode(footerTitle, footerKey, page ? true : false, keyPoints, fluentIcon);
	if (resType == ALNavigationType::Success) {
		d->addFooterPage(page, footerKey);
	}

	return resType;
}

bool CALNavigationBar::removeNavigationNode(const QString& nodeKey) {
	bool bRet{ true };

	Q_D(CALNavigationBar);

	const CALNavigationNode* node = d->navigationModel->getNavigationNode(nodeKey);
	if (!node) {
		node = d->footerModel->getNavigationNode(nodeKey);
	}
	if (!node) {
		return false;
	}

	if (node->getIsFooterNode()) {
		Q_EMIT sigNavigationNodeRemoved(ALNavigationType::FooterNode, nodeKey);
		bRet &= d->footerModel->removeNavigationNode(nodeKey);
		d->footerView->setFixedHeight(40 * d->footerModel->getFooterNodeCount());
	} else {
		QStringList removeKeyList = d->navigationModel->removeNavigationNode(nodeKey);
		d->initNodeModelIndex({});
		for (const auto& removeKey : removeKeyList) {
			Q_EMIT sigNavigationNodeRemoved(ALNavigationType::PageNode, removeKey);
		}
	}
	bRet &= d->navigationSuggestBox->removeSuggestion(d->mapSuggestKey.value(nodeKey));

	return bRet;
}

void CALNavigationBar::setIsTransparent(const bool isTransparent) {
	d_func()->isTransparent = isTransparent;
	Q_EMIT sigIsTransparentChanged();
}

bool CALNavigationBar::getIsTransparent() const {
	return d_func()->isTransparent;
}

void CALNavigationBar::setNodeKeyPoints(const QString& nodeKey, const int keyPoints) {
	Q_D(CALNavigationBar);

	CALNavigationNode* node = d->navigationModel->getNavigationNode(nodeKey);
	if (!node) {
		node = d->footerModel->getNavigationNode(nodeKey);
	} else if (node->getIsExpanderNode() || keyPoints < 0) {
		return;
	}

	if (!node) {
		return;
	}

	node->setKeyPoints(keyPoints);
	node->getIsFooterNode() ? d->footerView->viewport()->update() : d->navigationView->viewport()->update();
}

int CALNavigationBar::getNodeKeyPoints(const QString& nodeKey) const {
	Q_D(const CALNavigationBar);

	const CALNavigationNode* node = d->navigationModel->getNavigationNode(nodeKey);
	if (!node) {
		node = d->footerModel->getNavigationNode(nodeKey);
	} else if (node->getIsExpanderNode()) {
		return -1;
	}

	if (!node) {
		return -1;
	}

	return node->getKeyPoints();
}

void CALNavigationBar::navigation(const QString& pageKey, const bool isLogClicked) {
	Q_D(CALNavigationBar);

	const CALNavigationNode* node = d->navigationModel->getNavigationNode(pageKey);
	if (!node) {
		node = d->footerModel->getNavigationNode(pageKey);
	}
	if (node) {
		if (node->getIsFooterNode()) {
			d->slotFooterViewClicked(node->getModelIndex(), isLogClicked);
		} else {
			if (!node->getIsExpanderNode()) {
				d->slotTreeViewClicked(node->getModelIndex(), isLogClicked);
			}
		}
	}
}

void CALNavigationBar::setDisplayMode(const ALNavigationType::NavigationDisplayMode& displayMode, const bool isAnimation) {
	Q_D(CALNavigationBar);

	if (d->currentDisplayMode == displayMode || displayMode == ALNavigationType::Auto) {
		return;
	}

	d->doComponentAnimation(displayMode, isAnimation);
	d->raiseNavigationBar();
}

void CALNavigationBar::paintEvent(QPaintEvent* event) {
	Q_D(CALNavigationBar);

	if (!d->isTransparent) {
		QPainter painter(this);
		painter.save();
		painter.setPen(ALThemeColor(d->themeMode, ALThemeType::PopupBorder));
		painter.setBrush(ALThemeColor(d->themeMode, ALThemeType::PopupBase));
		QRect baseRect = this->rect();
		baseRect.adjust(-1, 0, -1, 0);
		QPainterPath path;
		path.moveTo(baseRect.topLeft());
		path.lineTo(QPoint(baseRect.right() - 8, baseRect.y()));
		path.arcTo(QRectF(baseRect.right() - 16.0, baseRect.y(), 16.0, 16.0), 90.0, -90.0);
		path.lineTo(QPoint(baseRect.right(), baseRect.bottom() - 8));
		path.arcTo(QRectF(baseRect.right() - 16.0, baseRect.bottom() - 16.0, 16.0, 16.0), 0.0, -90.0);
		path.lineTo(baseRect.bottomLeft());
		path.closeSubpath();
		painter.drawPath(path);
		painter.restore();
	}

	QWidget::paintEvent(event);
}
}
