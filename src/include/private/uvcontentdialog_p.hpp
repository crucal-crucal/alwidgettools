#pragma once

#include <QObject>

class QVBoxLayout;
class CUVPushButton;
class CUVContentDialog;

class CUVContentDialogPrivate final : public QObject {
	Q_OBJECT
	Q_DISABLE_COPY(CUVContentDialogPrivate)
	Q_DECLARE_PUBLIC(CUVContentDialog)

public:
	explicit CUVContentDialogPrivate(CUVContentDialog* q, QObject* parent = nullptr);
	~CUVContentDialogPrivate() override;

protected:
	CUVContentDialog* const q_ptr{ nullptr };

private:
	/// interface
	QWidget* centerWidget{ nullptr };
	QWidget* buttonWidget{ nullptr };
	QVBoxLayout* mainVLayout{ nullptr };
	CUVPushButton* closeButton{ nullptr };
	CUVPushButton* cancelButton{ nullptr };
};
