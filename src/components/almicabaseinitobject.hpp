#pragma once

#include <QImage>

#include "alwidgettoolsdef.hpp"

/**
 * @brief \namespace AL
 */
namespace AL {
class CALApplicationPrivate;

class CALMicaBaseInitObject final : public QObject {
	Q_OBJECT

public:
	explicit CALMicaBaseInitObject(CALApplicationPrivate* appPrivate, QObject* parent = nullptr);
	~CALMicaBaseInitObject() override;

	Q_SLOT void slotInitMicaBase(QImage image);

Q_SIGNALS:
	 void sigInitFinished();

private:
	CALApplicationPrivate* appPrivate{ nullptr };
};
}
