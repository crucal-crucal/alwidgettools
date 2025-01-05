#include "e_basepage.hpp"

#include <QHBoxLayout>

#include "uvawesometoolbutton.hpp"
#include "uvmenu.hpp"
#include "uvtext.hpp"
#include "uvthememanager.hpp"

E_BasePage::E_BasePage(QWidget* parent): CUVScrollPage(parent) {
	connect(UVTheme, &CUVThemeManager::sigThemeModeChanged, this, [=]() {
		if (!parent) {
			update();
		}
	});
}

E_BasePage::~E_BasePage() = default;

void E_BasePage::createCustomWidget(const QString& desText) {
	// top
	const auto customWidget = new QWidget(this);
	const auto subTitleText = new CUVText(this);
	subTitleText->setText("subTitle");
	subTitleText->setTextInteractionFlags(Qt::TextSelectableByMouse);
	subTitleText->setTextPixelSize(11);

	const auto documentationButton = new CUVAwesomeToolButton(this);
	documentationButton->setFixedHeight(35);
	documentationButton->setIsTransparent(false);
	documentationButton->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
	documentationButton->setText(tr("Documentation"));
	documentationButton->setAweSomeIcon(UVIcon::CUVAweSomeIcon::FileDoc);
	const auto documentationMenu = new CUVMenu(this);
	documentationMenu->addAction(UVIcon::CUVAweSomeIcon::CardsBlank, tr("CardsBlank"));
	documentationMenu->addAction(UVIcon::CUVAweSomeIcon::EarthAmericas, tr("EarthAmericas"));
	documentationButton->setMenu(documentationMenu);

	const auto sourceButton = new CUVAwesomeToolButton(this);
	sourceButton->setFixedHeight(35);
	sourceButton->setIsTransparent(false);
	sourceButton->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
	sourceButton->setText(tr("Source"));
	sourceButton->setAweSomeIcon(UVIcon::CUVAweSomeIcon::NfcSymbol);
	const auto sourceMenu = new CUVMenu(this);
	sourceMenu->addAction(UVIcon::CUVAweSomeIcon::FireBurner, tr("FireBurner"));
	sourceMenu->addAction(UVIcon::CUVAweSomeIcon::Galaxy, tr("Galaxy"));
	sourceButton->setMenu(sourceMenu);

	const auto themeButton = new CUVAwesomeToolButton(this);
	themeButton->setFixedSize(35, 35);
	themeButton->setIsTransparent(false);
	themeButton->setAweSomeIcon(UVIcon::CUVAweSomeIcon::MoonStars);
	connect(themeButton, &CUVAwesomeToolButton::clicked, this, [=]() { UVTheme->setThemeMode(UVTheme->getThemeMode() == UVThemeType::Light ? UVThemeType::Dark : UVThemeType::Light); });

	const auto buttonLayout = new QHBoxLayout;
	buttonLayout->addWidget(documentationButton);
	buttonLayout->addSpacing(5);
	buttonLayout->addWidget(sourceButton);
	buttonLayout->addStretch();
	buttonLayout->addWidget(themeButton);
	buttonLayout->addSpacing(15);

	const auto descText = new CUVText(this);
	descText->setText(desText);
	descText->setTextPixelSize(13);

	const auto topVLayout = new QVBoxLayout(customWidget);
	topVLayout->setContentsMargins(0, 0, 0, 0);
	topVLayout->addWidget(subTitleText);
	topVLayout->addSpacing(5);
	topVLayout->addLayout(buttonLayout);
	topVLayout->addSpacing(5);
	topVLayout->addWidget(descText);
	setCustomWidget(customWidget);
}
