#include "allog.hpp"

#include <QDir>
#include <QDateTime>
#include <QDebug>
#include <QFile>
#include <QMutex>
#ifndef QT_NO_DEBUG
#include <iostream>
#endif
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
#include <QTextStream>
#endif

#include "allog_p.hpp"

/**
 * @brief \namespace AL
 */
namespace AL {
QMutex& globalMessageLogMutex() {
	static QMutex mutex;
	return mutex;
}

QString& globalLogFileNameTime() {
	static QString logFileNameTime;
	return logFileNameTime;
}

/**
 * @brief \class CALLogPrivate
 * Internal class for CALLog
 * @param q pointer to the public class
 * @param parent pointer to the parent class
 */
CALLogPrivate::CALLogPrivate(CALLog* q, QObject* parent): QObject(parent), q_ptr(q) {
}

CALLogPrivate::~CALLogPrivate() = default;

void CALLogPrivate::messageLogHander(const QtMsgType type, const QMessageLogContext& context, const QString& msg) {
	QString logTime = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz");
	QString level{};
	switch (type) {
		case QtDebugMsg: level = "Debug";
			break;
		case QtWarningMsg: level = "Warning";
			break;
		case QtCriticalMsg: level = "Critical";
			break;
		case QtInfoMsg: level = "Info";
			break;
		case QtFatalMsg: level = "Fatal";
			break;
		default:
			qCritical("Unknown message type");
			return;
	}
	const QString logInfo = QString("[%1 - %2](function: %3, line: %4) -> %5").arg(level, logTime, context.function, QString::number(context.line), msg);
	qDebug() << logInfo;

	CALLog* log = CALLog::instance();
	Q_EMIT log->sigLogMessage(logInfo);
	writeLogToFile(logInfo, log);
}

void CALLogPrivate::clearLogFile() const {
	if (isLogFileNameWithTime) {
		const QString logTime = QDateTime::currentDateTime().toString("yyyy-MM-dd_hh-mm-ss");
		globalLogFileNameTime() = logTime;
	} else {
		if (QFile file(QDir(logSavePath).filePath(logFileName + ".log")); file.exists() && file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate)) {
			file.close();
		}
	}
}

void CALLogPrivate::writeLogToFile(const QString& logInfo, const CALLog* log) {
	QMutexLocker locker(&globalMessageLogMutex());

	const QString logFileName = log->getLogFileName() + (log->getIsLogFileNameWithTime() ? globalLogFileNameTime() + ".log" : ".log");
	const QString logFilePath = QDir(log->getLogSavePath()).filePath(logFileName);
	QFile logFile(logFilePath);
	if (logFile.open(QIODevice::WriteOnly | QIODevice::Append)) {
		QTextStream logFileStream(&logFile);
#if QT_VERSION >= QT_VERSION_CHECK(5, 14, 0)
		logFileStream << logInfo << Qt::endl;
#else
		logFileStream << logInfo << endl;
#endif
		logFile.close();
	}
}

/**
 * @brief \class CALLog
 * @param parent pointer to the parent class
 */
CALLog::CALLog(QObject* parent): QObject(parent), d_ptr(new CALLogPrivate(this, this)) {
	Q_D(CALLog);

	d->logFileName = "allog";
	d->logSavePath = QDir::currentPath();
	d->isLogFileNameWithTime = true;
	d->clearLogFile();
	connect(this, &CALLog::sigLogFileNameChanged, d, &CALLogPrivate::clearLogFile);
	connect(this, &CALLog::sigLogSavePathChanged, d, &CALLogPrivate::clearLogFile);
	connect(this, &CALLog::sigIsLogFileNameWithTimeChanged, d, &CALLogPrivate::clearLogFile);
}

CALLog::~CALLog() = default;

CALLog* CALLog::instance() {
	return CALSingleton<CALLog>::instance();
}

void CALLog::setMessageLogEnable(const bool isEnable) {
	qInstallMessageHandler(isEnable ? d_func()->messageLogHander : nullptr); // NOLINT
}

void CALLog::setLogSavePath(const QString& path) {
	d_func()->logSavePath = path;
	Q_EMIT sigLogSavePathChanged();
}

QString CALLog::getLogSavePath() const {
	return d_func()->logSavePath;
}

void CALLog::setLogFileName(const QString& name) {
	d_func()->logFileName = name;
	Q_EMIT sigLogFileNameChanged();
}

QString CALLog::getLogFileName() const {
	return d_func()->logFileName;
}

void CALLog::setIsLogFileNameWithTime(const bool isWithTime) {
	d_func()->isLogFileNameWithTime = isWithTime;
	Q_EMIT sigIsLogFileNameWithTimeChanged();
}

bool CALLog::getIsLogFileNameWithTime() const {
	return d_func()->isLogFileNameWithTime;
}
}
