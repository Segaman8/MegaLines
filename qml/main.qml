/* INCLUDES */

import QtQuick 2.0
import QtQuick.Layouts 1.0
import QtQuick.Controls 1.0
import QtQuick.Window 2.12
import com.PlaygroundField 1.0

/****************************************//**
 * @brief Main Gui Form
 * @date 23.01.24
 * @author Segaman
 *******************************************/

Window {
    id: window
    width: 480
    height: 800
    visible: true
    title: "Mega Lines by Segaman"
    color: "#333"

    /****************************************//**
     * @name VARIABLES
     ********************************************/
    /// @{

    property string formName: "MainQml"

    property QtObject internal: QtObject {
        property real cellSize: window.width / fieldControler.width
    }

    PlaygroundFieldModel {
        id: fieldModel

        Component.onCompleted: attachControler (fieldControler);
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

    Component {
        id: compCell

        Rectangle {
            width:  window.internal.cellSize
            height: window.internal.cellSize
            radius: height
            color: {
                var colorSet =
                [
                    "#a00",
                    "#00a",
                    "#0a0",
                    "#aa0",
                    "#444",
                ];
                return colorSet[model.type];
            }
        }
    }

    /// @}
    /****************************************//**
     * Content
     ********************************************/

    GridView {
        anchors.centerIn: parent
        width: window.width
        height: window.width
        cellWidth: window.internal.cellSize
        cellHeight: window.internal.cellSize

        model: fieldModel
        delegate: compCell
    }
}

/*-----------------------------------------*/
