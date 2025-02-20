#include "aldockwidgettitlebar.hpp"

#include <QHBoxLayout>
#include <QPainter>

#include "aldockwidget.hpp"
#include "aliconbutton.hpp"
#include "altext.hpp"
#include "althememanager.hpp"

/**
 * @brief \namespace AL
 */
namespace AL {
/**
 * @brief \class CALDockWidgetTitleBar
 * @param parent pointer to the CALDockWidget class
 */
CALDockWidgetTitleBar::CALDockWidgetTitleBar(QWidget* parent): QWidget(parent) {
	m_dockWidget = dynamic_cast<CALDockWidget*>(parent);
	m_iconLabel = new QLabel(this);
	m_iconLabel->setPixmap(m_dockWidget->windowIcon().pixmap(QSize(18, 18)));
	m_titleLabel = new CALText(m_dockWidget->windowTitle(), this);
	m_titleLabel->setWordWrap(false);
	m_titleLabel->setTextPixelSize(13);

	m_floatButton = new CALIconButton(ALIcon::FluentIcon::Up, 13, 32, 26, this);
	m_floatButton->setLightHoverColor(ALThemeColor(ALThemeType::Light, ALThemeType::BasicHoverAlpha));
	m_floatButton->setDarkHoverColor(ALThemeColor(ALThemeType::Dark, ALThemeType::BasicHoverAlpha));
	connect(m_floatButton, &CALIconButton::clicked, this, &CALDockWidgetTitleBar::slotFloatButtonClicked);
	m_closeButton = new CALIconButton(ALIcon::FluentIcon::ChromeClose, 13, 32, 26, this);
	m_closeButton->setLightHoverColor(ALThemeColor(ALThemeType::Light, ALThemeType::BasicHoverAlpha));
	m_closeButton->setDarkHoverColor(ALThemeColor(ALThemeType::Dark, ALThemeType::BasicHoverAlpha));
	connect(m_closeButton, &CALIconButton::clicked, this, &CALDockWidgetTitleBar::slotCloseButtonClicked);

	setVisibleFromFeatures(m_dockWidget->features());
	connect(m_dockWidget, &CALDockWidget::featuresChanged, this, [this](const QDockWidget::DockWidgetFeatures& features) { setVisibleFromFeatures(features); });
	connect(m_dockWidget, &CALDockWidget::windowTitleChanged, this, [this](const QString& title) { m_titleLabel->setText(title); });
	connect(m_dockWidget, &CALDockWidget::windowIconChanged, this, [this](const QIcon& icon) { m_iconLabel->setPixmap(icon.pixmap(QSize(18, 18))); });

	const auto mainHLayout = new QHBoxLayout(this);
	mainHLayout->setContentsMargins(0, 0, 0, 0);
	mainHLayout->setSpacing(0);
	mainHLayout->addSpacing(5);
	mainHLayout->addWidget(m_iconLabel);
	mainHLayout->addSpacing(10);
	mainHLayout->addWidget(m_titleLabel);
	mainHLayout->addSpacing(10);
	mainHLayout->addStretch();
	mainHLayout->addWidget(m_floatButton);
	mainHLayout->addWidget(m_closeButton);

	m_themeMode = ALTheme->getThemeMode();
	connect(ALTheme, &CALThemeManager::sigThemeModeChanged, this, [this](const ALThemeType::ThemeMode& mode) { m_themeMode = mode; });
}

CALDockWidgetTitleBar::~CALDockWidgetTitleBar() = default;

void CALDockWidgetTitleBar::slotFloatButtonClicked() const {
	m_dockWidget->setFloating(!m_dockWidget->isFloating());
	changeFloatButtonIcon(m_dockWidget->isFloating());
}

void CALDockWidgetTitleBar::slotCloseButtonClicked() const {
	m_dockWidget->close();
}

void CALDockWidgetTitleBar::changeFloatButtonIcon(const bool isFloating) const {
	m_floatButton->setFluentIcon(isFloating ? ALIcon::FluentIcon::Down : ALIcon::FluentIcon::Up);
}

void CALDockWidgetTitleBar::paintEvent(QPaintEvent* event) {
	QPainter painter(this);
	painter.save();
	painter.setRenderHint(QPainter::Antialiasing);
	painter.setPen(Qt::NoPen);
	painter.setBrush(m_dockWidget->isFloating() ? Qt::transparent : ALThemeColor(m_themeMode, ALThemeType::BasicBaseAlpha));
	painter.drawRect(rect());
	painter.restore();
}

void CALDockWidgetTitleBar::setVisibleFromFeatures(const QDockWidget::DockWidgetFeatures& features) const {
	m_floatButton->setVisible(features.testFlag(QDockWidget::DockWidgetFloatable));
	m_closeButton->setVisible(features.testFlag(QDockWidget::DockWidgetClosable));
}
}
