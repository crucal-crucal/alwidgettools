#pragma once

#include <QWidget>

#include "alwidgettoolsdef.hpp"

namespace AL {
class CALThemeAnimationWidget final : public QWidget {
	Q_OBJECT
	Q_PROPERTY(qreal radius READ getRadius WRITE setRadius NOTIFY sigRadiusChanged)
	Q_PROPERTY(qreal endRadius READ getEndRadius WRITE setEndRadius NOTIFY sigEndRadiusChanged)
	Q_PROPERTY(QPoint center READ getCenter WRITE setCenter NOTIFY sigCenterChanged)
	Q_PROPERTY(QImage oldWindowImage READ getOldWindowImage WRITE setOldWindowImage NOTIFY sigOldWindowImageChanged)
	Q_PROPERTY(QImage newWindowImage READ getNewWindowImage WRITE setNewWindowImage NOTIFY sigNewWindowImageChanged)

public:
	explicit CALThemeAnimationWidget(QWidget* parent = nullptr);
	~CALThemeAnimationWidget() override;

	void startAnimation(int mesc);

	void setRadius(qreal radius);
	[[nodiscard]] qreal getRadius() const;

	void setEndRadius(qreal endRadius);
	[[nodiscard]] qreal getEndRadius() const;

	void setCenter(const QPoint& center);
	[[nodiscard]] QPoint getCenter() const;

	void setOldWindowImage(const QImage& oldWindowImage);
	[[nodiscard]] QImage getOldWindowImage() const;

	void setNewWindowImage(const QImage& newWindowImage);
	[[nodiscard]] QImage getNewWindowImage() const;

Q_SIGNALS:
	Q_SIGNAL void sigAnimationFinished();
	Q_SIGNAL void sigRadiusChanged();
	Q_SIGNAL void sigEndRadiusChanged();
	Q_SIGNAL void sigCenterChanged();
	Q_SIGNAL void sigOldWindowImageChanged();
	Q_SIGNAL void sigNewWindowImageChanged();

protected:
	void paintEvent(QPaintEvent* event) override;

private:
	qreal m_radius{};
	qreal m_endRadius{};
	QPoint m_center{};
	QImage m_oldWindowImage{};
	QImage m_newWindowImage{};
};
} // namespace AL