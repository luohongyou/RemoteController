// index_explorer.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。

// 版权声明：
// Copyright (C) 2020-2025 Coral Studio. All Rights Reserved.

#define ADDRESS "/explorer/"

#include <iostream>
#include "../../../HTMLFrame/HTMLFrame.h"
#pragma comment(lib, "../../../x64/Release/HTMLFrame.lib")

int main(int argc, char* argv[])
{
    SetAppDirectory("\\..\\..\\");

    HTMLFrame HTML;
    HTML.Register(PT_INDEX, ADDRESS, "文件管理", "文件管理", R"(<li><a href="/">主页</a></li><li>文件管理</li>)", true);

    std::string Code = (std::string)R"(
<div class="portfolio-info">
    <form class="toolbar">
    <div class="btn-group btn-group-sm" role="group" style="width:100%;">)" + (HTML.User.IsCloudStorageUser() ? "" : R"(
      <button type="button" class="btn btn-primary" onclick="SwitchQuickStartBar() " id="qbarbutton" data-bs-toggle="tooltip" data-bs-placement="bottom" data-bs-title="显示/隐藏导航窗格"><i class="bi bi-list-ul"></i></button>)") + R"(
      <button type="button" class="btn btn-explorer" id="prebutton" onclick="PreLoadAddress(); RefreshFileList();" data-bs-toggle="tooltip" data-bs-placement="bottom" data-bs-title="返回上一个文件夹"><i class="bi bi-arrow-left"></i></button>
      <button type="button" class="btn btn-explorer" id="nextbutton" onclick="NextLoadAddress(); RefreshFileList();" data-bs-toggle="tooltip" data-bs-placement="bottom" data-bs-title="前进到下一个文件夹"><i class="bi bi-arrow-right"></i></button>
      <button type="button" class="btn btn-explorer" id="upbutton" onclick="UpFolder() " data-bs-toggle="tooltip" data-bs-placement="bottom" data-bs-title="上移到上一级文件夹"><i class="bi bi-arrow-up"></i></button>
      <button type="submit" class="btn btn-explorer" id="refreshbutton" data-bs-toggle="tooltip" data-bs-placement="bottom" data-bs-title="刷新当前文件夹"><i class="bi bi-arrow-clockwise"></i></button>
      <input class="form-control form-control-sm" type="text" placeholder="输入要访问的文件夹" id="address" autocomplete="off" style="z-index:500" )" + (!HTML.User.IsCloudStorageUser() ? "" : R"(value="共享云盘")") + R"(>

      <button type="button" class="btn btn-explorer dropdown-toggle" data-bs-toggle="dropdown" data-bs-auto-close="outside"><i class="bi bi-check-all"></i></button>
      <div class="dropdown-menu gap-1 p-2 rounded-3 mx-0 shadow w-220px dropdown-menu-end">
        <div class="px-2 py-2">
          
          <ul class="nav nav-underline">
            <li class="nav-item">
              <a class="nav-link active" data-bs-toggle="tab" data-bs-target="#download-pane" role="tab">下载列表</a>
            </li>
            <li class="nav-item">
              <a class="nav-link" data-bs-toggle="tab" data-bs-target="#clipboard-pane" role="tab">剪贴板</a>
            </li>
          </ul>
          <br>

          <div class="tab-content">

            <div class="tab-pane fade show active" id="download-pane" role="tabpanel" tabindex="0">
              <div id="downloadnone" align="center" style="font-size:14px;">暂无正在进行的文件下载</div>
              <table id="downloadlist" class="table table-hover table-borderless" style="font-size:14px;"></table>
            </div>

            <div class="tab-pane fade" id="clipboard-pane" role="tabpanel" tabindex="1">
              <div id="clipboarddata" align="center" style="font-size:14px;">文件剪贴板为空</div>
              <br>
            </div>

          </div>

        </div>
      </div>


      <button type="button" class="btn btn-explorer dropdown-toggle" data-bs-toggle="dropdown" data-bs-auto-close="outside"><i class="bi bi-gear-fill"></i></button>
      <div class="dropdown-menu gap-1 p-2 rounded-3 mx-0 shadow w-220px dropdown-menu-end">
        <div class="px-4 py-3">
          <div class="form-check form-switch">
            <input class="form-check-input" type="checkbox" role="switch" id="ShowInvisibleItems">
            <label class="form-check-label" for="ShowInvisibleItems">显示隐藏的项目</label>
          </div>

          <div class="dropdown-divider"></div>
          <div class="form-check form-switch">
            <input class="form-check-input" type="checkbox" role="switch" id="IsTouchMode">
            <label class="form-check-label" for="IsTouchMode">触控模式</label>
          </div>
          <div class="form-check form-switch">
            <input class="form-check-input" type="checkbox" role="switch" id="IsFixed">
            <label class="form-check-label" for="IsFixed">省略过长的内容</label>
          </div>
        </div>
      </div>

    </div>
    </form>

)" + (HTML.User.IsCloudStorageUser() ? R"(<div id="quickstartframe"></div>)" : R"(
    <div class="row gy-2">
      <div id="quickstartframe" class="col-lg-2">
        <div id="quickstart" style="overflow:auto;"></div>
      </div>)") + R"(
      <div id="filelistframe")" + (HTML.User.IsCloudStorageUser() ? "" : R"( class="col-lg-10")") + R"(>

        <div class="bg-auto" style="width:100%;">
        <div class="btn-group btn-group-sm" role="group" )" + (HTML.User.GetUserType() == UT_GUEST ? "style='display:none'" : "") + R"(>
          <button type="button" class="btn btn-explorer dropdown-toggle" id="file_add" data-bs-toggle="dropdown" data-bs-auto-close="outside" disabled><i class="bi bi-plus-circle"></i>&nbsp;新建/上传</button>
          <div class="dropdown-menu gap-1 p-2 rounded-3 mx-0 shadow w-220px">
            <li><a class="dropdown-item rounded-2" href="javascript:void(0) " onclick="menu_NewFolder() "><i class="bi bi-folder2-open"></i>&nbsp;新建文件夹</a></li>
            <li><hr class="dropdown-divider"></li>
            <div class="px-2 py-2">
              <b>上传文件</b>
              <form onsubmit="explorerUpload(event) " name="expUploadForm" enctype="multipart/form-data" id="expUploadFormArea">
                <div class="mb-3">
                  <label for="expformFile" class="form-label">选择要上传的文件</label>
                  <input class="form-control form-control-sm" id="expformFile" type="file">
                </div>
                <div class="d-grid gap-2 d-md-flex">
                  <button type="submit" class="btn btn-primary btn-sm" >上传</button>
                </div>
              </form>
              <div class="progress" role="progressbar" id="fileProgress" style="display:none">
                <div class="progress-bar" style="width: 0%" id="uploadprogressbar" ></div>
              </div>
            </div>
          </div>
          <button type="button" class="btn btn-explorer" id="file_cut" onclick="File_Cut();" disabled><i class="bi bi-scissors"></i>&nbsp;剪切</button>
          <button type="button" class="btn btn-explorer" id="file_copy" onclick="File_Copy();" disabled><i class="bi bi-files"></i>&nbsp;复制</button>
          <button type="button" class="btn btn-explorer" id="file_paste" onclick="File_Paste();" disabled><i class="bi bi-clipboard"></i>&nbsp;粘贴</button>
          <button type="button" class="btn btn-explorer" id="file_rename" onclick="File_Rename();" disabled><i class="bi bi-input-cursor-text"></i>&nbsp;重命名</button>
          <button type="button" class="btn btn-explorer" id="file_delete" onclick="File_Delete();" disabled><i class="bi bi-trash"></i>&nbsp;删除</button>
        </div>
        </div>

        <div id="filelist" style="overflow:auto; height:500px"></div>
      </div>
)" + (HTML.User.IsCloudStorageUser() ? "" : R"(    </div>)") + R"(
</div>

<ul id="filecontextMenu" class="dropdown-menu gap-1 p-2 rounded-3 mx-0 shadow w-220px user-select-none" style="display:none; position:absolute;">
  <li><a class="dropdown-item rounded-2" href="javascript:void(0) " onclick="menu_Open() " id="contextmenuopen"><i class="bi bi-eye"></i>&nbsp;预览</a></li>
  <li><a class="dropdown-item rounded-2" href="javascript:void(0) " onclick="menu_Download() " id="contextmenudownload"><i class="bi bi-download"></i>&nbsp;下载</a></li>
  <li><a class="dropdown-item rounded-2 )" + (HTML.User.GetUserType() == UT_GUEST ? "disabled" : "") + R"(" href="javascript:void(0) " onclick="menu_CopyToCloud();" id="contextmenucloud"><i class="bi bi-cloud-plus"></i>&nbsp;转存到共享云盘</a></li>
  <li><a class="dropdown-item rounded-2 )" + (HTML.User.GetUserType() == UT_GUEST ? "disabled" : "") + R"(" href="javascript:void(0) " onclick="menu_RemoteOpen();" id="contextmenuremote"><i class="bi bi-window-plus"></i>&nbsp;在远程计算机上打开</a></li>
  <li id="contextmenudivider"><hr class="dropdown-divider"></li>
  <li><a class="dropdown-item rounded-2 )" + (HTML.User.GetUserType() == UT_GUEST ? "disabled" : "") + R"(" href="javascript:void(0) " onclick="File_Cut();" id="contextmenucut"><i class="bi bi-scissors"></i>&nbsp;剪切</a></li>
  <li><a class="dropdown-item rounded-2 )" + (HTML.User.GetUserType() == UT_GUEST ? "disabled" : "") + R"(" href="javascript:void(0) " onclick="File_Copy();" id="contextmenucopy"><i class="bi bi-files"></i>&nbsp;复制</a></li>
  <li><a class="dropdown-item rounded-2 )" + (HTML.User.GetUserType() == UT_GUEST ? "disabled" : "") + R"(" href="javascript:void(0) " onclick="File_Rename();" id="contextmenurename"><i class="bi bi-input-cursor-text"></i>&nbsp;重命名</a></li>
  <li><a class="dropdown-item rounded-2 )" + (HTML.User.GetUserType() == UT_GUEST ? "disabled" : "") + R"(" href="javascript:void(0) " onclick="File_Delete();" id="contextmenudelete"><i class="bi bi-trash"></i>&nbsp;删除</a></li>
</ul>

<div class="modal fade" tabindex="-1" data-bs-backdrop="static" role="dialog" id="modalPreview">
  <div class="modal-dialog modal-lg modal-dialog-centered modal-dialog-scrollable">

      <div class="modal-content rounded-4 shadow">
        <div class="modal-header p-5 pb-4 border-bottom-0">
          <h1 class="mb-0 fs-5" id="QuicklookFilenameArea">预览</h1>
          <button type="button" class="btn-close" data-bs-dismiss="modal" aria-label="Close"></button>
        </div>

        <div class="modal-body p-5 pt-0" align="center">
          <div id="QuicklookContextArea"></div>
          <iframe src="" width="100%" style="height:400px; border:none; display:none;" id="QuicklookContextAreaIframe"></iframe>
        </div>
      </div>
    </div>

  </div>
</div>

<div class="modal fade" tabindex="-1" data-bs-backdrop="static" role="dialog" id="modalRename">
  <div class="modal-dialog modal-dialog-centered modal-dialog-scrollable">

      <div class="modal-content rounded-4 shadow">
        <div class="modal-header p-5 pb-4 border-bottom-0">
          <h1 class="mb-0 fs-4">重命名</h1>
          <button type="button" class="btn-close" data-bs-dismiss="modal" aria-label="Close"></button>
        </div>

        <div class="modal-body p-5 pt-0" align="center">
          <p align="left" style="word-break: break-all;"><small id="RenameOldFilenameArea"></small></p>
          <form class="RenameArea">
            <div class="form-floating mb-3">
              <input class="form-control rounded-3" id="NewFileName" autocomplete="off">
              <label for="NewFileName">新的名称</label>
            </div>
            <button class="w-100 mb-2 btn btn-lg rounded-3 btn-primary" type="submit" data-bs-dismiss="modal">确认</button>
          </form>
        </div>
      </div>
    </div>

  </div>
</div>

<div class="modal fade" tabindex="-1" data-bs-backdrop="static" role="dialog" id="modalNewFolder">
  <div class="modal-dialog modal-dialog-centered modal-dialog-scrollable">

      <div class="modal-content rounded-4 shadow">
        <div class="modal-header p-5 pb-4 border-bottom-0">
          <h1 class="mb-0 fs-4">新建文件夹</h1>
          <button type="button" class="btn-close" data-bs-dismiss="modal" aria-label="Close"></button>
        </div>

        <div class="modal-body p-5 pt-0" align="center">
          <p align="left" style="word-break: break-all;"><small id="FolderAddressArea"></small></p>
          <form class="NewFolderArea">
            <div class="form-floating mb-3">
              <input class="form-control rounded-3" id="FolderName" autocomplete="off">
              <label for="FolderName">文件夹名称</label>
            </div>
            <button class="w-100 mb-2 btn btn-lg rounded-3 btn-primary" type="submit" data-bs-dismiss="modal">确认</button>
          </form>
        </div>
      </div>
    </div>

  </div>
</div>

<script src="/assets/js/filelist.js?version=)" + (std::string)PROGRAM_BUILD_S + R"("></script>
)";

    HTML.Custom(Code);
    
    HTML.Output();
    return 0;
}
