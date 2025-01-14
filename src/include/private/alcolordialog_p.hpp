#pragma once

#include <QColor>

#include "alwidgettoolsdef.hpp"

class QSlider;

/**
 * @brief \namespace AL
 */
namespace AL {
class CALAppBar;
class CALColorDialog;
class CALColorPicker;
class CALColorDisplayModel;
class CALColorDisplayDelegate;
class CALColorPreview;
class CALColorValueSliderStyle;
class CALBaseListView;
class CALLineEdit;
class CALComboBox;
class CALText;
class CALPushButton;
class CALAwesomeToolButton;

class CALColorDialogPrivate final : public QObject {
	Q_OBJECT
	Q_DISABLE_COPY(CALColorDialogPrivate)
	Q_DECLARE_PUBLIC(CALColorDialog)

public:
	explicit CALColorDialogPrivate(CALColorDialog* q, QObject* parent = nullptr);
	~CALColorDialogPrivate() override;

	Q_SLOT void slotColorPickerColorChanged(const QColor& selectedColor);
	Q_SLOT void slotColorValueSliderChanged(int value);
	Q_SLOT void slotColorModeChanged(int index);
	Q_SLOT void slotHtmlEditFocusOut(const QString& text) const;
	Q_SLOT void slotHtmlEditChanged(const QString& text);
	Q_SLOT void slotColorEditChanged(const QString& text);
	Q_SLOT void slotBasicColorViewClicked(const QModelIndex& index);
	Q_SLOT void slotCustomColorViewClicked(const QModelIndex& index);
	Q_SLOT void slotAddCustomColorButtonClicked() const;
	Q_SLOT void slotRemoveCustomColorButtonClicked() const;

protected:
	CALColorDialog* const q_ptr{ nullptr };

private:
	ALThemeType::ThemeMode themeMode{};
	QColor currentColor{};

	CALAppBar* appBar{ nullptr };
	CALColorPicker* colorPicker{ nullptr };
	CALColorPreview* colorPreview{ nullptr };
	CALColorValueSliderStyle* colorValueSliderStyle{ nullptr };
	CALLineEdit* htmlEdit{ nullptr };
	CALComboBox* modeComboBox{ nullptr };
	CALLineEdit* firstEdit{ nullptr };
	CALLineEdit* secondEdit{ nullptr };
	CALLineEdit* thirdEdit{ nullptr };
	CALText* firstText{ nullptr };
	CALText* secondText{ nullptr };
	CALText* thirdText{ nullptr };
	CALBaseListView* basicColorView{ nullptr };
	CALColorDisplayModel* basicColorModel{ nullptr };
	CALColorDisplayDelegate* basicColorDelegate{ nullptr };
	CALAwesomeToolButton* addCustomColorButton{ nullptr };
	CALAwesomeToolButton* removeCustomColorButton{ nullptr };
	CALBaseListView* customColorView{ nullptr };
	CALColorDisplayModel* customColorModel{ nullptr };
	CALColorDisplayDelegate* customColorDelegate{ nullptr };
	CALPushButton* overButton{ nullptr };
	CALPushButton* cancelButton{ nullptr };
	QSlider* colorValueSlider{ nullptr };

	void initBasicColor() const;
	void initCustomColor() const;
	void updateHtmlEditValue() const;
	void updateEditValue() const;
	void updateColorPreview() const;
	void updateColorValueSlider() const;
	[[nodiscard]] static QString completeColorText(QString text);
	[[nodiscard]] QString getHexRgbValue() const;
	[[nodiscard]] QColor getColorFromEdit() const;
};
}
