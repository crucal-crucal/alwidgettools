#pragma once

#include <QComboBox>

#include "uvwidgettoolsdef.hpp"

class CUVMultiSelectComboBoxPrivate;

class CUVWIDGETTOOLS_EXPORT CUVMultiSelectComboBox : public QComboBox {
	Q_OBJECT
	Q_DISABLE_COPY(CUVMultiSelectComboBox)
	Q_DECLARE_PRIVATE(CUVMultiSelectComboBox)
	Q_PROPERTY(int borderRadius READ getBorderRadius WRITE setBorderRadius NOTIFY sigBorderRadiusChanged)

public:
	explicit CUVMultiSelectComboBox(QWidget* parent = nullptr);
	~CUVMultiSelectComboBox() override;

	void setCurrentSelection(const QString& selection);
	void setCurrentSelection(const QStringList& selection);
	void setCurrentSelection(int index);
	void setCurrentSelection(const QList<int>& selectionIndex);
	[[nodiscard]] QStringList getCurrentSelection() const;
	[[nodiscard]] QList<int> getCurrentSelectionIndex() const;
	void setBorderRadius(int radius);
	[[nodiscard]] int getBorderRadius() const;
	void selectAllItems();
	void deselectAllItems();
	[[nodiscard]] bool isAllSelected() const;

signals:
	void sigItemSelectionChanged(const QVector<bool>& itemSelection);
	void sigCurrentTextListChanged(const QStringList& selectedTextList);
	void sigBorderRadiusChanged();

protected:
	const QScopedPointer<CUVMultiSelectComboBoxPrivate> d_ptr{ nullptr };

	void paintEvent(QPaintEvent* event) override;
	void showPopup() override;
	void hidePopup() override;
};
