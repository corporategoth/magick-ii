// utils.js - javascript utilities for www.magick.tm pages.



function ShowGNU()
{ 
GnuWindow = window.open('http://www.gnu.org/copyleft/gpl.txt','GnuWindow','width=600,height=400,toolbar=no,location=no,scrollbars=yes') 
}




function hideLinkLines()
{
if(document.getElementsByTagName)
{
el = document.getElementsByTagName('a')
for(i=0;i<el.length;i++)
{
el[i].onfocus = function() { this.blur() }
}
delete el
}
}
onload = hideLinkLines



//  Function openWindow()
//  - imgPage:   html page which is loaded in the new window
//  - imgName:   name of the image (not filename)
//  - imgWidth:  width of the image (actually, the width of the new window)
//  - imgHeight: height of the image (actually, the width of the new window)
//
function openWindow(imgPage, imgName, imgWidth, imgHeight)
{
	window.open(	imgPage, 
			imgName, 
			'width=' + width + ',height=' + height + 		',resizable=yes,scrollbars=yes,toolbar=no,location=no,directories=no,status=no,menubar=no,copyhistory=no');
}