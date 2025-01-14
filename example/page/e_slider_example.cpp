#include "e_slider_example.hpp"

#include <QDebug>
#include <QHBoxLayout>
#include <QMetaEnum>

#include "alscrollpagearea.hpp"
#include "alslider.hpp"
#include "altext.hpp"
#include "altoggleswitch.hpp"

using namespace AL;

E_Slider_Example::E_Slider_Example(QWidget* parent): E_BasePage(parent) {
	setWindowTitle("CALSlider");
	createCustomWidget("some thing...");

	const auto centralWidget = new QWidget(this);
	centralWidget->setWindowTitle(this->windowTitle());
	mainVLayout = new QVBoxLayout(centralWidget);
	mainVLayout->setContentsMargins(0, 0, 0, 0);
	mainVLayout->setSpacing(5);

	const auto slider = new CALSlider(this);
	const auto sliderArea = new CALScrollPageArea(this);
	const auto sliderHLayout = new QHBoxLayout(sliderArea);
	const auto sliderText = new CALText("CALSlider", this);
	sliderText->setTextPixelSize(15);
	sliderHLayout->addWidget(sliderText);
	sliderHLayout->addWidget(slider);
	sliderHLayout->addStretch();
	const auto sliderDisable = new CALToggleSwitch(this);
	const auto sliderDisableText = new CALText(tr("Disable"), this);
	sliderDisableText->setTextPixelSize(15);
	connect(sliderDisable, &CALToggleSwitch::sigToggleChanged, slider, &CALSlider::setDisabled);
	sliderHLayout->addWidget(sliderDisableText);
	sliderHLayout->addWidget(sliderDisable);
	sliderHLayout->addSpacing(10);

	mainVLayout->addWidget(sliderArea);

	mainVLayout->addStretch();
	addCentralWidget(centralWidget, true, true, 0);
}

E_Slider_Example::~E_Slider_Example() = default;
