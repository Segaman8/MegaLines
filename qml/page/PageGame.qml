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

    /* internal variables*/

    property QtObject internal: QtObject {
        /* constants */

        readonly property real moveAnimSpeed: 125

        /* variables */

        property bool landscape: root.height < root.width
        property real fieldSize: landscape ? root.height : root.width
        property real cellSize: fieldSize / fieldControler.width
        property int score: 0
        property var removed: []
        property bool removedActive: false

        /* animations */

        Behavior on score {
            PropertyAnimation {
                duration: 500
                easing.type: Easing.OutQuad
            }
        }
    }

    /* moved animation variables */

    property QtObject moved: QtObject {
        property bool active: false
        property int fromX
        property int fromY
        property int toX
        property int toY
        property int type
    }

    /* model */

    PlaygroundFieldModel {
        id: fieldModel

        Component.onCompleted: attachControler (fieldControler);
    }

    /* moved animation timer */

    Timer {
        id: movedDeactivationTimer
        interval: root.internal.moveAnimSpeed
        repeat: false
        running: false
        onTriggered: {
            root.moved.active           = false;
            root.internal.removedActive = false;
            root.internal.removed       = [];
        }
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

    function slotDestroyed (a_positions) {
        root.internal.removed       = a_positions;
        root.internal.removedActive = true;
    }

    /// @}
    /****************************************//**
     * @name COMPONENTS
     ********************************************/
    /// @{

    /* sphere object */

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

    /* cell delegate */

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

                // onMarginChanged: {
                //     if (margin === parent.height)
                //         destroyedImage.begin();
                // }
            }

            /* destroyed */

            Image {
                id: destroyedImage
                anchors.fill: parent
                source: "qrc:/assets/Star.png"
                opacity: 0

                transform: Rotation {
                    id: rotation
                    origin.x: width / 2
                    origin.y: height / 2
                    angle: 0
                }

                property bool active: root.internal.removedActive

                //function begin() {
                onActiveChanged: {
                    if (active === false)
                        return;

                    //console.log(`${model.x}x${model.y}`);

                    /* check if removed */
                    var found = false;
                    for (var i = 0; i < root.internal.removed.length; i++)
                    {
                        var pos = root.internal.removed[i];
                        if (pos.x === model.x
                            && pos.y === model.y)
                        {
                            found   = true;
                            break;
                        }
                    }

                    /* if not found, return */
                    if (found === false)
                        return;

                    /* start animation */
                    opacity = 1;
                    destroyedTimer.start();
                }

                Behavior on opacity { PropertyAnimation { duration: 25 } }

                PropertyAnimation {
                    target: rotation
                    property: "angle"
                    from: 0
                    to: 360
                    duration: 500
                    easing.type: Easing.Linear
                    loops: Animation.Infinite
                    running: destroyedImage.opacity !== 0
                }

                Timer {
                    id: destroyedTimer
                    interval: 250
                    repeat: false
                    running: false
                    onTriggered: destroyedImage.opacity = 0
                }
            }

            /* clickable */

            MouseArea {
                anchors.fill: parent
                enabled: !root.moved.active
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
        id: scoreLabel
        anchors.top: parent.top
        anchors.right: parent.right
        anchors.topMargin: 12
        anchors.rightMargin: 20
        z: 50

        font.pixelSize: 20
        text: `Score: ${root.internal.score}`
        color: "#ddd"
    }

    /* menu button */

    ButtonWidget {
        anchors.top: scoreLabel.top
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

        /* change animation speed */

        onActiveChanged: {
            var speed   = active ? root.internal.moveAnimSpeed : 0;

            movedSphereAnimX.duration   = speed;
            movedSphereAnimY.duration   = speed;

            /* deactivate after animation is done */
            if (active)
                movedDeactivationTimer.start();
        }
    }
}

/*-----------------------------------------*/
