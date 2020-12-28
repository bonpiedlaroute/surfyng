var ismobile   = /Android|webOS|iPhone|iPad|iPod|BlackBerry/i.test(navigator.userAgent) || (navigator.platform === 'MacIntel' && navigator.maxTouchPoints > 1);

var gotoTopButton = document.getElementById("gotoTopButton");

window.onscroll=function () {
    //set scroll position in session storage
    sessionStorage.setItem("scrollX", ismobile?String(window.pageXOffset):String(window.scrollX));
    sessionStorage.setItem("scrollY", ismobile?String(window.pageYOffset):String(window.scrollY));

    var currentScrollY = ismobile?window.pageYOffset:window.scrollY;

    if (currentScrollY > 60 ) {
        gotoTopButton.style.display = "block";
    } else {
      gotoTopButton.style.display = "none";
    }
};
function scrollToTop() {
  window.scrollTo({ top: 0, behavior: 'smooth' });
}
