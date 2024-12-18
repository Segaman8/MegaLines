/* INCLUDES */

import QtQuick 2.0
import QtQuick.Layouts 1.0
import QtQuick.Controls 1.0

/****************************************//**
 * @brief Rotating Image Widget
 * @date 18.12.24
 * @author Segaman
 *******************************************/

Image {
    id: root

    /****************************************//**
     * @name VARIABLES
     ********************************************/
    /// @{

    fillMode: Image.PreserveAspectFit

    transform: Rotation {
        id: rotation
        origin.x: root.width / 2
        origin.y: root.height / 2
        angle: 0
    }

    property real animAngle: 3.125
    property int duration: 1000

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

    PropertyAnimation {
        target: rotation
        property: "angle"
        from: 0 - root.animAngle
        to: root.animAngle
        duration: root.duration
        easing.type: Easing.InOutSine
        loops: 1
        running: true

        onStopped: {
            if (from === 0 - root.animAngle)
            {
                from = root.animAngle;
                to   = 0 - root.animAngle;
                start();
            }
            else
            {
                from = 0 - root.animAngle;
                to   = root.animAngle;
                start();
            }
        }
    }
}

/*-----------------------------------------*/
