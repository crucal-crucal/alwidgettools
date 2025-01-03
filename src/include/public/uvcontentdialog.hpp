#pragma once

#include "uvbasedialog.hpp"

class CUVContentDialogPrivate;

class CUVWIDGETTOOLS_EXPORT CUVContentDialog : public CUVBaseDialog {
	Q_OBJECT
	Q_DECLARE_PRIVATE(CUVContentDialog)

public:
	explicit CUVContentDialog(QWidget* parent);
	~CUVContentDialog() override;

	Q_SLOT virtual void slotCloseButtonClicked();
	Q_SLOT virtual void slotCancelButtonClicked();

	void setCenterWidget(QWidget* centerWidget);

	void setCloseButtonText(const QString& text);
	void setCancelButtonText(const QString& text);

signals:
	void sigCloseButtonClicked();
	void sigCancelButtonClicked();

protected:
	const QScopedPointer<CUVContentDialogPrivate> d_ptr{ nullptr };

	void keyPressEvent(QKeyEvent* event) override;
};
