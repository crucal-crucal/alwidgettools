#include "uvnavigationbar.hpp"

#include <QDebug>
#include <QLayout>
#include <QPainter>
#include <QPainterPath>
#include <QPropertyAnimation>

#include "uvawesometoolbutton.hpp"
#include "uvbaselistview.hpp"
#include "uvcustomwidget.hpp"
#include "uvfooterdelegate.hpp"
#include "uvfootermodel.hpp"
#include "uviconbutton.hpp"
#include "uvinteractivecard.hpp"
#include "uvmenu.hpp"
#include "uvnavigationbar_p.hpp"
#include "uvnavigationmodel.hpp"
#include "uvnavigationnode.hpp"
#include "uvnavigationrouter.hpp"
#include "uvnavigationview.hpp"
#include "uvsuggestbox.hpp"
#include "uvthememanager.hpp"

/**
 * @brief \class CUVNavigationBarPrivate
 * Internal class for CUVNavigationBar
 * @param q pointer to the public class
 * @param parent pointer to the parent class
 */
CUVNavigationBarPrivate::CUVNavigationBarPrivate(CUVNavigationBar* q, QObject* parent): QObject(parent), q_ptr(q) {
	isShowUserCard = true;
	currentDisplayMode = UVNavigationType::Maximal;
}

CUVNavigationBarPrivate::~CUVNavigationBarPrivate() = default;

void CUVNavigationBarPrivate::setNavigationViewWidth(const int width) {
	navigationViewWidth = width;
	Q_EMIT sigNavigationViewWidthChanged();
}

int CUVNavigationBarPrivate::getNavigationViewWidth() const {
	return navigationViewWidth;
}

void CUVNavigationBarPrivate::slotNavigationButtonClicked() {
	q_func()->setDisplayMode(currentDisplayMode == UVNavigationType::Compact ? UVNavigationType::Maximal : UVNavigationType::Compact);
}

void CUVNavigationBarPrivate::slotNavigationOpenNewWindow(const QString& nodeKey) {
	const QMetaObject* meta = mapPageMeta.value(nodeKey);
	if (!meta) {
		return;
	}

	if (const auto widget = qobject_cast<QWidget*>(meta->newInstance())) {
		const auto floatWidget = new CUVCustomWidget(q_func());
		floatWidget->setWindowIcon(widget->windowIcon());
		floatWidget->setWindowTitle(widget->windowTitle());
		floatWidget->setCenterWidget(widget);
		floatWidget->show();
	}
}

void CUVNavigationBarPrivate::invokableNavigationRouteBack(const QVariantMap& routeData) {
	q_func()->navigation(routeData.value("CUVPageKey").toString(), false);
}

void CUVNavigationBarPrivate::slotTreeViewClicked(const QModelIndex& index, const bool isLogRoute) {
	Q_Q(CUVNavigationBar);

	if (index.isValid()) {
		const auto node = static_cast<CUVNavigationNode*>(index.internalPointer());
		if (!node) {
			return;
		}
		if (node->getIsExpanderNode()) {
			if (currentDisplayMode == UVNavigationType::Compact) {
				if (node->getIsHasPageChild()) {
					// 展开菜单
					if (CUVMenu* menu = mapCompactMenu.value(node)) {
						const QPoint nodeTopRight = navigationView->mapToGlobal(navigationView->visualRect(node->getModelIndex()).topRight());
						menu->popup(QPoint(nodeTopRight.x() + 10, nodeTopRight.y()));
					}
				}
			} else {
				if (node->getIsHasChild()) {
					QVariantMap data;
					const bool isExpanded = navigationView->isExpanded(index);
					data.insert(isExpanded ? "Collapse" : "Expand", QVariant::fromValue(node));
					node->setIsExpanded(isExpanded);
					navigationView->navigationNodeStateChanged(data);
					isExpanded ? navigationView->collapse(index) : navigationView->expand(index);
				}
			}
		} else {
			if (node->getKeyPoints()) {
				node->setKeyPoints(0);
				navigationView->update();
			}
			if (const CUVNavigationNode* selectedNode = navigationModel->getSelectedNode(); selectedNode != node) {
				// 记录跳转
				if (isLogRoute) {
					QVariantMap routeData{};
					QStringList pageKeyList{};
					if (selectedNode) {
						pageKeyList.append(selectedNode->getNodeKey());
					} else if (footerModel->getSelectedNode()) {
						pageKeyList.append(footerModel->getSelectedNode()->getNodeKey());
					}

					routeData.insert("CUVPageKey", pageKeyList);
					CUVNavigationRouter::instance()->navigationRoute(this, "invokableNavigationRouteBack", routeData);
				}
				Q_EMIT q->sigNavigationNodeClicked(UVNavigationType::PageNode, node->getNodeKey());

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
				if (currentDisplayMode == UVNavigationType::Compact) {
					if (CUVNavigationNode* originNode = node->getOriginalNode(); originNode == node) {
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
				if (!node->getIsVisible() && currentDisplayMode != UVNavigationType::Compact) {
					expandSelectedNodeParent();
				}
			}
		}
		resetNodeSelected();
	}
}

void CUVNavigationBarPrivate::slotFooterViewClicked(const QModelIndex& index, const bool isLogRoute) {
	Q_Q(CUVNavigationBar);

	const auto node = index.data(Qt::UserRole).value<CUVNavigationNode*>();
	if (node->getKeyPoints()) {
		node->setKeyPoints(0);
		navigationView->update();
	}
	if (const CUVNavigationNode* selectedNode = footerModel->getSelectedNode(); selectedNode != node) {
		// 记录跳转
		if (isLogRoute && node->getIsHasFooterNode()) {
			QVariantMap routaData{};
			QStringList pageKeyList{};
			if (selectedNode) {
				pageKeyList.append(selectedNode->getNodeKey());
			} else {
				if (navigationModel->getSelectedNode()) {
					pageKeyList.append(navigationModel->getSelectedNode()->getNodeKey());
				}
			}
			routaData.insert("CUVPageKey", pageKeyList);
			CUVNavigationRouter::instance()->navigationRoute(this, "invokableNavigationRouteBack", routaData);
		}
		Q_EMIT q->sigNavigationNodeClicked(UVNavigationType::FooterNode, node->getNodeKey());

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

void CUVNavigationBarPrivate::resetNodeSelected() const {
	navigationView->clearSelection();
	CUVNavigationNode* selectedNode = navigationModel->getSelectedNode();
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
		CUVNavigationNode* parentNode = selectedNode->getParentNode();
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

void CUVNavigationBarPrivate::expandSelectedNodeParent() const {
	CUVNavigationNode* parentNode = navigationModel->getSelectedNode()->getParentNode();
	while (parentNode && !parentNode->getIsRootNode()) {
		QVariantMap postData{};
		postData.insert("Expand", QVariant::fromValue(parentNode));
		navigationView->navigationNodeStateChanged(postData);
		parentNode->setIsExpanded(true);
		navigationView->expand(parentNode->getModelIndex());
		parentNode = parentNode->getParentNode();
	}
}

void CUVNavigationBarPrivate::initNodeModelIndex(const QModelIndex& parentIndex) { // NOLINT
	const int rowCount = navigationModel->rowCount(parentIndex);
	for (int row = 0; row < rowCount; row++) {
		QModelIndex index = navigationModel->index(row, 0, parentIndex);
		const auto childNode = static_cast<CUVNavigationNode*>(index.internalPointer());
		childNode->setModelIndex(index);
		if (navigationModel->hasChildren(index)) {
			initNodeModelIndex(index);
		}
	}
}

void CUVNavigationBarPrivate::addStackedPage(QWidget* page, const QString& pageKey) {
	Q_Q(CUVNavigationBar);

	if (page) {
		page->setProperty("CUVPageKey", pageKey);
	}
	Q_EMIT q->sigNavigationNodeAdded(UVNavigationType::PageNode, pageKey, page);
	const CUVNavigationNode* node = navigationModel->getNavigationNode(pageKey);
	QVariantMap suggestData{};
	suggestData.insert("CUVNodeType", "Stacked");
	suggestData.insert("CUVPageKey", pageKey);
	navigationSuggestBox->addSuggestion(node->getAwesomeIcon(), node->getNodeTitle(), suggestData);
}

void CUVNavigationBarPrivate::addFooterPage(QWidget* page, const QString& footKey) {
	Q_Q(CUVNavigationBar);

	if (page) {
		page->setProperty("CUVPageKey", footKey);
	}
	Q_EMIT q->sigNavigationNodeAdded(UVNavigationType::FooterNode, footKey, page);
	footerView->setFixedHeight(40 * footerModel->getFooterNodeCount());
	const CUVNavigationNode* node = footerModel->getNavigationNode(footKey);
	QVariantMap suggestData{};
	suggestData.insert("CUVNodeType", "Footer");
	suggestData.insert("CUVPageKey", footKey);
	navigationSuggestBox->addSuggestion(node->getAwesomeIcon(), node->getNodeTitle(), suggestData);
}

void CUVNavigationBarPrivate::raiseNavigationBar() {
	q_func()->raise();
}

void CUVNavigationBarPrivate::doComponentAnimation(const UVNavigationType::NavigationDisplayMode& displayMode, const bool isAnimation) {
	switch (displayMode) {
		case UVNavigationType::Minimal: {
			doNavigationBarWidthAnimation(displayMode, isAnimation);
			if (currentDisplayMode == UVNavigationType::Maximal) {
				searchButton->setVisible(true);
				userInfoCard->setVisible(false);
				navigationSuggestBox->setVisible(false);
				userButton->setVisible(isShowUserCard);
				handleNavigationExpandState(true);
			}
			currentDisplayMode = displayMode;
			break;
		}
		case UVNavigationType::Compact: {
			doNavigationBarWidthAnimation(displayMode, isAnimation);
			doNavigationViewWidthAnimation(isAnimation);
			if (currentDisplayMode != UVNavigationType::Minimal) {
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
		case UVNavigationType::Maximal: {
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

void CUVNavigationBarPrivate::handleNavigationExpandState(const bool isSave) {
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

void CUVNavigationBarPrivate::handleMaximalToCompactLayout() const {
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

void CUVNavigationBarPrivate::handleCompactToMaximalLayout() const {
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

void CUVNavigationBarPrivate::resetLayout() const {
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

void CUVNavigationBarPrivate::doNavigationBarWidthAnimation(const UVNavigationType::NavigationDisplayMode& displayMode, const bool isAnimation) {
	Q_Q(CUVNavigationBar);

	const auto navigationBarWidthAnimation = new QPropertyAnimation(q, "maximumWidth");
	navigationBarWidthAnimation->setEasingCurve(QEasingCurve::OutCubic);
	navigationBarWidthAnimation->setDuration(isAnimation ? 285 : 0);
	navigationBarWidthAnimation->setStartValue(q->width());

	switch (displayMode) {
		case UVNavigationType::Minimal: {
			connect(navigationBarWidthAnimation, &QPropertyAnimation::valueChanged, this, [=](const QVariant& value) { q->setFixedWidth(value.toInt()); });
			navigationBarWidthAnimation->setEndValue(0);
			break;
		}
		case UVNavigationType::Compact: {
			connect(navigationBarWidthAnimation, &QPropertyAnimation::valueChanged, this, [=](const QVariant& value) { q->setFixedWidth(value.toInt()); });
			navigationBarWidthAnimation->setEndValue(47);
			break;
		}
		case UVNavigationType::Maximal: {
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

void CUVNavigationBarPrivate::doNavigationViewWidthAnimation(const bool isAnimation) {
	const auto navigationViewWidthAnimation = new QPropertyAnimation(this, "navigationViewWidth");
	connect(navigationViewWidthAnimation, &QPropertyAnimation::valueChanged, this, [=](const QVariant& value) { navigationView->setColumnWidth(0, value.toInt()); });
	navigationViewWidthAnimation->setEasingCurve(QEasingCurve::OutCubic);
	navigationViewWidthAnimation->setStartValue(navigationView->columnWidth(0));
	navigationViewWidthAnimation->setEndValue(40);
	navigationViewWidthAnimation->setDuration(isAnimation ? 285 : 0);
	navigationViewWidthAnimation->start(QAbstractAnimation::DeleteWhenStopped);
}

void CUVNavigationBarPrivate::doNavigationButtonAnimation(const bool isCompact, const bool isAnimation) {
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

void CUVNavigationBarPrivate::doSearchButtonAnimation(const bool isCompact, const bool isAnimation) const {
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

void CUVNavigationBarPrivate::doUserButtonAnimation(const bool isCompact, const bool isAnimation) {
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
 * @brief \class CUVNavigationBar
 * @param parent pointer to the parent class
 */
CUVNavigationBar::CUVNavigationBar(QWidget* parent): QWidget(parent), d_ptr(new CUVNavigationBarPrivate(this, this)) {
	Q_D(CUVNavigationBar);

	setFixedWidth(300);
	d->isTransparent = true;
	/// user
	// user card
	d->userInfoCard = new CUVInteractiveCard(this);
	d->userInfoCard->setCardPixmap(QPixmap(":/image/crucal.png"));
	d->userInfoCard->setTitle("crucal");
	d->userInfoCard->setSubTitle("uvwidgettools");
	connect(d->userInfoCard, &CUVInteractiveCard::clicked, this, &CUVNavigationBar::sigUserInfoCardClicked);
	// user button
	d->userButton = new CUVIconButton(QPixmap(":/image/crucal.png"), this);
	d->userButton->setFixedSize(36, 36);
	d->userButton->setVisible(false);
	d->userButton->setBorderRadius(8);
	connect(d->userButton, &CUVIconButton::clicked, this, &CUVNavigationBar::sigUserInfoCardClicked);
	// user button layout
	d->userButtonVLayout = new QVBoxLayout;
	d->userButtonVLayout->setAlignment(Qt::AlignLeft);
	d->userButtonVLayout->setContentsMargins(0, 0, 0, 0);
	d->userButtonVLayout->setSpacing(0);
	d->userButtonVLayout->addWidget(d->userButton);
	// user card layout
	const auto userCardLayout = new QHBoxLayout;
	userCardLayout->setContentsMargins(0, 0, 0, 0);
	userCardLayout->setSpacing(3);
	userCardLayout->addLayout(d->userButtonVLayout);
	userCardLayout->addWidget(d->userInfoCard);

	/// Search bar and button group
	// navigation button
	d->navigationButton = new CUVAwesomeToolButton(this);
	d->navigationButton->setFixedSize(40, 38);
	d->navigationButton->setAweSomeIcon(UVIcon::CUVAweSomeIcon::Bars);
	d->navigationButton->setBorderRadius(8);
	connect(d->navigationButton, &CUVAwesomeToolButton::clicked, d, &CUVNavigationBarPrivate::slotNavigationButtonClicked);
	// search button
	d->searchButton = new CUVAwesomeToolButton(this);
	d->searchButton->setFixedSize(40, 38);
	d->searchButton->setAweSomeIcon(UVIcon::CUVAweSomeIcon::MagnifyingGlass);
	d->searchButton->setBorderRadius(8);
	d->searchButton->setVisible(false);
	connect(d->searchButton, &CUVAwesomeToolButton::clicked, d, &CUVNavigationBarPrivate::slotNavigationButtonClicked);
	// navigation suggest box
	d->navigationSuggestBox = new CUVSuggestBox(this);
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
	connect(d->navigationSuggestBox, &CUVSuggestBox::sigSuggestionClicked, this, [=](const QString& suggestText, const QVariantMap& suggestData) {
		if (suggestData.value("CUVNodeType").toString() == "Stacked") {
			if (const CUVNavigationNode* node = d->navigationModel->getNavigationNode(suggestData.value("CUVPageKey").toString())) {
				d->slotTreeViewClicked(node->getModelIndex());
			}
		} else {
			if (const CUVNavigationNode* node = d->navigationModel->getNavigationNode(suggestData.value("CUVPageKey").toString())) {
				d->slotFooterViewClicked(node->getModelIndex());
			}
		}
	});

	/// navigation model and view
	// model
	d->navigationModel = new CUVNavigationModel(this);
	// view
	d->navigationView = new CUVNavigationView(this);
	d->navigationView->setModel(d->navigationModel);
	connect(d->navigationView, &CUVNavigationView::sigNavigationClicked, this, [=](const QModelIndex& index) { d->slotTreeViewClicked(index); });
	connect(d->navigationView, &CUVNavigationView::sigNavigationOpenNewWindow, d, &CUVNavigationBarPrivate::slotNavigationOpenNewWindow);

	/// footer model, and delegate
	// model
	d->footerModel = new CUVFooterModel(this);
	// view
	d->footerView = new CUVBaseListView(this);
	d->footerView->setFixedHeight(0);
	d->footerView->setModel(d->footerModel);
	// delegate
	d->footerDelegate = new CUVFooterDelegate(this);
	d->footerDelegate->setCUVBaseListView(d->footerView);
	d->footerView->setItemDelegate(d->footerDelegate);
	connect(d->footerView, &CUVBaseListView::sigMousePress, this, [=](const QModelIndex& index) {
		d->footerDelegate->setPressIndex(index);
		d->footerView->viewport()->update();
	});
	connect(d->footerView, &CUVBaseListView::sigMouseRelease, this, [=](const QModelIndex& index) {
		d->footerDelegate->setPressIndex({});
		d->footerView->viewport()->update();
	});
	connect(d->footerView, &CUVBaseListView::sigMouseDoubleClick, this, [=](const QModelIndex& index) {
		d->footerDelegate->setPressIndex(index);
		d->footerView->viewport()->update();
	});
	connect(d->footerView, &CUVBaseListView::clicked, this, [=](const QModelIndex& index) { d->slotFooterViewClicked(index); });

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
	d->themeMode = UVTheme->getThemeMode();
	connect(UVTheme, &CUVThemeManager::sigThemeModeChanged, this, [=](const UVThemeType::ThemeMode& mode) { d->themeMode = mode; });
}

CUVNavigationBar::~CUVNavigationBar() = default;

void CUVNavigationBar::setUserInfoCardVisible(const bool isVisible) {
	Q_D(CUVNavigationBar);

	d->isShowUserCard = isVisible;
	d->userInfoCard->setVisible(isVisible);
	d->userButton->setVisible(!isVisible);
}

void CUVNavigationBar::setUserInfoCardPixmap(const QPixmap& pix) {
	Q_D(CUVNavigationBar);

	d->userInfoCard->setCardPixmap(pix);
	d->userButton->setPixmap(pix);
}

void CUVNavigationBar::setUserInfoCardTitle(const QString& title) {
	d_func()->userInfoCard->setTitle(title);
}

void CUVNavigationBar::setUserInfoCardSubTitle(const QString& subTitle) {
	d_func()->userInfoCard->setSubTitle(subTitle);
}

UVNavigationType::NodeOperateReturnType CUVNavigationBar::addExpanderNode(const QString& expanderTitle, QString& expanderKey, const UVIcon::CUVAweSomeIcon& awewomeIcon) {
	Q_D(CUVNavigationBar);

	const UVNavigationType::NodeOperateReturnType resType = d->navigationModel->addExpanderNode(expanderTitle, expanderKey, awewomeIcon);
	if (resType == UVNavigationType::Success) {
		d->initNodeModelIndex({});
		d->resetNodeSelected();
	}

	return resType;
}

UVNavigationType::NodeOperateReturnType CUVNavigationBar::addExpanderNode(const QString& expanderTitle, QString& expanderKey, const QString& targetExpanderKey, const UVIcon::CUVAweSomeIcon& awewomeIcon) {
	Q_D(CUVNavigationBar);

	const UVNavigationType::NodeOperateReturnType resType = d->navigationModel->addExpanderNode(expanderTitle, expanderKey, targetExpanderKey, awewomeIcon);
	if (resType == UVNavigationType::Success) {
		d->initNodeModelIndex({});
		d->resetNodeSelected();
	}

	return resType;
}

UVNavigationType::NodeOperateReturnType CUVNavigationBar::addPageNode(const QString& pageTitle, QWidget* page, const UVIcon::CUVAweSomeIcon& awewomeIcon) {
	Q_D(CUVNavigationBar);
	if (!page) {
		return UVNavigationType::PageInvalid;
	}

	QString pageKey{};
	const UVNavigationType::NodeOperateReturnType resType = d->navigationModel->addPageNode(pageTitle, pageKey, awewomeIcon);
	if (resType == UVNavigationType::Success) {
		d->mapPageMeta.insert(pageKey, page->metaObject());
		d->addStackedPage(page, pageKey);
		d->initNodeModelIndex({});
		d->resetNodeSelected();
	}

	return resType;
}

UVNavigationType::NodeOperateReturnType CUVNavigationBar::addPageNode(const QString& pageTitle, QWidget* page, const QString& targetExpanderKey, const UVIcon::CUVAweSomeIcon& awewomeIcon) {
	Q_D(CUVNavigationBar);
	if (!page) {
		return UVNavigationType::PageInvalid;
	}
	if (targetExpanderKey.isEmpty()) {
		return UVNavigationType::TargetNodeInvalid;
	}

	QString pageKey{};
	const UVNavigationType::NodeOperateReturnType resType = d->navigationModel->addPageNode(pageTitle, pageKey, targetExpanderKey, awewomeIcon);
	if (resType == UVNavigationType::Success) {
		d->mapPageMeta.insert(pageKey, page->metaObject());
		CUVNavigationNode* node = d->navigationModel->getNavigationNode(pageKey);
		if (CUVNavigationNode* originalNode = node->getOriginalNode(); d->mapCompactMenu.contains(originalNode)) {
			CUVMenu* menu = d->mapCompactMenu.value(originalNode);
			const QAction* action = menu->addAction(node->getAwesomeIcon(), node->getNodeTitle());
			connect(action, &QAction::triggered, this, [=]() { d->slotTreeViewClicked(node->getModelIndex()); });
		} else {
			const auto menu = new CUVMenu(const_cast<CUVNavigationBar*>(this));
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

UVNavigationType::NodeOperateReturnType CUVNavigationBar::addPageNode(const QString& pageTitle, QWidget* page, const int keyPoints, const UVIcon::CUVAweSomeIcon& awewomeIcon) {
	Q_D(CUVNavigationBar);
	if (!page) {
		return UVNavigationType::PageInvalid;
	}

	QString pageKey{};
	const UVNavigationType::NodeOperateReturnType resType = d->navigationModel->addPageNode(pageTitle, pageKey, keyPoints, awewomeIcon);
	if (resType == UVNavigationType::Success) {
		d->mapPageMeta.insert(pageKey, page->metaObject());
		d->addStackedPage(page, pageKey);
		d->initNodeModelIndex({});
		d->resetNodeSelected();
	}

	return resType;
}

UVNavigationType::NodeOperateReturnType CUVNavigationBar::addPageNode(const QString& pageTitle, QWidget* page, const QString& targetExpanderKey, const int keyPoints, const UVIcon::CUVAweSomeIcon& awewomeIcon) {
	Q_D(CUVNavigationBar);
	if (!page) {
		return UVNavigationType::PageInvalid;
	}
	if (targetExpanderKey.isEmpty()) {
		return UVNavigationType::TargetNodeInvalid;
	}

	QString pageKey{};
	const UVNavigationType::NodeOperateReturnType resType = d->navigationModel->addPageNode(pageTitle, pageKey, targetExpanderKey, keyPoints, awewomeIcon);
	if (resType == UVNavigationType::Success) {
		d->mapPageMeta.insert(pageKey, page->metaObject());
		CUVNavigationNode* node = d->navigationModel->getNavigationNode(pageKey);
		if (CUVNavigationNode* originalNode = node->getOriginalNode(); d->mapCompactMenu.contains(originalNode)) {
			CUVMenu* menu = d->mapCompactMenu.value(originalNode);
			const QAction* action = menu->addAction(node->getAwesomeIcon(), node->getNodeTitle());
			connect(action, &QAction::triggered, this, [=]() { d->slotTreeViewClicked(node->getModelIndex()); });
		} else {
			const auto menu = new CUVMenu(const_cast<CUVNavigationBar*>(this));
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

UVNavigationType::NodeOperateReturnType CUVNavigationBar::addFooterNode(const QString& footerTitle, QString& footerKey, const int keyPoints, const UVIcon::CUVAweSomeIcon& awewomeIcon) {
	return addFooterNode(footerTitle, nullptr, footerKey, keyPoints, awewomeIcon);
}

UVNavigationType::NodeOperateReturnType CUVNavigationBar::addFooterNode(const QString& footerTitle, QWidget* page, QString& footerKey, const int keyPoints, const UVIcon::CUVAweSomeIcon& awewomeIcon) {
	Q_D(CUVNavigationBar);

	const auto resType = d->footerModel->addFooterNode(footerTitle, footerKey, page ? true : false, keyPoints, awewomeIcon);
	if (resType == UVNavigationType::Success) {
		d->addFooterPage(page, footerKey);
	}

	return resType;
}

void CUVNavigationBar::setIsTransparent(const bool isTransparent) {
	d_func()->isTransparent = isTransparent;
	Q_EMIT sigIsTransparentChanged();
}

bool CUVNavigationBar::getIsTransparent() const {
	return d_func()->isTransparent;
}

void CUVNavigationBar::setNodeKeyPoints(const QString& nodeKey, const int keyPoints) {
	Q_D(CUVNavigationBar);

	CUVNavigationNode* node = d->navigationModel->getNavigationNode(nodeKey);
	if (!node || node->getIsExpanderNode() || keyPoints < 0) {
		return;
	}

	node->setKeyPoints(keyPoints);
	node->getIsFooterNode() ? d->footerView->viewport()->update() : d->navigationView->viewport()->update();
}

int CUVNavigationBar::getNodeKeyPoints(const QString& nodeKey) const {
	const CUVNavigationNode* node = d_func()->navigationModel->getNavigationNode(nodeKey);
	if (!node || node->getIsExpanderNode()) {
		return -1;
	}

	return node->getKeyPoints();
}

void CUVNavigationBar::navigation(const QString& pageKey, const bool isLogClicked) {
	Q_D(CUVNavigationBar);

	const CUVNavigationNode* node = d->navigationModel->getNavigationNode(pageKey);
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

void CUVNavigationBar::setDisplayMode(const UVNavigationType::NavigationDisplayMode& displayMode, const bool isAnimation) {
	Q_D(CUVNavigationBar);

	if (d->currentDisplayMode == displayMode || displayMode == UVNavigationType::Auto) {
		return;
	}

	d->doComponentAnimation(displayMode, isAnimation);
	d->raiseNavigationBar();
}

void CUVNavigationBar::paintEvent(QPaintEvent* event) {
	Q_D(CUVNavigationBar);

	if (!d->isTransparent) {
		QPainter painter(this);
		painter.save();
		painter.setPen(UVThemeColor(d->themeMode, UVThemeType::PopupBorder));
		painter.setBrush(UVThemeColor(d->themeMode, UVThemeType::PopupBase));
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
