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


//const url = 'https://surfyn.fr:7878/search/ad?id='+ window.location.search.substr(1);
const url = 'http://127.0.0.1:7878/search/ad?id='+ window.location.search.substr(1);

var puppeter = document.getElementById("target");
if(!puppeter)
{
  fetch(url)
  .then(function(resp) { return resp.json(); } )
  .then(function(data) {

    generate_details_page(data);

  })
  .catch(function(error) {

    console.log(error);
  });
}


function generate_details_page(data)
{
  if(data.length == 0 )
  {
    document.title = "Annonce introuvable - Surfyn";
    document.getElementsByTagName('meta')["description"].content = "Cette annonce n'est plus disponible";
    var announces_found = document.getElementById("nb_announces_found");
    announces_found.innerHTML = "Cette annonce n'est plus disponible";
    const metaRobots = document.createElement('meta');
    metaRobots.name = 'robots';
    metaRobots.content = 'noindex';
    document.head.appendChild(metaRobots);
  }
  else
  {
    var nb_room = "";

    var ismobile   = /Android|webOS|iPhone|iPad|iPod|BlackBerry/i.test(navigator.userAgent);
    var det_container_div;
    var main_content = document.getElementById("main-content");
    for(var i = 0; i < data.length; i++)
    {
      if(Object.keys(data[i]).length === 0)
          continue;

      if(i%2 == 0 )
      {
        det_container_div = createNode("div");
        det_container_div.className = "det_container";
      }

      var det_ad_container_div = createNode("div");
      det_ad_container_div.className = "det_ad_container";

      if(i%2 != 0 )
      det_ad_container_div.style.marginLeft = ismobile ? "15px" : "70px";

      var ad_link = createNode("a");
      ad_link.style.display = "block";
      ad_link.href = data[i].ANNOUNCE_LINK;
      ad_link.target = "_blank";
      ad_link.rel = "nofollow";

      var det_ad_image_container_div = createNode("div");
      det_ad_image_container_div.className = "det_ad_image_container";

      var det_ad_image_frame_div = createNode("div");
      det_ad_image_frame_div.className = "det_ad_image_frame";

      var det_ad_img = createNode("img");
      det_ad_img.className = "det_ad_image";
      det_ad_img.src = data[i].hasOwnProperty('IMAGE')? data[i].IMAGE: "logo.svg";
      /* filling alt attribute */
      var alt = "";
      alt += data[i].PROPERTY_TYPE + " a ";
      alt += data[i].SEARCH_TYPE == "For sale"? "vendre ":"louer ";
      alt += data[i].CITY ;
      if(data[i].hasOwnProperty('ROOMS'))
      {
        alt += " - " + data[i].ROOMS;
        alt += " pièce(s)";
      }
      if(data[i].hasOwnProperty('SURFACE'))
      {
        alt += " - " + data[i].SURFACE;
        alt += " m2";
      }
      alt += " - Surfyn";
      det_ad_img.alt = alt;
      /*end filling alt attribute */
      det_ad_image_frame_div.appendChild(det_ad_img);

      var det_icon_div = createNode("div");
      det_icon_div.className = "det_ad_image_icon_container";

      var det_image_count_span =  createNode("span");
      det_image_count_span.className = "no_select";
      det_image_count_span.style.color = "white";
      det_image_count_span.style.fontWeight = "bold";
      det_image_count_span.innerHTML = data[i].IMAGE_COUNT;

      det_icon_div.appendChild(det_image_count_span);

      var icon_image = createNode("i");
      icon_image.className ="far fa-image det_ad_image_icon";
      det_icon_div.appendChild(icon_image);

      det_ad_image_frame_div.appendChild(det_icon_div);


      det_ad_image_container_div.appendChild(det_ad_image_frame_div);
      ad_link.appendChild(det_ad_image_container_div);

      var det_desc_container_div = createNode("div");
      det_desc_container_div.className = "det_ad_desc_container";

      var count_desc = 0;
      if(data[i].hasOwnProperty('TRANSPORT'))
      {
        count_desc++;

        var det_transport_desc_container_div = createNode("div");
        det_transport_desc_container_div.className = "det_ad_desc_shape det_ad_desc_border";
        var det_ad_icon_container_div = createNode("div");
        det_ad_icon_container_div.className = "det_ad_desc_center";
        det_ad_icon_container_div.style.width = "100%";
        det_ad_icon_container_div.style.height = "50%";

        var det_ad_icon_div = createNode("i");
        det_ad_icon_div.className = "fas fa-subway";
        det_ad_icon_div.style.color = "#4C8E99";

        det_ad_icon_container_div.appendChild(det_ad_icon_div);

        det_transport_desc_container_div.appendChild(det_ad_icon_container_div);

        var det_ad_desc_transport_div = createNode("div");
        det_ad_desc_transport_div.className = "det_ad_desc_center";
        det_ad_desc_transport_div.style.width = "100%";
        det_ad_desc_transport_div.style.height = "50%";

        var det_transport_span = createNode("span");
        det_transport_span.className = "no_select";
        det_transport_span.style.fontWeight = "bold";
        det_transport_span.style.color = "#4C8E99";
        if(ismobile)
        det_transport_span.style.fontSize = "12px";
        det_transport_span.innerHTML = "Transport";

        det_ad_desc_transport_div.appendChild(det_transport_span);

        det_transport_desc_container_div.appendChild(det_ad_desc_transport_div);

        det_desc_container_div.appendChild(det_transport_desc_container_div);

      }
      /* PRICE */
      {
        var det_price_container_div = createNode("div");
        det_price_container_div.className = "det_ad_desc_shape det_ad_desc_border det_ad_desc_center";

        var det_ad_ticker_div = createNode("div");
        det_ad_ticker_div.className = "det_ad_ticker";
        var det_ad_ticker = createNode("i");
        det_ad_ticker.className ="fas fa-check-circle det_ad_ticker_icon";
        det_ad_ticker.style.color = "green";
        det_ad_ticker_div.appendChild(det_ad_ticker);
        det_price_container_div.appendChild(det_ad_ticker);

        var det_price_span_div = createNode("div");
        det_price_span_div.className = "det_ad_span_box";

        var det_price_span = createNode("span");
        det_price_span.className = "no_select";
        det_price_span.style.fontWeight ="bold";
        det_price_span.style.color = "#4C8E99";
        if(ismobile)
        det_price_span.style.fontSize = "12px";

        if(data[i].hasOwnProperty('SEARCH_TYPE') && data[i].SEARCH_TYPE == "For rent")
        {
          det_price_span.innerHTML = String(data[i].PRICE);
          det_price_span.innerHTML += " €";
        }
        else {
          var prop_price = (data[i].PRICE / 1000) | 0 ;

          det_price_span.innerHTML = String(prop_price);
          det_price_span.innerHTML += " K€";
        }

        det_price_span_div.appendChild(det_price_span);

        det_price_container_div.appendChild(det_price_span_div);
        det_desc_container_div.appendChild(det_price_container_div);
      }
      /* SURFACE */
      {
        var det_surface_container_div = createNode("div");
        det_surface_container_div.className = "det_ad_desc_shape det_ad_desc_border det_ad_desc_center";

        if(!data[i].hasOwnProperty('SURFACE'))
        det_surface_container_div.style = "border:3px solid lightgray";


        var det_ad_ticker_div = createNode("div");
        det_ad_ticker_div.className = "det_ad_ticker";
        var det_ad_ticker = createNode("i");
        det_ad_ticker.className ="fas fa-check-circle det_ad_ticker_icon";

        if(!data[i].hasOwnProperty('SURFACE'))
        det_ad_ticker.style.color = "lightgray";
        else
        det_ad_ticker.style.color = "green";

        det_ad_ticker_div.appendChild(det_ad_ticker);
        det_surface_container_div.appendChild(det_ad_ticker);

        var det_surface_span_div = createNode("div");
        det_surface_span_div.className = "det_ad_span_box";

        var det_surface_span = createNode("span");
        det_surface_span.className = "no_select";
        det_surface_span.style.fontWeight ="bold";

        if(data[i].hasOwnProperty('SURFACE'))
        det_surface_span.style.color = "#4C8E99";
        else
        det_surface_span.style.color = "lightgray";

        if(ismobile)
        det_surface_span.style.fontSize = "12px";

        if(data[i].hasOwnProperty('SURFACE'))
        det_surface_span.innerHTML = String(data[i].SURFACE);

        det_surface_span.innerHTML += " m<sup>2";

        det_surface_span_div.appendChild(det_surface_span);

        det_surface_container_div.appendChild(det_surface_span_div);
        det_desc_container_div.appendChild(det_surface_container_div);
      }
        /* LOCATION */
      {
        var det_location_container_div = createNode("div");
        det_location_container_div.className = "det_ad_desc_shape det_ad_desc_border";

        if(!data[i].hasOwnProperty('LOCATION'))
        det_location_container_div.style = "border:3px solid lightgray";

        var det_location_icon_container_div = createNode("div");
        det_location_icon_container_div.className = "det_ad_desc_center";
        det_location_icon_container_div.style.width = "100%";
        det_location_icon_container_div.style.height = "50%";
        det_location_icon_container_div.style.flexDirection = "column";

        var det_ad_ticker = createNode("i");
        det_ad_ticker.className ="fas fa-check-circle det_ad_ticker_icon";

        if(!data[i].hasOwnProperty('LOCATION'))
        det_ad_ticker.style.color = "lightgray";
        else
        det_ad_ticker.style.color = "green";

        det_location_icon_container_div.appendChild(det_ad_ticker);

        var det_location_icon = createNode("i");
        det_location_icon.className = "fas fa-map-marker-alt";

        if(data[i].hasOwnProperty('LOCATION'))
        det_location_icon.style.color = "#4C8E99";
        else
        det_location_icon.style.color = "lightgray";

        det_location_icon_container_div.appendChild(det_location_icon);

        det_location_container_div.appendChild(det_location_icon_container_div);

        var det_location_text_div = createNode("div");
        det_location_text_div.className = "det_ad_desc_center";
        det_location_text_div.style.width = "100%";
        det_location_text_div.style.height = "50%";

        var det_location_text_span = createNode("span");
        det_location_text_span.className = "no_select";
        det_location_text_span.style.fontWeight = "bold";

        if(data[i].hasOwnProperty('LOCATION'))
        det_location_text_span.style.color = "#4C8E99";
        else
        det_location_text_span.style.color = "lightgray";

        if(ismobile)
        det_location_text_span.style.fontSize = "12px";
        det_location_text_span.innerHTML = "position";

        det_location_text_div.appendChild(det_location_text_span);
        det_location_container_div.appendChild(det_location_text_div);

        det_desc_container_div.appendChild(det_location_container_div);
      }
      /* ROOMS */
      {
        var det_nb_rooms_container_div = createNode("div");
        det_nb_rooms_container_div.className = "det_ad_desc_shape det_ad_desc_border det_ad_desc_center";

        if(!data[i].hasOwnProperty('ROOMS'))
        det_nb_rooms_container_div.style = "border:3px solid lightgray";


        var det_ad_ticker_div = createNode("div");
        det_ad_ticker_div.className = "det_ad_ticker";
        var det_ad_ticker = createNode("i");
        det_ad_ticker.className ="fas fa-check-circle det_ad_ticker_icon";

        if(!data[i].hasOwnProperty('ROOMS'))
        det_ad_ticker.style.color = "lightgray";
        else
        det_ad_ticker.style.color = "green";

        det_ad_ticker_div.appendChild(det_ad_ticker);
        det_nb_rooms_container_div.appendChild(det_ad_ticker);

        var det_nb_rooms_span_div = createNode("div");
        det_nb_rooms_span_div.className = "det_ad_span_box";

        var det_nb_rooms_span = createNode("span");
        det_nb_rooms_span.className = "no_select";
        det_nb_rooms_span.style.fontWeight = "bold";

        if(data[i].hasOwnProperty('ROOMS'))
        det_nb_rooms_span.style.color = "#4C8E99";
        else
        det_nb_rooms_span.style.color = "lightgray";

        if(ismobile)
        det_nb_rooms_span.style.fontSize = "12px";

        if(data[i].hasOwnProperty('ROOMS'))
        det_nb_rooms_span.innerHTML = String(data[i].ROOMS);
        if(nb_room.length === 0 )
          nb_room = det_nb_rooms_span.innerHTML;
        det_nb_rooms_span.innerHTML += " pièce(s)";

        det_nb_rooms_span_div.appendChild(det_nb_rooms_span);

        det_nb_rooms_container_div.appendChild(det_nb_rooms_span_div);
        det_desc_container_div.appendChild(det_nb_rooms_container_div);
      }

      /* CONSTRUCTION_YEAR */
      {
        var det_year_container_div = createNode("div");
        det_year_container_div.className = "det_ad_desc_shape det_ad_desc_border det_ad_desc_center";

        if(!data[i].hasOwnProperty('CONSTRUCTION_YEAR'))
        det_year_container_div.style = "border:3px solid lightgray";


        var det_ad_ticker_div = createNode("div");
        det_ad_ticker_div.className = "det_ad_ticker";
        var det_ad_ticker = createNode("i");
        det_ad_ticker.className ="fas fa-check-circle det_ad_ticker_icon";

        if(!data[i].hasOwnProperty('CONSTRUCTION_YEAR'))
        det_ad_ticker.style.color = "lightgray";
        else
        det_ad_ticker.style.color = "green";

        det_ad_ticker_div.appendChild(det_ad_ticker);
        det_year_container_div.appendChild(det_ad_ticker);

        var det_year_span_div = createNode("div");
        det_year_span_div.className = "det_ad_span_box";

        var det_year_span = createNode("span");
        det_year_span.className = "no_select";
        det_year_span.style.fontWeight = "bold";

        if(data[i].hasOwnProperty('CONSTRUCTION_YEAR'))
        det_year_span.style.color = "#4C8E99";
        else
        det_year_span.style.color = "lightgray";


        det_year_span.style.fontSize = "12px";

        if(data[i].hasOwnProperty('CONSTRUCTION_YEAR'))
        det_year_span.innerHTML = data[i].CONSTRUCTION_YEAR;
        else {
          det_year_span.innerHTML = "construction";
        }

        det_year_span_div.appendChild(det_year_span);

        det_year_container_div.appendChild(det_year_span_div);
        det_desc_container_div.appendChild(det_year_container_div);
      }
      /* TYPE Of HEATING */
      {
        var det_heating_container_div = createNode("div");
        det_heating_container_div.className = "det_ad_desc_shape det_ad_desc_border det_ad_desc_center";

        if(!data[i].hasOwnProperty('TYPE_OF_HEATING'))
        det_heating_container_div.style = "border:3px solid lightgray";


        var det_ad_ticker_div = createNode("div");
        det_ad_ticker_div.className = "det_ad_ticker";
        var det_ad_ticker = createNode("i");
        det_ad_ticker.className ="fas fa-check-circle det_ad_ticker_icon";

        if(!data[i].hasOwnProperty('TYPE_OF_HEATING'))
        det_ad_ticker.style.color = "lightgray";
        else
        det_ad_ticker.style.color = "green";

        det_ad_ticker_div.appendChild(det_ad_ticker);
        det_heating_container_div.appendChild(det_ad_ticker);

        var det_heating_span_div = createNode("div");
        det_heating_span_div.className = "det_ad_span_box";

        var det_heating_span = createNode("span");
        det_heating_span.className = "no_select";
        det_heating_span.style.fontWeight = "bold";

        if(data[i].hasOwnProperty('TYPE_OF_HEATING'))
        det_heating_span.style.color = "#4C8E99";
        else
        det_heating_span.style.color = "lightgray";

        //if(ismobile)
        det_heating_span.style.fontSize = "12px";

        if(data[i].hasOwnProperty('TYPE_OF_HEATING'))
        det_heating_span.innerHTML = data[i].TYPE_OF_HEATING;
        else {
          det_heating_span.innerHTML = "type de chauffage";
        }

        det_heating_span_div.appendChild(det_heating_span);

        det_heating_container_div.appendChild(det_heating_span_div);
        det_desc_container_div.appendChild(det_heating_container_div);
      }

      /* PARKING */
      {
        var det_parking_container_div = createNode("div");
        det_parking_container_div.className = "det_ad_desc_shape det_ad_desc_border det_ad_desc_center";

        if(!data[i].hasOwnProperty('PARKING'))
        det_parking_container_div.style = "border:3px solid lightgray";


        var det_ad_ticker_div = createNode("div");
        det_ad_ticker_div.className = "det_ad_ticker";
        var det_ad_ticker = createNode("i");
        det_ad_ticker.className ="fas fa-check-circle det_ad_ticker_icon";

        if(!data[i].hasOwnProperty('PARKING'))
        det_ad_ticker.style.color = "lightgray";
        else
        det_ad_ticker.style.color = "green";

        det_ad_ticker_div.appendChild(det_ad_ticker);
        det_parking_container_div.appendChild(det_ad_ticker);

        var det_parking_span_div = createNode("div");
        det_parking_span_div.className = "det_ad_span_box";

        var det_parking_span = createNode("span");
        det_parking_span.className = "no_select";
        det_parking_span.style.fontWeight = "bold";

        if(data[i].hasOwnProperty('PARKING'))
        det_parking_span.style.color = "#4C8E99";
        else
        det_parking_span.style.color = "lightgray";

        if(ismobile)
        det_parking_span.style.fontSize = "12px";

        if(data[i].hasOwnProperty('PARKING'))
        det_parking_span.innerHTML = data[i].PARKING;

        det_parking_span.innerHTML += " parking";

        det_parking_span_div.appendChild(det_parking_span);

        det_parking_container_div.appendChild(det_parking_span_div);
        det_desc_container_div.appendChild(det_parking_container_div);
      }

      /* FLOOR */
      {
        var det_ad_floor_div = createNode("div");

        det_ad_floor_div.className = "det_ad_desc_shape det_ad_desc_border det_ad_desc_center";

        if(!data[i].hasOwnProperty('FLOOR'))
        det_ad_floor_div.style = "border:3px solid lightgray";

        var det_ad_ticker_div = createNode("div");
        det_ad_ticker_div.className = "det_ad_ticker";
        var det_ad_ticker = createNode("i");
        det_ad_ticker.className ="fas fa-check-circle det_ad_ticker_icon";

        if(!data[i].hasOwnProperty('FLOOR'))
        det_ad_ticker.style.color = "lightgray";
        else
        det_ad_ticker.style.color = "green";

        det_ad_ticker_div.appendChild(det_ad_ticker);
        det_ad_floor_div.appendChild(det_ad_ticker);

        var det_floor_span_div = createNode("div");
        det_floor_span_div.className = "det_ad_span_box";

        var det_ad_floor_span = createNode("span");
        det_ad_floor_span.className = "no_select";
        det_ad_floor_span.style.fontWeight = "bold";

        if(data[i].hasOwnProperty('FLOOR'))
        det_ad_floor_span.style.color = "#4C8E99";
        else
        det_ad_floor_span.style.color = "lightgray";

        det_ad_floor_span.style.marginLeft = "10px";
        if(ismobile)
        det_ad_floor_span.style.fontSize = "12px";

        if(data[i].hasOwnProperty('FLOOR'))
        det_ad_floor_span.innerHTML = data[i].FLOOR;

        if(data[i].FLOOR != "rez-de-chaussée")
        det_ad_floor_span.innerHTML +=" étage"

        det_floor_span_div.appendChild(det_ad_floor_span);
        det_ad_floor_div.appendChild(det_floor_span_div);

        det_desc_container_div.appendChild(det_ad_floor_div);
      }
      /* CELLAR */

      {
        count_desc++;
        var det_ad_cellar_div = createNode("div");
        det_ad_cellar_div.className = "det_ad_desc_shape det_ad_desc_border det_ad_desc_center";

        if(!data[i].hasOwnProperty('CELLAR'))
        det_ad_cellar_div.style = "border:3px solid lightgray";

        var det_ad_ticker_div = createNode("div");
        det_ad_ticker_div.className = "det_ad_ticker";
        var det_ad_ticker = createNode("i");
        det_ad_ticker.className ="fas fa-check-circle det_ad_ticker_icon";

        if(!data[i].hasOwnProperty('CELLAR'))
        det_ad_ticker.style.color = "lightgray";
        else
        det_ad_ticker.style.color = "green";

        det_ad_ticker_div.appendChild(det_ad_ticker);
        det_ad_cellar_div.appendChild(det_ad_ticker);

        var det_cellar_span_div = createNode("div");
        det_cellar_span_div.className = "det_ad_span_box";

        var det_ad_cellar_span = createNode("span");
        det_ad_cellar_span.className = "no_select";
        det_ad_cellar_span.style.fontWeight = "bold";

        if(data[i].hasOwnProperty('CELLAR'))
        det_ad_cellar_span.style.color = "#4C8E99";
        else
        det_ad_cellar_span.style.color = "lightgray";

        if(ismobile)
        det_ad_cellar_span.style.fontSize = "12px";
        if(data[i].hasOwnProperty('CELLAR'))
        det_ad_cellar_span.innerHTML = data[i].CELLAR;

        det_ad_cellar_span.innerHTML += " cave(s)";

        det_cellar_span_div.appendChild(det_ad_cellar_span);

        det_ad_cellar_div.appendChild(det_cellar_span_div);

        det_desc_container_div.appendChild(det_ad_cellar_div);
      }
      /* BEDROOMS */
      {
        count_desc++;
        var det_ad_bedrooms_div = createNode("div");
        det_ad_bedrooms_div.className = "det_ad_desc_shape det_ad_desc_border det_ad_desc_center";

        if(!data[i].hasOwnProperty('BEDROOMS'))
        det_ad_bedrooms_div.style = "border:3px solid lightgray";

        var det_ad_ticker_div = createNode("div");
        det_ad_ticker_div.className = "det_ad_ticker";
        var det_ad_ticker = createNode("i");
        det_ad_ticker.className ="fas fa-check-circle det_ad_ticker_icon";

        if(!data[i].hasOwnProperty('BEDROOMS'))
        det_ad_ticker.style.color = "lightgray";
        else
        det_ad_ticker.style.color = "green";

        det_ad_ticker_div.appendChild(det_ad_ticker);
        det_ad_bedrooms_div.appendChild(det_ad_ticker);

        var det_bedrooms_span_div = createNode("div");
        det_bedrooms_span_div.className = "det_ad_span_box";

        var det_ad_bedrooms_span = createNode("span");
        det_ad_bedrooms_span.className = "no_select";
        det_ad_bedrooms_span.style.fontWeight = "bold";

        if(data[i].hasOwnProperty('BEDROOMS'))
        det_ad_bedrooms_span.style.color = "#4C8E99";
        else
        det_ad_bedrooms_span.style.color = "lightgray";

        det_ad_bedrooms_span.style.fontSize = "12px";

        if(data[i].hasOwnProperty('BEDROOMS'))
        det_ad_bedrooms_span.innerHTML = data[i].BEDROOMS;

        det_ad_bedrooms_span.innerHTML += " chambre(s)";

        det_bedrooms_span_div.appendChild(det_ad_bedrooms_span);

        det_ad_bedrooms_div.appendChild(det_bedrooms_span_div);

        det_desc_container_div.appendChild(det_ad_bedrooms_div);
      }

      /* LAND_SURFACE */
      {
        count_desc++;
        var det_ad_landsurface_div = createNode("div");
        det_ad_landsurface_div.className = "det_ad_desc_shape det_ad_desc_border det_ad_desc_center";

        if(!data[i].hasOwnProperty('LAND_SURFACE'))
        det_ad_landsurface_div.style = "border:3px solid lightgray";

        var det_ad_ticker_div = createNode("div");
        det_ad_ticker_div.className = "det_ad_ticker";
        var det_ad_ticker = createNode("i");
        det_ad_ticker.className ="fas fa-check-circle det_ad_ticker_icon";

        if(!data[i].hasOwnProperty('LAND_SURFACE'))
        det_ad_ticker.style.color = "lightgray";
        else
        det_ad_ticker.style.color = "green";

        det_ad_ticker_div.appendChild(det_ad_ticker);
        det_ad_landsurface_div.appendChild(det_ad_ticker);

        var det_landsurface_span_div = createNode("div");
        det_landsurface_span_div.className = "det_ad_span_box";

        var det_ad_landsurface_span = createNode("span");
        det_ad_landsurface_span.className = "no_select";
        det_ad_landsurface_span.style.fontWeight = "bold";

        if(data[i].hasOwnProperty('LAND_SURFACE'))
        det_ad_landsurface_span.style.color = "#4C8E99";
        else
        det_ad_landsurface_span.style.color = "lightgray";

        if(ismobile)
        det_ad_landsurface_span.style.fontSize = "12px";

        det_ad_landsurface_span.innerHTML = "terrain ";

        if(data[i].hasOwnProperty('LAND_SURFACE'))
        {
          det_ad_landsurface_span.innerHTML += parseInt(data[i].LAND_SURFACE);

          det_ad_landsurface_span.innerHTML += " m<sup>2</sup>";
        }



        det_landsurface_span_div.appendChild(det_ad_landsurface_span);

        det_ad_landsurface_div.appendChild(det_landsurface_span_div);

        det_desc_container_div.appendChild(det_ad_landsurface_div);
      }
      /* PRICE IN SQUARE METER */
      {
        count_desc++;
        var det_ad_pricebym2_div = createNode("div");
        det_ad_pricebym2_div.className = "det_ad_desc_shape det_ad_desc_border det_ad_desc_center";

        if(!data[i].hasOwnProperty('PRICE') || !data[i].hasOwnProperty('SURFACE'))
        det_ad_pricebym2_div.style = "border:3px solid lightgray";

        var det_ad_ticker_div = createNode("div");
        det_ad_ticker_div.className = "det_ad_ticker";
        var det_ad_ticker = createNode("i");
        det_ad_ticker.className ="fas fa-check-circle det_ad_ticker_icon";

        if(!data[i].hasOwnProperty('PRICE') || !data[i].hasOwnProperty('SURFACE'))
        det_ad_ticker.style.color = "lightgray";
        else
        det_ad_ticker.style.color = "green";

        det_ad_ticker_div.appendChild(det_ad_ticker);
        det_ad_pricebym2_div.appendChild(det_ad_ticker);

        var det_pricebym2_span_div = createNode("div");
        det_pricebym2_span_div.className = "det_ad_span_box";

        var det_ad_pricebym2_span = createNode("span");
        det_ad_pricebym2_span.className = "no_select";
        det_ad_pricebym2_span.style.fontWeight = "bold";

        if(data[i].hasOwnProperty('PRICE') && data[i].hasOwnProperty('SURFACE'))
        det_ad_pricebym2_span.style.color = "#4C8E99";
        else
        det_ad_pricebym2_span.style.color = "lightgray";

        if(ismobile)
        det_ad_pricebym2_span.style.fontSize = "12px";
        if(data[i].hasOwnProperty('PRICE') && data[i].hasOwnProperty('SURFACE'))
        {
          var price_bym2 = Math.floor(data[i].PRICE / data[i].SURFACE);
          det_ad_pricebym2_span.innerHTML = formatPrice(String(price_bym2)) + " €/m<sup>2</sup>";
        }

        det_pricebym2_span_div.appendChild(det_ad_pricebym2_span);

        det_ad_pricebym2_div.appendChild(det_pricebym2_span_div);

        det_desc_container_div.appendChild(det_ad_pricebym2_div);
      }

      ad_link.appendChild(det_desc_container_div);

      var det_ad_notary_fees_frame_div = createNode("div");
      det_ad_notary_fees_frame_div.className = "det_notary_fees_frame";
      if(data[i].hasOwnProperty('PRICE') && data[i].hasOwnProperty('SEARCH_TYPE') && data[i].SEARCH_TYPE == "For sale")
      {
          var det_ad_notary_fees_box_div = createNode("div");
          det_ad_notary_fees_box_div.className = "det_notary_fees_box";
          var det_ad_notary_fees_span = createNode("span");
          det_ad_notary_fees_span.className = "no_select det_notary_fees_text";
          det_ad_notary_fees_span.innerHTML = "+ ";
          var notary_fees = Math.floor(data[i].PRICE * 0.078);
          det_ad_notary_fees_span.innerHTML += formatPrice(String(notary_fees));
          det_ad_notary_fees_span.innerHTML += " € env. de frais de notaires"
          det_ad_notary_fees_box_div.appendChild(det_ad_notary_fees_span);
          det_ad_notary_fees_frame_div.appendChild(det_ad_notary_fees_box_div);
      }

      ad_link.appendChild(det_ad_notary_fees_frame_div);

      /* populate announce text description if present */
      var det_ad_text_description_frame = createNode("div");
      det_ad_text_description_frame.className = "announce_text_description_frame";



      if( data[i].hasOwnProperty('AD_TEXT_DESCRIPTION'))
      {
        var det_ad_text_description_header = createNode("div");
        det_ad_text_description_header.className = "announce_text_description_header";
        var det_ad_text_description_header_text = createNode("span");
        det_ad_text_description_header_text.className = "no_select announce_text_description_header_text";
        det_ad_text_description_header_text.style.fontWeight = "bold";
        det_ad_text_description_header_text.innerHTML = "Description";

        det_ad_text_description_header.appendChild(det_ad_text_description_header_text);
        det_ad_text_description_frame.appendChild(det_ad_text_description_header);

        var det_ad_text_description_container = createNode("div");
        det_ad_text_description_container.className = "announce_text_description_container";
        var det_ad_text_description_container_description = createNode("span");
        det_ad_text_description_container_description.className = "no_select announce_text_description_header_text";
        var ad_text_description = data[i].AD_TEXT_DESCRIPTION.slice(0,614);
        det_ad_text_description_container_description.innerHTML = ad_text_description;
        det_ad_text_description_container_description.innerHTML+= " [...]";

        det_ad_text_description_container.appendChild(det_ad_text_description_container_description);
        det_ad_text_description_frame.appendChild(det_ad_text_description_container);
      }




      ad_link.appendChild(det_ad_text_description_frame);


      var det_ad_logo_container_div = createNode("div");
      det_ad_logo_container_div.className = "det_ad_logo_container";
      var det_ad_logo_frame_div = createNode("div");
      det_ad_logo_frame_div.className = "det_ad_logo_frame";

      var det_img_logo = createNode("img");
      det_img_logo.className = "det_ad_logo";
      det_img_logo.src = data[i].SOURCE_LOGO;
      det_img_logo.alt = "origine de l'annonce immobiliere";

      det_ad_logo_frame_div.appendChild(det_img_logo);
      det_ad_logo_container_div.appendChild(det_ad_logo_frame_div);

      ad_link.appendChild(det_ad_logo_container_div);

      det_ad_container_div.appendChild(ad_link);

      det_container_div.appendChild(det_ad_container_div);
      if(i%2 == 0 || i == (data.length - 1) )
      main_content.appendChild(det_container_div);
    } //end of for loop

    var nb_similar = document.getElementById("nb_announces_found");
    var isFlat = false;
    if( data[0].hasOwnProperty('PROPERTY_TYPE'))
      isFlat = data[0].PROPERTY_TYPE == "Appartement";

    if( data.length == 1 )
    {
      nb_similar.innerHTML = "1 annonce pour ";

      nb_similar.innerHTML += isFlat ? "cet appartement ":"cette maison ";
    }
    else {

        nb_similar.innerHTML = data.length;
        nb_similar.innerHTML += " annonces correspondent à";

        nb_similar.innerHTML += isFlat? " ce même appartement ":" cette même maison ";
    }
    nb_similar.innerHTML += "de ";
    nb_similar.innerHTML += nb_room;
    nb_similar.innerHTML += " pièce(s) à ";

    var pagetitle = "";

    if( data[0].hasOwnProperty('PROPERTY_TYPE'))
    pagetitle += data[0].PROPERTY_TYPE == "Appartement"? "Appartement ":"Maison ";
    pagetitle += nb_room;
    pagetitle += " pièce(s) à ";
    var isForSale = false;
    if(data[0].hasOwnProperty('SEARCH_TYPE'))
    {
      pagetitle += data[0].SEARCH_TYPE == "For sale"? "vendre ":"louer ";
      isForSale = data[0].SEARCH_TYPE == "For sale";
    }
    var postalcode = ""
    if( data[0].hasOwnProperty('CITY'))
    {
      var cityname = data[0].CITY[0].toUpperCase() + data[0].CITY.slice(1);
      nb_similar.innerHTML += cityname;
      postalcode = getPostalCode(cityname);
      nb_similar.innerHTML += " (" + postalcode + ")";
      pagetitle += cityname;
      pagetitle += " (" + postalcode + ")";

      if(isForSale)
      {
        if( data[0].hasOwnProperty('MEDIAN_PRICE_BY_M2') )
        {
          var medianpricebox = document.getElementById("medianprice_box");
          medianpricebox.className = "row mx-auto det_median_price_box";
          var medianpricetext = document.getElementById("medianprice_text");
          medianpricetext.className = "det_median_price_text";
          medianpricetext.innerHTML = "Prix au m<sup>2</sup> médian ";
          if( data[0].hasOwnProperty('PROPERTY_TYPE'))
          medianpricetext.innerHTML += data[0].PROPERTY_TYPE == "Appartement"? "d'un appartement ":"d'une maison ";

          medianpricetext.innerHTML += "à " + cityname + " (" + postalcode + "): ";

          medianpricetext.innerHTML += formatPrice(data[0].MEDIAN_PRICE_BY_M2) + " €/m<sup>2</sup>";
        }
      }

      /* complete the page with city description */
      var city_description_text_header = document.getElementById("city_description_text_header");
      city_description_text_header.innerHTML = "En savoir plus sur ";
      city_description_text_header.innerHTML += cityname + " (" + postalcode + ")";
      const city_description_endpoint =  'https://surfyn.fr:7878/city_info?city='+ data[0].CITY;
      //const city_description_endpoint = 'http://127.0.0.1:7878/city_info?city='+ data[0].CITY;
      fetch(city_description_endpoint)
      .then(function(resp) { return resp.json(); } )
      .then(function(data) {

        if(data.hasOwnProperty("DESCRIPTION"))
        {
          var city_description_text = document.getElementById("city_description_text");
          city_description_text.innerHTML = data.DESCRIPTION;
        }
        if(data.hasOwnProperty("TRANSPORTATION"))
        {
          var city_transportation = document.getElementById("city_transportation");
          city_transportation.innerHTML = data.TRANSPORTATION;
        }
        if(data.hasOwnProperty("BIKE"))
        {
          var city_bike = document.getElementById("city_bike");
          city_bike.innerHTML = data.BIKE;
        }
        if(data.hasOwnProperty("GREEN_SPACE"))
        {
          var city_green_space = document.getElementById("city_green_space");
          city_green_space.innerHTML = data.GREEN_SPACE;
        }
        if(data.hasOwnProperty("RESTAURANT"))
        {
          var city_restaurant = document.getElementById("city_restaurant");
          city_restaurant.innerHTML = data.RESTAURANT;
        }
        if(data.hasOwnProperty("SCHOOL"))
        {
          var city_school = document.getElementById("city_school");
          city_school.innerHTML = data.SCHOOL;
        }
        if(data.hasOwnProperty("HOSPITAL"))
        {
          var city_hospital = document.getElementById("city_hospital");
          city_hospital.innerHTML = data.HOSPITAL;
        }
        if(data.hasOwnProperty("SHOPPING"))
        {
          var city_shopping = document.getElementById("city_shopping");
          city_shopping.innerHTML = data.SHOPPING;
        }
        if(data.hasOwnProperty("CINEMA"))
        {
          var city_cinema = document.getElementById("city_cinema");
          city_cinema.innerHTML = data.CINEMA;
        }
        var city_description_container = document.getElementById("city_description_container");
        city_description_container.style.display="flex";

      })
      .catch(function(error) {

        console.log(error);
      });
    }
    pagetitle += " - Surfyn"

    document.title = pagetitle;
    document.getElementsByTagName('meta')["description"].content = pagetitle;

    if(postalcode != "" && isForSale)
    {
      var dept_code = postalcode.slice(0,2);
      if( dept_code == "92")
      {
         var sf_estimate_link_container = createNode("div");
         sf_estimate_link_container.className = "sf_estimate_link_container";

         var estimate_link_icon = createNode("i");
         estimate_link_icon.className = "fas fa-calculator sf_estimate_link_icon";

         sf_estimate_link_container.appendChild(estimate_link_icon);

         var text = createNode("strong");
         text.innerHTML = "Estimer ce bien";
         var estimate_link_text = createNode("span");
         estimate_link_text.className = "sf_estimate_link_text";
         estimate_link_text.appendChild(text);

         sf_estimate_link_container.appendChild(estimate_link_text);

         var sf_estimate_link = createNode("a");
         sf_estimate_link.className = "sf_estimate_link";
         sf_estimate_link.href = "estimation-immobiliere-en-ligne.html";

         sf_estimate_link.appendChild(sf_estimate_link_container);

         var sf_estimate_link_frame = document.getElementById("estimate_link_frame");
         sf_estimate_link_frame.className = "sf_estimate_link_frame";
         sf_estimate_link_frame.appendChild(sf_estimate_link);

      }
    }


    /* populating visit file download link */
    var sf_visit_file_link_container = createNode("div");
    sf_visit_file_link_container.className = "sf_estimate_link_container";

    var visit_file_link_icon = createNode("i");
    visit_file_link_icon.className = "far fa-file-alt sf_estimate_link_icon";

    sf_visit_file_link_container.appendChild(visit_file_link_icon);

    var text = createNode("strong");
    text.innerHTML = "Fiche de visite pour ";
    if(isFlat)
      text.innerHTML += "cet appartement";
      else {
        text.innerHTML += "cette maison";
      }
    var visit_file_link_text = createNode("span");
    visit_file_link_text.className = "sf_estimate_link_text";
    visit_file_link_text.appendChild(text);

    sf_visit_file_link_container.appendChild(visit_file_link_text);

    var sf_visit_file_link = createNode("a");
    sf_visit_file_link.className = "sf_visit_file_link";
    if(isForSale)
    sf_visit_file_link.href = "data/docs/fiche-visite-achat-appartement-maison.pdf";
    else {
      sf_visit_file_link.href = "data/docs/fiche-visite-location-appartement-maison.pdf";
    }

    sf_visit_file_link.appendChild(sf_visit_file_link_container);

    var sf_visit_file_link_frame = document.getElementById("visit_file_link_frame");
    sf_visit_file_link_frame.className = "sf_estimate_link_frame";
    sf_visit_file_link_frame.appendChild(sf_visit_file_link);


  } //end of else if(data.length == 0 )

  var forPuppeteer = document.createElement("div");
  forPuppeteer.setAttribute("id", "target");
  forPuppeteer.style.visibility = 'hidden';
  main_content.appendChild(forPuppeteer);

  var facebook_icon = document.getElementById("facebook-icon");
  facebook_icon.style.color = "white";
  var twitter_icon = document.getElementById("twitter-icon");
  twitter_icon.style.color = "white";
  var instagram_icon = document.getElementById("instagram-icon");
  instagram_icon.style.color = "white";
  var linkedin_icon = document.getElementById("linkedin-icon");
  linkedin_icon.style.color = "white";
  var youtube_icon = document.getElementById("youtube-icon");
  youtube_icon.style.color = "white";
}
