#pragma once

#include <QImage>

#include "alwidgettoolsdef.hpp"

namespace AL {
class CALApplicationPrivate;

class CALMicaBaseInitObject final : public QObject {
	Q_OBJECT

public:
	explicit CALMicaBaseInitObject(CALApplicationPrivate* appPrivate, QObject* parent = nullptr);
	~CALMicaBaseInitObject() override;

	Q_SLOT void slotInitMicaBase(QImage image);

Q_SIGNALS:
	Q_SIGNAL void sigInitFinished();

private:
	CALApplicationPrivate* appPrivate{ nullptr };
};

} // namespace AL
