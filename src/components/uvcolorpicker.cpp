#include "uvcolorpicker.hpp"

#include <QMouseEvent>
#include <QPainter>
#include <QPainterPath>

#include "uvthememanager.hpp"

/**
 * @brief \class CUVColorPicker
 * @param parent pointer to the parent class
 */
CUVColorPicker::CUVColorPicker(QWidget* parent): QWidget(parent) {
	setFixedSize(260, 260);
	m_themeMode = UVTheme->getThemeMode();

	// 初始化色相图
	QPixmap colorPickerPix(QSize(257, 257));
	QImage colorPickerImage = colorPickerPix.toImage();
	for (int y = 0; y < colorPickerImage.height(); y++) {
		for (int x = 0; x < colorPickerImage.width(); x++) {
			QColor pixColor{};
			pixColor.setHsvF(1.0 / colorPickerPix.width() * x, 1.0 - 1.0 / colorPickerPix.height() * y, 1.0);
			pixColor = pixColor.toRgb();
			colorPickerImage.setPixelColor(x, y, pixColor);
		}
	}
	m_colorPickerImage = colorPickerImage;
}

CUVColorPicker::~CUVColorPicker() = default;

void CUVColorPicker::setThemeMode(const UVThemeType::ThemeMode& mode) {
	m_themeMode = mode;
}

UVThemeType::ThemeMode CUVColorPicker::getThemeMode() const {
	return m_themeMode;
}

void CUVColorPicker::setSelectedColor(const QColor& color) {
	m_selectedColor = color;
	const QColor hsvColor = color.toHsv();
	m_selectedPoint = QPoint(257 * hsvColor.hue() / 360.0, 255 - 257.0 * hsvColor.saturation() / 255.0); // NOLINT
	update();
}

QColor CUVColorPicker::getSelectedColor() const {
	return m_selectedColor;
}

void CUVColorPicker::mousePressEvent(QMouseEvent* event) {
	m_selectedPoint = adjustPointLimit(event->pos());
	m_selectedColor = m_colorPickerImage.pixelColor(m_selectedPoint);
	Q_EMIT sigSelectedColorChanged(m_selectedColor);
	QWidget::mousePressEvent(event);
	update();
}

void CUVColorPicker::mouseReleaseEvent(QMouseEvent* event) {
	m_selectedPoint = adjustPointLimit(event->pos());
	m_selectedColor = m_colorPickerImage.pixelColor(m_selectedPoint);
	Q_EMIT sigSelectedColorChanged(m_selectedColor);
	QWidget::mouseReleaseEvent(event);
	update();
}

void CUVColorPicker::mouseMoveEvent(QMouseEvent* event) {
	m_selectedPoint = adjustPointLimit(event->pos());
	m_selectedColor = m_colorPickerImage.pixelColor(m_selectedPoint);
	Q_EMIT sigSelectedColorChanged(m_selectedColor);
	QWidget::mouseMoveEvent(event);
	update();
}

void CUVColorPicker::paintEvent(QPaintEvent* event) {
	constexpr int penWidth = 3;
	constexpr qreal borderRadius = 5.0;

	QPainter painter(this);
	painter.save();
	painter.setRenderHint(QPainter::Antialiasing);
	// 色相图绘制
	QRect pickerRect = rect();
	pickerRect.adjust(penWidth, penWidth, -penWidth, -penWidth);
	painter.setPen(QPen(UVThemeColor(m_themeMode, UVThemeType::BasicBorder), penWidth));
	painter.drawRoundedRect(pickerRect, borderRadius, borderRadius);
	QPainterPath path;
	path.addRoundedRect(pickerRect, borderRadius, borderRadius);
	painter.setClipPath(path);
	painter.drawImage(pickerRect, m_colorPickerImage);

	// 取色点绘制
	if (!m_selectedPoint.isNull()) {
		painter.setPen(QPen(Qt::black, 2));
		painter.setBrush(Qt::NoBrush);
		painter.drawEllipse(m_selectedPoint, 8, 8);
	}
	painter.restore();
}

QPoint CUVColorPicker::adjustPointLimit(QPoint point) const {
	const QRect pickerRect = rect();
	constexpr int margin = 3;

	point.setX(qBound(margin, point.x(), pickerRect.right() - margin));
	point.setY(qBound(margin, point.y(), pickerRect.bottom() - margin));

	return point;
}
