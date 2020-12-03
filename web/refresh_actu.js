function createNode(element) {
    return document.createElement(element);
}

function append(parent, el) {
  return parent.append(el);
}

const maxItemsPerSource = 10;

function generate_actu(data, actu_source, image_url)
{
  const items = data.querySelectorAll("item");
  var main_content = document.getElementById("main-content");
  for(var i = 0; i < items.length && i < maxItemsPerSource; i++)
  {
    var actu_link  = createNode("a");
    actu_link.href = items[i].querySelector("link").innerHTML;
    actu_link.target = "_blank";

    var actu_immo_container = createNode("div");
    actu_immo_container.className = "row mx-auto sf_actu_immo_container";

    var actu_immo_image_container = createNode("div");
    actu_immo_image_container.className = "sf_actu_immo_image_container";

    var img = createNode("img");
    img.style.height = "100px";
    img.style.width = "100px";
    img.style.borderRadius = "8px";
    if(actu_source == "lesechos" || actu_source == "bfmimmo")
    {
      img.src = items[i].querySelector("enclosure").getAttribute('url');
    }
    else if( actu_source == "capital")
    {
      var media = items[i].getElementsByTagName("media\:content");
      img.src = media[0].getAttribute('url');
    }else if(actu_source == "lejournaldelagence")
    {
      img.src = items[i].querySelector("media").textContent;
    }else{
      img.src = image_url;
    }
    actu_immo_image_container.append(img);

    actu_immo_container.append(actu_immo_image_container);

    var actu_immo_text_container = createNode("div");
    actu_immo_text_container.className = "sf_actu_immo_text_container";
    var actu_immo_text_title = createNode("div");
    actu_immo_text_title.className = "sf_actu_immo_text_title";
    var title = createNode("strong");
    title.className = "sf_actu_immo_text_title_size";
    title.innerHTML = items[i].querySelector("title").textContent;


    actu_immo_text_title.append(title);
    actu_immo_text_container.append(actu_immo_text_title);

    var actu_immo_source_container = createNode("div");
    actu_immo_source_container.className = "sf_actu_immo_source_container";

    var actu_immo_text_source_1 = createNode("span");
    actu_immo_text_source_1.className = "sf_actu_immo_text_source_size";
    actu_immo_text_source_1.innerHTML = actu_source == "lefigaro"? "Le Figaro - Gestion de patrimoine immobilier": data.querySelector("title").textContent;
    actu_immo_source_container.append(actu_immo_text_source_1);

    var pubDate = items[i].querySelector("pubDate");
    if( pubDate !== null )
    {
      var actu_immo_text_source_2 = createNode("span");
      actu_immo_text_source_2.className = "sf_actu_immo_text_source_size";
      actu_immo_text_source_2.style.marginLeft = "3px";
      actu_immo_text_source_2.innerHTML = " - il y'a ";
      var firstDate = new Date(pubDate.innerHTML);

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

      actu_immo_source_container.append(actu_immo_text_source_2);

    }

    actu_immo_text_container.append(actu_immo_source_container);

    var actu_immo_text_box = createNode("div");
    actu_immo_text_box.className = "sf_actu_immo_text_box";

    var actu_immo_text_size = createNode("span");
    actu_immo_text_size.className = "sf_actu_immo_text_size";

    if(actu_source == "bfmimmo"){
      var index = v=items[i].querySelector("description").textContent.indexOf("<img");
      if(index != - 1)
      actu_immo_text_size.innerHTML = items[i].querySelector("description").textContent.slice(0,index);
      else {
        actu_immo_text_size.innerHTML = items[i].querySelector("description").textContent.slice(0,342);
      }
    }
    else {
      actu_immo_text_size.innerHTML = items[i].querySelector("description").textContent.slice(0,342);
    }

    actu_immo_text_box.append(actu_immo_text_size);

    actu_immo_text_container.append(actu_immo_text_box);

    actu_immo_container.append(actu_immo_text_container);

    actu_link.append(actu_immo_container);

    main_content.append(actu_link);
  }
}

const lesEchosRssUrl = "data/actus/lesechos_immobilier.xml";
fetch(lesEchosRssUrl)
  .then(response => response.text() )
  .then(str => new window.DOMParser().parseFromString(str, "text/xml"))
  .then(data => generate_actu(data, "lesechos", ""));

const capitalRssUrl = "data/actus/capital.xml";
fetch(capitalRssUrl)
    .then(response => response.text() )
    .then(str => new window.DOMParser().parseFromString(str, "text/xml"))
    .then(data => generate_actu(data, "capital", "https://www.capital.fr/logo_capital.png"));

const bfmimmoRssUrl = "data/actus/bfmimmobilier.xml";
fetch(bfmimmoRssUrl)
    .then(response => response.text() )
    .then(str => new window.DOMParser().parseFromString(str, "text/xml"))
    .then(data => generate_actu(data, "bfmimmo", "https://www.bfmtv.com/assets/images/BFM_BUSINESS_default_16x9.jpg"));

const journaldelagenceRssUrl = "data/actus/lejournaldelagence.xml";
fetch(journaldelagenceRssUrl)
    .then(response => response.text() )
    .then(str => new window.DOMParser().parseFromString(str, "text/xml"))
    .then(data => generate_actu(data, "lejournaldelagence", "https://www.journaldelagence.com/wp-content/themes/jda-2018/img/logo.png"));

const lefigaroRssUrl = "data/actus/figaro_immobilier.xml";
fetch(lefigaroRssUrl)
    .then(response => response.text() )
    .then(str => new window.DOMParser().parseFromString(str, "text/xml"))
    .then(data => generate_actu(data, "lefigaro", "http://a.f1g.fr/assets-img/i/f/m150.png"));

const businessImmoRssUrl = "data/actus/businessimmo.xml";
fetch(businessImmoRssUrl)
    .then(response => response.text() )
    .then(str => new window.DOMParser().parseFromString(str, "text/xml"))
    .then(data => generate_actu(data, "businessimmo", "https://www.businessimmo.com/images/logo-bi-nobaseline.svg"));
