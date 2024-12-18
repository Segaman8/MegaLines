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
        readonly property real moveAnimSpeed: 62.5
        property bool landscape: root.height < root.width
        property real fieldSize: landscape ? root.height : root.width
        property real cellSize: fieldSize / fieldControler.width
        property int score: 0

    }

    property QtObject moved: QtObject {
        property bool active: false
        property int fromX
        property int fromY
        property int toX
        property int toY
        property int type
    }

    PlaygroundFieldModel {
        id: fieldModel

        Component.onCompleted: attachControler (fieldControler);
    }

    Timer {
        id: movedDeactivationTimer
        interval: root.internal.moveAnimSpeed
        repeat: false
        running: false
        onTriggered: root.moved.active = false;
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

    function slotMoved (a_fromX, a_fromY, a_toX, a_toY, a_type) {
        root.moved.fromX    = a_fromX;
        root.moved.fromY    = a_fromY;
        root.moved.toX      = a_toX;
        root.moved.toX      = a_toY;
        root.moved.type     = a_type;

        movedSphere.moveTo (a_fromX, a_fromY);
        root.moved.active   = true;
        movedSphere.moveTo (a_toX, a_toY);
    }

    /// @}
    /****************************************//**
     * @name COMPONENTS
     ********************************************/
    /// @{

    Component {
        id: compSphere

        // property int type
        // property real margin
        // property bool selected

        Rectangle {
            id: compSphereRoot
            anchors.fill: parent
            anchors.margins: margin
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
                return colorSet[type];
            }
            border.width: selected * 2
            border.color: "#eee"

            /* variables */

            property int type: compSphereRoot.parent.type
            property real margin: compSphereRoot.parent.margin
            property bool selected: compSphereRoot.parent.selected

            /* grow/shrink animation */

            Behavior on margin {
                PropertyAnimation {
                    id: marginAnim
                    duration: 125
                }
            }

            /* sphere glass effect */

            Rectangle {
                anchors.fill: parent
                anchors.bottomMargin: parent.height * 0.25
                anchors.leftMargin: parent.width * 0.125
                anchors.rightMargin: parent.width * 0.125
                color: "#eee"
                radius: height
                opacity: 0.25
            }

            /* turn off shrink animation, if sphere destroyed */

            function updateAnim() {
                marginAnim.duration = type === 4 ? 125 : 0;
            }

            onTypeChanged: updateAnim()
            onMarginChanged: updateAnim()
        }
    }

    Component {
        id: compCell

        /* cell background */

        Rectangle {
            width:  root.internal.cellSize
            height: root.internal.cellSize
            radius: height * 0.125
            color: "#444"
            border.width: 2
            border.color: "#333"

            /* sphere */

            Loader {
                anchors.fill: parent
                sourceComponent: compSphere

                property int type: model.type
                property real margin: type < 4
                                    ? parent.height * 0.025
                                    : parent.height
                property bool selected: model.selected
                // property int xx: model.x
                // property int yy: model.y
            }

            // Rectangle {
            //     anchors.fill: parent
            //     anchors.margins: margin
            //     radius: height
            //     color: {
            //         var colorSet =
            //         [
            //             "#a00",
            //             "#00a",
            //             "#0a0",
            //             "#aa0",
            //             "#444",
            //         ];
            //         return colorSet[type];
            //     }
            //     border.width: model.selected * 2
            //     border.color: "#eee"

            //     /* variables */

            //     property int type: model.type
            //     property real margin: type < 4
            //                         ? parent.height * 0.025
            //                         : parent.height

            //     /* grow/shrink animation */

            //     Behavior on margin {
            //         PropertyAnimation {
            //             id: marginAnim
            //             duration: 125
            //         }
            //     }

            //     /* sphere glass effect */

            //     Rectangle {
            //         anchors.fill: parent
            //         anchors.bottomMargin: parent.height * 0.25
            //         anchors.leftMargin: parent.width * 0.125
            //         anchors.rightMargin: parent.width * 0.125
            //         color: "#eee"
            //         radius: height
            //         opacity: 0.25
            //     }

            //     /* turn off shrink animation, if sphere destroyed */

            //     onTypeChanged: {
            //         marginAnim.duration = type < 4 ? 125 : 0;
            //     }
            // }

            /* clickable */

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
        id: playgroundFieldView
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

    /* sphere moved animation */

    Loader {
        id: movedSphere
        z: 40
        width: root.internal.cellSize
        height: root.internal.cellSize
        visible: active
        sourceComponent: compSphere

        /* animations */

        Behavior on x {
            PropertyAnimation {
                id: movedSphereAnimX
                duration: 0
            }
        }

        Behavior on y {
            PropertyAnimation {
                id: movedSphereAnimY
                duration: 0
            }
        }

        /* functions */

        function moveTo (a_x, a_y) {
            x   = playgroundFieldView.x + a_x * width;
            y   = playgroundFieldView.y + a_y * height;
        }

        /* variables */

        property int type: root.moved.type
        property real margin: height * 0.025
        property bool selected: true

        property bool active: root.moved.active

        /* animation speed */

        onActiveChanged: {
            var speed   = active ? root.internal.moveAnimSpeed : 0;

            movedSphereAnimX.duration   = speed;
            movedSphereAnimY.duration   = speed;

            if (active)
                movedDeactivationTimer.start();
        }
    }
}

/*-----------------------------------------*/
