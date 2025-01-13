#include "alintvalidator.hpp"

namespace AL {
/**
 * @brief \class CALIntValidator
 * @param parent pointer to the parent class
 */
CALIntValidator::CALIntValidator(QObject* parent): QIntValidator(parent), m_isHexMode(false) {
}

CALIntValidator::CALIntValidator(const int bottom, const int top, QObject* parent): QIntValidator(bottom, top, parent), m_isHexMode(false) {
}

CALIntValidator::~CALIntValidator() = default;

QValidator::State CALIntValidator::validate(QString& chars, int& i) const {
	QString inputCopy = chars;
	if (m_isHexMode) {
		// Remove prefix for validation
		inputCopy.remove("#");
		if (!inputCopy.isEmpty()) {
			bool isInt{ false };

			if (const int value = inputCopy.toInt(&isInt, 16); !isInt || value < bottom() || value > top()) {
				return QValidator::Invalid;
			}

			if (const int maxHexLength = QString::number(top(), 16).length(); inputCopy.length() > maxHexLength) {
				return QValidator::Invalid;
			}
		}
		// Reapply prefix for consistent formatting
		inputCopy.prepend("#");
	} else {
		if (inputCopy.isEmpty()) {
			return QValidator::Intermediate;
		}

		bool isInt{ false };

		if (const int value = inputCopy.toInt(&isInt, 16); !isInt || value < bottom() || value > top()) {
			return QValidator::Invalid;
		}
	}
	chars = inputCopy;

	return QIntValidator::Acceptable;
}

void CALIntValidator::fixup(QString& chars) const {
	if (m_isHexMode) {
		// Fill with zeros and prepend prefix
		const int maxHexLength = QString::number(top(), 16).length();
		chars = QString("#") + completeInput(chars.remove("#"), maxHexLength);
	} else {
		// Set to the lower bound
		chars = QString::number(bottom());
	}
}

void CALIntValidator::setIsHexMode(const bool isHexMode) {
	m_isHexMode = isHexMode;
}

bool CALIntValidator::getIsHexMode() const {
	return m_isHexMode;
}

QString CALIntValidator::completeInput(const QString& input, const int length) {
	return input.rightJustified(length, '0');
}

} // namespace AL
