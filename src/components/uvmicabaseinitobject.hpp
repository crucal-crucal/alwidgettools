#pragma once

#include <QImage>
#include <QObject>

class CUVApplicationPrivate;

class CUVMicaBaseInitObject final : public QObject {
	Q_OBJECT

public:
	explicit CUVMicaBaseInitObject(CUVApplicationPrivate* appPrivate, QObject* parent = nullptr);
	~CUVMicaBaseInitObject() override;

	Q_SLOT void slotInitMicaBase(QImage image);

Q_SIGNALS:
	Q_SIGNAL void sigInitFinished();

private:
	CUVApplicationPrivate* appPrivate{ nullptr };
};
