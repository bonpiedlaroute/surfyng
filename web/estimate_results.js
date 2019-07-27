var mobile   = /Android|webOS|iPhone|iPad|iPod|BlackBerry/i.test(navigator.userAgent);


var url = "https://surfyn.fr:7878/predict?";
//var url = "http://127.0.0.1:7878/predict?";


const Params = new URLSearchParams(window.location.search);
const lon = Params.get('lon');

if(lon)
{
  url += "longitude=";
  url += lon;
}

const lat = Params.get('lat');
if(lat)
{
  url += "&latitude=";
  url += lat;
}

const code_postal = Params.get('postcode');
if(code_postal)
{
  url += "&code_postal=";
  url += code_postal;
}

const type_local = Params.get('prop_type');

if(type_local)
{
  url += "&code_type_local=";
  if(type_local == "1")
  {
    url +="2";
  }
  else {
    url += "1";
  }
}

const carrez = Params.get("carrez");

if(carrez)
{
  url += "&lot1_surface_carrez=";
  url += carrez;
}

const allarea = Params.get("allarea");

if(allarea)
{
  url += "&surface_reelle_bati=";
  url += allarea;
}

const rooms = Params.get("allarea");

if(rooms)
{
  url += "&nombre_pieces_principales=";
  url += rooms;
}

const ground_area = Params.get("ground_area");

if(ground_area)
{
  url += "&surface_terrain=";
  url += ground_area;
}

var nb_lots = 1;

const cave = Params.get("cave");

if(cave)
{
  nb_lots += 1;
}

const parking = Params.get("parking");

if(parking)
{
  nb_lots += 1;
}

url += "&nombre_lots=";
url += nb_lots;

let today = new Date().toISOString().slice(0, 10);

url += "&date_mutation=";
url += today;
var next_url="";
fetch(url)
.then(function(resp) { return resp.json(); } )
.then(function(data) {
  set_predicted_price(data);
  window.location.href = next_url;
})
.catch(function(error) {

  console.log(error);
});

function reverse(s){
    return s.split("").reverse().join("");
}

function formatPrice(p)
{
  var val = reverse(p);

  var tmp = val.replace(/(.{3})/g,"$1 ");

  return reverse(tmp);
}

function set_predicted_price(data)
{
  if(data.length > 0 )
  {
    next_url += "estimate_results.html?predicted_price=";
    next_url += formatPrice(String(data[0]));

  }

}
