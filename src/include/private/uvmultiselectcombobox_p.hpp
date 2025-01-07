#pragma once

#include <QObject>

#include "alwidgettoolsdef.hpp"

class CUVComboBoxStyle;
class CUVComboBoxView;
class CUVMultiSelectComboBox;

class CUVMultiSelectComboBoxPrivate final : public QObject {
	Q_OBJECT
	Q_DISABLE_COPY(CUVMultiSelectComboBoxPrivate)
	Q_DECLARE_PUBLIC(CUVMultiSelectComboBox)
	Q_PROPERTY(qreal expandIconRotate MEMBER expandIconRotate NOTIFY sigExpandIconRotateChanged)
	Q_PROPERTY(qreal expandMarkWidth MEMBER expandMarkWidth NOTIFY sigExpandMarkWidthChanged)

public:
	explicit CUVMultiSelectComboBoxPrivate(CUVMultiSelectComboBox* q, QObject* parent = nullptr);
	~CUVMultiSelectComboBoxPrivate() override;
	Q_SLOT void onItemPressed(const QModelIndex& index);

	void setExpandIconRotate(qreal expandIconRotate);
	[[nodiscard]] qreal getExpandIconRotate() const;

	void setExpandMarkWidth(qreal expandMarkWidth);
	[[nodiscard]] qreal getExpandMarkWidth() const;

signals:
	void sigExpandIconRotateChanged(qreal);
	void sigExpandMarkWidthChanged(qreal);

protected:
	CUVMultiSelectComboBox* const q_ptr{ nullptr };

private:
	ALThemeType::ThemeMode themeMode{};
	int borderRadius{};
	qreal expandIconRotate{};
	qreal expandMarkWidth{};

	CUVComboBoxStyle* comboBoxStyle{ nullptr };
	CUVComboBoxView* comboBoxView{ nullptr };
	QVector<bool> itemSelection{};
	bool isFirstPopup{ false };
	bool isAllowHidePopup{ false };
	QString currentText{};
	QStringList selectedTextList{};

	void refreshCurrentIndexs();
	void adjustSelectedVector();
};
