/* INCLUDES */

import QtQuick 2.0
import QtQuick.Layouts 1.0
import QtQuick.Controls 1.0

import "../widgets"

/****************************************//**
 * @brief Page Title Form
 * @date 17.12.24
 * @author Segaman
 *******************************************/

Item {
    id: root

    /****************************************//**
     * @name VARIABLES
     ********************************************/
    /// @{

    property string formName: "PageTitle"

    property QtObject internal: QtObject {
        readonly property real titleAnimAngle: 3.125
        readonly property real buttonHeight: 64
        property bool landscape: root.height < root.width
    }

    /// @}
    /****************************************//**
     * @name SIGNALS
     ********************************************/
    /// @{

    signal sigContinueClicked();
    signal sigNewGameClicked();
    signal sigQuitClicked();

    Component.onCompleted: bridgeManager.connectQmlForm (root);

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

    /* title image */

    RotatingImageWidget {
        id: titleImage
        x: root.internal.landscape
         ? width * 0.05
         : (root.width - width) / 2
        width: root.internal.landscape
             ? root.width / 2
             : root.width * 0.75
        height: root.internal.landscape
              ? root.height
              : width * 0.75
        source: "qrc:/assets/MegaLines.png"
    }

    /* controls */

    ColumnLayout {
        x: root.internal.landscape
         ? root.width * 0.575
         : (root.width - width) / 2
        y: root.internal.landscape
         ? (root.height - height) / 2
         : titleImage.height
        width: root.internal.landscape
             ? root.width * 0.375
             : root.width * 0.75
        height: root.internal.buttonHeight * 3

        ButtonWidget {
            Layout.fillWidth: true
            Layout.preferredHeight: root.internal.buttonHeight
            text: "Continue"
            onClicked: function() {
                //console.log(`${text} clicked!`);
                root.sigContinueClicked();
            }
        }

        ButtonWidget {
            Layout.fillWidth: true
            Layout.preferredHeight: root.internal.buttonHeight
            text: "New Game"
            onClicked: function() {
                //console.log(`${text} clicked!`);
                root.sigNewGameClicked();
            }
        }

        ButtonWidget {
            Layout.fillWidth: true
            Layout.preferredHeight: root.internal.buttonHeight
            text: "Quit"
            onClicked: function() {
                //console.log(`${text} clicked!`);
                root.sigQuitClicked();
            }
        }
    }
}

/*-----------------------------------------*/
