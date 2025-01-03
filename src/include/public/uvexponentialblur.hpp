#pragma once

#include <QImage>
#include <QObject>

#include "uvwidgettoolsdef.hpp"
#include "uvsingleton.hpp"

class CUVExponentialBlurPrivate;

class CUVWIDGETTOOLS_EXPORT CUVExponentialBlur : public QObject {
	Q_OBJECT
	Q_DECLARE_PRIVATE(CUVExponentialBlur)

public:
	static CUVExponentialBlur* instance();
	static QPixmap doExponentialBlur(const QImage& image, const quint64& blurRadius);

protected:
	const QScopedPointer<CUVExponentialBlurPrivate> d_ptr{ nullptr };

	friend class CUVSingleton<CUVExponentialBlur>;

private:
	explicit CUVExponentialBlur(QObject* parent = nullptr);
	~CUVExponentialBlur() override;
};
