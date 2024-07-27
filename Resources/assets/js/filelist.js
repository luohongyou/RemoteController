// filelist.js : Javascript functions for "File Explorer" module.

// Statement:
// Copyright (C) 2020-2024 Coral Studio. All Rights Reserved.

function GetStandardizedAddress(_address)
{
    var _Address = new String(_address);
    _Address.replace('/', '\\');
    _Address.replace('\n', '');

    if (_Address.length < 2 || _Address == "empty")
        return "";

    if (_Address.length == 2 && _Address[1] == ':')
        return _Address + '\\';

    if (_Address[_Address.length - 1] == '\\' && _Address.length != 3)
        _Address = _Address.substring(0, _Address.length - 1);

    if (_Address.length < 3)
        return "";

    return _Address;
}
function GetUpAddress(_address)
{
    var _Address = new String(_address);

    if (_Address.length <= 3 || _Address == "empty")
        return "";

    var cur;
    for (cur = _Address.length - 1; cur >= 0; cur--)
    {
        if (_Address[cur] == '\\')
            break;
    }
    if (cur == 0)
        return "";

    return _Address.substring(0, cur + 1);
}

var Address, _SortBy = 0, _SortDirection = 0, _ShowHide = 0, _CompactMode = 0;

var LastAddress = "";

var PreAddress = [];
var cur = -1, curMax = -1;

var IsOpen = 1;

function DefaultLoadAddress()
{
    document.getElementById("address").value = GetStandardizedAddress(document.getElementById("address").value);
    Address = document.getElementById("address").value;
    if (Address == "")
    {
        Address = "empty";
        document.getElementById('upbutton').disabled = true;
    }
    else document.getElementById('upbutton').disabled = false;

    if (LastAddress != Address)
    {
        PreAddress[++cur] = Address;
        curMax = cur;
        document.getElementById('nextbutton').disabled = true;
    }

    if (cur > 0) document.getElementById('prebutton').disabled = false;
    else document.getElementById('prebutton').disabled = true;
}
function PreLoadAddress()
{
    Address = document.getElementById("address").value = PreAddress[--cur];
    if (Address == "empty") document.getElementById('upbutton').disabled = true, document.getElementById("address").value = "";
    else document.getElementById('upbutton').disabled = false;

    if (cur > 0) document.getElementById('prebutton').disabled = false;
    else document.getElementById('prebutton').disabled = true;
    document.getElementById('nextbutton').disabled = false;
}
function NextLoadAddress()
{
    Address = document.getElementById("address").value = PreAddress[++cur];
    if (Address == "empty") document.getElementById('upbutton').disabled = true, document.getElementById("address").value = "";
    else document.getElementById('upbutton').disabled = false;

    if (cur < curMax) document.getElementById('nextbutton').disabled = false;
    else document.getElementById('nextbutton').disabled = true;
    document.getElementById('prebutton').disabled = false;
}
function ChangeFileListText(id, address)
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
            else if (res.substring(0, 22) == "<!-- ListFileError -->")
            {
                document.getElementById(id).innerHTML = "";
                swal({
                  title: "无法打开文件夹",
                  text: "打开文件夹时出现了错误。",
                  icon: "error",
                  buttons: false,
                  dangerMode: true,
                });
            }
            else
                document.getElementById(id).innerHTML = res;
        }
    })
}
function RefreshFileList()
{
    var _Address = HexEncode(encodeURI(Address));
    ChangeFileListText("filelist", "/explorer/filelist.exe?" + _ShowHide + "+" + _SortBy + "+" + _SortDirection + "+" + _CompactMode + "+" + _Address);

    if (LastAddress != Address)
    {
        LastAddress = Address;
        var scrollableElement = document.getElementById('filelist');
        scrollableElement.scrollTo({
            top: 0
        });
    }
}
function RefreshQuickStartBar()
{
    ChangeTextByAJAX("quickstart", "/explorer/quickstartbar.exe");
}

DefaultLoadAddress();
RefreshFileList();
RefreshQuickStartBar();

function OpenFolder(AddressHex)
{
    if (!IsOpen) 
    {
        IsOpen = 1;
        return;
    }
    
    document.getElementById("address").value = decodeURI(HexDecode(AddressHex));
    DefaultLoadAddress();
    RefreshFileList();
}
function UpFolder()
{
    document.getElementById("address").value = GetUpAddress(Address);
    DefaultLoadAddress();
    RefreshFileList();
}
$('.toolbar').on('submit',function(e){
    e.preventDefault();
    DefaultLoadAddress();
    RefreshFileList();
    RefreshQuickStartBar();
})

const byform = document.getElementById('SortBy');
byform.addEventListener('change', function(event) {
  if (event.target.type === 'radio') {
    _SortBy = event.target.value;
    RefreshFileList();
  }
});
const dirform = document.getElementById('SortDirection');
dirform.addEventListener('change', function(event) {
  if (event.target.type === 'radio') {
    _SortDirection = event.target.value;
    RefreshFileList();
  }
});
document.getElementById('ShowInvisibleItems').addEventListener('change', function() {
    if(this.checked) {
        _ShowHide = 1;
    } else {
        _ShowHide = 0;
    }
    RefreshFileList();
});
document.getElementById('IsCompactMode').addEventListener('change', function() {
    if(this.checked) {
        _CompactMode = 1;
    } else {
        _CompactMode = 0;
    }
    RefreshFileList();
});

var QuickStartBar = 1;
function SwitchQuickStartBar()
{
    if (QuickStartBar) // Disable
    {
        document.getElementById("quickstartframe").setAttribute("class", "col-lg-0");
        document.getElementById('quickstartframe').style.display="none";
        document.getElementById("filelistframe").setAttribute("class", "col-lg-12");
        document.getElementById("qbarbutton").setAttribute("class", "btn btn-explorer");

    }
    else               // Enable
    {
        document.getElementById("quickstartframe").setAttribute("class", "col-lg-2");
        document.getElementById('quickstartframe').style.display="block";
        document.getElementById("filelistframe").setAttribute("class", "col-lg-10");
        document.getElementById("qbarbutton").setAttribute("class", "btn btn-primary");
    }
    QuickStartBar = !QuickStartBar;
}

var table, newRow, CellIcon, CellName, CellAction;
function GetDownloadLink(_Address, _Name, address)
{
    $.ajax({
        type:'get', 
        url:address,
        async:true,
        success:function(FileAddress) {
            
            _Address = decodeURI(HexDecode(_Address));
            FileAddress = decodeURI(HexDecode(FileAddress));

            if (FileAddress.substring(0, 6) == "Failed")
            {
                CellAction.innerHTML = '<i class="bi bi-x-circle-fill">';
                CellAction.setAttribute("class", "text-danger");
                return;
            }

            FileAddress = "/temp/download/" + FileAddress.substring(0, 36) + "/FileDownloading.down";

            var a = document.createElement('a');
            a.href = FileAddress;
            a.download = _Name;
            a.click();
    
            CellAction.innerHTML = '<a href="' + FileAddress + '" download="' + _Name + '"><i class="bi bi-download"></i></a>';

        }
    })
}
function Download(_Address, _Name, _Icon, _IsFolder)
{
    _Name = decodeURI(HexDecode(_Name));
    _Icon = decodeURI(HexDecode(_Icon));

    document.getElementById('downloadnone').style.display = "none";
    table = document.getElementById("downloadlist");
    newRow = table.insertRow(-1); 

    CellIcon = newRow.insertCell(0);
    CellIcon.style.width = "4%";
    CellIcon.innerHTML = '<img src="/explorer/Resources/png/' + _Icon + '" width="18 % "/>';

    CellName = newRow.insertCell(1);
    CellName.innerHTML = _Name;

    CellAction = newRow.insertCell(2);
    CellAction.innerHTML = '<div class="spinner-border spinner-border-sm text-primary" role="status"><span class="visually-hidden">Loading...</span></div>';

    if (_IsFolder)
        _Name += ".zip";

    GetDownloadLink(_Address, _Name, "/explorer/filedownload.exe?" + _Address + "+" + _IsFolder);
}
function DownloadFolder(_Address, _Name, _Icon, _IsFolder)
{
    IsOpen = 0;
    swal({
      title: "打包下载文件夹",
      text: "确实要打包下载整个文件夹吗？这将可能花费较多的时间和空间。",
      icon: "warning",
      buttons: true,
      dangerMode: true,
      buttons: ["取消", "确定"],
    })
    .then((willDownload) => {
      if (willDownload) {
          Download(_Address, _Name, _Icon, _IsFolder);
      }
    });
}