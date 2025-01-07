#include "almicabaseinitobject.hpp"

#include "alapplication_p.hpp"
#include "alexponentialblur.hpp"

/**
 * @brief \class CALMicaBaseInitObject
 * @param appPrivate private data
 * @param parent pointer to the parent class
 */
CALMicaBaseInitObject::CALMicaBaseInitObject(CALApplicationPrivate* appPrivate, QObject* parent): QObject(parent), appPrivate(appPrivate) {
}

CALMicaBaseInitObject::~CALMicaBaseInitObject() = default;

void CALMicaBaseInitObject::slotInitMicaBase(QImage image) {
	// QColorDialog
	// 统一处理为 1920 * 1080 以节省空间
	image = image.scaled(1920, 1080, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);

	QImage blurImage = CALExponentialBlur::doExponentialBlur(image, 500).toImage();
	QImage lightImage = blurImage;
	QImage darkImage = blurImage;
	// auto lightMixColor = QColor(0xF3, 0xF3, 0xF3);
	// lightMixColor = lightMixColor.toHsv();
	// QColor darkMixColor = QColor(0x20, 0x20, 0x20);
	// darkMixColor = darkMixColor.toHsv();
	QColor originColor;
	QColor lightColor;
	QColor darkColor;
	int h, s, v;
	for (int y = 0; y < blurImage.height(); y++) {
		auto line = reinterpret_cast<QRgb*>(blurImage.scanLine(y));
		for (int x = 0; x < blurImage.width(); x++) {
			originColor = QColor(line[x]);
			originColor = originColor.toHsv();
			h = originColor.hsvHue();
			s = originColor.hsvSaturation();
			v = originColor.value();
			if (s / 20 > 11) {
				lightColor.setHsv(h, (s / 20 + 11) / 2, 250);
			} else {
				lightColor.setHsv(h, 11, 250);
			}
			lightColor = lightColor.toRgb();
			if (v / 1.1 > 40) {
				darkColor.setHsv(h, s / 2, static_cast<int>(v / 1.1 + 40) / 2);
			} else {
				darkColor.setHsv(h, s / 2, 40);
			}
			darkColor = darkColor.toRgb();
			lightImage.setPixel(x, y, qRgb(lightColor.red(), lightColor.green(), lightColor.blue()));
			darkImage.setPixel(x, y, qRgb(darkColor.red(), darkColor.green(), darkColor.blue()));
		}
	}
	appPrivate->lightBaseImage = lightImage.copy();
	appPrivate->darkBaseImage = darkImage.copy();
	Q_EMIT sigInitFinished();
}
