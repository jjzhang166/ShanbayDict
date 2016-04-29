import QtQuick 2.6
import QtQuick.Window 2.2
Window {
    id: window
    visible: false
    title:"ShanbayDict 2.0 Login Dialog"
    width:520
    height:loginForm.height
    objectName: "Login Window"
    signal signalLoginClick(string name,string pass,string captchacode)
    signal signalFreshCaptchaImg()
    signal signalRegisterClick()
    function setState(msg) {
        //console.log("QML received: " + msg);
        if(msg.indexOf("请登录")>0) loginForm.btn_login.enabled = true;
        loginForm.captcha_code.text = "";
        winState.visible = true;
        winState.text = msg;
    }
    function showCaptchaImg(url){
        //console.log("captcha_imd url:"+url);
        if(url.length>0){
            loginForm.row1.visible=true;
            loginForm.captcha_img.source=url;
        }else{
            loginForm.row1.visible=false;
        }
    }

    LoginForm {
        id: loginForm
        row1.visible: false;
        btn_login.onClicked: {
            winState.visible=false;
            signalLoginClick(username.text,password.text,captcha_code.text);
        }
        btn_register.onClicked: {
            signalRegisterClick();
        }

        captcha_img_mousearea.onClicked: {
            signalFreshCaptchaImg();
        }
        checkBox_savepass.onCheckedChanged: {
            configs.setSavepass(checkBox_savepass.checkedState);
        }

        Keys.onPressed: {
            if(username.focus) {
                password.forceActiveFocus();
                return;
            }
            if(password.focus) {
                if(row1.visible){
                    captcha_code.forceActiveFocus();
                    return;
                }
            }
            signalLoginClick(username.text,password.text,captcha_code.text);
        }
    }
    Text {
        id: winState
        color: "#0a3ea2"
        text: qsTr("Label state")
        anchors.bottom: parent.bottom
        style: Text.Normal
        font.bold: true
        font.pointSize: 12
    }
    Component.onCompleted: {
        setX(Screen.width / 2 - width / 2);
        setY(Screen.height / 2 - height / 2);
        loginForm.username.text = cfg.getUsername();
        loginForm.password.text = cfg.getUserpass();
        loginForm.checkBox_savepass.checked=cfg.isSavepass();
    }

}
