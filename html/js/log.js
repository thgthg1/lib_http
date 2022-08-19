function sendPost(num)
{
    var xmlhttp;
    var name;
    var passwd;
    var email;
	if (window.XMLHttpRequest)
	{
		xmlhttp=new XMLHttpRequest();
	}
	else
	{
		xmlhttp=new ActiveXObject("Microsoft.XMLHTTP");
	}
	xmlhttp.onreadystatechange=function()
	{
		if (xmlhttp.readyState==4 && xmlhttp.status==200)
		{
            var response=JSON.parse(xmlhttp.responseText);
            if(response.res==11)
            {
                //
                alert("your name had been registered");
            }
            else if(response.res==10)
            {
                window.location="log.html";
                //alert("register is success ");
            }
            else if(response.res==20)
            {
                window.location="main.html";
                //alert("log is success ");
            }
            else if(response.res==21)
            {
                alert("name or password is error");
            }
            
		}
	}
    xmlhttp.open("POST","test.zz",true);
    if(num==0)
    {
        name=document.getElementById("name");
        passwd=document.getElementById("passwd");
        email=document.getElementById("email").value;
        name=name.value;
        passwd=passwd.value;
        var str={"order":"register","name":name,"passwd":passwd,"email":email};
        xmlhttp.send(JSON.stringify(str));
        
    }
    else if(num==1)
    {
        name=document.getElementById("name1");
        passwd=document.getElementById("passwd1");
        name=name.value;
        passwd=passwd.value;
        var str={"order":"log","name":name,"passwd":passwd};
        xmlhttp.send(JSON.stringify(str));
        setCookie("name",name,7);
    }
	
}
function setCookie(cname,cvalue,exdays)
{
  var d = new Date();
  d.setTime(d.getTime()+(exdays*24*60*60*1000));
  var expires = "expires="+d.toGMTString();
  document.cookie = cname + "=" + cvalue + "; " + expires;
}
function getCookie(cname)
{
  var name = cname + "=";
  var ca = document.cookie.split(';');
  for(var i=0; i<ca.length; i++) 
  {
    var c = ca[i].trim();
    if (c.indexOf(name)==0) return c.substring(name.length,c.length);
  }
  return "";
}

