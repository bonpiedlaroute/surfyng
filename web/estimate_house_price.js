var mobile   = /Android|webOS|iPhone|iPad|iPod|BlackBerry/i.test(navigator.userAgent);


var eventdown = mobile ? "touchstart" : "mousedown";
var eventmove = mobile ? "touchmove" : "mousemove";
var eventup = mobile ? "touchend" : "mouseup";

var lon = 0.0;
var lat = 0.0;
var next_url = "";
var cityMap = new Map( [ ["ANTONY", "92160"],["CHATENAY-MALABRY", "92290"], ["SCEAUX","92330"], ["BOURG-LA-REINE","92340"], ["BAGNEUX","92220"], ["FONTENAY-AUX-ROSES","92260"]
, ["LE PLESSIS-ROBINSON","92350"], ["CLAMART","92140"], ["CHATILLON","92320"], ["MONTROUGE","92120"], ["MALAKOFF","92240"],["VANVES","92170"], ["ISSY-LES-MOULINEAUX","92130"]
, ["BOULOGNE-BILLANCOURT","92100"], ["MEUDON","92190"], ["SEVRES","92310"], ["CHAVILLE","92370"], ["VILLE-D'AVRAY","92410"], ["SAINT-CLOUD","92210"], ["MARNES-LA-COQUETTE","92430"]
, ["VAUCRESSON","92420"], ["GARCHES","92380"], ["SURESNES","92150"], ["PUTEAUX","92800"],["NANTERRE","92000"], ["COLOMBES","92700"], ["LA GARENNE-COLOMBES","92250"], ["BOIS-COLOMBES","92270"]
, ["COURBEVOIE","92400"], ["NEUILLY-SUR-SEINE","92200"], ["LEVALLOIS-PERRET","92300"], ["CLICHY","92110"], ["ASNIERES-SUR-SEINE","92600"], ["GENNEVILLIERS","92230"]
, ["VILLENEUVE-LA-GARENNE","92390"] ] )

function checkAddress()
{

  // we first check that we handle this city
  var new_city = document.getElementById("estimate_city");
  var feedback_msg = document.getElementById("feedback_msg");
  var msg_type = document.getElementById("msg_type");
  var city_check_msg = document.getElementById("city_check_msg");
  var city = new_city.value.trim().toUpperCase();
  //console.log("checkAddress");
  if( !cityMap.has(city))
  {
    new_city.className = "form-control is-invalid";
    city_check_msg.className = "feedback_box bad_feedback";
    msg_type.innerHTML = ""
    feedback_msg.innerHTML = "Désolé, pour l'instant nous supportons uniquement les villes du département des hauts-de-seine";
    feedback_msg.style.color ="red";
    return false;
  }
  var road_number = document.getElementById("road_number");
  var estimate_road = document.getElementById("estimate_road");
  new_city.className = "form-control sf_form_decoration";
  road_number.className = "form-control sf_form_decoration";
  estimate_road.className = "form-control sf_form_decoration";
  error_msg.innerHTML = "";

 //https://api-adresse.data.gouv.fr/search/?q=16+avenue+henri+barbusse+colombes&postcode=92700

  var url_geo = "https://api-adresse.data.gouv.fr/search/?q=";
  url_geo += road_number.value;
  url_geo += "+";
  url_geo += estimate_road.value.replace(/\s+/g,'+');
  url_geo += "+";
  url_geo += city.toLowerCase();
  url_geo += "&postcode=";
  url_geo += cityMap.get(city);
  console.log(url_geo);

  fetch(url_geo)
  .then(function(resp) { return resp.json(); } )
  .then(function(data_geo) {
    if( data_geo.features.length != 0 )
    {
       lon = data_geo.features[0].geometry.coordinates[0];
       lat = data_geo.features[0].geometry.coordinates[1];
       next_url += "?lon=";
       next_url += lon;

       next_url += "&lat=";
       next_url += lat;
       return true;
    }
    else {
      new_city.className = "form-control is-invalid sf_form_error";
      road_number.className = "form-control is-invalid sf_form_error";
      estimate_road.className = "form-control is-invalid sf_form_error";

      city_check_msg.className = "feedback_box bad_feedback";
      msg_type.innerHTML = ""
      feedback_msg.innerHTML = "Désolé, nous ne retrouvons pas l'adresse que vous avez rentré";
      feedback_msg.style.color ="red";

      return false;
    }

  })
  .catch(function(error) {
    error_msg.innerHTML = "Désolé, certains services ne sont pas démarrés";
    console.log(error);
    return false;
  });

}

var divLoft = document.getElementById("loft");
var divHouse = document.getElementById("house");
var divApart = document.getElementById("apart");
var divOffice = document.getElementById("office");
var carrezAreaValue = document.getElementById("carrez_area_value");
var areaValue = document.getElementById("area_value");
var areaErrorMsg = document.getElementById("area_error_msg");

var propertyType = Object.freeze({"loft":0, "house":1, "apart":2, "office":3});
var isSelected = [false, false, false, false];

function handleEvtDownHouse(event)
{
  if( isSelected[propertyType.house])
  {
    isSelected[propertyType.house] = false;
    divHouse.style.transform = "scale(0.85)";
    divHouse.style.border = "";
  }
  else
  {
    isSelected[propertyType.house] = true;
    divHouse.style.transform = "scale(1)";
    divHouse.style.border = "solid 3px #222545";
    var propTypeErrorMsg = document.getElementById("prop_type_error_msg");
    propTypeErrorMsg.innerHTML = "";
    propTypeErrorMsg.style.color = "";
    propTypeErrorMsg.style.fontSize = "";

    if(isSelected[propertyType.apart])
    {
      isSelected[propertyType.apart] = false;
      divApart.style.transform = "scale(0.85)";
      divApart.style.border = "";
    }
  }
}
function handleEvtDownApart(event)
{
  if( isSelected[propertyType.apart])
  {
    isSelected[propertyType.apart] = false;
    divApart.style.transform = "scale(0.85)";
    divApart.style.border = "";
  }
  else
  {
    isSelected[propertyType.apart] = true;
    divApart.style.transform = "scale(1)";
    divApart.style.border = "solid 3px #222545";
    var propTypeErrorMsg = document.getElementById("prop_type_error_msg");
    propTypeErrorMsg.innerHTML = "";
    propTypeErrorMsg.style.color = "";
    propTypeErrorMsg.style.fontSize = "";

    if(isSelected[propertyType.house])
    {
      isSelected[propertyType.house] = false;
      divHouse.style.transform = "scale(0.85)";
      divHouse.style.border = "";
    }
  }
}


if( divHouse && divApart )
{
  divHouse.addEventListener(eventdown, handleEvtDownHouse, true);
  divApart.addEventListener(eventdown, handleEvtDownApart, true);
}


function handleEvtDownPropertyType(event, divElt, evtType)
{
  if( isSelected[evtType])
  {
    isSelected[evtType] = false;
    divElt.style.transform = "scale(0.85)";
    divElt.style.border = "";
  }
  else
  {
    isSelected[evtType] = true;
    divElt.style.transform = "scale(1)";
    divElt.style.border = "solid 3px #222545";
  }
}

/* Handle rooms buttons */
var rooms = document.querySelectorAll('.sf_rooms');
var roomsErrorMsg = document.getElementById("rooms_error_msg");
var i = 0;
var isRoomsSelected = new Map();
for(i = 0; i < rooms.length; ++i)
{
  isRoomsSelected.set(rooms[i], false);
}

function handleEvtDownRooms(event)
{
  if(isRoomsSelected.get(event.currentTarget) )
  {
    isRoomsSelected.set(event.currentTarget, false);
    event.currentTarget.style.transform = "scale(0.85)";
    event.currentTarget.style.border = "";
  }
  else
  {
      isRoomsSelected.set(event.currentTarget, true);
      event.currentTarget.style.transform = "scale(1)";
      event.currentTarget.style.border = "solid 3px #222545";
      roomsErrorMsg.innerHTML = "";
      roomsErrorMsg.style.color = "";
      for(i = 0; i < rooms.length; ++i)
      {
          if( rooms[i] != event.currentTarget )
          {
            isRoomsSelected.set(rooms[i], false);
            rooms[i].style.transform = "scale(0.85)";
            rooms[i].style.border = "";
          }
      }

  }
}

for(i = 0; i < rooms.length; i++)
{
  rooms[i].addEventListener(eventdown, handleEvtDownRooms, true);
}

/* Handle bedrooms buttons */
var bedrooms = document.querySelectorAll('.sf_bedrooms');

var isBedRoomsSelected = new Map();
for(i = 0; i < bedrooms.length; ++i)
{
  isBedRoomsSelected.set(bedrooms[i], false);
}

function handleEvtDownBedRooms(event)
{
  if(isBedRoomsSelected.get(event.currentTarget) )
  {
    isBedRoomsSelected.set(event.currentTarget, false);
    event.currentTarget.style.transform = "scale(0.85)";
    event.currentTarget.style.border = "";
  }
  else
  {
      isBedRoomsSelected.set(event.currentTarget, true);
      event.currentTarget.style.transform = "scale(1)";
      event.currentTarget.style.border = "solid 3px #222545";

      for(i = 0; i < bedrooms.length; ++i)
      {
          if( bedrooms[i] != event.currentTarget )
          {
            isBedRoomsSelected.set(bedrooms[i], false);
            bedrooms[i].style.transform = "scale(0.85)";
            bedrooms[i].style.border = "";
          }
      }

  }
}
for(i = 0; i < rooms.length; i++)
{
  bedrooms[i].addEventListener(eventdown, handleEvtDownBedRooms, true);
}


var validate = document.getElementById("validate");
var isFirstTimePropType = true;


function isEmpty(str) {
    return (!str || 0 === str.length);
}
function checkArea()
{
  if( isEmpty(carrezAreaValue.value) && isEmpty(areaValue.value) )
  {
    areaErrorMsg.innerHTML = "* Vous devez renseigner au moins une surface";
    areaErrorMsg.style.color = "red";
    areaErrorMsg.style.fontSize = "12px";
    validate.href = "#";
    return false;
  }
  else {
    areaErrorMsg.innerHTML = "";
    areaErrorMsg.style.color = "";
    areaErrorMsg.style.fontSize = "";
    return true;
  }
}

function isOneRoomsSelected()
{
  for(i = 0; i < rooms.length; ++i)
  {
    if(isRoomsSelected.get(rooms[i]) == true)
    {
      return [true,(i+1).toString()];
    }
  }
  return [false];
}

function completenexturl()
{
  /* check if property type is set, and populate the value */
  var propTypeErrorMsg = document.getElementById("prop_type_error_msg");
  if( isSelected[propertyType.apart])
  {
    next_url += "&prop_type=1";
    propTypeErrorMsg.innerHTML = "";
    propTypeErrorMsg.style.color = "";
  }
  else {
    if(isSelected[propertyType.house])
    {
      next_url += "&prop_type=2";
      propTypeErrorMsg.innerHTML = "";
      propTypeErrorMsg.style.color = "";
    }
    else {
      propTypeErrorMsg.innerHTML = "* Vous devez indiquer au moins un type de bien";
      propTypeErrorMsg.style.color = "red";
      propTypeErrorMsg.style.fontSize = "12px";
      validate.href = "#";
      return;
    }
  }

  /* check if area is set and populate the value */
  if(checkArea() == false)
    return;

  next_url += "&carrez=";
  next_url += carrezAreaValue.value;
  next_url += "&allarea=";
  next_url += areaValue.value;

  /* check if number of rooms is set, and populate the value */
  var selectedRooms = isOneRoomsSelected();
  if( selectedRooms[0] )
  {
    next_url += "&rooms="
    next_url += selectedRooms[1];

    roomsErrorMsg.innerHTML = "";
    roomsErrorMsg.style.color = "";
  }
  else {
      roomsErrorMsg.innerHTML = "* Vous devez indiquer le nombre de pièces de votre bien";
      roomsErrorMsg.style.color = "red";
      roomsErrorMsg.style.fontSize = "12px";
      validate.href = "#";
      return;
  }

  validate.href = next_url;
  window.location.href = next_url;
}

function gotonextpage(event)
{

  next_url = "estimate_criteria_det.html";

  /* check if it is a valid address and populate gps coordinates */
  // we first check that we handle this city
  var new_city = document.getElementById("estimate_city");
  var feedback_msg = document.getElementById("feedback_msg");
  var msg_type = document.getElementById("msg_type");
  var city_check_msg = document.getElementById("city_check_msg");
  var city = new_city.value.trim().toUpperCase();

  if( !cityMap.has(city))
  {
    new_city.className = "form-control is-invalid";
    city_check_msg.className = "feedback_box bad_feedback";
    msg_type.innerHTML = ""
    feedback_msg.innerHTML = " Désolé, nous supportons uniquement les villes du département des hauts-de-seine";
    feedback_msg.style.color ="red";

    return false;
  }
  var road_number = document.getElementById("road_number");
  var estimate_road = document.getElementById("estimate_road");
  new_city.className = "form-control sf_form_decoration";
  road_number.className = "form-control sf_form_decoration";
  estimate_road.className = "form-control sf_form_decoration";
  city_check_msg.className = "feedback_box good_feedback";
  msg_type.innerHTML = "Info"
  feedback_msg.innerHTML = " : Uniquement les biens du département des hauts-de-seine pour l'instant &#128515;";
  feedback_msg.style.color ="black";

 //https://api-adresse.data.gouv.fr/search/?q=16+avenue+henri+barbusse+colombes&postcode=92700

  var url_geo = "https://api-adresse.data.gouv.fr/search/?q=";
  url_geo += road_number.value;
  url_geo += "+";
  url_geo += estimate_road.value.replace(/\s+/g,'+');
  url_geo += "+";
  url_geo += city.toLowerCase();
  url_geo += "&postcode=";
  url_geo += cityMap.get(city);
  console.log(url_geo);

  fetch(url_geo)
  .then(function(resp) { return resp.json(); } )
  .then(function(data_geo) {
    if( data_geo.features.length != 0 )
    {
       lon = data_geo.features[0].geometry.coordinates[0];
       lat = data_geo.features[0].geometry.coordinates[1];

       next_url += "?lon=";
       next_url += lon;

       next_url += "&lat=";
       next_url += lat;
       next_url += "&postcode=";
       next_url += cityMap.get(city);
       completenexturl();
    }
    else {
      new_city.className = "form-control is-invalid sf_form_error";
      road_number.className = "form-control is-invalid sf_form_error";
      estimate_road.className = "form-control is-invalid sf_form_error";
      city_check_msg.className = "feedback_box bad_feedback";
      msg_type.innerHTML = ""
      feedback_msg.innerHTML = " Désolé, nous ne retrouvons pas votre adresse";
      feedback_msg.style.color ="red";
      validate.href = "#";
      return ;
    }

  })
  .catch(function(error) {
    error_msg.innerHTML = "Désolé, certains services ne sont pas démarrés";
    console.log(error);
    validate.href = "#";
    return ;
  });
}

if( validate)
validate.onclick=gotonextpage;

function removeElement(elementId) {
    // Removes an element from the document
    var element = document.getElementById(elementId);
    if(element)
    element.parentNode.removeChild(element);
}

const Params = new URLSearchParams(window.location.search);
const propType = Params.get('prop_type');

if( propType == "1")
{
  removeElement("ground_area_header");
  removeElement("ground_area");
}
else {
  removeElement("nb_floor_header");
  removeElement("nb_floor");
  removeElement("floor_header");
  removeElement("floor");
}
var divCaveY = document.getElementById("cave_y");
var divCaveN = document.getElementById("cave_n");
var caveState = true;
function handleEvtDownCaveY(event)
{
  if(caveState == false )
  {
    caveState = true;
    divCaveY.style.backgroundColor = "white";
    divCaveN.style.backgroundColor = "#bfe5f5";
  }
}
function handleEvtDownCaveN(event)
{
  if(caveState == true )
  {
    caveState = false;
    divCaveY.style.backgroundColor = "#bfe5f5";
    divCaveN.style.backgroundColor = "white";
  }
}

if( divCaveY && divCaveN )
{
  divCaveY.addEventListener(eventdown, handleEvtDownCaveY, true);
  divCaveN.addEventListener(eventdown, handleEvtDownCaveN, true);
}


/* Handle parking buttons */
var parking = document.querySelectorAll('.sf_parking');

var isParkingSelected = new Map();
for(i = 0; i < parking.length; ++i)
{
  isParkingSelected.set(parking[i], false);
}

function handleEvtDownParking(event)
{
  if(isParkingSelected.get(event.currentTarget) )
  {
    isParkingSelected.set(event.currentTarget, false);
    event.currentTarget.style.transform = "scale(0.85)";
    event.currentTarget.style.border = "";
  }
  else
  {
      isParkingSelected.set(event.currentTarget, true);
      event.currentTarget.style.transform = "scale(1)";
      event.currentTarget.style.border = "solid 3px #222545";

      for(i = 0; i < parking.length; ++i)
      {
          if( parking[i] != event.currentTarget )
          {
            isParkingSelected.set(parking[i], false);
            parking[i].style.transform = "scale(0.85)";
            parking[i].style.border = "";
          }
      }
  }
}

for(i = 0; i < parking.length; i++)
{
  parking[i].addEventListener(eventdown, handleEvtDownParking, true);
}

function isOneParkingSelected()
{

  for(i = 0; i < parking.length; ++i)
  {
    if(isParkingSelected.get(parking[i]) == true)
    {
      return [true,(i+1).toString()];
    }
  }
  return [false];
}

  var estimate = document.getElementById("estimate");

function submit_request()
{


  var results_url = "estimate_loading.html?" + Params;
  var nb_floor_value = document.getElementById("nb_floor_value");
  if(nb_floor_value)
  {
    results_url += "&nb_floor=";
    results_url += nb_floor_value.value;
  }

  var floor_value = document.getElementById("floor_value");
  if(floor_value)
  {
    results_url += "&floor=";
    results_url += floor_value.value;
  }

  var ground_area_value = document.getElementById("ground_area_value");
  if(ground_area_value)
  {
    results_url += "&ground_area=";
    results_url += ground_area_value.value;
  }

  if(caveState)
  {
    results_url += "&cave=1";
  }
  else {
    results_url += "&cave=0";
  }

  var selectedParking = isOneParkingSelected();

  if(selectedParking[0])
  {
    results_url += "&parking=";
    results_url += selectedParking[1];
  }

  estimate.href = results_url;

}

if(estimate)
estimate.onclick = submit_request;
