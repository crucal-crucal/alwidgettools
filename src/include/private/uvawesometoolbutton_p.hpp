#pragma once

#include <QObject>

class CUVToolTip;
class CUVAwesomeToolButton;
class CUVAwesomeToolButtonStyle;

class CUVAwesomeToolButtonPrivate final : public QObject {
	Q_OBJECT
	Q_DISABLE_COPY(CUVAwesomeToolButtonPrivate)
	Q_DECLARE_PUBLIC(CUVAwesomeToolButton)

public:
	explicit CUVAwesomeToolButtonPrivate(CUVAwesomeToolButton* q, QObject* parent = nullptr);
	~CUVAwesomeToolButtonPrivate() override;

protected:
	CUVAwesomeToolButton* const q_ptr{ nullptr };

private:
	CUVAwesomeToolButtonStyle* style{ nullptr };
	CUVToolTip* tooltip{ nullptr };
};
