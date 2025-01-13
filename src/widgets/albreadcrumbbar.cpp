#include "albreadcrumbbar.hpp"

#include <QScroller>
#include <QVBoxLayout>

#include "albaselistview.hpp"
#include "albreadcrumbarmodel.hpp"
#include "albreadcrumbbardelegate.hpp"
#include "albreadcrumbbar_p.hpp"

namespace AL {
/**
 * @brief \class CALBreadcrumbBarPrivate
 * Interlnal class for CALBreadcrumbBar
 * @param q pointer to the publc class
 * @param parent pointer to the parent class
 */
CALBreadcrumbBarPrivate::CALBreadcrumbBarPrivate(CALBreadcrumbBar* q, QObject* parent): QObject(parent), q_ptr(q) {
}

CALBreadcrumbBarPrivate::~CALBreadcrumbBarPrivate() = default;

/**
 * @brief \class CALBreadcrumbBar
 * @param parent pointer to the parent class
 */
CALBreadcrumbBar::CALBreadcrumbBar(QWidget* parent): QWidget(parent), d_ptr(new CALBreadcrumbBarPrivate(this, this)) {
	Q_D(CALBreadcrumbBar);

	d->textPixelSize = 18;
	d->isAutoRemove = true;
	setFixedHeight(37);
	setObjectName("CALBreadcrumbBar");
	setStyleSheet("#CALBreadcrumbBar { background-color: transparent; }");
	setMouseTracking(true);

	d->listView = new CALBaseListView(this);
	d->listView->setMinimumHeight(0);
	d->listView->setFlow(QListView::LeftToRight);
	d->listView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	d->listView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

	d->listModel = new CALBreadcrumbBarModel(this);
	d->listView->setModel(d->listModel);

	d->listDelegate = new CALBreadcrumbBarDelegate(this);
	d->listView->setItemDelegate(d->listDelegate);
	connect(d->listView, &CALBaseListView::clicked, this, [=](const QModelIndex& index) {
		if (d->isAutoRemove) {
			if (d->listModel->getBreadcrumbListCount() != 1 && index.row() != d->listModel->getBreadcrumbListCount() * 2 - 2 && index.data(Qt::DisplayRole).toString() != ">") {
				Q_EMIT sigBreadcrumbClicked(index.data(Qt::DisplayRole).toString(), d->listModel->getBreadcrumbList());
				d->listModel->removeBreadcrumb(index.row() / 2 + 1);
			}
		} else {
			Q_EMIT sigBreadcrumbClicked(index.data(Qt::DisplayRole).toString(), d->listModel->getBreadcrumbList());
		}
	});

	QFont textFont = this->font();
	textFont.setLetterSpacing(QFont::AbsoluteSpacing, 0.5);
	textFont.setPixelSize(d->textPixelSize);
	d->listView->setFont(textFont);

	QScroller::grabGesture(d->listView->viewport(), QScroller::LeftMouseButtonGesture);
	QScroller* scroller = QScroller::scroller(d->listView->viewport());
	QScrollerProperties properties = scroller->scrollerProperties();
	properties.setScrollMetric(QScrollerProperties::MousePressEventDelay, 0);
	properties.setScrollMetric(QScrollerProperties::HorizontalOvershootPolicy, QScrollerProperties::OvershootAlwaysOn);
	properties.setScrollMetric(QScrollerProperties::OvershootScrollDistanceFactor, 0.35);
	properties.setScrollMetric(QScrollerProperties::OvershootScrollTime, 0.5);
	properties.setScrollMetric(QScrollerProperties::FrameRate, QScrollerProperties::Fps60);
	scroller->setScrollerProperties(properties);

	connect(scroller, &QScroller::stateChanged, this, [=](const QScroller::State& newState) {
		if (newState == QScroller::Pressed) {
			d->listDelegate->setPressIndex(d->listView->indexAt(d->listView->mapFromGlobal(QCursor::pos())));
			d->listView->viewport()->update();
		} else if (newState == QScroller::Scrolling || newState == QScroller::Inactive) {
			d->listDelegate->setPressIndex({});
		}
	});

	const auto mainVLayout = new QVBoxLayout(this);
	mainVLayout->setContentsMargins(0, 0, 0, 0);
	mainVLayout->addWidget(d->listView);
}

CALBreadcrumbBar::~CALBreadcrumbBar() = default;

void CALBreadcrumbBar::setBreadcrumbList(const QStringList& breadcrumbList) {
	d_func()->listModel->setBreadcrumbList(breadcrumbList);
}

QStringList CALBreadcrumbBar::appendBreadcrumb(const QString& breadCrumb) {
	Q_D(CALBreadcrumbBar);

	d->listModel->appendBreadcrumb(breadCrumb);
	return d->listModel->getBreadcrumbList();
}

QStringList CALBreadcrumbBar::removeBreadcrumb(const QString& breadCrumb) {
	Q_D(CALBreadcrumbBar);

	d->listModel->removeBreadcrumb(breadCrumb);
	return d->listModel->getBreadcrumbList();
}

int CALBreadcrumbBar::getBreadcrumbListCount() const {
	return d_func()->listModel->getBreadcrumbListCount();
}

QStringList CALBreadcrumbBar::getBreadcrumbList() const {
	return d_func()->listModel->getBreadcrumbList();
}

void CALBreadcrumbBar::setTextPixelSize(const int textPixelSize) {
	Q_D(CALBreadcrumbBar);

	if (textPixelSize > 0) {
		d->textPixelSize = textPixelSize;
		QFont textFont = this->font();
		textFont.setLetterSpacing(QFont::AbsoluteSpacing, 0.5);
		textFont.setPixelSize(textPixelSize);
		d->listView->setFont(textFont);
		Q_EMIT sigTextPixelSizeChanged();
	}
}

int CALBreadcrumbBar::getTextPixelSize() const {
	return d_func()->textPixelSize;
}

void CALBreadcrumbBar::setIsAutoRemove(const bool isAutoRemove) {
	d_func()->isAutoRemove = isAutoRemove;
	Q_EMIT sigIsAutoRemoveChanged();
}

bool CALBreadcrumbBar::getIsAutoRemove() const {
	return d_func()->isAutoRemove;
}
} // namespace AL