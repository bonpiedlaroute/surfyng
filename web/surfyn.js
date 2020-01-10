var pathname = window.location.pathname;
var pagename = pathname.split("/").pop();
var search_criteria_title = document.getElementById("search_criteria_title");
var inputPriceMin = document.getElementById("price_min");
var inputPriceMax = document.getElementById("price_max");
var inputAreaMin = document.getElementById("area_min");
var inputAreaMax = document.getElementById("area_max");

var valuePriceMin = document.getElementById("value_price_min");
var valuePriceMax = document.getElementById("value_price_max");
var valueAreaMin = document.getElementById("value_area_min");
var valueAreaMax = document.getElementById("value_area_max");

var mobile   = /Android|webOS|iPhone|iPad|iPod|BlackBerry/i.test(navigator.userAgent);
var eventdown = mobile ? "touchstart" : "mousedown";
var eventmove = mobile ? "touchmove" : "mousemove";
var eventup = mobile ? "touchend" : "mouseup";


var inputType = Object.freeze({"priceMin":0, "priceMax":1, "areaMin":2, "areaMax":3});

var offset = [0, 0, 0, 0];
var isDown = [false, false, false, false];
var values = [];
sessionStorage.setItem("scrollX", "0");
sessionStorage.setItem("scrollY", "0");
sessionStorage.setItem("needtosort", "");
sessionStorage.setItem("summary_json_data", "");
sessionStorage.setItem("search_type", "");
sessionStorage.setItem("search_city", "");
const Params = new URLSearchParams(window.location.search);
const searchType = Params.get('search_type');
if( searchType == "1")
{
  values[inputType.priceMin] = 50000;
  values[inputType.priceMax] = 4000000;
  valuePriceMin.innerHTML = "50 K€";
  valuePriceMax.innerHTML = "4000 K€";
  if( pagename == "criterederecherche.html")
  {
    document.title = "Vos critères de recherche d'achat";
    search_criteria_title.innerHTML += " d'achat";
  }
}
else {
  values[inputType.priceMin] = 100;
  values[inputType.priceMax] = 4000;
  valuePriceMin.innerHTML = "100 €";
  valuePriceMax.innerHTML = "4000 €";
  if( pagename == "criterederecherche.html")
  {
    document.title = "Vos critères de recherche de location";
    search_criteria_title.innerHTML += " de location";
  }
}
values[inputType.areaMin] = 10;
values[inputType.areaMax] = 400;


var startPosition = [inputPriceMin?inputPriceMin.offsetLeft:0, inputPriceMax?inputPriceMax.offsetLeft:0,
  inputAreaMin?inputAreaMin.offsetLeft:0, inputAreaMax?inputAreaMax.offsetLeft:0];
var lastPosition = [inputPriceMin?inputPriceMin.offsetLeft:0, inputPriceMax?inputPriceMax.offsetLeft:0,
  inputAreaMin?inputAreaMin.offsetLeft:0, inputAreaMax?inputAreaMax.offsetLeft:0];


function roundvalue(oldValue)
{
  var newValue = oldValue;

  var roundupNewValue = Math.ceil(newValue);

  if( (roundupNewValue - newValue) < 0.5)
  {
    newValue = Math.floor(newValue);
  }
  else {
    newValue = Math.ceil(newValue);
  }

  return newValue;
}

function computeValue(position, evtTypeMin, evtTypeMax)
{
  if( position == startPosition[evtTypeMin])
  {
    return values[evtTypeMin];
  }

  if( position == startPosition[evtTypeMax])
  {
    return values[evtTypeMax];
  }

  var minp = startPosition[evtTypeMin];
  var maxp = startPosition[evtTypeMax];

  var minv = Math.log(values[evtTypeMin]);
  var maxv = Math.log(values[evtTypeMax]);

  // calculate adjustment factor
  var scale = (maxv-minv) / (maxp-minp);

  return Math.exp(minv + scale*(position-minp));
}
function handleEvtDown(e, input, evttype)
{
  //console.log("Down");
  //console.log(e.changedTouches[0].clientX);
  isDown[evttype] = true;
  offset[evttype] = input.offsetLeft - (mobile ? e.changedTouches[0].clientX : e.clientX);
}
function handleEvtDownPriceMin(e)
{
  handleEvtDown(e, inputPriceMin, inputType.priceMin);
}
function handleEvtDownPriceMax(e)
{
  handleEvtDown(e, inputPriceMax, inputType.priceMax);;
}
function handleEvtDownAreaMin(e)
{
  handleEvtDown(e, inputAreaMin, inputType.areaMin);
}
function handleEvtDownAreaMax(e)
{
  handleEvtDown(e, inputAreaMax, inputType.areaMax);;
}
function handleEvtUp()
{
    isDown[inputType.priceMin] = false;
    isDown[inputType.priceMax] = false;
    isDown[inputType.areaMin] = false;
    isDown[inputType.areaMax] = false;
    //console.log("Up");
}

function handleEvtMove(event) {
    if(mobile === false)
    event.preventDefault();
    console.log("Move :");
    //console.log(event);
    //console.log(event.changedTouches[0].clientX);
    if (isDown[inputType.priceMin]) {
        //console.log(event);
        var newValue = (mobile ? event.changedTouches[0].clientX : event.clientX) + offset[inputType.priceMin];
        if( newValue <= startPosition[inputType.priceMin] )
        {
          newValue = startPosition[inputType.priceMin];
        }
        else {
          if ( newValue >= startPosition[inputType.priceMax])
          {
            newValue = startPosition[inputType.priceMax];
          }
          else
          {
            if(newValue > lastPosition[inputType.priceMax])
            {
              newValue = lastPosition[inputType.priceMax];
            }
          }
        }

        lastPosition[inputType.priceMin] = newValue;

        inputPriceMin.style.left = newValue + 'px';

        var newPriceMin = computeValue(newValue, inputType.priceMin, inputType.priceMax);


        if( searchType == "1")
        {
          newPriceMin = newPriceMin / 1000;
          newPriceMin = roundvalue(newPriceMin);
          valuePriceMin.innerHTML = newPriceMin + " K€";
        }
        else {
          newPriceMin = roundvalue(newPriceMin);
          valuePriceMin.innerHTML = newPriceMin + " €";
        }


      }
      else if (isDown[inputType.priceMax]) {

          var newValue = (mobile ? event.changedTouches[0].clientX : event.clientX) + offset[inputType.priceMax];
          if( newValue > startPosition[inputType.priceMax] )
          {
            newValue = startPosition[inputType.priceMax];
          }else {
            if( newValue < startPosition[inputType.priceMin] )
            {
              newValue = startPosition[inputType.priceMin];
            }
            else
            {
              if(newValue < lastPosition[inputType.priceMin])
              {
                newValue = lastPosition[inputType.priceMin];
              }
            }
          }
          lastPosition[inputType.priceMax] = newValue;
          inputPriceMax.style.left = newValue + 'px';

          var newPriceMax = computeValue(newValue, inputType.priceMin, inputType.priceMax);

          if( searchType == "1")
          {
            newPriceMax = newPriceMax / 1000;
            newPriceMax = roundvalue(newPriceMax);
            valuePriceMax.innerHTML = newPriceMax + " K€";
          }
          else{
            newPriceMax = roundvalue(newPriceMax);
            valuePriceMax.innerHTML = newPriceMax + " €";
          }


        }
        else {
          if (isDown[inputType.areaMin]) {

              var newValue = (mobile ? event.changedTouches[0].clientX : event.clientX) + offset[inputType.areaMin];
              if( newValue <= startPosition[inputType.areaMin] )
              {
                newValue = startPosition[inputType.areaMin];
              }
              else {
                if ( newValue >= startPosition[inputType.areaMax])
                {
                  newValue = startPosition[inputType.areaMax];
                }
                else
                {
                  if(newValue > lastPosition[inputType.areaMax])
                  {
                    newValue = lastPosition[inputType.areaMax];
                  }
                }
              }

              lastPosition[inputType.areaMin] = newValue;

              inputAreaMin.style.left = newValue + 'px';

              var newAreaMin = computeValue(newValue, inputType.areaMin, inputType.areaMax);

              newAreaMin = roundvalue(newAreaMin);


              valueAreaMin.innerHTML = newAreaMin + " m<sup>2";

            }
            else if (isDown[inputType.areaMax]) {

                var newValue = (mobile ? event.changedTouches[0].clientX : event.clientX) + offset[inputType.areaMax];
                if( newValue > startPosition[inputType.areaMax] )
                {
                  newValue = startPosition[inputType.areaMax];
                }else {
                  if( newValue < startPosition[inputType.areaMin] )
                  {
                    newValue = startPosition[inputType.areaMin];
                  }
                  else
                  {
                    if(newValue < lastPosition[inputType.areaMin])
                    {
                      newValue = lastPosition[inputType.areaMin];
                    }
                  }
                }
                lastPosition[inputType.areaMax] = newValue;
                inputAreaMax.style.left = newValue + 'px';

                var newAreaMax = computeValue(newValue, inputType.areaMin, inputType.areaMax);
                newAreaMax = roundvalue(newAreaMax);


                valueAreaMax.innerHTML = newAreaMax + " m<sup>2";

              }
        }
}

if( inputPriceMin && inputPriceMax && inputAreaMin && inputAreaMax)
{
  inputPriceMin.addEventListener(eventdown, handleEvtDownPriceMin, true);

  inputPriceMax.addEventListener(eventdown, handleEvtDownPriceMax, true);

  inputAreaMin.addEventListener(eventdown, handleEvtDownAreaMin, true);

  inputAreaMax.addEventListener(eventdown, handleEvtDownAreaMax, true);

  document.addEventListener(eventup, handleEvtUp, true);

  document.addEventListener(eventmove, handleEvtMove, true);

}




var divLoft = document.getElementById("loft");
var divHouse = document.getElementById("house");
var divApart = document.getElementById("apart");
var divOffice = document.getElementById("office");

var propertyType = Object.freeze({"loft":0, "house":1, "apart":2, "office":3});
var isSelected = [false, false, false, false];

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
    var propTypeErrorMsg = document.getElementById("prop_type_error_msg");
    propTypeErrorMsg.innerHTML ="";
  }
}
function handleEvtDownLoft(event)
{
  handleEvtDownPropertyType(event,divLoft, propertyType.loft);
}
function handleEvtDownHouse(event)
{
  handleEvtDownPropertyType(event,divHouse, propertyType.house);
}
function handleEvtDownApart(event)
{
  handleEvtDownPropertyType(event,divApart, propertyType.apart);
}
function handleEvtDownOffice(event)
{
  handleEvtDownPropertyType(event,divOffice, propertyType.office);
}

if( divLoft && divHouse && divApart && divOffice)
{
  divLoft.addEventListener(eventdown, handleEvtDownLoft, true);
  divHouse.addEventListener(eventdown, handleEvtDownHouse, true);
  divApart.addEventListener(eventdown, handleEvtDownApart, true);
  divOffice.addEventListener(eventdown, handleEvtDownOffice, true);
}


var validate = document.getElementById("validate");
var isFirstTimePropType = true;
var url_results = "";


function updatePropertyTypeUrlParam()
{
  url_results+="&prop_type=";
  isFirstTimePropType = false;
}
function updatePropertyTypeResult( value)
{
  if(isFirstTimePropType)
  {
    updatePropertyTypeUrlParam();
  }
  else {
    url_results+=","

  }
  url_results+=value;
}

var rooms = document.querySelectorAll('.sf_rooms');
var i = 0;
var isRoomsSelected = new Map();
if( rooms )
{
  for(i = 0; i < rooms.length; ++i)
  {
    isRoomsSelected.set(rooms[i], false);
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

function submit_request(event)
{
  var city = document.getElementById("search_city");
  url_results = "liste_annonces.html";

  if( checkCity() == false )
  {
    validate.href = "#"
    return ;
  }

  url_results += "?search_city=";
  url_results += city.value;

  url_results+="&search_type=";
  url_results+=searchType;

  var formattingLength= 0;
  if( searchType == "1")
    formattingLength = 3;
  else
    formattingLength = 2;

  url_results+="&price_min=";
  url_results+= valuePriceMin.innerHTML.substring(0, valuePriceMin.innerHTML.length - formattingLength);
  url_results+="&price_max=";
  url_results+= valuePriceMax.innerHTML.substring(0, valuePriceMax.innerHTML.length - formattingLength);
  var isOnePropertyTypeSelected = false;
  if(isSelected[propertyType.loft])
  {
    updatePropertyTypeUrlParam();

    url_results+="4";
    isOnePropertyTypeSelected = true;
  }
  if(isSelected[propertyType.house])
  {
    updatePropertyTypeResult("2");
    isOnePropertyTypeSelected = true;
  }
  if(isSelected[propertyType.apart])
  {
    updatePropertyTypeResult("1");
    isOnePropertyTypeSelected = true;
  }
  if(isSelected[propertyType.office])
  {
    updatePropertyTypeResult("3");
    isOnePropertyTypeSelected = true;
  }
  if(!isOnePropertyTypeSelected)
  {
    var propTypeErrorMsg = document.getElementById("prop_type_error_msg");
    propTypeErrorMsg.innerHTML = "* Vous devez indiquer au moins un type de bien";
    propTypeErrorMsg.style.color = "red";
    propTypeErrorMsg.style.fontSize = "12px";
    validate.href = "#";
    return;
  }
  url_results+="&area_min=";
  url_results+= valueAreaMin.innerHTML.substring(0, valueAreaMin.innerHTML.length - 14);
  url_results+="&area_max=";
  url_results+= valueAreaMax.innerHTML.substring(0, valueAreaMax.innerHTML.length - 14);;
  isFirstTimePropType = true;

  var selectedRooms = isOneRoomsSelected();
  if( selectedRooms[0] )
  {
    url_results += "&rooms="
    url_results += selectedRooms[1];
  }

  validate.href = url_results;
}

if( validate)
validate.onclick=submit_request;


function checkCity()
{
  var new_city = document.getElementById("search_city");
  var feedback_msg = document.getElementById("feedback_msg");
  var msg_type = document.getElementById("msg_type");
  var city_check_msg = document.getElementById("city_check_msg");
  if( new_city.value.trim().toUpperCase() == "COLOMBES" )
  {
    new_city.className = "form-control";
    city_check_msg.className = "feedback_box good_feedback";
    msg_type.innerHTML = "Info"
    feedback_msg.innerHTML = " : Uniquement la ville de Colombes pour l'instant &#128515;";
    feedback_msg.style.color ="black";
    return true;
  }
  else
  {
    new_city.className = "form-control is-invalid";
    city_check_msg.className = "feedback_box bad_feedback";
    msg_type.innerHTML = ""
    feedback_msg.innerHTML = " Désolé, nous supportons uniquement la ville de Colombes";
    feedback_msg.style.color ="red";
    return false;
  }
}

/* Handle rooms buttons */

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

if( rooms )
{
  for(i = 0; i < rooms.length; i++)
  {
    rooms[i].addEventListener(eventdown, handleEvtDownRooms, true);
  }
}
