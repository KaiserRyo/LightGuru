/*
    Copyright 2013 Gael Jaffrain

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "lightguru.h"
#include "RegistrationHandler.hpp"

#include <Qt/qdeclarativedebug.h>

#include <QLocale>
#include <QTranslator>

using ::bb::cascades::Application;

Q_DECL_EXPORT int main(int argc, char **argv)
{
    // Instantiate the main application constructor.
    Application app(argc, argv);

    // Set up the translator.
    QTranslator translator;
    QString locale_string = QLocale().name();
    QString filename = QString("lightguru_%1").arg(locale_string);
    if (translator.load(filename, "app/native/qm")) {
        app.installTranslator(&translator);
    }

    // Define LightGuru UUID
    const QUuid uuid(QLatin1String("aef8d7f0-f277-11e2-b778-0800200c9a66"));

    // Register with BBM.
    RegistrationHandler* registrationHandler = new RegistrationHandler(uuid, &app);

    // Initialize our application.
    LightGuruApp *mainApp = new LightGuruApp(registrationHandler->context(), &app);

    QObject::connect(registrationHandler, SIGNAL(stateChanged()), mainApp, SLOT(show()));

    // Now that the signal is connected to the slot we can fire the registration process
    // When BBM registration process will be finished, then the main UI will be shown
    registrationHandler->registerApplication ();

    // We complete the transaction started in the main application constructor and start the
    // client event loop here. When loop is exited the Application deletes the scene which
    // deletes all its children.
    return Application::exec();
}

