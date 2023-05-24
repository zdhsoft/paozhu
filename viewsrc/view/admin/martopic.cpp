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
//g++ viewsrc/view/admin/martopic.cpp
namespace http {

namespace view {
	namespace admin{
 		 std::string martopic(const struct view_param &vinfo,http::OBJ_VALUE &obj)
			{
 
                     std::ostringstream echo;

        
 			 echo<<"<!doctype html>\n<html lang=\"en\" data-bs-theme=\"auto\">\n\n<head>\n  <meta charset=\"utf-8\">\n  <meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">\n  <meta name=\"description\" content=\"\">\n  <title>栏目管理</title>\n\n  <link href=\"/assets/dist/css/bootstrap.min.css\" rel=\"stylesheet\">\n  <link rel=\"stylesheet\" href=\"/assets/icons/font/bootstrap-icons.css\">\n\n  <link href=\"/css/dashboard.css\" rel=\"stylesheet\">\n  <script src=\"/js/jquery.min.js\"></script>  \n</head>\n\n<body>\n\n  <div class=\"container-fluid\">\n\n    <h5 class=\"card-title mt-2\">栏目管理</h5>\n    <hr>\n    <div class=\"row p-3\">\n      <div class=\"col-10\">\n        <table class=\"table table-bordered\">\n          <thead>\n            <tr>\n              <th scope=\"col\">#</th>\n              <th scope=\"col\">名称</th>\n              <th scope=\"col\">url</th>\n              <th scope=\"col\">类型</th>\n              <th scope=\"col\">编辑</th>\n            </tr>\n          </thead>\n          <tbody>\n            ";
 for(auto &a:obj["list"].as_array()){ 
 			 echo<<"              <tr id=\"topicid_";
 echo<<a.second["id"].to_string(); 
 			 echo<<"\">\n                <td>\n                  ";
 echo<<a.second["id"].to_string(); 
 			 echo<<"                </td>\n                <td ";
 if(a.second["parentid"].to_int()>0){ 
 			 echo<<"style=\"padding-left:";
 echo<<(a.second["level"].to_int()*15+15); 
 			 echo<<"px\"";
 } 
 			 echo<<">\n                  ";
 echo<<a.second["value"].as_string(); 
 			 echo<<"                </td>\n                <td>\n                  ";
 echo<<a.second["urlpath"].as_string(); 
 			 echo<<"                </td>\n                <td>\n                  ";
 echo<<a.second["cateid"].as_string(); 
 			 echo<<"                </td>\n                <td>\n                  <a href=\"/admin/edittopic?id=";
 echo<<a.second["id"].to_string(); 
 			 echo<<"\">编辑</a>｜\n                  <a href=\"/admin/deletetopic?id=";
 echo<<a.second["id"].to_string(); 
 			 echo<<"\" onclick=\"return deletetopic(";
 echo<<a.second["id"].to_string(); 
 			 echo<<")\">删除</a>\n                </td>\n              </tr>\n              ";
 } 
 			 echo<<"          </tbody>\n\n        </table>\n      </div>\n    </div>\n\n  </div>\n\n  <script src=\"/assets/dist/js/bootstrap.bundle.min.js\"></script>\n  \n  <script lang=\"javascript\">\n  \n    function deletetopic(id)\n    {\n      if(confirm('是否删除?'))\n      {          \n          $.getJSON(\"/admin/deletetopic?id=\"+id,function(result){\n                if(result.code==0)\n                {\n                \n                     $(\"#topicid_\"+id).remove();\n                }\n                \n          });\n\n      }\n      return false;\n    }\n  \n  </script>\n</body>\n\n</html>";

                  return echo.str();
             }

         }
        }
       }
    