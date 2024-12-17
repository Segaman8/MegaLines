/* INCLUDES */

import QtQuick 2.0
import QtQuick.Layouts 1.0
import QtQuick.Controls 1.0
import QtQuick.Window 2.12
import com.PlaygroundField 1.0

/****************************************//**
 * @brief Page Game Form
 * @date 17.12.24
 * @author Segaman
 *******************************************/

Item {
    id: root

    /****************************************//**
     * @name VARIABLES
     ********************************************/
    /// @{

    property string formName: "PageGame"

    property QtObject internal: QtObject {
        property real cellSize: root.width / fieldControler.width
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

    Component {
        id: compCell

        Rectangle {
            width:  root.internal.cellSize
            height: root.internal.cellSize
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
        width: root.width
        height: root.width
        cellWidth: root.internal.cellSize
        cellHeight: root.internal.cellSize

        model: fieldModel
        delegate: compCell
    }
}

/*-----------------------------------------*/
