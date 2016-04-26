import QtQuick 2.6
import QtQuick.Controls 1.5
import QtQuick.Controls.Styles 1.4
import QtQuick.Layouts 1.3

Rectangle {
    id: rectangle1
    property alias textWord: textWord
    property alias btn_query: btn_query
    property alias btn_sound0: btn_sound0
    property alias btn_sound1: btn_sound1
    property alias word_name: word_name
    property alias pronu_us: pronu_us
    property alias pronu_uk: pronu_uk
    property alias text_def: text_def
    property alias btn_addword: btn_addword
    //anchors.fill: parent

    radius: 2
    //color: "#ffffff"
    TextField {
        id: textWord
        x: 8
        y: 17
        width: 360
        height: 32
        text: "good"
        font.pointSize: 18
        placeholderText: qsTr("Text Field")
    }
    Button {
        id: btn_query
        x: 380
        y: 20
        width: 55
        height: 27
        text: qsTr("查询")
    }


    RowLayout {
        id: rowLayout_pronu
        x: 8
        y: 57
        width: 180
        height: 21
        Text {
            id: word_name
            text: qsTr("Text")
            style: Text.Normal
            font.bold: true
            verticalAlignment: Text.AlignBottom
            font.pixelSize: 18
        }
        Text {
            visible: pronu_uk.visible
            text: qsTr("US:")
            font.italic: true
            font.bold: false
            font.pixelSize: 12
        }
        Text {
            id: pronu_us
            visible: pronu_uk.visible
            color: "gray"
            text: qsTr("/ɡʊd/")
            font.pixelSize: 12
        }
        ToolButton {
            id: btn_sound0
            width: 16
            height: 16
            iconSource: "qrc:/img/sound0.ico"
        }
        Text {
            visible: pronu_uk.visible
            text: qsTr("UK:")
            font.italic: true
            font.bold: true
            font.pixelSize: 12
        }
        Text {
            id: pronu_uk
            color: "gray"
            text: qsTr("/ɡʊd/")
            font.bold: true
            font.pixelSize: 12
        }
        ToolButton {
            id: btn_sound1
            width: 16
            height: 16
            iconSource: "qrc:/img/sound1.ico"
        }

    }
    Text {
        id: text_def
        x: 8
        y: 95
        width: 601
        //height: 77
        text: qsTr("word definition")
        textFormat: Text.RichText
        wrapMode: Text.WrapAnywhere
        font.pixelSize: 15
    }

    ToolButton {
        id: btn_addword
        x: 579
        y: 57
        tooltip: qsTr("添加进生词库，加入背单词计划")
        iconSource: "qrc:/img/addword1.ico"
    }
}
