import QtQuick 2.5
import QtQuick.Window 2.0
import Gyco 1.0

Window {
    visible: true
    title: "HYCO ТК-15 Телеметрия"

    height: 600
    width: 600
    color: "transparent"
    MouseArea {
        anchors.fill: parent
        onClicked: {
            Qt.quit();
        }
    }

    TK15 {
    id: tk
    }
    Rectangle {
        color: 'black';
        anchors.fill: parent
        border.color: "yellow"
        radius:20
        border.width: 3
        focus:true
        Column{
            id: c
            spacing: 15
            anchors.margins:   10
            anchors.top: parent.top
            anchors.left: parent.left
            //padding:5

            Text
            {
                Rectangle{anchors.fill: parent
                gradient: Gradient {
                    //GradientStop { position: 0.0; color: "black" }
                    GradientStop { position: 0.7; color: "transparent" }
                    GradientStop { position: 1.0; color: "blue" }
                }
                }

                text: qsTr("Адрес:порт: "+tk.address+":"+tk.port)
                color: "yellow"
                font.bold: true
                font.pointSize: 20

            }
            Text {
                Rectangle{anchors.fill: parent
                gradient: Gradient {
                    //GradientStop { position: 0.0; color: "black" }
                    GradientStop { position: 0.7; color: "transparent" }
                    GradientStop { position: 1.0; color: "blue" }
                }
                }
                text: qsTr("Данные:"+tk.data)
                color: "#ffff00"

                font.bold: true
                font.pointSize: 20

            }
            Text {
                text: qsTr("Ток1:"+tk.current[0])
                color: "#ffff00"
                font.bold: true
                font.pointSize: 20
            }
            Text {
                text: qsTr("Ток2:"+tk.current[1])
                color: "#ffff00"
                font.bold: true
                font.pointSize: 20
            }

            }
    }

}
