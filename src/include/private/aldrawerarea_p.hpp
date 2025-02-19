#pragma once

#include "aldrawercontainer.hpp"
#include "aldrawerheader.hpp"
#include "alwidgettoolsdef.hpp"

/**
 * @brief \namespace AL
 */
namespace AL {
class CALDrawerArea;

class CALDrawerAreaPrivate final : public QObject {
	Q_OBJECT
	Q_DISABLE_COPY(CALDrawerAreaPrivate)
	Q_DECLARE_PUBLIC(CALDrawerArea)

public:
	explicit CALDrawerAreaPrivate(CALDrawerArea* q, QObject* parent = nullptr);
	~CALDrawerAreaPrivate() override;

	Q_SLOT void slotDrawerHeaderClicked(bool isExpand);

	void setExpandState(bool expand);

protected:
	CALDrawerArea* const q_ptr{ nullptr };

private:
	ALThemeType::ThemeMode themeMode{};
	CALDrawerContainer* drawerContainer{ nullptr };
	CALDrawerHeader* drawerHeader{ nullptr };
};
}
