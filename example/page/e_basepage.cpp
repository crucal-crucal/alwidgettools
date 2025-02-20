#include "e_basepage.hpp"

#include <QHBoxLayout>

#include "altoolbutton.hpp"
#include "almenu.hpp"
#include "altext.hpp"
#include "althememanager.hpp"

using namespace AL;

E_BasePage::E_BasePage(QWidget* parent): CALScrollPage(parent) {
	connect(ALTheme, &CALThemeManager::sigThemeModeChanged, this, [this, parent]() {
		if (!parent) {
			update();
		}
	});
}

E_BasePage::~E_BasePage() = default;

void E_BasePage::createCustomWidget(const QString& desText) {
	// top
	const auto customWidget = new QWidget(this);
	const auto subTitleText = new CALText(this);
	subTitleText->setText("subTitle");
	subTitleText->setTextInteractionFlags(Qt::TextSelectableByMouse);
	subTitleText->setTextPixelSize(11);

	const auto documentationButton = new CALToolButton(this);
	documentationButton->setFixedHeight(35);
	documentationButton->setIsTransparent(false);
	documentationButton->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
	documentationButton->setText("Documentation");
	documentationButton->setAweSomeIcon(ALIcon::AweSomeIcon::FileDoc);
	const auto documentationMenu = new CALMenu(this);
	documentationMenu->addAction(ALIcon::AweSomeIcon::CardsBlank, "CardsBlank");
	documentationMenu->addAction(ALIcon::AweSomeIcon::EarthAmericas, "EarthAmericas");
	documentationButton->setMenu(documentationMenu);

	const auto sourceButton = new CALToolButton(this);
	sourceButton->setFixedHeight(35);
	sourceButton->setIsTransparent(false);
	sourceButton->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
	sourceButton->setText("Source");
	sourceButton->setAweSomeIcon(ALIcon::AweSomeIcon::NfcSymbol);
	const auto sourceMenu = new CALMenu(this);
	sourceMenu->addAction(ALIcon::AweSomeIcon::FireBurner, "FireBurner");
	sourceMenu->addAction(ALIcon::AweSomeIcon::Galaxy, "Galaxy");
	sourceButton->setMenu(sourceMenu);

	const auto themeButton = new CALToolButton(this);
	themeButton->setFixedSize(35, 35);
	themeButton->setIsTransparent(false);
	themeButton->setAweSomeIcon(ALIcon::AweSomeIcon::MoonStars);
	connect(themeButton, &CALToolButton::clicked, this, []() { ALTheme->setThemeMode(ALTheme->getThemeMode() == ALThemeType::Light ? ALThemeType::Dark : ALThemeType::Light); });

	const auto buttonLayout = new QHBoxLayout;
	buttonLayout->addWidget(documentationButton);
	buttonLayout->addSpacing(5);
	buttonLayout->addWidget(sourceButton);
	buttonLayout->addStretch();
	buttonLayout->addWidget(themeButton);
	buttonLayout->addSpacing(15);

	const auto descText = new CALText(this);
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
