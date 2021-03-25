var mobile   = /Android|webOS|iPhone|iPad|iPod|BlackBerry/i.test(navigator.userAgent);


var eventdown = mobile ? "touchstart" : "mousedown";
var eventmove = mobile ? "touchmove" : "mousemove";
var eventup = mobile ? "touchend" : "mouseup";

function isEmpty(str) {
    return (!str || 0 === str.length);
}


// First page

var sellSelect = document.getElementById("sell");
var rentSelect = document.getElementById("rent");

var loftSelect = document.getElementById("loft");
var houseSelect = document.getElementById("house");
var appartSelect = document.getElementById("appart");
var officeSelect = document.getElementById("office");

var surfaceInput = document.getElementById("surface_deposit");
	
var priceInput = document.getElementById("price_deposit");


// Second page
var descriptionInput = document.getElementById("deposit_description");

var imageInput1 = document.getElementById("uploadImage1");
var imageInput1 = document.getElementById("uploadImage2");
var imageInput1 = document.getElementById("uploadImage3");
var imageInput1 = document.getElementById("uploadImage4");


// Error fields
var title_error = document.getElementById("title_error_message");
var search_error = document.getElementById("category_error_message");
var prop_error = document.getElementById("prop_type_error_message");
var area_error = document.getElementById("area_error_message");
var rooms_error = document.getElementById("rooms_error_message");
var price_error = document.getElementById("price_error_message");

var descritpion_error = document.getElementById("descritpion_error_message");

function validate_first_step()
{	
	var prop_result = isPropertyTypeSelected();
	var rooms_result = isOneRoomsSelected();

 	if(checkInputs(input=0))
	{
	 	if((isSelectedSearch[searchType.sell] && !isSelectedSearch[searchType.rent]) || (!isSelectedSearch[searchType.sell] && isSelectedSearch[searchType.rent]))
	 	{
	 		if(rooms_result[0])
	 		{
	 			if(prop_result[0])
	 			{
	 				rooms_error.innerHTML = ""
	 				rooms_error.style.color = "";
	 				rooms_error.style.fontsize = "";

	 				prop_error.innerHTML = "";
	 				prop_error.style.color = "";
	 				prop_error.style.fontsize = "";

	 				sessionStorage.setItem("title", titleInput.value);
	 				sessionStorage.setItem("search_type", isSelectedSearch[searchType.sell] ? divSell.id: divRent.id);
	 				sessionStorage.setItem("prop_type", prop_result[1].id);
	 				sessionStorage.setItem("area", areaInput.value);
	 				sessionStorage.setItem("rooms", rooms_result[1]);
	 				sessionStorage.setItem("price", priceInput.value);

	 				window.location.href = "depot-annonce-2.html";
	 			}
	 			else
	 			{
	 				prop_error.innerHTML = "* Vous devez sélectionner le type de votre bien.";
 					prop_error.style.color = "red";
 					prop_error.style.fontsize = "12px";
 					return;
	 			}

	 		}
	 		else
	 		{
	 			rooms_error.innerHTML = "* Vous devez renseigner le nombre de pièces de votre bien."
				rooms_error.style.color = "red";
 				rooms_error.style.fontsize = "12px";
 				return;
	 		}
	 	}
	 	else
	 	{
	 		search_error.innerHTML = "* Vous devez sélectionner la catégorie de votre bien.";
			search_error.style.color = "red";
			search_error.style.fontsize = "12px";
			return;
	 	}
	}

	 
}

function validate_second_step()
{
	return;
}

function handle_errors()
{
	return;
}

// Handle actions for Search Type
var divSell = document.getElementById("sell")
var divRent = document.getElementById("rent")

var searchType = Object.freeze({
	"sell": 0,
	"rent": 1
});

var isSelectedSearch = [false, false];

function handleEventDownSell(event)
{
	if(isSelectedSearch[searchType.sell])
	{
		isSelectedSearch[searchType.sell] = false;
		divSell.style.transform = "scale(0.85)";
		divSell.style.border = "";
	}
	else
	{
		isSelectedSearch[searchType.sell] =  true;
		divSell.style.transform = "scale(1)";
		divSell.style.border = "solid 3px #222545";

		search_error.innerHTML = "";
		search_error.style.color = "";
		search_error.style.fontsize = "";

		if (isSelectedSearch[searchType.rent])
		{
			isSelectedSearch[searchType.rent] = false;
			divRent.style.transform = "scale(0.85)";
			divRent.style.border = "";
		}
	}
}

function handleEventDownRent(event)
{
	if(isSelectedSearch[searchType.rent])
	{
		isSelectedSearch[searchType.rent] = false;
		divRent.style.transform = "scale(0.85)";
		divRent.style.border = "";
	}
	else
	{
		isSelectedSearch[searchType.rent] =  true;
		divRent.style.transform = "scale(1)";
		divRent.style.border = "solid 3px #222545";

		search_error.innerHTML = "";
		search_error.style.color = "";
		search_error.style.fontsize = "";

		if (isSelectedSearch[searchType.sell])
		{
			isSelectedSearch[searchType.sell] = false;
			divSell.style.transform = "scale(0.85)";
			divSell.style.border = "";
		}
	}
}

if(divSell && divRent)
{
	divSell.addEventListener(eventdown, handleEventDownSell, true);
	divRent.addEventListener(eventdown, handleEventDownRent, true);
}

// Handle actions for Property Type
var divLoft = document.getElementById("loft");
var divHouse = document.getElementById("house");
var divAppart = document.getElementById("apart");
var divOffice = document.getElementById("office");

var propertyType = Object.freeze({
	"loft": 0, 
	"house": 1,
	"appart": 2,
	"office": 3
});

var reverseProperty = Object.freeze({
	"0": divLoft,
	"1": divHouse,
	"2": divAppart,
	"3": divOffice
})

var isSelected = [false, false, false, false];

function handleEventDownHouse(event)
{
	if(isSelected[propertyType.house])
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
		
		prop_error.innerHTML = "";
		prop_error.style.color = "";
		prop_error.style.fontsize = "";

		if(isSelected[propertyType.loft])
		{
			isSelected[propertyType.loft] = false;
			divLoft.style.transform = "scale(0.85)";
			divLoft.style.border = "";
		}

		if(isSelected[propertyType.appart])
		{
			isSelected[propertyType.appart] = false;
			divAppart.style.transform = "scale(0.85)";
			divAppart.style.border = "";
		}

		if(isSelected[propertyType.office])
		{
			isSelected[propertyType.office] = false;
			divOffice.style.transform = "scale(0.85)";
			divOffice.style.border = "";
		}
	}
}

function handleEventDownAppart(event)
{
	if(isSelected[propertyType.appart])
	{
		isSelected[propertyType.appart] = false;
		divAppart.style.transform = "scale(0.85)";
		divAppart.style.border = "";
	}
	else
	{
		isSelected[propertyType.appart] = true;
		divAppart.style.transform = "scale(1)";
		divAppart.style.border = "solid 3px #222545";
		
		prop_error.innerHTML = "";
		prop_error.style.color = "";
		prop_error.style.fontsize = "";

		if(isSelected[propertyType.loft])
		{
			isSelected[propertyType.loft] = false;
			divLoft.style.transform = "scale(0.85)";
			divLoft.style.border = "";
		}

		if(isSelected[propertyType.house])
		{
			isSelected[propertyType.house] = false;
			divHouse.style.transform = "scale(0.85)";
			divHouse.style.border = "";
		}

		if(isSelected[propertyType.office])
		{
			isSelected[propertyType.office] = false;
			divOffice.style.transform = "scale(0.85)";
			divOffice.style.border = "";
		}
	}
}

function handleEventDownLoft(event)
{
	if(isSelected[propertyType.loft])
	{
		isSelected[propertyType.loft] = false;
		divLoft.style.transform = "scale(0.85)";
		divLoft.style.border = "";
	}
	else
	{
		isSelected[propertyType.loft] = true;
		divLoft.style.transform = "scale(1)";
		divLoft.style.border = "solid 3px #222545";
		
		prop_error.innerHTML = "";
		prop_error.style.color = "";
		prop_error.style.fontsize = "";

		if(isSelected[propertyType.appart])
		{
			isSelected[propertyType.appart] = false;
			divAppart.style.transform = "scale(0.85)";
			divAppart.style.border = "";
		}

		if(isSelected[propertyType.house])
		{
			isSelected[propertyType.house] = false;
			divHouse.style.transform = "scale(0.85)";
			divHouse.style.border = "";
		}

		if(isSelected[propertyType.office])
		{
			isSelected[propertyType.office] = false;
			divOffice.style.transform = "scale(0.85)";
			divOffice.style.border = "";
		}
	}
}

function handleEventDownOffice(event)
{
	if(isSelected[propertyType.office])
	{
		isSelected[propertyType.office] = false;
		divOffice.style.transform = "scale(0.85)";
		divOffice.style.border = "";
	}
	else
	{
		isSelected[propertyType.office] = true;
		divOffice.style.transform = "scale(1)";
		divOffice.style.border = "solid 3px #222545";
		
		prop_error.innerHTML = "";
		prop_error.style.color = "";
		prop_error.style.fontsize = "";
		
		if(isSelected[propertyType.appart])
		{
			isSelected[propertyType.appart] = false;
			divAppart.style.transform = "scale(0.85)";
			divAppart.style.border = "";
		}

		if(isSelected[propertyType.house])
		{
			isSelected[propertyType.house] = false;
			divHouse.style.transform = "scale(0.85)";
			divHouse.style.border = "";
		}

		if(isSelected[propertyType.loft])
		{
			isSelected[propertyType.loft] = false;
			divLoft.style.transform = "scale(0.85)";
			divLoft.style.border = "";
		}
	}
}

if(divLoft && divHouse && divAppart && divHouse)
{
	divHouse.addEventListener(eventdown, handleEventDownHouse, true);
	divAppart.addEventListener(eventdown, handleEventDownAppart, true);
	divOffice.addEventListener(eventdown, handleEventDownOffice, true);
	divLoft.addEventListener(eventdown, handleEventDownLoft, true);
}

// Handle rooms buttons
var rooms = document.querySelectorAll('.sf_rooms');
var roomsErrorMsg = document.getElementById("rooms_error_msg");
var i = 0;
var isRoomsSelected = new Map();
for(i = 0; i < rooms.length; ++i)
{
  isRoomsSelected.set(rooms[i], false);
}

function handleEventDownRooms(event)
{
  if(isRoomsSelected.get(event.currentTarget))
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
	  rooms_error.innerHTML = "";
	  rooms_error.style.color = "";
	  for(i = 0; i < rooms.length; ++i)
	  {
	      if(rooms[i] != event.currentTarget)
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
  rooms[i].addEventListener(eventdown, handleEventDownRooms, true);
}

var areaInput = document.getElementById("surface_deposit");
var priceInput = document.getElementById("price_deposit");
var titleInput = document.getElementById("title_deposit");

var validate = document.getElementById("validate");

function checkInputs(input=0)
{
	if((isEmpty(titleInput.value) && input == 1) || (isEmpty(titleInput.value) && input == 0))
	{
		title_error.innerHTML = "* Vous devez renseigner un titre pour votre annonce.";
		title_error.style.color = "red";
		title_error.style.fontsize = "12px";
		validate.href = "javascript:void(0)";
		return false;
	}
	else
	{
		title_error.innerHTML = "";
		title_error.style.color = "";
		title_error.style.fontsize = "";
	}

	if((isEmpty(areaInput.value) && input == 2) || (isEmpty(areaInput.value) && input == 0))
	{
		area_error.innerHTML = "* Vous devez renseigner la surface de votre bien.";
		area_error.style.color = "red";
		area_error.style.fontsize = "12px";
		validate.href = "javascript:void(0)";
		return false;
	}
	else
	{
		area_error.innerHTML = "";
		area_error.style.color = "";
		area_error.style.fontsize = "";
		validate.href = "javascript:void(0)";
	}

	if((isEmpty(priceInput.value) && input == 3) || (isEmpty(priceInput.value) && input == 0))
	{
		price_error.innerHTML = "* Vous devez renseigner le prix de votre bien.";
		price_error.style.color = "red";
		price_error.style.fontsize = "12px";
		validate.href = "javascript:void(0)";
		return false;
	}
	else
	{
		price_error.innerHTML = "";
		price_error.style.color = "";
		price_error.style.fontsize = "";
	}

	return true;
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

function isPropertyTypeSelected()
{
	for(i=0; i < isSelected.length; i++)
	{
		if(isSelected[i])
		{
			return [true, reverseProperty[i]];
		}
	}
	return [false];
}

function allowOnlyNumber(e)
{
	var keypressed = e.which || e.keycode;
	if(keypressed >= 48 && keypressed <= 57)
		return true;
	else 
		return false;
}