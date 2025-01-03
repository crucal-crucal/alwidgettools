#include "uvexponentialblur.hpp"

#include <QPixmap>
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
#include <cmath>
#endif

#include "uvexponentialblur_p.hpp"

int CUVExponentialBlurPrivate::_aprec = 12;
int CUVExponentialBlurPrivate::_zprec = 7;

/**
 * @brief \class CUVExponentialBlurPrivate
 * Internal class for CUVExponentialBlur
 * @param q pointer to the public class
 * @param parent pointer to the parent class
 */
CUVExponentialBlurPrivate::CUVExponentialBlurPrivate(CUVExponentialBlur* q, QObject* parent): QObject(parent), q_ptr(q) {
}

CUVExponentialBlurPrivate::~CUVExponentialBlurPrivate() = default;

void CUVExponentialBlurPrivate::drawExponentialBlur(QImage& image, const quint64& qRadius) {
	if (qRadius < 1) {
		return;
	}

	image = image.convertToFormat(QImage::Format_ARGB32_Premultiplied);
	const int alpha = static_cast<int>((1 << _aprec) * (1.0f - std::exp(-2.3f / static_cast<float>(qRadius + 1)))); // NOLINT
	const int height = image.height();
	const int width = image.width();
	for (int row = 0; row < height; row++) {
		drawColumnBlur(image, row, alpha);
	}

	for (int col = 0; col < width; col++) {
		drawColumnBlur(image, col, alpha);
	}
}

void CUVExponentialBlurPrivate::drawRowBlur(QImage& image, const int& row, const int& alpha) {
	const auto ptr = reinterpret_cast<QRgb*>(image.scanLine(row));
	const int width = image.width();

	int zR = *reinterpret_cast<unsigned char*>(ptr) << _zprec;
	int zG = *(reinterpret_cast<unsigned char*>(ptr) + 1) << _zprec;
	int zB = *(reinterpret_cast<unsigned char*>(ptr) + 2) << _zprec;
	int zA = *(reinterpret_cast<unsigned char*>(ptr) + 3) << _zprec;

	for (int index = 0; index < width; index++) {
		drawInnerBlur(reinterpret_cast<unsigned char*>(&ptr[index]), zR, zG, zB, zA, alpha);
	}
	for (int index = width - 2; index >= 0; index--) {
		drawInnerBlur(reinterpret_cast<unsigned char*>(&ptr[index]), zR, zG, zB, zA, alpha);
	}
}

void CUVExponentialBlurPrivate::drawColumnBlur(QImage& image, const int& column, const int& alpha) {
	auto ptr = reinterpret_cast<QRgb*>(image.bits());
	ptr += column;
	const int height = image.height();
	const int width = image.width();

	int zR = *reinterpret_cast<unsigned char*>(ptr) << _zprec;
	int zG = *(reinterpret_cast<unsigned char*>(ptr) + 1) << _zprec;
	int zB = *(reinterpret_cast<unsigned char*>(ptr) + 2) << _zprec;
	int zA = *(reinterpret_cast<unsigned char*>(ptr) + 3) << _zprec;

	for (int index = width; index < (height - 1) * width; index += width) {
		drawInnerBlur(reinterpret_cast<unsigned char*>(&ptr[index]), zR, zG, zB, zA, alpha);
	}

	for (int index = (height - 2) * width; index >= 0; index -= width) {
		drawInnerBlur(reinterpret_cast<unsigned char*>(&ptr[index]), zR, zG, zB, zA, alpha);
	}
}

void CUVExponentialBlurPrivate::drawInnerBlur(unsigned char* bptr, int& zR, int& zG, int& zB, int& zA, const int alpha) {
	const int R = *bptr;
	const int G = *(bptr + 1);
	const int B = *(bptr + 2);
	const int A = *(bptr + 3);

	zR += (alpha * ((R << _zprec) - zR)) >> _zprec;
	zG += (alpha * ((G << _zprec) - zG)) >> _zprec;
	zB += (alpha * ((B << _zprec) - zB)) >> _zprec;
	zA += (alpha * ((A << _zprec) - zA)) >> _zprec;

	*bptr = zR >> _zprec;
	*(bptr + 1) = zG >> _zprec;
	*(bptr + 2) = zB >> _zprec;
	*(bptr + 3) = zA >> _zprec;
}

/**
 * @brief \class CUVExponentialBlur
 * @return CUVExponentialBlur instance
 */
CUVExponentialBlur* CUVExponentialBlur::instance() {
	return CUVSingleton<CUVExponentialBlur>::instance();
}

QPixmap CUVExponentialBlur::doExponentialBlur(const QImage& image, const quint64& blurRadius) {
	QImage shadowImage = image.convertToFormat(QImage::Format_ARGB32);
	CUVExponentialBlur::instance()->d_func()->drawExponentialBlur(shadowImage, blurRadius); // NOLINT
	return QPixmap::fromImage(shadowImage);
}

CUVExponentialBlur::CUVExponentialBlur(QObject* parent): QObject(parent), d_ptr(new CUVExponentialBlurPrivate(this, this)) {
}

CUVExponentialBlur::~CUVExponentialBlur() = default;
