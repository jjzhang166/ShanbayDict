import QtQuick 2.6
import QtQuick.Controls 1.5
import QtQuick.Layouts 1.3
import QtQuick.Controls.Styles 1.3

Item {
    id: item1
    property alias btn_login: btn_login
    property alias username: username
    property alias password: password
    property alias labelState: labelState
    property alias captcha_code:captcha_code
    property alias captcha_img:captcha_img
    property alias captcha_img_mousearea:captcha_img_mousearea
    property alias checkBox_savepass: checkBox_savepass
    property alias row1:row1
    Label {
        id: label1
        x: 13
        y: 70
        text: qsTr("账号：")
        font.pointSize: 15
        font.bold: true
    }

    TextField {
        id: username
        x: 75
        y: 60
        width: 180
        height: 40
        text: ""
        font.bold: true
        font.pointSize: 12
        placeholderText: qsTr("用户名或邮箱")
    }

    Label {
        id: label2
        x: 266
        y: 71
        width: 30.7
        height: 20
        text: qsTr("密码:")
        font.bold: true
        font.pointSize: 15
    }

    TextField {
        id: password
        x: 314
        y: 60
        width: 180
        height: 41
        text: ""
        echoMode: 2
        placeholderText: qsTr("your password")
    }

    ColumnLayout {
        id: columnLayout1
        x: 12
        y: 116
        width: 476
        Row {
            id: row1
            Layout.fillWidth: true
            visible: true
            Label {
                id: label3
                text: qsTr("验证码：")
                anchors.verticalCenter: parent.verticalCenter
                font.bold: true
                font.pointSize: 12
            }

            TextField {
                id: captcha_code
                height:41
                anchors.verticalCenter: parent.verticalCenter
                width: 210
                placeholderText: qsTr("点击图片换一幅")
            }
            Image {
                id: captcha_img
                width: 166
                height: 50
                source: ""
                MouseArea{
                    id: captcha_img_mousearea
                    height: 50
                    anchors.fill: parent
                    cursorShape: Qt.PointingHandCursor
                }
            }
        }
        Row{
            id: row2
            Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
            Button {
                id: btn_login
                width: 90
                height: 30
                text: qsTr("登录")
                anchors.verticalCenter: parent.verticalCenter
                style: ButtonStyle {
                      label: Text {
                          renderType: Text.NativeRendering
                          verticalAlignment: Text.AlignVCenter
                          horizontalAlignment: Text.AlignHCenter
                          font.family: "Helvetica"
                          font.bold: true
                          font.pointSize: 12
                          text: control.text
                      }
                }
            }
        }
        Row {
            id: row3
            Layout.alignment: Qt.AlignRight | Qt.AlignVCenter
            CheckBox {
                id: checkBox_savepass
                text: qsTr("记住密码")
                checked: true
            }
        }
        Row {
            id: row4
            Label {
                id: labelState
                color: "#0a3ea2"
                text: qsTr("Label state")
                style: Text.Normal
                font.bold: true
                font.pointSize: 12
            }
        }
    }


}
