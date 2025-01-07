#pragma once

#include <QColor>
#include <QModelIndex>

#include "alwidgettoolsdef.hpp"

class QSlider;

class CALAppBar;
class CUVColorDialog;
class CUVColorPicker;
class CUVColorDisplayModel;
class CUVColorDisplayDelegate;
class CUVColorPreview;
class CUVColorValueSliderStyle;
class CUVBaseListView;
class CUVLineEdit;
class CUVComboBox;
class CUVText;
class CALPushButton;
class CUVAwesomeToolButton;

class CUVColorDialogPrivate final : public QObject {
	Q_OBJECT
	Q_DISABLE_COPY(CUVColorDialogPrivate)
	Q_DECLARE_PUBLIC(CUVColorDialog)

public:
	explicit CUVColorDialogPrivate(CUVColorDialog* q, QObject* parent = nullptr);
	~CUVColorDialogPrivate() override;

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
	CUVColorDialog* const q_ptr{ nullptr };

private:
	ALThemeType::ThemeMode themeMode{};
	QColor currentColor{};

	CALAppBar* appBar{ nullptr };
	CUVColorPicker* colorPicker{ nullptr };
	CUVColorPreview* colorPreview{ nullptr };
	CUVColorValueSliderStyle* colorValueSliderStyle{ nullptr };
	CUVLineEdit* htmlEdit{ nullptr };
	CUVComboBox* modeComboBox{ nullptr };
	CUVLineEdit* firstEdit{ nullptr };
	CUVLineEdit* secondEdit{ nullptr };
	CUVLineEdit* thirdEdit{ nullptr };
	CUVText* firstText{ nullptr };
	CUVText* secondText{ nullptr };
	CUVText* thirdText{ nullptr };
	CUVBaseListView* basicColorView{ nullptr };
	CUVColorDisplayModel* basicColorModel{ nullptr };
	CUVColorDisplayDelegate* basicColorDelegate{ nullptr };
	CUVAwesomeToolButton* addCustomColorButton{ nullptr };
	CUVAwesomeToolButton* removeCustomColorButton{ nullptr };
	CUVBaseListView* customColorView{ nullptr };
	CUVColorDisplayModel* customColorModel{ nullptr };
	CUVColorDisplayDelegate* customColorDelegate{ nullptr };
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
