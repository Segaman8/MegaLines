/* INCLUDES */

import QtQuick 2.0
import QtQuick.Layouts 1.0
import QtQuick.Controls 2.0
import QtQuick.Window 2.12
import com.PlaygroundField 1.0

import "page"

/****************************************//**
 * @brief Main Gui Form
 * @date 16.12.24
 * @author Segaman
 *******************************************/

Window {
    id: window
    width: 480
    height: 800
    visible: false
    title: "Mega Lines by Segaman"
    color: "#333"

    /****************************************//**
     * @name VARIABLES
     ********************************************/
    /// @{

    property string formName: "MainQml"

    property int tab: 0
    property QtObject internal: QtObject {
    }

    /// @}
    /****************************************//**
     * @name SIGNALS
     ********************************************/
    /// @{

    Component.onCompleted: bridgeManager.connectQmlForm (window);

    /// @}
    /****************************************//**
     * @name FUNCTIONS
     ********************************************/
    /// @{

    /// @}
    /****************************************//**
     * @name COMPONENTS
     ********************************************/
    /// @{

    /// @}
    /****************************************//**
     * Content
     ********************************************/

    SwipeView {
        anchors.fill: parent
        currentIndex: tab
        interactive: false

        PageTitle {}
        PageGame {}
    }
}

/*-----------------------------------------*/
