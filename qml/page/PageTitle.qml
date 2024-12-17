/* INCLUDES */

import QtQuick 2.0
import QtQuick.Layouts 1.0
import QtQuick.Controls 1.0

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
        property bool landscape: root.height < root.width
    }

    /// @}
    /****************************************//**
     * @name SIGNALS
     ********************************************/
    /// @{

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

    Image {
        x: root.internal.landscape
         ? width * 0.05
         : (root.width - width) / 2
        width: root.internal.landscape
             ? root.width / 2
             : root.width * 0.75
        height: root.internal.landscape
              ? root.height
              : width * 0.75

        fillMode: Image.PreserveAspectFit
        source: "qrc:/assets/MegaLines.png"

        transform: Rotation {
            id: titleRotation
            origin.x: width / 2
            origin.y: height / 2
            angle: 0
        }

        PropertyAnimation {
            target: titleRotation
            property: "angle"
            from: 0 - root.internal.titleAnimAngle
            to: root.internal.titleAnimAngle
            duration: 1000
            easing.type: Easing.InOutSine
            loops: 1
            running: true

            onStopped: {
                if (from === 0 - root.internal.titleAnimAngle)
                {
                    from = root.internal.titleAnimAngle;
                    to   = 0 - root.internal.titleAnimAngle;
                    start();
                }
                else
                {
                    from = 0 - root.internal.titleAnimAngle;
                    to   = root.internal.titleAnimAngle;
                    start();
                }
            }
        }
    }
}

/*-----------------------------------------*/
