#include "alexponentialblur.hpp"

#include <QPixmap>
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
#include <QtMath>
#endif

#include "alexponentialblur_p.hpp"

/**
 * @brief \namespace AL
 */
namespace AL {
int CALExponentialBlurPrivate::aprec = 12;
int CALExponentialBlurPrivate::zprec = 7;

/**
 * @brief \class CALExponentialBlurPrivate
 * Internal class for CALExponentialBlur
 * @param q pointer to the public class
 * @param parent pointer to the parent class
 */
CALExponentialBlurPrivate::CALExponentialBlurPrivate(CALExponentialBlur* q, QObject* parent): QObject(parent), q_ptr(q) {
}

CALExponentialBlurPrivate::~CALExponentialBlurPrivate() = default;

void CALExponentialBlurPrivate::drawExponentialBlur(QImage& image, const quint64& qRadius) {
	if (qRadius < 1) {
		return;
	}

	image = image.convertToFormat(QImage::Format_ARGB32_Premultiplied);
	const int alpha = static_cast<int>((1 << aprec) * (1.0f - std::exp(-2.3f / static_cast<float>(qRadius + 1)))); // NOLINT
	for (int row = 0; row < image.height(); ++row) {
		drawColumnBlur(image, row, alpha);
	}

	for (int col = 0; col < image.width(); ++col) {
		drawColumnBlur(image, col, alpha);
	}
}

void CALExponentialBlurPrivate::drawRowBlur(QImage& image, const int& row, const int& alpha) {
	const auto ptr = reinterpret_cast<QRgb*>(image.scanLine(row));
	const int width = image.width();

	int zR = *reinterpret_cast<unsigned char*>(ptr) << zprec;
	int zG = *(reinterpret_cast<unsigned char*>(ptr) + 1) << zprec;
	int zB = *(reinterpret_cast<unsigned char*>(ptr) + 2) << zprec;
	int zA = *(reinterpret_cast<unsigned char*>(ptr) + 3) << zprec;

	for (int index = 0; index < width; ++index) {
		drawInnerBlur(reinterpret_cast<unsigned char*>(&ptr[index]), zR, zG, zB, zA, alpha);
	}
	for (int index = width - 2; index >= 0; --index) {
		drawInnerBlur(reinterpret_cast<unsigned char*>(&ptr[index]), zR, zG, zB, zA, alpha);
	}
}

void CALExponentialBlurPrivate::drawColumnBlur(QImage& image, const int& column, const int& alpha) {
	auto ptr = reinterpret_cast<QRgb*>(image.bits());
	ptr += column;
	const int height = image.height();
	const int width = image.width();

	int zR = *reinterpret_cast<unsigned char*>(ptr) << zprec;
	int zG = *(reinterpret_cast<unsigned char*>(ptr) + 1) << zprec;
	int zB = *(reinterpret_cast<unsigned char*>(ptr) + 2) << zprec;
	int zA = *(reinterpret_cast<unsigned char*>(ptr) + 3) << zprec;

	for (int index = width; index < (height - 1) * width; index += width) {
		drawInnerBlur(reinterpret_cast<unsigned char*>(&ptr[index]), zR, zG, zB, zA, alpha);
	}

	for (int index = (height - 2) * width; index >= 0; index -= width) {
		drawInnerBlur(reinterpret_cast<unsigned char*>(&ptr[index]), zR, zG, zB, zA, alpha);
	}
}

void CALExponentialBlurPrivate::drawInnerBlur(unsigned char* bptr, int& zR, int& zG, int& zB, int& zA, const int alpha) {
	const int R = *bptr;
	const int G = *(bptr + 1);
	const int B = *(bptr + 2);
	const int A = *(bptr + 3);

	zR += (alpha * ((R << zprec) - zR)) >> zprec;
	zG += (alpha * ((G << zprec) - zG)) >> zprec;
	zB += (alpha * ((B << zprec) - zB)) >> zprec;
	zA += (alpha * ((A << zprec) - zA)) >> zprec;

	*bptr = zR >> zprec;
	*(bptr + 1) = zG >> zprec;
	*(bptr + 2) = zB >> zprec;
	*(bptr + 3) = zA >> zprec;
}

/**
 * @brief \class CALExponentialBlur
 * @return CALExponentialBlur instance
 */
CALExponentialBlur* CALExponentialBlur::instance() {
	return CALSingleton<CALExponentialBlur>::instance();
}

QPixmap CALExponentialBlur::doExponentialBlur(const QImage& image, const quint64& blurRadius) {
	QImage shadowImage = image.convertToFormat(QImage::Format_ARGB32);
	CALExponentialBlur::instance()->d_func()->drawExponentialBlur(shadowImage, blurRadius); // NOLINT
	return QPixmap::fromImage(shadowImage);
}

CALExponentialBlur::CALExponentialBlur(QObject* parent): QObject(parent), d_ptr(new CALExponentialBlurPrivate(this, this)) {
}

CALExponentialBlur::~CALExponentialBlur() = default;
}
