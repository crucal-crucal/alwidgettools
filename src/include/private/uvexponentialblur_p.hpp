#pragma once

#include <QObject>

class CUVExponentialBlur;

class CUVExponentialBlurPrivate final : public QObject {
	Q_OBJECT
	Q_DISABLE_COPY(CUVExponentialBlurPrivate)
	Q_DECLARE_PUBLIC(CUVExponentialBlur)

public:
	explicit CUVExponentialBlurPrivate(CUVExponentialBlur* q, QObject* parent = nullptr);
	~CUVExponentialBlurPrivate() override;

protected:
	CUVExponentialBlur* const q_ptr{ nullptr };

private:
	static int _aprec;
	static int _zprec;
	static void drawExponentialBlur(QImage& image, const quint64& qRadius);
	static void drawRowBlur(QImage& image, const int& row, const int& alpha);
	static void drawColumnBlur(QImage& image, const int& column, const int& alpha);
	static void drawInnerBlur(unsigned char* bptr, int& zR, int& zG, int& zB, int& zA, int alpha);
};
