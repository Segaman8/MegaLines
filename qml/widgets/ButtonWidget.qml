/* INCLUDES */

import QtQuick 2.0
import QtQuick.Layouts 1.0
import QtQuick.Controls 1.0

/****************************************//**
 * @brief Button Widget
 * @date 17.12.24
 * @author Segaman
 *******************************************/

Rectangle {
    id: root
    color: root.internal.hovered ? "#666" : "#555"
    radius: height * 0.125

    /****************************************//**
     * @name VARIABLES
     ********************************************/
    /// @{

    property string text
    property var onClicked

    property QtObject internal: QtObject {
        property bool hovered: false
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

    Rectangle {
        anchors.fill: parent
        anchors.bottomMargin: parent.height / 2
        z: 20
        radius: parent.radius
        color: "#eee"
        opacity: root.internal.hovered ? 0.175 : 0.125
    }

    Text {
        anchors.fill: parent
        z: 30
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        font.pixelSize: 20
        text: root.text
        color: "#eee"
    }

    MouseArea {
        anchors.fill: parent
        z: 40
        hoverEnabled: true
        onClicked: root.onClicked();
        onEntered: root.internal.hovered = true;
        onExited:  root.internal.hovered = false;
    }

}

/*-----------------------------------------*/
