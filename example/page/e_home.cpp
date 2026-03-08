#include "e_home.hpp"

#include <QVBoxLayout>

#include "alacrylicurlcard.hpp"
#include "alflowlayout.hpp"
#include "alimagecard.hpp"
#include "almessagebar.hpp"
#include "alpopularcard.hpp"
#include "alscrollarea.hpp"
#include "altext.hpp"

using namespace AL;

E_Home::E_Home(QWidget* parent): E_BasePage(parent) {
	setWindowTitle("Home");
	setTitleVisible(false);
	setContentsMargins(2, 2, 0, 0);

	const auto centralWidget = new QWidget(this);
	centralWidget->setWindowTitle(this->windowTitle());
	mainVLayout = new QVBoxLayout(centralWidget);
	mainVLayout->setContentsMargins(0, 0, 0, 0);
	mainVLayout->setSpacing(5);

	/// titleCardArea
	initTitleCardArea();
	/// popularCardArea
	initPopularArea();

	mainVLayout->addStretch();
	addCentralWidget(centralWidget);
}

E_Home::~E_Home() = default;

void E_Home::initTitleCardArea() {
	const auto desText = new CALText("QWidget", this);
	desText->setTextPixelSize(18);
	const auto titleText = new CALText("alwidgettools", this);
	titleText->setTextPixelSize(30);
	const auto titleVLayout = new QVBoxLayout;
	titleVLayout->setContentsMargins(30, 60, 0, 0);
	titleVLayout->addWidget(desText);
	titleVLayout->addWidget(titleText);
	const auto backgroundCard = new CALImageCard(this);
	backgroundCard->setBorderRadius(10);
	backgroundCard->setFixedHeight(400);
	backgroundCard->setMaximumAspectRatio(1.7);
	backgroundCard->setCardImage(QImage(":example/promotion/promotion4.jpg"));
	const auto urlCard1 = new CALAcrylicUrlCard(this);
	urlCard1->setCardPixmapSize({ 62, 62 });
	urlCard1->setFixedSize(195, 225);
	urlCard1->setTitlePixelSize(17);
	urlCard1->setTitleSpacing(25);
	urlCard1->setSubTitlePixelSize(13);
	urlCard1->setTitle("urlCard1");
	urlCard1->setSubTitle("A Qt Widget Library");
	urlCard1->setCardPixmap(QPixmap(":example/promotion/promotion2.jpg"));
	urlCard1->setUrl("https://github.com/crucal-crucal/alwidgettools");
	connect(urlCard1, &CALAcrylicUrlCard::sigOpenError, this, [this](const QString& errMsg) { CALMessageBar::error(tr("open error"), errMsg, 2000, ALMessageBarType::Top, window()); });
	const auto urlCard2 = new CALAcrylicUrlCard(this);
	urlCard2->setCardPixmapSize({ 62, 62 });
	urlCard2->setFixedSize(195, 225);
	urlCard2->setTitlePixelSize(17);
	urlCard2->setTitleSpacing(25);
	urlCard2->setSubTitlePixelSize(13);
	urlCard2->setTitle("urlCard2");
	urlCard2->setSubTitle("A Qt Widget Library");
	urlCard2->setCardPixmap(QPixmap(":example/promotion/promotion3.jpg"));
	urlCard2->setUrl("https://github.com/crucal-crucal/alwidgettools");
	connect(urlCard2, &CALAcrylicUrlCard::sigOpenError, this, [this](const QString& errMsg) { CALMessageBar::error(tr("open error"), errMsg, 2000, ALMessageBarType::Top, window()); });
	const auto cardScrollArea = new CALScrollArea(this);
	cardScrollArea->setWidgetResizable(true);
	cardScrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	cardScrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	cardScrollArea->setIsGrabGesture(true, 0);
	cardScrollArea->setIsOverShoot(Qt::Horizontal, true);
	const auto cardScrollAreaWidget = new QWidget(this);
	cardScrollAreaWidget->setStyleSheet("background-color: transparent;");
	cardScrollArea->setWidget(cardScrollAreaWidget);
	const auto urlCardHLayout = new QHBoxLayout;
	urlCardHLayout->setContentsMargins(30, 0, 0, 0);
	urlCardHLayout->setSpacing(15);
	urlCardHLayout->addWidget(urlCard1);
	urlCardHLayout->addWidget(urlCard2);
	urlCardHLayout->addStretch();
	const auto cardScrollAreaVLayout = new QVBoxLayout(cardScrollAreaWidget);
	cardScrollAreaVLayout->setContentsMargins(0, 0, 0, 0);
	cardScrollAreaVLayout->addStretch();
	cardScrollAreaVLayout->addLayout(urlCardHLayout);
	const auto backgroundVLayout = new QVBoxLayout(backgroundCard);
	backgroundVLayout->setContentsMargins(0, 0, 0, 0);
	backgroundVLayout->addLayout(titleVLayout);
	backgroundVLayout->addWidget(cardScrollArea);

	mainVLayout->addWidget(backgroundCard);
	mainVLayout->addSpacing(20);
}

void E_Home::initPopularArea() {
	const auto flowText = new CALText(tr("Recommendation card"), this);
	flowText->setTextPixelSize(20);
	const auto flowTextHAlyout = new QHBoxLayout;
	flowTextHAlyout->setContentsMargins(33, 0, 0, 0);
	flowTextHAlyout->addWidget(flowText);

	const auto flowLayout = new CALFlowLayout(0, 5, 5);
	flowLayout->setContentsMargins(30, 0, 0, 0);
	flowLayout->setIsAnimation(true);

	const auto toggleSwitchCard = new CALPopularCard(this);
	toggleSwitchCard->setCardPixmap(QPixmap(":example/home/toggleswitch.png"));
	toggleSwitchCard->setTitle("Toggle Switch");
	toggleSwitchCard->setSubTitle("A custom toggleSwitch widget");
	toggleSwitchCard->setDetailedText("Toggle Switch is a custom widget that provides a toggle switch with a custom style and animation effect.");
	toggleSwitchCard->setCardFloatPixmap(QPixmap(":example/home/toggleswitch.png"));
	connect(toggleSwitchCard, &CALPopularCard::sigPopularCardButtonClicked, this, &E_Home::sigToggleSwitchCardClicked);
	flowLayout->addWidget(toggleSwitchCard);

	const auto spinBoxCard = new CALPopularCard(this);
	spinBoxCard->setCardPixmap(QPixmap(":example/home/spinbox.png"));
	spinBoxCard->setTitle("Spin Box");
	spinBoxCard->setSubTitle("A custom spinBox widget");
	spinBoxCard->setDetailedText("Spin Box is a custom widget that provides a spin box with a custom style and animation effect.");
	spinBoxCard->setCardFloatPixmap(QPixmap(":example/home/spinbox.png"));
	connect(spinBoxCard, &CALPopularCard::sigPopularCardButtonClicked, this, &E_Home::sigSpinBoxCardClicked);
	flowLayout->addWidget(spinBoxCard);

	const auto sliderCard = new CALPopularCard(this);
	sliderCard->setCardPixmap(QPixmap(":example/home/slider.png"));
	sliderCard->setTitle("Slider");
	sliderCard->setSubTitle("A custom slider widget");
	sliderCard->setDetailedText("Slider is a custom widget that provides a slider with a custom style and animation effect.");
	sliderCard->setCardFloatPixmap(QPixmap(":example/home/slider.png"));
	connect(sliderCard, &CALPopularCard::sigPopularCardButtonClicked, this, &E_Home::sigSliderCardClicked);
	flowLayout->addWidget(sliderCard);

	const auto radioButtonCard = new CALPopularCard(this);
	radioButtonCard->setCardPixmap(QPixmap(":example/home/radiobutton.png"));
	radioButtonCard->setTitle("Radio Button");
	radioButtonCard->setSubTitle("A custom radioButton widget");
	radioButtonCard->setDetailedText("Radio Button is a custom widget that provides a radio button with a custom style and animation effect.");
	radioButtonCard->setCardFloatPixmap(QPixmap(":example/home/radiobutton.png"));
	connect(radioButtonCard, &CALPopularCard::sigPopularCardButtonClicked, this, &E_Home::sigRadioButtonCardClicked);
	flowLayout->addWidget(radioButtonCard);

	const auto progressBarCard = new CALPopularCard(this);
	progressBarCard->setCardPixmap(QPixmap(":example/home/progressbar.png"));
	progressBarCard->setTitle("Progress Bar");
	progressBarCard->setSubTitle("A custom progressBar widget");
	progressBarCard->setDetailedText("Progress Bar is a custom widget that provides a progress bar with a custom style and animation effect.");
	progressBarCard->setCardFloatPixmap(QPixmap(":example/home/progressbar.png"));
	connect(progressBarCard, &CALPopularCard::sigPopularCardButtonClicked, this, &E_Home::sigProgressBarCardClicked);
	flowLayout->addWidget(progressBarCard);

	const auto plainTextEditCard = new CALPopularCard(this);
	plainTextEditCard->setCardPixmap(QPixmap(":example/home/edit.png"));
	plainTextEditCard->setTitle("Plain Text Edit");
	plainTextEditCard->setSubTitle("A custom plainTextEdit widget");
	plainTextEditCard->setDetailedText("Plain Text Edit is a custom widget that provides a plain text edit with a custom style and animation effect.");
	plainTextEditCard->setCardFloatPixmap(QPixmap(":example/home/edit.png"));
	connect(plainTextEditCard, &CALPopularCard::sigPopularCardButtonClicked, this, &E_Home::sigPlainTextEditCardClicked);
	flowLayout->addWidget(plainTextEditCard);

	const auto mutiSelectComboBoxCard = new CALPopularCard(this);
	mutiSelectComboBoxCard->setCardPixmap(QPixmap(":example/home/combobox.png"));
	mutiSelectComboBoxCard->setTitle("Muti Select ComboBox");
	mutiSelectComboBoxCard->setSubTitle("A custom mutiSelectcomboBox widget");
	mutiSelectComboBoxCard->setDetailedText("Muti Select ComboBox is a custom widget that provides a muti select combobox with a custom style and animation effect.");
	mutiSelectComboBoxCard->setCardFloatPixmap(QPixmap(":example/home/combobox.png"));
	connect(mutiSelectComboBoxCard, &CALPopularCard::sigPopularCardButtonClicked, this, &E_Home::sigMutiSelectComboBoxCardClicked);
	flowLayout->addWidget(mutiSelectComboBoxCard);

	const auto messageBarCard = new CALPopularCard(this);
	messageBarCard->setCardPixmap(QPixmap(":example/home/messagebar.png"));
	messageBarCard->setTitle("Message Bar");
	messageBarCard->setSubTitle("A custom messageBar widget");
	messageBarCard->setDetailedText("Message Bar is a custom widget that provides a message bar with a custom style and animation effect.");
	messageBarCard->setCardFloatPixmap(QPixmap(":example/home/messagebar.png"));
	connect(messageBarCard, &CALPopularCard::sigPopularCardButtonClicked, this, &E_Home::sigMessageBarCardClicked);
	flowLayout->addWidget(messageBarCard);

	const auto circularProgressCard = new CALPopularCard(this);
	circularProgressCard->setCardPixmap(QPixmap(":example/home/circularprogressbar.png"));
	circularProgressCard->setTitle("Circular Progress");
	circularProgressCard->setSubTitle("A custom circularProgress widget");
	circularProgressCard->setDetailedText("Circular Progress is a custom widget that provides a circular progress with a custom style and animation effect.");
	circularProgressCard->setCardFloatPixmap(QPixmap(":example/home/circularprogressbar.png"));
	connect(circularProgressCard, &CALPopularCard::sigPopularCardButtonClicked, this, &E_Home::sigCircularProgressCardClicked);
	flowLayout->addWidget(circularProgressCard);

	const auto checkBoxCard = new CALPopularCard(this);
	checkBoxCard->setCardPixmap(QPixmap(":example/home/checkbox.png"));
	checkBoxCard->setTitle("Check Box");
	checkBoxCard->setSubTitle("A custom checkBox widget");
	checkBoxCard->setDetailedText("Check Box is a custom widget that provides a check box with a custom style and animation effect.");
	checkBoxCard->setCardFloatPixmap(QPixmap(":example/home/checkbox.png"));
	connect(checkBoxCard, &CALPopularCard::sigPopularCardButtonClicked, this, &E_Home::sigCheckBoxCardClicked);
	flowLayout->addWidget(checkBoxCard);

	const auto tabWidgetCard = new CALPopularCard(this);
	tabWidgetCard->setCardPixmap(QPixmap(":example/home/tabwidget.png"));
	tabWidgetCard->setTitle("Tab Widget");
	tabWidgetCard->setSubTitle("A custom tabWidget");
	tabWidgetCard->setDetailedText("Tab Widget is a custom widget that provides a tab widget with a custom style and animation effect.");
	tabWidgetCard->setCardFloatPixmap(QPixmap(":example/home/tabwidget.png"));
	connect(tabWidgetCard, &CALPopularCard::sigPopularCardButtonClicked, this, &E_Home::sigTabWidgetCardClicked);
	flowLayout->addWidget(tabWidgetCard);

	const auto listViewCard = new CALPopularCard(this);
	listViewCard->setCardPixmap(QPixmap(":example/home/listview.png"));
	listViewCard->setTitle("List View");
	listViewCard->setSubTitle("A custom listView widget");
	listViewCard->setDetailedText("List View is a custom widget that provides a list view with a custom style and animation effect.");
	listViewCard->setCardFloatPixmap(QPixmap(":example/home/listview.png"));
	connect(listViewCard, &CALPopularCard::sigPopularCardButtonClicked, this, &E_Home::sigListViewCardClicked);
	flowLayout->addWidget(listViewCard);

	const auto tableView = new CALPopularCard(this);
	tableView->setCardPixmap(QPixmap(":example/home/tableview.png"));
	tableView->setTitle("Table View");
	tableView->setSubTitle("A custom tableView widget");
	tableView->setDetailedText("Table View is a custom widget that provides a table view with a custom style and animation effect.");
	tableView->setCardFloatPixmap(QPixmap(":example/home/tableview.png"));
	connect(tableView, &CALPopularCard::sigPopularCardButtonClicked, this, &E_Home::sigTableViewCardClicked);
	flowLayout->addWidget(tableView);

	const auto treeView = new CALPopularCard(this);
	treeView->setCardPixmap(QPixmap(":example/home/treeview.png"));
	treeView->setTitle("Tree View");
	treeView->setSubTitle("A custom treeView widget");
	treeView->setDetailedText("Tree View is a custom widget that provides a tree view with a custom style and animation effect.");
	treeView->setCardFloatPixmap(QPixmap(":example/home/treeview.png"));
	connect(treeView, &CALPopularCard::sigPopularCardButtonClicked, this, &E_Home::sigTreeViewCardClicked);
	flowLayout->addWidget(treeView);

	mainVLayout->addLayout(flowTextHAlyout);
	mainVLayout->addSpacing(10);
	mainVLayout->addLayout(flowLayout);
}
