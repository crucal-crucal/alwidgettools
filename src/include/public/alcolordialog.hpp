#pragma once

#include <QDialog>

#include "alappbar.hpp"
#include "alwidgettoolsdef.hpp"

class CUVColorDialogPrivate;

class CALWIDGETTOOLS_EXPORT CUVColorDialog : public QDialog {
	Q_OBJECT
	Q_DECLARE_PRIVATE(CUVColorDialog)
	Q_PROPERTY(QColor currentColor READ getCurrentColor WRITE setCurrentColor NOTIFY sigCurrentColorChanged)
	Q_TAKEOVER_NATIVEEVENT_H

public:
	explicit CUVColorDialog(QWidget* parent = nullptr);
	~CUVColorDialog() override;

	void setCurrentColor(const QColor& color);
	[[nodiscard]] QColor getCurrentColor() const;

	[[nodiscard]] QList<QColor> getCustomColorList() const;
	[[nodiscard]] QColor getCustomColor(int index) const;
	[[nodiscard]] QString getCurrentColorRgb() const;

Q_SIGNALS:
	Q_SIGNAL void sigCurrentColorChanged();
	Q_SIGNAL void sigColorSelected(const QColor& color);

protected:
	const QScopedPointer<CUVColorDialogPrivate> d_ptr{ nullptr };

	void paintEvent(QPaintEvent* event) override;
};
