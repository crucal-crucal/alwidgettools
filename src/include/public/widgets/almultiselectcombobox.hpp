#pragma once

#include <QComboBox>

#include "alwidgettoolsdef.hpp"

/**
 * @brief \namespace AL
 */
namespace AL {
class CALMultiSelectComboBoxPrivate;

class CALWIDGETTOOLS_EXPORT CALMultiSelectComboBox : public QComboBox {
	Q_OBJECT
	Q_DISABLE_COPY(CALMultiSelectComboBox)
	Q_DECLARE_PRIVATE(CALMultiSelectComboBox)
	Q_PROPERTY(int borderRadius READ getBorderRadius WRITE setBorderRadius NOTIFY sigBorderRadiusChanged)

public:
	explicit CALMultiSelectComboBox(QWidget* parent = nullptr);
	~CALMultiSelectComboBox() override;

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
	const QScopedPointer<CALMultiSelectComboBoxPrivate> d_ptr{ nullptr };

	void paintEvent(QPaintEvent* event) override;
	void showPopup() override;
	void hidePopup() override;
};
}
