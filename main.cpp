#include "mainwindow.h"
#include <QApplication>
#include <QCommandLineOption>
#include <QCommandLineParser>
#include <QCursor>
#include <QDesktopWidget>
#include <QRect>
#include <QScreen>
#include <iostream>

static QScreen *findScreenWithCursor()
{
    QPoint pos = QCursor::pos();

    for (QScreen *screen : QGuiApplication::screens()) {
        QRect screenRect = screen->geometry();
        if (screenRect.contains(pos)) {
            return screen;
        }
    }

    return QApplication::desktop()->screen();
}

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    auto optionRealm = QCommandLineOption("realm", "The authentication realm.", "realm");
    auto optionUrl = QCommandLineOption("url", "The already built SAML URL.\nThis takes precedence over [host:port].", "url");
    auto optionKeepOpen = QCommandLineOption("keep-open", "Do not close the browser automatically.");
    auto optionWaitForUrl = QCommandLineOption({"w", "wait-url"}, "Wait for the URL matching <url-regex> before printing the cookie.");
    auto defaultUrlRegex = "/sslvpn/portal\\.html";
    auto urlRegexDescription = QString("A regex to detect the URL to wait for.\nThe default is \"%1\".").arg(defaultUrlRegex);
    auto optionUrlRegex = QCommandLineOption("url-regex", urlRegexDescription, "url-regex", defaultUrlRegex);

    QCommandLineParser parser;
    parser.addPositionalArgument("host", "The VPN gateway host with an optional port.", "[host:port]");
    parser.addOption(optionRealm);
    parser.addOption(optionWaitForUrl);
    parser.addOption(optionUrlRegex);
    parser.addOption(optionUrl);
    parser.addOption(optionKeepOpen);
    parser.addOption(QCommandLineOption("remote-debugging-port", "Remote debugging server port.", "port"));
    parser.addHelpOption();
    parser.addVersionOption();

    if (!parser.parse(QCoreApplication::arguments())) {
        parser.showHelp(1);
    }

    if (parser.isSet("help")) {
        parser.showHelp(0);
    }

    QString url = parser.value(optionUrl);
    if (url.isEmpty()) {
        if (parser.positionalArguments().size() < 1) {
            parser.showHelp(1);
        }
        url = "https://" + parser.positionalArguments()[0] + "/remote/saml/start";
        QString realm = parser.value(optionRealm);
        if (!realm.isEmpty()) {
            url += "?realm=" + realm;
        }
    }

    bool keepOpen = parser.isSet(optionKeepOpen);
    bool waitForUrl = parser.isSet(optionWaitForUrl);

    auto urlRegex = QRegularExpression();
    if (waitForUrl) {
        QString pattern = parser.value(optionUrlRegex);
        urlRegex.setPattern(pattern);
        if (!urlRegex.isValid()) {
            std::cerr << "The given <url-regex> is not valid." << std::endl;
            exit(1);
        }
    }

    MainWindow w(keepOpen, waitForUrl ? &urlRegex : nullptr);
    w.loadUrl(url);
    w.resize(1024, 760);
    w.move(findScreenWithCursor()->geometry().center() - w.rect().center());
    w.show();

    return app.exec();
}
