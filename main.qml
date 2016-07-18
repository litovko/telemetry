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
            property var someNumber: tk.current
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
                        console.log("destroying: " + i)
                        c.children[i-1].destroy()
                      }
                    for (var i=0;i<tk.current.length;i++) {
                      var r = parametrstring.createObject(c)
                      r.text=tk.current[i]
                    }
                }
            }
    }

}
