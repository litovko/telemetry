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
        onClicked: {
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
            height: 160
            color: "black"

            Row {
                anchors.margins:   3
                anchors.top: parent.top
                anchors.left: parent.left
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
                        value: tk.temperature
                        centerТext: "t\u00B0"
                        bottomText: "Температура"
                        warningThreshold: maximumValue*0.8
                        minorTickmarks:1
                    }
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
                        warningThreshold: maximumValue*0.8
                        minorTickmarks:4
                    }
                 }
                Rectangle {
                    //anchors.margins: 5
                    width:  10;
                    height: 150;
                    color: tk.overshort_1===true?"red":"grey"
                }
                Rectangle {
                    //anchors.margins: 5
                    width:  10;
                    height: 150;
                    color: tk.overshort_2===true?"red":"green"
                }
                Rectangle {
                    color:"transparent";
                    width:  150;
                    height: 150;
                    anchors.margins: 5
                    Pribor {
                        width: parent.width-10; height: parent.height-10
                        maximumValue: 400
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
                    }
                 }
            }

        }
        Column{
            id: c
            //spacing: 10
            anchors.margins:   10
            anchors.top: panel.bottom
            anchors.left: parent.left
//            anchors.top: r.bottom
//            anchors.left: r.left
            property var someNumber: tk.list

            Component {
                    id: parametrstring
                    //property string str: "null"
                    Text
                    {
                        id: t
                        Rectangle{ border.color: "#f10606";anchors.fill: parent
                            opacity: 0.5
                        gradient: Gradient {
                            GradientStop {
                                position: 0.00;
                                color: "#cf7474";
                            }
                            GradientStop {
                                position: 0.50;
                                color: "#00aaff";
                            }
                            GradientStop {
                                position: 1.00;
                                color: "#888888";
                            }
                        }
                        }

                        text: qsTr("Адрес:порт: "+tk.address+":"+tk.port)
                        color: "yellow"
                        font.bold: true
                        font.pointSize: 20

                    }
                }
            onSomeNumberChanged: {

                    for(var i = c.children.length; i > 0 ; i--) {
                        //console.log("destroying: " + i)
                        c.children[i-1].destroy()
                      }
                    for (var i=0;i<tk.list.length;i++) {
                      var r = parametrstring.createObject(c)
                      r.text=tk.list[i]
                    }
                }
            }
    }

}
