import QtQuick 2.6
import QtQuick.Window 2.2
Window {
    id: window
    width: 630
    height: 360
    visible: false
    title: qsTr("扇贝词典")
    flags: Qt.Dialog
    AboutForm {
        labelAbout.text: cfg.getLicenceInfo()
        labelAbout.onLinkActivated: Qt.openUrlExternally(link)
    }
}
