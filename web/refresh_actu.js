function createNode(element) {
    return document.createElement(element);
}

function append(parent, el) {
  return parent.append(el);
}
function addGoogleAds(news_content)
{
  var container = createNode("div");
  container.style.display = "block";
  container.style.width ="100%";
  container.style.marginTop = "10px";

  var actu_immo_container = createNode("div");
  actu_immo_container.className = "sf_actu_immo_gads_container";


  var ins = createNode("ins");
  ins.className = "adsbygoogle"
  ins.style.display ="block";
  ins.setAttribute('data-ad-format', "fluid");
  ins.setAttribute('data-ad-layout-key', "-fb+5w+4e-db+86");
  ins.setAttribute('data-ad-client', "ca-pub-3950735685733517");
  ins.setAttribute('data-ad-slot', "2808340986");

  actu_immo_container.append(ins);

  var script = createNode("script");
  script.innerHTML = "var isPageAlreadyRendered = document.getElementById(\"prerendered-page\"); if(isPageAlreadyRendered) {(adsbygoogle = window.adsbygoogle || []).push({});}";
  actu_immo_container.append(script);

  container.append(actu_immo_container);
  news_content.append(container);
}
const maxItemsPerSource = 10;
var logo_url = {"lesechos":"data/lesechos.svg", "notaires":"https://www.notaires.fr/sites/all/themes/custom/notaires_v2/assets/img/Notaires_de_France.png",
                "bfmimmo":"https://www.lavieimmo.com/refonte/img/logos/LVI.svg"};
var AllActuData = [];
var current_page = 1;
var records_per_page = 30;

function extract_actu_data(data, actu_source, image_url)
{
  const items = data.querySelectorAll("item");

  for(var i = 0; i < items.length && i < maxItemsPerSource; i++)
  {
    var actuitem = {"link":"", "img":"", "title":"", "source":"","pubdate":new Date(),"description":""};

    actuitem.link = items[i].querySelector("link").innerHTML;

    if(actu_source == "lesechos" || actu_source == "bfmimmo" || actu_source == "notaires")
    {
      var img_enclosure = items[i].querySelector("enclosure");
      if(img_enclosure)
      {
        actuitem.img = img_enclosure.getAttribute('url');
      }
      else {
        actuitem.img = logo_url[actu_source];
      }
    }
    else if( actu_source == "capital")
    {
      var media = items[i].getElementsByTagName("media\:content");
      actuitem.img = media[0].getAttribute('url');
    }else if(actu_source == "lejournaldelagence")
    {
      actuitem.img = items[i].querySelector("media").textContent;
    }else{
      actuitem.img = image_url;
    }


    actuitem.title = items[i].querySelector("title").textContent;


    if(actu_source == "lefigaro")
    {
      actuitem.source  = "Le Figaro - immobilier";
    }
    else {
      if(actu_source == "lesechos")
      {
        actuitem.source = "Les Echos - Immobilier";
      }
      else {
        if( actu_source == "capital")
        {
          actuitem.source = "https://www.capital.fr";
        }
        else if( actu_source == "notaires"){
            actuitem.source = "Notaires - Immobilier"
        } else{
          actuitem.source = data.querySelector("title").textContent;
        }
      }
    }

    var pubDate = items[i].querySelector("pubDate");
    var today = new Date()
    var lastdate = new Date().setDate(today.getDate()-30)
    if( pubDate !== null )
    {
      if(today < new Date(pubDate.innerHTML))
      {
        actuitem.pubdate= lastdate;
      }
      else
      {
        if( new Date(lastdate) < new Date(pubDate.innerHTML))
        actuitem.pubdate= pubDate.innerHTML;
        else {
          continue;
        }
      }
    }
    else {
      actuitem.pubdate = lastdate;
    }

    actuitem.description = items[i].querySelector("description").textContent.slice(0,342);

    AllActuData.push(actuitem);
  }
}


function generate_actu(items, empty=false)
{
  var news_content = document.getElementById("news-content");

  if(empty)
    news_content.innerHTML ="";

  var min_index = (current_page-1) * records_per_page;
  var max_index = (current_page * records_per_page);

  //for(var i = 0; i < items.length ; i++)
  for (var i = min_index; i < max_index && i < items.length; i++)
  {
    var actu_link  = createNode("a");
    actu_link.href = items[i].link;
    actu_link.target = "_blank";

    var actu_immo_container = createNode("div");
    actu_immo_container.className = "row mx-auto sf_actu_immo_container";

    var actu_immo_image_container = createNode("div");
    actu_immo_image_container.className = "sf_actu_immo_image_container";

    var img = createNode("img");
    img.style.height = "100px";
    img.style.width = "100px";
    img.style.borderRadius = "8px";

    img.src = items[i].img;
    img.alt = items[i].title;

    actu_immo_image_container.append(img);

    actu_immo_container.append(actu_immo_image_container);

    var actu_immo_text_container = createNode("div");
    actu_immo_text_container.className = "sf_actu_immo_text_container";
    var actu_immo_text_title = createNode("div");
    actu_immo_text_title.className = "sf_actu_immo_text_title";
    var title = createNode("strong");
    title.className = "sf_actu_immo_text_title_size";
    title.innerHTML = items[i].title;


    actu_immo_text_title.append(title);
    actu_immo_text_container.append(actu_immo_text_title);

    var actu_immo_source_container = createNode("div");
    actu_immo_source_container.className = "sf_actu_immo_source_container";

    var actu_immo_text_source_1 = createNode("span");
    actu_immo_text_source_1.className = "sf_actu_immo_text_source_size";


    actu_immo_text_source_1.innerHTML = items[i].source;


    actu_immo_source_container.append(actu_immo_text_source_1);


    var actu_immo_text_source_2 = createNode("span");
    actu_immo_text_source_2.className = "sf_actu_immo_text_source_size";
    actu_immo_text_source_2.style.marginLeft = "3px";
    if(items[i].source != "Logement - Business Immo")
    {
      actu_immo_text_source_2.innerHTML = " - il y'a ";
      var firstDate = new Date(items[i].pubdate);

      var currentDate = new Date();

      var elapsedTime = (currentDate - firstDate)/(1000*60*60);

      if( elapsedTime < 24 )
      {
        actu_immo_text_source_2.innerHTML += String(Math.floor(elapsedTime));
        actu_immo_text_source_2.innerHTML += "h";
      }
      else {
               var elapsedTimeInDays = Math.floor(elapsedTime / 24);
               actu_immo_text_source_2.innerHTML+= String(elapsedTimeInDays);
               if(elapsedTimeInDays == 1)
                actu_immo_text_source_2.innerHTML+=" jour";
               else
                actu_immo_text_source_2.innerHTML+=" jours";
           }
    }

    actu_immo_source_container.append(actu_immo_text_source_2);



    actu_immo_text_container.append(actu_immo_source_container);

    var actu_immo_text_box = createNode("div");
    actu_immo_text_box.className = "sf_actu_immo_text_box";

    var actu_immo_text_size = createNode("span");
    actu_immo_text_size.className = "sf_actu_immo_text_size";

    actu_immo_text_size.innerHTML = items[i].description;

    actu_immo_text_size.innerHTML = actu_immo_text_size.innerHTML.replace(/&nbsp;/g,' ');
    actu_immo_text_box.append(actu_immo_text_size);

    actu_immo_text_container.append(actu_immo_text_box);

    actu_immo_container.append(actu_immo_text_container);

    actu_link.append(actu_immo_container);

    news_content.append(actu_link);

    if( (i+1)%5 == 0)
    {
      addGoogleAds(news_content);
    }
  }

  var header_content = document.getElementById("header-content");
  var forPuppeteer = createNode("div");
  forPuppeteer.setAttribute("id", "prerendered-page");
  forPuppeteer.setAttribute("data-ad-json", JSON.stringify(AllActuData));
  forPuppeteer.style.visibility = 'hidden';
  header_content.appendChild(forPuppeteer);
}

const lesEchosRssUrl = "data/actus/lesechos_immobilier.xml";
const capitalRssUrl = "data/actus/capital.xml";
const journaldelagenceRssUrl = "data/actus/lejournaldelagence.xml";
const lefigaroRssUrl = "data/actus/figaro_immobilier.xml";
const businessImmoRssUrl = "data/actus/businessimmo.xml";
const notairesImmoUrl = "data/actus/immobiliernotaires.xml"
var puppeteer = document.getElementById('prerendered-page');
if(!puppeteer)
{
  fetch(lesEchosRssUrl)
    .then(response => response.text() )
    .then(str => new window.DOMParser().parseFromString(str, "text/xml"))
    .then(data => extract_actu_data(data, "lesechos", ""))
    .then(function() {return fetch(capitalRssUrl);})
    .then(response => response.text() )
    .then(str => new window.DOMParser().parseFromString(str, "text/xml"))
    .then(data => extract_actu_data(data, "capital", "https://www.capital.fr/logo_capital.png"))
    .then(function(){return fetch(journaldelagenceRssUrl);})
    .then(response => response.text() )
    .then(str => new window.DOMParser().parseFromString(str, "text/xml"))
    .then(data => extract_actu_data(data, "lejournaldelagence", "https://www.journaldelagence.com/wp-content/themes/jda-2018/img/logo.png"))
    .then(function(){return fetch(lefigaroRssUrl);})
    .then(response => response.text() )
    .then(str => new window.DOMParser().parseFromString(str, "text/xml"))
    .then(data => extract_actu_data(data, "lefigaro", "http://a.f1g.fr/assets-img/i/f/m150.png"))
    .then(function(){return fetch(businessImmoRssUrl);})
    .then(response => response.text() )
    .then(str => new window.DOMParser().parseFromString(str, "text/xml"))
    .then(data => extract_actu_data(data, "businessimmo", "https://www.businessimmo.com/images/logo-bi-nobaseline.svg"))
    .then(function(){return fetch(notairesImmoUrl);})
    .then(response => response.text() )
    .then(str => new window.DOMParser().parseFromString(str, "text/xml"))
    .then(data => extract_actu_data(data, "notaires", ""))
    .then(function() {
      AllActuData.sort(function(lhs, rhs) { return new Date(rhs.pubdate) - new Date(lhs.pubdate); } );
      generate_actu(AllActuData);

      /* set pagination */
      var pagination_box = createNode("div");
      pagination_box.className = "sf_pagination_box";

      var range_pagination_container = createNode("div");
      range_pagination_container.className = "sf_pagination_container";
      var range_pagination = createNode("a");
      range_pagination.id = "range-pagination";
      var min_page = (current_page-1) * records_per_page + 1;
      var max_page = Math.min((current_page * records_per_page),  AllActuData.length);
      range_pagination.innerHTML = min_page + " - " + max_page;
      range_pagination_container.appendChild(range_pagination);
      pagination_box.appendChild(range_pagination_container);

      var pagination_container = createNode("div");
      pagination_container.className = "sf_pagination_container";

      var prevpage_line = createNode("a");
      prevpage_line.href ="javascript:prevPage()";
      prevpage_line.id = "btn_prev";
      prevpage_line.className ="generic_infos_desc_text";
      var prevpage_icon = createNode("i");
      prevpage_icon.className = "fas fa-chevron-left sf_generic_infos_desc_text_icon";
      prevpage_line.appendChild(prevpage_icon);
      prevpage_line.innerHTML = " page précédente";
      if (current_page == 1) {
          prevpage_line.style.visibility = "hidden";
      } else {
          prevpage_line.style.visibility = "visible";
      }
      pagination_container.appendChild(prevpage_line);

      var nextpage_line = createNode("a");
      nextpage_line.href ="javascript:nextPage()";
      nextpage_line.id = "btn_next";
      nextpage_line.className ="generic_infos_desc_text";
      nextpage_line.style.marginLeft = "15px";
      var nextpage_icon = createNode("i");
      nextpage_icon.className = "fas fa-chevron-right sf_generic_infos_desc_text_icon";
      nextpage_line.appendChild(prevpage_icon);
      nextpage_line.innerHTML = " page suivante";
      var number_of_pages = Math.ceil(AllActuData.length / records_per_page);
      if (current_page == number_of_pages) {
          nextpage_line.style.visibility = "hidden";
      } else {
          nextpage_line.style.visibility = "visible";
      }

      pagination_container.appendChild(nextpage_line);

      var page_line = createNode("a");
      page_line.style.marginLeft = "15px";
      page_line.innerHTML = "page: ";
      var nbpage_line = createNode("span");
      nbpage_line.id = "list_page";


      nbpage_line.innerHTML = current_page + "/" + number_of_pages;

      page_line.appendChild(nbpage_line);
      pagination_container.appendChild(page_line);

      pagination_box.appendChild(pagination_container);
      var news_pagination = document.getElementById("news-pagination");
      news_pagination.appendChild(pagination_box);
    }).catch(function(error){
      console.log(error);
    });
}
