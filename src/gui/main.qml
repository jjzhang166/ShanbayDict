import QtQuick 2.6
import QtQuick.Window 2.2
import QtMultimedia 5.6
import Qt.labs.settings 1.0
Window {
    id: window
    visible: false
    width: 800
    height: 600
    property var voc
    signal signalBtnqueryClick(string word)
    signal signalBtnaddwordClick(string type,string id)
    //slot
    function addWordRet(retstr){
        console.log(" add word ret"+retstr);
        var json=JSON.parse(retstr);
        if(json.status_code !== 0 ){
            mainForm.word_name.text = "添加新词失败：" + json.msg;
            return;
        }
        updateBtnaddword(json.data.id);
    }
    function updateBtnaddword(learning_id){
        if(learning_id&&learning_id!==0){
            mainForm.btn_addword.iconSource = "qrc:/img/addword0.ico";
            mainForm.btn_addword.tooltip = qsTr("忘记了，重新加入背单词计划");
        }else{
            mainForm.btn_addword.iconSource = "qrc:/img/addword1.ico";
            mainForm.btn_addword.tooltip = qsTr("添加进生词库，加入背单词计划");
        }
    }

    function showWord(wordstr){
        //console.log("wordstr:"+wordstr);
        var json = JSON.parse(wordstr);
        if(json.status_code !== 0 ){
            mainForm.word_name.text = mainForm.textWord.text+" not found." + json.msg;
            mainForm.btn_addword.visible = false;
            mainForm.pronu_uk.visible = false;
            mainForm.btn_sound0.visible = mainForm.btn_sound1.visible = false;
            return;
        }        
        mainForm.btn_addword.visible = true;
        mainForm.pronu_us.visible = true;
        mainForm.btn_sound0.visible = mainForm.btn_sound1.visible = true;
        voc = json.data;
        updateBtnaddword(voc.learning_id);
        mainForm.word_name.text = voc.content;
        mainForm.pronu_uk.text = "/"+voc.pronunciations.uk+"/";
        mainForm.pronu_us.text = "/"+voc.pronunciations.us+"/";
        if(voc.has_audio){
            mainForm.btn_sound0.visible = mainForm.btn_sound1.visible = true;
            playSound0.source = voc.uk_audio;
            playSound1.source = voc.us_audio;
            if(configs.isAutospeak()) playSound1.play();
        }else{
            mainForm.btn_sound0.visible = mainForm.btn_sound1.visible = false;
        }
        var def_en_info = "<table style='background-Color:mintcream' cellSpacing=6>";
        for(var pos in voc.en_definitions){
            //console.log(pos);
            def_en_info += "<tr><td align='right'><b>"+pos +":<b></td><td>";
            var defs=voc.en_definitions[pos];
            for(var i=0;i<defs.length;i++){
                def_en_info += "<b>"+(i+1) + ".</b>" + defs[i] +"<br>";
            }
            def_en_info = def_en_info.substring(0,def_en_info.length-4);
            def_en_info+="</td></tr>"
            //console.log(def_en_info);
        }
        def_en_info+="</table>"
        mainForm.text_def.text =voc.definition+ def_en_info ;
        //console.log("wordinfo.definition:"+voc.definition);
    }
    Audio {
            id: playSound0
        }
    Audio {
            id: playSound1
        }

    Flickable {
        id: view
        anchors.fill: parent
        contentWidth: mainForm.width
        contentHeight: mainForm.height

        MainForm {
            id: mainForm
            width:  text_def.width + 20
            height: text_def.height + 100
            btn_query.onClicked: {
                signalBtnqueryClick(textWord.text);
            }
            btn_addword.onClicked: {
                if(voc.learning_id&&voc.learning_id!==0){
                    signalBtnaddwordClick("relearn",voc.learning_id);
                }else{
                    signalBtnaddwordClick("add",voc.object_id);
                }
            }

            btn_sound0.onClicked: {
                //console.log(mainForm.height+"  "+view.contentHeight)
                playSound0.play();
            }
            btn_sound1.onClicked: {
                playSound1.play();
            }
            Keys.onPressed: {
                console.log(event.key + "enterrrrrrrrrrrrrrrrrrrrrrrrrrrr"+Qt.Key_Enter +" "+Qt.Key_Return)
                if(event.key === Qt.Key_Enter || event.key === Qt.Key_Return){
                    if(textWord.focus) {
                        signalBtnqueryClick(textWord.text);
                        return;
                    }
                }
            }
            Component.onCompleted: {
                textWord.forceActiveFocus();
            }
        }

        // Only show the scrollbars when the view is moving.
        states: State {
            name: "ShowBars"
            when: view.movingVertically || view.movingHorizontally
            PropertyChanges { target: verticalScrollBar; opacity: 1 }
            PropertyChanges { target: horizontalScrollBar; opacity: 1 }
        }

        transitions: Transition {
            NumberAnimation { properties: "opacity"; duration: 400 }
        }
    }

    // Attach scrollbars to the right and bottom edges of the view.
    ScrollBar {
        id: verticalScrollBar
        width: 12; height: view.height-12
        anchors.right: view.right
        opacity: 0
        orientation: Qt.Vertical
        position: view.visibleArea.yPosition
        pageSize: view.visibleArea.heightRatio
    }

    ScrollBar {
        id: horizontalScrollBar
        width: view.width-12; height: 12
        anchors.bottom: view.bottom
        opacity: 0
        orientation: Qt.Horizontal
        position: view.visibleArea.xPosition
        pageSize: view.visibleArea.widthRatio
    }
    Component.onCompleted: {
        setX(Screen.width / 2 - width / 2);
        setY(Screen.height / 2 - height / 2);
        mainForm.word_name.text = "";
        mainForm.text_def.text = "";
        mainForm.btn_addword.visible = false;
        mainForm.pronu_us.visible = false;
        mainForm.btn_sound0.visible = mainForm.btn_sound1.visible = false;
        console.log("main window"+configs.value("getscreentext"));
    }
    onClosing:{
        console.log("main windows is closing");
        //visible = false;
        //close.accepted = false;
    }
}
