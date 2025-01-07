#include "e_settings.hpp"

#include <QDebug>
#include <QHBoxLayout>
#include <QMetaEnum>

#include "alapplication.hpp"
#include "alcombobox.hpp"
#include "uvmainwindow.hpp"
#include "uvradiobutton.hpp"
#include "uvscrollpagearea.hpp"
#include "uvtext.hpp"
#include "althememanager.hpp"
#include "uvtoggleswitch.hpp"

E_Settings::E_Settings(QWidget* parent): E_BasePage(parent) {
	m_mainWindow = qobject_cast<CALMainWindow*>(parent);
	setWindowTitle("Settings");

	const auto centralWidget = new QWidget(this);
	centralWidget->setWindowTitle("Settings");
	m_mainVLayout = new QVBoxLayout(centralWidget);
	m_mainVLayout->setContentsMargins(0, 0, 0, 0);
	m_mainVLayout->setSpacing(5);

	/// themeSwitchArea
	initThemeSwitchArea();
	/// micaSwitchArea
	initMicaSwitchArea();
	/// navigationTypeArea
	initNavigationDisplayModeArea();

	m_mainVLayout->addStretch();
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

	const auto themeText = new CUVText("theme", this);
	themeText->setWordWrap(false);
	themeText->setTextPixelSize(18);
	const auto themeComboBox = new CUVComboBox(this);
	for (int i = 0; i < metaEnum.keyCount(); i++) {
		themeComboBox->addItem(metaEnum.key(i));
	}

	const auto themeSwitchArea = new CUVScrollPageArea(this);
	const auto themeSwitchHLayout = new QHBoxLayout(themeSwitchArea);
	const auto themeSwitchText = new CUVText("themeSwitch", this);
	themeSwitchText->setWordWrap(false);
	themeSwitchText->setTextPixelSize(15);
	themeSwitchHLayout->addWidget(themeSwitchText);
	themeSwitchHLayout->addStretch();
	themeSwitchHLayout->addWidget(themeComboBox);
	connect(themeComboBox, &CUVComboBox::currentTextChanged, this, [=](const QString& type) {
		if (metaEnum.isValid()) {
			if (const int value = metaEnum.keyToValue(type.toLocal8Bit().constData()); value != -1) {
				UVTheme->setThemeMode(static_cast<ALThemeType::ThemeMode>(value));
			} else {
				qWarning() << "Invalid theme mode: " << type;
			}
		}
	});
	connect(UVTheme, &CUVThemeManager::sigThemeModeChanged, this, [=](const ALThemeType::ThemeMode& mode) {
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

	m_mainVLayout->addSpacing(30);
	m_mainVLayout->addWidget(themeText);
	m_mainVLayout->addSpacing(10);
	m_mainVLayout->addWidget(themeSwitchArea);
}

void E_Settings::initMicaSwitchArea() {
	const auto helperText = new CUVText("app settings", this);
	helperText->setWordWrap(false);
	helperText->setTextPixelSize(18);
	const auto micaSwitchButton = new CUVToggleSwitch(this);
	const auto micaSwitchArea = new CUVScrollPageArea(this);
	const auto micaSwitchHLayout = new QHBoxLayout(micaSwitchArea);
	const auto micaSwitchText = new CUVText("micaSwitch(cross-platform)", this);
	micaSwitchText->setWordWrap(false);
	micaSwitchText->setTextPixelSize(15);
	micaSwitchHLayout->addWidget(micaSwitchText);
	micaSwitchHLayout->addStretch();
	micaSwitchHLayout->addWidget(micaSwitchButton);
	connect(micaSwitchButton, &CUVToggleSwitch::sigToggleChanged, uvApp, &CUVApplication::setIsEnableMica);

	m_mainVLayout->addSpacing(15);
	m_mainVLayout->addWidget(helperText);
	m_mainVLayout->addSpacing(10);
	m_mainVLayout->addWidget(micaSwitchArea);
}

void E_Settings::initNavigationDisplayModeArea() {
	const QMetaObject& metaObject = ALNavigationType::staticMetaObject;
	const QMetaEnum metaEnum = metaObject.enumerator(metaObject.indexOfEnumerator("NavigationDisplayMode"));
	if (!metaEnum.isValid()) {
		qWarning() << "Failed to retrieve NavigationDisplayMode meta information.";
		return;
	}

	const auto navigationDisplayModeArea = new CUVScrollPageArea(this);
	const auto navigationDisplayModeHLayout = new QHBoxLayout(navigationDisplayModeArea);
	const auto navigationDisplayModeText = new CUVText("navigationDisplayMode", this);
	navigationDisplayModeText->setWordWrap(false);
	navigationDisplayModeText->setTextPixelSize(15);
	navigationDisplayModeHLayout->addWidget(navigationDisplayModeText);
	navigationDisplayModeHLayout->addStretch();

	for (int i = 0; i < metaEnum.keyCount(); i++) {
		const char* key = metaEnum.key(i);
		const int value = metaEnum.value(i);

		const auto radioButton = new CUVRadioButton(this);
		radioButton->setText(key);
		radioButton->setChecked(m_mainWindow->getNavigationDisplayMode() == static_cast<ALNavigationType::NavigationDisplayMode>(value));
		connect(radioButton, &CUVRadioButton::toggled, this, [=](const bool checked) {
			if (checked) {
				m_mainWindow->setNavigationDisplayMode(static_cast<ALNavigationType::NavigationDisplayMode>(value));
			}
		});

		navigationDisplayModeHLayout->addWidget(radioButton);
	}


	m_mainVLayout->addWidget(navigationDisplayModeArea);
}
