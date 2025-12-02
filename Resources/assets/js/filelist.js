// filelist.js : Javascript functions for "File Explorer" module.

// Statement:
// Copyright (C) 2020-2025 Coral Studio. All Rights Reserved.

function InDev()
{
    notyf.error("该功能还在开发中，即将推出");
}
function GetStandardizedAddress(_address)
{
    var _Address = new String(_address);
    _Address = _Address.replace(/\//g, "\\");
    _Address = _Address.replace(/\n/g, '');

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

var Address, _SortBy = 0, _SortDirection = 0, _ShowHide = 0, _CompactMode = 0, _IsFixed = 1;

var LastAddress = "";

var PreAddress = [];
var cur = -1, curMax = -1;

var IsOpen = 1;

var contextMenuSelect = "";
var IsSelectedFolder = 0;
var _IsItemClicking = 0;
var _IsClickingThreeDots = 0;

var ClipBoard = "";
var ClipBoardStatus = 0; // 0=empty; 1=copy; 2=cut
var IsFolderInClipBoard = 0;
var ClipBoardSelect = "";

const notyf = new Notyf({
    duration: 2000,
    position: {
      x: 'center',
      y: 'bottom',
    }
  });

function EnableToolbar()
{
    if (Address == "empty") return;

    document.getElementById('file_cut').disabled = false;
    document.getElementById('file_copy').disabled = false;
    document.getElementById('file_rename').disabled = false;
    document.getElementById('file_delete').disabled = false;
}
function DisableToolbar()
{
    document.getElementById('file_cut').disabled = true;
    document.getElementById('file_copy').disabled = true;
    document.getElementById('file_rename').disabled = true;
    document.getElementById('file_delete').disabled = true;
}
document.addEventListener('click', function () {
    if (_IsClickingThreeDots)
    {
        _IsClickingThreeDots = 0;
        return 0;
    }

    document.getElementById('filecontextMenu').style.display = 'none';
});
document.getElementById('filelist').addEventListener('click', function () {
    if (_IsItemClicking) {
        _IsItemClicking = 0;
        return;
    }
    clearSelectedItem();
    contextMenuSelect = "";
});
document.getElementById('quickstartframe').addEventListener('click', function () {
    clearSelectedItem();
    contextMenuSelect = "";
});

function clearSelectedItem()
{
    if (contextMenuSelect != "")
        document.getElementById(contextMenuSelect).className = IsSelectedFolder ? "folder_item" : "file_item";
    DisableToolbar();
}
function ContextMenuCheckSpecialFolder()
{
    if (Address == "empty")
    {
        document.getElementById('contextmenudownload').style.display = "none";
        document.getElementById('contextmenuremote').style.display = "block";
        document.getElementById('contextmenucloud').style.display = "none";
        document.getElementById('contextmenucut').style.display = "none";
        document.getElementById('contextmenucopy').style.display = "none";
        document.getElementById('contextmenurename').style.display = "none";
        document.getElementById('contextmenudelete').style.display = "none";
        document.getElementById('contextmenudivider').style.display = "none";
    }
    else if (Address.substring(0, 8) == "共享云盘")
    {
        document.getElementById('contextmenudownload').style.display = "block";
        document.getElementById('contextmenuremote').style.display = "none";
        document.getElementById('contextmenucloud').style.display = "none";
        document.getElementById('contextmenucut').style.display = "block";
        document.getElementById('contextmenucopy').style.display = "block";
        document.getElementById('contextmenurename').style.display = "block";
        document.getElementById('contextmenudelete').style.display = "block";
        document.getElementById('contextmenudivider').style.display = "block";
    }
    else
    {
        document.getElementById('contextmenudownload').style.display = "block";
        document.getElementById('contextmenuremote').style.display = "block";
        document.getElementById('contextmenucloud').style.display = "block";
        document.getElementById('contextmenucut').style.display = "block";
        document.getElementById('contextmenucopy').style.display = "block";
        document.getElementById('contextmenurename').style.display = "block";
        document.getElementById('contextmenudelete').style.display = "block";
        document.getElementById('contextmenudivider').style.display = "block";
    }
}
function ContextMenu(id, IsFolder)
{
    IsOpen = 0;
    _IsItemClicking = 1;
    _IsClickingThreeDots = 1;

    var selectedItem = document.getElementById(id);
    clearSelectedItem();
    contextMenuSelect = id;
    IsSelectedFolder = IsFolder;

    if (IsFolder)
    {
        document.getElementById('contextmenuopen').innerHTML = '<i class="bi bi-folder2-open"></i>&nbsp;打开';
        selectedItem.className = "folder_item table-selected";
    }
    else
    {
        document.getElementById('contextmenuopen').innerHTML = '<i class="bi bi-eye"></i>&nbsp;预览';
        selectedItem.className = "file_item table-selected";
    }
    ContextMenuCheckSpecialFolder();
    EnableToolbar();

    var menu = document.getElementById('filecontextMenu');
    var d3btn = getElementPos(id + "_d");
    menu.style.display = 'block';
    menu.style.left = `${d3btn.right - getElementPos('filecontextMenu').width}px`;
    menu.style.top = `${d3btn.bottom}px`;
}
function SetContextMenus()
{
    const targetDivs = document.getElementsByClassName('file_item');
    const targetDivs2 = document.getElementsByClassName('folder_item');

    for (const div of targetDivs) {
        div.addEventListener('contextmenu', function (e) {
            e.preventDefault();

            clearSelectedItem();
            contextMenuSelect = this.id;
            IsSelectedFolder = 0;
            this.className = "file_item table-selected";

            document.getElementById('contextmenuopen').innerHTML = '<i class="bi bi-eye"></i>&nbsp;预览';
            ContextMenuCheckSpecialFolder();

            EnableToolbar();
            var menu = document.getElementById('filecontextMenu');
            menu.style.display = 'block';
            menu.style.left = `${e.pageX}px`;
            menu.style.top = `${e.pageY}px`;
        });
        if (_CompactMode) {
            div.addEventListener('click', function (e) {
                _IsItemClicking = 1;
                clearSelectedItem();
                contextMenuSelect = this.id;
                IsSelectedFolder = 0;
                this.className = "file_item table-selected";
                EnableToolbar();
            });
        }
    }
    for (const div of targetDivs2) {
        div.addEventListener('contextmenu', function (e) {
            e.preventDefault();

            clearSelectedItem();
            contextMenuSelect = this.id;
            IsSelectedFolder = 1;
            this.className = "folder_item table-selected";

            document.getElementById('contextmenuopen').innerHTML = '<i class="bi bi-folder2-open"></i>&nbsp;打开';
            ContextMenuCheckSpecialFolder();

            EnableToolbar();
            var menu = document.getElementById('filecontextMenu');
            menu.style.display = 'block';
            menu.style.left = `${e.pageX}px`;
            menu.style.top = `${e.pageY}px`;
        });
        if (_CompactMode) {
            div.addEventListener('click', function (e) {
                _IsItemClicking = 1;
                clearSelectedItem();
                contextMenuSelect = this.id;
                IsSelectedFolder = 1;
                this.className = "folder_item table-selected";
                EnableToolbar();
            });
        }
    }
}

function scrollToRightOnFocus()
{
    var input = document.getElementById('address');
    input.scrollLeft = input.scrollWidth;
}

function DefaultLoadAddress()
{
    document.getElementById("address").value = GetStandardizedAddress(document.getElementById("address").value);
    Address = document.getElementById("address").value;
    if (Address == "")
    {
        if (document.getElementById("quickstart") == null) document.getElementById("address").value = Address = "共享云盘";
        else Address = "empty";
        document.getElementById('upbutton').disabled = true;
    }
    else if ((document.getElementById("quickstart") == null) && (Address == "共享云盘"))
        document.getElementById('upbutton').disabled = true;
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
    if (Address == "empty") 
        document.getElementById('upbutton').disabled = true, document.getElementById("address").value = "";
    else if ((document.getElementById("quickstart") == null) && (Address == "共享云盘"))
        document.getElementById('upbutton').disabled = true;
    else document.getElementById('upbutton').disabled = false;

    if (cur > 0) document.getElementById('prebutton').disabled = false;
    else document.getElementById('prebutton').disabled = true;
    document.getElementById('nextbutton').disabled = false;
}
function NextLoadAddress()
{
    Address = document.getElementById("address").value = PreAddress[++cur];
    if (Address == "empty") 
        document.getElementById('upbutton').disabled = true, document.getElementById("address").value = "";
    else if ((document.getElementById("quickstart") == null) && (Address == "共享云盘"))
        document.getElementById('upbutton').disabled = true;
    else document.getElementById('upbutton').disabled = false;

    if (cur < curMax) document.getElementById('nextbutton').disabled = false;
    else document.getElementById('nextbutton').disabled = true;
    document.getElementById('prebutton').disabled = false;
}

function ShowFileListLoading()
{
    document.getElementById('filelist').innerHTML = `
    <br>
    <div align="center">
      <span class="spinner-border spinner-border-sm" aria-hidden="true"></span>
      <span role="status">正在加载...</span>
    </div>
    `;
}
function ChangeFileListText(id, address)
{
    var TimerID = setTimeout("ShowFileListLoading()", 100);
    $.ajax({
        type:'post', 
        url:address,
        async:true,
        dataType:'text',
        data:'',
        success:function(res) {
            clearTimeout(TimerID);

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

            SetContextMenus();
            if (LastAddress != Address)
            {
                LastAddress = Address;
                var scrollableElement = document.getElementById('filelist');
                scrollableElement.scrollTo({
                    top: 0
                });
            }
        }
    })
}
function RefreshFileList()
{
    var _Address = HexEncode(encodeURI(Address));
    ChangeFileListText("filelist", "/explorer/filelist.exe?" + _ShowHide + "+" + _SortBy + "+" + _SortDirection + "+" + _CompactMode + "+" + _IsFixed + "+" + _Address);
    contextMenuSelect = "";
    DisableToolbar();

    if (Address == "empty" || ClipBoardStatus == 0)
        document.getElementById('file_paste').disabled = true;
    else
        document.getElementById('file_paste').disabled = false;

    if (Address != "empty")
        document.getElementById('file_add').disabled = false;
    else
        document.getElementById('file_add').disabled = true;

    ClipBoardSelect = "";
    scrollToRightOnFocus();
}
function RefreshQuickStartBar()
{
    if (document.getElementById("quickstart") != null)
        AsyncChangeTextByAJAX("quickstart", "/explorer/quickstartbar.exe");
}

function OpenFolderByHex(AddressHex)
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
function OpenFolder(Item)
{
    AddressHex = document.getElementById(Item).getAttribute('name');
    OpenFolderByHex(AddressHex);
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

function ChangeSort(_Mode)
{
    if (_Mode == _SortBy)
        _SortDirection = (_SortDirection == "1" ? "0" : "1");
    else if (_Mode == "0") // Name
        _SortBy = _SortDirection = 0;
    else if (_Mode == "1")
        _SortBy = _SortDirection = 1;
    RefreshFileList();
}
document.getElementById('ShowInvisibleItems').addEventListener('change', function() {
    if(this.checked) {
        _ShowHide = 1;
    } else {
        _ShowHide = 0;
    }
    RefreshFileList();
});
document.getElementById('IsTouchMode').addEventListener('change', function() {
    if(this.checked) {
        _CompactMode = 0;
    } else {
        _CompactMode = 1;
    }
    RefreshFileList();
});
document.getElementById('IsFixed').addEventListener('change', function() {
    if(this.checked) {
        _IsFixed = 1;
    } else {
        _IsFixed = 0;
    }
    RefreshFileList();
});

_CompactMode = (isTouchDevice() ? 0 : 1);
document.getElementById('IsTouchMode').checked = isTouchDevice();
document.getElementById('IsFixed').checked = true;
DefaultLoadAddress();
RefreshFileList();
RefreshQuickStartBar();

var QuickStartBar = 1;
function SwitchQuickStartBar()
{
    if (QuickStartBar) // Disable
    {
        document.getElementById("quickstartframe").setAttribute("class", "col-lg-0");
        document.getElementById('quickstartframe').style.display = "none";
        document.getElementById("filelistframe").setAttribute("class", "col-lg-12");
        document.getElementById("qbarbutton").setAttribute("class", "btn btn-explorer");

    }
    else               // Enable
    {
        document.getElementById("quickstartframe").setAttribute("class", "col-lg-2");
        document.getElementById('quickstartframe').style.display = "block";
        document.getElementById("filelistframe").setAttribute("class", "col-lg-10");
        document.getElementById("qbarbutton").setAttribute("class", "btn btn-primary");
    }
    QuickStartBar = !QuickStartBar;
}

var table, newRow, CellIcon, CellName;
function GetDownloadLink(_Address, _Name, address, CellAction)
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
                notyf.error("尝试下载 " + _Name + " 时出现错误");
                return;
            }

            FileAddress = "/temp/download/" + FileAddress.substring(0, 36) + "/FileDownloading.down";

            var a = document.createElement('a');
            a.href = FileAddress;
            a.download = _Name;
            a.click();
    
            CellAction.innerHTML = '<a href="' + FileAddress + '" download="' + _Name + '"><i class="bi bi-download"></i></a>';
            notyf.success("文件 " + _Name + " 已经准备好，现在将开始下载");
        }
    })
}
function Download(_id, _IsFolder)
{
    if (!_IsFolder)
        IsOpen = 0;

    var _Table = document.getElementById("FileTable");
    var _Name = _Table.rows[_id].cells[1].innerHTML;
    var _Icon = document.getElementById("File_" + _id + "_i").getAttribute('src');
    var _Address = document.getElementById("File_" + _id).getAttribute('name');

    document.getElementById('downloadnone').style.display = "none";
    table = document.getElementById("downloadlist");
    newRow = table.insertRow(-1); 

    CellIcon = newRow.insertCell(0);
    CellIcon.style.width = "4%";
    CellIcon.innerHTML = '<img src="' + _Icon + '" width="18 % "/>';

    CellName = newRow.insertCell(1);
    CellName.innerHTML = _Name;

    var CellAction = newRow.insertCell(2);
    CellAction.innerHTML = '<div class="spinner-border spinner-border-sm text-primary" role="status"><span class="visually-hidden">Loading...</span></div>';

    if (_IsFolder)
        _Name += ".zip";

    GetDownloadLink(_Address, _Name, "/explorer/filedownload.exe?" + _Address + "+" + _IsFolder, CellAction);
}
function DownloadFolder(_id, _IsFolder)
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
          Download(_id, _IsFolder);
      }
    });
}
function OpenFile(Item)
{
    if (!IsOpen) 
    {
        IsOpen = 1;
        return;
    }

    AddressHex = document.getElementById(Item).getAttribute('name');

    var _FileName = decodeURI(HexDecode(AddressHex));
    _FileName = _FileName.substring(_FileName.lastIndexOf("\\") + 1);

    document.getElementById('QuicklookContextArea').style.display = "block";
    document.getElementById('QuicklookContextArea').innerHTML = '<div class="spinner-border m-5" role="status"></div>';
    document.getElementById('QuicklookContextAreaIframe').style.display = "none";
    document.getElementById('QuicklookContextAreaIframe').src = "";
    document.getElementById('QuicklookFilenameArea').innerHTML = '<b>' + _FileName + '</b> - 预览';
    $.ajax({
        type:'get', 
        url:"/explorer/quicklook.exe?" + AddressHex,
        async:true,
        success:function(Data) {
            if (Data.substring(0, 18) == "<!-- UseIframe -->")
            {
                document.getElementById('QuicklookContextArea').style.display = "none";
                document.getElementById('QuicklookContextAreaIframe').style.display = "block";
                document.getElementById('QuicklookContextAreaIframe').src = Data.substring(18);
            }
            else
                document.getElementById('QuicklookContextArea').innerHTML = Data;
        }
    });

    $('#modalPreview').modal('show');
}
document.getElementById('modalPreview').addEventListener('hidden.bs.modal', event => {
    document.getElementById('QuicklookContextArea').innerHTML = "";
    document.getElementById('QuicklookContextAreaIframe').src = "";
})

function menu_Open()
{
    if (IsSelectedFolder)
        OpenFolder(contextMenuSelect);
    else
        OpenFile(contextMenuSelect);
}
function menu_Download()
{
    if (IsSelectedFolder)
        DownloadFolder(parseInt(contextMenuSelect.substring(5)), 1);
    else
        Download(parseInt(contextMenuSelect.substring(5)), 0);
}
function menu_CopyToCloud()
{
    var _Table = document.getElementById("FileTable");
    var _Name = _Table.rows[parseInt(contextMenuSelect.substring(5))].cells[1].innerHTML;

    if (IsSelectedFolder)
    {
        swal({
          title: "转存文件夹",
          text: "确实要转存整个文件夹到共享云盘吗？这将可能花费较多的时间和空间。",
          icon: "warning",
          buttons: true,
          dangerMode: true,
          buttons: ["取消", "确定"],
        })
        .then((willSave) => {
          if (willSave) {
            $.ajax({
                type:'post', 
                url:"/explorer/expactionhandler.exe?2+" + document.getElementById(contextMenuSelect).getAttribute('name') + "+2545352538352542312545342542412541422545342542412539312545372539422539380a",
                async:true,
                dataType:'text',
                data:'',
                success:function(res) {
                    if (res != "Yes")
                    {
                        notyf.error("转存 " + _Name + " 到云盘时出现错误");
                    }
                    else
                    {
                        notyf.success("成功转存 " + _Name + " 到共享云盘");
                    }
                }
            });
          }
        });
        return;
    }
    $.ajax({
        type:'post', 
        url:"/explorer/expactionhandler.exe?2+" + document.getElementById(contextMenuSelect).getAttribute('name') + "+2545352538352542312545342542412541422545342542412539312545372539422539380a",
        async:true,
        dataType:'text',
        data:'',
        success:function(res) {
            if (res != "Yes")
            {
                notyf.error("转存 " + _Name + " 到云盘时出现错误");
            }
            else
            {
                notyf.success("成功转存 " + _Name + " 到共享云盘");
            }
        }
    })
}
function menu_RemoteOpen()
{
    var AddressHex = document.getElementById(contextMenuSelect).getAttribute('name');
    var _FileName = decodeURI(HexDecode(AddressHex));
    _FileName = _FileName.substring(_FileName.lastIndexOf("\\") + 1);

    swal({
      title: "远程打开文件",
      text: "确实要在远程计算机上打开“" + _FileName + "”吗？",
      icon: "warning",
      buttons: true,
      dangerMode: true,
      buttons: ["取消", "确定"],
    })
    .then((willOpen) => {
      if (willOpen) {
        $.ajax({
            type:'post', 
            url:"/explorer/expactionhandler.exe?1+" + document.getElementById(contextMenuSelect).getAttribute('name') + "+0",
            async:true,
            dataType:'text',
            data:'',
            success:function(res) {
                if (res != "Yes")
                {
                    notyf.error("请求在远程计算机上打开 " + _FileName + " 失败");
                }
                else
                {
                    notyf.success("已请求在远程计算机上打开 " + _FileName);
                }
            }
        })
      }
    });
}
function menu_NewFolder()
{
    document.getElementById("FolderAddressArea").innerHTML = "在 " + Address + " 中新建文件夹：";
    document.getElementById("FolderName").value = "";

    $('#modalNewFolder').modal('show');
}
function flushClipboard()
{
    ClipBoard = "";
    IsFolderInClipBoard = 0;
    ClipBoardStatus = 0;

    document.getElementById("clipboarddata").align = "center";
    document.getElementById("clipboarddata").innerHTML = "文件剪贴板为空";

    document.getElementById('file_paste').disabled = true;

    if (ClipBoardSelect != "")
        document.getElementById(ClipBoardSelect).style.opacity = "1";
    ClipBoardSelect = "";
}
function File_Cut()
{
    flushClipboard();

    ClipBoard = document.getElementById(contextMenuSelect).getAttribute('name');
    IsFolderInClipBoard = IsSelectedFolder;
    ClipBoardStatus = 2;
    
    document.getElementById("clipboarddata").align = "left";
    document.getElementById("clipboarddata").innerHTML = decodeURI(HexDecode(ClipBoard));

    document.getElementById('file_paste').disabled = false;

    ClipBoardSelect = contextMenuSelect;
    document.getElementById(ClipBoardSelect).style.opacity = "0.5";
}
function File_Copy()
{
    flushClipboard();

    ClipBoard = document.getElementById(contextMenuSelect).getAttribute('name');
    IsFolderInClipBoard = IsSelectedFolder;
    ClipBoardStatus = 1;
    
    document.getElementById("clipboarddata").align = "left";
    document.getElementById("clipboarddata").innerHTML = decodeURI(HexDecode(ClipBoard));

    document.getElementById('file_paste').disabled = false;
}
function File_Paste()
{
    var _ClipBoardStatus = ClipBoardStatus;

    var AddressHex = HexEncode(encodeURI(Address));
    var _FileName = decodeURI(HexDecode(ClipBoard));
    _FileName = _FileName.substring(_FileName.lastIndexOf("\\") + 1);

    $.ajax({
        type:'post', 
        url:(_ClipBoardStatus == 2 ? "/explorer/expactionhandler.exe?7+" : "/explorer/expactionhandler.exe?6+") + AddressHex + "+" + ClipBoard,
        async:true,
        dataType:'text',
        data:'',
        success:function(res) {
            if (res != "Yes")
            {
                if (_ClipBoardStatus == 2)
                    notyf.error("移动 " + _FileName + " 时出现错误");
                else
                    notyf.error("复制 " + _FileName + " 时出现错误");
            }
            else
            {
                if (_ClipBoardStatus == 2)
                    notyf.success("成功移动了 " + _FileName);
                else
                    notyf.success("成功复制了 " + _FileName);

            }
            RefreshFileList();
        }
    });

    if (_ClipBoardStatus == 2)
        flushClipboard();
}
function File_Rename()
{
    var AddressHex = document.getElementById(contextMenuSelect).getAttribute('name');
    var _FileName = decodeURI(HexDecode(AddressHex));
    document.getElementById("RenameOldFilenameArea").innerHTML = "将 " + _FileName + " 的名称修改为：";
    _FileName = _FileName.substring(_FileName.lastIndexOf("\\") + 1);
    document.getElementById("NewFileName").value = _FileName;

    $('#modalRename').modal('show');
}
function CheckFileName(str) {
    const regex = /[\n\\/:*?"<>|]/;
    return regex.test(str);
}
$('.RenameArea').on('submit',function(e){
    e.preventDefault();

    var NewFileName = document.getElementById("NewFileName").value;

    if (NewFileName == "" || CheckFileName(NewFileName))
    {
        swal({
          title: "新的名称无效",
          text: '名称不能为空，且不能包含下列字符：\n\\ / : * ? " < > |',
          icon: "error",
          buttons: false,
          dangerMode: true,
        })
        .then(() => {
            $('#modalRename').modal('show');
        });
        return;
    }

    var AddressHex = document.getElementById(contextMenuSelect).getAttribute('name');
    $.ajax({
        type:'post', 
        url:"/explorer/expactionhandler.exe?5+" + AddressHex + "+" + HexEncode(encodeURI(NewFileName)),
        async:true,
        dataType:'text',
        data:'',
        success:function(res) {
            if (res != "Yes")
            {
                swal({
                  title: "重命名失败",
                  text: '请检查是否已经有重名的项目',
                  icon: "error",
                  buttons: false,
                  dangerMode: true,
                })
                .then(() => {
                    $('#modalRename').modal('show');
                });
            }
            else
            {
                notyf.success("成功更改了新名称为 " + NewFileName);
                RefreshFileList();
            }
        }
    });
});
$('.NewFolderArea').on('submit',function(e){
    e.preventDefault();

    var FolderName = document.getElementById("FolderName").value;

    if (FolderName == "" || CheckFileName(FolderName))
    {
        swal({
          title: "文件夹名无效",
          text: '名称不能为空，且不能包含下列字符：\n\\ / : * ? " < > |',
          icon: "error",
          buttons: false,
          dangerMode: true,
        })
        .then(() => {
            $('#modalNewFolder').modal('show');
        });
        return;
    }

    var NewFolderAddress = Address;
    if (Address.substr(-1) != '\\')
        NewFolderAddress += '\\';
    NewFolderAddress += FolderName;
    NewFolderAddress = GetStandardizedAddress(NewFolderAddress);
    NewFolderAddress = HexEncode(encodeURI(NewFolderAddress));

    $.ajax({
        type:'post', 
        url:"/explorer/expactionhandler.exe?4+" + NewFolderAddress + "+0",
        async:true,
        dataType:'text',
        data:'',
        success:function(res) {
            if (res != "Yes")
            {
                swal({
                  title: "新建文件夹失败",
                  text: '请检查是否已经有重名的项目',
                  icon: "error",
                  buttons: false,
                  dangerMode: true,
                })
                .then(() => {
                    $('#modalNewFolder').modal('show');
                });
            }
            else
            {
                notyf.success("成功新建了文件夹 " + FolderName);
                RefreshFileList();
            }
        }
    });
});
function File_Delete()
{
    var AddressHex = document.getElementById(contextMenuSelect).getAttribute('name');
    var _FileName = decodeURI(HexDecode(AddressHex));
    _FileName = _FileName.substring(_FileName.lastIndexOf("\\") + 1);

    swal({
      title: IsSelectedFolder ? "删除文件夹" : "删除文件",
      text: "确实要删除“" + _FileName + "”吗？此操作不可逆！",
      icon: "warning",
      buttons: true,
      dangerMode: true,
      buttons: ["取消", "确定"],
    })
    .then((willDel) => {
      if (willDel) {
        $.ajax({
            type:'post', 
            url:"/explorer/expactionhandler.exe?3+" + AddressHex + "+0",
            async:true,
            dataType:'text',
            data:'',
            success:function(res) {
                if (res != "Yes")
                {
                    notyf.error("删除 " + _FileName + " 时出现错误");
                }
                else
                {
                    notyf.success("成功删除了 " + _FileName);
                }
                RefreshFileList();
            }
        });
      }
    });
}
function explorerUpload(e) {
    e.preventDefault();

    var file = document.getElementById('expformFile').files[0];

    if (file.size / 1024 / 1024 / 1024 > 4)
    {
        swal({
          title: "文件大小过大",
          text: "不支持上传大小超过4GB的文件",
          icon: "error",
          buttons: false,
          dangerMode: false,
        });
        return;
    }

    document.getElementById('fileProgress').style.display = "block";
    document.getElementById('expUploadFormArea').style.display = "none";

    var formData = new FormData();
    formData.append("file", file);

    $.ajax({
        url: '/explorer/expuploadhandler.exe?' + HexEncode(encodeURI(Address)),
        type: 'POST',
        cache: false,
        data: formData,
        contentType: false,
        processData: false,
        xhr: function() {
            var xhr = $.ajaxSettings.xhr();
            if (xhr.upload) {
                xhr.upload.addEventListener('progress', function(event) {
                    var percentComplete = Math.floor((event.loaded / event.total) * 100);
                    $('#uploadprogressbar').width(percentComplete + '%').text(percentComplete + '%');
                }, false);
            }
            return xhr;
        },
        success: function (response) {
            document.getElementById('fileProgress').style.display = "none";
            document.getElementById('expUploadFormArea').style.display = "block";
            $('#uploadprogressbar').width('0%').text('0%');

            if (response == "Yes") {
                document.getElementById('expformFile').value = "";
                RefreshFileList();
                swal({
                  title: "上传文件成功",
                  text: "成功将文件上传到了指定文件夹",
                  icon: "success",
                  buttons: false,
                  dangerMode: false,
                });
            }
            else {
                swal({
                  title: "上传文件错误",
                  text: "上传文件时出现错误，文件没有上传",
                  icon: "error",
                  buttons: false,
                  dangerMode: false,
                });
            }

        },
        error: function (xhr, status, error) {
            document.getElementById('fileProgress').style.display = "none";
            document.getElementById('expUploadFormArea').style.display = "block";
            $('#uploadprogressbar').width('0%').text('0%');

            console.log('Error uploading file:', error);
            swal({
              title: "上传文件错误",
              text: "上传文件时出现错误，文件没有上传",
              icon: "error",
              buttons: false,
              dangerMode: false,
            });
        }
    });
}
