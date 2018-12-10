var inputPriceMin = document.getElementById("price_min");
var inputPriceMax = document.getElementById("price_max");
var inputAreaMin = document.getElementById("area_min");
var inputAreaMax = document.getElementById("area_max");

var inputType = Object.freeze({"priceMin":0, "priceMax":1, "areaMin":2, "areaMax":3});

var offset = [0, 0, 0, 0];
var isDown = [false, false, false, false];
var values = [50000, 4000000, 10, 400];
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
  isDown[evttype] = true;
  offset[evttype] = input.offsetLeft - e.clientX;
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
}

function handleEvtMove(event) {
    event.preventDefault();
    if (isDown[inputType.priceMin]) {

        var newValue = event.clientX + offset[inputType.priceMin];
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

        newPriceMin = newPriceMin / 1000;
        newPriceMin = roundvalue(newPriceMin);


        inputPriceMin.innerHTML = newPriceMin + " K€";

      }
      else if (isDown[inputType.priceMax]) {

          var newValue = event.clientX + offset[inputType.priceMax];
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
          newPriceMax = newPriceMax / 1000;
          newPriceMax = roundvalue(newPriceMax);


          inputPriceMax.innerHTML = newPriceMax + " K€";

        }
        else {
          if (isDown[inputType.areaMin]) {

              var newValue = event.clientX + offset[inputType.areaMin];
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


              inputAreaMin.innerHTML = newAreaMin + " m2";

            }
            else if (isDown[inputType.areaMax]) {

                var newValue = event.clientX + offset[inputType.areaMax];
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


                inputAreaMax.innerHTML = newAreaMax + " m2";

              }
        }
}

if( inputPriceMin && inputPriceMax && inputAreaMin && inputAreaMax)
{
  inputPriceMin.addEventListener('mousedown', handleEvtDownPriceMin, true);

  inputPriceMax.addEventListener('mousedown', handleEvtDownPriceMax, true);

  inputAreaMin.addEventListener('mousedown', handleEvtDownAreaMin, true);

  inputAreaMax.addEventListener('mousedown', handleEvtDownAreaMax, true);

  document.addEventListener('mouseup', handleEvtUp, true);

  document.addEventListener('mousemove', handleEvtMove, true);

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
    divElt.style.border = "solid 3px blue";
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
  divLoft.addEventListener('mousedown', handleEvtDownLoft, true);
  divHouse.addEventListener('mousedown', handleEvtDownHouse, true);
  divApart.addEventListener('mousedown', handleEvtDownApart, true);
  divOffice.addEventListener('mousedown', handleEvtDownOffice, true);
}


var validate = document.getElementById("validate");
var isFirstTimePropType = true;
var url_results = "";


function updatePropertyTypeUrlParam()
{
  url_results+="?prop_type=";
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
function submit_request(event)
{
  url_results = "results_summary.html?";
  const urlParams = new URLSearchParams(window.location.search);
  const search_type = urlParams.get('search_type');
  url_results+="search_type=";
  url_results+=search_type;
  url_results+="?price_min=";
  url_results+= inputPriceMin.innerHTML;
  url_results+="?price_max=";
  url_results+= inputPriceMax.innerHTML;

  if(isSelected[propertyType.loft])
  {
    updatePropertyTypeUrlParam();

    url_results+="4";
  }
  if(isSelected[propertyType.house])
  {
    updatePropertyTypeResult("2");
  }
  if(isSelected[propertyType.apart])
  {
    updatePropertyTypeResult("1");
  }
  if(isSelected[propertyType.office])
  {
    updatePropertyTypeResult("3");
  }

  url_results+="?area_min=";
  url_results+= inputAreaMin.innerHTML;
  url_results+="?area_max=";
  url_results+= inputAreaMax.innerHTML;

  validate.href = url_results;
}

if( validate)
validate.onclick=submit_request;
console.log(url_results);
/*
const userAction = async () => {
  const response = await fetch('http://surfyn.xyz/search/');
  const myJson = await response.json(); //extract JSON from the http response
  // do something with myJson
}*/
