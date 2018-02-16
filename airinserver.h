#ifndef AIRINSERVER_H
#define AIRINSERVER_H

/*
        This is Airin 4, an advanced WebSocket chat server
     Licensed under the new BSD 3-Clause license, see LICENSE
            Made by Asterleen ~ https://asterleen.com
*/

#include <QObject>
#include <QList>
#include <QString>
#include <QStringList>
#include <QDateTime>
#include <QMap>
#include <QFile>
#include <QRegExp>
#include <QSettings>

#include <QWebSocket>
#include <QWebSocketServer>
#include <QNetworkRequest>

#include <QSslConfiguration>
#include <QSslCertificate>
#include <QSslKey>

#include "airindata.h"
#include "airinlogger.h"

#include "airinclient.h"
#include "airindatabase.h"
#include "airincommands.h"


#define AIRIN_VERSION "4.6"
#define AIRIN_MAX_API_LEVEL 3
#define AIRIN_MIN_API_LEVEL 2


class AirinServer : public QObject
{
    Q_OBJECT
public:
    explicit AirinServer(QString config, QObject *parent = 0);
    ~AirinServer();

    enum LogOrder
    {
        LogAscend,
        LogDescend
    };


    // These interfaces are used by command processor
    uint clientsCount();
    QList<AirinClient *> getClients();
    void disconnectClientsByXId(QString externalId);
    void disconnectClientsByAddress (QString address);
    void disconnectClientsByHash (QString hash);
    void messageBroadcast(QString message, uint apiLevel = 0);
    void broadcastForXId (QString externalId, QString message);
    void setShadowbanned (QString externalId, bool isShBanned);
    void setMotd(QString newMotd);
    QString getMotd();
    QString defaultChatName();
    void sendMotd (AirinClient *client, QString motdToSend);
    bool isOnline (QString externalId);
    bool isNameDistinct(AirinClient *client);
    void loadConfigFromDatabase();


private:
    QString config;

    uint serverPort;
    LogLevel outputLogLevel;
    uint defaultMessageAmount;
    uint maxMessageAmount;
    uint maxMessageLen;
    uint maxNameLen;
    uint minMessageDelay;
    uint sqlServerPing;
    uint initTimeout;
    uint colorResetMax;
    uint clientPingPollInterval;
    uint clientPingMissTolerance;
    bool serverSecure;
    bool delayTroll; // block user again and again by resetting the delay time counter
    bool useXAuth;
    bool readonlyAllowed;
    bool checkNamesDistinctness;
    bool serverReady;
    bool continueWithoutDB;
    bool forceDefaultName;
    QString hashSalt;
    QString logFile;
    QString sqlDbType;
    QString sqlHost;
    QString sqlDatabase;
    QString sqlUsername;
    QString sqlPassword;
    QString sslCertFile;
    QString sslIntermediateCertFile;
    QString sslKeyFile;
    QString defaultUserName;
    QString deprecationMessage;

    QMap<QString, uint> lastMessageTime;

    QWebSocketServer *server;
    QList<AirinClient *> clients;

    QSslConfiguration sslConfiguration;

    QSettings *settings;

    void loadConfig(QString configName);
    void setupSsl();

    void processClientCommand (AirinClient *client, QString command);
    void processMessage (AirinClient *client, QString recCode, QString message);
    void processMessageAPI (AirinClient *client, QString messageAmount,
                            QString messageOffset = QString(), LogOrder order = LogAscend);

    void sendGreeting(AirinClient *client);

    bool chkString(const QString &s);
    bool checkAuth(AirinClient *client);

signals:

public slots:
    void log(QString message, LogLevel logLevel = LL_DEBUG, QString component = "acore");
    void logAdmin (QString message, LogLevel logLevel = LL_DEBUG);

    void clientMessage(QString message);
    void clientDisconnect();
    void clientInitTimeout();
    void clientPingTimeout();
    void clientPingMissed();

    void serverNewConnection();
    void serverRestart();


};


#endif // AIRINSERVER_H