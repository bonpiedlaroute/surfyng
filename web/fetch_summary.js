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


  //const url = 'http://surfyn.xyz/search/all'+ window.location.search;
  const url = 'http://127.0.0.1:7878/search/all'+ window.location.search;


  fetch(url)
  .then(function(resp) { return resp.json(); } )
  .then(function(data) {
    console.log("success of fetch");
    generate_summary_page(data);

  })
  .catch(function(error) {
    loadJSON("data/announces_summary.json",
    function (data) { generate_summary_page(JSON.parse(data.response));}, function(err) {console.log(err);})
    console.log(error);
  });

  function generate_summary_page(data)
  {
    console.log("in generate_summary_page");
    if( data.length == 0)
    {
      console.log("no data");
      var announces_found = document.getElementById("nb_announces_found");
      announces_found.innerHTML = "(0)";

      var text = createNode("p");

      text.innerHTML = " Aucune annonce ne correspond à vos critères ";

      document.body.appendChild(text);
    }
    else
    {
          console.log("there is " +data.length + "data" );
          var announces_found = document.getElementById("nb_announces_found");
          announces_found.innerHTML = "("+ data.length +")";
          for(var i = 0; i < data.length; i++)
          {
               var ad_link = createNode("a");
               ad_link.style.display = "block";
               ad_link.href = "results_details.html?"+data[i].ID;

               var ad_div = createNode("div");
               ad_div.style.height = "200px";
               ad_div.className = "row mx-auto";

               var ad_container_div = createNode("div");
               ad_container_div.className = "announce_container";


               var ad_image_container_div = createNode("div");
               ad_image_container_div.className = "announce_image_container";

               var ad_image = createNode("img");
               ad_image.src = data[i].IMAGE;
               ad_image.className = "announce_image";

               ad_image_container_div.appendChild(ad_image);

               var ad_summary_container_div = createNode("div");
               ad_summary_container_div.className = "announce_summary_container";

               var ad_summary_desc_div1 = createNode("div");
               ad_summary_desc_div1.className = "announce_summary_desc";

               var ad_summary_details_p = createNode("p");
               ad_summary_details_p.className = "announce_summary_details";
               ad_summary_details_p.innerHTML = data[i].PROPERTY_TYPE;
               ad_summary_details_p.innerHTML += "<br/>";
               ad_summary_details_p.innerHTML += data[i].SURFACE;
               ad_summary_details_p.innerHTML += " - ";
               ad_summary_details_p.innerHTML += data[i].NB_ROOMS;

               ad_summary_desc_div1.appendChild(ad_summary_details_p);

               var ad_summary_refresh_time_p = createNode("p");
               ad_summary_refresh_time_p.className = "announce_summary_refresh_time";
               ad_summary_refresh_time_p.innerHTML = data[i].HISTORY;

               ad_summary_desc_div1.appendChild(ad_summary_refresh_time_p);

               ad_summary_container_div.appendChild(ad_summary_desc_div1);

               var ad_summary_desc_div2 = createNode("div");
               ad_summary_desc_div2.className = "announce_summary_desc";

              var ad_icon = createNode("i");
              ad_icon.className = "fas fa-map-marker-alt announce_summary_icon";
              ad_summary_desc_div2.appendChild(ad_icon);

              var ad_summary_city_p = createNode("p");
              ad_summary_city_p.className = "announce_summary_city";
              ad_summary_city_p.innerHTML = data[i].CITY;
              ad_summary_desc_div2.appendChild(ad_summary_city_p);

              var ad_price_p = createNode("p");
              ad_price_p.style.float = "right";
              ad_price_p.innerHTML = data[i].PRICE;
              ad_summary_desc_div2.appendChild(ad_price_p);

              ad_summary_container_div.appendChild(ad_summary_desc_div2);

              var ad_source_frame_div = createNode("div");
              ad_source_frame_div.style.height = "63px";
              ad_source_frame_div.style.width = "100%";

              var ad_source_div = createNode("div");
              ad_source_div.className = "announce_source"

              var img_url =
              {
              seloger:"data/SL0.svg",
              logicimmo:"data/li0.svg",
              leboncoin: "data/lbc0.svg"
              }
              var max_offset = 40;
              var max_zindex = 10;
              for(var j = 0; j < data[i].SOURCES.length; j++)
              {
                var ad_source_container_div = createNode("div");
                ad_source_container_div.className = "announce_source_container";
                var new_offset = max_offset - j * 20;
                ad_source_container_div.style.right = new_offset + "px";
                var new_zindex = max_zindex - j;
                ad_source_container_div.style.zIndex = new_zindex;

                var img_logo = createNode("img");
                img_logo.src = img_url[data[i].SOURCES[j]];
                img_logo.className = "announce_source_logo";
                ad_source_container_div.appendChild(img_logo);

                ad_source_div.appendChild(ad_source_container_div);
              }

              ad_source_frame_div.appendChild(ad_source_div);

              ad_summary_container_div.appendChild(ad_source_frame_div);

              ad_container_div.appendChild(ad_image_container_div);
              ad_container_div.appendChild(ad_summary_container_div);

              ad_div.appendChild(ad_container_div);

              ad_link.appendChild(ad_div);

              document.body.appendChild(ad_link);
              if( i != data.length - 1)
              {
                var line_div = createNode("div");
                line_div.className = "sf_line_results row mx-auto";
                document.body.appendChild(line_div);
              }
          }
    }
  }
