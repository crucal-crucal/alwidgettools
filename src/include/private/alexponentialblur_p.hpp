#pragma once

#include "alwidgettoolsdef.hpp"

namespace AL {
class CALExponentialBlur;

class CALExponentialBlurPrivate final : public QObject {
	Q_OBJECT
	Q_DISABLE_COPY(CALExponentialBlurPrivate)
	Q_DECLARE_PUBLIC(CALExponentialBlur)

public:
	explicit CALExponentialBlurPrivate(CALExponentialBlur* q, QObject* parent = nullptr);
	~CALExponentialBlurPrivate() override;

protected:
	CALExponentialBlur* const q_ptr{ nullptr };

private:
	static int _aprec;
	static int _zprec;
	static void drawExponentialBlur(QImage& image, const quint64& qRadius);
	static void drawRowBlur(QImage& image, const int& row, const int& alpha);
	static void drawColumnBlur(QImage& image, const int& column, const int& alpha);
	static void drawInnerBlur(unsigned char* bptr, int& zR, int& zG, int& zB, int& zA, int alpha);
};
} // namespace AL