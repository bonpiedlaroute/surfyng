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


  function SrcProcessor(ad_sum_src_provider, color, text)
  {
    var border = "2px solid ";
    border += color;
    ad_sum_src_provider.style.border = border;
    var ad_sum_src_provider_text = createNode("div");
    ad_sum_src_provider_text.className = "ad_summary_src_provider_text";
    ad_sum_src_provider_text.style.color = color;
    ad_sum_src_provider_text.innerHTML = text;
    ad_sum_src_provider.appendChild(ad_sum_src_provider_text);
  }

  function ByTimeStamp(lhs, rhs)
  {
    var d1, d2;
    if(!lhs.hasOwnProperty("FIRST_TIMESTAMP"))
    {
      d1 = new Date(lhs.TIMESTAMP);
    }
    else {
      d1 = new Date(lhs.FIRST_TIMESTAMP);
    }

    if(!rhs.hasOwnProperty("FIRST_TIMESTAMP"))
    {
      d2 = new Date(rhs.TIMESTAMP);
    }
    else {
      d2 = new Date(rhs.FIRST_TIMESTAMP);
    }

    return d2 - d1; //descending order
  }

  var ProcessorBySource = new Map();
  ProcessorBySource.set("seloger", function(param) { SrcProcessor(param, "red", "SeLoger"); });
  ProcessorBySource.set("leboncoin", function(param) { SrcProcessor(param, "orange", "Leboncoin"); });
  ProcessorBySource.set("bienici", function(param) { SrcProcessor(param, "#FFC331", "Bien'ici"); });
  ProcessorBySource.set("laforet", function(param) { SrcProcessor(param, "blue", "Laforêt"); });
  ProcessorBySource.set("orpi", function(param) { SrcProcessor(param, "red", "Orpi"); });
  ProcessorBySource.set("stephaneplazaimo", function(param) { SrcProcessor(param, "red", "SPlazaIm"); });
  ProcessorBySource.set("foncia", function(param) { SrcProcessor(param, "blue", "Foncia"); });
  ProcessorBySource.set("century21", function(param) { SrcProcessor(param, "black", "Century21"); });
  ProcessorBySource.set("guyhoquet", function(param) { SrcProcessor(param, "blue", "Guyhoquet"); });
  ProcessorBySource.set("arthurimmo", function(param) { SrcProcessor(param, "#FFC331", "Arthurimm"); });
  ProcessorBySource.set("logicimmo", function(param) { SrcProcessor(param, "black", "Logicimmo"); });
  ProcessorBySource.set("eraimmo", function(param) { SrcProcessor(param, "red", "Eraimmo"); });
  ProcessorBySource.set("pap", function(param) { SrcProcessor(param, "blue", "PAP"); });
  ProcessorBySource.set("iadfrance", function(param) { SrcProcessor(param, "blue", "i@d"); });
  ProcessorBySource.set("paruvendu", function(param) { SrcProcessor(param, "black", "Paruvendu"); });
  ProcessorBySource.set("avendrealouer", function(param) { SrcProcessor(param, "#27CCC3", "AvdreAlou"); });
  ProcessorBySource.set("nestenn", function(param) { SrcProcessor(param, "#27CCC3", "nestenn"); });
  ProcessorBySource.set("agenceprincipale", function(param) { SrcProcessor(param, "#FFC331", "Agenprinc"); });
  ProcessorBySource.set("lefigaroimmobilier", function(param) { SrcProcessor(param, "red", "Figaroimm"); });
  ProcessorBySource.set("etreproprio", function(param) { SrcProcessor(param, "green", "Etrepropr"); });
  ProcessorBySource.set("efficity", function(param) {SrcProcessor(param, "green", "Efficity"); });
  ProcessorBySource.set("fnaim", function(param) {SrcProcessor(param, "yellow", "Fnaim"); });

  var url = 'https://surfyn.fr:7878/search/all';
  //var url = 'http://127.0.0.1:7878/search/all';
  var url_params = '?';

function buildurlparams()
{
  var pathname = window.location.pathname.split("/").slice(1);

  var s_city = pathname[3].split("-");

  if( s_city[0].toUpperCase() in postalCodeByCity && getPostalCode(s_city[0]) == s_city[1])
  {
    var city_param = s_city[0][0].toUpperCase();
    city_param += s_city[0].slice(1);
    url_params += "search_city=" + city_param;
  }
  else {
    return false;
  }
  if(pathname[1] == "achat")
  {
    url_params += "&search_type=1";
  }
  else {
    if(pathname[1] == "location")
    {
      url_params += "&search_type=2";
    }
    else {
      return false;
    }
  }

  if(pathname[2] == "appartements")
  {
    url_params += "&prop_type=1";
  }else {
    if(pathname[2] == "maisons")
    {
      url_params += "&prop_type=2";
    }
    else if( pathname[2] == "appartements-studios")
    {
      url_params += "&prop_type=1";
      url_params += "&rooms=1";
    }else
    {
      return false;
    }
  }

  return true;
}

// Check if div with id #prerendered-page is already on the DOM to
// To prevent re-hydratation
var puppeteer = document.getElementById('prerendered-page');

if (!puppeteer)
{
  if(window.location.search == "")
  {
      if(buildurlparams() == true)
      {
        var rel_link = document.getElementById("canonical_url");
        rel_link.rel = "canonical";
        rel_link.href = window.location.pathname;
        url += url_params;
        buildpage();
      }
      else
      {
        var announces_found = document.getElementById("nb_announces_found");

        announces_found.innerHTML = "Aucune annonce ";
        announces_found.innerHTML += " ne correspond à vos critères";
      }
  }
  else
  {
    url += window.location.search;
    buildpage();
  }
}

function buildpage()
{
  var summary_json_data = "";
  if (puppeteer)
  {
    summary_json_data = puppeteer.getAttribute("data-ad-json");
  }

  if(summary_json_data)
  {
    var newdata = JSON.parse(summary_json_data);
    generate_summary_page(newdata);
  }
  else {
    fetch(url)
    .then(function(resp) { return resp.json(); } )
    .then(function(data) {
      data.sort(ByTimeStamp);
      generate_summary_page(data);
      //sessionStorage.setItem("summary_json_data", JSON.stringify(data));
    })
    .catch(function(error) {
      loadJSON("data/announces_summary.json",
      function (data) { generate_summary_page(JSON.parse(data.response));}, function(err) {console.log(err);})
      console.log(error);
    });
  }
}

function addGoogleAds(main_content)
{
  var ad_link = createNode("div");
  ad_link.className = "announce_link";

  var ins = createNode("ins");
  ins.className = "adsbygoogle"
  ins.style.display ="block";
  ins.setAttribute('data-ad-format', "fluid");
  ins.setAttribute('data-ad-layout-key', "-fb+5w+4e-db+86");
  ins.setAttribute('data-ad-client', "ca-pub-3950735685733517");
  ins.setAttribute('data-ad-slot', "2808340986");

  ad_link.append(ins);
  var script = createNode("script");
  script.type ="text/javascript"
  script.innerHTML = "var isPageAlreadyRendered = document.getElementById(\"prerendered-page\"); if(isPageAlreadyRendered) {(adsbygoogle = window.adsbygoogle || []).push({});}";

  ad_link.append(script);

  main_content.append(ad_link);

  var separator = createNode("div");
  separator.className = "sf_line_results row mx-auto";
  main_content.append(separator);
}



var ismobile   = /Android|webOS|iPhone|iPad|iPod|BlackBerry/i.test(navigator.userAgent);

function generate_summary_page(data, empty=false)
{

  if(empty)
  {
      // Delete content of main-content element
      var main_content = document.getElementById("main-content");
      main_content.innerHTML = "";
  }
  else
  {
      var url_parameters = ''
      if(window.location.search == "")
        url_parameters = url_params;
      else
        url_parameters = window.location.search;


    const Params = new URLSearchParams(url_parameters);
    var searchType = Params.get('search_type');
    var pagetitle = "";
    var propertyType_param = Params.get('prop_type');
    var split_propertyType = propertyType_param.split(",");
    var first_proptype = true;
    var propertyType = "";
    var title = "";
    var meta_description = "Toutes les annonces"

    if (split_propertyType.indexOf("1") !== -1 || split_propertyType.indexOf("3") !== -1)
    {
      if(!first_proptype)
        pagetitle += ", ";
        else {
          first_proptype = false;
        }
      propertyType = "appartements";
      meta_description += " d'";
      pagetitle += "Appartement"
    }
    else {
      meta_description += " de "
    }

    if (split_propertyType.indexOf("2") !== -1 || split_propertyType.indexOf("4") !== -1)
    {
      if(!first_proptype)
        propertyType += ", ";
        else {
          first_proptype = false;
        }
      propertyType += "maisons";
      pagetitle += "Maison"
    }
      meta_description+= propertyType;
      var rooms_text = "";
      var rooms = Params.get('rooms');
      if(rooms)
      {
        pagetitle += " ";
        if( rooms == "1")
        {
          pagetitle += "studio";
          rooms_text = " studios";
        }else
        {
            pagetitle += rooms;
            pagetitle += " pièces"
            rooms_text = " " + rooms;
            rooms_text += " pièces";
        }
        meta_description+= rooms_text;

      }
      pagetitle += " à ";
      meta_description += " à ";
      pagetitle += searchType == 1 ? "vendre ": "louer ";
      meta_description += searchType == 1 ? "vendre ": "louer ";
      meta_description += " à ";

      var search_city = Params.get('search_city');
      pagetitle += search_city;
      meta_description += search_city;
      var postalcode = getPostalCode(search_city);
      pagetitle +=" ("+ postalcode + ")";
      meta_description += " ("+ postalcode + ")";
      pagetitle += " - Surfyn";
      meta_description += " actualisées en temps réel, à partir de tous les sites de petites annonces immobilières et d'agences immobilières couvrant la ville de "
      meta_description += search_city;
      meta_description += "  - Surfyn";

      document.title = pagetitle;

      document.getElementsByTagName('meta')["description"].content = meta_description;

      var realestate_agencies_city = document.getElementById('realestate_agencies_city');
      realestate_agencies_city.href = 'agence-immobiliere-'+ search_city.toLowerCase() +'-' + postalcode + '.html';
      realestate_agencies_city.innerHTML = 'Agences immobilières à ' + search_city + " ("+ postalcode + ")";


  var announces_found = document.getElementById("nb_announces_found");
  if( data.length == 0)
  {
    announces_found.innerHTML = "Aucune annonce à ";
    announces_found.innerHTML += search_city;
    announces_found.innerHTML += " ne correspond à vos critères";
  }
  else
  {
    if (!empty)
    {
        announces_found.innerHTML = data.length
        announces_found.innerHTML += data.length == 1 ? " annonce ": " annonces ";
        announces_found.innerHTML += (split_propertyType.indexOf("1") !== -1 || split_propertyType.indexOf("3") !== -1) ? "d'": "de "
        announces_found.innerHTML += propertyType + rooms_text + " à ";
        announces_found.innerHTML += searchType == 1 ? "vendre à " : "louer à ";
        announces_found.innerHTML += search_city;

        announces_found.innerHTML += " (" +postalcode+")";
    }

    var main_content = document.getElementById("main-content");
    var footer = document.getElementsByTagName('footer')[0];
    for(var i = 0; i < data.length; i++)
    {

        var ad_link = createNode("div");
        ad_link.className = "announce_link";
        url_path = "/annonce/";
        url_path += searchType == 1 ? "achat/" : "location/";

        var ad_div = createNode("div");
        ad_div.className = "row mx-auto announce_frame";

        var ad_container_div = createNode("div");
        ad_container_div.className = "announce_container";


        var ad_image_container_div = createNode("div");
        ad_image_container_div.className = "announce_image_container";

        var ad_image = createNode("img");
        ad_image.src = data[i].hasOwnProperty('IMAGE')? data[i].IMAGE: "logo.svg";
        /* filling alt attribute */
        var alt = "";
        alt += data[i].PROPERTY_TYPE + " a ";
        alt += searchType == 1 ? "vendre " : "louer ";
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
        ad_image.alt = alt;
        /*end filling alt attribute */
        ad_image.className = "announce_image";

        ad_image_container_div.appendChild(ad_image);

        var ad_summary_container_div = createNode("div");
        ad_summary_container_div.className = "announce_summary_container";

        var ad_summary_desc_div1 = createNode("div");
        ad_summary_desc_div1.className = "announce_summary_desc";

        var ad_summary_details_h2 = createNode("h2");
        ad_summary_details_h2.className = "announce_summary_details";
        ad_summary_details_h2.innerHTML = data[i].PROPERTY_TYPE;
        ad_summary_details_h2.innerHTML += "<br/>";

        url_path += data[i].PROPERTY_TYPE.toLowerCase();
        if(data[i].hasOwnProperty('SURFACE'))
        {
         ad_summary_details_h2.innerHTML += data[i].SURFACE;
         ad_summary_details_h2.innerHTML += " m<sup>2";
        }

        if(data[i].hasOwnProperty('ROOMS'))
        {
         ad_summary_details_h2.innerHTML += " - ";
         ad_summary_details_h2.innerHTML += data[i].ROOMS;
         ad_summary_details_h2.innerHTML += " pièce(s)";

         if(data[i].ROOMS == "1")
         url_path += "-studios/";
         else {
           url_path += "-" + data[i].ROOMS + "-pieces/";
         }
        }

        ad_summary_desc_div1.appendChild(ad_summary_details_h2);

        var ad_summary_history = createNode("a");
        ad_summary_history.className = "announce_summary_history";
        ad_summary_history.innerHTML = "il y'a ";

        var firstDate;
        if(data[i].hasOwnProperty('FIRST_TIMESTAMP'))
        {
          firstDate = new Date(data[i].FIRST_TIMESTAMP);
        }
        else {
          firstDate = new Date(data[i].TIMESTAMP);
        }
        var currentDate = new Date();

        var elapsedTime = (currentDate - firstDate)/(1000*60*60);

        if( elapsedTime < 24 )
        {
          ad_summary_history.innerHTML+= String(Math.floor(elapsedTime));
          ad_summary_history.innerHTML+="h";
        }
        else {
                 var elapsedTimeInDays = Math.floor(elapsedTime / 24);
                 if(elapsedTimeInDays < 31)
                 {
                   ad_summary_history.innerHTML+= String(elapsedTimeInDays);
                   if(elapsedTimeInDays == 1)
                    ad_summary_history.innerHTML+=" jour";
                   else
                    ad_summary_history.innerHTML+=" jours";
                 }
                 else
                 {
                   var elapsedTimeInMonths = Math.floor(elapsedTime / (24*30) );
                   ad_summary_history.innerHTML+= String(elapsedTimeInMonths);
                   ad_summary_history.innerHTML+=" mois";
                 }

             }

        ad_summary_desc_div1.appendChild(ad_summary_history);

        ad_summary_container_div.appendChild(ad_summary_desc_div1);

        var ad_summary_desc_div2 = createNode("div");
        ad_summary_desc_div2.className = "announce_summary_desc";

        var ad_summary_city_container_div = createNode("div");
        ad_summary_city_container_div.className ="announce_summary_city_container";

        var ad_icon = createNode("i");
        ad_icon.className = "fas fa-map-marker-alt announce_summary_icon";
        ad_summary_city_container_div.appendChild(ad_icon);

        var ad_summary_city_p = createNode("p");
        ad_summary_city_p.className = "announce_summary_city";
        ad_summary_city_p.innerHTML = "<strong>" + data[i].CITY[0].toUpperCase() + data[i].CITY.slice(1) + "</strong>";
        ad_summary_city_container_div.appendChild(ad_summary_city_p);
        url_path += data[i].CITY;
        url_path += "-" + getPostalCode(data[i].CITY);
        var ad_price_p = createNode("p");
        ad_price_p.style.float = "right";
        ad_price_p.style.color = "#4c94bc";
        ad_price_p.style.margin = "0";
        ad_price_p.innerHTML = "<strong>" + formatPrice(String(data[i].PRICE)) + " €" + "</strong>";
        ad_summary_city_container_div.appendChild(ad_price_p);
        ad_summary_desc_div2.appendChild(ad_summary_city_container_div);

        var ad_summary_postalcode_container_div = createNode("div");
        ad_summary_postalcode_container_div.className ="announce_summary_city_container";

        var ad_summary_postalcode_p = createNode("p");
        ad_summary_postalcode_p.style.float = "left";
        ad_summary_postalcode_p.style.color = "#4c94bc";
        ad_summary_postalcode_p.style.margin = "0 0 0 30px";
        ad_summary_postalcode_p.style.fontSize = "14px";
        ad_summary_postalcode_p.innerHTML = "(" + getPostalCode(data[i].CITY) + ")";
        ad_summary_postalcode_container_div.appendChild(ad_summary_postalcode_p);


        var ad_price_bym2_p = createNode("p");
        ad_price_bym2_p.style.float = "right";
        ad_price_bym2_p.style.color = "#4c94bc";
        ad_price_bym2_p.style.margin = "0";
        ad_price_bym2_p.style.fontSize = "14px";
        var price_bym2 = Math.floor(data[i].PRICE / data[i].SURFACE);
        ad_price_bym2_p.innerHTML =  formatPrice(String(price_bym2)) + " €/m<sup>2</sup>";
        ad_summary_postalcode_container_div.appendChild(ad_price_bym2_p);

        ad_summary_desc_div2.appendChild(ad_summary_postalcode_container_div);

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
        arthurimmo:false,
        eraimmo:false,
        pap:false,
        iadfrance:false,
        paruvendu:false,
        avendrealouer:false,
        nestenn:false,
        agenceprincipale:false,
        lefigaroimmobilier:false,
        etreproprio:false,
        efficity:false,
        fnaim:false
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

        ad_link.setAttribute("onclick", "document.location='" + url_path + "?" + data[i].ID + "'") ;
        ad_link.style.cursor = "pointer";

        ad_link.appendChild(ad_div);

        main_content.appendChild(ad_link);

        var line_div = createNode("div");
        line_div.className = "sf_line_results row mx-auto";
        main_content.appendChild(line_div);


        /* adding google adsense */
        if( (i+1)%5 == 0)
        {
          addGoogleAds(main_content);
        }


    }

  }

  var header_content = document.getElementById("header-content");
  var forPuppeteer = createNode("div");
  forPuppeteer.setAttribute("id", "prerendered-page");
  forPuppeteer.setAttribute("data-ad-json", JSON.stringify(data));
  forPuppeteer.style.visibility = 'hidden';
  header_content.appendChild(forPuppeteer);

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
}

function gotosearchcriteria()
{
  var url_parameters = '';
  if(window.location.search == "")
    url_parameters = url_params;
  else
    url_parameters = window.location.search;

  const Params = new URLSearchParams(url_parameters);
  const searchType = Params.get('search_type');

  var search_criteria = document.getElementById("modify_search_criteria");

  if( searchType == "1")
    search_criteria.href = "/recherche/achat/immobilier?search_type=1";
  else {
    search_criteria.href = "/recherche/location/immobilier?search_type=2";
    }


}
