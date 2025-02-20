#include "alcustomtabwidget.hpp"

#include <QVariant>
#include <QVBoxLayout>

#include "altabbar.hpp"
#include "altabwidget.hpp"

/**
 * @brief \namespace AL
 */
namespace AL {
/**
 * @brief \class CALCustomTabWidget
 * @param parent pointer to the parent class
 */
CALCustomTabWidget::CALCustomTabWidget(QWidget* parent): CALCustomWidget(parent) {
	resize(700, 500);
	setWindowTitle("");
	setWindowIcon(QIcon());
	m_customTabWidget = new CALTabWidget(this);
	QTabBar* originTabBar = m_customTabWidget->tabBar();
	originTabBar->hide();
	m_customTabBar = new CALTabBar(this);
	m_customTabBar->setObjectName("CALCustomTabBar");
	connect(m_customTabBar, &CALTabBar::tabMoved, this, [this](const int form, const int to) { m_customTabWidget->tabBar()->moveTab(form, to); });
	connect(m_customTabBar, &CALTabBar::currentChanged, this, [this](const int index) { m_customTabWidget->setCurrentIndex(index); });
	connect(m_customTabBar, &CALTabBar::tabCloseRequested, originTabBar, &QTabBar::tabCloseRequested);
	connect(m_customTabWidget, &CALTabWidget::currentChanged, this, [this](const int index) {
		if (index == -1) {
			close();
		}
	});

	const auto customWidget = new QWidget(this);
	const auto customVLayout = new QVBoxLayout(customWidget);
	customVLayout->setContentsMargins(10, 0, 10, 0);
	customVLayout->addStretch();
	customVLayout->addWidget(m_customTabBar);
	m_appBar->setCustomWidget(ALAppBarType::LeftArea, customWidget);
	setCenterWidget(m_customTabWidget);
}

CALCustomTabWidget::~CALCustomTabWidget() {
	while (m_customTabWidget->count() > 0) {
		QWidget* closeWidget = m_customTabWidget->widget(0);
		if (const auto originTabWidget = closeWidget->property("CALOriginTabWidget").value<CALTabWidget*>()) {
			closeWidget->setProperty("CurrentCustomTabBar", QVariant::fromValue<CALTabBar*>(nullptr));
			originTabWidget->addTab(closeWidget, m_customTabWidget->tabIcon(0), m_customTabWidget->tabText(0));
		} else {
			m_customTabWidget->removeTab(0);
		}
	}
}

void CALCustomTabWidget::addTab(QWidget* widget, const QIcon& tabIcon, const QString& tabTitle) const {
	m_customTabBar->addTab(tabIcon, tabTitle);
	m_customTabWidget->addTab(widget, tabIcon, tabTitle);
}

CALTabBar* CALCustomTabWidget::getCustomTabBar() const {
	return m_customTabBar;
}

CALTabWidget* CALCustomTabWidget::getCustomTabWidget() const {
	return m_customTabWidget;
}
}
