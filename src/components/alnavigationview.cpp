#include "alnavigationview.hpp"

#include <QMouseEvent>
#include <QScroller>

#include "almenu.hpp"
#include "alnavigationstyle.hpp"
#include "alscrollbar.hpp"
#include "altooltip.hpp"

/**
 * @brief \namespace AL
 */
namespace AL {
/**
 * @brief \class CALNavigationView
 * @param parent pointer to the parent class
 */
CALNavigationView::CALNavigationView(QWidget* parent): QTreeView(parent) {
	setObjectName("CALNavigationView");
	setStyleSheet("#CALNavigationView { background-color: transparent; }");
	setAnimated(true);
	setHeaderHidden(true);
	setRootIsDecorated(false);
	setExpandsOnDoubleClick(false);
	setAutoScroll(false);
	setMouseTracking(true);
	setSelectionMode(QAbstractItemView::NoSelection);
	installEventFilter(this);

	setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

	const auto vertivalScrollBar = new CALScrollBar(this);
	connect(vertivalScrollBar, &CALScrollBar::sigRangeAnimationFinished, this, [this]() { doItemsLayout(); });
	setVerticalScrollBar(vertivalScrollBar);
	setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
	const auto floatVerticalScrollBar = new CALScrollBar(vertivalScrollBar, this);
	floatVerticalScrollBar->setIsAnimation(true);
	floatVerticalScrollBar->installEventFilter(this);

	m_navigationStyle = new CALNavigationStyle(this->style());
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

	connect(scroller, &QScroller::stateChanged, this, [this](const QScroller::State& newState) {
		if (newState == QScroller::Pressed) {
			m_navigationStyle->setPressIndex(indexAt(mapFromGlobal(QCursor::pos())));
			viewport()->update();
		} else if (newState == QScroller::Scrolling || newState == QScroller::Inactive) {
			m_navigationStyle->setPressIndex({});
		}
	});

	setContextMenuPolicy(Qt::CustomContextMenu);
	connect(this, &CALNavigationView::customContextMenuRequested, this, &CALNavigationView::slotCustomContextMenuRequested);

	m_compactToolTip = new CALToolTip(this, false);
}

CALNavigationView::~CALNavigationView() = default;

void CALNavigationView::navigationNodeStateChanged(const QVariantMap& data) const {
	m_navigationStyle->navigationNodeStateChanged(data);
}

void CALNavigationView::slotCustomContextMenuRequested(const QPoint& pos) {
	const QModelIndex posIndex = indexAt(pos);
	if (!posIndex.isValid()) {
		return;
	}

	if (const auto posNode = static_cast<CALNavigationNode*>(posIndex.internalPointer()); !posNode->getIsExpanderNode()) {
		CALMenu menu;
		menu.setMenuItemHeight(27);
		const QAction* openAction = menu.addAction(ALIcon::AweSomeIcon::ObjectGroup, tr("Opens in a new window"));
		connect(openAction, &QAction::triggered, this, [this, posNode]() { Q_EMIT sigNavigationOpenNewWindow(posNode->getNodeKey()); });
		menu.exec(mapToGlobal(pos));
	}
}

void CALNavigationView::mouseReleaseEvent(QMouseEvent* event) {
	QTreeView::mouseReleaseEvent(event);
	if (event->button() == Qt::LeftButton) {
		if (const QModelIndex index = indexAt(event->pos()); index.isValid()) {
			Q_EMIT sigNavigationClicked(index);
		}
		m_navigationStyle->setPressIndex({});
	}
}

void CALNavigationView::mouseDoubleClickEvent(QMouseEvent* event) {
	m_navigationStyle->setPressIndex(indexAt(event->pos()));
	viewport()->update();
	QTreeView::mouseDoubleClickEvent(event);
}

void CALNavigationView::mouseMoveEvent(QMouseEvent* event) {
	if (width() <= 60) {
		const QModelIndex posIndex = indexAt(event->pos());
		if (!posIndex.isValid()) {
			m_compactToolTip->hide();
			return;
		}
		const auto posNode = static_cast<CALNavigationNode*>(posIndex.internalPointer());
		m_compactToolTip->setText(posNode->getNodeTitle());
		m_compactToolTip->updatePos();
		m_compactToolTip->show();
	} else {
		m_compactToolTip->hide();
	}

	QTreeView::mouseMoveEvent(event);
}

bool CALNavigationView::eventFilter(QObject* watched, QEvent* event) {
	switch (event->type()) {
		case QEvent::MouseMove:
		case QEvent::HoverMove: {
			if (width() <= 60) {
				const QModelIndex posIndex = indexAt(mapToGlobal(QCursor::pos()));
				if (!posIndex.isValid()) {
					m_compactToolTip->hide();
					break;
				}
				const auto posNode = static_cast<CALNavigationNode*>(posIndex.internalPointer());
				m_compactToolTip->setText(posNode->getNodeTitle());
				m_compactToolTip->updatePos();
				m_compactToolTip->show();
			} else {
				m_compactToolTip->hide();
			}
			break;
		}
		case QEvent::Leave: {
			m_compactToolTip->hide();
			break;
		}
		default: {
			break;
		}
	}

	return QTreeView::eventFilter(watched, event);
}
}
