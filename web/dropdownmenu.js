var nb_dropdown_click = 0;
function show_dropdown_list()
{

  var filter_parameter = document.getElementById("filter_parameter");
  filter_parameter.className = "parameter_content show_parameter_content";
  nb_dropdown_click += 1;

  var dropdown = document.getElementById("dropdownmenu");
  dropdown.style.zIndex = "6";
}

window.onclick = function(event) {

  if (event.target.className != "fas fa-bars" || ( (nb_dropdown_click % 2) == 0) )
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

    var dropdown = document.getElementById("dropdownmenu");
    dropdown.style.zIndex = "4";
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
