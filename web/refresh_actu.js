function createNode(element) {
    return document.createElement(element);
}

function append(parent, el) {
  return parent.append(el);
}
function addGoogleAds(main_content)
{
  var actu_immo_container = createNode("div");
  actu_immo_container.className = "row mx-auto sf_actu_immo_container";

  var ins = createNode("ins");
  ins.className = "adsbygoogle"
  ins.style.display ="block";
  ins.setAttribute('data-ad-format', "fluid");
  ins.setAttribute('data-ad-layout-key', "-fb+5w+4e-db+86");
  ins.setAttribute('data-ad-client', "ca-pub-3950735685733517");
  ins.setAttribute('data-ad-slot', "2808340986");
  actu_immo_container.append(ins);

  var script = createNode("script");
  script.innerHTML = "(adsbygoogle = window.adsbygoogle || []).push({})";
  actu_immo_container.append(script);

  main_content.append(actu_immo_container)
}
const maxItemsPerSource = 10;
var logo_url = {"lesechos":"data/lesechos.svg", "notaires":"https://www.notaires.fr/sites/all/themes/custom/notaires_v2/assets/img/Notaires_de_France.png",
                "bfmimmo":"https://www.lavieimmo.com/refonte/img/logos/LVI.svg"};
var AllActuData = [];

function extract_actu_data(data, actu_source, image_url)
{
  const items = data.querySelectorAll("item");
  var main_content = document.getElementById("main-content");
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
      if( new Date(lastdate) < new Date(pubDate.innerHTML))
      actuitem.pubdate= pubDate.innerHTML;
      else {
        continue;
      }
    }
    else {

      actuitem.pubdate = lastdate;
    }

    actuitem.description = items[i].querySelector("description").textContent.slice(0,342);

    AllActuData.push(actuitem);
  }
}


function generate_actu(items)
{
  var main_content = document.getElementById("main-content");
  for(var i = 0; i < items.length ; i++)
  {
    var actu_link  = createNode("a");
    actu_link.href = items[i].link;
    actu_link.target = "_blank";
    actu_link.rel = "nofollow";

    var actu_immo_container = createNode("div");
    actu_immo_container.className = "row mx-auto sf_actu_immo_container";

    var actu_immo_image_container = createNode("div");
    actu_immo_image_container.className = "sf_actu_immo_image_container";

    var img = createNode("img");
    img.style.height = "100px";
    img.style.width = "100px";
    img.style.borderRadius = "8px";

    img.src = items[i].img;

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

    main_content.append(actu_link);

    if( (i+1)%5 == 0)
    {
      addGoogleAds(main_content);
    }
  }
}

const lesEchosRssUrl = "data/actus/lesechos_immobilier.xml";
const capitalRssUrl = "data/actus/capital.xml";
const journaldelagenceRssUrl = "data/actus/lejournaldelagence.xml";
const lefigaroRssUrl = "data/actus/figaro_immobilier.xml";
const businessImmoRssUrl = "data/actus/businessimmo.xml";
const notairesImmoUrl = "data/actus/immobiliernotaires.xml"

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
  }).catch(function(error){
    console.log(error);
  });
