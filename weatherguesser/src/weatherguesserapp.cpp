/* Copyright (c) 2012 Research In Motion Limited.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include "weatherguesserapp.h"
#include "citymodel.h"
#include "weathermodel.h"

#include <bb/cascades/TabbedPane>
#include <bb/cascades/QmlDocument>

WeatherGuesserApp::WeatherGuesserApp()
{
    // We set up the application Organization and name, this is used by QSettings
    // when saving values to the persistent store, in this app the home page is kept in the settings.
    QCoreApplication::setOrganizationName("Example");
    QCoreApplication::setApplicationName("Weather Guesser");

    // Here we create a QMLDocument and load it, we are using build patterns.
    mQmlDocument = QmlDocument::create().load("main.qml");
    mQmlDocument->setParent(this);

    // Create the cities and weather model, these are pages which is not part of the
    // NavigationPane, the application will handle navigation to these.
    createCitiesModel();
    createWeatherModel();

    // Set up the favorite and home page, load models and connects to the appropriate signals.
    createFavoritesModel();
    createHomeModel();

    if (!mQmlDocument->hasErrors()) {

        // The application navigationPane is created from QML.
        TabbedPane *tabs = mQmlDocument->createRootNode<TabbedPane>();

        if (tabs) {
            // Finally the main scene for the application is set to NavigationPane.
            Application::setScene(tabs);
        }
    }
}

WeatherGuesserApp::~WeatherGuesserApp()
{

}

void WeatherGuesserApp::createCitiesModel()
{
    // Create and bind the cityModel so that it can be accessed in the ContinentCitiesPage.QML
    // via the _cityModel context property.
    CityModel *cityModel = new CityModel(QStringList() << "name", "continents_connection", this);
    mQmlDocument->setContextProperty("_cityModel", cityModel);
}

void WeatherGuesserApp::createWeatherModel()
{
    // Create a WeatherModel that will load a weather forecast based on its
    // city property (see WeatherPage.qml and FavoritePage.qml).
    WeatherModel *weatherModel = new WeatherModel(this);
    mQmlDocument->setContextProperty("_weatherModel", weatherModel);
}

void WeatherGuesserApp::createFavoritesModel()
{
    // Create a CityModel that will load the favorite cites which are presented
    // in a list on the FavoritePage.qml.
    CityModel *favoriteModel = new CityModel(QStringList() << "name", "favorites_connection", this);
    mQmlDocument->setContextProperty("_favoriteModel", favoriteModel);

    // Initial load of the favorites list.
    favoriteModel->loadFavoriteCities();
}

void WeatherGuesserApp::createHomeModel()
{
    // The Home page is a special case for the WeatherModel and is set to be used
    // on the first tab in main.qml (see also WeatherItem.qml).
    WeatherModel *homeModel = new WeatherModel(this);
    mQmlDocument->setContextProperty("_homeModel", homeModel);

    // Connect to the homeModel city changed signal in order to update the application
    // setting for the home city so that it will be set on the next time the app launches.
    connect(homeModel, SIGNAL(cityChanged(QString )), this, SLOT(onUpdateHomeCity(QString )));

    // Start loading weather data for the home page, if no home town is stored in
    // the application settings London is loaded as the home town.
    QSettings settings;
    QString homeTown = "London";
    if (!settings.value("homeCity").isNull()) {
        homeTown = settings.value("homeCity").toString();
    }

    homeModel->setCity(homeTown);
}

void WeatherGuesserApp::onUpdateHomeCity(QString city)
{
    // Store the home town in the application settings and set the hometown city property.
    QSettings settings;
    settings.setValue("homeCity", QVariant(city));
}
