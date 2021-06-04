function loadJSON(path, success, error) {

    var xobj = new XMLHttpRequest();
    xobj.overrideMimeType("application/json");
    xobj.open('GET', path, true);
    xobj.onreadystatechange = function () {
        if (xobj.readyState == 4 && xobj.status == "200") {
            // Required use of an anonymous callback as .open will NOT return a value but simply returns undefined in asynchronous mode
            if (success)
                success(xobj);
        }
        else {
            if (error) {
                error(xobj);
            }
        }
    };
    xobj.send(null);
}

function createNode(element) {
    return document.createElement(element);
}

//const url_dt = 'https://surfyn.fr:7878/search/ad?id='+ window.location.search.substr(1);
 const url_dt = 'http://127.0.0.1:7878/search/ad?id=' + window.location.search.substr(1);

console.log(url_dt);


// fetch(url_dt)
//     .then(function (resp) { return resp.json(); })
//     .then(function (data) {
//         generate_details_page_surfyn(data);
//     })
//     .catch(function (error) {
//         console.log(error);
//     });


function generate_details_page_surfyn(data) {
    if (data == undefined) {
        document.title = "Annonce introuvable - Surfyn";
        document.getElementsByTagName('meta')["description"].content = "Cette annonce n'est plus disponible";
        var announces_found = document.getElementById("nb_announces_found");
        announces_found.innerHTML = "Cette annonce n'est plus disponible";
        const metaRobots = document.createElement('meta');
        metaRobots.name = 'robots';
        metaRobots.content = 'noindex';
        document.head.appendChild(metaRobots);
    }
    else {
        var ismobile = /Android|webOS|iPhone|iPad|iPod|BlackBerry/i.test(navigator.userAgent);

        // Page Title

        var pagetitle = "";
        pagetitle += data[0].PROPERTY_TYPE + " ";

        pagetitle += data[0].ROOMS;
        pagetitle += " pièce(s) à ";

        pagetitle += data[0].SEARCH_TYPE == "For sale" ? "vendre " : "louer ";

        pagetitle += data[0].CITY;
        pagetitle += " (" + getPostalCode(data[0].CITY) + ") ";

        pagetitle += String(data[0].SURFACE);
        pagetitle += " m2 ";
        pagetitle += String(data[0].PRICE);
        pagetitle += " €";

        // META Description
        document.getElementsByTagName("meta")["description"].content = pagetitle;
        document.getElementsByTagName("meta")["description"].content += ". " + data[0].AD_TEXT_DESCRIPTION.slice(0, 150);

        pagetitle += " - Surfyn"

        document.title = pagetitle

        // var main = document.getElementsByClassName("container")[0];
        var main = document.getElementById("main-content");
        main.className = "container";

        var title_header = createNode("h1");
        title_header.id = "ad_title";
        title_header.className = "mt-4 mb-2 text-center";

        var address_header = createNode("h5");
        address_header.id = "ad_address";
        address_header.className = "text-center text-muted";

        var department_header = createNode("h5");
        department_header.id = "ad_dp";
        department_header.className = "text-center mb-4 text-muted";

        var row_container = createNode("div");
        row_container.className = "row";

        var container = createNode("div");
        container.id = "subcontainer";
        container.className = "col-sm-12 col-md-12 col-lg-8";

        var side_container = createNode("div");
        side_container.id = "side_container";
        side_container.className = "col-sm-12 col-md-12 col-lg-4 text-center";

        row_container.appendChild(container);
        row_container.appendChild(side_container);

        main.appendChild(title_header);
        main.appendChild(address_header);
        main.appendChild(department_header);
        main.appendChild(row_container);

        // var container = document.getElementById("subcontainer");

        for (var i = 0; i < data.length; i++) {
            if (Object.keys(data[i]).length === 0)
                continue

            if (i == 0) {
                // Title
                var title = document.getElementById("ad_title");
                title.innerHTML = data[i].SEARCH_TYPE == "For sale" ? "Achat - " : "Location - ";
                title.innerHTML += data[i].ANNOUNCE_TITLE;

                // Address
                var address = document.getElementById("ad_address");
                address.innerHTML = data[i].ADDRESS;

                // Department
                var department = document.getElementById("ad_dp");
                department.innerHTML = "-- " + data[i].CITY + "(" + getPostalCode(data[i].CITY) + ") --";
                // Carousel
                var carousel = createNode("div");
                carousel.id = "myCarousel";
                carousel.className = "carousel slide";
                carousel.setAttribute("data-ride", "carousel");

                var carousel_list = createNode("ol");
                carousel_list.className = "carousel-indicators";

                var images = data[i].IMAGE.replaceAll('\'', '\"');
                images = images.replaceAll('u\"', '\"');
                images = JSON.parse(images);

                var counter = 1;

                for (image in images) {
                    var list_item = createNode("li")
                    list_item.setAttribute("data-target", "#myCarousel");
                    list_item.setAttribute("data-slide", counter.toString());
                    carousel_list.appendChild(list_item);
                    counter += 1;
                }

                carousel.appendChild(carousel_list);

                var carousel_content = createNode("div");
                carousel_content.className = "carousel-inner";
                counter = 1;

                for (image in images) {
                    var carousel_item = createNode("div");
                    carousel_item.className = counter == 1 ? "carousel-item active" : "carousel-item";

                    var carousel_item_image = createNode("img");
                    carousel_item_image.className = "slide-image h-75 rounded";
                    carousel_item_image.src = images[image];
                    carousel_item.appendChild(carousel_item_image);

                    var carousel_item_positioning = createNode("div");
                    carousel_item_positioning.className = "h-100 w-100";
                    carousel_item_positioning.style = "background-color: #14131382  !important;position: relative;"
                    carousel_item.appendChild(carousel_item_positioning);
                    counter += 1

                    carousel_content.appendChild(carousel_item);
                }

                carousel.appendChild(carousel_content);

                // Previous carousel button
                var carousel_prev_button = createNode("a");
                carousel_prev_button.className = "carousel-control-prev";
                carousel_prev_button.href = "#myCarousel";
                carousel_prev_button.setAttribute("role", "button");
                carousel_prev_button.setAttribute("data-slide", "prev");

                var carousel_prev_button_content1 = createNode("span");
                carousel_prev_button_content1.className = "carousel-control-prev-icon";
                carousel_prev_button_content1.setAttribute("aria-hidden", "true");

                var carousel_prev_button_content2 = createNode("span");
                carousel_prev_button_content2.className = "sr-only";
                carousel_prev_button_content2.innerHTML = "Previous";

                carousel_prev_button.appendChild(carousel_prev_button_content1);
                carousel_prev_button.appendChild(carousel_prev_button_content2);

                // Next carousel button
                var carousel_next_button = createNode("a");
                carousel_next_button.className = "carousel-control-next";
                carousel_next_button.href = "#myCarousel";
                carousel_next_button.setAttribute("role", "button");
                carousel_next_button.setAttribute("data-slide", "next");

                var carousel_next_button_content1 = createNode("span");
                carousel_next_button_content1.className = "carousel-control-next-icon";
                carousel_next_button_content1.setAttribute("aria-hidden", "true");

                var carousel_next_button_content2 = createNode("span");
                carousel_next_button_content2.className = "sr-only";
                carousel_next_button_content2.innerHTML = "Next";

                carousel_next_button.appendChild(carousel_next_button_content1);
                carousel_next_button.appendChild(carousel_next_button_content2);

                carousel.appendChild(carousel_prev_button);
                carousel.appendChild(carousel_next_button);

                container.appendChild(carousel);

                // Publication date
                var months = ["Janvier", "Fevrier", "Mars", "Avril", "Mai", "Juin", "Juillet", "Août", "Septembre", "Octobre", "Novembre", "Décembre"];
                var days = ["Dimanche", "Lundi", "Mardi", "Mercredi", "Jeudi", "Vendredi", "Samedi"];

                var publication_date = createNode("div");
                publication_date.className = "text-muted";

                var publication_content = createNode("span");
                publication_content.style = "font-size: 12px;";
                publication_content.innerHTML = "Publié le ";
                var date = new Date(data[i].TIMESTAMP);
                publication_content.innerHTML += days[date.getUTCDay()] + ", " + date.getUTCDate() + " " + months[date.getUTCMonth()] + " " + date.getUTCFullYear();
                publication_content.innerHTML += " à ";
                publication_content.innerHTML += date.getUTCHours() + ":" + date.getUTCMinutes();
                publication_content.innerHTML += " sur <strong>Surfyn</strong>"

                publication_date.appendChild(publication_content);

                container.appendChild(publication_date);

                // Description
                var description_container = createNode("div");
                description_container.className = "w-100 mt-4";

                var description_title = createNode("h2");
                description_title.style = "color: #3a838a";
                description_title.innerHTML = "DESCRIPTION DU BIEN";

                description_container.appendChild(description_title);

                var description_content = createNode("p");
                description_content.className = "text-justify";
                description_content.innerHTML = data[i].AD_TEXT_DESCRIPTION;

                description_container.appendChild(description_content);

                container.appendChild(description_container);

                // Caracteristics
                var details_container = createNode("div");
                details_container.className = "w-100 mt-5";

                var details_title = createNode("h2");
                details_title.style = "color: #3a838a";
                details_title.innerHTML = "CARACTERISTIQUES";

                details_container.appendChild(details_title);

                var details_info_container = createNode("div");
                details_info_container.className = "row p-2";
                details_info_container.style = "color: #555;";

                // Surface
                var details_surface = createNode("div");
                details_surface.className = "col-sm-12 col-md-4 sf_ad_details";

                var details_surface_inner = createNode("div");
                details_surface_inner.className = "d-flex align-items-center";

                var svg_surface_icon = document.createElementNS('http://www.w3.org/2000/svg', 'svg');
                svg_surface_icon.setAttribute("viewBox", "0 0 24 24");
                svg_surface_icon.setAttribute("fill", "#3a838a");

                var path_surface_icon = document.createElementNS("http://www.w3.org/2000/svg", "path");
                path_surface_icon.setAttribute("d", "M20 2H4c-1.1 0-2 .9-2 2v16a2 2 0 002 2h16c1.11 0 2-.89 2-2V4a2 2 0 00-2-2M4 6l2-2h4.9L4 10.9V6m0 7.7L13.7 4h4.9L4 18.6v-4.9M20 18l-2 2h-4.9l6.9-6.9V18m0-7.7L10.3 20H5.4L20 5.4v4.9z");

                svg_surface_icon.appendChild(path_surface_icon);

                var surface_content = createNode("span");
                surface_content.className = "ml-2";
                surface_content.innerHTML = "Surface: ";
                surface_content.innerHTML += data[i].SURFACE;
                surface_content.innerHTML += " m²";

                details_surface_inner.appendChild(svg_surface_icon)
                details_surface_inner.appendChild(surface_content);

                details_surface.appendChild(details_surface_inner);
                details_info_container.appendChild(details_surface);

                // Bedrooms
                var details_bedrooms = createNode("div");
                details_bedrooms.className = "col-sm-12 col-md-4 sf_ad_details";

                var details_bedrooms_inner = createNode("div");
                details_bedrooms_inner.className = "d-flex align-items-center";

                var svg_bedrooms_icon = document.createElementNS('http://www.w3.org/2000/svg', 'svg');
                svg_bedrooms_icon.setAttribute("viewBox", "0 0 24 24");
                svg_bedrooms_icon.setAttribute("fill", "#3a838a");

                var path_bedrooms_icon = document.createElementNS('http://www.w3.org/2000/svg', 'path');
                path_bedrooms_icon.setAttribute("d", "M19 7h-8v7H3V5H1v15h2v-3h18v3h2v-9a4 4 0 00-4-4M7 13a3 3 0 003-3 3 3 0 00-3-3 3 3 0 00-3 3 3 3 0 003 3z");

                svg_bedrooms_icon.appendChild(path_bedrooms_icon);

                var bedrooms_content = createNode("span");
                bedrooms_content.className = "ml-2";
                bedrooms_content.innerHTML = "Chambre(s): ";
                bedrooms_content.innerHTML += data[i].BEDROOMS;

                details_bedrooms_inner.appendChild(svg_bedrooms_icon)
                details_bedrooms_inner.appendChild(bedrooms_content);

                details_bedrooms.appendChild(details_bedrooms_inner);
                details_info_container.appendChild(details_bedrooms);

                // Cellar
                if (data[i].hasOwnProperty("CELLAR")) {
                    var details_cellar = createNode("div");
                    details_cellar.className = "col-sm-12 col-md-4 sf_ad_details";

                    var details_cellar_inner = createNode("div");
                    details_cellar_inner.className = "d-flex align-items-center";

                    var svg_cellar_icon = document.createElementNS('http://www.w3.org/2000/svg', 'svg');
                    svg_cellar_icon.setAttribute("viewBox", "0 0 24 24");
                    svg_cellar_icon.setAttribute("fill", "#3a838a");

                    var path_cellar_icon = document.createElementNS('http://www.w3.org/2000/svg', 'path');
                    path_cellar_icon.setAttribute("d", "M19 20h-2v-9H7v9H5V9l7-4 7 4v11M8 12h8v2H8v-2m0 3h8v2H8v-2m8 3v2H8v-2h8z");

                    svg_cellar_icon.appendChild(path_cellar_icon);

                    var cellar_content = createNode("span");
                    cellar_content.className = "ml-2";
                    cellar_content.innerHTML = "Cave(s): ";
                    cellar_content.innerHTML += data[i].CELLAR;

                    details_cellar_inner.appendChild(svg_cellar_icon)
                    details_cellar_inner.appendChild(cellar_content);

                    details_cellar.appendChild(details_cellar_inner);
                    details_info_container.appendChild(details_cellar);
                }

                // Type of heating
                var details_heating = createNode("div");
                details_heating.className = "col-sm-12 col-md-4 sf_ad_details";

                var details_heating_inner = createNode("div");
                details_heating_inner.className = "d-flex align-items-center";

                var svg_heating_icon = document.createElementNS('http://www.w3.org/2000/svg', 'svg');
                svg_heating_icon.setAttribute("viewBox", "0 0 24 24");
                svg_heating_icon.setAttribute("fill", "#3a838a");

                var path_heating_icon = document.createElementNS('http://www.w3.org/2000/svg', 'path');
                path_heating_icon.setAttribute("d", "M8.58 14c-.28-.45-.47-.97-.52-1.5h7.88c-.05.53-.24 1.05-.47 1.5H8.58M12 16c-1.03 0-1.92-.39-2.62-1h5.25c-.71.61-1.6 1-2.63 1m0-8c1.03 0 1.92.39 2.63 1H9.38c.7-.61 1.59-1 2.62-1m-3.42 2h6.84c.28.45.47.97.52 1.5H8.06c.05-.53.24-1.05.52-1.5M3 3v18h18V3H3m9 15c-3.33 0-6-2.67-6-6s2.67-6 6-6 6 2.67 6 6-2.67 6-6 6z");

                svg_heating_icon.appendChild(path_heating_icon);

                var heating_content = createNode("span");
                heating_content.className = "ml-2";
                heating_content.innerHTML = data[i].TYPE_OF_HEATING;

                details_heating_inner.appendChild(svg_heating_icon);
                details_heating_inner.appendChild(heating_content);

                details_heating.appendChild(details_heating_inner);
                details_info_container.appendChild(details_heating);

                details_container.appendChild(details_info_container);

                container.appendChild(details_container);

                // Another caracteristics
                var others_details_container = createNode("div");
                others_details_container.className = "w-100 mt-5";

                var others_details_title = createNode("h2");
                others_details_title.style = "color: #3a838a";
                others_details_title.innerHTML = "AUTRES INFORMATIONS";

                others_details_container.appendChild(others_details_title);

                var others_details_info_container = createNode("div");
                others_details_info_container.className = "row p-2";
                others_details_info_container.style = "color: #555;";

                // Land Surface
                if (data[i].hasOwnProperty("LAND_SURFACE")) {
                    var details_land = createNode("div");
                    details_land.className = "col-sm-12 col-md-4 sf_ad_details";

                    var details_land_inner = createNode("div");
                    details_land_inner.className = "d-flex align-items-center";

                    var svg_land_icon = document.createElementNS('http://www.w3.org/2000/svg', 'svg');
                    svg_land_icon.setAttribute("viewBox", "0 0 24 24");
                    svg_land_icon.setAttribute("fill", "#3a838a");

                    var path_land_icon = document.createElementNS('http://www.w3.org/2000/svg', 'path');
                    path_land_icon.setAttribute("d", "M20 2H4c-1.1 0-2 .9-2 2v16a2 2 0 002 2h16c1.11 0 2-.89 2-2V4a2 2 0 00-2-2M4 6l2-2h4.9L4 10.9V6m0 7.7L13.7 4h4.9L4 18.6v-4.9M20 18l-2 2h-4.9l6.9-6.9V18m0-7.7L10.3 20H5.4L20 5.4v4.9z");

                    svg_land_icon.appendChild(path_land_icon);

                    var land_content = createNode("span");
                    land_content.className = "ml-2";
                    land_content.innerHTML = "Terrain: ";
                    land_content.innerHTML += data[i].LAND_SURFACE;
                    land_content.innerHTML += " m²";

                    details_land_inner.appendChild(svg_land_icon)
                    details_land_inner.appendChild(land_content);

                    details_land.appendChild(details_land_inner);
                    others_details_info_container.appendChild(details_land);
                }

                // Floor
                if (data[i].hasOwnProperty("FLOOR")) {

                    var details_floor = createNode("div");
                    details_floor.className = "col-sm-12 col-md-4 sf_ad_details";

                    var details_floor_inner = createNode("div");
                    details_floor_inner.className = "d-flex align-items-center";

                    var svg_floor_icon = document.createElementNS('http://www.w3.org/2000/svg', 'svg');
                    svg_floor_icon.setAttribute("viewBox", "0 0 24 24");
                    svg_floor_icon.setAttribute("fill", "#3a838a");

                    var path_floor_icon = document.createElementNS('http://www.w3.org/2000/svg', 'path');
                    path_floor_icon.setAttribute("d", "M12 3L2 12h3v8h14v-8h3L12 3M9 8h2v8h4v2H9V8z");

                    svg_floor_icon.appendChild(path_floor_icon);

                    var floor_content = createNode("span");
                    floor_content.className = "ml-2";
                    floor_content.innerHTML = "Etage: ";
                    floor_content.innerHTML += data[i].FLOOR;
                    floor_content.innerHTML += data[i].FLOOR == "1" ? "er" : "e";

                    details_floor_inner.appendChild(svg_floor_icon)
                    details_floor_inner.appendChild(floor_content);

                    details_floor.appendChild(details_floor_inner);
                    others_details_info_container.appendChild(details_floor);
                }

                // Construction year
                if (data[i].hasOwnProperty("CONSTRUCTION_YEAR")) {
                    var details_const = createNode("div");
                    details_const.className = "col-sm-12 col-md-4 sf_ad_details";

                    var details_const_inner = createNode("div");
                    details_const_inner.className = "d-flex align-items-center";

                    var svg_const_icon = document.createElementNS('http://www.w3.org/2000/svg', 'svg');
                    svg_const_icon.setAttribute("viewBox", "0 0 24 24");
                    svg_const_icon.setAttribute("fill", "#3a838a");

                    var path_const_icon = document.createElementNS('http://www.w3.org/2000/svg', 'path');
                    path_const_icon.setAttribute("d", "M6 1v2H5a2 2 0 00-2 2v14a2 2 0 002 2h14a2 2 0 002-2V5a2 2 0 00-2-2h-1V1h-2v2H8V1H6M5 8h14v11H5V8m7.19 1c-.87 0-1.57.2-2.11.59-.52.41-.78.98-.77 1.77l.01.03h1.93c.01-.3.1-.53.28-.69a1 1 0 01.66-.23c.31 0 .57.1.75.28.18.19.26.45.26.75 0 .32-.07.59-.23.82-.14.23-.35.43-.61.59-.51.34-.86.64-1.05.91-.2.26-.31.68-.31 1.18h2c0-.31.04-.56.13-.74.09-.18.26-.36.51-.52.45-.24.82-.53 1.11-.93.29-.4.44-.81.44-1.31 0-.76-.27-1.37-.81-1.82-.53-.45-1.26-.68-2.19-.68M11 16v2h2v-2h-2z");

                    svg_const_icon.appendChild(path_const_icon);

                    var const_content = createNode("span");
                    const_content.className = "ml-2";
                    const_content.innerHTML = "Construction: ";
                    const_content.innerHTML += data[i].CONSTRUCTION_YEAR;

                    details_const_inner.appendChild(svg_const_icon)
                    details_const_inner.appendChild(const_content);

                    details_const.appendChild(details_const_inner);
                    others_details_info_container.appendChild(details_const);
                }

                // Parking
                if (data[i].PARKING != "Non") {
                    var details_parking = createNode("div");
                    details_parking.className = "col-sm-12 col-md-4 sf_ad_details";

                    var details_parking_inner = createNode("div");
                    details_parking_inner.className = "d-flex align-items-center";

                    var svg_parking_icon = document.createElementNS('http://www.w3.org/2000/svg', 'svg');
                    svg_parking_icon.setAttribute("viewBox", "0 0 24 24");
                    svg_parking_icon.setAttribute("fill", "#3a838a");

                    var path_parking_icon = document.createElementNS('http://www.w3.org/2000/svg', 'path');
                    path_parking_icon.setAttribute("d", "M13.2 11H10V7h3.2a2 2 0 012 2 2 2 0 01-2 2M13 3H6v18h4v-6h3a6 6 0 006-6c0-3.32-2.69-6-6-6z");

                    svg_parking_icon.appendChild(path_parking_icon);

                    var parking_content = createNode("span");
                    parking_content.className = "ml-2";
                    parking_content.innerHTML = "Parking: ";
                    parking_content.innerHTML += data[i].PARKING;

                    details_parking_inner.appendChild(svg_parking_icon)
                    details_parking_inner.appendChild(parking_content);

                    details_parking.appendChild(details_parking_inner);
                    others_details_info_container.appendChild(details_parking);
                }

                // Lift
                if (data[i].LIFT != "Non") {
                    var details_lift = createNode("div");
                    details_lift.className = "col-sm-12 col-md-4 sf_ad_details";

                    var details_lift_inner = createNode("div");
                    details_lift_inner.className = "d-flex align-items-center";

                    var svg_lift_icon = document.createElementNS('http://www.w3.org/2000/svg', 'svg');
                    svg_lift_icon.setAttribute("viewBox", "0 0 24 24");
                    svg_lift_icon.setAttribute("fill", "#3a838a");

                    var path_lift_icon = document.createElementNS('http://www.w3.org/2000/svg', 'path');
                    path_lift_icon.setAttribute("d", "M19 3H5C3.9 3 3 3.9 3 5V19C3 20.1 3.9 21 5 21H19C20.1 21 21 20.1 21 19V5C21 3.9 20.1 3 19 3M8 5C8.8 5 9.5 5.7 9.5 6.5S8.8 8 8 8 6.5 7.3 6.5 6.5 7.2 5 8 5M11 14H9.5V19H6.5V14H5V11C5 9.9 5.9 9 7 9H9C10.1 9 11 9.9 11 11V14M15.9 19L13 14.2H18.8L15.9 19M13 10L16 5L19 10H13Z");

                    svg_lift_icon.appendChild(path_lift_icon);

                    var lift_content = createNode("span");
                    lift_content.className = "ml-2";
                    lift_content.innerHTML = "Ascenseur: ";
                    lift_content.innerHTML += data[i].LIFT;

                    details_lift_inner.appendChild(svg_lift_icon)
                    details_lift_inner.appendChild(lift_content);

                    details_lift.appendChild(details_lift_inner);
                    others_details_info_container.appendChild(details_lift);
                }

                // Balcony
                if (data[i].BALCONY != "Non") {
                    var details_balcony = createNode("div");
                    details_balcony.className = "col-sm-12 col-md-4 sf_ad_details";

                    var details_balcony_inner = createNode("div");
                    details_balcony_inner.className = "d-flex align-items-center";

                    var svg_balcony_icon = document.createElementNS('http://www.w3.org/2000/svg', 'svg');
                    svg_balcony_icon.setAttribute("viewBox", "0 0 24 24");
                    svg_balcony_icon.setAttribute("fill", "#3a838a");

                    var path_balcony_icon = document.createElementNS('http://www.w3.org/2000/svg', 'path');
                    path_balcony_icon.setAttribute("d", "M19 3H5C3.9 3 3 3.9 3 5V19C3 20.1 3.9 21 5 21H19C20.1 21 21 20.1 21 19V5C21 3.9 20.1 3 19 3M8 5C8.8 5 9.5 5.7 9.5 6.5S8.8 8 8 8 6.5 7.3 6.5 6.5 7.2 5 8 5M11 14H9.5V19H6.5V14H5V11C5 9.9 5.9 9 7 9H9C10.1 9 11 9.9 11 11V14M15.9 19L13 14.2H18.8L15.9 19M13 10L16 5L19 10H13Z");

                    svg_balcony_icon.appendChild(path_balcony_icon);

                    var balcony_content = createNode("span");
                    balcony_content.className = "ml-2";
                    balcony_content.innerHTML = "Balcon: ";
                    balcony_content.innerHTML += data[i].BALCONY;

                    details_balcony_inner.appendChild(svg_balcony_icon)
                    details_balcony_inner.appendChild(balcony_content);

                    details_balcony.appendChild(details_balcony_inner);
                    others_details_info_container.appendChild(details_balcony);
                }

                others_details_container.appendChild(others_details_info_container);
                container.appendChild(others_details_container);

                // Side menu
                // var side_container = document.getElementById("side_container");

                var details_card = createNode("div");
                details_card.className = "card price-card pt-3 mb-3";

                // Price
                var card_price = createNode("h3");
                card_price.style = "color: #3a838a";
                card_price.innerHTML = formatPrice(String(data[i].PRICE)) + " €";

                details_card.appendChild(card_price);

                // Rooms and Surface
                var card_surface_rooms = createNode("div");
                card_surface_rooms.className = "mt-2 mb-2 d-flex border-bottom border-top justify-content-around align-items-center p-2";

                var surface_content = createNode("h5");
                surface_content.innerHTML = data[i].SURFACE + " m²";

                var rooms_content = createNode("h5");
                rooms_content.innerHTML = data[i].ROOMS + " pièce(s)";

                card_surface_rooms.appendChild(surface_content);
                card_surface_rooms.appendChild(rooms_content);

                details_card.appendChild(card_surface_rooms);

                // Icons
                var icons_container = createNode("div");
                icons_container.className = "d-flex justify-content-between p-3 border-bottom";

                if (data[i].BALCONY != "Non") {
                    var balcony_icon = createNode("img");
                    balcony_icon.className = "icon_detail rounded";
                    balcony_icon.src = "balcony.svg";

                    icons_container.appendChild(balcony_icon);
                }

                if (data[i].LIFT != "Non") {
                    var lift_icon = createNode("img");
                    lift_icon.className = "icon_detail rounded";
                    lift_icon.src = "elevator.svg";

                    icons_container.appendChild(lift_icon);
                }

                if (data[i].PARKING != "Non") {
                    var parking_icon = createNode("img");
                    parking_icon.className = "icon_detail rounded"
                    parking_icon.src = "parking.svg";

                    icons_container.appendChild(parking_icon);
                }

                if (data[i].BALCONY != "Non" || data[i].LIFT != "Non" || data[i].PARKING != "Non") {
                    details_card.appendChild(icons_container);
                }

                // Share icons
                var share_container = createNode("div");
                share_container.className = "d-flex justify-content-between p-3";

                // Facebook
                var facebook_link = createNode("a");
                facebook_link.target = "_blank";
                facebook_link.href = "http://www.facebook.com/share.php?u=" + data[i].ANNOUNCE_LINK;

                var facebook_icon = document.createElementNS('http://www.w3.org/2000/svg', 'svg');
                facebook_icon.setAttribute("viewBox", "0 0 24 24");
                facebook_icon.setAttribute("fill", "#3a838a");

                var facebook_path = document.createElementNS('http://www.w3.org/2000/svg', 'path');
                facebook_path.setAttribute("d", "M12 2.04c-5.5 0-10 4.49-10 10.02 0 5 3.66 9.15 8.44 9.9v-7H7.9v-2.9h2.54V9.85c0-2.51 1.49-3.89 3.78-3.89 1.09 0 2.23.19 2.23.19v2.47h-1.26c-1.24 0-1.63.77-1.63 1.56v1.88h2.78l-.45 2.9h-2.33v7a10 10 0 008.44-9.9c0-5.53-4.5-10.02-10-10.02z");

                facebook_icon.appendChild(facebook_path);
                facebook_link.appendChild(facebook_icon);

                share_container.appendChild(facebook_link);

                // Twitter
                var twitter_link = createNode("a");
                twitter_link.target = "_blank";
                twitter_link.href = "http://twitter.com/share?text=" + pagetitle + "&url=" + data[i].ANNOUNCE_LINK;

                var twitter_icon = document.createElementNS('http://www.w3.org/2000/svg', 'svg');
                twitter_icon.setAttribute("viewBox", "0 0 24 24");
                twitter_icon.setAttribute("fill", "#3a838a");

                var twitter_path = document.createElementNS('http://www.w3.org/2000/svg', 'path');
                twitter_path.setAttribute("d", "M22.46 6c-.77.35-1.6.58-2.46.69.88-.53 1.56-1.37 1.88-2.38-.83.5-1.75.85-2.72 1.05C18.37 4.5 17.26 4 16 4c-2.35 0-4.27 1.92-4.27 4.29 0 .34.04.67.11.98C8.28 9.09 5.11 7.38 3 4.79c-.37.63-.58 1.37-.58 2.15 0 1.49.75 2.81 1.91 3.56-.71 0-1.37-.2-1.95-.5v.03c0 2.08 1.48 3.82 3.44 4.21a4.22 4.22 0 01-1.93.07 4.28 4.28 0 004 2.98 8.521 8.521 0 01-5.33 1.84c-.34 0-.68-.02-1.02-.06C3.44 20.29 5.7 21 8.12 21 16 21 20.33 14.46 20.33 8.79c0-.19 0-.37-.01-.56.84-.6 1.56-1.36 2.14-2.23z");

                twitter_icon.appendChild(twitter_path);
                twitter_link.appendChild(twitter_icon);

                share_container.appendChild(twitter_link);

                // LinkedIn
                var linkedin_link = createNode("a");
                linkedin_link.target = "_blank";
                linkedin_link.href = "https://www.linkedin.com/shareArticle?mini=true&url=" + data[i].ANNOUNCE_LINK;

                var linkedin_icon = document.createElementNS('http://www.w3.org/2000/svg', 'svg');
                linkedin_icon.setAttribute("viewBox", "0 0 24 24");
                linkedin_icon.setAttribute("fill", "#3a838a");

                var linkedin_path = document.createElementNS('http://www.w3.org/2000/svg', 'path');
                linkedin_path.setAttribute("d", "M19 3a2 2 0 012 2v14a2 2 0 01-2 2H5a2 2 0 01-2-2V5a2 2 0 012-2h14m-.5 15.5v-5.3a3.26 3.26 0 00-3.26-3.26c-.85 0-1.84.52-2.32 1.3v-1.11h-2.79v8.37h2.79v-4.93c0-.77.62-1.4 1.39-1.4a1.4 1.4 0 011.4 1.4v4.93h2.79M6.88 8.56a1.68 1.68 0 001.68-1.68c0-.93-.75-1.69-1.68-1.69a1.69 1.69 0 00-1.69 1.69c0 .93.76 1.68 1.69 1.68m1.39 9.94v-8.37H5.5v8.37h2.77z");

                linkedin_icon.appendChild(linkedin_path);
                linkedin_link.appendChild(linkedin_icon);

                share_container.appendChild(linkedin_link);

                // Mail
                var mail_link = createNode("a");
                mail_link.target = "_blank";
                mail_link.href = "mailto:?subject= Je veux partager avec toi ce bien immobilier&amp;body=" + pagetitle + "\nClique sur le lien: " + data[i].ANNOUNCE_LINK;

                var mail_icon = document.createElementNS('http://www.w3.org/2000/svg', 'svg');
                mail_icon.setAttribute("viewBox", "0 0 24 24");
                mail_icon.setAttribute("fill", "#3a838a");

                var mail_path = document.createElementNS('http://www.w3.org/2000/svg', 'path');
                mail_path.setAttribute("d", "M20 4H4c-1.11 0-2 .89-2 2v12a2 2 0 002 2h16a2 2 0 002-2V6a2 2 0 00-2-2m-3 13H7v-2h10m0-2H7v-2h10m3-2h-3V6h3");

                mail_icon.appendChild(mail_path);
                mail_link.appendChild(mail_icon);

                share_container.appendChild(mail_link);

                details_card.appendChild(share_container);
                side_container.appendChild(details_card);

                // Patners title
                // var partners_title = createNode("h4");
                // partners_title.style = "color: #3a838a";
                // partners_title.innerHTML = "NOS PARTENAIRES";

                // side_container.appendChild(partners_title);

                // var partners_container = createNode("div");
                // partners_container.className = "d-flex flex-column justify-content-center align-items-center";

                // var partner1 = createNode("img");
                // partner1.className = "mb-2 rounded";
                // partner1.style = "height: 100px;width: 100px;";
                // partner1.src = "partner1.jpg";

                // var partner2 = createNode("img");
                // partner2.className = "mb-2 rounded";
                // partner2.style = "height: 100px;width: 100px;";
                // partner2.src = "partner2.jpg";

                // var partner3 = createNode("img");
                // partner3.className = "mb-2 rounded";
                // partner3.style = "height: 100px;width: 100px;";
                // partner3.src = "partner1.jpg";

                // partners_container.appendChild(partner1);
                // partners_container.appendChild(partner2);
                // partners_container.appendChild(partner3);

                // side_container.appendChild(partners_container);
            }
            else {
                // Similars Announces
                similar_container = createNode("div");
                similar_container.className = "w-100 mt-5";

                similar_title = createNode("h2");
                similar_title.style = "color: #3a838a";
                similar_title.innerHTML = (data.length-1) + " annonces identiques publiées sur d'autres plateformes";

                similar_container.appendChild(similar_title);

                container.appendChild(similar_container);
                data.shift();
                generate_details_page(data, true);
            }

        }

        var header_content = document.getElementById("header-content");

        var forPuppeteer = document.createElement("div");
        forPuppeteer.setAttribute("id", "prerendered-page");
        forPuppeteer.style.visibility = 'hidden';
        header_content.appendChild(forPuppeteer);

    }
}
