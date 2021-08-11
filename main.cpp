#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQuickView>
#include <QQmlContext>
#include <backend.h>
#include <QDebug>
#include <QtSql>

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);

    qDebug() << QSqlDatabase::drivers();

    QQmlApplicationEngine engine;
    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    Backend backend;
    engine.rootContext()->setContextProperty("App",&backend);
    engine.rootContext()->setContextProperty("contacts_model",backend.contactsModel);
    engine.rootContext()->setContextProperty("chat_model",backend.contactsModel->getChatModel());

    engine.load(url);

    return app.exec();
}
