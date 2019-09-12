previous_link = document.getElementById("previous_link");
previous_link.onclick = gotopreviouspage;
var ismobile   = /Android|webOS|iPhone|iPad|iPod|BlackBerry/i.test(navigator.userAgent);

function gotopreviouspage()
{
  sessionStorage.setItem("scrollX", "0");
  sessionStorage.setItem("scrollY", "0");
  window.history.back();
}

window.onscroll=function () {
    //set scroll position in session storage
    sessionStorage.setItem("scrollX", ismobile?String(window.pageXOffset):String(window.scrollX));
    sessionStorage.setItem("scrollY", ismobile?String(window.pageYOffset):String(window.scrollY));

};
var init = function () {
    window.focus();
    //get scroll position in session storage and go to that scroll position
    setTimeout(function() {window.scrollTo(parseInt(sessionStorage.getItem("scrollX"), 10),
     parseInt(sessionStorage.getItem("scrollY"), 10)) ;}, 100);
};
window.onload = init;
