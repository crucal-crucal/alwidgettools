#include "e_settings.hpp"

#include <QDebug>
#include <QHBoxLayout>
#include <QMetaEnum>

#include "alapplication.hpp"
#include "alcombobox.hpp"
#include "almainwindow.hpp"
#include "alradiobutton.hpp"
#include "alscrollpagearea.hpp"
#include "altext.hpp"
#include "althememanager.hpp"
#include "altoggleswitch.hpp"

E_Settings::E_Settings(QWidget* parent): E_BasePage(parent) {
	m_mainWindow = qobject_cast<CALMainWindow*>(parent);
	setWindowTitle("Settings");

	const auto centralWidget = new QWidget(this);
	centralWidget->setWindowTitle("Settings");
	mainVLayout = new QVBoxLayout(centralWidget);
	mainVLayout->setContentsMargins(0, 0, 0, 0);
	mainVLayout->setSpacing(5);

	/// themeSwitchArea
	initThemeSwitchArea();
	/// micaSwitchArea
	initMicaSwitchArea();
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

	const auto themeText = new CALText("theme", this);
	themeText->setWordWrap(false);
	themeText->setTextPixelSize(18);
	const auto themeComboBox = new CALComboBox(this);
	for (int i = 0; i < metaEnum.keyCount(); i++) {
		themeComboBox->addItem(metaEnum.key(i));
	}

	const auto themeSwitchArea = new CALScrollPageArea(this);
	const auto themeSwitchHLayout = new QHBoxLayout(themeSwitchArea);
	const auto themeSwitchText = new CALText("themeSwitch", this);
	themeSwitchText->setWordWrap(false);
	themeSwitchText->setTextPixelSize(15);
	themeSwitchHLayout->addWidget(themeSwitchText);
	themeSwitchHLayout->addStretch();
	themeSwitchHLayout->addWidget(themeComboBox);
	connect(themeComboBox, &CALComboBox::currentTextChanged, this, [=](const QString& type) {
		if (metaEnum.isValid()) {
			if (const int value = metaEnum.keyToValue(type.toLocal8Bit().constData()); value != -1) {
				ALTheme->setThemeMode(static_cast<ALThemeType::ThemeMode>(value));
			} else {
				qWarning() << "Invalid theme mode: " << type;
			}
		}
	});
	connect(ALTheme, &CALThemeManager::sigThemeModeChanged, this, [=](const ALThemeType::ThemeMode& mode) {
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
	const auto helperText = new CALText("app settings", this);
	helperText->setWordWrap(false);
	helperText->setTextPixelSize(18);
	const auto micaSwitchButton = new CALToggleSwitch(this);
	const auto micaSwitchArea = new CALScrollPageArea(this);
	const auto micaSwitchHLayout = new QHBoxLayout(micaSwitchArea);
	const auto micaSwitchText = new CALText("micaSwitch(cross-platform)", this);
	micaSwitchText->setWordWrap(false);
	micaSwitchText->setTextPixelSize(15);
	micaSwitchHLayout->addWidget(micaSwitchText);
	micaSwitchHLayout->addStretch();
	micaSwitchHLayout->addWidget(micaSwitchButton);
	connect(micaSwitchButton, &CALToggleSwitch::sigToggleChanged, alApp, &CALApplication::setIsEnableMica);

	mainVLayout->addSpacing(15);
	mainVLayout->addWidget(helperText);
	mainVLayout->addSpacing(10);
	mainVLayout->addWidget(micaSwitchArea);
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
	const auto navigationDisplayModeText = new CALText("navigationDisplayMode", this);
	navigationDisplayModeText->setWordWrap(false);
	navigationDisplayModeText->setTextPixelSize(15);
	navigationDisplayModeHLayout->addWidget(navigationDisplayModeText);
	navigationDisplayModeHLayout->addStretch();

	for (int i = 0; i < metaEnum.keyCount(); i++) {
		const char* key = metaEnum.key(i);
		const int value = metaEnum.value(i);

		const auto radioButton = new CALRadioButton(this);
		radioButton->setText(key);
		radioButton->setChecked(m_mainWindow->getNavigationDisplayMode() == static_cast<ALNavigationType::NavigationDisplayMode>(value));
		connect(radioButton, &CALRadioButton::toggled, this, [=](const bool checked) {
			if (checked) {
				m_mainWindow->setNavigationDisplayMode(static_cast<ALNavigationType::NavigationDisplayMode>(value));
			}
		});

		navigationDisplayModeHLayout->addWidget(radioButton);
	}


	mainVLayout->addWidget(navigationDisplayModeArea);
}
