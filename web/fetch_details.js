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


  //const url = 'http://surfyn.xyz/search/ad?id='+ window.location.search.substr(1);
  const url = 'http://127.0.0.1:7878/search/ad?id='+ window.location.search.substr(1);


  fetch(url)
  .then(function(resp) { return resp.json(); } )
  .then(function(data) {

    generate_details_page(data);

  })
  .catch(function(error) {
    var id = window.location.search.substr(1);
    const localurl = "data/announces_details" + id + ".json";
    loadJSON(localurl,
    function (data) { generate_details_page(JSON.parse(data.response));}, function(err) {console.log(err);})
    console.log(error);
  });


function generate_details_page(data)
{

  if(data.length == 0)
  {
    var announces_found = document.getElementById("nb_announces_found");
    announces_found.innerHTML = " Nous avons trouvé pour vous aucune announce ";

    var text = createNode("p");

    text.innerHTML = " Aucune annonce ne correspond à vos critères ";

    document.body.appendChild(text);
  }
  else
  {
    var nb_similar = document.getElementById("nb_announces_found");
    nb_similar.innerHTML = "Nous avons trouvé pour vous " + data.length + " annonce(s) correspondant à ce même bien";

    var ismobile   = /Android|webOS|iPhone|iPad|iPod|BlackBerry/i.test(navigator.userAgent);
    var det_container_div;
    for(var i = 0; i < data.length; i++)
    {
      if(i%2 == 0 )
      {
        det_container_div = createNode("div");
        det_container_div.className = "det_container";
      }

      var det_ad_container_div = createNode("div");
      det_ad_container_div.className = "det_ad_container";

      if(i%2 != 0 )
      det_ad_container_div.style.marginLeft = ismobile ? "20px" : "70px";

      var ad_link = createNode("a");
      ad_link.style.display = "block";
      ad_link.href = data[i].ANNOUNCE_LINK;

      var det_ad_image_container_div = createNode("div");
      det_ad_image_container_div.className = "det_ad_image_container";

      var det_ad_image_frame_div = createNode("div");
      det_ad_image_frame_div.className = "det_ad_image_frame";

      var det_ad_img = createNode("img");
      det_ad_img.className = "det_ad_image";
      det_ad_img.src = data[i].IMAGE;
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

      if(data[i].hasOwnProperty('SURFACE'))
      {
        count_desc++;

        var det_surface_container_div = createNode("div");
        det_surface_container_div.className = "det_ad_desc_shape det_ad_desc_border det_ad_desc_center";

        var det_surface_span = createNode("span");
        det_surface_span.className = "no_select";
        det_surface_span.style.fontWeight ="bold";
        det_surface_span.style.color = "#4C8E99";
        if(ismobile)
        det_surface_span.style.fontSize = "12px";
        det_surface_span.innerHTML = data[i].SURFACE;

        det_surface_container_div.appendChild(det_surface_span);
        det_desc_container_div.appendChild(det_surface_container_div);
      }

      if(data[i].hasOwnProperty('NB_ROOMS'))
      {
        count_desc++;

        var det_nb_rooms_container_div = createNode("div");
        det_nb_rooms_container_div.className = "det_ad_desc_shape det_ad_desc_border det_ad_desc_center";
        var det_nb_rooms_span = createNode("span");
        det_nb_rooms_span.className = "no_select";
        det_nb_rooms_span.style.fontWeight = "bold";
        det_nb_rooms_span.style.color = "#4C8E99";
        if(ismobile)
        det_nb_rooms_span.style.fontSize = "12px";
        det_nb_rooms_span.innerHTML = data[i].NB_ROOMS;

        det_nb_rooms_container_div.appendChild(det_nb_rooms_span);
        det_desc_container_div.appendChild(det_nb_rooms_container_div);
      }

      if(data[i].hasOwnProperty('LOCATION'))
      {
        count_desc++;
        var det_location_container_div = createNode("div");
        det_location_container_div.className = "det_ad_desc_shape det_ad_desc_border";
        var det_location_icon_container_div = createNode("div");
        det_location_icon_container_div.className = "det_ad_desc_center";
        det_location_icon_container_div.style.width = "100%";
        det_location_icon_container_div.style.height = "50%";

        var det_location_icon = createNode("i");
        det_location_icon.className = "fas fa-map-marker-alt";
        det_location_icon.style.color = "#4C8E99";

        det_location_icon_container_div.appendChild(det_location_icon);

        det_location_container_div.appendChild(det_location_icon_container_div);

        var det_location_text_div = createNode("div");
        det_location_text_div.className = "det_ad_desc_center";
        det_location_text_div.style.width = "100%";
        det_location_text_div.style.height = "50%";

        var det_location_text_span = createNode("span");
        det_location_text_span.className = "no_select";
        det_location_text_span.style.fontWeight = "bold";
        det_location_text_span.style.color = "#4C8E99";
        if(ismobile)
        det_location_text_span.style.fontSize = "12px";
        det_location_text_span.innerHTML = "Position";

        det_location_text_div.appendChild(det_location_text_span);
        det_location_container_div.appendChild(det_location_text_div);

        det_desc_container_div.appendChild(det_location_container_div);

      }

      if(data[i].hasOwnProperty('FLOOR'))
      {
        count_desc++;

        var det_ad_floor_div = createNode("div");

        det_ad_floor_div.className = "det_ad_desc_shape det_ad_desc_border det_ad_desc_center";
        var det_ad_floor_span = createNode("span");
        det_ad_floor_span.className = "no_select";
        det_ad_floor_span.style.fontWeight = "bold";
        det_ad_floor_span.style.color = "#4C8E99";
        det_ad_floor_span.style.marginLeft = "10px";
        if(ismobile)
        det_ad_floor_span.style.fontSize = "12px";
        det_ad_floor_span.innerHTML = data[i].FLOOR;

        det_ad_floor_div.appendChild(det_ad_floor_span);

        det_desc_container_div.appendChild(det_ad_floor_div);
      }

      if(data[i].hasOwnProperty('CELLAR'))
      {
        count_desc++;
        var det_ad_cellar_div = createNode("div");
        det_ad_cellar_div.className = "det_ad_desc_shape det_ad_desc_border det_ad_desc_center";
        var det_ad_cellar_span = createNode("span");
        det_ad_cellar_span.className = "no_select";
        det_ad_cellar_span.style.fontWeight = "bold";
        det_ad_cellar_span.style.color = "#4C8E99";
        if(ismobile)
        det_ad_cellar_span.style.fontSize = "12px";
        det_ad_cellar_span.innerHTML = data[i].CELLAR;

        det_ad_cellar_div.appendChild(det_ad_cellar_span);

        det_desc_container_div.appendChild(det_ad_cellar_div);
      }
      for( var j = 0 ; j < (9 - count_desc); j++)
      {
        var div = createNode("div");
        div.className = "det_ad_desc_shape";

        det_desc_container_div.appendChild(div);
      }

      ad_link.appendChild(det_desc_container_div);

      var det_ad_logo_container_div = createNode("div");
      det_ad_logo_container_div.className = "det_ad_logo_container";
      var det_ad_logo_frame_div = createNode("div");
      det_ad_logo_frame_div.className = "det_ad_logo_frame";

      var det_img_logo = createNode("img");
      det_img_logo.className = "det_ad_logo";
      det_img_logo.src = data[i].SOURCE_LOGO;

      det_ad_logo_frame_div.appendChild(det_img_logo);
      det_ad_logo_container_div.appendChild(det_ad_logo_frame_div);

      ad_link.appendChild(det_ad_logo_container_div);

      det_ad_container_div.appendChild(ad_link);

      det_container_div.appendChild(det_ad_container_div);
      if(i%2 == 0 || i == (data.length - 1) )
      document.body.appendChild(det_container_div);
    }
  }
}
