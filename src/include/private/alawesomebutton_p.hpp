#pragma once

#include <QColor>
#include <QObject>
#include <QFont>

#include "alwidgettoolsdef.hpp"

using namespace ALIcon;

class CALToolTip;
class CALAwesomeButton;

class CALAwesomeButtonPrivate final : public QObject {
	Q_OBJECT
	Q_DISABLE_COPY(CALAwesomeButtonPrivate)
	Q_DECLARE_PUBLIC(CALAwesomeButton)

public:
	explicit CALAwesomeButtonPrivate(CALAwesomeButton* q, QObject* parent = nullptr);
	~CALAwesomeButtonPrivate() override;

	void init();

protected:
	CALAwesomeButton* const q_ptr{ nullptr };

private:
	CALToolTip* tooltip{ nullptr };
	int borderRaius{};
	qreal opacity{};
	QFont iconFont{};
	AweSomeIcon iconType{};
	QColor ligntSelectedColor{};
	QColor darkSelectedColor{};
	QColor lightHoverColor{};
	QColor darkHoverColor{};
	QColor lightIconColor{};
	QColor darkIconColor{};
	bool isSelected{};
};
