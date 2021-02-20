function loadJSON(path, success, error) {

    var xobj = new XMLHttpRequest();
        xobj.overrideMimeType("application/json");
    xobj.open('GET', path, true);
    xobj.onreadystatechange = function () {
          if (xobj.readyState == 4 && xobj.status == "200") {
            // Required use of an anonymous callback as .open will NOT return a value but simply returns undefined in asynchronous mode
            if(success)
            success(xobj);
          }
          else {
              if(error)
              {
                error(xobj);
              }
          }
    };
    xobj.send(null);
 }

function createNode(element) {
    return document.createElement(element);
}

function append(parent, el) {
  return parent.appendChild(el);
}
function deactivate_alert_by_id(event){
  var endpoint = "https://surfyn.fr:7878/deactivate_alert?alert_id=";
  //var endpoint ="http://localhost:7878/deactivate_alert?alert_id="
  endpoint += event.currentTarget.id;
  fetch(endpoint, {
    method : 'POST'
  })
  .then(function(resp) {
    window.location.reload();
    showSuccessMessage("generic-box-message-result","generic-message-result","Votre alerte email a bien été supprimée");
   } )
  .catch(function(error) {
    console.log(error);
  });
}

function generate_my_search_page(data)
{
 var main_section = document.getElementById("mysearch-main-content");
 var my_research_frame;
 for(var i = 0; i < data.length; i++)
 {

   if( i%2 == 0)
   {
     if( i != 0)
     {
       main_section.appendChild(my_research_frame);
     }
     my_research_frame = createNode("div");
   }

   my_research_frame.className ="sf_my_research_frame";

   var my_research_container = createNode("div");
   my_research_container.className = "sf_my_research_container";

   if(i%2==0)
    my_research_container.style.marginLeft = "0px";

   var my_research_link = createNode("a");
   my_research_link.href = data[i].ADS_URL;
   my_research_link.className ="sf_my_research_link";

   var my_research_email_alert = createNode("div");
   my_research_email_alert.className = "sf_my_research_email_alert";

   if(data[i].ALERT_STATUS == "ON")
   {
     var email_alert_icon = createNode("i");
     email_alert_icon.className = "fas fa-bell sf_bell_alert_icon";

     my_research_email_alert.append(email_alert_icon);

     var email_alert_msg  = createNode("span");
     email_alert_msg.className = "sf_alert_text";
     email_alert_msg.innerHTML = "Alerte email activée";

     my_research_email_alert.append(email_alert_msg);
   }

   my_research_link.append(my_research_email_alert);

   var my_research_type_box = createNode("div");
   my_research_type_box.className = "sf_my_research_email_alert";
   var my_research_type = createNode("span");
   my_research_type.className = "sf_my_research_text";

   my_research_type.innerHTML = data[i].SEARCH_TYPE == "For sale"? "Achat":"Location";

   my_research_type_box.append(my_research_type);
   my_research_link.append(my_research_type_box);

   var my_research_infos = createNode("div");
   my_research_infos.className = "sf_my_research_infos";

   var property_type_box = createNode("div");
   property_type_box.className = "sf_my_research_small_box";


   var property_type = createNode("span");
   property_type.className = "sf_my_research_text";
   property_type.innerHTML = data[i].PROPERTY_TYPE;

   property_type_box.append(property_type);
   my_research_infos.append(property_type_box);

   var rooms_box = createNode("div");
   rooms_box.className = "sf_my_research_small_box";


   var rooms = createNode("span");
   rooms.className = "sf_my_research_text";
   rooms.innerHTML = String(data[i].ROOMS);
   rooms.innerHTML += " pièce(s)";

   rooms_box.append(rooms);
   my_research_infos.append(rooms_box);

   my_research_link.append(my_research_infos);

   var price_range_box = createNode("div");
   price_range_box.className = "sf_my_research_large_box";

   var price_range = createNode("span");
   price_range.className = "sf_my_research_text";
   price_range.innerHTML = "entre ";
   price_range.innerHTML += data[i].SEARCH_TYPE == "For sale"? String(data[i].PRICE_MIN/1000) + "k€" : String(data[i].PRICE_MIN) + "€";
   price_range.innerHTML += " et ";
   price_range.innerHTML += data[i].SEARCH_TYPE == "For sale"? String(data[i].PRICE_MAX/1000) + "k€" : String(data[i].PRICE_MAX) + "€";

   price_range_box.append(price_range);
   my_research_link.append(price_range_box);

   var surface_range_box = createNode("div");
   surface_range_box.className = "sf_my_research_large_box";


   var surface_range = createNode("span");
   surface_range.className = "sf_my_research_text";
   surface_range.innerHTML = "entre ";
   surface_range.innerHTML += String(data[i].AREA_MIN) + " m<sup>2</sup>";
   surface_range.innerHTML += " et ";
   surface_range.innerHTML += String(data[i].AREA_MAX) + " m<sup>2</sup>";

   surface_range_box.append(surface_range);
   my_research_link.append(surface_range_box);


   var city_box = createNode("div");
   city_box.className = "sf_my_research_city_box";

   var city = createNode("span");
   city.className = "sf_my_research_text";
   city.innerHTML = String(data[i].CITY);

   city_box.append(city);
   my_research_link.append(city_box);

   my_research_container.append(my_research_link);

   var delete_alert_box = createNode("div");
   delete_alert_box.className = "sf_my_research_small_box";
   delete_alert_box.style.cursor = "pointer";
   delete_alert_box.id = data[i].ID;
   delete_alert_box.onclick = deactivate_alert_by_id;

   var delete_alert_icon = createNode("i");
   delete_alert_icon.className = "fas fa-trash-alt sf_alert_trash";
   delete_alert_box.append(delete_alert_icon);

   my_research_container.append(delete_alert_box);

   my_research_frame.append(my_research_container);
 }

  main_section.appendChild(my_research_frame);

}


/*loadJSON("data/criteria.json",
function (data) { generate_my_search_page(JSON.parse(data.response));}, function(err) {console.log(err);});
*/
var url = 'https://surfyn.fr:7878/my_realestate_search?userid=';
//var url = 'http://localhost:7878/my_realestate_search?userid=';

var userid = sessionStorage.getItem("user_id");
url += userid;
fetch(url)
.then(function(resp) { return resp.json(); } )
.then(function(data) {
  generate_my_search_page(data);
})
.catch(function(error) {
    console.log(error);
});
