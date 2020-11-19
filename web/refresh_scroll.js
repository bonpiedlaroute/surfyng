var ismobile   = /Android|webOS|iPhone|iPad|iPod|BlackBerry/i.test(navigator.userAgent);

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



var nb_dropdown_click = 0;
function show_dropdown_list()
{

  var filter_parameter = document.getElementById("filter_parameter");
  filter_parameter.className = "parameter_content show_parameter_content";
  nb_dropdown_click += 1;
}

window.onclick = function(event) {

  if (event.target.className != "fas fa-sort-amount-down" || ( (nb_dropdown_click % 2) == 0) )
  {
    var dropdowns = document.getElementsByClassName("parameter_content");
    var i;
    for (i = 0; i < dropdowns.length; i++) {
      var openDropdown = dropdowns[i];
      if (openDropdown.classList.contains('show_parameter_content')) {
        openDropdown.classList.remove('show_parameter_content');
      }
    }
    nb_dropdown_click = 0;
  }

}

function sortbyprice()
{
  sessionStorage.setItem("needtosort", "byprice");
}

function sortbysurface()
{
  sessionStorage.setItem("needtosort", "bysurface");
}

function sortbydate()
{
  sessionStorage.setItem("needtosort", "bydate");
}

function sortbypricebym2()
{
  sessionStorage.setItem("needtosort", "bypricebym2");
}
