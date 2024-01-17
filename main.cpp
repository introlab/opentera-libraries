#include <QCoreApplication>
#include <QLocale>
#include <QTranslator>
#include <QCommandLineParser>
#include <QCommandLineOption>
#include <QDebug>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    QCoreApplication::setApplicationName("OpenTera Libraries Tools");
    QCoreApplication::setApplicationVersion("1.0");

    QCommandLineParser parser;
    parser.setApplicationDescription("A tool to test and execute operations on the OpenTera libraries");
    parser.addHelpOption();
    parser.addVersionOption();
    QCommandLineOption register_option("register-device", "Register a device to OpenTera server");
    parser.addOption(register_option);
    QCommandLineOption device_name_option("device-name", "Device name to use");
    parser.addOption(device_name_option);
    QCommandLineOption device_type_option("id-device-type", "ID of the device type used to register");
    parser.addOption(device_type_option);

    parser.addPositionalArgument("server", "Server URL");
    parser.addPositionalArgument("port", "Server port");


    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages) {
        const QString baseName = "opentera-libraries_" + QLocale(locale).name();
        if (translator.load(":/i18n/" + baseName)) {
            a.installTranslator(&translator);
            break;
        }
    }

    // Process the actual command line arguments given by the user
    parser.process(a);

    const QStringList args = parser.positionalArguments();
    if (args.length() < 2){
        qCritical() << "Missing required server informations";
        return 1;
    }

    if (args[0].isEmpty()){
        qCritical() << "Missing server name";
        return 1;
    }
    QString server_url = args[0];
    if (args[1].isEmpty()){
        qCritical() << "Missing server port";
        return 1;
    }
    int server_port = args[1].toInt();

    qInfo() << "Using server" << QString(server_url + ":" + args[1]);

    if (parser.isSet(register_option)){
        qInfo() << "-> Registering device option";
        QString device_name = parser.value(device_name_option);
        int id_device_type = parser.value(device_type_option).toInt();
        if (device_name.isEmpty()){
            qCritical() << "   Missing device name";
            return 1;
        }
        if (id_device_type <=0){
            qCritical() << "   Missing id_device_type";
            return 1;
        }

        // Registering device

    }

    qWarning() << "Nothing to do! Exiting...";

    return 0;
}
