import QtQuick 2.6
import QtQuick.Window 2.2

Window {
    visible: false
    title:"Login Dialog"
    width:500
    height:300
    objectName: "Login Window"
    signal signalLoginClick(string name,string pass,string captchacode)
    signal signalFreshCaptchaImg()
    signal signalClosing()
    onClosing: {
        console.log("OnClosing fired");
        signalClosing();
    }
    function setState(msg) {
        console.log("QML received: " + msg);
        loginForm.captcha_code.text = "";
        loginForm.labelState.visible = true;
        loginForm.labelState.text = msg;
    }
    function showCaptchaImg(url){
        console.log("captcha_imd url:"+url);

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
            loginForm.labelState.visible=false;
            signalLoginClick(username.text,password.text,captcha_code.text);
        }
        captcha_img_mousearea.onClicked: {
            signalFreshCaptchaImg();
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
    Component.onCompleted: {
        setX(Screen.width / 2 - width / 2);
        setY(Screen.height / 2 - height / 2);
    }

}
