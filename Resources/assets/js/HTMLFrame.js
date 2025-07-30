// HTMLFrame.js : Global Javascript functions for Coral Remote Controller.

// Statement:
// Copyright (C) 2020-2025 Coral Studio. All Rights Reserved.

function CRLogout()
{
    document.cookie = "CORAL_REMOTE_CONTROLLER_MANAGE=; expires=Thu, 01 Jan 1970 00:00:00 UTC; path=/";
	window.location.href="/login";
}
function getCookie(cname) {
       var name = cname + "=";
    var decodedCookie = decodeURIComponent(document.cookie);
    var ca = decodedCookie.split(';');
    for (var i = 0; i < ca.length; i++) {
        var c = ca[i];
        while (c.charAt(0) == ' ') {
            c = c.substring(1);
        }
        if (c.indexOf(name) == 0) {
            return c.substring(name.length, c.length);
        }
    }
    return "";
}
function ChangeTextByAJAX(id, address)
{
    $.ajax({
        type:'post', 
        url:address,
        async:false,
        dataType:'text',
        data:'',
        success:function(res) {
            if (res.substring(0, 24) == "<!-- AuthcheckFailed -->")
                CRLogout();
            else
                document.getElementById(id).innerHTML = res;
        }
    })
}
function AsyncChangeTextByAJAX(id, address)
{
    $.ajax({
        type:'post', 
        url:address,
        async:true,
        dataType:'text',
        data:'',
        success:function(res) {
            if (res.substring(0, 24) == "<!-- AuthcheckFailed -->")
                CRLogout();
            else
                document.getElementById(id).innerHTML = res;
        }
    })
}
function GetTextByAJAX(address)
{
    var ans;
    $.ajax({
        type:'post', 
        url:address,
        async:false,
        dataType:'text',
        data:'',
        success:function(res) {
            ans = res;
        }
    })
    return ans;
}

// Hex
var HexDecode = function (hex) {
	var arr = hex.split("")
	var out = ""
	for (var i = 0; i < arr.length / 2; i++) {
		var tmp = "0x" + arr[i * 2] + arr[i * 2 + 1]
		var charValue = String.fromCharCode(tmp);
		out += charValue;
	}
return out
};
var HexEncode = function (str) {
	var val = "";
	for (var i = 0; i < str.length; i++) {
		if (val == "")
			val = str.charCodeAt(i).toString(16);
		else
			val += str.charCodeAt(i).toString(16);
	}
	val += "0a"
	return val
}
function checkPasswordComplexity(password, isStrict) {
    var minLength;
    if (isStrict) minLength = 8;
    else minLength = 6;
    const hasLowercase = /[a-z]/.test(password);
    const hasUppercase = /[A-Z]/.test(password);
    const hasNumber = /\d/.test(password);
   
    var isValid;
    if (isStrict) isValid = (password.length >= minLength) && hasLowercase && hasUppercase && hasNumber;
    else isValid = (password.length >= minLength);
   
    return isValid;
}

function getElementPos(id)
{
    const element = document.getElementById(id);
    const rect = element.getBoundingClientRect();
    const scrollLeft = window.pageXOffset || document.documentElement.scrollLeft;
    const scrollTop = window.pageYOffset || document.documentElement.scrollTop;
    const position = {
    top: rect.top + scrollTop,
    left: rect.left + scrollLeft,
    bottom: rect.bottom + scrollTop,
    right: rect.right + scrollLeft,
    width: rect.width || rect.right - rect.left,
    height: rect.height || rect.bottom - rect.top
    };
    return position;
}

function isTouchDevice() {
  return (
    'ontouchstart' in window ||
    navigator.maxTouchPoints > 0
  );
}