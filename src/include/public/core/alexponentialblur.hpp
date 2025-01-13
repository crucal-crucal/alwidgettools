#pragma once

#include <QImage>
#include <QObject>

#include "alwidgettoolsdef.hpp"
#include "alsingleton.hpp"

namespace AL {
class CALExponentialBlurPrivate;

class CALWIDGETTOOLS_EXPORT CALExponentialBlur : public QObject {
	Q_OBJECT
	Q_DECLARE_PRIVATE(CALExponentialBlur)

public:
	static CALExponentialBlur* instance();
	static QPixmap doExponentialBlur(const QImage& image, const quint64& blurRadius);

protected:
	const QScopedPointer<CALExponentialBlurPrivate> d_ptr{ nullptr };

	friend class AL::CALSingleton<CALExponentialBlur>;

private:
	explicit CALExponentialBlur(QObject* parent = nullptr);
	~CALExponentialBlur() override;
};

} // namespace AL
