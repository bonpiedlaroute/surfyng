var mobile = /Android|webOS|iPhone|iPad|iPod|BlackBerry/i.test(navigator.userAgent);
var dataParams = {};


var eventdown = mobile ? "touchstart" : "mousedown";
var eventmove = mobile ? "touchmove" : "mousemove";
var eventup = mobile ? "touchend" : "mouseup";

// var url = 'https://surfyn.fr:7878/announcedeposit';
var url = "http://localhost:7878/announcedeposit";


if (!firebase.apps.length) {
    var firebaseConfig = {
        apiKey: "AIzaSyAWjNYovhM8IDUWL_ZGZ9jnb4i14dLQSe4",
        authDomain: "surfyn.firebaseapp.com",
        databaseURL: "https://surfyn.firebaseio.com",
        projectId: "surfyn",
        storageBucket: "surfyn.appspot.com",
        messagingSenderId: "1080663386908",
        appId: "1:1080663386908:web:d0c38731a4d55de5dea8d9",
        measurementId: "G-7X8E11EEFD"
    };
    console.log("firebase initialization");
    firebase.initializeApp(firebaseConfig);
}

// Storage instance
var storage = firebase.storage();
// Storage reference
var storageRef = storage.ref();

function isEmpty(str) {
    return (!str || 0 === str.length);
}

function isConnectedUser() {
    return sessionStorage["user_id"];
}

function backHome() {
    window.location.href = "/";
}

// First page

var sellSelect = document.getElementById("sell");
var rentSelect = document.getElementById("rent");

var loftSelect = document.getElementById("loft");
var houseSelect = document.getElementById("house");
var appartSelect = document.getElementById("appart");
var officeSelect = document.getElementById("office");

var priceInput = document.getElementById("price_deposit");


// Second page
var descriptionInput = document.getElementById("deposit_description");
var parkingInput = document.getElementById("parking");
var parkingNumberInput = document.getElementById("parking_number");
var cellarInput = document.getElementById("cellar");
var cellarNumberInput = document.getElementById("cellar_number");
var constructionYearInput = document.getElementById("construction_year");
var heatingInput = document.getElementById("type_of_heating");
var liftInput = document.getElementById("lift");
var balconyInput = document.getElementById("balcony");
var floorInput = document.getElementById("floor");
var floorDiv = document.getElementById("floor_div");


var imageInput1 = document.getElementById("uploadImage1");
var imageInput1 = document.getElementById("uploadImage2");
var imageInput1 = document.getElementById("uploadImage3");
var imageInput1 = document.getElementById("uploadImage4");


// Error fields
var title_error = document.getElementById("title_error_message");
var search_error = document.getElementById("category_error_message");
var city_error = document.getElementById("city_error_message");
var prop_error = document.getElementById("prop_type_error_message");
var area_error = document.getElementById("area_error_message");
var rooms_error = document.getElementById("rooms_error_message");
var bedrooms_error = document.getElementById("bedrooms_error_message");
var price_error = document.getElementById("price_error_message");

var description_error = document.getElementById("description_error_message");
var address_error = document.getElementById("address_error_message");
var phone_error = document.getElementById("phone_error_message");
var images_error = document.getElementById("images_error_message");

function validate_first_step() {
    var prop_result = isPropertyTypeSelected();
    var rooms_result = isOneRoomsSelected();
    var bedrooms_result = isOneBedroomsSelected();

    if (isConnectedUser()) {
        if (checkInputs(input = 0)) {
            if ((isSelectedSearch[searchType.sell] && !isSelectedSearch[searchType.rent]) || (!isSelectedSearch[searchType.sell] && isSelectedSearch[searchType.rent])) {
                if (rooms_result[0]) {
                    if (bedrooms_result[0]) {
                        if (prop_result[0]) {
                            rooms_error.innerHTML = ""
                            rooms_error.style.color = "";
                            rooms_error.style.fontsize = "";

                            prop_error.innerHTML = "";
                            prop_error.style.color = "";
                            prop_error.style.fontsize = "";

                            sessionStorage.setItem("title", titleInput.value);
                            sessionStorage.setItem("search_type", isSelectedSearch[searchType.sell] ? divSell.id : divRent.id);
                            sessionStorage.setItem("prop_type", prop_result[1].id);
                            sessionStorage.setItem("area", areaInput.value);
                            if(!isEmpty(landSurfaceInput.value))
                                sessionStorage.setItem("land_surface", landSurfaceInput.value);
                            sessionStorage.setItem("rooms", rooms_result[1]);
                            sessionStorage.setItem("bedrooms", bedrooms_result[1]);
                            sessionStorage.setItem("price", priceInput.value);
                            sessionStorage.setItem("city", cityInput.value);

                            window.location.href = "valider-deposer-une-annonce.html";
                        } else {
                            prop_error.innerHTML = "* Vous devez sélectionner le type de votre bien.";
                            prop_error.style.color = "red";
                            prop_error.style.fontsize = "12px";
                            return;
                        }
                    }
                    else {
                        bedrooms_error.innerHTML = "* Vous devez renseigner le nombre de chambre de votre bien";
                        bedrooms_error.style.color = "red";
                        bedrooms_error.fontsize = "12px";
                        return;
                    }

                } else {
                    rooms_error.innerHTML = "* Vous devez renseigner le nombre de pièces de votre bien."
                    rooms_error.style.color = "red";
                    rooms_error.style.fontsize = "12px";
                    return;
                }
            } else {
                search_error.innerHTML = "* Vous devez sélectionner la catégorie de votre bien.";
                search_error.style.color = "red";
                search_error.style.fontsize = "12px";
                return;
            }
        }
    } else {
        $("#login").modal({
            focus: true,
            show: true
        });
    }


}

function validate_second_step() {

    if (isConnectedUser()) {
        if (checkAddress()) {
            if (checkDescription()) {
                if (isAllImagesSet()) {
                    if (checkPhone()) {
                        sessionStorage.setItem("address", addressInput.value);
                        sessionStorage.setItem("description", descriptionInput.value)
                        sessionStorage.setItem("parking", parkingInput.checked ? parkingNumberInput.value : 'Non');
                        sessionStorage.setItem("cellar", cellarInput.checked ? cellarNumberInput.value : 'Non');
                        sessionStorage.setItem("type_of_heating", heatingInput.value);
                        sessionStorage.setItem("phone", phoneInput.value);
                        sessionStorage.setItem("construction_year", constructionYearInput.value);
                        sessionStorage.setItem("lift", liftInput.checked ? 'Oui' : 'Non');
                        sessionStorage.setItem("balcony", balconyInput.checked ? 'Oui' : 'Non');

                        if(liftInput.checked)
                        {
                            sessionStorage.setItem("floor", floorInput.value);
                            dataParams["floor"] = floorInput.value;
                        }

                        if (isEmpty(phoneInput.value) === false)
                            dataParams["phone"] = phoneInput.value;
                        
                        if (isEmpty(construction_year) === false)
                            dataParams["construction_year"] = constructionYearInput.value;

                        dataParams["address"] = addressInput.value;
                        dataParams["description"] = descriptionInput.value;
                        dataParams["parking"] = parkingInput.checked ? parkingNumberInput.value : 'Non';
                        dataParams["cellar"] = cellarInput.checked ? cellarNumberInput.value : 'Non';
                        dataParams["type_of_heating"] = heatingInput.value;
                        dataParams["lift"] = sessionStorage.getItem("lift");
                        dataParams["balcony"] = sessionStorage.getItem("balcony");

                        console.log(dataParams);
                        if (buildParams()) {
                            console.log(url);
                            fetch(url, {
                                method: "POST",
                                body: JSON.stringify(dataParams),
                                referrer: "same-origin",
                            })
                                .then(function (response) {
                                    if(response.ok)
                                        showSuccessAnnounceDeposit();
                                    else
                                        showErrorAnnounceDeposit();
                                })
                                .catch(function (error) {
                                    console.log(error);
                                });
                        }
                    }
                } else {
                    images_error.innerHTML = "* Vous devez sélectionner 4 images pour décrire votre bien.";
                    images_error.style.color = "red";
                    images_error.style.fontsize = "12px";
                }
            }
        }
    } else {
        $("#login").modal({
            focus: true,
            show: true
        });
    }
}

function showSuccessAnnounceDeposit() {
    alert("Annonce enregistrer avec succès");
    window.location.href = "mesrecherches.html";
}

function showErrorAnnounceDeposit() {
    alert("Erreur: Impossible d'enregistrer l'annonce");
}

function buildParams() {
    if (sessionStorage.getItem("title"))
        dataParams["title"] = sessionStorage.getItem("title");
    else
        return false;

    if (sessionStorage.getItem("search_type"))
        dataParams["search_type"] = searchType[sessionStorage.getItem("search_type")] + 1;
    else
        return false;

    if (sessionStorage.getItem("prop_type"))
        dataParams["prop_type"] = propertyType[sessionStorage.getItem("prop_type")];
    else
        return false;

    if (sessionStorage.getItem("area"))
        dataParams["surface"] = sessionStorage.getItem("area");
    else
        return false;
    
    if (sessionStorage.getItem("land_surface"))
        dataParams["land_surface"] = sessionStorage.getItem("land_surface");
    else
        return false;

    if (sessionStorage.getItem("rooms"))
        dataParams["rooms"] = sessionStorage.getItem("rooms");
    else
        return false;

    if (sessionStorage.getItem("bedrooms"))
        dataParams["bedrooms"] = sessionStorage.getItem("bedrooms");
    else
        return false;

    if (sessionStorage.getItem("price"))
        dataParams["price"] = sessionStorage.getItem("price");
    else
        return false;

    if (sessionStorage.getItem("city"))
        dataParams["city"] = sessionStorage.getItem("city");

    if (sessionStorage.getItem("user_id"))
        dataParams["user_id"] = sessionStorage.getItem("user_id");
    else
        return false;

    return true;
}

function handle_errors() {
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

function handleEventDownSell(event) {
    if (isSelectedSearch[searchType.sell]) {
        isSelectedSearch[searchType.sell] = false;
        divSell.style.transform = "scale(0.85)";
        divSell.style.border = "";
    } else {
        isSelectedSearch[searchType.sell] = true;
        divSell.style.transform = "scale(1)";
        divSell.style.border = "solid 3px #222545";

        search_error.innerHTML = "";
        search_error.style.color = "";
        search_error.style.fontsize = "";

        if (isSelectedSearch[searchType.rent]) {
            isSelectedSearch[searchType.rent] = false;
            divRent.style.transform = "scale(0.85)";
            divRent.style.border = "";
        }
    }
}

function handleEventDownRent(event) {
    if (isSelectedSearch[searchType.rent]) {
        isSelectedSearch[searchType.rent] = false;
        divRent.style.transform = "scale(0.85)";
        divRent.style.border = "";
    } else {
        isSelectedSearch[searchType.rent] = true;
        divRent.style.transform = "scale(1)";
        divRent.style.border = "solid 3px #222545";

        search_error.innerHTML = "";
        search_error.style.color = "";
        search_error.style.fontsize = "";

        if (isSelectedSearch[searchType.sell]) {
            isSelectedSearch[searchType.sell] = false;
            divSell.style.transform = "scale(0.85)";
            divSell.style.border = "";
        }
    }
}

if (divSell && divRent) {
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
    "apart": 2,
    "office": 3
});

var reverseProperty = Object.freeze({
    "0": divLoft,
    "1": divHouse,
    "2": divAppart,
    "3": divOffice
})

var isSelected = [false, false, false, false];

function handleEventDownHouse(event) {
    if (isSelected[propertyType.house]) {
        isSelected[propertyType.house] = false;
        divHouse.style.transform = "scale(0.85)";
        divHouse.style.border = "";
    } else {
        isSelected[propertyType.house] = true;
        divHouse.style.transform = "scale(1)";
        divHouse.style.border = "solid 3px #222545";

        prop_error.innerHTML = "";
        prop_error.style.color = "";
        prop_error.style.fontsize = "";

        if (isSelected[propertyType.loft]) {
            isSelected[propertyType.loft] = false;
            divLoft.style.transform = "scale(0.85)";
            divLoft.style.border = "";
        }

        if (isSelected[propertyType.apart]) {
            isSelected[propertyType.apart] = false;
            divAppart.style.transform = "scale(0.85)";
            divAppart.style.border = "";
        }

        if (isSelected[propertyType.office]) {
            isSelected[propertyType.office] = false;
            divOffice.style.transform = "scale(0.85)";
            divOffice.style.border = "";
        }
    }
}

function handleEventDownAppart(event) {
    if (isSelected[propertyType.apart]) {
        isSelected[propertyType.apart] = false;
        divAppart.style.transform = "scale(0.85)";
        divAppart.style.border = "";
    } else {
        isSelected[propertyType.apart] = true;
        divAppart.style.transform = "scale(1)";
        divAppart.style.border = "solid 3px #222545";

        prop_error.innerHTML = "";
        prop_error.style.color = "";
        prop_error.style.fontsize = "";

        if (isSelected[propertyType.loft]) {
            isSelected[propertyType.loft] = false;
            divLoft.style.transform = "scale(0.85)";
            divLoft.style.border = "";
        }

        if (isSelected[propertyType.house]) {
            isSelected[propertyType.house] = false;
            divHouse.style.transform = "scale(0.85)";
            divHouse.style.border = "";
        }

        if (isSelected[propertyType.office]) {
            isSelected[propertyType.office] = false;
            divOffice.style.transform = "scale(0.85)";
            divOffice.style.border = "";
        }
    }
}

function handleEventDownLoft(event) {
    if (isSelected[propertyType.loft]) {
        isSelected[propertyType.loft] = false;
        divLoft.style.transform = "scale(0.85)";
        divLoft.style.border = "";
    } else {
        isSelected[propertyType.loft] = true;
        divLoft.style.transform = "scale(1)";
        divLoft.style.border = "solid 3px #222545";

        prop_error.innerHTML = "";
        prop_error.style.color = "";
        prop_error.style.fontsize = "";

        if (isSelected[propertyType.apart]) {
            isSelected[propertyType.apart] = false;
            divAppart.style.transform = "scale(0.85)";
            divAppart.style.border = "";
        }

        if (isSelected[propertyType.house]) {
            isSelected[propertyType.house] = false;
            divHouse.style.transform = "scale(0.85)";
            divHouse.style.border = "";
        }

        if (isSelected[propertyType.office]) {
            isSelected[propertyType.office] = false;
            divOffice.style.transform = "scale(0.85)";
            divOffice.style.border = "";
        }
    }
}

function handleEventDownOffice(event) {
    if (isSelected[propertyType.office]) {
        isSelected[propertyType.office] = false;
        divOffice.style.transform = "scale(0.85)";
        divOffice.style.border = "";
    } else {
        isSelected[propertyType.office] = true;
        divOffice.style.transform = "scale(1)";
        divOffice.style.border = "solid 3px #222545";

        prop_error.innerHTML = "";
        prop_error.style.color = "";
        prop_error.style.fontsize = "";

        if (isSelected[propertyType.apart]) {
            isSelected[propertyType.apart] = false;
            divAppart.style.transform = "scale(0.85)";
            divAppart.style.border = "";
        }

        if (isSelected[propertyType.house]) {
            isSelected[propertyType.house] = false;
            divHouse.style.transform = "scale(0.85)";
            divHouse.style.border = "";
        }

        if (isSelected[propertyType.loft]) {
            isSelected[propertyType.loft] = false;
            divLoft.style.transform = "scale(0.85)";
            divLoft.style.border = "";
        }
    }
}

if (divLoft && divHouse && divAppart && divHouse) {
    divHouse.addEventListener(eventdown, handleEventDownHouse, true);
    divAppart.addEventListener(eventdown, handleEventDownAppart, true);
    divOffice.addEventListener(eventdown, handleEventDownOffice, true);
    divLoft.addEventListener(eventdown, handleEventDownLoft, true);
}

// Handle rooms and bedrooms buttons
var rooms = document.querySelectorAll('.sf_rooms');
var bedrooms = document.querySelectorAll('.sf_bedrooms');

var i = 0;
var isRoomsSelected = new Map();
var isBedroomsSelected = new Map();

for (i = 0; i < rooms.length; ++i) {
    isRoomsSelected.set(rooms[i], false);
    isBedroomsSelected.set(bedrooms[i], false);
}

function handleEventDownRooms(event, is_rooms) {
    if (is_rooms) {
        if (isRoomsSelected.get(event.currentTarget)) {
            isRoomsSelected.set(event.currentTarget, false);
            event.currentTarget.style.transform = "scale(0.85)";
            event.currentTarget.style.border = "";
        }
        else {
            isRoomsSelected.set(event.currentTarget, true);
            event.currentTarget.style.transform = "scale(1)";
            event.currentTarget.style.border = "solid 3px #222545";
            rooms_error.innerHTML = "";
            rooms_error.style.color = "";
            for (i = 0; i < rooms.length; ++i) {
                if (rooms[i] != event.currentTarget) {
                    isRoomsSelected.set(rooms[i], false);
                    rooms[i].style.transform = "scale(0.85)";
                    rooms[i].style.border = "";
                }
            }
        }
    }
    else {
        if (isBedroomsSelected.get(event.currentTarget)) {
            isBedroomsSelected.set(event.currentTarget, false);
            event.currentTarget.style.transform = "scale(0.85)";
            event.currentTarget.style.border = "";
        }
        else {
            isBedroomsSelected.set(event.currentTarget, true);
            event.currentTarget.style.transform = "scale(1)";
            event.currentTarget.style.border = "solid 3px #222545";
            bedrooms_error.innerHTML = "";
            bedrooms_error.style.color = "";
            for (i = 0; i < bedrooms.length; ++i) {
                if (bedrooms[i] != event.currentTarget) {
                    isBedroomsSelected.set(bedrooms[i], false);
                    bedrooms[i].style.transform = "scale(0.85)";
                    bedrooms[i].style.border = "";
                }
            }
        }
    }
}

for (i = 0; i < rooms.length; i++) {
    rooms[i].addEventListener(eventdown, function (event) { handleEventDownRooms(event, true) }, true);
    bedrooms[i].addEventListener(eventdown, function (event) { handleEventDownRooms(event, false) }, true);
}

var areaInput = document.getElementById("surface_deposit");
var landSurfaceInput = document.getElementById("land_surface");
var priceInput = document.getElementById("price_deposit");
var titleInput = document.getElementById("title_deposit");
var cityInput = document.getElementById("city_deposit");
var addressInput = document.getElementById("address_deposit");
var phoneInput = document.getElementById("phone_deposit");

var validate = document.getElementById("validate");

function checkInputs(input = 0) {
    if (titleInput) {
        if ((isEmpty(titleInput.value) && input == 1) || (isEmpty(titleInput.value) && input == 0)) {
            title_error.innerHTML = "* Vous devez renseigner un titre pour votre annonce.";
            title_error.style.color = "red";
            title_error.style.fontsize = "12px";
            validate.href = "javascript:void(0)";
            return false;
        } else {
            title_error.innerHTML = "";
            title_error.style.color = "";
            title_error.style.fontsize = "";
        }
    }

    if (areaInput) {
        if ((isEmpty(areaInput.value) && input == 2) || (isEmpty(areaInput.value) && input == 0)) {
            area_error.innerHTML = "* Vous devez renseigner la surface de votre bien.";
            area_error.style.color = "red";
            area_error.style.fontsize = "12px";
            validate.href = "javascript:void(0)";
            return false;
        } else {
            area_error.innerHTML = "";
            area_error.style.color = "";
            area_error.style.fontsize = "";
        }
    }

    if (priceInput) {
        if ((isEmpty(priceInput.value) && input == 3) || (isEmpty(priceInput.value) && input == 0)) {
            price_error.innerHTML = "* Vous devez renseigner le prix de votre bien.";
            price_error.style.color = "red";
            price_error.style.fontsize = "12px";
            validate.href = "javascript:void(0)";
            return false;
        } else {
            price_error.innerHTML = "";
            price_error.style.color = "";
            price_error.style.fontsize = "";
        }
    }

    if (cityInput) {
        if ((isEmpty(cityInput.value) && input == 4) || (isEmpty(cityInput.value) && input == 0)) {
            city_error.innerHTML = "* Vous devez renseigner la ville de votre bien.";
            city_error.style.color = "red";
            city_error.style.fontsize = "12px";
            validate.href = "javascript:void(0)";
            return false;
        } else {
            if (available_cities.includes(cityInput.value)) {
                city_error.innerHTML = "";
                city_error.style.color = "";
                city_error.style.fontsize = "";
            } else {
                if (input == 4) {
                    city_error.innerHTML = "* Vous devez renseigner une des villes prises en compte (Houilles, Puteaux, Colombes et Nanterre).";
                    city_error.style.color = "red";
                    city_error.style.fontsize = "12px";
                    validate.href = "javascript:void(0)";
                    return false;
                }

            }

        }
    }


    return true;
}

function isOneRoomsSelected() {
    for (i = 0; i < rooms.length; ++i) {
        if (isRoomsSelected.get(rooms[i]) == true) {
            return [true, (i + 1).toString()];
        }

    }
    return [false];
}

function isOneBedroomsSelected() {
    for (i = 0; i < bedrooms.length; i++) {
        if (isBedroomsSelected.get(bedrooms[i]) == true) {
            return [true, (i + 1).toString()];
        }
    }
    return [false];
}

function isPropertyTypeSelected() {
    for (i = 0; i < isSelected.length; i++) {
        if (isSelected[i]) {
            return [true, reverseProperty[i]];
        }
    }
    return [false];
}

function allowOnlyNumber(e) {
    var keypressed = e.which || e.keycode;
    if (keypressed >= 48 && keypressed <= 57)
        return true;
    else
        return false;
}

// Functions to manage second step form

function checkAddress() {
    if (isEmpty(addressInput.value)) {
        address_error.innerHTML = "* Vous devez renseigner l'adresse de votre bien."
        address_error.style.color = "red";
        address_error.style.fontsize = "12px";
        validate.href = "javascript:void(0)";
        return false;
    } else {
        address_error.innerHTML = "";
        address_error.style.color = "";
        address_error.style.fontsize = "";
    }

    return true;
}


function checkDescription() {
    if (isEmpty(descriptionInput.value)) {
        description_error.innerHTML = "* Vous devez renseigner une description de votre bien.";
        description_error.style.color = "red";
        description_error.style.fontsize = "12px";
        validate.href = "javascript:void(0)";
        return false;

    } else {
        description_error.innerHTML = "";
        description_error.style.color = "";
        description_error.style.fontsize = "";
    }

    return true;
}

function checkPhone() {
    if (isEmpty(phoneInput.value) === false && phoneInput.value.length < 10) {
        phone_error.innerHTML = "* Vous devez renseigner un numéro de téléphone français."
        phone_error.style.color = "red";
        phone_error.style.fontsize = "12px";
        validate.href = "javascript:void(0)";
        return false;
    } else {
        phone_error.innerHTML = "";
        phone_error.style.color = "";
        phone_error.style.fontsize = "";
    }

    return true;
}

// Handle images
var imageInput1 = document.getElementById("uploadImage1");
var imageInput2 = document.getElementById("uploadImage2");
var imageInput3 = document.getElementById("uploadImage3");
var imageInput4 = document.getElementById("uploadImage4");
var imageInput5 = document.getElementById("uploadImage5");


var images = [false, false, false, false, false];

function validateAndUpload(input) {
    var url = window.URL || window.webkitURL;
    var file = input.files[0];
    var progress_text = document.getElementById("progress");

    if (file) {
        var image = new Image();

        image.onerror = function () {
            alert("Invalid image");
        };

        image.src = URL.createObjectURL(file);
        sessionStorage.setItem('image' + input.id.slice(-1), image);
        // imagesData.append('image'+input.id.slice(-1), file);

        // Upload image on Cloud Storage Bucket
        var uploadTask = storageRef.child('deposit/puteaux/' + file.name).put(file);

        var index = input.id.slice(-1);
        console.log('Image ' + index);
        // Listen for error and completion of the upload
        uploadTask.on(firebase.storage.TaskEvent.STATE_CHANGED,

            (snapshot) => {
                var progress = (snapshot.bytesTransferred / snapshot.totalBytes) * 100;
                // console.log('Upload is ' + progress + '% done.');
                if (progress == 100) {

                    images[index - 1] = true;

                    $("#overlay-" + index).css("display", "none");
                    $("#imageOK" + index).css("display", "block");
                    setTimeout(function () {
                        $("#imageOK" + index).css("display", "none");
                    }, 3000);

                }
                switch (snapshot.state) {
                    case firebase.storage.TaskEvent.PAUSED:
                        console.log('Upload is Paused');
                        break;
                    case firebase.storage.TaskEvent.RUNNING:
                        console.log('Upload is running')
                        break;
                }
            },

            (error) => {
                switch (error.code) {
                    case 'storage/unauthorized':
                        console.log('Error: Upload unauthorized');
                    case 'storage/canceled':
                        console.log('Error: Upload canceled');
                    case 'storage/unknow':
                        console.log('Error: ' + error.serverResponse);
                }
            },

            () => {
                // Upload completed successfully
                uploadTask.snapshot.ref.getDownloadURL().then((downloadURL) => {
                    console.log('File available at', downloadURL);
                    dataParams['image' + input.id.slice(-1)] = downloadURL;
                });
            }
        );
    }
}

function isAllImagesSet() {
    for (i = 0; i < images.length; i++) {
        if (images[i] === false) {
            return false;
        }
    }
    images_error.innerHTML = "";
    images_error.style.color = "";
    images_error.style.fontsize = "";
    return true;
}

function display_number_field(input)
{
    if(input.id == "parking")
    {
        if(input.checked)
            parkingNumberInput.style.display = "block";
        else
            parkingNumberInput.style.display = "none"
    }
    else if(input.id == 'cellar')
    {
        if(input.checked)
            cellarNumberInput.style.display = "block";
        else
            cellarNumberInput.style.display = "none";
    }
}

function display_floor(input)
{
    console.log(input.checked);
    if(input.checked)
        floorDiv.style.display = "flex";
    else
        floorDiv.style.display = "none";
}

function clearStorage()
{
    sessionStorage.removeItem("");
}