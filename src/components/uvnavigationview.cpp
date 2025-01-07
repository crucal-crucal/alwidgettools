#include "uvnavigationview.hpp"

#include <QMouseEvent>
#include <QScroller>

#include "uvmenu.hpp"
#include "uvnavigationstyle.hpp"
#include "uvscrollbar.hpp"

/**
 * @brief \class CUVNavigationView
 * @param parent pointer to the parent class
 */
CUVNavigationView::CUVNavigationView(QWidget* parent): QTreeView(parent) {
	setObjectName("CUVNavigationView");
	setStyleSheet("#CUVNavigationView { background-color: transparent; }");
	setAnimated(true);
	setHeaderHidden(true);
	setRootIsDecorated(false);
	setExpandsOnDoubleClick(false);
	setAutoScroll(false);
	setMouseTracking(true);
	setSelectionMode(QAbstractItemView::NoSelection);

	setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

	const auto vertivalScrollBar = new CUVScrollBar(this);
	connect(vertivalScrollBar, &CUVScrollBar::sigRangeAnimationFinished, this, [=]() { doItemsLayout(); });
	setVerticalScrollBar(vertivalScrollBar);
	setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
	const auto floatVerticalScrollBar = new CUVScrollBar(vertivalScrollBar, this);
	floatVerticalScrollBar->setIsAnimation(true);

	m_navigationStyle = new CUVNavigationStyle(this->style());
	m_navigationStyle->setNavigationView(this);
	setStyle(m_navigationStyle);

	QScroller::grabGesture(this->viewport(), QScroller::LeftMouseButtonGesture);
	QScroller* scroller = QScroller::scroller(this->viewport());
	QScrollerProperties properties = scroller->scrollerProperties();
	properties.setScrollMetric(QScrollerProperties::MousePressEventDelay, 0);
	properties.setScrollMetric(QScrollerProperties::HorizontalOvershootPolicy, QScrollerProperties::OvershootAlwaysOff);
	properties.setScrollMetric(QScrollerProperties::VerticalOvershootPolicy, QScrollerProperties::OvershootAlwaysOn);
	properties.setScrollMetric(QScrollerProperties::OvershootDragResistanceFactor, 0.35);
	properties.setScrollMetric(QScrollerProperties::OvershootScrollTime, 0.5);
	properties.setScrollMetric(QScrollerProperties::FrameRate, QScrollerProperties::Fps60);
	scroller->setScrollerProperties(properties);

	connect(scroller, &QScroller::stateChanged, this, [=](const QScroller::State& newState) {
		if (newState == QScroller::Pressed) {
			m_navigationStyle->setPressIndex(indexAt(mapFromGlobal(QCursor::pos())));
			viewport()->update();
		} else if (newState == QScroller::Scrolling || newState == QScroller::Inactive) {
			m_navigationStyle->setPressIndex({});
		}
	});

	setContextMenuPolicy(Qt::CustomContextMenu);
	connect(this, &CUVNavigationView::customContextMenuRequested, this, &CUVNavigationView::slotCustomContextMenuRequested);
}

CUVNavigationView::~CUVNavigationView() = default;

void CUVNavigationView::navigationNodeStateChanged(const QVariantMap& data) const {
	m_navigationStyle->navigationNodeStateChanged(data);
}

void CUVNavigationView::slotCustomContextMenuRequested(const QPoint& pos) {
	const QModelIndex posIndex = indexAt(pos);
	if (!posIndex.isValid()) {
		return;
	}

	if (const auto posNode = static_cast<CUVNavigationNode*>(posIndex.internalPointer()); !posNode->getIsExpanderNode()) {
		CUVMenu menu;
		menu.setMenuItemHeight(27);
		const QAction* openAction = menu.addAction(ALIcon::AweSomeIcon::ObjectGroup, tr("Opens in a new window"));
		connect(openAction, &QAction::triggered, this, [=]() { Q_EMIT sigNavigationOpenNewWindow(posNode->getNodeKey()); });
		menu.exec(mapToGlobal(pos));
	}
}

void CUVNavigationView::mouseReleaseEvent(QMouseEvent* event) {
	QTreeView::mouseReleaseEvent(event);
	if (event->button() == Qt::LeftButton) {
		if (const QModelIndex index = indexAt(event->pos()); index.isValid()) {
			Q_EMIT sigNavigationClicked(index);
		}
		m_navigationStyle->setPressIndex({});
	}
}

void CUVNavigationView::mouseDoubleClickEvent(QMouseEvent* event) {
	m_navigationStyle->setPressIndex(indexAt(event->pos()));
	viewport()->update();
	QTreeView::mouseDoubleClickEvent(event);
}
