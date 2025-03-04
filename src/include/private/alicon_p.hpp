#pragma once

#include "alwidgettoolsdef.hpp"

/**
 * @brief \namespace AL
 */
namespace AL {
class CALAwesomeIconType;

class CALAwesomeIconTypePrivate {
public:
	explicit CALAwesomeIconTypePrivate(const ALIcon::AweSomeIcon& awesomeicon);
	~CALAwesomeIconTypePrivate();
	ALIcon::AweSomeIcon icon{};

	friend class CALAwesomeIconType;
};

class CALFluentIconType;

class CALFluentIconTypePrivate {
public:
	explicit CALFluentIconTypePrivate(const ALIcon::FluentIcon& fluenticon);
	~CALFluentIconTypePrivate();
	ALIcon::FluentIcon icon{};

	friend class CALFluentIconType;
};
}
