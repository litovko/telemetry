import QtQuick 2.5
import QtQuick.Window 2.0
import Qt.labs.settings 1.0
import Gyco 1.0

Window {
    id: win
    visible: true
    title: "HYCO ТК-15 Телеметрия"
    color: "black"
    height: 700
    width: 800
    //color: "transparent"
    MouseArea {
        anchors.fill: parent
        onDoubleClicked: {
            win.fcommand("FULLSCREEN")
        }
    }
    Settings {
        property alias x: win.x
        property alias y: win.y
        property alias width: win.width
        property alias height: win.height
    }
    TK15 {
        id: tk
    }

    function fcommand (cmd) {
        console.log ("COMMAND="+cmd)
        switch(cmd) {
          case "QUIT":
              Qt.quit();
              break;
          case "PLAY":

              break;

          case "PROGRAM SETTINGS":

              break;
          case "HELP":

              break;

          case "MENU":

              break;
          case "FULLSCREEN":
              win.visibility = win.visibility===Window.FullScreen?Window.Maximized:Window.FullScreen;
              break;
        }

    }

    Rectangle {
        id: rect
        //color: "black";
        color: "transparent"
        anchors.fill: parent
        border.color: "yellow"
        radius:20
        border.width: 3
        focus:true
        Timer {
                interval: 5000; running: true; repeat: true
                onTriggered: {
                    rect.border.color = "black"
                    t2.start()
                }

            }
        Timer {
            id:t2
                interval: 10; running: false; repeat: false
                onTriggered: rect.border.color = "yellow"
            }

        Keys.onPressed: {
            //console.log("KeY:"+event.key)
            if (event.key === Qt.Key_F10 || event.key === Qt.Key_0) win.fcommand("QUIT")
            if (event.key === Qt.Key_F12 || event.key === Qt.Key_Equal) win.fcommand("FULLSCREEN")
        }
        MyBubble2 {
            id: bubble
            //anchors.centerIn: parent
            height:300
            width:300
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.top: panel.bottom
            anchors.topMargin: 20
            roll: Math.round(tk.angle1)
            pitch: Math.round(tk.angle2)
        }
        Rectangle {
            id: panel
            anchors.margins:   10
            anchors.top: parent.top
            anchors.left: parent.left
            border.color: "gray"
            radius: 20
            gradient: Gradient {
                GradientStop {
                    position: 0.00;
                    color: "#000000";
                }
                GradientStop {
                    position: 1.00;
                    color: "#222222";
                }
            }
            opacity: 0.8
            width: rect.width-anchors.margins*2
            height: prib.height
            color: "black"

            Flow {
                id: prib
                anchors.margins:   3
                anchors.top: parent.top
                anchors.left: parent.left
                anchors.right: parent.right

                MyLamp {
                    width:  150;
                    height: 150;
                    active: tk.overshort_1
                    bottomText: "Овершот 1"
                }
                MyLamp {
                    width:  150;
                    height: 150;
                    active: tk.overshort_2
                    bottomText: "Овершот 2"
                }


                Rectangle {
                    color:"transparent";
                    width:  150;
                    height: 150;
                    anchors.margins: 5
//                    anchors.top: parent.top
//                    anchors.left: parent.left
                    Pribor {
                        width: parent.width-10; height: parent.height-10
                        maximumValue: 200
                        minimunValue: 0
                        stepSize: 50
                        //anchors.centerIn: parent
                        value: tk.pressure
                        centerТext: "Атм."
                        bottomText: "Давление"
                        warningThreshold: maximumValue*0.6
                        minorTickmarks:4
                        precision: 0
                    }
                 }
                Rectangle {
                    color:"transparent";
                    width:  150;
                    height: 150;
                    anchors.margins: 5
                    Pribor {
                        width: parent.width-10; height: parent.height-10
                        maximumValue: 500
                        minimunValue: 0
                        stepSize: 100

                        value: tk.voltage
                        centerТext: "V"
                        bottomText: "Напряжение"
                        warningThreshold: maximumValue*0.8
                        minorTickmarks:4
                    }
                 }

                Rectangle {
                    color:"transparent";
                    width:  150;
                    height: 150;
                    anchors.margins: 5
                    Pribor {
                        width: parent.width-10; height: parent.height-10
                        maximumValue: 100
                        minimunValue: 0
                        stepSize: 20
                        //anchors.centerIn: parent
                        value: tk.current1
                        centerТext: "A"
                        bottomText: "Ток1"
                        warningThreshold: maximumValue*0.8
                        minorTickmarks:4
                        precision: 1
                    }
                 }
                Rectangle {
                    color:"transparent";
                    width:  150;
                    height: 150;
                    anchors.margins: 5
                    Pribor {
                        width: parent.width-10; height: parent.height-10
                        maximumValue: 100
                        minimunValue: 0
                        stepSize: 20
                        //anchors.centerIn: parent
                        value: tk.current2
                        centerТext: "A"
                        bottomText: "Ток2"
                        warningThreshold: maximumValue*0.8
                        minorTickmarks:4
                        precision: 1
                    }
                 }
                Rectangle {
                    color:"transparent";
                    width:  150;
                    height: 150;
                    anchors.margins: 5
                    Pribor {
                        width: parent.width-10; height: parent.height-10
                        maximumValue: 100
                        minimunValue: 0
                        stepSize: 20
                        //anchors.centerIn: parent
                        value: tk.current3
                        centerТext: "A"
                        bottomText: "Ток3"
                        warningThreshold: maximumValue*0.8
                        minorTickmarks:4
                        precision: 1
                    }
                 }
                Rectangle {
                    color:"transparent";
                    width:  150;
                    height: 150;
                    anchors.margins: 0
//                    anchors.top: parent.top
//                    anchors.left: parent.left
                    Pribor {
                        width: parent.width-10; height: parent.height-10
                        maximumValue: 120
                        minimunValue: -20
                        stepSize: 20
                        //anchors.centerIn: parent
                        value: 9//tk.temperature
                        centerТext: "t\u00B0"
                        bottomText: "Температура"
                        warningThreshold: maximumValue*0.8
                        minorTickmarks:1
                    }
                 }
            }

        }
        Row {
            anchors.bottom: parent.bottom
            anchors.left: parent.left
            anchors.margins: 20
            spacing: 20
            Rectangle {
                width: 50
                height: 50
                color: "black"
                border.color: "darkgray"
                radius: 5
                MyLamp {
//                    width:  50;
//                    height: 50;
                    anchors.fill: parent
                    active: tk.client_connected
                    bottomText: "Связь"
                }
            }
            Rectangle {
                width: 100
                height: 50
                color: "black"
                border.color: "darkgray"
                radius: 5
                Text {
                    anchors.centerIn: parent
                    color: "#e3e60f"
                    text: ""+tk.count
                    font.bold: true

                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                    font.pointSize: 10
                }
            }
            Rectangle {
                width: 250
                height: 50
                color: "black"
                border.color: "darkgray"
                radius: 5
                Text {
                    anchors.centerIn: parent
                    color: "#e3e60f"
                    text:  " Адрес:"+tk.address+ " Порт:" +tk.port+ " "+(tk.tcp?"TCP":"UDP");
                    font.bold: true
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                    font.pointSize: 10
                }
            }
        }
    }


}
