/* INCLUDES */

import QtQuick 2.0
import QtQuick.Layouts 1.0
import QtQuick.Controls 1.0
import com.PlaygroundField 1.0

import "../widgets"

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
    property bool gameOver: false

    property QtObject internal: QtObject {
        property bool landscape: root.height < root.width
        property real fieldSize: landscape ? root.height : root.width
        property real cellSize: fieldSize / fieldControler.width
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
    signal sigMenuClicked();

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
            radius: height * 0.125
            color: "#444"
            border.width: 2
            border.color: "#333"

            Rectangle {
                anchors.fill: parent
                anchors.margins: parent.height * 0.025
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
                visible: model.type < 4

                Rectangle {
                    anchors.fill: parent
                    anchors.bottomMargin: parent.height * 0.25
                    anchors.leftMargin: parent.width * 0.125
                    anchors.rightMargin: parent.width * 0.125
                    color: "#eee"
                    radius: height
                    opacity: 0.25
                }
            }

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
        z: 50

        font.pixelSize: 14
        text: `Score: ${root.internal.score}`
        color: "#ddd"
    }

    /* menu button */

    ButtonWidget {
        anchors.top: parent.top
        anchors.right: parent.right
        anchors.topMargin: 30
        z: 50

        width: 96
        height: 40
        text: "Menu"
        onClicked: function() {
            root.sigMenuClicked();
        }
    }

    /* play field */

    GridView {
        y: root.internal.landscape
         ? 0
         : (root.height - height) / 2
        z: 10
        width: root.internal.fieldSize
        height: root.internal.fieldSize
        cellWidth: root.internal.cellSize
        cellHeight: root.internal.cellSize

        model: fieldModel
        delegate: compCell
    }

    /* game over */

    RotatingImageWidget {
        x: root.internal.landscape
         ? width * 0.05
         : (root.width - width) / 2
        y: (root.height - height) / 2
        z: 50
        width: root.internal.landscape
             ? root.width / 2
             : root.width * 0.75
        height: root.internal.landscape
              ? root.height
              : width * 0.75
        visible: root.gameOver
        source: "qrc:/assets/GameOver.png"
    }
}

/*-----------------------------------------*/
