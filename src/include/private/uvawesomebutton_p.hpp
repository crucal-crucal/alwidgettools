#pragma once

#include <QColor>
#include <QObject>
#include <QFont>

#include "uvwidgettoolsdef.hpp"

using namespace UVIcon;

class CUVToolTip;
class CUVAwesomeButton;

class CUVAwesomeButtonPrivate final : public QObject {
	Q_OBJECT
	Q_DISABLE_COPY(CUVAwesomeButtonPrivate)
	Q_DECLARE_PUBLIC(CUVAwesomeButton)

public:
	explicit CUVAwesomeButtonPrivate(CUVAwesomeButton* q, QObject* parent = nullptr);
	~CUVAwesomeButtonPrivate() override;

	void init();

protected:
	CUVAwesomeButton* const q_ptr{ nullptr };

private:
	CUVToolTip* tooltip{ nullptr };
	int borderRaius{};
	qreal opacity{};
	QFont iconFont{};
	CUVAweSomeIcon iconType{};
	QColor ligntSelectedColor{};
	QColor darkSelectedColor{};
	QColor lightHoverColor{};
	QColor darkHoverColor{};
	QColor lightIconColor{};
	QColor darkIconColor{};
	bool isSelected{};
};
