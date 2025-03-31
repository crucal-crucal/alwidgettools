#include "aldrawercontainer.hpp"

#include <QGraphicsOpacityEffect>
#include <QPainter>
#include <QPropertyAnimation>
#include <QVBoxLayout>

#include "althememanager.hpp"

/**
 * @brief \namespace AL
 */
namespace AL {
CALDrawerContainer::CALDrawerContainer(QWidget* parent): QWidget(parent) {
	m_borderRadius = 6;

	m_mainVLayout = new QVBoxLayout(this);
	m_mainVLayout->setContentsMargins(0, 0, 0, 0);

	m_containerWidget = new QWidget(this);
	m_containerWidget->setObjectName("CALDrawerContainerWidget");
	m_containerWidget->setStyleSheet("#CALDrawerContainerWidget { background-color: transparent; }");

	m_containerVLayout = new QVBoxLayout(m_containerWidget);
	m_containerVLayout->setContentsMargins(0, 0, 0, 0);
	m_containerVLayout->setSpacing(0);

	m_mainVLayout->addWidget(m_containerWidget);

	m_opacityEffect = new QGraphicsOpacityEffect(this);
	m_opacityEffect->setOpacity(0);
	setGraphicsEffect(m_opacityEffect);

	m_themeMode = ALTheme->getThemeMode();
	connect(ALTheme, &CALThemeManager::sigThemeModeChanged, this, [this](const ALThemeType::ThemeMode& mode) { m_themeMode = mode; });

	setObjectName("CALDrawerContainer");
	setStyleSheet("#CALDrawerContainer { background-color: transparent; }");
	setMaximumHeight(0);
}

CALDrawerContainer::~CALDrawerContainer() = default;

bool CALDrawerContainer::addDrawer(QWidget* widget) {
	if (!widget) {
		return false;
	}

	m_containerVLayout->addWidget(widget);
	m_drawerWidgetList.append(widget);

	return true;
}

bool CALDrawerContainer::removeDrawer(QWidget* widget) {
	if (!widget) {
		return false;
	}

	m_containerVLayout->removeWidget(widget);
	return m_drawerWidgetList.removeOne(widget);
}

void CALDrawerContainer::doDrawerAnimation(const bool isExpand) {
	if (m_containerVLayout->isEmpty()) {
		return;
	}

	const auto heightAnimation = new QPropertyAnimation(this, "maximumHeight");
	connect(heightAnimation, &QPropertyAnimation::valueChanged, this, [this](const QVariant& value) { setMinimumHeight(value.toInt()); });
	heightAnimation->setEasingCurve(QEasingCurve::OutCubic);
	heightAnimation->setDuration(isExpand ? 300 : 450);
	heightAnimation->setStartValue(maximumHeight());
	heightAnimation->setEndValue(isExpand ? calculateContainertMiniHeight() : 0);
	heightAnimation->start(QAbstractAnimation::DeleteWhenStopped);

	const auto opacityAnimation = new QPropertyAnimation(m_opacityEffect, "opacity");
	opacityAnimation->setEasingCurve(isExpand ? QEasingCurve::InCirc : QEasingCurve::OutCubic);
	opacityAnimation->setDuration(isExpand ? 300 : 0);
	opacityAnimation->setStartValue(m_opacityEffect->opacity());
	opacityAnimation->setEndValue(isExpand ? 1 : 0);
	opacityAnimation->start(QAbstractAnimation::DeleteWhenStopped);
}

void CALDrawerContainer::setBorderRadius(const int radius) {
	m_borderRadius = radius;
}

int CALDrawerContainer::getBorderRadius() const {
	return m_borderRadius;
}

void CALDrawerContainer::paintEvent(QPaintEvent* event) {
	QPainter painter(this);
	painter.save();
	painter.setRenderHint(QPainter::Antialiasing);
	/// background
	painter.setPen(ALThemeColor(m_themeMode, ALThemeType::BasicBorder));
	painter.setBrush(ALThemeColor(m_themeMode, ALThemeType::BasicBaseAlpha));
	const QRect foregroundRect(1, 1 - 2 * m_borderRadius, width() - 2, height() - 2 + 2 * m_borderRadius);
	painter.drawRoundedRect(foregroundRect, m_borderRadius, m_borderRadius);
	/// 分割线绘制
	int drawerHeight = 0;
	for (const auto& widget : m_drawerWidgetList) {
		drawerHeight += widget->height();
		painter.drawLine(0, drawerHeight, width(), drawerHeight);
	}
	painter.restore();
}

int CALDrawerContainer::calculateContainertMiniHeight() const {
	int minimumHeight = 0;
	for (const auto& widget : m_drawerWidgetList) {
		minimumHeight += widget->minimumHeight();
	}
	minimumHeight = qMax(100, minimumHeight);
	return minimumHeight;
}
}
