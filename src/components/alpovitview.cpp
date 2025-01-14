#include "alpovitview.hpp"

#include <QMouseEvent>
#include <QPainter>
#include <QPropertyAnimation>

#include "alpovitstyle.hpp"
#include "alscrollbar.hpp"

/**
 * @brief \namespace AL
 */
namespace AL {
/**
 * @brief \class CALPovitView
 * @param parent pointer to the parent class
 */
CALPovitView::CALPovitView(QWidget* parent): QListView(parent) {
	m_markX = 0;
	m_markWidth = 40;
	m_markAnimationWidth = 0;
	setObjectName("CALPovitView");
	setStyleSheet("#CALPovitView { background-color: transparent; }");
	setMouseTracking(true);
	setVerticalScrollBar(new CALScrollBar(this));
	setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
	setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
	setHorizontalScrollBar(new CALScrollBar(this));
	setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
	setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);
}

CALPovitView::~CALPovitView() = default;

void CALPovitView::doCurrentIndexChangedAnimation(const QModelIndex& index) {
	if (index.row() >= 0) {
		const QRect newIndexRect = visualRect(index);
		const auto markXAnimation = new QPropertyAnimation(this, "markX");
		connect(markXAnimation, &QPropertyAnimation::valueChanged, this, [=]() { update(); });
		markXAnimation->setDuration(300);
		markXAnimation->setEasingCurve(QEasingCurve::InOutSine);
		if (m_style->getCurrentIndex() >= 0) {
			markXAnimation->setStartValue(m_markX);
			markXAnimation->setEndValue(newIndexRect.center().x() - m_markWidth / 2);
		} else {
			markXAnimation->setStartValue(newIndexRect.center().x());
			markXAnimation->setEndValue(newIndexRect.center().x() - m_markWidth / 2);

			const auto markWidthAnimation = new QPropertyAnimation(this, "markAnimationWidth");
			markWidthAnimation->setDuration(300);
			markWidthAnimation->setEasingCurve(QEasingCurve::InOutSine);
			markWidthAnimation->setStartValue(0);
			markWidthAnimation->setEndValue(m_markWidth);
			markWidthAnimation->start(QAbstractAnimation::DeleteWhenStopped);
		}
		markXAnimation->start(QAbstractAnimation::DeleteWhenStopped);
	} else {
		const QRect newIndexRect = visualRect(model()->index(m_style->getCurrentIndex(), 0));
		const auto markXAnimation = new QPropertyAnimation(this, "markX");
		connect(markXAnimation, &QPropertyAnimation::valueChanged, this, [=]() { update(); });
		markXAnimation->setDuration(300);
		markXAnimation->setEasingCurve(QEasingCurve::InOutSine);
		markXAnimation->setStartValue(m_markX);
		markXAnimation->setEndValue(newIndexRect.center().x());
		markXAnimation->start(QAbstractAnimation::DeleteWhenStopped);

		const auto markWidthAnimation = new QPropertyAnimation(this, "markAnimationWidth");
		markWidthAnimation->setDuration(300);
		markWidthAnimation->setEasingCurve(QEasingCurve::InOutSine);
		markWidthAnimation->setStartValue(m_markWidth);
		markWidthAnimation->setEndValue(0);
		markWidthAnimation->start(QAbstractAnimation::DeleteWhenStopped);
	}
	setCurrentIndex(index);
}

void CALPovitView::setMarkX(const int x) {
	m_markX = x;
	Q_EMIT sigMarkXChanged();
}

int CALPovitView::getMarkX() const {
	return m_markX;
}

void CALPovitView::setMarkAnimationWidth(const int width) {
	m_markAnimationWidth = width;
	Q_EMIT sigMarkAnimationWidthChanged();
}

int CALPovitView::getMarkAnimationWidth() const {
	return m_markAnimationWidth;
}

void CALPovitView::setMarkWidth(const int width) {
	m_markWidth = width;
}

int CALPovitView::getMarkWidth() const {
	return m_markWidth;
}

void CALPovitView::setPovitStyle(CALPovitStyle* style) {
	m_style = style;
}

CALPovitStyle* CALPovitView::getPovitStyle() const {
	return m_style;
}

void CALPovitView::mouseReleaseEvent(QMouseEvent* event) {
	if (event->button() == Qt::LeftButton) {
		m_style->setPressIndex({});
		viewport()->update();
	}
	QListView::mouseReleaseEvent(event);
}

void CALPovitView::mouseDoubleClickEvent(QMouseEvent* event) {
	m_style->setPressIndex(indexAt(event->pos()));
	viewport()->update();
	QListView::mouseDoubleClickEvent(event);
}

void CALPovitView::paintEvent(QPaintEvent* event) {
	QPainter painter(viewport());
	painter.save();
	const QRect viewPortRect = viewport()->rect();
	painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);
	painter.setPen(Qt::NoPen);
	painter.setBrush(m_style->getMarkColor());
	painter.drawRoundedRect(QRect(m_markX, viewPortRect.bottom() - 4, m_markAnimationWidth, 3), 3, 3);
	painter.restore();
	QListView::paintEvent(event);
}
}
