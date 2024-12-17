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
        property int score: 0
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

    signal sigEntityClicked (int a_index);

    Component.onCompleted: bridgeManager.connectQmlForm (root);

    /// @}
    /****************************************//**
     * @name FUNCTIONS
     ********************************************/
    /// @{

    function slotScoreUpdate (a_value) {
        root.internal.score = a_value;
    }

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
            border.width: model.selected * 2
            border.color: "#eee"

            MouseArea {
                anchors.fill: parent
                onClicked: root.sigEntityClicked (model.index);
            }
        }
    }

    /// @}
    /****************************************//**
     * Content
     ********************************************/

    /* score */

    Text {
        anchors.top: parent.top
        anchors.right: parent.right
        anchors.topMargin: 12
        anchors.rightMargin: 20

        font.pixelSize: 14
        text: `Score: ${root.internal.score}`
        color: "#ddd"
    }

    /* play field */

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
