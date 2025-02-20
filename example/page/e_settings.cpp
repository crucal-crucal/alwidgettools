#include "e_settings.hpp"

#include <QDebug>
#include <QHBoxLayout>
#include <QMetaEnum>

#include "alapplication.hpp"
#include "alcombobox.hpp"
#include "allog.hpp"
#include "almainwindow.hpp"
#include "almessagebar.hpp"
#include "alradiobutton.hpp"
#include "alscrollpagearea.hpp"
#include "altext.hpp"
#include "althememanager.hpp"
#include "altoggleswitch.hpp"

using namespace AL;

E_Settings::E_Settings(QWidget* parent): E_BasePage(parent) {
	m_mainWindow = qobject_cast<CALMainWindow*>(parent);
	setWindowTitle(tr("Settings"));

	const auto centralWidget = new QWidget(this);
	centralWidget->setWindowTitle(this->windowTitle());
	mainVLayout = new QVBoxLayout(centralWidget);
	mainVLayout->setContentsMargins(0, 0, 0, 0);
	mainVLayout->setSpacing(5);

	/// themeSwitchArea
	initThemeSwitchArea();
	/// micaSwitchArea
	initMicaSwitchArea();
	/// logSwitchArea
	initLogSwitchArea();
	/// navigationBarEnabledArea
	initNavigationBarEnabledArea();
	/// navigationTypeArea
	initNavigationDisplayModeArea();

	mainVLayout->addStretch();
	addCentralWidget(centralWidget, true, true, 0);
}

E_Settings::~E_Settings() = default;

void E_Settings::initThemeSwitchArea() {
	const QMetaObject& metaObject = ALThemeType::staticMetaObject;
	const QMetaEnum metaEnum = metaObject.enumerator(metaObject.indexOfEnumerator("ThemeMode"));
	if (!metaEnum.isValid()) {
		qWarning() << "Invalid ThemeMode meta enum";
		return;
	}

	const auto themeText = new CALText(tr("theme"), this);
	themeText->setWordWrap(false);
	themeText->setTextPixelSize(18);
	const auto themeComboBox = new CALComboBox(this);
	for (int i = 0; i < metaEnum.keyCount(); ++i) {
		themeComboBox->addItem(metaEnum.key(i));
	}

	const auto themeSwitchArea = new CALScrollPageArea(this);
	const auto themeSwitchHLayout = new QHBoxLayout(themeSwitchArea);
	const auto themeSwitchText = new CALText(tr("themeSwitch"), this);
	themeSwitchText->setWordWrap(false);
	themeSwitchText->setTextPixelSize(15);
	themeSwitchHLayout->addWidget(themeSwitchText);
	themeSwitchHLayout->addStretch();
	themeSwitchHLayout->addWidget(themeComboBox);
	connect(themeComboBox, &CALComboBox::currentTextChanged, this, [metaEnum](const QString& type) {
		if (metaEnum.isValid()) {
			if (const int value = metaEnum.keyToValue(type.toLocal8Bit().constData()); value != -1) {
				const auto mode = static_cast<ALThemeType::ThemeMode>(value);
				ALTheme->setThemeMode(mode);
				CALMessageBar::success(tr("theme switch"), mode == ALThemeType::Light ? tr("light theme") : tr("black theme"), 2000, ALMessageBarType::Top);
			} else {
				CALMessageBar::error(tr("theme switch"), tr("Switch error value == -1"), 2000, ALMessageBarType::Top);
			}
		}
	});
	connect(ALTheme, &CALThemeManager::sigThemeModeChanged, this, [metaEnum, themeComboBox](const ALThemeType::ThemeMode& mode) {
		if (metaEnum.isValid()) {
			if (const char* key = metaEnum.valueToKey(static_cast<int>(mode))) {
				themeComboBox->blockSignals(true);
				themeComboBox->setCurrentText(QString::fromLocal8Bit(key));
				themeComboBox->blockSignals(false);
			} else {
				qWarning() << "Invalid theme mode: " << mode;
			}
		}
	});

	mainVLayout->addSpacing(30);
	mainVLayout->addWidget(themeText);
	mainVLayout->addSpacing(10);
	mainVLayout->addWidget(themeSwitchArea);
}

void E_Settings::initMicaSwitchArea() {
	const auto helperText = new CALText(tr("app settings"), this);
	helperText->setWordWrap(false);
	helperText->setTextPixelSize(18);
	const auto micaSwitchButton = new CALToggleSwitch(this);
	const auto micaSwitchArea = new CALScrollPageArea(this);
	const auto micaSwitchHLayout = new QHBoxLayout(micaSwitchArea);
	const auto micaSwitchText = new CALText(tr("micaSwitch(cross-platform)"), this);
	micaSwitchText->setWordWrap(false);
	micaSwitchText->setTextPixelSize(15);
	micaSwitchHLayout->addWidget(micaSwitchText);
	micaSwitchHLayout->addStretch();
	micaSwitchHLayout->addWidget(micaSwitchButton);
	connect(micaSwitchButton, &CALToggleSwitch::sigToggleChanged, this, [](const bool toggled) {
		alApp->setIsEnableMica(toggled);
		CALMessageBar::success(tr("mica switch"), toggled ? tr("open mica mode success") : tr("close mica mode success"), 2000, ALMessageBarType::Top);
	});

	mainVLayout->addSpacing(15);
	mainVLayout->addWidget(helperText);
	mainVLayout->addSpacing(10);
	mainVLayout->addWidget(micaSwitchArea);
}

void E_Settings::initLogSwitchArea() {
	const auto logSwitchButton = new CALToggleSwitch(this);
	const auto logSwitchArea = new CALScrollPageArea(this);
	const auto logSwitchHLayout = new QHBoxLayout(logSwitchArea);
	const auto logSwitchText = new CALText(tr("open log"), this);
	logSwitchText->setWordWrap(false);
	logSwitchText->setTextPixelSize(15);
	logSwitchHLayout->addWidget(logSwitchText);
	logSwitchHLayout->addStretch();
	logSwitchHLayout->addWidget(logSwitchButton);
	connect(logSwitchButton, &CALToggleSwitch::sigToggleChanged, this, [](const bool toggled) {
		CALLog::instance()->setMessageLogEnable(toggled);
		CALMessageBar::success(tr("log switch"), toggled ? tr("open log success") : tr("close log success"), 2000, ALMessageBarType::Top);
		if (toggled) {
			qInfo() << "open log to file";
		}
	});

	mainVLayout->addSpacing(10);
	mainVLayout->addWidget(logSwitchArea);
}

void E_Settings::initNavigationDisplayModeArea() {
	const QMetaObject& metaObject = ALNavigationType::staticMetaObject;
	const QMetaEnum metaEnum = metaObject.enumerator(metaObject.indexOfEnumerator("NavigationDisplayMode"));
	if (!metaEnum.isValid()) {
		qWarning() << "Failed to retrieve NavigationDisplayMode meta information.";
		return;
	}

	const auto navigationDisplayModeArea = new CALScrollPageArea(this);
	const auto navigationDisplayModeHLayout = new QHBoxLayout(navigationDisplayModeArea);
	const auto navigationDisplayModeText = new CALText(tr("navigationDisplayMode"), this);
	navigationDisplayModeText->setWordWrap(false);
	navigationDisplayModeText->setTextPixelSize(15);
	navigationDisplayModeHLayout->addWidget(navigationDisplayModeText);
	navigationDisplayModeHLayout->addStretch();

	for (int i = 0; i < metaEnum.keyCount(); ++i) {
		const char* key = metaEnum.key(i);
		const int value = metaEnum.value(i);

		const auto radioButton = new CALRadioButton(this);
		radioButton->setText(key);
		radioButton->setChecked(m_mainWindow->getNavigationDisplayMode() == static_cast<ALNavigationType::NavigationDisplayMode>(value));
		connect(radioButton, &CALRadioButton::toggled, this, [this, radioButton, value](const bool checked) {
			if (checked) {
				m_mainWindow->setNavigationDisplayMode(static_cast<ALNavigationType::NavigationDisplayMode>(value));
				if (m_mainWindow->getIsNavigationBarEnable()) {
					CALMessageBar::success(tr("Navigation Mode switch"), tr("switch to ") + radioButton->text() + tr(" mode success"), 2000, ALMessageBarType::Top);
				} else {
					CALMessageBar::information(tr("Navigation Mode switch"), tr("Navigation Bar is Disabled"), 2000, ALMessageBarType::Top);
				}
			}
		});

		navigationDisplayModeHLayout->addWidget(radioButton);
	}


	mainVLayout->addWidget(navigationDisplayModeArea);
}

void E_Settings::initNavigationBarEnabledArea() {
	const auto navigationBarEnabledButton = new CALToggleSwitch(this);
	navigationBarEnabledButton->setIsToggled(true);
	const auto navigationBarEnabledArea = new CALScrollPageArea(this);
	const auto navigationBarEnabledHLayout = new QHBoxLayout(navigationBarEnabledArea);
	const auto navigationBarEnabledText = new CALText(tr("navigation bar enabled"), this);
	navigationBarEnabledText->setWordWrap(false);
	navigationBarEnabledText->setTextPixelSize(15);
	navigationBarEnabledHLayout->addWidget(navigationBarEnabledText);
	navigationBarEnabledHLayout->addStretch();
	navigationBarEnabledHLayout->addWidget(navigationBarEnabledButton);
	connect(navigationBarEnabledButton, &CALToggleSwitch::sigToggleChanged, m_mainWindow, &CALMainWindow::setIsNavigationBarEnable);

	mainVLayout->addSpacing(10);
	mainVLayout->addWidget(navigationBarEnabledArea);
}
