#pragma once

#include "albasedialog.hpp"

class CALContentDialogPrivate;

class CALWIDGETTOOLS_EXPORT CALContentDialog : public CUVBaseDialog {
	Q_OBJECT
	Q_DECLARE_PRIVATE(CALContentDialog)

public:
	explicit CALContentDialog(QWidget* parent);
	~CALContentDialog() override;

	Q_SLOT virtual void slotCloseButtonClicked();
	Q_SLOT virtual void slotCancelButtonClicked();

	void setCenterWidget(QWidget* centerWidget);

	void setCloseButtonText(const QString& text);
	void setCancelButtonText(const QString& text);

signals:
	void sigCloseButtonClicked();
	void sigCancelButtonClicked();

protected:
	const QScopedPointer<CALContentDialogPrivate> d_ptr{ nullptr };

	void keyPressEvent(QKeyEvent* event) override;
};
