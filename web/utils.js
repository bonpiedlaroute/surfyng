function reverse(s) {
  return s.split("").reverse().join("");
}

function formatPrice(p) {
  var val = reverse(p);

  var tmp = val.replace(/(.{3})/g, "$1 ");

  return reverse(tmp);
}

function draw_map(lat, lon) {
  // Get the map element
  // map_element = document.getElementById('map');

  // Map initialization
  var map = L.map('map').setView([lat, lon], 16);

  // Load tiles
  L.tileLayer('https://{s}.tile.openstreetmap.fr/osmfr/{z}/{x}/{y}.png', {
    attribution: 'Map data &copy; <a href="/osm.org/copyright">OpenStreetMap</a>/ODbL - rendu <a href="//openstreetmap.fr">OSM France</a>',
    minZoom: 15,
    maxZoom: 20
  }).addTo(map);

  // Adding a marker and set up a popup
  // var marker = L.marker([lat, lon]).addTo(map);
  // marker.bindPopup("<b>Paris<b>");


  // Adding a circle
  var circle = L.circle([lat, lon], 150, {
    color: '#32d3d1',
    fillColor: '#b3efef',
    fillOpacity: 0.2,
  }).addTo(map);

  svg_circle = circle._path.viewportElement;
  svg_circle.setAttribute('style', svg_circle.getAttribute('style') + ' height: ' + svg_circle.getAttribute('height') + 'px;');
}

function getLocation(address, callback)
{
  var url = "https://nominatim.openstreetmap.org/search?format=json&limit=1&q=" + address;
  
  var loc = fetch(url)
    .then(function (resp) { return resp.json(); })
    .then(function (data) {
      if (data[0].hasOwnProperty('lat') && data[0].hasOwnProperty('lon')) {
        return 'lat=' + data[0].lat + ';lon=' + data[0].lon;
      }
      else {
        return false;
      }
    })
    .catch(function (error) {
      console.log(error);
      return false;
    });
  
    return loc;
}