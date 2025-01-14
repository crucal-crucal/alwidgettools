#pragma once

#include <QObject>

class QVBoxLayout;

/**
 * @brief \namespace AL
 */
namespace AL {
class CALPushButton;
class CALContentDialog;

class CALContentDialogPrivate final : public QObject {
	Q_OBJECT
	Q_DISABLE_COPY(CALContentDialogPrivate)
	Q_DECLARE_PUBLIC(CALContentDialog)

public:
	explicit CALContentDialogPrivate(CALContentDialog* q, QObject* parent = nullptr);
	~CALContentDialogPrivate() override;

protected:
	CALContentDialog* const q_ptr{ nullptr };

private:
	/// interface
	QWidget* centerWidget{ nullptr };
	QWidget* buttonWidget{ nullptr };
	QVBoxLayout* mainVLayout{ nullptr };
	CALPushButton* closeButton{ nullptr };
	CALPushButton* cancelButton{ nullptr };
};
}
