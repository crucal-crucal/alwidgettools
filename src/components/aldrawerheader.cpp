#include "aldrawerheader.hpp"

#include <QMouseEvent>
#include <QPainter>
#include <QPropertyAnimation>
#include <QVBoxLayout>

#include "althememanager.hpp"

/**
 * @brief \namespace AL
 */
namespace AL {
CALDrawerHeader::CALDrawerHeader(QWidget* parent): QWidget(parent) {
	m_borderRadius = 6;
	m_isExpand = false;
	m_isPressed = false;
	m_expandIconRotate = 0.0;
	m_themeMode = ALTheme->getThemeMode();
	connect(ALTheme, &CALThemeManager::sigThemeModeChanged, this, [this](const ALThemeType::ThemeMode& mode) { m_themeMode = mode; });

	setFixedHeight(75);
	setMouseTracking(true);
	setObjectName("CALDrawerHeader");
	setStyleSheet("#CALDrawerHeader { background-color: transparent; }");

	m_mainVLayout = new QVBoxLayout(this);
	m_mainVLayout->setContentsMargins(0, 0, 0, 0);
	setContentsMargins(0, 0, 30, 0);
}

CALDrawerHeader::~CALDrawerHeader() = default;

void CALDrawerHeader::setHeaderWidget(QWidget* widget) {
	if (!widget) {
		return;
	}

	if (m_headerWidget) {
		m_mainVLayout->removeWidget(m_headerWidget);
		m_headerWidget->deleteLater();
	}
	m_mainVLayout->addWidget(widget);
	m_headerWidget = widget;
}

void CALDrawerHeader::doExpandOrCollpaseAnimation() {
	const auto rotateAnimation = new QPropertyAnimation(this, "expandIconRotate");
	connect(rotateAnimation, &QPropertyAnimation::valueChanged, this, [this](const QVariant&) { update(); });
	rotateAnimation->setDuration(300);
	rotateAnimation->setEasingCurve(QEasingCurve::InOutSine);
	rotateAnimation->setStartValue(m_expandIconRotate);
	rotateAnimation->setEndValue(m_isExpand ? 180.0 : 0.0);
	rotateAnimation->start(QAbstractAnimation::DeleteWhenStopped);
}

void CALDrawerHeader::setBorderRadius(const int radius) {
	m_borderRadius = radius;
}

int CALDrawerHeader::getBorderRadius() const {
	return m_borderRadius;
}

void CALDrawerHeader::setExpandIconRotate(const qreal rotate) {
	m_expandIconRotate = rotate;
	Q_EMIT sigExpandIconRotateChanged();
}

qreal CALDrawerHeader::getExpandIconRotate() const {
	return m_expandIconRotate;
}

void CALDrawerHeader::setIsExpand(const bool isExpand) {
	this->m_isExpand = isExpand;
}

bool CALDrawerHeader::getIsExpand() const {
	return m_isExpand;
}

bool CALDrawerHeader::event(QEvent* event) {
	switch (event->type()) {
		case QEvent::Enter:
		case QEvent::Leave: {
			update();
			break;
		}
		default: {
			break;
		}
	}

	return QWidget::event(event);
}

void CALDrawerHeader::mousePressEvent(QMouseEvent* event) {
	if (const QWidget* posWidget = childAt(event->pos()); !posWidget || (posWidget->objectName().isEmpty())) {
		m_isPressed = true;
		update();
	}

	QWidget::mousePressEvent(event);
}

void CALDrawerHeader::mouseReleaseEvent(QMouseEvent* event) {
	if (const QWidget* posWidget = childAt(event->pos()); !posWidget || (posWidget->objectName().isEmpty())) {
		m_isPressed = false;
		m_isExpand = !m_isExpand;
		doExpandOrCollpaseAnimation();
		Q_EMIT sigDrawerHeaderClicked(m_isExpand);
	}

	QWidget::mouseReleaseEvent(event);
}

void CALDrawerHeader::mouseMoveEvent(QMouseEvent* event) {
	update();
	QWidget::mouseMoveEvent(event);
}

void CALDrawerHeader::paintEvent(QPaintEvent* event) {
	QPainter painter(this);
	painter.save();
	painter.setRenderHint(QPainter::Antialiasing);
	painter.setPen(ALThemeColor(m_themeMode, ALThemeType::BasicBorder));
	const bool isUnderMouse = underMouse();
	painter.setBrush(ALThemeColor(m_themeMode, m_isPressed ? ALThemeType::BasicSelectedAlpha : isUnderMouse ? ALThemeType::BasicPressAlpha : ALThemeType::BasicBaseAlpha));
	const QRect foregroundRect(1, 1, width() - 2, m_isExpand ? height() + m_borderRadius : height() - 2);
	painter.drawRoundedRect(foregroundRect, m_borderRadius, m_borderRadius);
	/// 底边线绘制
	if (isUnderMouse) {
		painter.setPen(ALThemeColor(m_themeMode, ALThemeType::BasicBorderDeep));
	}
	if (m_isExpand) {
		painter.drawLine(0, height() - 1, width(), height() - 1);
	} else {
		painter.drawLine(m_borderRadius, height() - 1, width() - m_borderRadius, height() - 1);
		painter.drawArc(QRectF(1, height() - 1 - 2 * m_borderRadius, 2 * m_borderRadius, 2 * m_borderRadius), 240 * 16, 30 * 16);
		painter.drawArc(QRectF(width() - 2 * m_borderRadius, height() - 1 - 2 * m_borderRadius, 2 * m_borderRadius, 2 * m_borderRadius), -90 * 16, 30 * 16);
	}
	/// 展开图标绘制
	QFont iconFont(ALIcon::getEnumTypeFontName(ALIcon::Fluent));
	iconFont.setPixelSize(17);
	painter.setFont(iconFont);
	painter.setPen(ALThemeColor(m_themeMode, isEnabled() ? ALThemeType::BasicText : ALThemeType::BasicTextDisable));
	const QRectF expandIconRect(width() - 25, 0, 20, height());
	painter.translate(expandIconRect.x() + expandIconRect.width() / 2.0 - 2.0, expandIconRect.y() + expandIconRect.height() / 2.0);
	painter.rotate(m_expandIconRotate);
	painter.translate(-expandIconRect.x() - expandIconRect.width() / 2.0 + 2.0, -expandIconRect.y() - expandIconRect.height() / 2.0);
	painter.drawText(expandIconRect, Qt::AlignCenter, QChar(static_cast<unsigned short>(ALIcon::FluentIcon::ChevronDown)));
	painter.restore();
}
}
