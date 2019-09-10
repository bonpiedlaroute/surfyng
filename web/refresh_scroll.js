previous_link = document.getElementById("previous_link");
previous_link.onclick = gotopreviouspage;

function gotopreviouspage()
{
  sessionStorage.setItem("scrollX", "0");
  sessionStorage.setItem("scrollY", "0");
  window.history.back();
}

window.onscroll=function () {
    //set scroll position in session storage
    sessionStorage.setItem("scrollX", String(window.scrollX));
    sessionStorage.setItem("scrollY", String(window.scrollY));

};
var init = function () {
    window.focus();
    //get scroll position in session storage and go to that scroll position
    window.scrollTo(parseInt(sessionStorage.getItem("scrollX"), 10),
     parseInt(sessionStorage.getItem("scrollY"), 10));
};
window.onload = init;
