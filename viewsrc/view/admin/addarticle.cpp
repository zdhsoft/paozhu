#include<iostream>
#include <cstdio>
#include <string>
#include <sstream>
#include <map> 
#include <vector>
#include <ctime>
#include <array>
#include <sys/time.h>
#include <sys/stat.h>
#include <unistd.h>
#include <cstdlib>
#include "request.h"
#include "datetime.h"
#include "cookie.h"
#include "urlcode.h"
#include "loadviewso.h"
#include "viewso_param.h"
#include "http_so_common_api.h"
#include "viewsrc.h"
//g++ viewsrc/view/admin/addarticle.cpp
namespace http {

namespace view {
	namespace admin{
 		 std::string addarticle(const struct view_param &vinfo,http::OBJ_VALUE &obj)
			{
 
                     std::ostringstream echo;

        
 			 echo<<"<!doctype html>\n<html lang=\"en\" data-bs-theme=\"auto\">\n\n<head>\n  <meta charset=\"utf-8\">\n  <meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">\n  <meta name=\"description\" content=\"\">\n  <title>add article</title>\n\n  <link href=\"/assets/dist/css/bootstrap.min.css\" rel=\"stylesheet\">\n  <link rel=\"stylesheet\" href=\"/assets/icons/font/bootstrap-icons.css\">\n  <link rel=\"stylesheet\" href=\"/libs/tree/select-tree.css\">\n \n  <link href=\"/css/dashboard.css\" rel=\"stylesheet\">\n  <script src=\"/js/jquery.min.js\"></script>\n  <script src=\"/libs/tree/select-tree.js\"></script>\n  \n<!--  <script type=\"text/javascript\" charset=\"utf-8\" src=\"/ueditor/ueditor.config.js\"></script>\n  <script type=\"text/javascript\" charset=\"utf-8\" src=\"/ueditor/ueditor.all.js\"> </script>\n  <script type=\"text/javascript\" charset=\"utf-8\" src=\"/ueditor/lang/zh-cn/zh-cn.js\"></script>-->\n  \n  <style type=\"text/css\">\n   \n\n    .edui-editor {\n      width: 830px !important;\n      min-height: 300px;\n      height: auto;\n    }\n\n    #edui1_iframeholder {\n      min-height: 200px;\n    }\n\n    body {\n      background-color: #FFFFFF;\n    }\n  </style>\n</head>\n\n<body>\n\n  <div class=\"container-fluid\">\n\n    <h5 class=\"card-title mt-2\">添加文章</h5>\n    <hr>\n    <div class=\"row\">\n      <div class=\"col-12\">\n        <form id=\"form\" action=\"addarticlepost\" method=\"post\">\n\n          <div class=\"row mb-3\">\n            <label for=\"topicid\" class=\"col-sm-2 col-form-label text-sm-end\">选择栏目</label>\n            <div class=\"col-sm-10\">\n                   <select class=\"form-select\" id=\"topicid\" name=\"topicid\"></select>\n            </div>\n          </div>\n          <div class=\"row mb-3\">\n            <label for=\"title\" class=\"col-sm-2 col-form-label text-sm-end\">标题</label>\n            <div class=\"col-sm-10\">\n              <input type=\"text\" class=\"form-control\" id=\"title\" name=\"title\" value=\"\">\n            </div>\n          </div>\n          <div class=\"row mb-3\">\n            <label for=\"author\" class=\"col-sm-2 col-form-label text-sm-end\">作者</label>\n            <div class=\"col-sm-10\">\n              <input type=\"text\" class=\"form-control\" id=\"author\" name=\"author\" value=\"\">\n            </div>\n          </div>\n          <div class=\"row mb-3\">\n            <label for=\"fromsource\" class=\"col-sm-2 col-form-label text-sm-end\">来源</label>\n            <div class=\"col-sm-10\">\n              <input type=\"text\" class=\"form-control\" id=\"fromsource\" name=\"fromsource\" value=\"\">\n            </div>\n          </div> \n         <div class=\"row mb-3\">\n            <label for=\"icoimg\" class=\"col-sm-2 col-form-label text-sm-end\">文章头图</label>\n            <div class=\"col-sm-6\">\n              <div class=\"input-group mb-3\">\n              <input type=\"text\" class=\"form-control\" id=\"icoimg\" name=\"icoimg\" value=\"\" placeholder=\"列表使用或头图使用\">\n              <input class=\"form-control\" type=\"file\" style=\"width:100px\" id=\"relate-icoimg\" accept=\".png,.jpg,.jpeg\">\n              </div>\n            </div>\n            <div class=\"col-sm-4\">\n              <div class=\"progress\" id=\"progress-bar-show\" style=\"display:none\" role=\"progressbar\" aria-valuenow=\"0\" aria-valuemin=\"0\" aria-valuemax=\"100\">\n                  <div class=\"progress-bar\" id=\"progress-bar\" style=\"width: 0%\">0</div>\n                </div>\n              <div id=\"progress-bar-result\"></div>  \n            </div>\n          </div> \n          <div class=\"row mb-3\">\n            <label for=\"keywords\" class=\"col-sm-2 col-form-label text-sm-end\">关键词</label>\n            <div class=\"col-sm-10\">\n              <textarea class=\"form-control\" id=\"keywords\" rows=\"3\" name=\"keywords\"></textarea>\n            </div>\n          </div>\n \n          <div class=\"row mb-3\">\n            <label for=\"summary\" class=\"col-sm-2 col-form-label text-sm-end\">文章摘要</label>\n            <div class=\"col-sm-10\">\n              <textarea class=\"form-control\" id=\"summary\" rows=\"3\" name=\"summary\"\n                placeholder=\"250字以内\"></textarea>\n            </div>\n          </div>\n          <div class=\"row mb-3\">\n            <label for=\"content\" class=\"col-sm-2 col-form-label text-sm-end\">文章内容</label>\n            <div class=\"col-sm-10\">\n              <textarea id=\"editor\" style=\"width:830px\" rows=\"10\" name=\"content\"></textarea>\n            </div>\n          </div>\n\n          <div class=\"row mb-3\">\n            <label for=\"relatecontent\" class=\"col-sm-2 col-form-label text-sm-end\">相关内容</label>\n            <div class=\"col-sm-7\">\n            <div class=\"input-group mb-3\">\n  <input type=\"text\" class=\"form-control\" id=\"relatecontent\" name=\"relatecontent\" placeholder=\"选择相关内容\" aria-label=\"相关内容\" aria-describedby=\"relate-addon\">\n  <button class=\"btn btn-outline-secondary\" type=\"button\" id=\"relate-addon\" data-bs-toggle=\"modal\" data-bs-target=\"#exampleModal\">选择</button>\n</div>\n\n\n            </div>\n          </div>\n\n          <div class=\"row mb-3\">\n            <div class=\"col-sm-10 offset-sm-2\">\n              <button type=\"submit\" class=\"btn btn-primary\">确认</button>\n            </div>\n          </div>\n\n        </form>\n      </div>\n    </div>\n\n  </div>\n\n\n<div class=\"modal modal-lg fade\" id=\"exampleModal\" tabindex=\"-1\" aria-labelledby=\"exampleModalLabel\" aria-hidden=\"true\">\n  <div class=\"modal-dialog\">\n    <div class=\"modal-content\">\n      <div class=\"modal-header\">\n        <h1 class=\"modal-title fs-5\" id=\"exampleModalLabel\">相关内容</h1>\n        <button type=\"button\" class=\"btn-close\" data-bs-dismiss=\"modal\" aria-label=\"Close\"></button>\n      </div>\n      <div class=\"modal-body\">\n      <table class=\"table table-hover\" id=\"selecttable\">\n      <thead>\n        <tr>\n          <th scope=\"col\">#</th>\n          <th scope=\"col\">标题</th>\n          <th scope=\"col\">选择</th>\n        </tr>\n      </thead>\n      <tbody> \n      </tbody>\n\n      </table>\n      \n      <ul class=\"pagination\">\n        <li class=\"page-item\" id=\"select_pre\">\n          <a class=\"page-link\" href=\"#\" onclick=\"return select_prepage()\">前一页</a>\n        </li>\n        <li class=\"page-item\" id=\"select_next\">\n          <a class=\"page-link\" href=\"#\" onclick=\"return select_nextpage()\">下一页</a>\n        </li>\n      </ul>\n\n      </div>\n      <div class=\"modal-footer\">\n        <button type=\"button\" class=\"btn btn-secondary\" data-bs-dismiss=\"modal\">关闭</button>\n         \n      </div>\n    </div>\n  </div>\n</div>\n\n<div class=\"modal fade\" id=\"myfiletobigModal\" data-bs-backdrop=\"static\" data-bs-keyboard=\"false\" tabindex=\"-1\" aria-labelledby=\"staticBackdropLabel\" aria-hidden=\"true\">\n  <div class=\"modal-dialog\">\n    <div class=\"modal-content\">\n      <div class=\"modal-header\">\n        <h1 class=\"modal-title fs-5\" id=\"staticBackdropLabel\">提示</h1>\n        <button type=\"button\" class=\"btn-close\" data-bs-dismiss=\"modal\" aria-label=\"Close\"></button>\n      </div>\n      <div class=\"modal-body\">\n         文件太大了，请传4M以下的文件！\n      </div>\n      <div class=\"modal-footer\">\n        <button type=\"button\" class=\"btn btn-secondary\" data-bs-dismiss=\"modal\">关闭</button>\n         \n      </div>\n    </div>\n  </div>\n</div>\n\n<div class=\"modal fade\" id=\"myfilesuccessModal\" data-bs-backdrop=\"static\" data-bs-keyboard=\"false\" tabindex=\"-1\" aria-labelledby=\"myfilesuccess\" aria-hidden=\"true\">\n  <div class=\"modal-dialog\">\n    <div class=\"modal-content\">\n      <div class=\"modal-header\">\n        <h1 class=\"modal-title fs-5\" id=\"myfilesuccess\">提示</h1>\n        <button type=\"button\" class=\"btn-close\" data-bs-dismiss=\"modal\" aria-label=\"Close\"></button>\n      </div>\n      <div class=\"modal-body\">\n         上传成功！\n      </div>\n      <div class=\"modal-footer\">\n        <button type=\"button\" class=\"btn btn-secondary\" data-bs-dismiss=\"modal\">确定</button>\n         \n      </div>\n    </div>\n  </div>\n</div>\n\n \n  <script src=\"/assets/dist/js/bootstrap.bundle.min.js\"></script>\n  <script src=\"/nicedit/nicEdit.js\" type=\"text/javascript\"></script>\n  <script lang=\"javascript\">\n    //var ue = UE.getEditor('editor');\n    \n   bkLib.onDomLoaded(function() {\n\t  new nicEditor({fullPanel : true,iconsPath : '/nicedit/nicEditorIcons.gif'}).panelInstance('editor');\n    });\n    \n    var current_page=";
echo<<obj["pageinfo"]["current"].to_int(); 
 			 echo<<";\n    var min_page=";
echo<<obj["pageinfo"]["min"].to_int(); 
 			 echo<<";\n    var max_page=";
echo<<obj["pageinfo"]["max"].to_int(); 
 			 echo<<";\n    \n    var topicid=";
echo<<obj["topicid"].to_int(); 
 			 echo<<";\n    var topic_json = ";
echo << obj["list"].to_json(); 
 			 echo<<" ;\n    $('#topicid').append($('<option>').val(\"0\").text(\"所有栏目\").attr(\"selected\", true));\n    for (var i = 0; i < topic_json.length; i++) \n    {\n      if(topic_json[i].id==topicid)\n      {\n        $('#topicid').append($('<option>').val(topic_json[i].id).text(topic_json[i].value).data(\"pid\", topic_json[i].parentid).attr(\"selected\",\"selected\"));\n      }\n      else\n      {      \n        $('#topicid').append($('<option>').val(topic_json[i].id).text(topic_json[i].value).data(\"pid\", topic_json[i].parentid));\n      }\n    }\n    var treeselect =$(\"#topicid\").selectTree({ expandLevel: 1,\n        changeCallBack:function(data){\n           topicid=data[0].value;\n        }\n    });\n\n    $('#exampleModal').on('show.bs.modal', event => {\n            \n         $.getJSON(\"/admin/gettoparticle?topicid=\"+topicid,function(result){\n            if(result.code==0)\n            {   \n               addselect_content(result.alist);   \n            }\n          });\n    });\n    function addselect_content(alist)\n    {\n        var stable=$(\"#selecttable tbody\");\n        stable.empty();\n        \n        var relateval = $(\"#relatecontent\").val(); \n        const artArray = relateval.split(\",\");   \n            \n        for (let i = 0; i < alist.length; i++) {\n \n          const row = document.createElement(\"tr\");\n        \n          const cell = document.createElement(\"td\");\n          const cellText = document.createTextNode(alist[i].aid);\n          cell.appendChild(cellText);\n       \n          row.appendChild(cell);\n        \n      \n          const cella = document.createElement(\"td\");\n          const cellTexta = document.createTextNode(alist[i].title);\n          cella.appendChild(cellTexta);\n       \n          row.appendChild(cella);\n          \n          \n          var x = document.createElement(\"input\");\n          x.setAttribute(\"type\", \"checkbox\");\n          x.setAttribute(\"value\",alist[i].aid);\n          x.setAttribute(\"onClick\",\"select_addart(this.value,this)\");\n         \n          if(artArray.length>0)\n          {\n              for (let j = 0; j < artArray.length; j++) {\n                    if(alist[i].aid==artArray[j])\n                    {\n                        x.setAttribute(\"checked\", \"checked\");\n                        break;\n                    }\n              }\n          }\n          \n          const cellb = document.createElement(\"td\");\n          cellb.appendChild(x);\n          row.appendChild(cellb);\n          \n          stable.append(row);\n        }\n    }\n    \n    function select_addart(id,obj)\n    {\n        var relateval = $(\"#relatecontent\").val(); \n        const artArray = relateval.split(\",\");   \n  \n        if(obj.checked)\n        {\n           let n=0;\n           for (let j = 0; j < artArray.length; j++) {\n                if(id==artArray[j])\n                {\n                    n=1;\n                    break;\n                }\n\n            }     \n            if(n!=1)\n            {\n                artArray.push(id);\n                \n            }\n            $(\"#relatecontent\").val(artArray.join(\",\"));\n            \n        }\n        else\n        {\n            let newArray=[];\n            for (let j = 0; j < artArray.length; j++) {\n                    if(id==artArray[j])\n                    {\n                        continue;\n                    }\n                    newArray.push(artArray[j]);\n            }  \n            $(\"#relatecontent\").val(newArray.join(\",\"));\n        }\n    }\n    \n    \n        var current_page=";
echo<<obj["pageinfo"]["current"].to_int(); 
 			 echo<<";\n    var min_page=";
echo<<obj["pageinfo"]["min"].to_int(); 
 			 echo<<";\n    var max_page=";
echo<<obj["pageinfo"]["max"].to_int(); 
 			 echo<<";\n    \n    function select_prepage()\n    {\n           \n           if(current_page<1)\n           {\n                $(\"#select_pre\").addClass(\"disabled\");\n                return false;\n           }\n           \n           if($(\"#select_pre\").hasClass(\"disabled\"))\n           {\n                $(\"#select_pre\").removeClass(\"disabled\");\n           }\n           \n           var page=current_page-1;\n           $.getJSON(\"/admin/gettoparticle?page=\"+page+\"&topicid=\"+topicid,function(result){\n           if(result.code==0)\n           {   \n               addselect_content(result.alist);   \n           }\n          });\n         return false;\n    }\n    \n    function select_nextpage()\n    {\n           \n           if(current_page>=max_page)\n           {\n                $(\"#select_next\").addClass(\"disabled\");\n                return false;\n           }\n           if($(\"#select_next\").hasClass(\"disabled\"))\n           {\n                $(\"#select_next\").removeClass(\"disabled\");\n           }\n           \n           var page=current_page+1;\n           $.getJSON(\"/admin/gettoparticle?page=\"+page+\"&topicid=\"+topicid,function(result){\n           if(result.code==0)\n           {   \n               addselect_content(result.alist);   \n           }\n          });\n         return false;\n    }\n    \n    $(document).ready(function(e) {\n        $(\"#relate-icoimg\").change(function(){\n            var fd=new FormData();\n            if($(this).get(0).files[0].size>4596640){\n                $('#myfiletobigModal').modal('show');\n                return false;\n            }\n            fd.append(\"upfile\",$(this).get(0).files[0]);\n            $(\"#progress-bar-show\").show();\n            $(\"#progress-bar-result\").hide();\n            $.ajax({\n                url:\"/imageapi/upload?action=uploadimage\",\n                type:\"POST\",\n                processData: false,\n                contentType:false,\n                data: fd,\n                dataType: \"json\",\n                xhr: function() {  // custom xhr\n                    myXhr = $.ajaxSettings.xhr();\n                    if(myXhr.upload){ // check if upload property exists\n                        myXhr.upload.addEventListener('progress',function(evt){\n                            var percentComplete = Math.round(evt.loaded*100 / evt.total);\n                            $( \"#progress-bar\").css(\"width\",percentComplete*2);\n                            $( \"#progress-bar\").html(percentComplete+'%');\n                        }, false); // for handling the progress of the upload\n                    }\n                    return myXhr;\n                },\n                success:function(data){\n                    if(data.state==\"SUCCESS\"){\n                        $(\"#progress-bar-show\").hide();\n                        $(\"#progress-bar-result\").show();\n                        $(\"#icoimg\").val(data.url);\n\t\t\t\t\t\t$(\"#progress-bar-result\").html(\"<a href='\"+data.url+\"' target=_blank><img height=\\\"50\\\" width=\\\"50\\\"  src=\\\"\"+data.url+\"\\\"/></a>\");\n                       $('#myfilesuccessModal').modal('show');\n                    }\n                     \n                }\n            });\n        });\n    });\t  \n  </script>\n</body>\n\n</html>";

                  return echo.str();
             }

         }
        }
       }
    