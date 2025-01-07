#include "albreadcrumbbar.hpp"

#include <QScroller>
#include <QVBoxLayout>

#include "uvbaselistview.hpp"
#include "uvbreadcrumbarmodel.hpp"
#include "uvbreadcrumbbardelegate.hpp"
#include "uvbreadcrumbbar_p.hpp"

/**
 * @brief \class CUVBreadcrumbBarPrivate
 * Interlnal class for CUVBreadcrumbBar
 * @param q pointer to the publc class
 * @param parent pointer to the parent class
 */
CUVBreadcrumbBarPrivate::CUVBreadcrumbBarPrivate(CUVBreadcrumbBar* q, QObject* parent): QObject(parent), q_ptr(q) {
}

CUVBreadcrumbBarPrivate::~CUVBreadcrumbBarPrivate() = default;

/**
 * @brief \class CUVBreadcrumbBar
 * @param parent pointer to the parent class
 */
CUVBreadcrumbBar::CUVBreadcrumbBar(QWidget* parent): QWidget(parent), d_ptr(new CUVBreadcrumbBarPrivate(this, this)) {
	Q_D(CUVBreadcrumbBar);

	d->textPixelSize = 18;
	d->isAutoRemove = true;
	setFixedHeight(37);
	setObjectName("CUVBreadcrumbBar");
	setStyleSheet("#CUVBreadcrumbBar { background-color: transparent; }");
	setMouseTracking(true);

	d->listView = new CUVBaseListView(this);
	d->listView->setMinimumHeight(0);
	d->listView->setFlow(QListView::LeftToRight);
	d->listView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	d->listView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

	d->listModel = new CUVBreadcrumbBarModel(this);
	d->listView->setModel(d->listModel);

	d->listDelegate = new CUVBreadcrumbBarDelegate(this);
	d->listView->setItemDelegate(d->listDelegate);
	connect(d->listView, &CUVBaseListView::clicked, this, [=](const QModelIndex& index) {
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

CUVBreadcrumbBar::~CUVBreadcrumbBar() = default;

void CUVBreadcrumbBar::setBreadcrumbList(const QStringList& breadcrumbList) {
	d_func()->listModel->setBreadcrumbList(breadcrumbList);
}

QStringList CUVBreadcrumbBar::appendBreadcrumb(const QString& breadCrumb) {
	Q_D(CUVBreadcrumbBar);

	d->listModel->appendBreadcrumb(breadCrumb);
	return d->listModel->getBreadcrumbList();
}

QStringList CUVBreadcrumbBar::removeBreadcrumb(const QString& breadCrumb) {
	Q_D(CUVBreadcrumbBar);

	d->listModel->removeBreadcrumb(breadCrumb);
	return d->listModel->getBreadcrumbList();
}

int CUVBreadcrumbBar::getBreadcrumbListCount() const {
	return d_func()->listModel->getBreadcrumbListCount();
}

QStringList CUVBreadcrumbBar::getBreadcrumbList() const {
	return d_func()->listModel->getBreadcrumbList();
}

void CUVBreadcrumbBar::setTextPixelSize(const int textPixelSize) {
	Q_D(CUVBreadcrumbBar);

	if (textPixelSize > 0) {
		d->textPixelSize = textPixelSize;
		QFont textFont = this->font();
		textFont.setLetterSpacing(QFont::AbsoluteSpacing, 0.5);
		textFont.setPixelSize(textPixelSize);
		d->listView->setFont(textFont);
		Q_EMIT sigTextPixelSizeChanged();
	}
}

int CUVBreadcrumbBar::getTextPixelSize() const {
	return d_func()->textPixelSize;
}

void CUVBreadcrumbBar::setIsAutoRemove(const bool isAutoRemove) {
	d_func()->isAutoRemove = isAutoRemove;
	Q_EMIT sigIsAutoRemoveChanged();
}

bool CUVBreadcrumbBar::getIsAutoRemove() const {
	return d_func()->isAutoRemove;
}
