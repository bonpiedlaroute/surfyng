sessionStorage.setItem("scrollX", "0");
sessionStorage.setItem("scrollY", "0");
sessionStorage.setItem("needtosort", "");
sessionStorage.setItem("summary_json_data", "");

function update_actu(data, begin, end )
{
  const items = data.querySelectorAll("item");
  var nb_news = 0;
  for (var i = begin; i <= end; i++)
  {
    var actu_link_id = "actu_link_" + String(i);
    var actu_link = document.getElementById(actu_link_id);
    actu_link.href = items[nb_news].querySelector("link").innerHTML;;

    var actu_title_id = "actu_title_" + String(i);
    var actu_title = document.getElementById(actu_title_id);
    actu_title.innerHTML = items[nb_news].querySelector("title").textContent;

    var actu_text_id = "actu_text_" + String(i);
    var actu_text = document.getElementById(actu_text_id);
    actu_text.innerHTML = items[nb_news].querySelector("description").textContent;


    var actu_source_time_id = "actu_source_time_" + String(i);
    var actu_source_time = document.getElementById(actu_source_time_id);
    actu_source_time.style.marginLeft = "3px";
    actu_source_time.innerHTML = " - il y'a ";
    var pubDate = items[nb_news].querySelector("pubDate");

    var firstDate = new Date(pubDate.innerHTML);

    var currentDate = new Date();

    var elapsedTime = (currentDate - firstDate)/(1000*60*60);

    if( elapsedTime < 24 )
    {
      actu_source_time.innerHTML += String(Math.floor(elapsedTime));
      actu_source_time.innerHTML += "h";
    }
    else
    {
       var elapsedTimeInDays = Math.floor(elapsedTime / 24);
       actu_source_time.innerHTML+= String(elapsedTimeInDays);
       if(elapsedTimeInDays == 1)
        actu_source_time.innerHTML+=" jour";
       else
        actu_source_time.innerHTML+=" jours";
    }

    nb_news++;
  }

}

function goToDeposit()
{
  window.location.href = "deposer-une-annonce.html";
//     var user_id = sessionStorage.getItem("user_id");
//     if(user_id)
//     {
//       window.location.href = "deposer-une-annonce.html";
//     }
//     else
//     {
//       $("#login").modal(
//         {
//           focus: true,
//           show: true
//         });
//     }
}


const lefigaroRssUrl = "data/actus/figaro_immobilier.xml";
fetch(lefigaroRssUrl)
  .then(response => response.text() )
  .then(str => new window.DOMParser().parseFromString(str, "text/xml"))
  .then(function(data) {
    update_actu(data, 1, 2);
  });

const lesEchosRssUrl = "data/actus/lesechos_immobilier.xml";
fetch(lesEchosRssUrl)
      .then(response => response.text() )
      .then(str => new window.DOMParser().parseFromString(str, "text/xml"))
      .then(function(data) {
        update_actu(data, 3, 4);
      } );

var last_actu = document.getElementById("last_actu");
last_actu.style.display = "flex";
