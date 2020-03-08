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

  function SelogerProcessor(ad_sum_src_provider)
  {
    ad_sum_src_provider.style.border = "2px solid red";
    var ad_sum_src_provider_text = createNode("p");
    ad_sum_src_provider_text.className = "ad_summary_src_provider_text";
    ad_sum_src_provider_text.style.color ="red";
    ad_sum_src_provider_text.innerHTML ="SeLoger";
    ad_sum_src_provider.appendChild(ad_sum_src_provider_text);
  }
  function LeboncoinProcessor(ad_sum_src_provider)
  {
    ad_sum_src_provider.style.border = "2px solid orange";
    var ad_sum_src_provider_text = createNode("div");
    ad_sum_src_provider_text.className = "ad_summary_src_provider_text";
    ad_sum_src_provider_text.style.color ="orange";
    ad_sum_src_provider_text.innerHTML ="Leboncoin";
    ad_sum_src_provider.appendChild(ad_sum_src_provider_text);
  }

  function BieniciProcessor(ad_sum_src_provider)
  {
    ad_sum_src_provider.style.border = "2px solid #FFC331";
    var ad_sum_src_provider_text = createNode("div");
    ad_sum_src_provider_text.className = "ad_summary_src_provider_text";
    ad_sum_src_provider_text.style.color ="#FFC331";
    ad_sum_src_provider_text.innerHTML ="Bien'ici";
    ad_sum_src_provider.appendChild(ad_sum_src_provider_text);
  }

  function LaforetProcessor(ad_sum_src_provider)
  {
    ad_sum_src_provider.style.border = "2px solid blue";
    var ad_sum_src_provider_text = createNode("div");
    ad_sum_src_provider_text.className = "ad_summary_src_provider_text";
    ad_sum_src_provider_text.style.color ="blue";
    ad_sum_src_provider_text.innerHTML ="Laforêt";
    ad_sum_src_provider.appendChild(ad_sum_src_provider_text);
  }
  function OrpiProcessor(ad_sum_src_provider)
  {
    ad_sum_src_provider.style.border = "2px solid red";
    var ad_sum_src_provider_text = createNode("div");
    ad_sum_src_provider_text.className = "ad_summary_src_provider_text";
    ad_sum_src_provider_text.style.color ="red";
    ad_sum_src_provider_text.innerHTML ="Orpi";
    ad_sum_src_provider.appendChild(ad_sum_src_provider_text);
  }

  function SPlazaImProcessor(ad_sum_src_provider)
  {
    ad_sum_src_provider.style.border = "2px solid red";
    var ad_sum_src_provider_text = createNode("div");
    ad_sum_src_provider_text.className = "ad_summary_src_provider_text";
    ad_sum_src_provider_text.style.color ="red";
    ad_sum_src_provider_text.innerHTML ="SPlazaIm";
    ad_sum_src_provider.appendChild(ad_sum_src_provider_text);
  }

  function FonciaProcessor(ad_sum_src_provider)
  {
    ad_sum_src_provider.style.border = "2px solid blue";
    var ad_sum_src_provider_text = createNode("div");
    ad_sum_src_provider_text.className = "ad_summary_src_provider_text";
    ad_sum_src_provider_text.style.color ="blue";
    ad_sum_src_provider_text.innerHTML ="Foncia";
    ad_sum_src_provider.appendChild(ad_sum_src_provider_text);
  }
  function Century21Processor(ad_sum_src_provider)
  {
    ad_sum_src_provider.style.border = "2px solid black";
    var ad_sum_src_provider_text = createNode("div");
    ad_sum_src_provider_text.className = "ad_summary_src_provider_text";
    ad_sum_src_provider_text.style.color ="black";
    ad_sum_src_provider_text.innerHTML ="Century21";
    ad_sum_src_provider.appendChild(ad_sum_src_provider_text);
  }
  function GuyHoquetProcessor(ad_sum_src_provider)
  {
    ad_sum_src_provider.style.border = "2px solid blue";
    var ad_sum_src_provider_text = createNode("div");
    ad_sum_src_provider_text.className = "ad_summary_src_provider_text";
    ad_sum_src_provider_text.style.color ="blue";
    ad_sum_src_provider_text.innerHTML ="Guyhoquet";
    ad_sum_src_provider.appendChild(ad_sum_src_provider_text);
  }

  function ArthurImmoProcessor(ad_sum_src_provider)
  {
    ad_sum_src_provider.style.border = "2px solid #FFC331";
    var ad_sum_src_provider_text = createNode("div");
    ad_sum_src_provider_text.className = "ad_summary_src_provider_text";
    ad_sum_src_provider_text.style.color ="#FFC331";
    ad_sum_src_provider_text.innerHTML ="Arthurimm";
    ad_sum_src_provider.appendChild(ad_sum_src_provider_text);
  }

  function ByTimeStamp(lhs, rhs)
  {
    if( lhs.hasOwnProperty("FIRST_TIMESTAMP") && rhs.hasOwnProperty("FIRST_TIMESTAMP"))
    {
      var d1 = new Date(lhs.FIRST_TIMESTAMP);
      var d2 = new Date(rhs.FIRST_TIMESTAMP);
      return d2 - d1; //descending order
    }
    else {
      var d1 = new Date(lhs.TIMESTAMP);
      var d2 = new Date(rhs.TIMESTAMP);
      return d2 - d1; //descending order
    }
  }

  var ProcessorBySource = new Map();
  ProcessorBySource.set("seloger", SelogerProcessor);
  ProcessorBySource.set("leboncoin", LeboncoinProcessor);
  ProcessorBySource.set("bienici", BieniciProcessor);
  ProcessorBySource.set("laforet", LaforetProcessor);
  ProcessorBySource.set("orpi", OrpiProcessor);
  ProcessorBySource.set("stephaneplazaimo", SPlazaImProcessor);
  ProcessorBySource.set("foncia", FonciaProcessor);
  ProcessorBySource.set("century21", Century21Processor);
  ProcessorBySource.set("guyhoquet", GuyHoquetProcessor);
  ProcessorBySource.set("arthurimmo", ArthurImmoProcessor);


  const url = 'https://surfyn.fr:7878/search/all'+ window.location.search;
  //const url = 'http://127.0.0.1:7878/search/all'+ window.location.search;

  var summary_json_data = sessionStorage.getItem("summary_json_data");
  var needtosort = sessionStorage.getItem("needtosort");
  if(needtosort && summary_json_data)
  {
    var newdata = JSON.parse(summary_json_data);
    if(needtosort == "byprice")
    {
      newdata.sort(function(lhs, rhs)
          {
            return lhs.PRICE - rhs.PRICE;
          });
    }
    else
    {
      if(needtosort == "bysurface")
      {
        newdata.sort(function(lhs, rhs)
            {
              return lhs.SURFACE - rhs.SURFACE;
            });
      }
      else{
        if( needtosort == "bydate")
        {
          newdata.sort(ByTimeStamp);
        }
      }
    }

    generate_summary_page(newdata);
  }
  else {
    fetch(url)
    .then(function(resp) { return resp.json(); } )
    .then(function(data) {
      data.sort(ByTimeStamp);
      generate_summary_page(data);
      sessionStorage.setItem("summary_json_data", JSON.stringify(data));
    })
    .catch(function(error) {
      loadJSON("data/announces_summary.json",
      function (data) { generate_summary_page(JSON.parse(data.response));}, function(err) {console.log(err);})
      console.log(error);
    });
  }

  function reverse(s){
      return s.split("").reverse().join("");
  }

  function formatPrice(p)
  {
    var val = reverse(p);

    var tmp = val.replace(/(.{3})/g,"$1 ");

    return reverse(tmp);
  }


var ismobile   = /Android|webOS|iPhone|iPad|iPod|BlackBerry/i.test(navigator.userAgent);

function generate_summary_page(data)
{
  const Params = new URLSearchParams(window.location.search);
  const searchType = Params.get('search_type');
  var pagetitle = searchType == 1 ? "Vente ": "Location ";
  var propertyType_param = Params.get('prop_type');
  var split_propertyType = propertyType_param.split(",");
  var first_proptype = true;
  var propertyType = "";
  if (split_propertyType.indexOf("1") !== -1)
  {
    if(!first_proptype)
      pagetitle += ", ";
      else {
        first_proptype = false;
      }
    propertyType = "appartements";
  }
  if (split_propertyType.indexOf("2") !== -1)
  {
    if(!first_proptype)
      propertyType += ", ";
      else {
        first_proptype = false;
      }
    propertyType += "maisons";
  }
  pagetitle += propertyType;
  pagetitle += " à ";
  var search_city = Params.get('search_city');
  pagetitle += search_city;
  document.title = pagetitle;


  var announces_found = document.getElementById("nb_announces_found");
  if( data.length == 0)
  {
    announces_found.innerHTML = "Aucune annonce à ";
    announces_found.innerHTML += search_city;
    announces_found.innerHTML += " ne correspond à vos critères";
  }
  else
  {
        announces_found.innerHTML = data.length
        announces_found.innerHTML += data.length == 1 ? " annonce ": " annonces ";
        announces_found.innerHTML += split_propertyType.indexOf("1") !== -1? "d'": "de "
        announces_found.innerHTML += propertyType + " à ";
        announces_found.innerHTML += searchType == 1 ? "vendre à " : "louer à ";
        announces_found.innerHTML += search_city;
        for(var i = 0; i < data.length; i++)
        {
             var ad_link = createNode("a");
             ad_link.style.display = "block";
             ad_link.href = "annonce_detaille.html?"+data[i].ID;

             var ad_div = createNode("div");
             ad_div.className = "row mx-auto announce_frame";

             var ad_container_div = createNode("div");
             ad_container_div.className = "announce_container";


             var ad_image_container_div = createNode("div");
             ad_image_container_div.className = "announce_image_container";

             var ad_image = createNode("img");
             ad_image.src = data[i].hasOwnProperty('IMAGE')? data[i].IMAGE: "logo.svg";
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

             if(data[i].hasOwnProperty('SURFACE'))
             {
               ad_summary_details_p.innerHTML += data[i].SURFACE;
               ad_summary_details_p.innerHTML += " m<sup>2";
             }

             if(data[i].hasOwnProperty('ROOMS'))
             {
               ad_summary_details_p.innerHTML += " - ";
               ad_summary_details_p.innerHTML += data[i].ROOMS;
               ad_summary_details_p.innerHTML += " pièce(s)";
             }

             ad_summary_desc_div1.appendChild(ad_summary_details_p);

             //var ad_summary_refresh_time_p = createNode("p");
             //ad_summary_refresh_time_p.className = "announce_summary_refresh_time";
             //ad_summary_refresh_time_p.innerHTML = data[i].HISTORY;
             //ad_summary_refresh_time_p.innerHTML = "il y a 1h";

             //ad_summary_desc_div1.appendChild(ad_summary_refresh_time_p);

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
            ad_price_p.style.color = "#4c94bc";
            ad_price_p.innerHTML = formatPrice(String(data[i].PRICE));
            ad_price_p.innerHTML += " €";
            ad_summary_desc_div2.appendChild(ad_price_p);

            ad_summary_container_div.appendChild(ad_summary_desc_div2);

            var ad_source_frame_div = createNode("div");
            ad_source_frame_div.className = "ad_summary_src";

            var ad_source_div = createNode("div");
            ad_source_div.className = "ad_summary_src_text_cont";

            var ad_sum_src_text = createNode("p");
            ad_sum_src_text.className = "ad_summary_src_text";
            ad_sum_src_text.innerHTML = "Ce bien apparaît sur:";
            ad_source_div.appendChild(ad_sum_src_text);

            ad_source_frame_div.appendChild(ad_source_div);

            var added_src =
            {
            seloger:false,
            logicimmo:false,
            leboncoin: false,
            bienici: false,
            laforet:false,
            orpi:false,
            stephaneplazaimo:false,
            foncia:false,
            century21:false,
            guyhoquet:false,
            arthurimmo:false
            }
            var ad_sum_src_provider_cont = createNode("div");
            ad_sum_src_provider_cont.className = "ad_summary_src_provider_cont";
            var count_added_src = 0;
            for(var j = 0; j < data[i].SOURCES.length; j++)
            {
              if(added_src[data[i].SOURCES[j]] == false )
              {
                if(!ismobile || count_added_src < 3)
                {
                  var ad_sum_src_provider = createNode("div");
                  ad_sum_src_provider.className = "ad_summary_src_provider";

                  if( ProcessorBySource.has(data[i].SOURCES[j]))
                  {
                    var func = ProcessorBySource.get(data[i].SOURCES[j])
                    func(ad_sum_src_provider);
                  }

                  ad_sum_src_provider_cont.appendChild(ad_sum_src_provider);
                  added_src[data[i].SOURCES[j]] = true;
                }

                count_added_src++;
              }
            }
            if( ismobile && count_added_src > 3)
            {
              var ad_sum_src_provider = createNode("div");
              ad_sum_src_provider.className = "ad_summary_src_provider";

              ad_sum_src_provider.style.border = "2px solid #4c94bc";
              ad_sum_src_provider.style.width = "15%";
              var ad_sum_src_provider_text = createNode("div");
              ad_sum_src_provider_text.className = "ad_summary_src_provider_text";
              ad_sum_src_provider_text.style.color ="#4c94bc";
              ad_sum_src_provider_text.innerHTML ="...";
              ad_sum_src_provider.appendChild(ad_sum_src_provider_text);

              ad_sum_src_provider_cont.appendChild(ad_sum_src_provider);
            }

            ad_source_frame_div.appendChild(ad_sum_src_provider_cont);

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
