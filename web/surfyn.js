var inputPriceMin = document.getElementById("price_min");
var inputPriceMax = document.getElementById("price_max");
var inputAreaMin = document.getElementById("area_min");
var inputAreaMax = document.getElementById("area_max");

var inputType = Object.freeze({"priceMin":0, "priceMax":1, "areaMin":2, "areaMax":3});

var offset = [0, 0, 0, 0];
var isDown = [false, false, false, false];
var values = [50000, 4000000, 10, 400];
var startPosition = [inputPriceMin.offsetLeft, inputPriceMax.offsetLeft, inputAreaMin.offsetLeft, inputAreaMax.offsetLeft];
var lastPosition = [inputPriceMin.offsetLeft, inputPriceMax.offsetLeft, inputAreaMin.offsetLeft, inputAreaMax.offsetLeft];


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
inputPriceMin.addEventListener('mousedown', handleEvtDownPriceMin, true);

inputPriceMax.addEventListener('mousedown', handleEvtDownPriceMax, true);

inputAreaMin.addEventListener('mousedown', handleEvtDownAreaMin, true);

inputAreaMax.addEventListener('mousedown', handleEvtDownAreaMax, true);

document.addEventListener('mouseup', handleEvtUp, true);

document.addEventListener('mousemove', handleEvtMove, true);
