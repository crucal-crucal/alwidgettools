#include "alpovit.hpp"

#include <QScroller>
#include <QVBoxLayout>

#include "alpovitmodel.hpp"
#include "alpovitstyle.hpp"
#include "alpovitview.hpp"
#include "alpovit_p.hpp"
#include "alwinshadowhelper.hpp"

/**
 * @brief \namespace AL
 */
namespace AL {
/**
 * @brief \class CALPovitPrivate
 * Internal class for CALPovit
 * @param q pointer to the public class
 * @param parent pointer to the parent class
 */
CALPovitPrivate::CALPovitPrivate(CALPovit* q, QObject* parent): QObject(parent), q_ptr(q) {
}

CALPovitPrivate::~CALPovitPrivate() = default;

void CALPovitPrivate::checkCurrentIndex() {
	Q_Q(CALPovit);

	if (const QModelIndex currentIndex = view->currentIndex(); currentIndex.row() != style->getCurrentIndex()) {
		view->doCurrentIndexChangedAnimation(currentIndex);
		style->setCurrentIndex(currentIndex.row());
		Q_EMIT q->sigCurrentIndexChanged();
	}
}

/**
 * @brief \class CALPovit
 * @param parent pointer to the parent class
 */
CALPovit::CALPovit(QWidget* parent): QWidget(parent), d_ptr(new CALPovitPrivate(this, this)) {
	Q_D(CALPovit);

	setFixedHeight(40);
	setObjectName("CALPovit");
	setStyleSheet("#CALPovit { background-color: transparent; }");
	setMouseTracking(true);

	d->textPixelSize = 20;
	d->model = new CALPovitModel(this);
	d->view = new CALPovitView(this);
	d->style = new CALPovitStyle(d->view->style());
	d->view->setMinimumHeight(0);
	d->view->setFlow(QListView::LeftToRight);
	d->view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	d->view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	d->view->setModel(d->model);
	d->view->setStyle(d->style);
	d->view->setPovitStyle(d->style);
	connect(d->view, &CALPovitView::clicked, this, [=](const QModelIndex& index) {
		if (index.row() != d->style->getCurrentIndex()) {
			Q_EMIT sigCurrentIndexChanged();
		}
		d->view->doCurrentIndexChangedAnimation(index);
		d->style->setCurrentIndex(index.row());
		Q_EMIT sigPovitClicked(index.row());
	});
	connect(d->view, &CALPovitView::doubleClicked, this, [=](const QModelIndex& index) {
		Q_EMIT sigPovitDoubleClicked(index.row());
	});

	QFont textFont = this->font();
	textFont.setLetterSpacing(QFont::AbsoluteSpacing, 0.5);
	textFont.setPixelSize(d->textPixelSize);
	d->view->setFont(textFont);

	QScroller::grabGesture(d->view->viewport(), QScroller::LeftMouseButtonGesture);
	QScroller* scroller = QScroller::scroller(d->view->viewport());
	QScrollerProperties properties = scroller->scrollerProperties();
	properties.setScrollMetric(QScrollerProperties::MousePressEventDelay, 0);
	properties.setScrollMetric(QScrollerProperties::HorizontalOvershootPolicy, QScrollerProperties::OvershootAlwaysOn);
	properties.setScrollMetric(QScrollerProperties::OvershootDragResistanceFactor, 0.35);
	properties.setScrollMetric(QScrollerProperties::OvershootScrollTime, 0.5);
	properties.setScrollMetric(QScrollerProperties::FrameRate, QScrollerProperties::Fps60);
	scroller->setScrollerProperties(properties);

	connect(scroller, &QScroller::stateChanged, this, [=](const QScroller::State& newState) {
		if (newState == QScroller::Pressed) {
			d->style->setPressIndex(d->view->indexAt(d->view->mapFromGlobal(QCursor::pos())));
			d->view->viewport()->update();
		} else if (newState == QScroller::Inactive || newState == QScroller::Scrolling) {
			d->style->setPressIndex({});
			d->view->viewport()->update();
		}
	});
	const auto mainVLayout = new QVBoxLayout(this);
	mainVLayout->setContentsMargins(0, 0, 0, 0);
	mainVLayout->addWidget(d->view);
}

CALPovit::~CALPovit() = default;

int CALPovit::appendPovit(const QString& povieTitle) {
	return d_func()->model->appendPovit(povieTitle);
}

bool CALPovit::removePovit(const QString& povieTitle) {
	return d_func()->model->removePovit(povieTitle);
}

bool CALPovit::removePovit(const int index) {
	return d_func()->model->removePovit(index);
}

void CALPovit::setTextPixelSize(const int textPixelSize) {
	Q_D(CALPovit);

	if (textPixelSize > 0) {
		d->textPixelSize = textPixelSize;
		QFont textFont = this->font();
		textFont.setLetterSpacing(QFont::AbsoluteSpacing, 0.5);
		textFont.setPixelSize(textPixelSize);
		d->view->setFont(textFont);
		Q_EMIT sigTextPixelSizeChanged();
	}
}

int CALPovit::getTextPixelSize() const {
	return d_func()->textPixelSize;
}

void CALPovit::setCurrentIndex(const int currentindex) {
	Q_D(CALPovit);

	if (currentindex < d->model->getPovitListCount()) {
		const QModelIndex index = d->model->index(currentindex, 0);
		d->view->doCurrentIndexChangedAnimation(index);
		if (index.row() != d->style->getCurrentIndex()) {
			Q_EMIT sigCurrentIndexChanged();
		}
		d->style->setCurrentIndex(currentindex);
	}
}

int CALPovit::currentIndex() const {
	return d_func()->view->currentIndex().row();
}

void CALPovit::setPovitSpacing(const int spacing) {
	Q_D(CALPovit);

	if (spacing >= 0) {
		d->style->setPovitSpacing(spacing);
		d->view->doItemsLayout();
		Q_EMIT sigPovitSpacingChanged();
	}
}

int CALPovit::getPovitSpacing() const {
	return d_func()->style->getPovitSpacing();
}

void CALPovit::setMarkWidth(const int width) {
	Q_D(CALPovit);

	if (width >= 0) {
		d->view->setMarkWidth(width);
		d->view->viewport()->update();
		Q_EMIT sigMarkWidthChanged();
	}
}

int CALPovit::getMarkWidth() const {
	return d_func()->view->getMarkWidth();
}
}
