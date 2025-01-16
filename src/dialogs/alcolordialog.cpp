#include "alcolordialog.hpp"

#include <QHBoxLayout>
#include <QPainter>

#include "altoolbutton.hpp"
#include "albaselistview.hpp"
#include "alcolordialog_p.hpp"
#include "alcolordisplaydelegate.hpp"
#include "alcolordisplaymodel.hpp"
#include "alcolorpicker.hpp"
#include "alcolorpreview.hpp"
#include "alcolorvaluesliderstyle.hpp"
#include "alcombobox.hpp"
#include "alintvalidator.hpp"
#include "allineedit.hpp"
#include "alpushbutton.hpp"
#include "altext.hpp"
#include "althememanager.hpp"

/**
 * @brief \namespace AL
 */
namespace AL {
/**
 * @brief \class CALColorDialogPrivate
 * Internal class for CALColorDialog
 * @param q pointer to the public class
 * @param parent pointer to the parent class
 */
CALColorDialogPrivate::CALColorDialogPrivate(CALColorDialog* q, QObject* parent): QObject(parent), q_ptr(q) {
}

CALColorDialogPrivate::~CALColorDialogPrivate() = default;

void CALColorDialogPrivate::slotColorPickerColorChanged(const QColor& selectedColor) {
	Q_Q(CALColorDialog);

	QColor valueColor = selectedColor.toHsv();
	valueColor.setHsv(valueColor.hue(), valueColor.saturation(), colorValueSlider->value());
	currentColor = valueColor;
	updateHtmlEditValue();
	updateEditValue();
	updateColorPreview();
	updateColorValueSlider();

	Q_EMIT q->sigCurrentColorChanged();
}

void CALColorDialogPrivate::slotColorValueSliderChanged(const int value) {
	Q_Q(CALColorDialog);

	QColor baseColor = currentColor.toHsv();
	baseColor.setHsv(baseColor.hue(), baseColor.saturation(), colorValueSlider->value());
	q->setCurrentColor(baseColor);
}

void CALColorDialogPrivate::slotColorModeChanged(const int index) {
	if (index == 0) {
		firstText->setText(tr("red"));
		secondText->setText(tr("green"));
		thirdText->setText(tr("blue"));
		firstEdit->setValidator(new CALIntValidator(0, 255, this));
	} else if (index == 1) {
		firstText->setText(tr("hue"));
		secondText->setText(tr("saturation"));
		thirdText->setText(tr("value"));
		firstEdit->setValidator(new CALIntValidator(0, 359, this));
	}
	updateEditValue();
}

void CALColorDialogPrivate::slotHtmlEditFocusOut(const QString& text) const {
	// 自动补全
	if (text == "#") {
		htmlEdit->setText("#000000");
	} else if (text.length() > 1 && text.length() < 7) {
		htmlEdit->setText(completeColorText(text));
	}
}

void CALColorDialogPrivate::slotHtmlEditChanged(const QString& text) {
	if (text.length() > 1) {
		q_func()->setCurrentColor(QColor(completeColorText(text)));
	}
}

void CALColorDialogPrivate::slotColorEditChanged(const QString& text) {
	q_func()->setCurrentColor(getColorFromEdit());
}

void CALColorDialogPrivate::slotBasicColorViewClicked(const QModelIndex& index) {
	q_func()->setCurrentColor(index.data(Qt::UserRole).value<QColor>());
}

void CALColorDialogPrivate::slotCustomColorViewClicked(const QModelIndex& index) {
	if (const auto color = index.data(Qt::UserRole).value<QColor>(); color.isValid()) {
		q_func()->setCurrentColor(color);
	}
}

void CALColorDialogPrivate::slotAddCustomColorButtonClicked() const {
	if (const QModelIndexList selectedIndexs = customColorView->selectionModel()->selectedIndexes(); !selectedIndexs.isEmpty()) {
		customColorModel->replaceDisplayColor(currentColor, selectedIndexs[0].row());
	}
}

void CALColorDialogPrivate::slotRemoveCustomColorButtonClicked() const {
	if (const QModelIndexList selectedIndexs = customColorView->selectionModel()->selectedIndexes(); !selectedIndexs.isEmpty()) {
		customColorModel->replaceDisplayColor({}, selectedIndexs[0].row());
	}
}

void CALColorDialogPrivate::initBasicColor() const {
	QList<QColor> basicColorList;
	basicColorList << QColor(0xF0, 0x87, 0x84)
		<< QColor(0xEB, 0x33, 0x24)
		<< QColor(0x77, 0x43, 0x42)
		<< QColor(0x8E, 0x40, 0x3A)
		<< QColor(0x3A, 0x06, 0x03)
		<< QColor(0x9F, 0xFC, 0xFD)
		<< QColor(0x73, 0xFB, 0xFD)
		<< QColor(0x32, 0x82, 0xF6)
		<< QColor(0x00, 0x23, 0xF5)
		<< QColor(0x00, 0x12, 0x9A)
		<< QColor(0x16, 0x41, 0x7C)
		<< QColor(0x00, 0x0C, 0x7B)
		<< QColor(0xFF, 0xFE, 0x91)
		<< QColor(0xFF, 0xFD, 0x55)
		<< QColor(0xF0, 0x9B, 0x59)
		<< QColor(0xF0, 0x86, 0x50)
		<< QColor(0x78, 0x43, 0x15)
		<< QColor(0x81, 0x7F, 0x26)
		<< QColor(0x7E, 0x84, 0xF7)
		<< QColor(0x73, 0x2B, 0xF5)
		<< QColor(0x35, 0x80, 0xBB)
		<< QColor(0x00, 0x02, 0x3D)
		<< QColor(0x58, 0x13, 0x5E)
		<< QColor(0x3A, 0x08, 0x3E)
		<< QColor(0xA1, 0xFB, 0x8E)
		<< QColor(0xA1, 0xFA, 0x4F)
		<< QColor(0x75, 0xF9, 0x4D)
		<< QColor(0x75, 0xFA, 0x61)
		<< QColor(0x75, 0xFA, 0x8D)
		<< QColor(0x81, 0x80, 0x49)
		<< QColor(0xEF, 0x88, 0xBE)
		<< QColor(0xEE, 0x8A, 0xF8)
		<< QColor(0xEA, 0x3F, 0xF7)
		<< QColor(0xEA, 0x36, 0x80)
		<< QColor(0x7F, 0x82, 0xBB)
		<< QColor(0x75, 0x16, 0x3F)
		<< QColor(0x37, 0x7D, 0x22)
		<< QColor(0x37, 0x7E, 0x47)
		<< QColor(0x36, 0x7E, 0x7F)
		<< QColor(0x50, 0x7F, 0x80)
		<< QColor(0x18, 0x3E, 0x0C)
		<< QColor(0x17, 0x3F, 0x3F)
		<< QColor(0x74, 0x1B, 0x7C)
		<< QColor(0x39, 0x10, 0x7B)
		<< QColor(0x00, 0x00, 0x00)
		<< QColor(0x80, 0x80, 0x80)
		<< QColor(0xC0, 0xC0, 0xC0)
		<< QColor(0xFF, 0xFF, 0xFF);
	basicColorModel->appendDisplayColor(basicColorList);
}

void CALColorDialogPrivate::initCustomColor() const {
	QList<QColor> customColorList{};
	for (int i = 0; i < 24; ++i) {
		customColorList.append(QColor());
	}
	customColorModel->appendDisplayColor(customColorList);
}

void CALColorDialogPrivate::updateHtmlEditValue() const {
	if (!htmlEdit->hasFocus()) {
		// 非编辑模式下, 进行自动补全
		htmlEdit->setText(getHexRgbValue());
	}
}

void CALColorDialogPrivate::updateEditValue() const {
	if (modeComboBox->currentIndex() == 0) {
		firstEdit->setText(QString::number(currentColor.red()));
		secondEdit->setText(QString::number(currentColor.green()));
		thirdEdit->setText(QString::number(currentColor.blue()));
	} else {
		const QColor hsvColor = currentColor.toHsv();
		const int hue = qMin(0, hsvColor.hue());
		firstEdit->setText(QString::number(hue));
		secondEdit->setText(QString::number(hsvColor.saturation()));
		thirdEdit->setText(QString::number(hsvColor.value()));
	}
}

void CALColorDialogPrivate::updateColorPreview() const {
	colorPreview->setBaseColor(currentColor);
	colorPreview->update();
}

void CALColorDialogPrivate::updateColorValueSlider() const {
	colorValueSlider->blockSignals(true);
	colorValueSlider->setValue(currentColor.value());
	colorValueSlider->blockSignals(false);
	colorValueSliderStyle->setBaseColor(currentColor);
	colorValueSlider->update();
}

QString CALColorDialogPrivate::completeColorText(QString text) {
	text.remove("#");
	return "#" + text.rightJustified(6, '0');
}

QString CALColorDialogPrivate::getHexRgbValue() const {
	return QString("#%1%2%3")
	       .arg(currentColor.red(), 2, 16, QChar('0'))
	       .arg(currentColor.green(), 2, 16, QChar('0'))
	       .arg(currentColor.blue(), 2, 16, QChar('0'))
	       .toUpper();
}

QColor CALColorDialogPrivate::getColorFromEdit() const {
	QColor editColor{};
	if (modeComboBox->currentIndex() == 0) { // RGB
		editColor.setRgb(firstEdit->text().toInt(), secondEdit->text().toInt(), thirdEdit->text().toInt());
	} else { // HSV
		editColor.setHsv(firstEdit->text().toInt(), secondEdit->text().toInt(), thirdEdit->text().toInt());
	}

	return editColor;
}


AL_TAKEOVER_NATIVEEVENT_CPP(CALColorDialog, d_func()->appBar)
/**
 * @brief \class CALColorDialog
 * @param parent pointer to the parent class
 */
CALColorDialog::CALColorDialog(QWidget* parent): QDialog(parent), d_ptr(new CALColorDialogPrivate(this, this)) {
	Q_D(CALColorDialog);

	setFixedSize(620, 630);
	setObjectName("CALColorDialog");
	setWindowTitle("CALColorDialog");
	setWindowModality(Qt::ApplicationModal);

	/// appBar
	d->appBar = new CALAppBar(this);
	d->appBar->setAppBarHeight(30);
	d->appBar->setIsStayTop(true);
	d->appBar->setWindowButtonFlags(ALAppBarType::CloseButtonHint);
	d->appBar->setIsDefaultClosed(false);
	connect(d->appBar, &CALAppBar::sigCloseButtonClicked, this, &CALColorDialog::close);
	/// color picker
	const auto colorPickerText = new CALText("edit color", this);
	colorPickerText->setTextPixelSize(17);
	const auto colorPickerTextHLayout = new QHBoxLayout;
	colorPickerTextHLayout->setContentsMargins(0, 0, 0, 0);
	colorPickerTextHLayout->addSpacing(3);
	colorPickerTextHLayout->addWidget(colorPickerText);
	d->colorPicker = new CALColorPicker(this);
	connect(d->colorPicker, &CALColorPicker::sigSelectedColorChanged, d, &CALColorDialogPrivate::slotColorPickerColorChanged);
	const auto colorPickerVLayout = new QVBoxLayout;
	colorPickerVLayout->setContentsMargins(0, 0, 0, 0);
	colorPickerVLayout->addWidget(d->colorPicker);
	colorPickerVLayout->addStretch();
	/// color preview
	d->colorPreview = new CALColorPreview(this);
	const auto colorPreviewVLayout = new QVBoxLayout;
	colorPreviewVLayout->setContentsMargins(0, 0, 0, 0);
	colorPreviewVLayout->addWidget(d->colorPreview);
	colorPreviewVLayout->addStretch();
	/// color value slider
	d->colorValueSlider = new QSlider(this);
	d->colorValueSlider->setFixedHeight(257);
	d->colorValueSlider->setOrientation(Qt::Vertical);
	d->colorValueSlider->setRange(0, 255);
	d->colorValueSlider->setValue(255);
	d->colorValueSliderStyle = new CALColorValueSliderStyle;
	d->colorValueSlider->setStyle(d->colorValueSliderStyle);
	connect(d->colorValueSlider, &QSlider::valueChanged, d, &CALColorDialogPrivate::slotColorValueSliderChanged);
	const auto colorValueSliderVLayout = new QVBoxLayout;
	colorValueSliderVLayout->setContentsMargins(0, 0, 0, 0);
	colorValueSliderVLayout->addWidget(d->colorValueSlider);
	colorValueSliderVLayout->addStretch();
	/// color controller
	d->htmlEdit = new CALLineEdit(this);
	d->htmlEdit->setText("#000000");
	const auto htmlValidator = new CALIntValidator(0x000000, 0xFFFFFF, this);
	htmlValidator->setIsHexMode(true);
	d->htmlEdit->setValidator(htmlValidator);
	d->htmlEdit->setFixedSize(120, 33);
	connect(d->htmlEdit, &CALLineEdit::sigFocusOut, d, &CALColorDialogPrivate::slotHtmlEditFocusOut);
	connect(d->htmlEdit, &CALLineEdit::textChanged, d, &CALColorDialogPrivate::slotHtmlEditChanged);
	/// value preview
	const auto htmlEditHLayout = new QHBoxLayout;
	htmlEditHLayout->setContentsMargins(0, 0, 0, 0);
	htmlEditHLayout->addSpacing(6);
	htmlEditHLayout->addWidget(d->htmlEdit);
	htmlEditHLayout->addStretch();
	d->modeComboBox = new CALComboBox(this);
	d->modeComboBox->setFixedSize(132, 33);
	d->modeComboBox->addItem("RGB");
	d->modeComboBox->addItem("HSV");
	d->modeComboBox->setCurrentIndex(0);
	connect(d->modeComboBox, QOverload<int>::of(&CALComboBox::currentIndexChanged), d, &CALColorDialogPrivate::slotColorModeChanged);
	/// R or H
	d->firstEdit = new CALLineEdit(this);
	d->firstEdit->setText("0");
	d->firstEdit->setFixedSize(120, 33);
	d->firstEdit->setValidator(new CALIntValidator(0, 255, this));
	connect(d->firstEdit, &CALLineEdit::textEdited, d, &CALColorDialogPrivate::slotColorEditChanged);
	d->firstText = new CALText(tr("red"), this);
	d->firstText->setTextPixelSize(13);
	const auto firstEditHLayout = new QHBoxLayout;
	firstEditHLayout->setContentsMargins(0, 0, 0, 0);
	firstEditHLayout->addSpacing(6);
	firstEditHLayout->addWidget(d->firstEdit);
	firstEditHLayout->addWidget(d->firstText);
	firstEditHLayout->addStretch();
	/// G or S
	d->secondEdit = new CALLineEdit(this);
	d->secondEdit->setText("0");
	d->secondEdit->setFixedSize(120, 33);
	d->secondEdit->setValidator(new CALIntValidator(0, 255, this));
	connect(d->secondEdit, &CALLineEdit::textEdited, d, &CALColorDialogPrivate::slotColorEditChanged);
	d->secondText = new CALText(tr("green"), this);
	d->secondText->setTextPixelSize(13);
	const auto secondEditHLayout = new QHBoxLayout;
	secondEditHLayout->setContentsMargins(0, 0, 0, 0);
	secondEditHLayout->addSpacing(6);
	secondEditHLayout->addWidget(d->secondEdit);
	secondEditHLayout->addWidget(d->secondText);
	secondEditHLayout->addStretch();
	/// B or V
	d->thirdEdit = new CALLineEdit(this);
	d->thirdEdit->setText("0");
	d->thirdEdit->setFixedSize(120, 33);
	d->thirdEdit->setValidator(new CALIntValidator(0, 255, this));
	connect(d->thirdEdit, &CALLineEdit::textEdited, d, &CALColorDialogPrivate::slotColorEditChanged);
	d->thirdText = new CALText(tr("blue"), this);
	d->thirdText->setTextPixelSize(13);
	const auto thirdEditHLayout = new QHBoxLayout;
	thirdEditHLayout->setContentsMargins(0, 0, 0, 0);
	thirdEditHLayout->addSpacing(6);
	thirdEditHLayout->addWidget(d->thirdEdit);
	thirdEditHLayout->addWidget(d->thirdText);
	thirdEditHLayout->addStretch();
	/// color edit layout
	const auto paramControlVLayout = new QVBoxLayout;
	paramControlVLayout->setContentsMargins(0, 0, 0, 0);
	paramControlVLayout->setSpacing(10);
	paramControlVLayout->addLayout(htmlEditHLayout);
	paramControlVLayout->addWidget(d->modeComboBox);
	paramControlVLayout->addLayout(firstEditHLayout);
	paramControlVLayout->addLayout(secondEditHLayout);
	paramControlVLayout->addLayout(thirdEditHLayout);
	paramControlVLayout->addStretch();
	const auto colorControlHLayout = new QHBoxLayout;
	colorControlHLayout->setContentsMargins(0, 0, 0, 0);
	colorControlHLayout->setSpacing(0);
	colorControlHLayout->addLayout(colorPickerVLayout);
	colorControlHLayout->addSpacing(5);
	colorControlHLayout->addLayout(colorPreviewVLayout);
	colorControlHLayout->addSpacing(20);
	colorControlHLayout->addLayout(colorValueSliderVLayout);
	colorControlHLayout->addSpacing(10);
	colorControlHLayout->addLayout(paramControlVLayout);
	colorControlHLayout->addStretch();
	/// base color
	d->basicColorView = new CALBaseListView(this);
	d->basicColorView->setFixedSize(380, 170);
	d->basicColorView->setSelectionMode(QListView::NoSelection);
	d->basicColorView->setFlow(QListView::LeftToRight);
	d->basicColorView->setViewMode(QListView::IconMode);
	d->basicColorView->setResizeMode(QListView::Adjust);
	d->basicColorModel = new CALColorDisplayModel(this);
	d->basicColorView->setModel(d->basicColorModel);
	d->basicColorDelegate = new CALColorDisplayDelegate(this);
	d->basicColorView->setItemDelegate(d->basicColorDelegate);
	d->initBasicColor();
	connect(d->basicColorView, &CALBaseListView::clicked, d, &CALColorDialogPrivate::slotBasicColorViewClicked);
	const auto basicColorViewText = new CALText(tr("basic color"), this);
	basicColorViewText->setTextPixelSize(14);
	const auto basicColorVLayout = new QVBoxLayout;
	basicColorVLayout->setContentsMargins(0, 0, 0, 0);
	basicColorVLayout->addWidget(basicColorViewText);
	basicColorVLayout->addSpacing(15);
	basicColorVLayout->addWidget(d->basicColorView);
	basicColorVLayout->addStretch();
	/// custom color
	d->customColorView = new CALBaseListView(this);
	d->customColorView->setFixedSize(200, 170);
	d->customColorView->setSelectionMode(QListView::SingleSelection);
	d->customColorView->setFlow(QListView::LeftToRight);
	d->customColorView->setViewMode(QListView::IconMode);
	d->customColorView->setResizeMode(QListView::Adjust);
	d->customColorModel = new CALColorDisplayModel(this);
	d->customColorView->setModel(d->customColorModel);
	d->customColorDelegate = new CALColorDisplayDelegate(this);
	d->customColorView->setItemDelegate(d->customColorDelegate);
	d->initCustomColor();
	connect(d->customColorView, &CALBaseListView::clicked, d, &CALColorDialogPrivate::slotCustomColorViewClicked);
	/// add & remove button
	d->addCustomColorButton = new CALToolButton(this);
	d->addCustomColorButton->setIsTransparent(false);
	d->addCustomColorButton->setAweSomeIcon(ALIcon::AweSomeIcon::Plus);
	connect(d->addCustomColorButton, &CALToolButton::clicked, d, &CALColorDialogPrivate::slotAddCustomColorButtonClicked);
	d->removeCustomColorButton = new CALToolButton(this);
	d->removeCustomColorButton->setIsTransparent(false);
	d->removeCustomColorButton->setAweSomeIcon(ALIcon::AweSomeIcon::Minus);
	connect(d->removeCustomColorButton, &CALToolButton::clicked, d, &CALColorDialogPrivate::slotRemoveCustomColorButtonClicked);
	const auto customColorViewText = new CALText(tr("custom color"), this);
	customColorViewText->setTextPixelSize(14);
	const auto customColorHLayout = new QHBoxLayout;
	customColorHLayout->setContentsMargins(0, 0, 0, 0);
	customColorHLayout->addWidget(customColorViewText);
	customColorHLayout->addSpacing(35);
	customColorHLayout->addWidget(d->addCustomColorButton);
	customColorHLayout->addWidget(d->removeCustomColorButton);
	customColorHLayout->addStretch();
	const auto customColorVLayout = new QVBoxLayout;
	customColorVLayout->setContentsMargins(0, 0, 0, 0);
	customColorVLayout->addLayout(customColorHLayout);
	customColorVLayout->addSpacing(5);
	customColorVLayout->addWidget(d->customColorView);
	customColorVLayout->addStretch();
	const auto colorDisplayHLayout = new QHBoxLayout;
	colorDisplayHLayout->setContentsMargins(0, 0, 0, 0);
	colorDisplayHLayout->addLayout(basicColorVLayout);
	colorDisplayHLayout->addLayout(customColorVLayout);
	colorDisplayHLayout->addStretch();
	/// ok & cancel button
	d->overButton = new CALPushButton(tr("ok"), this);
	d->overButton->setBorderRadius(6);
	d->overButton->setLightDefaultColor(ALThemeColor(ALThemeType::Light, ALThemeType::PrimaryNormal));
	d->overButton->setLightHoverColor(ALThemeColor(ALThemeType::Light, ALThemeType::PrimaryHover));
	d->overButton->setLightPressedColor(ALThemeColor(ALThemeType::Light, ALThemeType::PrimaryPress));
	d->overButton->setLightTextColor(Qt::white);
	d->overButton->setDarkDefaultColor(ALThemeColor(ALThemeType::Dark, ALThemeType::PrimaryNormal));
	d->overButton->setDarkHoverColor(ALThemeColor(ALThemeType::Dark, ALThemeType::PrimaryHover));
	d->overButton->setDarkPressedColor(ALThemeColor(ALThemeType::Dark, ALThemeType::PrimaryPress));
	d->overButton->setDarkTextColor(Qt::white);
	connect(d->overButton, &CALPushButton::clicked, this, [=]() {
		Q_EMIT sigColorSelected(d->currentColor);
		close();
	});
	d->cancelButton = new CALPushButton(tr("cancel"), this);
	d->cancelButton->setBorderRadius(6);
	connect(d->cancelButton, &CALPushButton::clicked, this, &CALColorDialog::close);
	const auto buttonHLayout = new QHBoxLayout;
	buttonHLayout->setContentsMargins(0, 0, 0, 0);
	buttonHLayout->addWidget(d->overButton);
	buttonHLayout->addWidget(d->cancelButton);
	/// main layout
	const auto mainVLayout = new QVBoxLayout(this);
	mainVLayout->setContentsMargins(20, 10, 11, 20);
	mainVLayout->addLayout(colorPickerTextHLayout);
	mainVLayout->addSpacing(3);
	mainVLayout->addLayout(colorControlHLayout);
	mainVLayout->addSpacing(120);
	mainVLayout->addLayout(colorDisplayHLayout);
	mainVLayout->addLayout(buttonHLayout);
	mainVLayout->addStretch();
	/// theme
	d->themeMode = ALTheme->getThemeMode();
	connect(ALTheme, &CALThemeManager::sigThemeModeChanged, this, [=](const ALThemeType::ThemeMode& mode) { d->themeMode = mode; });
	setCurrentColor(QColor(0x80, 0xFF, 0xEF));
}

CALColorDialog::~CALColorDialog() = default;

void CALColorDialog::setCurrentColor(const QColor& color) {
	Q_D(CALColorDialog);

	d->currentColor = color;
	d->updateHtmlEditValue();
	d->updateEditValue();
	d->updateColorPreview();
	d->colorPicker->setSelectedColor(color);

	Q_EMIT sigCurrentColorChanged();
}

QColor CALColorDialog::getCurrentColor() const {
	return d_func()->currentColor;
}

QList<QColor> CALColorDialog::getCustomColorList() const {
	return d_func()->customColorModel->getDisplayColorList();
}

QColor CALColorDialog::getCustomColor(const int index) const {
	return d_func()->customColorModel->getDisplayColor(index);
}

QString CALColorDialog::getCurrentColorRgb() const {
	return d_func()->getHexRgbValue();
}

void CALColorDialog::paintEvent(QPaintEvent* event) {
	Q_D(CALColorDialog);

	QPainter painter(this);
	painter.save();
	painter.setRenderHint(QPainter::Antialiasing);
	painter.setPen(Qt::NoPen);
	// 背景绘制
	painter.setBrush(ALThemeColor(d->themeMode, ALThemeType::DialogBase));
	painter.drawRect(rect());
	// 按钮背景绘制
	painter.setBrush(ALThemeColor(d->themeMode, ALThemeType::DialogLayoutArea));
	painter.drawRect(QRect(0, height() - 78, width(), 78));
	painter.restore();

	QDialog::paintEvent(event);
}
}
