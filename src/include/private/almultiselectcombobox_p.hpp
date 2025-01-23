#pragma once

#include <QObject>

#include "alwidgettoolsdef.hpp"

/**
 * @brief \namespace AL
 */
namespace AL {
class CALComboBoxStyle;
class CALComboBoxView;
class CALMultiSelectComboBox;

class CALMultiSelectComboBoxPrivate final : public QObject {
	Q_OBJECT
	Q_DISABLE_COPY(CALMultiSelectComboBoxPrivate)
	Q_DECLARE_PUBLIC(CALMultiSelectComboBox)
	Q_PROPERTY(qreal expandIconRotate MEMBER expandIconRotate NOTIFY sigExpandIconRotateChanged)
	Q_PROPERTY(qreal expandMarkWidth MEMBER expandMarkWidth NOTIFY sigExpandMarkWidthChanged)

public:
	explicit CALMultiSelectComboBoxPrivate(CALMultiSelectComboBox* q, QObject* parent = nullptr);
	~CALMultiSelectComboBoxPrivate() override;
	Q_SLOT void onItemPressed(const QModelIndex& index);

	void setExpandIconRotate(qreal expandIconRotate);
	[[nodiscard]] qreal getExpandIconRotate() const;

	void setExpandMarkWidth(qreal expandMarkWidth);
	[[nodiscard]] qreal getExpandMarkWidth() const;

Q_SIGNALS:
	Q_SIGNAL void sigExpandIconRotateChanged(qreal);
	Q_SIGNAL void sigExpandMarkWidthChanged(qreal);

protected:
	CALMultiSelectComboBox* const q_ptr{ nullptr };

private:
	ALThemeType::ThemeMode themeMode{};
	int borderRadius{};
	qreal expandIconRotate{};
	qreal expandMarkWidth{};

	CALComboBoxStyle* comboBoxStyle{ nullptr };
	CALComboBoxView* comboBoxView{ nullptr };
	QVector<bool> itemSelection{};
	bool isFirstPopup{};
	bool isAllowHidePopup{};
	QString currentText{};
	QStringList selectedTextList{};

	void refreshCurrentIndexs();
	void adjustSelectedVector();
};
}
